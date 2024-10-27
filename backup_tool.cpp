#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <sstream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <cstdio>
#include <zip.h>
#include <cstdlib>
#include <sys/stat.h>
#include <sys/mount.h>
#include <unistd.h>

namespace fs = std::filesystem;

struct Config {
    std::string nfs_server;
    std::string nfs_share;
    std::string mount_point;
    std::string source_directory;
    int backup_retention;
};

Config load_config() {
    std::ifstream config_file("backup_config.json");
    if (!config_file) {
        Config default_config = {"10.0.30.20", "/mnt/MainStorage/backups/rpi", "nfs_backup", "/home/pi", 14};
        std::ofstream new_config("backup_config.json");
        new_config << "{\n"
                   << "  \"nfs_server\": \"" << default_config.nfs_server << "\",\n"
                   << "  \"nfs_share\": \"" << default_config.nfs_share << "\",\n"
                   << "  \"mount_point\": \"" << default_config.mount_point << "\",\n"
                   << "  \"source_directory\": \"" << default_config.source_directory << "\",\n"
                   << "  \"backup_retention\": " << default_config.backup_retention << "\n"
                   << "}";
        return default_config;
    }
    std::string line;
    Config config;
    while (std::getline(config_file, line)) {
        if (line.find("nfs_server") != std::string::npos)
            config.nfs_server = line.substr(line.find(':') + 3, line.rfind('"') - line.find(':') - 3);
        else if (line.find("nfs_share") != std::string::npos)
            config.nfs_share = line.substr(line.find(':') + 3, line.rfind('"') - line.find(':') - 3);
        else if (line.find("mount_point") != std::string::npos)
            config.mount_point = line.substr(line.find(':') + 3, line.rfind('"') - line.find(':') - 3);
        else if (line.find("source_directory") != std::string::npos)
            config.source_directory = line.substr(line.find(':') + 3, line.rfind('"') - line.find(':') - 3);
        else if (line.find("backup_retention") != std::string::npos)
            config.backup_retention = std::stoi(line.substr(line.find(':') + 2));
    }
    return config;
}

void save_config(const Config& config) {
    std::ofstream config_file("backup_config.json");
    config_file << "{\n"
                << "  \"nfs_server\": \"" << config.nfs_server << "\",\n"
                << "  \"nfs_share\": \"" << config.nfs_share << "\",\n"
                << "  \"mount_point\": \"" << config.mount_point << "\",\n"
                << "  \"source_directory\": \"" << config.source_directory << "\",\n"
                << "  \"backup_retention\": " << config.backup_retention << "\n"
                << "}";
}

void mount_nfs_share(const Config& config) {
    if (!fs::is_directory(config.mount_point)) {
        fs::create_directory(config.mount_point);
    }
    std::string command = "sudo mount -t nfs " + config.nfs_server + ":" + config.nfs_share + " " + config.mount_point;
    if (std::system(command.c_str()) == 0) {
        std::cout << "NFS share successfully mounted." << std::endl;
    } else {
        std::cerr << "Error mounting NFS share." << std::endl;
    }
}

void unmount_nfs_share(const std::string& mount_point) {
    std::string command = "sudo umount " + mount_point;
    if (std::system(command.c_str()) == 0) {
        std::cout << "NFS share successfully unmounted." << std::endl;
    } else {
        std::cerr << "Error unmounting NFS share." << std::endl;
    }
}

void create_backup(const Config& config) {
    std::string hostname = fs::path("/proc/sys/kernel/hostname").string();
    std::string host_backup_directory = config.mount_point + "/rpi/" + hostname;
    if (!fs::is_directory(host_backup_directory)) {
        fs::create_directories(host_backup_directory);
    }

    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::ostringstream ss;
    ss << std::put_time(std::localtime(&now_c), "%Y-%m-%d_%H-%M-%S");
    std::string backup_filename = host_backup_directory + "/backup_" + ss.str() + ".zip";

    int error = 0;
    zip_t* zip = zip_open(backup_filename.c_str(), ZIP_CREATE, &error);
    if (zip == nullptr) {
        std::cerr << "Error creating zip file." << std::endl;
        return;
    }

    size_t total_files = std::distance(fs::recursive_directory_iterator(config.source_directory), fs::recursive_directory_iterator{});
    std::cout << "Total files to process: " << total_files << std::endl;
    std::cout << "Progress: " << std::endl;

    for (const auto& entry : fs::recursive_directory_iterator(config.source_directory)) {
        if (entry.is_regular_file()) {
            std::string relative_path = fs::relative(entry.path(), config.source_directory).string();
            zip_source_t* source = zip_source_file(zip, entry.path().c_str(), 0, 0);
            if (source == nullptr) {
                std::cerr << "Error adding file to zip: " << entry.path() << std::endl;
                continue;
            }
            zip_file_add(zip, relative_path.c_str(), source, ZIP_FL_ENC_UTF_8);
            std::cout << '.'; std::cout.flush();
        }
    }
    zip_close(zip);
    std::cout << "Backup successfully created: " << backup_filename << std::endl;
}

void restore_backup(const Config& config) {
    std::string hostname = fs::path("/proc/sys/kernel/hostname").string();
    std::string host_backup_directory = config.mount_point + "/rpi/" + hostname;

    std::vector<std::string> backups;
    for (const auto& entry : fs::directory_iterator(host_backup_directory)) {
        if (entry.path().extension() == ".zip") {
            backups.push_back(entry.path().filename().string());
        }
    }
    if (backups.empty()) {
        std::cerr << "No backups available." << std::endl;
        return;
    }

    std::cout << "Available Backups:" << std::endl;
    for (size_t i = 0; i < backups.size(); ++i) {
        std::cout << i + 1 << ": " << backups[i] << std::endl;
    }
    std::cout << backups.size() + 1 << ": Back to main menu" << std::endl;

    size_t choice;
    std::cin >> choice;
    if (choice == backups.size() + 1) {
        return;
    }

    std::string restore_file = host_backup_directory + "/" + backups[choice - 1];
    std::string restore_target = fs::path("~/restored_data").string();
    if (!fs::is_directory(restore_target)) {
        fs::create_directory(restore_target);
    }

    int error = 0;
    zip_t* zip = zip_open(restore_file.c_str(), 0, &error);
    if (zip == nullptr) {
        std::cerr << "Error opening zip file." << std::endl;
        return;
    }

    size_t num_files = zip_get_num_entries(zip, 0);

    for (size_t i = 0; i < num_files; ++i) {
        const char* name = zip_get_name(zip, i, 0);
        if (name == nullptr) {
            continue;
        }
        std::string full_path = restore_target + "/" + name;
        fs::create_directories(fs::path(full_path).parent_path());

        zip_file_t* file = zip_fopen_index(zip, i, 0);
        std::ofstream out(full_path, std::ios::binary);
        char buffer[4096];
        zip_int64_t bytes_read;
        while ((bytes_read = zip_fread(file, buffer, sizeof(buffer))) > 0) {
            out.write(buffer, bytes_read);
        }
        zip_fclose(file);
    }
    zip_close(zip);
    std::cout << "Successfully restored from: " << restore_file << " to " << restore_target << std::endl;
}

int main() {
    Config config = load_config();
    mount_nfs_share(config);
    
    int choice;
    while (true) {
        std::cout << "\nBackup and Restore Tool" << std::endl;
        std::cout << "1: Create new backup" << std::endl;
        std::cout << "2: Restore backup" << std::endl;
        std::cout << "3: Exit" << std::endl;
        std::cout << "Choose an option: ";
        std::cin >> choice;
        if (choice == 1) {
            create_backup(config);
        } else if (choice == 2) {
            restore_backup(config);
        } else if (choice == 3) {
            break;
        } else {
            std::cerr << "Invalid selection." << std::endl;
        }
    }
    unmount_nfs_share(config.mount_point);
    return 0;
}
