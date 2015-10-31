// thanks to http://stackoverflow.com/a/24479532
// see also http://www.gnu.org/software/libc/manual/html_node/Argp.html
// see also http://crasseux.com/books/ctutorial/Processing-command-line-options.html

#include <argp.h>
#include <stdbool.h>

const char *argp_program_version = "mkbd.chunks 0.1";

static char doc[] = "\n\
Creates a chunked block device directory.\n\
See https://github.com/pepaslabs/nbdkit-chunks-plugin\n\
\n\
The following size suffixes from GNU dd are honored (e.g. --size 64G):\n\
kB,KB=1000, k,K=1024, MB=1000000, M=1048576, G, GB, T, and TB.\n\
\n\
Examples:\n\
\n\
Create an 8G block device in chunks of 256k in the current directory:\n\
mkbd.chunks .\n\
\n\
Create a 100G block device in chunks of 1M in /srv/nbds/mydisk:\n\
mkbd.chunks -s 100G -c 1M /srv/nbds/mydisk\n\
";

static char args_doc[] = "DIRECTORY";

static struct argp_option options[] = {
    { "size", 's', 0, 0, "Size of the block device in bytes (default 8G)."},
    { "chunk-size", 'c', 0, 0, "Size of each chunk in bytes (default 256k)."},
    { 0 } 
};