/*****************************************************************************
* patfinder.c:
* Performs bit-oriented pattern matching
* Writes output to patfound.txt
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
#define TX_FILE "/gnuradio/etc/texto"
#define WR_FILE "/tmp/patfound"
#define N_COMPARE 3
#define FOUND 0x01
#define POSSIBLE 0x02
#define ERROR 0x04
#define END_OF_FIRST_ZEROS 0x8


int main(int argc, char **argv)
{
	uint8_t i, j, offset, flags, ncmp, diff;
	uint8_t *buffer, *origin;
	char *rx_file, *tx_file, *out_file;
	FILE *fp;
	FILE *gp;
	
	switch (argc)
	{
		case 4: 
			tx_file = argv[argc-2];
			ncmp = atoi(argv[argc-3]) + 1;
			break;
		case 3:	
			tx_file = argv[argc-2];
			ncmp = N_COMPARE + 1;
			break;
		case 2:
			tx_file = TX_FILE;
			ncmp = N_COMPARE + 1;
			break;
		default:
			printf("Usage: patfinder [ N ] [ pattern_file ] file_to_match\n"
					"\tN: number of bytes to match at the beginning\n"
					"\tpattern_file: original from which to obtain the pattern\n"
					"\tfile_to_match: received file, where the pattern must be found\n\n");
			return EXIT_FAILURE;
	}

	rx_file = argv[argc-1];
	out_file = WR_FILE;
	buffer = (uint8_t*)malloc(ncmp);
	origin = (uint8_t*)malloc(ncmp); 
	fp = fopen(rx_file, "rb"); 
	gp = fopen(tx_file, "rb"); 
	if (fp == NULL || gp == NULL)
	{
		perror("Error opening files");
		free(buffer);
		free(origin);
		return EXIT_FAILURE;
	}
	
	/* initialize buffer and read pattern into origin */
	for (j = 0; j < ncmp; j ++)
	{
		buffer[j] = 0x00;
		if (fread(origin+j, sizeof(uint8_t), 1, gp) != 1)
		{
			perror("Error reading pattern from file");
			free(buffer);
			free(origin);
			fclose(gp);
			fclose(fp);
			return EXIT_FAILURE;
		}
	}

	fclose(gp);

	printf("\nFile: %s. Pattern to match:\n\t'%s'\n\t" , tx_file, origin);
	binprint(origin, ncmp);

	gp = fopen(out_file, "w+"); 
	if (gp == NULL)
	{
		perror("Error opening file");
		free(buffer);
		free(origin);
		fclose(fp);
		return EXIT_FAILURE;
	}

	offset = 8;
	flags = 0x00;
	printf("Searching %s\n", rx_file);


	do /* Main loop: while not end of file */
	{
		if (!(flags & FOUND)){
			printf("\rTrying char %lu... ", ftell(fp));
			/* binprint(buffer, ncmp); */
			fflush(stdout);
		}

		/* read byte and append to buffer */
		if (((fread(buffer + ncmp - 1, sizeof(uint8_t), 1, fp)) & 0xFE ) != 0)
		{
			perror("Error reading from file");
			return EXIT_FAILURE;
		}
		

		if (!(flags & END_OF_FIRST_ZEROS) && buffer[ncmp-1])
		{
			flags |= END_OF_FIRST_ZEROS;
		}
		
		if (flags & END_OF_FIRST_ZEROS)
		{
			/* Shift 8 bits while still searching for the pattern.
			 * If the pattern was found, we still need to shift `offset' bits here.
			 */
			for (i = 0; i < offset; i ++) 
			{
				if (!(flags & FOUND)) /* try to match */
				{
					diff = bincmp(buffer, origin, ncmp-1);
					if (diff == 0)
					{
						printf("success (bit offset = %d)\nText written to %s:\n\t", i, out_file);
						offset = i;
						flags |= FOUND;
						break;
					}
					/* TODO if (diff < thresh) */
				}
				bit_lshift(buffer, ncmp);
			}
			if (flags & FOUND)
			{
				printf("%c", (char)buffer[0]);

				if (fwrite(buffer, sizeof(uint8_t), 1, gp) != 1)
				{
					perror("Error writing to file");
					return EXIT_FAILURE;
				}

				for (i = offset; i < 8; i ++)
					bit_lshift(buffer, ncmp);
			}
		}
			

		/* Catch file errors */
		if (ferror(fp)) {
			flags  |= ERROR;
			perror("Errorr; ");
		}
	}
	while(!feof(fp));

	/* End of main loop*/	

	/* Fush buffer */
	if (fwrite(buffer, sizeof(uint8_t), ncmp, gp) != ncmp)
	{
		perror("Error writing the remains");
		return EXIT_FAILURE;
	}
	printf("%s", buffer);
	printf("\n");
	if (fflush(fp) == EOF)
		flags |= ERROR;
	fclose(fp);
	fclose(gp);
	free(buffer);
	free(origin);
	if(flags & ERROR)
		return EXIT_FAILURE;
	else
		return EXIT_SUCCESS;
}
