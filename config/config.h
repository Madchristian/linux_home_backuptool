#ifndef CONFIG_H
#define CONFIG_H

#include <string>

struct Config {
    std::string nfs_server;
    std::string nfs_share;
    std::string mount_point;
    std::string source_directory;
    int backup_retention;
};

Config load_config();
void save_config(const Config& config);
void edit_config(Config& config);

#endif
