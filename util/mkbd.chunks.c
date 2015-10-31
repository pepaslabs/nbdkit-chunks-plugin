#include <stdlib.h>
#include "metadata_t.h"
#include "cmdline.h"

int main(int argc, char *argv[])
{
	struct gengetopt_args_info args;
	if (cmdline_parser(argc, argv, &args) != 0)
	{
    	return EXIT_FAILURE;
	}

    return EXIT_FAILURE;
}
