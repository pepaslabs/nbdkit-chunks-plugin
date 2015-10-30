/* chunks_math.h
 * See https://github.com/pepaslabs/nbdkit-chunks-plugin
 * Copyright (C) 2015 Jason Pepas.
 * Released under the terms of the MIT license.  See https://opensource.org/licenses/MIT
 */

#include <stdbool.h>
#include <stdint.h>

bool is_divisible_by(uint64_t x, uint64_t y);
bool is_power_of_two(uint64_t x);