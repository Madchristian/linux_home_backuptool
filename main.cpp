#include "config.h"
#include "filehandler.h"
#include "nfs.h"
#include "progress.h"
#include <iostream>
#include <cstdlib>
#include <string>
#include <filesystem>
#include <fstream>
#include <yaml-cpp/yaml.h>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <unistd.h>
#include <termios.h>
#include <thread>
#include <atomic>

namespace fs = std::filesystem;

void setup_logging() {
    // Set up logging to both a rotating file and console for real-time monitoring.
    auto log_directory = fs::current_path();
    auto log_file = log_directory / "backup_tool.log";
    // Placeholder for actual logging implementation as the original logging library was incorrect.
    std::cout << "Logging setup completed: " << log_file << std::endl;
}

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

void disable_canonical_mode() {
    struct termios tty;
    tcgetattr(STDIN_FILENO, &tty);
    tty.c_lflag &= ~ICANON;
    tcsetattr(STDIN_FILENO, TCSANOW, &tty);
}

void enable_canonical_mode() {
    struct termios tty;
    tcgetattr(STDIN_FILENO, &tty);
    tty.c_lflag |= ICANON;
    tcsetattr(STDIN_FILENO, TCSANOW, &tty);
}

int main() {
    setup_logging();
    Config config = Config::load_config();
    nfs::mount_nfs_share(config);
    
    disable_canonical_mode();
    
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
            std::string hostname = []() -> std::string {
                char buffer[256];
                gethostname(buffer, sizeof(buffer));
                return std::string(buffer);
            }();
            config.backup_destination = config.backup_destination + "/" + hostname;
            fs::create_directories(config.backup_destination);

            // Progress display setup
            std::atomic<size_t> progress{0};
            size_t total_files = std::distance(fs::recursive_directory_iterator(config.backup_source), fs::recursive_directory_iterator{});
            std::thread progress_thread(progress::show_progress, std::ref(progress), total_files);
            
            // Actual backup process
            filehandler::create_backup(config);
            
            progress = total_files;
            progress_thread.join();
        } else if (choice == 2) {
            std::cout << "Available backups in " << config.backup_destination << ":\n";
            bool backups_found = false;
            for (const auto& entry : fs::directory_iterator(config.backup_destination)) {
                if (entry.is_regular_file()) {
                    std::cout << entry.path().filename().string() << std::endl;
                    backups_found = true;
                }
            }
            if (!backups_found) {
                std::cout << "No backups found." << std::endl;
            } else {
                filehandler::restore_backup(config);
            }
        } else if (choice == 3) {
            config.edit_config();
        } else if (choice == 4) {
            std::cout << "Available backups in " << config.backup_destination << ":\n";
            bool backups_found = false;
            for (const auto& entry : fs::directory_iterator(config.backup_destination)) {
                if (entry.is_regular_file()) {
                    std::cout << entry.path().filename().string() << std::endl;
                    backups_found = true;
                }
            }
            if (!backups_found) {
                std::cout << "No backups found." << std::endl;
            } else {
                filehandler::delete_backup(config);
            }
        } else if (choice == 5) {
            add_cronjob(config);
        } else if (choice == 6) {
            break;
        } else {
            std::cerr << "Invalid selection." << std::endl;
        }
    }
    
    enable_canonical_mode();
    
    nfs::unmount_nfs_share(config.mount_point);
    return 0;
}
