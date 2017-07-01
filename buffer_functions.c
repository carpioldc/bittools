/*****************************************************************************
*
* Copyright (C) 2017 Jorge Carpio
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*
*****************************************************************************/

#include "buffer_functions.h"

/**
 * Function: bit_lshift
 * 	Shift buffer one bit to the left
 *
 * 	buffer: array of elements whose bits are to be shifted. Each element
 * 		is considered to be at the left of the following one.
 * 	len: length of the buffer
 */ 	
void bit_lshift( uint8_t *buffer, int len)
{
	uint8_t c, j; /* 'c' stands for carry */
	c = 0;

	for(j = len - 1; j < len; j --)
	{
		if(buffer[j] & 0x80)
			c = c | 1;

		buffer[j] = (buffer[j] << 1) | (c > 1);
		c = (c & 1) << 1;
	}
}

/**
 * Function: byte_lshift
 * 	Shift a buffer one byte to the left
 *
 * 	buffer: array of elements whose bits are to be shifted. Each element
 * 		is considered to be at the left of the following one.
 * 	len: length of the buffer
 */
void byte_lshift( uint8_t *buffer, int len)
{
	uint8_t i;
	for(i=0; i<len-1; i++) /* loop n bit shifts */
		buffer[i] = buffer[i+1];
	buffer[len-1] = 0x00;
}


/**
 * Function: bincmp
 * 	Binary comparison of two byte array
 *	
 *	tx: byte to be matched
 *	rx: byte to match against tx
 *	nb: number of bytes to match
 *
 *	Returns: number of differing bits
 */
uint8_t bincmp( uint8_t *tx, uint8_t *rx, uint8_t nb)
{
	/* TODO: enable comparison at bit level (nb)*/
	uint8_t j, diff;
	diff = 0;

	for(j = 0; j < nb; j ++) /* loop bytes */
	{
		if(rx[j] != tx[j] )
			diff += _byteones(rx[j] ^ tx[j]);
	}
	return diff;
}

/**
 * Function: binprint
 * 	Dump buffer content to stdout (hex format)
 *
 * 	buffer: array of bytes
 * 	size: number of bytes to print
 */
void binprint( uint8_t *buffer, size_t size)
{
	/* static int line = 0; */
	int j;
	/* printf("[ %d ] ", line); */
	for(j = 0; j < size; j++)
		printf("0x%x ", buffer[j]);

	printf("\n");
	/* line ++; */
}

/**
 * Funcion: _byteones
 * 	Returns the number of bits set to one in c.
 */ 
uint8_t _byteones( uint8_t c)
{
	uint8_t diff;
	diff = 0;
	if (c & 0x01) diff ++;
	if (c & 0x02) diff ++;
	if (c & 0x04) diff ++;
	if (c & 0x08) diff ++;
	if (c & 0x10) diff ++;
	if (c & 0x20) diff ++;
	if (c & 0x40) diff ++;
	if (c & 0x80) diff ++;
	return diff;
}
