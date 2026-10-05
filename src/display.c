#include "display.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

static char get_type_char(mode_t mode)
{
    if (S_ISREG(mode))
        return '-';
    if (S_ISDIR(mode))
        return 'd';
    if (S_ISLNK(mode))
        return 'l';
    if (S_ISCHR(mode))
        return 'c';
    if (S_ISBLK(mode))
        return 'b';
    if (S_ISSOCK(mode))
        return 's';
    if (S_ISFIFO(mode))
        return 'p';

    return '?';
}

static void make_permissions(mode_t mode, char *perm)
{
    perm[0] = get_type_char(mode);

    perm[1] = (mode & S_IRUSR) ? 'r' : '-';
    perm[2] = (mode & S_IWUSR) ? 'w' : '-';
    perm[3] = (mode & S_IXUSR) ? 'x' : '-';

    perm[4] = (mode & S_IRGRP) ? 'r' : '-';
    perm[5] = (mode & S_IWGRP) ? 'w' : '-';
    perm[6] = (mode & S_IXGRP) ? 'x' : '-';

    perm[7] = (mode & S_IROTH) ? 'r' : '-';
    perm[8] = (mode & S_IWOTH) ? 'w' : '-';
    perm[9] = (mode & S_IXOTH) ? 'x' : '-';

    if (mode & S_ISUID)
        perm[3] = (mode & S_IXUSR) ? 's' : 'S';

    if (mode & S_ISGID)
        perm[6] = (mode & S_IXGRP) ? 's' : 'S';

    if (mode & S_ISVTX)
        perm[9] = (mode & S_IXOTH) ? 't' : 'T';

    perm[10] = '\0';
}

static void print_size(off_t size, const Options *options)
{
    if (!options->human)
    {
        printf("%lld", (long long)size);
        return;
    }

    if (size < 1024)
        printf("%lld", (long long)size);
    else if (size < 1024 * 1024)
        printf("%.1fK", (double)size / 1024.0);
    else if (size < 1024LL * 1024LL * 1024LL)
        printf("%.1fM", (double)size / (1024.0 * 1024.0));
    else
        printf("%.1fG",
               (double)size / (1024.0 * 1024.0 * 1024.0));
}

static long long get_block_size(const Options *options)
{
    const char *env;
    long long block_size;

    if (options->kilobytes)
        return 1024;

    env = getenv("BLOCKSIZE");

    if (env != NULL && env[0] != '\0')
    {
        block_size = atoll(env);

        if (block_size > 0)
            return block_size;
    }

    return 512;
}

static long long get_blocks(const FileInfo *file,
                            const Options *options)
{
    long long bytes;
    long long block_size;

    bytes = (long long)file->st.st_blocks * 512;
    block_size = get_block_size(options);

    if (block_size <= 0)
        block_size = 512;

    return (bytes + block_size - 1) / block_size;
}

static int is_terminal(void)
{
    return isatty(STDOUT_FILENO);
}

static void print_file_name(const FileInfo *file,
                            const Options *options)
{
    const unsigned char *p;

    p = (const unsigned char *)file->name;

    while (*p)
    {
        if (options->quote && (*p < 32 || *p == 127))
            putchar('?');
        else
            putchar(*p);

        p++;
    }
}

static void print_classification(const FileInfo *file)
{
    mode_t mode;

    mode = file->st.st_mode;

    if (S_ISDIR(mode))
        putchar('/');
    else if (S_ISLNK(mode))
        putchar('@');
    else if (S_ISSOCK(mode))
        putchar('=');
    else if (S_ISFIFO(mode))
        putchar('|');
    else if (S_ISREG(mode) &&
             (mode & (S_IXUSR | S_IXGRP | S_IXOTH)))
        putchar('*');
}

static void print_symlink_target(const FileInfo *file)
{
    char target[4096];
    ssize_t length;

    if (!S_ISLNK(file->st.st_mode))
        return;

    length = readlink(file->path, target, sizeof(target) - 1);

    if (length < 0)
        return;

    target[length] = '\0';

    printf(" -> %s", target);
}

static time_t get_display_time(const FileInfo *file,
                               const Options *options)
{
    if (options->use_ctime)
        return file->st.st_ctime;

    if (options->use_atime)
        return file->st.st_atime;

    return file->st.st_mtime;
}

static void print_time(const FileInfo *file,
                       const Options *options)
{
    struct tm *tm_info;
    char time_buffer[64];
    time_t display_time;

    display_time = get_display_time(file, options);

    tm_info = localtime(&display_time);

    if (tm_info == NULL)
    {
        printf(" ? ");
        return;
    }

    strftime(time_buffer, sizeof(time_buffer),
             "%b %e %H:%M", tm_info);

    printf(" %s ", time_buffer);
}

void display_file(const FileInfo *file, const Options *options)
{
    char permissions[11];
    struct passwd *pw;
    struct group *gr;

    if (options->inode)
        printf("%llu ",
               (unsigned long long)file->st.st_ino);

    if (options->blocks)
    {
        if (options->human)
        {
            print_size(
                (off_t)((long long)file->st.st_blocks * 512),
                options
            );
        }
        else
        {
            printf("%lld", get_blocks(file, options));
        }

        putchar(' ');
    }

    if (!options->long_format && !options->numeric)
    {
        print_file_name(file, options);

        if (options->classify)
            print_classification(file);

        putchar('\n');
        return;
    }

    make_permissions(file->st.st_mode, permissions);

    printf("%s %lu ",
           permissions,
           (unsigned long)file->st.st_nlink);

    if (options->numeric)
    {
        printf("%lu %lu ",
               (unsigned long)file->st.st_uid,
               (unsigned long)file->st.st_gid);
    }
    else
    {
        pw = getpwuid(file->st.st_uid);
        gr = getgrgid(file->st.st_gid);

        if (pw != NULL)
            printf("%s ", pw->pw_name);
        else
            printf("%lu ", (unsigned long)file->st.st_uid);

        if (gr != NULL)
            printf("%s ", gr->gr_name);
        else
            printf("%lu ", (unsigned long)file->st.st_gid);
    }

    print_size(file->st.st_size, options);

    print_time(file, options);

    print_file_name(file, options);

    if (options->classify)
        print_classification(file);

    print_symlink_target(file);

    putchar('\n');
}

void display_files(FileInfo *files, int count,
                   const Options *options)
{
    int i;

    if (files == NULL || options == NULL)
        return;

    for (i = 0; i < count; i++)
        display_file(&files[i], options);
}

void display_total(FileInfo *files, int count,
                   const Options *options)
{
    long long total;
    long long total_bytes;
    int i;

    if (files == NULL || count <= 0 || options == NULL)
        return;

    if (!is_terminal())
        return;

    total = 0;

    for (i = 0; i < count; i++)
        total += get_blocks(&files[i], options);

    printf("total ");

    if (options->human)
    {
        total_bytes = total * get_block_size(options);
        print_size((off_t)total_bytes, options);
    }
    else
    {
        printf("%lld", total);
    }

    putchar('\n');
}
