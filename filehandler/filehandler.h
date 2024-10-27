#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include "config.h"

void create_backup(const Config& config);
void restore_backup(const Config& config);
void delete_backup(const Config& config);

#endif
