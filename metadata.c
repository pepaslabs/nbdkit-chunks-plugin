#include "metadata.h"
#include "chunks_math.h"

void metadata_v0_init(metadata_t *inout)
{
    inout->v0.magic = CHUNKS_METADATA_MAGIC;
    inout->v0.metadata_version = 1;
}

bool metadata_dev_size_is_sane(metadata_t *metadata)
{
    return is_divisible_by(metadata->v1.dev_size, metadata->v1.chunk_size);
}

bool metadata_chunk_size_is_sane(metadata_t *metadata)
{
    return is_power_of_two(metadata->v1.chunk_size);
}

bool metadata_chunks_per_subdir_is_sane(metadata_t *metadata)
{
    return is_power_of_ten(metadata->v1.chunks_per_subdir)
        || metadata->v1.chunks_per_subdir == 0;
}
