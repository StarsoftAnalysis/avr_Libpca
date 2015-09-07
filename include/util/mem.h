#ifndef MEM_H_MEXYVT5O
#define MEM_H_MEXYVT5O

/* Copyright (C)
 * 2015 - Tomasz Wisniewski
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 */


/**
 * @brief exchange value between variables
 *
 * This implementation doesn't use a temporary variable
 *
 * @param __x first value
 * @param __y second value
 *
 */
#define UTIL_MEM_BYTE_SWAP(__x, __y) \
	__x ^= __y; __y ^= __x; __x ^= __y


/**
 * @brief small macro to zero a memory buffer
 *
 * It produces less code than memset()
 *
 * @param __ptr pointer to buffer
 * @param __len mutable counter initialized to the size of the buffer
 *
 */
#define UTIL_MEM_ZERO(__ptr, __len) while (__len--) \
    (*(char *)(__ptr)) = 0x00


#endif /* end of include guard: MEM_H_MEXYVT5O */
