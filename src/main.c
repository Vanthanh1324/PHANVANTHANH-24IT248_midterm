#include "options.h"
#include "fileinfo.h"
#include "sort.h"
#include "display.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

static int is_hidden(const char *name)
{
    return name[0] == '.';
}

static int load_directory(const char *path,
                          const Options *options,
                          FileInfo **files,
                          int *count)
{
    DIR *dir;
    struct dirent *entry;
    FileInfo *list;
    int capacity;

    dir = opendir(path);

    if (dir == NULL)
    {
        perror(path);
        return -1;
    }

    capacity = 16;
    *count = 0;

    list = malloc(sizeof(FileInfo) * capacity);

    if (list == NULL)
    {
        closedir(dir);
        return -1;
    }

    while ((entry = readdir(dir)) != NULL)
    {
        FileInfo info;

        if (!options->all && !options->almost_all)
        {
            if (is_hidden(entry->d_name))
                continue;
        }

        if (options->almost_all)
        {
            if (strcmp(entry->d_name, ".") == 0 ||
                strcmp(entry->d_name, "..") == 0)
                continue;
        }

        if (*count >= capacity)
        {
            FileInfo *new_list;

            capacity *= 2;

            new_list = realloc(list,
                               sizeof(FileInfo) * capacity);

            if (new_list == NULL)
            {
                int i;

                for (i = 0; i < *count; i++)
                    fileinfo_free(&list[i]);

                free(list);
                closedir(dir);

                return -1;
            }

            list = new_list;
        }

        if (fileinfo_load(&info,
                          path,
                          entry->d_name) == -1)
        {
            fprintf(stderr,
                    "ls: cannot access %s/%s\n",
                    path,
                    entry->d_name);

            continue;
        }

        list[*count] = info;
        (*count)++;
    }

    closedir(dir);

    *files = list;

    return 0;
}

static int display_path(const char *path,
                        const Options *options,
                        int recursive_level,
                        int show_header)
{
    struct stat st;
    FileInfo file;
    FileInfo *files;
    int count;
    int result;
    int i;

    if (lstat(path, &st) == -1)
    {
        perror(path);
        return 1;
    }

    /*
     * -d: hien thi thu muc nhu mot file.
     */
    if (options->directory)
    {
        file.name = strdup(path);
        file.path = strdup(path);

        if (file.name == NULL || file.path == NULL)
        {
            free(file.name);
            free(file.path);
            return 1;
        }

        file.st = st;

        display_file(&file, options);

        fileinfo_free(&file);

        return 0;
    }

    /*
     * Neu la file thuong thi hien thi truc tiep.
     */
    if (!S_ISDIR(st.st_mode))
    {
        file.name = strdup(path);
        file.path = strdup(path);

        if (file.name == NULL || file.path == NULL)
        {
            free(file.name);
            free(file.path);
            return 1;
        }

        file.st = st;

        display_file(&file, options);

        fileinfo_free(&file);

        return 0;
    }

    /*
     * Doc noi dung thu muc.
     */
    result = load_directory(path,
                            options,
                            &files,
                            &count);

    if (result != 0)
        return 1;

    /*
     * Sap xep noi dung thu muc.
     */
    sort_files(files, count, options);

    /*
     * In ten thu muc khi co nhieu operand
     * hoac dang de quy.
     */
    if (show_header || recursive_level > 0)
    {
        if (recursive_level > 0)
            printf("\n%s:\n", path);
        else
            printf("%s:\n", path);
    }

    /*
     * Hien thi noi dung thu muc.
     */
    if (options->blocks || options->long_format)
        display_total(files, count, options);
    display_files(files, count, options);

    /*
     * De quy.
     */
    if (options->recursive)
    {
        for (i = 0; i < count; i++)
        {
            if (!S_ISDIR(files[i].st.st_mode))
                continue;

            if (strcmp(files[i].name, ".") == 0 ||
                strcmp(files[i].name, "..") == 0)
                continue;

            result = display_path(files[i].path,
                                  options,
                                  recursive_level + 1,
                                  0);

            if (result != 0)
                result = 1;
        }
    }

    for (i = 0; i < count; i++)
        fileinfo_free(&files[i]);

    free(files);

    return 0;
}

static int is_directory_operand(const char *path)
{
    struct stat st;

    if (lstat(path, &st) == -1)
        return 0;

    return S_ISDIR(st.st_mode);
}

static const Options *operand_options;

static int compare_operand_names(const void *a,
                                 const void *b)
{
    const char * const *name_a;
    const char * const *name_b;
    int result;

    name_a = (const char * const *)a;
    name_b = (const char * const *)b;

    result = strcmp(*name_a, *name_b);

    if (operand_options->reverse)
        result = -result;

    return result;
}
int main(int argc, char **argv)
{
    Options options;
    operand_options = &options;
    int first_file;
    int file_count;
    int dir_count;
    int i;
    int result;
    char **file_operands;
    char **dir_operands;

    options_init(&options);

    if (options_parse(&options,
                      argc,
                      argv,
                      &first_file) != 0)
    {
        fprintf(stderr, "ls: invalid option\n");
        return 1;
    }

    /*
     * Khong co operand.
     */
    if (first_file >= argc)
    {
        return display_path(".",
                            &options,
                            0,
                            0);
    }

    /*
     * Neu -d thi tat ca operand duoc xem nhu file.
     */
    result = 0;
    if (options.directory)
    {
        for (i = first_file; i < argc; i++)
        {
            if (display_path(argv[i],
                             &options,
                             0,
                             0) != 0)
            {
                result = 1;
            }
        }

        return result;
    }

    file_count = 0;
    dir_count = 0;

    /*
     * Dem file va thu muc.
     */
    for (i = first_file; i < argc; i++)
    {
        if (is_directory_operand(argv[i]))
            dir_count++;
        else
            file_count++;
    }

    file_operands = NULL;
    dir_operands = NULL;

    if (file_count > 0)
    {
        file_operands = malloc(sizeof(char *) * file_count);

        if (file_operands == NULL)
            return 1;
    }

    if (dir_count > 0)
    {
        dir_operands = malloc(sizeof(char *) * dir_count);

        if (dir_operands == NULL)
        {
            free(file_operands);
            return 1;
        }
    }

    file_count = 0;
    dir_count = 0;

    /*
     * Tach file va directory.
     */
    for (i = first_file; i < argc; i++)
    {
        if (is_directory_operand(argv[i]))
        {
            dir_operands[dir_count] = argv[i];
            dir_count++;
        }
        else
        {
            file_operands[file_count] = argv[i];
            file_count++;
        }
    }

    /*

     * Sap xep file operand.
     */
    if (!options.unsorted && file_count > 1)
    {
        qsort(file_operands,
              file_count,
              sizeof(char *),
              compare_operand_names);
    }

    /*
     * Sap xep directory operand.
     */
    if (!options.unsorted && dir_count > 1)
    {
        qsort(dir_operands,
              dir_count,
              sizeof(char *),
              compare_operand_names);
    }

    result = 0;

    /*
     * File hien thi truoc.
     */
    for (i = 0; i < file_count; i++)
    {
        if (display_path(file_operands[i],
                         &options,
                         0,
                         0) != 0)
        {
            result = 1;
        }
    }

    /*
     * Sau do hien thi directory.
     */
    for (i = 0; i < dir_count; i++)
    {
        if (display_path(dir_operands[i],
                         &options,
                         0,
                         dir_count > 1 || file_count > 0) != 0)
        {
            result = 1;
        }
    }

    free(file_operands);
    free(dir_operands);

    return result;
}
