# Linux Home Backup Tool / Linux Home Backup-Tool

## English Version

This project provides a simple backup and restore tool designed to back up data from multiple Raspberry Pis to a central NFS server. It is built using Python and offers an easy-to-use command-line interface (CLI).

### Features

- **Backup as a compressed ZIP archive**: The script creates a compressed ZIP archive of the specified source directory.
- **NFS Support**: Data is backed up to a central server over NFS.
- **Backup Rotation**: The script retains a maximum of 14 backups per device, with older backups being automatically deleted.
- **Hostname-based Backup Directories**: Backups are stored in a directory that uses the hostname of each Raspberry Pi to easily distinguish backups from different devices.
- **Configuration File**: A configuration file (`backup_config.json`) allows users to specify parameters such as the NFS server, target path, source directory, and backup retention time. This file can be edited either manually or via the configuration menu.
- **User Menu for Interaction**: An interactive menu guides the user through creating new backups, restoring from existing backups, managing configuration, and deleting backups.

### Requirements

1. **Python 3**: This script is written in Python 3 and uses Python standard libraries.
2. **Virtual Environment**: It is recommended to run the script within a virtual environment.
3. **NFS Support**: Make sure `nfs-common` is installed on your Raspberry Pi.

#### Install `nfs-common`

To install the required package, run:

```sh
sudo apt-get update
sudo apt-get install nfs-common
```

### Setup

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

### Usage

#### Manually Run the Script

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

#### Automatic Backups via Cron Job

To automate backups every day, set up a cron job:

```sh
crontab -e
```

Add the following line to execute the backup daily at 3 AM:

```sh
0 3 * * * cd /path/to/linux_home_backuptool && source venv/bin/activate && /usr/bin/python3 backup.py backup --source /home/pi > /path/to/linux_home_backuptool/cronjob.log 2>&1
```

Replace `/path/to/linux_home_backuptool` with the actual path where you cloned the repository.

### Detailed Features

#### 1. NFS Mount
The script mounts an NFS share based on the settings in the configuration file (`backup_config.json`). The directory is mounted as `nfs_backup` within the current working directory. If the mount fails, an error is logged.

#### 2. Backup Creation
The script creates a ZIP archive of the specified source directory. The archive is stored in a directory that uses the hostname of the device, along with a timestamp to ensure uniqueness. Older backups are automatically deleted, keeping only the number specified in the configuration.

#### 3. Restoration
During restoration, the user can select a specific backup based on the timestamp. The backup is extracted to the `~/restored_data` directory.

#### 4. Configuration Management
The menu allows the user to edit the program configuration without manually editing the configuration file. Parameters like NFS server, target directory, retention period, etc., can be adjusted easily.

#### 5. Deleting Backups
Users can delete either a single backup or all backups of a particular device.

### Log Files
The script generates a log file (`backup_tool.log`) in the program directory that records all actions and errors. A maximum of 3 log files of size 5 MB each are retained.

### Notes
- **NFS Access**: The script uses `sudo` to mount the NFS share, so appropriate permissions are required.
- **Permission Management**: Ensure that the user running the script has enough permissions to access and write to the NFS share.

### Improvements
If you have ideas for improvements or encounter any issues, feel free to create issues on the GitHub repository or submit pull requests.

### Author
Created by Madchristian

Enjoy backing up your data! 😊

---

## Deutsche Version

Dieses Projekt bietet ein einfaches Backup- und Wiederherstellungs-Tool, das dazu dient, Daten von mehreren Raspberry Pis auf einen zentralen NFS-Server zu sichern. Es wurde in Python erstellt und bietet eine benutzerfreundliche Kommandozeilen-Schnittstelle (CLI).

### Funktionen

- **Backup als komprimiertes ZIP-Archiv**: Das Skript erstellt ein komprimiertes ZIP-Archiv des angegebenen Quellverzeichnisses.
- **NFS-Unterstützung**: Daten werden über NFS auf einen zentralen Server gesichert.
- **Backup-Rotation**: Das Skript behält maximal 14 Backups pro Gerät und löscht ältere Backups automatisch.
- **Hostnamen-basierte Backup-Verzeichnisse**: Backups werden in einem Verzeichnis gespeichert, das den Hostnamen jedes Raspberry Pi verwendet, um die Backups verschiedener Geräte leicht zu unterscheiden.
- **Konfigurationsdatei**: Eine Konfigurationsdatei (`backup_config.json`) ermöglicht es Benutzern, Parameter wie den NFS-Server, den Zielpfad, das Quellverzeichnis und die Aufbewahrungszeit für Backups anzugeben. Diese Datei kann entweder manuell oder über das Konfigurationsmenü bearbeitet werden.
- **Benutzermenü für die Interaktion**: Ein interaktives Menü führt den Benutzer durch das Erstellen neuer Backups, die Wiederherstellung bestehender Backups, die Verwaltung der Konfiguration und das Löschen von Backups.

### Anforderungen

1. **Python 3**: Dieses Skript wurde in Python 3 geschrieben und verwendet Python-Standardbibliotheken.
2. **Virtuelle Umgebung**: Es wird empfohlen, das Skript in einer virtuellen Umgebung auszuführen.
3. **NFS-Unterstützung**: Stellen Sie sicher, dass `nfs-common` auf Ihrem Raspberry Pi installiert ist.

#### Installieren von `nfs-common`

Um das erforderliche Paket zu installieren, führen Sie Folgendes aus:

```sh
sudo apt-get update
sudo apt-get install nfs-common
```

### Einrichtung

1. **Repository klonen**

   Klonen Sie das Git-Repository auf Ihren Raspberry Pi:

   ```sh
   git clone https://github.com/Madchristian/linux_home_backuptool.git
   cd linux_home_backuptool
   ```

2. **Virtuelle Umgebung erstellen**

   Erstellen und aktivieren Sie eine virtuelle Umgebung:

   ```sh
   python3 -m venv venv
   source venv/bin/activate
   ```

3. **Abhängigkeiten installieren**

   Installieren Sie die erforderlichen Pakete aus `requirements.txt`:

   ```sh
   pip install -r requirements.txt
   ```

### Verwendung

#### Skript manuell ausführen

Aktivieren Sie die virtuelle Umgebung und führen Sie das Skript aus:

```sh
source venv/bin/activate
python3 backup.py
```

Das Skript präsentiert Ihnen ein Menü mit den folgenden Optionen:

1. **Neues Backup erstellen**: Erstellen Sie ein neues Backup des angegebenen Verzeichnisses.
2. **Backup wiederherstellen**: Wählen Sie ein verfügbares Backup aus und stellen Sie es wieder her.
3. **Konfiguration bearbeiten**: Ändern Sie Einstellungen wie den NFS-Server, das Zielverzeichnis und die Aufbewahrung.
4. **Backup löschen**: Löschen Sie ein bestimmtes Backup oder alle Backups eines Geräts.
5. **Beenden**: Beenden Sie das Programm.

#### Automatische Backups über Cron-Job

Um tägliche Backups zu automatisieren, richten Sie einen Cron-Job ein:

```sh
crontab -e
```

Fügen Sie die folgende Zeile hinzu, um das Backup täglich um 3 Uhr morgens auszuführen:

```sh
0 3 * * * cd /path/to/linux_home_backuptool && source venv/bin/activate && /usr/bin/python3 backup.py backup --source /home/pi > /path/to/linux_home_backuptool/cronjob.log 2>&1
```

Ersetzen Sie `/path/to/linux_home_backuptool` durch den tatsächlichen Pfad, an dem Sie das Repository geklont haben.

### Detaillierte Funktionen

#### 1. NFS-Mount
Das Skript mountet ein NFS-Share basierend auf den Einstellungen in der Konfigurationsdatei (`backup_config.json`). Das Verzeichnis wird als `nfs_backup` innerhalb des aktuellen Arbeitsverzeichnisses gemountet. Wenn das Mounten fehlschlägt, wird ein Fehler protokolliert.

#### 2. Backup-Erstellung
Das Skript erstellt ein ZIP-Archiv des angegebenen Quellverzeichnisses. Das Archiv wird in einem Verzeichnis gespeichert, das den Hostnamen des Geräts verwendet, zusammen mit einem Zeitstempel zur Sicherstellung der Eindeutigkeit. Ältere Backups werden automatisch gelöscht, wobei nur die Anzahl beibehalten wird, die in der Konfiguration angegeben ist.

#### 3. Wiederherstellung
Während der Wiederherstellung kann der Benutzer ein spezifisches Backup basierend auf dem Zeitstempel auswählen. Das Backup wird in das Verzeichnis `~/restored_data` extrahiert.

#### 4. Konfigurationsverwaltung
Das Menü ermöglicht es dem Benutzer, die Programmkonfiguration zu bearbeiten, ohne die Konfigurationsdatei manuell zu ändern. Parameter wie NFS-Server, Zielverzeichnis, Aufbewahrungszeitraum usw. können leicht angepasst werden.

#### 5. Backups löschen
Benutzer können entweder ein einzelnes Backup oder alle Backups eines bestimmten Geräts löschen.

### Log-Dateien
Das Skript erstellt eine Log-Datei (`backup_tool.log`) im Programmverzeichnis, die alle Aktionen und Fehler aufzeichnet. Es werden maximal 3 Log-Dateien mit einer Größe von jeweils 5 MB beibehalten.

### Hinweise
- **NFS-Zugriff**: Das Skript verwendet `sudo`, um das NFS-Share zu mounten, daher sind entsprechende Berechtigungen erforderlich.
- **Berechtigungsverwaltung**: Stellen Sie sicher, dass der Benutzer, der das Skript ausführt, ausreichende Berechtigungen hat, um auf das NFS-Share zuzugreifen und darauf zu schreiben.

### Verbesserungen
Wenn Sie Verbesserungsvorschläge haben oder auf Probleme stoßen, können Sie Issues im GitHub-Repository erstellen oder Pull Requests einreichen.

### Autor
Erstellt von Madchristian

Viel Spaß beim Sichern Ihrer Daten! 😊

