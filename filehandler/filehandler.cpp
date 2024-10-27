#include "filehandler.h"
#include <filesystem>
#include <iostream>
#include <cstdlib>

namespace filehandler {

void create_backup(const Config& config) {
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::string backup_name = "backup_" + std::to_string(now_c) + ".tar.gz";

    std::string backup_path = config.backup_destination + "/" + backup_name;
    std::string command = "tar -czf " + backup_path + " -C " + config.backup_source + " .";
    
    int ret_code = std::system(command.c_str());
    if (ret_code == 0) {
        std::cout << "Backup successfully created: " << backup_path << std::endl;
    } else {
        std::cerr << "Failed to create backup." << std::endl;
    }
}

void restore_backup(const Config& config) {
    std::string backup_file;
    std::cout << "Enter the backup file name to restore: ";
    std::cin >> backup_file;

    std::string backup_path = config.backup_destination + "/" + backup_file;
    std::string restore_target = config.restore_destination.empty() ? config.backup_source : config.restore_destination;

    std::string command = "tar -xzf " + backup_path + " -C " + restore_target;

    int ret_code = std::system(command.c_str());
    if (ret_code == 0) {
        std::cout << "Backup successfully restored from: " << backup_path << std::endl;
    } else {
        std::cerr << "Failed to restore backup." << std::endl;
    }
}

void delete_backup(const Config& config) {
    std::string backup_file;
    std::cout << "Enter the backup file name to delete: ";
    std::cin >> backup_file;

    std::string backup_path = config.backup_destination + "/" + backup_file;

    if (std::filesystem::remove(backup_path)) {
        std::cout << "Backup successfully deleted: " << backup_path << std::endl;
    } else {
        std::cerr << "Failed to delete backup." << std::endl;
    }
}

} // namespace filehandler
