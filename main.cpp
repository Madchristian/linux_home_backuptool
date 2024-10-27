#include "config.h"
#include "filehandler.h"
#include "nfs.h"
#include <iostream>

int main() {
    Config config = load_config();
    mount_nfs_share(config);
    
    int choice;
    while (true) {
        std::cout << "\nBackup and Restore Tool" << std::endl;
        std::cout << "1: Create new backup" << std::endl;
        std::cout << "2: Restore backup" << std::endl;
        std::cout << "3: Edit configuration" << std::endl;
        std::cout << "4: Delete backup" << std::endl;
        std::cout << "5: Exit" << std::endl;
        std::cout << "Choose an option: ";
        std::cin >> choice;
        if (choice == 1) {
            create_backup(config);
        } else if (choice == 2) {
            restore_backup(config);
        } else if (choice == 3) {
            edit_config(config);
        } else if (choice == 4) {
            delete_backup(config);
        } else if (choice == 5) {
            break;
        } else {
            std::cerr << "Invalid selection." << std::endl;
        }
    }
    unmount_nfs_share(config.mount_point);
    return 0;
}
