/* chunks_can_write.h
 * See https://github.com/pepaslabs/nbdkit-chunks-plugin
 * Copyright (C) 2015 Jason Pepas.
 * Released under the terms of the MIT license.  See https://opensource.org/licenses/MIT
 */

// see also https://github.com/libguestfs/nbdkit/blob/master/docs/nbdkit-plugin.pod

#ifndef CHUNKS_CAN_WRITE_H
#define CHUNKS_CAN_WRITE_H

int chunks_can_write(void *passed_handle);

#endif // CHUNKS_CAN_WRITE_H