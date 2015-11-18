/* chunks_math.h
 * See https://github.com/pepaslabs/nbdkit-chunks-plugin
 * Copyright (C) 2015 Jason Pepas.
 * Released under the terms of the MIT license.  See https://opensource.org/licenses/MIT
 */

#ifndef CHUNKS_MATH_H
#define CHUNKS_MATH_H

#include <stdbool.h>
#include <stdint.h>

bool is_divisible_by(uint64_t x, uint64_t y);
bool is_power_of_two(uint64_t x);
bool is_power_of_ten(uint64_t x);

#endif // CHUNKS_MATH_H
