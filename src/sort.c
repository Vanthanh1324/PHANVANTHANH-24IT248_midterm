#include "sort.h"
#include <string.h>
#include <stdlib.h>

static const Options *sort_options;

static int compare_name(const FileInfo *a, const FileInfo *b)
{
    return strcmp(a->name, b->name);
}

static time_t get_sort_time(const FileInfo *file)
{
    if (sort_options->use_ctime)
        return file->st.st_ctime;

    if (sort_options->use_atime)
        return file->st.st_atime;

    return file->st.st_mtime;
}

static int compare_files(const void *pa, const void *pb)
{
    const FileInfo *a;
    const FileInfo *b;
    int result;

    a = (const FileInfo *)pa;
    b = (const FileInfo *)pb;

    if (sort_options->sort_time)
    {
        time_t ta;
        time_t tb;

        ta = get_sort_time(a);
        tb = get_sort_time(b);

        if (ta > tb)
            result = -1;
        else if (ta < tb)
            result = 1;
        else
            result = compare_name(a, b);
    }
    else if (sort_options->sort_size)
    {
        if (a->st.st_size > b->st.st_size)
            result = -1;
        else if (a->st.st_size < b->st.st_size)
            result = 1;
        else
            result = compare_name(a, b);
    }
    else
    {
        result = compare_name(a, b);
    }

    if (sort_options->reverse)
        result = -result;

    return result;
}

int sort_files(FileInfo *files, int count, const Options *options)
{
    if (files == NULL || count <= 1 || options == NULL)
        return 0;

    if (options->unsorted)
        return 0;

    sort_options = options;

    qsort(files, count, sizeof(FileInfo), compare_files);

    return 0;
}
