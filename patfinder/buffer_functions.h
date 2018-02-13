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

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

/* Change the uint32_t cast if you change this macro*/
#define BUFF_SIZE 4

void bit_lshift( uint8_t *buffer, int len);
void byte_lshift( uint8_t *buffer, int len);
uint8_t bincmp( uint8_t *tx, uint8_t *rx, uint8_t nb);
void binprint( uint8_t *buffer, size_t size);
uint8_t _byteones( uint8_t c);
