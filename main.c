/*
	gb-convert by siObyte
	-github.com/siobyte
	-siobyte.xyz

	example usage:
	"./gb-convert -tiles mytiles.png >> output.txt"

	TODO:
	-map editing/converting
	-better image size support
*/

#define STB_IMAGE_IMPLEMENTATION
#include <inc/stb_image.h>
#include <stdio.h>
#include <stdlib.h>

void ConvertTiles(const char *file);

int main(int argc, char *argv[])
{
	if (strcmp("-tiles", argv[1]) == 0)
		ConvertTiles(argv[2]);

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
	int byte = 0;
	int tilecount = w/8;
	unsigned short int tiledata[192*16];
	for (int i=0; i<tilecount; i++) {
		for (int y=0; y<8; y++) {
			int shift = 7;
			for (int x=0; x<8; x++) {
				int index = 4 * (((y * w) + x) + (i*8));
				unsigned short int color  = 0b00000001;
				unsigned short int color2 = 0b00000001;

				if (data[index] > 60 && data[index] < 120) {
					color  = 0b00000000;
					color2 = 0b00000001;
				} else if (data[index] > 119 && data[index] < 180) {
					color  = 0b00000001;
					color2 = 0b00000000;
				} else if (data[index] > 179) {
					color  = 0b00000000;
					color2 = 0b00000000;
				}
				
				tiledata[byte]   |= (color << shift);
				tiledata[byte+1] |= (color2 << shift);
				shift--;
			}
			byte+=2;
		}
	}

	/* print out the bytes */
	int c;
	for (int i=0; i<byte; i++) {
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
}











