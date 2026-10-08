#include "options.h"
#include <unistd.h>

void options_init(Options *options)
{
    options->all = 0;
    options->almost_all = (geteuid() == 0);
    options->use_ctime = 0;
    options->directory = 0;
    options->classify = 0;
    options->unsorted = 0;
    options->human = 0;
    options->inode = 0;
    options->kilobytes = 0;
    options->long_format = 0;
    options->numeric = 0;

    /*
     * NetBSD ls: -q is the default for terminals,
     * while raw output is the default for non-terminals.
     */
    options->quote = isatty(STDOUT_FILENO);
    options->recursive = 0;
    options->reverse = 0;
    options->sort_size = 0;
    options->blocks = 0;
    options->sort_time = 0;
    options->use_atime = 0;
    options->raw = !options->quote;
}

int options_parse(Options *options, int argc, char **argv,
                  int *first_file)
{
    int i;
    int j;
    char c;

    i = 1;

    while (i < argc)
    {
        if (argv[i][0] != '-')
            break;

        if (argv[i][1] == '\0')
            break;

        if (argv[i][1] == '-' && argv[i][2] == '\0')
        {
            i++;
            break;
        }

        j = 1;

        while (argv[i][j] != '\0')
        {
            c = argv[i][j];

            if (c == 'a')
                options->all = 1;
            else if (c == 'A')
                options->almost_all = 1;
            else if (c == 'c')
            {
                options->use_ctime = 1;
                options->use_atime = 0;
            }
            else if (c == 'd')
            {
                options->directory = 1;
                options->recursive = 0;
            }
            else if (c == 'F')
                options->classify = 1;
            else if (c == 'f')
            {
                options->unsorted = 1;
                options->all = 1;
            }
            else if (c == 'h')
            {
                options->human = 1;
                options->kilobytes = 0;
            }
            else if (c == 'i')
                options->inode = 1;
            else if (c == 'k')
            {
                options->kilobytes = 1;
                options->human = 0;
            }
            else if (c == 'l')
                options->long_format = 1;
            else if (c == 'n')
            {
                options->numeric = 1;
                options->long_format = 1;
            }
            else if (c == 'q')
            {
                options->quote = 1;
                options->raw = 0;
            }
            else if (c == 'R')
            {
                options->recursive = 1;
                options->directory = 0;
            }
            else if (c == 'r')
                options->reverse = 1;
            else if (c == 'S')
            {
                options->sort_size = 1;
                options->sort_time = 0;
            }
            else if (c == 's')
                options->blocks = 1;
            else if (c == 't')
            {
                options->sort_time = 1;
                options->sort_size = 0;
            }
            else if (c == 'u')
            {
                options->use_atime = 1;
                options->use_ctime = 0;
            }
            else if (c == 'w')
            {
                options->raw = 1;
                options->quote = 0;
            }
            else
                return -1;

            j++;
        }

        i++;
    }

    *first_file = i;

    return 0;
}
