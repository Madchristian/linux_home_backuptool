#include "config.h"
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

Config load_config() {
    std::ifstream config_file("backup_config.json");
    if (!config_file) {
        Config default_config = {"10.0.30.20", "/mnt/MainStorage/backups/rpi", "nfs_backup", "/home/pi", 14};
        json j = {
            {"nfs_server", default_config.nfs_server},
            {"nfs_share", default_config.nfs_share},
            {"mount_point", default_config.mount_point},
            {"source_directory", default_config.source_directory},
            {"backup_retention", default_config.backup_retention}
        };
        std::ofstream new_config("backup_config.json");
        new_config << j.dump(4);
        return default_config;
    }
    json j;
    config_file >> j;
    Config config;
    config.nfs_server = j["nfs_server"];
    config.nfs_share = j["nfs_share"];
    config.mount_point = j["mount_point"];
    config.source_directory = j["source_directory"];
    config.backup_retention = j["backup_retention"];
    return config;
}

void save_config(const Config& config) {
    json j = {
        {"nfs_server", config.nfs_server},
        {"nfs_share", config.nfs_share},
        {"mount_point", config.mount_point},
        {"source_directory", config.source_directory},
        {"backup_retention", config.backup_retention}
    };
    std::ofstream config_file("backup_config.json");
    config_file << j.dump(4);
}

void edit_config(Config& config) {
    std::cout << "\nEdit Configuration:" << std::endl;
    std::cout << "1: NFS Server (current: " << config.nfs_server << ")" << std::endl;
    std::cout << "2: NFS Share (current: " << config.nfs_share << ")" << std::endl;
    // Implement similar to Python version for editing.
}

