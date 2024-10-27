import os
import shutil
import argparse
from datetime import datetime
import subprocess
import logging
from logging.handlers import RotatingFileHandler
import socket
import zipfile
import json

# Logging configuration
def setup_logging():
    log_directory = os.path.dirname(os.path.abspath(__file__))
    log_file = os.path.join(log_directory, 'backup_tool.log')
    handler = RotatingFileHandler(log_file, maxBytes=5*1024*1024, backupCount=3)  # 5MB log files with rotation (3 backups)
    logging.basicConfig(level=logging.INFO, handlers=[handler], format='%(asctime)s - %(levelname)s - %(message)s')

# Load and save configuration
def load_config():
    config_path = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'backup_config.json')
    if not os.path.exists(config_path):
        default_config = {
            "nfs_server": "10.0.30.20",
            "nfs_share": "/mnt/MainStorage/backups/rpi",
            "mount_point": "nfs_backup",
            "source_directory": "/home/pi",
            "backup_retention": 14
        }
        with open(config_path, 'w') as config_file:
            json.dump(default_config, config_file, indent=4)
    with open(config_path, 'r') as config_file:
        return json.load(config_file)

def save_config(config):
    config_path = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'backup_config.json')
    with open(config_path, 'w') as config_file:
        json.dump(config, config_file, indent=4)


def mount_nfs_share(nfs_server, nfs_share, mount_point):
    try:
        if not os.path.ismount(mount_point):
            os.makedirs(mount_point, exist_ok=True)
            subprocess.run(["sudo", "mount", "-o", "rw,nfsvers=4", f"{nfs_server}:{nfs_share}", mount_point], check=True)
            subprocess.run(["sudo", "chmod", "777", mount_point], check=True)
            logging.info(f"NFS share {nfs_server}:{nfs_share} successfully mounted at {mount_point}")
    except subprocess.CalledProcessError as e:
        logging.error(f"Error mounting NFS share: {e}")


def backup(config):
    try:
        source_directory = config['source_directory']
        if not os.path.exists(source_directory):
            logging.error(f"Error: Source directory {source_directory} does not exist.")
            return
        
        hostname = socket.gethostname()
        host_backup_directory = os.path.join(config['mount_point'], 'rpi', hostname)
        if not os.path.exists(host_backup_directory):
            os.makedirs(host_backup_directory)
        
        date_time = datetime.now().strftime('%Y-%m-%d_%H-%M-%S')
        backup_file = os.path.join(host_backup_directory, f'backup_{date_time}.zip')
        
        # Create a compressed archive of the source directory
        with zipfile.ZipFile(backup_file, 'w', zipfile.ZIP_DEFLATED) as backup_zip:
            for root, dirs, files in os.walk(source_directory):
                for file in files:
                    file_path = os.path.join(root, file)
                    backup_zip.write(file_path, os.path.relpath(file_path, source_directory))
        
        logging.info(f"Backup successfully created: {backup_file}")
        manage_old_backups(host_backup_directory, config['backup_retention'])
    except Exception as e:
        logging.error(f"Error creating backup: {e}")


def restore(config):
    try:
        hostname = socket.gethostname()
        host_backup_directory = os.path.join(config['mount_point'], hostname)
        backups = sorted([f for f in os.listdir(host_backup_directory) if f.startswith('backup_') and f.endswith('.zip')])
        if not backups:
            logging.error("No backups available for restoration.")
            return
        
        print("Available Backups:")
        for i, backup in enumerate(backups, 1):
            print(f"{i}: {backup}")
        print(f"{len(backups) + 1}: Back to main menu")
        
        backup_choice = input("Please choose the backup to restore (number): ")
        if backup_choice == str(len(backups) + 1):
            return
        try:
            backup_index = int(backup_choice) - 1
            if 0 <= backup_index < len(backups):
                restore_file = os.path.join(host_backup_directory, backups[backup_index])
                restore_target = os.path.expanduser('~/restored_data')
                if not os.path.exists(restore_target):
                    os.makedirs(restore_target)
                
                # Extract the archive to the target directory
                with zipfile.ZipFile(restore_file, 'r') as backup_zip:
                    backup_zip.extractall(restore_target)
                
                logging.info(f"Successfully restored from: {restore_file} to {restore_target}")
            else:
                print("Invalid selection.")
        except ValueError:
            print("Invalid input.")
    except Exception as e:
        logging.error(f"Error during restoration: {e}")


def manage_old_backups(directory, max_backups=14):
    try:
        backups = sorted([f for f in os.listdir(directory) if f.startswith('backup_') and f.endswith('.zip')])
        while len(backups) > max_backups:
            old_backup = backups.pop(0)
            os.remove(os.path.join(directory, old_backup))
            logging.info(f"Deleted old backup: {old_backup}")
    except Exception as e:
        logging.error(f"Error deleting old backups: {e}")


def configure(config):
    print("\nConfiguration Menu")
    print(f"1: NFS Server (current: {config['nfs_server']})")
    print(f"2: NFS Share (current: {config['nfs_share']})")
    print(f"3: Mount Point (current: {config['mount_point']})")
    print(f"4: Source Directory (current: {config['source_directory']})")
    print(f"5: Backup Retention in days (current: {config['backup_retention']})")
    print("6: Back to main menu")
    choice = input("Please choose an option (1-6): ")
    if choice == '1':
        config['nfs_server'] = input("New NFS Server: ")
    elif choice == '2':
        config['nfs_share'] = input("New NFS Share: ")
    elif choice == '3':
        config['mount_point'] = input("New Mount Point: ")
    elif choice == '4':
        config['source_directory'] = input("New Source Directory: ")
    elif choice == '5':
        try:
            config['backup_retention'] = int(input("New Backup Retention (number of backups): "))
        except ValueError:
            print("Invalid input. Please enter a number.")
    elif choice == '6':
        return
    else:
        print("Invalid selection.")
    save_config(config)


def display_menu(config):
    while True:
        print("\nBackup and Restore Tool")
        print("1: Create new backup")
        print("2: Restore backup")
        print("3: Edit configuration")
        print("4: Delete backup")
        print("5: Exit")
        choice = input("Please choose an option (1-5): ")
        if choice == '1':
            backup(config)
        elif choice == '2':
            restore(config)
        elif choice == '3':
            configure(config)
        elif choice == '4':
            delete_backup(config)
        elif choice == '5':
            print("Program exited.")
            break
        else:
            print("Invalid selection. Please choose an option between 1 and 5.")


def delete_backup(config):
    try:
        hostname = socket.gethostname()
        host_backup_directory = os.path.join(config['mount_point'], hostname)
        backups = sorted([f for f in os.listdir(host_backup_directory) if f.startswith('backup_') and f.endswith('.zip')])
        if not backups:
            print("No backups available to delete.")
            return
        
        print("Available Backups:")
        for i, backup in enumerate(backups, 1):
            print(f"{i}: {backup}")
        print(f"{len(backups) + 1}: Delete all backups")
        print(f"{len(backups) + 2}: Back to main menu")
        
        backup_choice = input("Please choose the backup to delete (number): ")
        if backup_choice == str(len(backups) + 2):
            return
        try:
            backup_index = int(backup_choice) - 1
            if 0 <= backup_index < len(backups):
                os.remove(os.path.join(host_backup_directory, backups[backup_index]))
                logging.info(f"Backup deleted: {backups[backup_index]}")
            elif backup_index == len(backups):
                for backup in backups:
                    os.remove(os.path.join(host_backup_directory, backup))
                    logging.info(f"Backup deleted: {backup}")
            else:
                print("Invalid selection.")
        except ValueError:
            print("Invalid input.")
    except Exception as e:
        logging.error(f"Error deleting backups: {e}")


def main():
    setup_logging()
    config = load_config()
    mount_nfs_share(config['nfs_server'], config['nfs_share'], config['mount_point'])
    display_menu(config)


if __name__ == "__main__":
    main()
