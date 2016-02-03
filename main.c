/*
	gb-convert by siObyte
	-github.com/siobyte
	-siobyte.xyz

	example usage:
	"./gb-convert -tiles mytiles.png -map mymap.png >> output.txt"

	TODO:
	-better image size support
*/

#define STB_IMAGE_IMPLEMENTATION
#include <inc/stb_image.h>
#include <stdio.h>
#include <stdlib.h>

void ConvertTiles(const char *file);
void ConvertMap(const char *file);

int tilecount = 0;
int bytecount = 0;
unsigned short int tiledata[192*16];
unsigned short int mapdata[32*32];

int main(int argc, char *argv[])
{
	int i;
	for (int i=0; i<argc; i++) {
		if (strcmp("-tiles", argv[i]) == 0)
			ConvertTiles(argv[i+1]);
		else if (strcmp("-map", argv[i]) == 0)
			ConvertMap(argv[i+1]);
	}

	return 1;
}

void ConvertTiles(const char *file)
{
	int n, w, h;
	unsigned char *data = stbi_load(file, &w, &h, &n, 4);

	if (data == NULL) {
		printf("Failed to load file %s\n", file);	
		return;	
	}

	/* 8x8 pixels, 2 bits per pixel, 2 bytes per line */
	int i, x, y;
	tilecount = floor(w/8);
	for (int i=0; i<tilecount; i++) {
		for (int y=0; y<8; y++) {
			int shift = 7;
			for (int x=0; x<8; x++) {
				int index = 4 * (((y * w) + x) + (i*8));
				unsigned short int cbit1  = 0b00000001;
				unsigned short int cbit2  = 0b00000001;

				if (data[index] > 60 && data[index] < 120) {
					cbit1  = 0b00000000;
					cbit2  = 0b00000001;
				} else if (data[index] > 119 && data[index] < 180) {
					cbit1  = 0b00000001;
					cbit2  = 0b00000000;
				} else if (data[index] > 179) {
					cbit1  = 0b00000000;
					cbit2  = 0b00000000;
				}
				
				tiledata[bytecount]   |= (cbit1 << shift);
				tiledata[bytecount+1] |= (cbit2 << shift);
				shift--;
			}
			bytecount+=2;
		}
	}

	/* print out the bytes */
	int c;
	printf("TILE_DATA: \n");
	for (int i=0; i<bytecount; i++) {
		if (c == 0)
			printf("\nDB ");

		printf("$");
		
		if(tiledata[i] < 0x10)
			printf("0");
		
		printf("%x", tiledata[i]);

		if (c < 15)
			printf(",");	
		
		c++;
		
		if (c > 15)
			c = 0;
	}

	printf("\n\n");
}

void ConvertMap(const char *file)
{
	int n, w, h;
	unsigned char *data = stbi_load(file, &w, &h, &n, 4);

	if (data == NULL) {
		printf("Failed to load file %s\n", file);	
		return;	
	}

	/* clear map data */
	int i;
	for (int i=0; i<32*32; i++)
		mapdata[i] = 0;

	/* match each tile in the map to a tile in the tiles  */
	int maptilecount = floor(w/8)*floor(h/8);
	int byte         = 0;
	int tile         = 0;
	int tilex        = 0;
	int tiley        = 0;
	int x, y;

	for (int i=0; i<maptilecount; i++) {
		byte = 0;
		tile = 0;

		unsigned short int singletile[16];
		for (int y=0; y<16; y++)
			singletile[y] = 0;
	
		/* get single tile data */
		for (int y=0; y<8; y++) {
			int shift = 7;
			for (int x=0; x<8; x++) {					
				int index = 4 * (((y+tiley) * w) + (x + tilex));
				unsigned short int cbit1  = 0b00000001;
				unsigned short int cbit2  = 0b00000001;
				
				if (data[index] > 60 && data[index] < 120) {
					cbit1  = 0b00000000;
					cbit2  = 0b00000001;
				} else if (data[index] > 119 && data[index] < 180) {
					cbit1  = 0b00000001;
					cbit2  = 0b00000000;
				} else if (data[index] > 179) {
					cbit1  = 0b00000000;
					cbit2  = 0b00000000;
				}
				
				singletile[byte]   |= (cbit1 << shift);
				singletile[byte+1] |= (cbit2 << shift);
				shift--;
			}
			byte+=2;
		}

		tilex += 8;
		if (tilex > w-1) {
			tiley += 8;
			tilex  = 0;	
		}	

		/* match against on in tilemap */
		int c;
		for (int c=0; c<tilecount; c++) {
			int matchcount = 0;
			for (int x=0; x<16; x++) {
				if (singletile[x] == tiledata[x + (c*16)]) {
					matchcount++;
				}
			}
			if (matchcount > 15) {
				tile = c;
				break;
			}
		}
	
		mapdata[i] = tile;
	}

	/* print out the map data */
	printf("MAP_DATA: \n");
	int c = 0;
	for (int y=0; y<32; y++) {
		printf("\nDB ");
		for (int x=0; x<32; x++) {
			printf("$");
			if(mapdata[c] < 0x10)
				printf("0");
			printf("%x", mapdata[c]);
			if (x < 31)
				printf(",");	
			c++;
		}
	}
}


