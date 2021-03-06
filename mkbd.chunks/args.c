// thanks to http://stackoverflow.com/a/24479532
// see also http://www.gnu.org/software/libc/manual/html_node/Argp.html
// see also http://crasseux.com/books/ctutorial/Processing-command-line-options.html
// based on the example at http://crasseux.com/books/ctutorial/argp-example.html

#include "args.h"
#include "args_t.h"

#include <argp.h>
#include <stdbool.h>

const char *argp_program_version = "mkbd.chunks 0.1";

/*
   OPTIONS.  Field 1 in ARGP.
   Order of fields: {NAME, KEY, ARG, FLAGS, DOC}.
*/
static struct argp_option options[] =
{
    { "size", 's', "BYTES", 0, "Size of the block device in bytes (must be divisible by chunk size) (default 8G)." },
    { "chunk-size", 'c', "BYTES", 0, "Size of each chunk in bytes (must be a power of 2) (default 256k)." },
    { "chunks-per-subdir", 'd', "COUNT", 0, "Number of chunks per subdirectory (must be a power of 10, or zero to disable subdirectories) (default 1000)." },
    { 0 } 
};

/*
   PARSER. Field 2 in ARGP.
   Order of parameters: KEY, ARG, STATE.
*/
static error_t parse_opt(int key, char *arg, struct argp_state *state)
{
    args_t *args = state->input;

    switch (key)
    {
        case 's':
            args->size_str = arg;
            break;

        case 'c':
            args->chunk_size_str = arg;
            break;

        case 'd':
            args->chunks_per_subdir_str = arg;
            break;

        case ARGP_KEY_ARG:
            if (state->arg_num >= 1)
            {
                argp_usage(state);
            }
            args->directory = arg;
            break;

        case ARGP_KEY_END:
            if (state->arg_num < 1)
            {
                argp_usage(state);
            }
            break;

        default:
            return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

/*
   ARGS_DOC. Field 3 in ARGP.
   A description of the non-option command-line arguments that we accept.
*/
static char args_doc[] = "DIRECTORY";

/*
  DOC.  Field 4 in ARGP.
  Program documentation.
*/
static char doc[] = "\n\
Creates a chunked block device directory.\n\
\v\
The following size suffixes from GNU dd are honored (e.g. --size 64G):\n\
kB,KB=1000, k,K=1024, MB=1000000, M=1048576, G, GB, T, and TB.\n\
\n\
Example: Create a 100G block device in chunks of 1M in '/srv/nbds/mydisk/':\n\
\n\
    mkbd.chunks -s 100G -c 1M /srv/nbds/mydisk\n\
\n\
See https://github.com/pepaslabs/nbdkit-chunks-plugin";

/*
   The ARGP structure itself.
*/
static struct argp argp = {options, parse_opt, args_doc, doc};

extern args_t args;

void parse_args(int argc, char *argv[])
{
    // defaults
    args.size_str = "8G";
    args.chunk_size_str = "256k";
    args.chunks_per_subdir_str = "1000";

    argp_parse(&argp, argc, argv, 0, 0, &args);
}
