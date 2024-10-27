#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include "config.h"

namespace filehandler {
    void create_backup(const Config& config);
    void restore_backup(const Config& config);
    void delete_backup(const Config& config);
}

#endif
