# Backup Tool - Installation (English / Deutsch)

## English Version

### Introduction
This Backup Tool allows you to create backups and restore them easily on Linux systems. The tool is now available as a precompiled executable that can be downloaded from the latest GitHub release. It ensures that backups are safely stored and managed with configurable settings.

### Getting Started

1. **Download the Backup Tool**
   - Run the following command to download the latest version of the `backup_tool` executable directly from GitHub:
     ```sh
     curl -LO https://github.com/Madchristian/linux_home_backuptool/releases/latest/download/backup_tool
     ```

2. **Make the Tool Executable**
   - Once downloaded, open a terminal and navigate to the directory where the file is located.
   - Run the following command to make the tool executable:
     ```sh
     chmod +x backup_tool
     ```

3. **Run the Backup Tool**
   - You can now run the backup tool directly from the terminal:
     ```sh
     ./backup_tool
     ```

### Usage
- The tool provides a menu interface that allows you to:
  - Create a new backup
  - Restore a backup
  - Edit configuration settings
  - Delete old backups
  - Add a cronjob for automated backups

### Prerequisites
- The following packages are required on your Linux system to use this tool effectively:
  - `nfs-common` (for mounting NFS shares)
  - `libzip` (for handling zip archives)

### Installation of Dependencies
Run the following command to install required dependencies:
```sh
sudo apt-get update
sudo apt-get install -y nfs-common libzip-dev
```

### Notes
- Ensure that the NFS share is accessible before running the tool.
- The tool needs to be run with appropriate permissions if NFS mounts or root-level changes are required.

## Deutsche Version

### Einleitung
Dieses Backup-Tool ermöglicht es, Sicherungen zu erstellen und einfach auf Linux-Systemen wiederherzustellen. Das Tool ist jetzt als vorkompilierte ausführbare Datei verfügbar, die aus dem neuesten GitHub-Release heruntergeladen werden kann. Es sorgt dafür, dass Backups sicher gespeichert und verwaltet werden, mit konfigurierbaren Einstellungen.

### Erste Schritte

1. **Backup-Tool herunterladen**
   - Lade die neueste Version der ausführbaren Datei `backup_tool` direkt von GitHub herunter:
     ```sh
     curl -LO https://github.com/Madchristian/linux_home_backuptool/releases/latest/download/backup_tool
     ```

2. **Das Tool ausführbar machen**
   - Nach dem Herunterladen öffne ein Terminal und navigiere zu dem Verzeichnis, in dem sich die Datei befindet.
   - Führe folgenden Befehl aus, um das Tool ausführbar zu machen:
     ```sh
     chmod +x backup_tool
     ```

3. **Das Backup-Tool ausführen**
   - Du kannst das Backup-Tool nun direkt im Terminal ausführen:
     ```sh
     ./backup_tool
     ```

### Verwendung
- Das Tool bietet eine Menüoberfläche, die es ermöglicht:
  - Eine neue Sicherung zu erstellen
  - Eine Sicherung wiederherzustellen
  - Konfigurationseinstellungen zu bearbeiten
  - Alte Sicherungen zu löschen
  - Einen Cronjob für automatische Sicherungen hinzuzufügen

### Voraussetzungen
- Die folgenden Pakete sind auf deinem Linux-System erforderlich, um dieses Tool effektiv zu nutzen:
  - `nfs-common` (zum Einbinden von NFS-Shares)
  - `libzip` (zum Umgang mit Zip-Archiven)

### Installation der Abhängigkeiten
Führe den folgenden Befehl aus, um die erforderlichen Abhängigkeiten zu installieren:
```sh
sudo apt-get update
sudo apt-get install -y nfs-common libzip-dev
```

### Hinweise
- Stelle sicher, dass der NFS-Share vor dem Starten des Tools zugänglich ist.
- Das Tool muss mit den entsprechenden Berechtigungen ausgeführt werden, wenn NFS-Mounts oder Änderungen auf Root-Ebene erforderlich sind.