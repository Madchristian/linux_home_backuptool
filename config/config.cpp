#include "config.h"
#include "filehandler.h"
#include "nfs.h"
#include <iostream>
#include <cstdlib>
#include <string>
#include <filesystem>
#include <fstream>
#include <yaml-cpp/yaml.h>

void save_config(const Config& config) {
    YAML::Emitter out;
    out << YAML::BeginMap;
    out << YAML::Key << "nfs_server" << YAML::Value << config.nfs_server;
    out << YAML::Key << "nfs_share" << YAML::Value << config.nfs_share;
    out << YAML::Key << "mount_point" << YAML::Value << config.mount_point;
    out << YAML::Key << "source_directory" << YAML::Value << config.source_directory;
    out << YAML::Key << "backup_retention" << YAML::Value << config.backup_retention;
    out << YAML::EndMap;

    std::ofstream config_file("backup_config.yaml");
    if (!config_file) {
        std::cerr << "Error saving configuration." << std::endl;
        return;
    }
    config_file << out.c_str();
}

Config load_config() {
    Config config;
    if (!fs::exists("backup_config.yaml")) {
        // Set default values if config file doesn't exist
        config.nfs_server = "10.0.30.20";
        config.nfs_share = "/mnt/MainStorage/backups/rpi";
        config.mount_point = "nfs_backup";
        config.source_directory = "/home/pi";
        config.backup_retention = 14;
        save_config(config);
        return config;
    }

    try {
        YAML::Node config_file = YAML::LoadFile("backup_config.yaml");
        config.nfs_server = config_file["nfs_server"].as<std::string>();
        config.nfs_share = config_file["nfs_share"].as<std::string>();
        config.mount_point = config_file["mount_point"].as<std::string>();
        config.source_directory = config_file["source_directory"].as<std::string>();
        config.backup_retention = config_file["backup_retention"].as<int>();
    } catch (const std::exception& e) {
        std::cerr << "Error loading configuration: " << e.what() << std::endl;
    }

    return config;
}

void edit_config(Config& config) {
    std::cout << "\nEdit Configuration:" << std::endl;
    std::cout << "1: NFS Server (current: " << config.nfs_server << ")" << std::endl;
    std::cout << "2: NFS Share (current: " << config.nfs_share << ")" << std::endl;
    std::cout << "3: Mount Point (current: " << config.mount_point << ")" << std::endl;
    std::cout << "4: Source Directory (current: " << config.source_directory << ")" << std::endl;
    std::cout << "5: Backup Retention (current: " << config.backup_retention << " days)" << std::endl;
    std::cout << "6: Back to main menu" << std::endl;
    std::cout << "Choose an option: ";
    int choice;
    std::cin >> choice;
    if (choice == 1) {
        std::cout << "Enter new NFS Server: ";
        std::cin >> config.nfs_server;
    } else if (choice == 2) {
        std::cout << "Enter new NFS Share: ";
        std::cin >> config.nfs_share;
    } else if (choice == 3) {
        std::cout << "Enter new Mount Point: ";
        std::cin >> config.mount_point;
    } else if (choice == 4) {
        std::cout << "Enter new Source Directory: ";
        std::cin >> config.source_directory;
    } else if (choice == 5) {
        std::cout << "Enter new Backup Retention (days): ";
        std::cin >> config.backup_retention;
    } else if (choice == 6) {
        return;
    } else {
        std::cerr << "Invalid selection." << std::endl;
    }
    save_config(config);
}