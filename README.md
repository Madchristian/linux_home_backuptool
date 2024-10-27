# 🍓 Raspberry Pi Backup and Restore Tool 🔄💾

## 🌟 Overview

This tool helps you make copies of your Raspberry Pi files 📁, fix them if they break 🛠️, and keep them safe 🔄. It saves files over the network 🌐, uses zip 🗜️ to pack them up, and saves them at a set time ⏰. You can also change settings ⚙️, like where the files go and how long they are kept.

## 🚀 What It Can Do

1. **🔄 Make a Backup**: Make a new copy of your important files 📁.
2. **🛠️ Fix Files**: Get your files back if something goes wrong.
3. **⚙️ Change Settings**: Change things like where the files are saved 🌐, and how long to keep them ⏳.
4. **⏰ Automatic Backups**: Make backups every day at the same time 🕒.
5. **🗑️ Manage Backups**: Delete old copies 🗑️ to save space.

## 📋 What You Need

- **🐍 Python 3**: This tool uses Python 3.
- **📊 `tqdm`**: This shows progress while making or fixing a backup.
- **🗜️ `zipfile`**: This helps pack up the files.
- **🔑 `sudo` Access**: You need `sudo` to run some commands.
- **🌐 NFS Share**: A place on the network to save files.

📦 To get the things you need, run this:

```bash
sudo apt install nfs-common python python-venv
```

## ⚙️ How to Set It Up

1. **📂 Get the Files**:
   ```bash
   git clone https://github.com/Madchristian/linux_home_backuptool.git
   ```
2. **📁 Go to the Folder**:
   ```bash
   cd linux_home_backuptool
   ```
3. **🐍 Set Up a Virtual Environment**:
   ```bash
   python3 -m venv venv
   source venv/bin/activate
   ```
4. **📦 Install What You Need**:
   ```bash
   pip install -r requirements.txt
   ```

## ⚙️ How to Change Settings

The tool saves settings in a file called `backup_config.json`. This file is made the first time you run the tool. The settings include:

- **🌐 NFS Server**: The address of your NFS server.
- **📂 NFS Share**: The place on the server where files are saved.
- **📁 Mount Point**: The folder where the NFS share is used.
- **📁 Source Directory**: The folder to back up.
- **⏳ Backup Retention**: How many copies to keep.

## 🖥️ How to Use It

### ▶️ Run the Tool

To run the tool and see the menu:

```bash
python3 backup.py
```

\
*This is what the menu looks like*

From the menu, you can pick:

1. **🔄 Make a New Backup**: Copy your files 📁.
2. **🛠️ Restore a Backup**: Get back your files from a backup.
3. **⚙️ Edit Settings**: Change things like where files are saved 🌐.
4. **🗑️ Delete Backups**: Remove old backups.
5. **⏰ Add a Cronjob**: Make backups every day.
6. **🚪 Exit**: Stop the tool.

### 🔄 Make a New Backup

Pick this to make a new backup. The tool will use the NFS share, pack up the files into a zip, and save them.

### 🛠️ Restore a Backup

Pick this to get back your files. The tool will show you the backups you have 🔄, and you can pick one to restore.

### ⏰ Add a Cronjob

This makes the tool run every day at 3 AM 🕒 to make backups.

### 🗑️ Delete Backups

You can delete one backup 🗑️ or all of them.

### ⚙️ Edit Settings

You can change things like the NFS server 🌐, NFS share 📂, and how many backups to keep ⏳. The tool will use the new settings next time.

## ⏰ Cronjob Details

The tool runs this command every day at 3 AM:

```bash
0 3 * * * /path/to/backup.py backup
```

This command is added automatically if you choose it in the menu 📋.

## 📝 Logging

The tool makes a log file (`backup_tool.log`) in the same folder. This log keeps track of what happens 🛠️, any errors ❌, and other messages 💬. The log file doesn’t get too big because it makes new ones when needed.

## 🔒 Be Careful!

- The tool uses `sudo` to mount and unmount the NFS share 🌐. Be careful, because `sudo` can make big changes to your system. Use it in a safe place.
- Be careful with NFS permissions (`chmod 755` is used by default).
- Don’t use permissions that are too open (`chmod 777`).

## 🛠️ Fixing Problems

### 🔑 Permission Problems

Make sure you have the right permissions 👤.

To check if you have `sudo` access:

```bash
sudo -v
```

If it doesn’t work, add yourself to the `sudoers` group:

```bash
sudo usermod -aG sudo <your_username>
```

### ⏰ Cronjob Not Running

To see if the cron service is running:

```bash
systemctl status cron
```

If it’s not running, start it:

```bash
sudo systemctl start cron
```

To see if your cron job is scheduled:

```bash
crontab -l
```

Make sure the cron service is turned on:

```bash
sudo systemctl enable cron
sudo systemctl start cron
```

### 🌐 NFS Share Problems

Check the server address 🌐 and the NFS share 📂 to make sure everything is working.

## 📜 License

This tool is under the Apache 2.0 License 📄. See the `LICENSE` file for more details.

## 🤝 Help Out

You can copy the repository, send changes, or suggest new ideas ✨.

---

The text now uses simpler language to make it easier for young children to understand. 😊

