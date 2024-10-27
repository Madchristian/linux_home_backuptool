#include "config.h"
#include "filehandler.h"
#include "nfs.h"
#include <iostream>
#include <cstdlib>
#include <string>
#include <filesystem>
#include <fstream>
#include <yaml-cpp/yaml.h>

namespace fs = std::filesystem;

void add_cronjob(const Config& config) {
    // Get the current executable path
    std::string exec_path = fs::current_path().string() + "/backup_tool";
    // Adding a cronjob to execute the backup daily at 3 AM using the current executable path
    std::string cron_command = "(crontab -l 2>/dev/null; echo \"0 3 * * * " + exec_path + " create_backup\") | crontab -";
    int ret_code = std::system(cron_command.c_str());
    if (ret_code == 0) {
        std::cout << "Cronjob successfully added for daily backup at 3 AM." << std::endl;
    } else {
        std::cerr << "Failed to add cronjob." << std::endl;
    }
}

int main() {
    Config config = Config::load_config();
    nfs::mount_nfs_share(config);
    
    int choice;
    while (true) {
        std::cout << "\nBackup and Restore Tool" << std::endl;
        std::cout << "1: Create new backup" << std::endl;
        std::cout << "2: Restore backup" << std::endl;
        std::cout << "3: Edit configuration" << std::endl;
        std::cout << "4: Delete backup" << std::endl;
        std::cout << "5: Add cronjob for automated backups" << std::endl;
        std::cout << "6: Exit" << std::endl;
        std::cout << "Choose an option: ";
        std::cin >> choice;
        if (choice == 1) {
            filehandler::create_backup(config);
        } else if (choice == 2) {
            filehandler::restore_backup(config);
        } else if (choice == 3) {
            config.edit_config();
        } else if (choice == 4) {
            filehandler::delete_backup(config);
        } else if (choice == 5) {
            add_cronjob(config);
        } else if (choice == 6) {
            break;
        } else {
            std::cerr << "Invalid selection." << std::endl;
        }
    }
    nfs::unmount_nfs_share(config.mount_point);
    return 0;
}
