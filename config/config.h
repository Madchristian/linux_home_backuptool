#ifndef CONFIG_H
#define CONFIG_H

#include <string>

class Config {
public:
    std::string nfs_server;
    std::string nfs_share;
    std::string mount_point;
    std::string backup_source;
    std::string backup_destination;
    std::string restore_destination;  // Optional, falls nötig
    int backup_retention;

    static Config load_config();
    void save_config() const;
    void edit_config();
};

#endif // CONFIG_H
