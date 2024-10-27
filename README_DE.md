# Linux Home Backup Tool

Dieses Projekt bietet ein einfaches Backup- und Restore-Tool, das entwickelt wurde, um Daten von mehreren Raspberry Pis auf einen zentralen NFS-Server zu sichern. Es verwendet eine Python-basierte Anwendung mit einer benutzerfreundlichen CLI (Command Line Interface).

## Funktionen

- **Backup als komprimiertes ZIP-Archiv**: Das Skript erstellt ein komprimiertes ZIP-Archiv des angegebenen Quellverzeichnisses.
- **NFS-Unterstützung**: Daten werden über NFS auf einen zentralen Server gesichert.
- **Backup-Rotation**: Das Skript behält maximal 14 Backups für jedes Gerät, ältere Backups werden automatisch gelöscht.
- **Hostname-basierte Backup-Verzeichnisse**: Die Backups werden in einem Verzeichnis abgelegt, das den Hostnamen des jeweiligen Raspberry Pis verwendet, um eine einfache Unterscheidung der Backups von verschiedenen Geräten zu ermöglichen.
- **Konfigurationsdatei**: Eine Konfigurationsdatei (`backup_config.json`) ermöglicht es dem Benutzer, Parameter wie NFS-Server, Zielpfad, Backup-Retention und andere Optionen festzulegen und zu ändern.
- **Menü zur Benutzerführung**: Ein einfaches, geschachteltes Menü führt den Benutzer durch das Erstellen eines neuen Backups, die Wiederherstellung eines bestehenden Backups, die Verwaltung der Konfiguration und das Löschen von Backups.

## Voraussetzungen

1. **Python 3**: Dieses Skript ist für Python 3 geschrieben und verwendet Python-Standardbibliotheken.
2. **Virtuelle Umgebung**: Es wird empfohlen, das Skript in einer virtuellen Umgebung zu betreiben.
3. **NFS-Unterstützung**: Stelle sicher, dass `nfs-common` auf deinem Raspberry Pi installiert ist.

### Installation von `nfs-common`

Führe folgenden Befehl aus, um die notwendigen Pakete zu installieren:

```sh
sudo apt-get update
sudo apt-get install nfs-common
```

## Einrichtung

1. **Repository klonen**

   Klone das Git-Repository auf deinem Raspberry Pi:

   ```sh
   git clone https://github.com/Madchristian/linux_home_backuptool.git
   cd linux_home_backuptool
   ```

2. **Virtuelle Umgebung erstellen**

   Erstelle eine virtuelle Umgebung und aktiviere sie:

   ```sh
   python3 -m venv venv
   source venv/bin/activate
   ```

3. **Abhängigkeiten installieren**

   Installiere die nötigen Pakete aus der `requirements.txt`:

   ```sh
   pip install -r requirements.txt
   ```

## Konfiguration

Das Skript verwendet eine Konfigurationsdatei (`backup_config.json`), um Parameter wie NFS-Server, Zielpfad, Quellverzeichnis und Backup-Retention-Zeit zu verwalten. Diese Datei kann entweder manuell oder durch das Konfigurationsmenü im Skript bearbeitet werden.

Beispiel für eine Konfigurationsdatei:

```json
{
  "nfs_server": "10.0.30.20",
  "nfs_share": "/mnt/MainStorage/backups/rpi",
  "mount_point": "nfs_backup",
  "source_directory": "/home/pi",
  "backup_retention": 14
}
```

## Nutzung

### Manuelle Ausführung des Skripts

Aktiviere die virtuelle Umgebung und führe das Skript aus:

```sh
source venv/bin/activate
python3 backup.py
```

Das Skript präsentiert dir ein Menü mit folgenden Optionen:

1. **Neues Backup erstellen**: Erstelle ein neues Backup des gewünschten Verzeichnisses.
2. **Backup wiederherstellen**: Wähle ein vorhandenes Backup aus und stelle es wieder her.
3. **Konfiguration bearbeiten**: Ändere Einstellungen wie den NFS-Server, das Zielverzeichnis, die Backup-Retention usw.
4. **Backup löschen**: Lösche ein spezifisches Backup oder alle Backups eines Geräts.
5. **Beenden**: Beende das Programm.

### Automatische Backups mittels Cron-Job

Um täglich automatische Backups zu erstellen, kannst du einen Cron-Job einrichten:

```sh
crontab -e
```
Füge dann die folgende Zeile hinzu, um das Backup täglich um 3 Uhr morgens durchzuführen:

```sh
0 3 * * * cd /pfad/zum/linux_home_backuptool && source venv/bin/activate && /usr/bin/python3 backup.py backup --source /home/pi > /pfad/zum/linux_home_backuptool/cronjob.log 2>&1
```

Ersetze `/pfad/zum/linux_home_backuptool` durch den tatsächlichen Pfad, in dem du das Repository geklont hast.

## Funktionen im Detail

### 1. NFS-Mount
Das Skript mountet ein NFS-Share basierend auf den Einstellungen in der Konfigurationsdatei (`backup_config.json`). Das Verzeichnis wird als `nfs_backup` im aktuellen Programmverzeichnis gemountet. Falls das Mounten fehlschlägt, wird ein Fehler ins Log geschrieben.

### 2. Backup-Erstellung
Das Skript erstellt ein ZIP-Archiv des angegebenen Quellverzeichnisses. Das Archiv wird unter einem Verzeichnis mit dem Hostnamen des Geräts und einem Zeitstempel gespeichert, um eine eindeutige Identifikation zu gewährleisten. Außerdem werden automatisch ältere Backups gelöscht, sodass maximal die in der Konfiguration festgelegte Anzahl an Backups erhalten bleibt.

### 3. Wiederherstellung
Bei der Wiederherstellung kann der Benutzer ein spezifisches Backup anhand des Datums auswählen. Das Backup wird in das Verzeichnis `~/restored_data` entpackt.

### 4. Konfigurationsverwaltung
Das Menü ermöglicht es dem Benutzer, die Konfiguration des Programms direkt zu ändern, ohne die Konfigurationsdatei manuell bearbeiten zu müssen. Es werden Parameter wie NFS-Server, Backup-Zielverzeichnis, Retentionszeit usw. unterstützt.

### 5. Backup-Löschung
Der Benutzer kann entweder ein einzelnes Backup auswählen und löschen oder alle Backups eines bestimmten Geräts entfernen.

## Log-Dateien
Das Skript erstellt eine Log-Datei (`backup_tool.log`) im Programmverzeichnis, in der alle Aktionen sowie Fehler festgehalten werden. Es werden maximal 3 Log-Dateien mit einer Größe von je 5 MB beibehalten.

## Hinweise
- **NFS-Zugriff**: Das Skript verwendet `sudo` zum Mounten des NFS-Shares, was bedeutet, dass du entsprechende Berechtigungen benötigst.
- **Rechteverwaltung**: Stelle sicher, dass der Benutzer `backup_sa` oder der Benutzer, der das Skript ausführt, genügend Rechte hat, um auf das NFS-Share zuzugreifen und dort zu schreiben.

## Verbesserungen
Falls du Ideen für Verbesserungen hast oder auf Probleme stößt, bist du herzlich eingeladen, Issues im GitHub-Repository zu erstellen oder Pull Requests einzureichen.

## Autor
Erstellt von Madchristian

Viel Spaß beim Sichern deiner Daten! 😊

