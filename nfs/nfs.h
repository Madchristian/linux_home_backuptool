#ifndef NFS_H
#define NFS_H

#include "config.h"

void mount_nfs_share(const Config& config);
void unmount_nfs_share(const std::string& mount_point);

#endif
