#include "metadata.h"

void metadata_init_v0(metadata_t *inout)
{
    inout->v0.magic = CHUNKS_METADATA_MAGIC;
    inout->v0.metadata_version = 1;
}