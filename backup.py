import os
import shutil
import argparse
from datetime import datetime
import subprocess
import logging
from logging.handlers import RotatingFileHandler
import socket
import zipfile

# Log-Konfiguration
def setup_logging():
    log_directory = os.path.dirname(os.path.abspath(__file__))
    log_file = os.path.join(log_directory, 'backup_tool.log')
    handler = RotatingFileHandler(log_file, maxBytes=5*1024*1024, backupCount=3)  # 5MB Log-Dateien mit Rotation (3 Backups)
    logging.basicConfig(level=logging.INFO, handlers=[handler], format='%(asctime)s - %(levelname)s - %(message)s')


def mount_nfs_share(nfs_server, nfs_share, mount_point):
    try:
        if not os.path.ismount(mount_point):
            os.makedirs(mount_point, exist_ok=True)
            subprocess.run(["sudo", "mount", "-o", "rw,nfsvers=4", f"{nfs_server}:{nfs_share}", mount_point], check=True)
            subprocess.run(["sudo", "chmod", "777", mount_point], check=True)
            logging.info(f"NFS Share {nfs_server}:{nfs_share} erfolgreich gemountet auf {mount_point}")
    except subprocess.CalledProcessError as e:
        logging.error(f"Fehler beim Mounten des NFS-Shares: {e}")


def backup(source_directory, target_directory):
    try:
        if not os.path.exists(source_directory):
            logging.error(f"Fehler: Das Quellverzeichnis {source_directory} existiert nicht.")
            return
        
        hostname = socket.gethostname()
        host_backup_directory = os.path.join(target_directory, hostname)
        if not os.path.exists(host_backup_directory):
            os.makedirs(host_backup_directory)
        
        date_time = datetime.now().strftime('%Y-%m-%d_%H-%M-%S')
        backup_file = os.path.join(host_backup_directory, f'backup_{date_time}.zip')
        
        # Erstelle ein komprimiertes Archiv des Quellverzeichnisses
        with zipfile.ZipFile(backup_file, 'w', zipfile.ZIP_DEFLATED) as backup_zip:
            for root, dirs, files in os.walk(source_directory):
                for file in files:
                    file_path = os.path.join(root, file)
                    backup_zip.write(file_path, os.path.relpath(file_path, source_directory))
        
        logging.info(f"Backup erfolgreich erstellt: {backup_file}")
        manage_old_backups(host_backup_directory)
    except Exception as e:
        logging.error(f"Fehler beim Erstellen des Backups: {e}")


def restore(target_directory, restore_date):
    try:
        hostname = socket.gethostname()
        host_backup_directory = os.path.join(target_directory, hostname)
        restore_file = os.path.join(host_backup_directory, f'backup_{restore_date}.zip')
        if not os.path.exists(restore_file):
            logging.error(f"Fehler: Kein Backup für das Datum {restore_date} gefunden.")
            return
        
        # Zielverzeichnis für die Wiederherstellung anpassen
        restore_target = os.path.expanduser('~/restored_data')
        if not os.path.exists(restore_target):
            os.makedirs(restore_target)
        
        # Entpacke das Archiv in das Zielverzeichnis
        with zipfile.ZipFile(restore_file, 'r') as backup_zip:
            backup_zip.extractall(restore_target)
        
        logging.info(f"Wiederherstellung erfolgreich aus: {restore_file} nach {restore_target}")
    except Exception as e:
        logging.error(f"Fehler bei der Wiederherstellung: {e}")


def manage_old_backups(directory, max_backups=14):
    try:
        backups = sorted([f for f in os.listdir(directory) if f.startswith('backup_') and f.endswith('.zip')])
        while len(backups) > max_backups:
            old_backup = backups.pop(0)
            os.remove(os.path.join(directory, old_backup))
            logging.info(f"Altes Backup gelöscht: {old_backup}")
    except Exception as e:
        logging.error(f"Fehler beim Löschen alter Backups: {e}")


def display_menu():
    print("\nBackup und Restore Tool")
    print("1: Erstelle neues Backup")
    print("2: Restore Backup")
    print("3: Beenden")
    choice = input("Bitte wähle eine Option (1-3): ")
    return choice


def list_backups(target_directory):
    hostname = socket.gethostname()
    host_backup_directory = os.path.join(target_directory, hostname)
    if not os.path.exists(host_backup_directory):
        print("Keine Backups gefunden.")
        return []
    backups = sorted([f for f in os.listdir(host_backup_directory) if f.startswith('backup_') and f.endswith('.zip')])
    for i, backup in enumerate(backups, 1):
        print(f"{i}: {backup}")
    return backups


def main():
    setup_logging()
    # Hardcodierte NFS-Server und Share-Informationen
    nfs_server = "10.0.30.20"
    nfs_share = "/mnt/MainStorage/backups/rpi"
    mount_point = os.path.join(os.path.dirname(os.path.abspath(__file__)), "nfs_backup")

    # NFS Share mounten
    mount_nfs_share(nfs_server, nfs_share, mount_point)

    while True:
        choice = display_menu()
        if choice == '1':
            source_directory = input("Bitte gib das Quellverzeichnis für das Backup an: ")
            backup(source_directory, mount_point)
        elif choice == '2':
            backups = list_backups(mount_point)
            if backups:
                backup_choice = input("Bitte wähle das Backup, das wiederhergestellt werden soll (Nummer): ")
                try:
                    backup_index = int(backup_choice) - 1
                    if 0 <= backup_index < len(backups):
                        restore_date = backups[backup_index].replace('backup_', '').replace('.zip', '')
                        restore(mount_point, restore_date)
                    else:
                        print("Ungültige Auswahl.")
                except ValueError:
                    print("Ungültige Eingabe.")
        elif choice == '3':
            print("Programm beendet.")
            break
        else:
            print("Ungültige Auswahl. Bitte wähle eine Option zwischen 1 und 3.")


if __name__ == "__main__":
    main()

# Cron-Job Setup:
# Füge diese Zeile mit 'crontab -e' hinzu, um das Backup jeden Tag um 3 Uhr Nachts auszuführen:
# 0 3 * * * /usr/bin/python3 /path/to/this/script.py backup --source <source_directory>
