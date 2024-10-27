#include "config.h"
#include <iostream>
#include <fstream>
#include <yaml-cpp/yaml.h>

Config Config::load_config() {
    YAML::Node config_node = YAML::LoadFile("config/backup_config.yaml");

    Config config;
    config.nfs_server = config_node["nfs_server"].as<std::string>();
    config.nfs_share = config_node["nfs_share"].as<std::string>();
    config.mount_point = config_node["mount_point"].as<std::string>();
    config.backup_source = config_node["backup_source"].as<std::string>();
    config.backup_destination = config_node["backup_destination"].as<std::string>();
    if (config_node["restore_destination"]) {
        config.restore_destination = config_node["restore_destination"].as<std::string>();
    }
    config.backup_retention = config_node["backup_retention"].as<int>();

    return config;
}

void Config::save_config() const {
    YAML::Node config_node;
    config_node["nfs_server"] = nfs_server;
    config_node["nfs_share"] = nfs_share;
    config_node["mount_point"] = mount_point;
    config_node["backup_source"] = backup_source;
    config_node["backup_destination"] = backup_destination;
    config_node["restore_destination"] = restore_destination;
    config_node["backup_retention"] = backup_retention;

    std::ofstream fout("config/backup_config.yaml");
    fout << config_node;
}

void Config::edit_config() {
    std::cout << "\nEdit Configuration:" << std::endl;
    std::cout << "1: NFS Server (current: " << nfs_server << ")" << std::endl;
    std::cout << "2: NFS Share (current: " << nfs_share << ")" << std::endl;
    std::cout << "3: Mount Point (current: " << mount_point << ")" << std::endl;
    std::cout << "4: Backup Source (current: " << backup_source << ")" << std::endl;
    std::cout << "5: Backup Destination (current: " << backup_destination << ")" << std::endl;
    std::cout << "6: Restore Destination (current: " << restore_destination << ")" << std::endl;
    std::cout << "7: Backup Retention (current: " << backup_retention << " days)" << std::endl;
    std::cout << "8: Back to main menu" << std::endl;

    int choice;
    std::cin >> choice;

    switch (choice) {
        case 1:
            std::cout << "New NFS Server: ";
            std::cin >> nfs_server;
            break;
        case 2:
            std::cout << "New NFS Share: ";
            std::cin >> nfs_share;
            break;
        case 3:
            std::cout << "New Mount Point: ";
            std::cin >> mount_point;
            break;
        case 4:
            std::cout << "New Backup Source: ";
            std::cin >> backup_source;
            break;
        case 5:
            std::cout << "New Backup Destination: ";
            std::cin >> backup_destination;
            break;
        case 6:
            std::cout << "New Restore Destination: ";
            std::cin >> restore_destination;
            break;
        case 7:
            std::cout << "New Backup Retention (days): ";
            std::cin >> backup_retention;
            break;
        case 8:
            return;
        default:
            std::cout << "Invalid choice." << std::endl;
            break;
    }

    save_config();
}