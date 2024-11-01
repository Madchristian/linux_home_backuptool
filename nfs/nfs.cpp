#include "nfs.h"
#include <iostream>
#include <cstdlib>
#include <filesystem>

namespace nfs {
    void mount_nfs_share(const Config& config) {
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
}
