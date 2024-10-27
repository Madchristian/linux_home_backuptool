# Linux Home Backup Tool

This project provides a simple backup and restore tool designed to back up data from multiple Raspberry Pis to a central NFS server. It is built using Python and offers an easy-to-use command-line interface (CLI).

## Features

- **Backup as a compressed ZIP archive**: The script creates a compressed ZIP archive of the specified source directory.
- **NFS Support**: Data is backed up to a central server over NFS.
- **Backup Rotation**: The script retains a maximum of 14 backups per device, with older backups being automatically deleted.
- **Hostname-based Backup Directories**: Backups are stored in a directory that uses the hostname of each Raspberry Pi to easily distinguish backups from different devices.
- **Configuration File**: A configuration file (`backup_config.json`) allows users to specify parameters such as the NFS server, target path, source directory, and backup retention time. This file can be edited either manually or via the configuration menu.
- **User Menu for Interaction**: An interactive menu guides the user through creating new backups, restoring from existing backups, managing configuration, and deleting backups.

## Requirements

1. **Python 3**: This script is written in Python 3 and uses Python standard libraries.
2. **Virtual Environment**: It is recommended to run the script within a virtual environment.
3. **NFS Support**: Make sure `nfs-common` is installed on your Raspberry Pi.

### Install `nfs-common`

To install the required package, run:

```sh
sudo apt-get update
sudo apt-get install nfs-common
```

## Setup

1. **Clone the Repository**

   Clone the Git repository to your Raspberry Pi:

   ```sh
   git clone https://github.com/Madchristian/linux_home_backuptool.git
   cd linux_home_backuptool
   ```

2. **Create a Virtual Environment**

   Create and activate a virtual environment:

   ```sh
   python3 -m venv venv
   source venv/bin/activate
   ```

3. **Install Dependencies**

   Install the required packages from `requirements.txt`:

   ```sh
   pip install -r requirements.txt
   ```

## Usage

### Manually Run the Script

Activate the virtual environment and run the script:

```sh
source venv/bin/activate
python3 backup.py
```

The script presents you with a menu of the following options:

1. **Create New Backup**: Create a new backup of the specified directory.
2. **Restore Backup**: Choose an available backup and restore it.
3. **Edit Configuration**: Change settings such as NFS server, target directory, and retention.
4. **Delete Backup**: Delete a specific backup or all backups of a device.
5. **Exit**: Exit the program.

### Automatic Backups via Cron Job

To automate backups every day, set up a cron job:

```sh
crontab -e
```

Add the following line to execute the backup daily at 3 AM:

```sh
0 3 * * * cd /path/to/linux_home_backuptool && source venv/bin/activate && /usr/bin/python3 backup.py backup --source /home/pi > /path/to/linux_home_backuptool/cronjob.log 2>&1
```

Replace `/path/to/linux_home_backuptool` with the actual path where you cloned the repository.

## Detailed Features

### 1. NFS Mount
The script mounts an NFS share based on the settings in the configuration file (`backup_config.json`). The directory is mounted as `nfs_backup` within the current working directory. If the mount fails, an error is logged.

### 2. Backup Creation
The script creates a ZIP archive of the specified source directory. The archive is stored in a directory that uses the hostname of the device, along with a timestamp to ensure uniqueness. Older backups are automatically deleted, keeping only the number specified in the configuration.

### 3. Restoration
During restoration, the user can select a specific backup based on the timestamp. The backup is extracted to the `~/restored_data` directory.

### 4. Configuration Management
The menu allows the user to edit the program configuration without manually editing the configuration file. Parameters like NFS server, target directory, retention period, etc., can be adjusted easily.

### 5. Deleting Backups
Users can delete either a single backup or all backups of a particular device.

## Log Files
The script generates a log file (`backup_tool.log`) in the program directory that records all actions and errors. A maximum of 3 log files of size 5 MB each are retained.

## Notes
- **NFS Access**: The script uses `sudo` to mount the NFS share, so appropriate permissions are required.
- **Permission Management**: Ensure that the user running the script has enough permissions to access and write to the NFS share.

## Improvements
If you have ideas for improvements or encounter any issues, feel free to create issues on the GitHub repository or submit pull requests.

## Author
Created by Madchristian

Enjoy backing up your data! 😊