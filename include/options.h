#ifndef OPTIONS_H
#define OPTIONS_H

typedef struct {
    int all;          /* -a */
    int almost_all;   /* -A */
    int use_ctime;    /* -c */
    int directory;    /* -d */
    int classify;     /* -F */
    int unsorted;     /* -f */
    int human;        /* -h */
    int inode;        /* -i */
    int kilobytes;    /* -k */
    int long_format;  /* -l */
    int numeric;      /* -n */
    int quote;        /* -q */
    int recursive;    /* -R */
    int reverse;      /* -r */
    int sort_size;    /* -S */
    int blocks;       /* -s */
    int sort_time;    /* -t */
    int use_atime;    /* -u */
    int raw;          /* -w */
} Options;

void options_init(Options *options);
int options_parse(Options *options, int argc, char **argv, int *first_file);

#endif
