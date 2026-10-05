#ifndef DISPLAY_H
#define DISPLAY_H

#include "fileinfo.h"
#include "options.h"

void display_file(const FileInfo *file, const Options *options);

void display_files(FileInfo *files, int count, const Options *options);

void display_total(FileInfo *files, int count,
                   const Options *options);

#endif
