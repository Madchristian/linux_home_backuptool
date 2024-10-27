# 🍓 Raspberry Pi Backup and Restore Tool 🔄💾

## 🌟 Overview

This tool 🛠️ allows users 👥 to easily create ✨, restore 🛠️, and manage 📂 backups 🔄 of their Raspberry Pi files 📁. It utilizes NFS 🌐 for network storage, zip 🗜️ compression for file archiving, and cron ⏰ for automated scheduled backups. Users can also modify ✏️ the backup configuration ⚙️, such as the NFS server 🌐, NFS share 📂, source directory 📁, and retention period ⏳.

## 🚀 Features

1. **🔄 Backup**: Create a new backup of the specified source directory 📁, compressing the data into a ZIP archive and storing it in a dedicated location on the NFS server.
2. **🛠️ Restore**: Restore a previously created backup by selecting from a list of available backups and extracting the contents to a target directory.
3. **⚙️ Configuration**: Edit settings ✏️, including the NFS server 🌐 address, the NFS share 📂 path, the source directory 📁 to be backed up, and the retention policy ⏳ that controls how many backups are kept.
4. **⏰ Automated Backups**: Set up a cronjob to automate daily backups 🗓️, ensuring your data is consistently saved at a specific time (default at 3 AM).
5. **🗑️ Backup Management**: Delete 🗑️ single or multiple backups, allowing you to free up space and manage storage effectively.

## 📋 Requirements

- **🐍 Python 3**: The script is written in Python 3 and uses standard libraries 📚.
- **📊 ********`tqdm`******** Library**: Provides progress bars 📊 during backup and restore operations.
- **🗜️ ********`zipfile`******** Library**: Part of the Python Standard Library, used for compression 🗜️.
- **🔑 ********`sudo`******** Access**: Required to run mount and chmod commands.
- **🌐 NFS Share**: An NFS share set up on the network.

📦 Install necessary libraries:

```bash
sudo apt install nfs-common python python-venv
```

## ⚙️ Installation

1. **📂 Clone the Repository**:
   ```bash
   git clone https://github.com/Madchristian/linux_home_backuptool.git
   ```
2. **📁 Navigate to the Project Directory**:
   ```bash
   cd linux_home_backuptool
   ```
3. **🐍 Create and Activate a Virtual Environment** (optional but recommended):
   ```bash
   python3 -m venv venv
   source venv/bin/activate
   ```
4. **📦 Install Dependencies**:
   ```bash
   pip install -r requirements.txt
   ```

## ⚙️ Configuration

The backup tool uses a configuration file (`backup_config.json`) to store settings ⚙️. This file is automatically created on the first run if it doesn't exist. Configuration options include:

- **🌐 NFS Server**: The IP address of your NFS server.
- **📂 NFS Share**: Path to the NFS share on the server.
- **📁 Mount Point**: Directory where the NFS share is mounted.
- **📁 Source Directory**: Directory to be backed up.
- **⏳ Backup Retention**: Number of backups to retain.

## 🖥️ Usage

### ▶️ Running the Backup Tool

To run the tool 🛠️ and access its main menu 📋:

```bash
python3 backup.py
```

![Main Menu Screenshot](images/main_menu.png)  
_Example of the main menu interface_

From the main menu 📋, you can choose to:

1. **🔄 Create a New Backup**: Create a new backup of the specified directory 📁.
2. **🛠️ Restore a Backup**: Choose an available backup and restore it.
3. **⚙️ Edit Configuration**: Update settings such as the NFS server 🌐, target directory 📁, and retention policy ⏳.
4. **🗑️ Delete Backups**: Delete a specific backup or all backups of a device.
5. **⏰ Add a Cronjob for Automated Backups**: Schedule daily backups 🗓️.
6. **🚪 Exit**: Exit the program.

### 🔄 Create a New Backup

Select the option to create a new backup. The script will mount the NFS share, compress 🗜️ the source directory into a zip file, and store it in the mounted directory.

### 🛠️ Restore a Backup

Select the option to restore a backup. The tool will list available backups 🔄, allowing you to select one to restore. The files will be extracted into the directory `~/restored_data`.

### ⏰ Add a Cronjob for Automated Backups

The tool includes functionality to add a cronjob for daily automated backups. The cronjob is set to run at 3 AM 🕒 every day.

### 🗑️ Delete Backups

You can delete individual backups 🗑️ or all backups at once.

### ⚙️ Edit Configuration

The configuration can be updated directly within the tool ⚙️. Options include setting a new NFS server 🌐, NFS share 📂, mount point 📁, source directory 📁, or backup retention policy ⏳. After changing the configuration, the tool will remount the NFS share if necessary.

## ⏰ Cronjob Details

The cronjob for automated backups runs the following command at 3 AM 🕒 every day:

```bash
0 3 * * * /path/to/backup.py backup
```

The cronjob is added automatically if you select the corresponding option from the main menu 📋.

## 📝 Logging

The tool creates a log file (`backup_tool.log`) within the script's directory to track operations 🛠️, errors ❌, and status messages 💬. It also supports rotating logs 🔄 to avoid excessive log file growth 📈. A maximum of 3 log files of size 5 MB each are retained.

## 🔒 Security Considerations

- The script requires `sudo` to mount/unmount NFS shares 🌐 and change permissions 🔑. Be aware that granting `sudo` permissions can pose security risks, as it allows for potentially unsafe system modifications. It is recommended to run the script in a controlled environment to minimize these risks.
- Be cautious ⚠️ with NFS permissions (`chmod 755` is used by default; adjust as needed for your environment).
- Avoid setting overly permissive access (e.g., `chmod 777`) on shared directories.

## 🛠️ Troubleshooting

### 🔑 Permission Denied Errors

Ensure that your user 👤 is part of the `sudoers` group and has permission to run mount commands.

To verify sudo permissions, run:
```bash
sudo -v
```
If the command fails, add your user to the `sudoers` group:
```bash
sudo usermod -aG sudo <your_username>
```

### ⏰ Cronjob Not Running

To check if the cron service is running, use:
```bash
systemctl status cron
```
If the service is not active, start it with:
```bash
sudo systemctl start cron
```
Additionally, verify that your cronjob is properly scheduled by listing your cron jobs:
```bash
crontab -l
```

Make sure the cron service is enabled and running on your system 🖥️:

```bash
sudo systemctl enable cron
sudo systemctl start cron
```

### 🌐 NFS Share Fails to Mount

Double-check the NFS server address 🌐, share path 📂, and verify that the NFS service is running properly on the server.

## 📜 License

This project is licensed under the MIT License 📄. See the `LICENSE` file for details.

## 🤝 Contributions

Feel free to fork the repository, submit pull requests, or open issues for suggestions and improvements ✨.

---

I have polished the text with emojis to enhance visual understanding. Let me know if any further adjustments are needed! 😊

