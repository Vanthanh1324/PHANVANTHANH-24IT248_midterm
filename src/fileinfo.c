#include "fileinfo.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

int fileinfo_load(FileInfo *info, const char *path, const char *name)
{
    size_t path_len;

    if (info == NULL || path == NULL || name == NULL)
        return -1;

    info->name = NULL;
    info->path = NULL;

    info->name = strdup(name);
    if (info->name == NULL)
        return -1;

    path_len = strlen(path) + 1 + strlen(name) + 1;

    info->path = malloc(path_len);
    if (info->path == NULL)
    {
        free(info->name);
        info->name = NULL;
        return -1;
    }

    if (strcmp(path, "/") == 0)
        snprintf(info->path, path_len, "/%s", name);
    else
        snprintf(info->path, path_len, "%s/%s", path, name);

    if (lstat(info->path, &info->st) == -1)
    {
        fileinfo_free(info);
        return -1;
    }

    return 0;
}

void fileinfo_free(FileInfo *info)
{
    if (info == NULL)
        return;

    free(info->name);
    free(info->path);

    info->name = NULL;
    info->path = NULL;
}
