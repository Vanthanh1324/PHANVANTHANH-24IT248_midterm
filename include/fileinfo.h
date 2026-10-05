#ifndef FILEINFO_H
#define FILEINFO_H

#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>

typedef struct {
    char *name;
    char *path;
    struct stat st;
} FileInfo;

int fileinfo_load(FileInfo *info, const char *path, const char *name);
void fileinfo_free(FileInfo *info);

#endif
