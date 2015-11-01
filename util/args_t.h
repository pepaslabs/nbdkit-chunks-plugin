// thanks to http://stackoverflow.com/a/24479532
// see also http://www.gnu.org/software/libc/manual/html_node/Argp.html
// see also http://crasseux.com/books/ctutorial/Processing-command-line-options.html
// based on the example at http://crasseux.com/books/ctutorial/argp-example.html

#ifndef ARGS_T_H
#define ARGS_T_H

/* This structure is used by main to communicate with parse_opt. */
struct _args_t
{
    char *directory;    /* DIRECTORY */
    char *size;         /* Argument for -s */
    char *chunk_size;   /* Argument for -c */
};
typedef struct _args_t args_t;

#endif // ARGS_T_H
