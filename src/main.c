/*
  gb-convert by exezin
  -github.com/exezin
  -http://exez.in

  example usage:
  "./gb-convert -tiles mytiles.png -map mymap.png >> output.txt"

  TODO:
  -better image size support (?)
  -???
*/

#define STB_IMAGE_IMPLEMENTATION
#include <inc/stb_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#define TILE_WIDTH 8
#define TILE_HEIGHT 8

void convert_tiles(const char *file);
void convert_map(const char *file);

int tilecount = 0;
int bytecount = 0;
uint8_t tiledata[192*16];
uint8_t mapdata[32*32];

// pixel shades
uint8_t tilev[8] = {
  0b00000001, 0b00000001,
  0b00000000, 0b00000001,
  0b00000001, 0b00000000,
  0b00000000, 0b00000000
};

int main(int argc, char *argv[])
{
  int i;
  for (int i=0; i<argc; i++) {
    if (strcmp("-tiles", argv[i]) == 0)
      convert_tiles(argv[i+1]);
    else if (strcmp("-map", argv[i]) == 0)
      convert_map(argv[i+1]);
    else if (strcmp("-i", argv[i]) == 0) {
      // invert pixel shades
      tilev[0] = 0b00000000;
      tilev[1] = 0b00000000;
      tilev[2] = 0b00000000;
      tilev[3] = 0b00000001;
      tilev[4] = 0b00000001;
      tilev[5] = 0b00000000;
      tilev[6] = 0b00000001;
      tilev[7] = 0b00000001;
    }
  }

  return 1;
}

void convert_tiles(const char *file)
{
  int n, w, h;
  uint8_t *data = stbi_load(file, &w, &h, &n, 4);

  if (data == NULL) {
    printf("Failed to load file %s\n", file); 
    return;
  }

  // 8x8 pixels, 2 bits per pixel, 2 bytes per line
  tilecount = floor(w/TILE_WIDTH)*floor(h/TILE_HEIGHT);
  int x_offset = 0;
  for (int i=0; i<tilecount; i++) {
    for (int y=0; y<TILE_HEIGHT; y++) {
      int shift = 7;
      for (int x=0; x<TILE_WIDTH; x++) {
        // get index of first pixel for given tile index
        int a = floor(i/(w/TILE_WIDTH))*(w*TILE_WIDTH);
        int index = 4 * (a + (y * w) + (x_offset*TILE_WIDTH) + x);
        uint8_t cbit1 = tilev[0];
        uint8_t cbit2 = tilev[1];

        // get pixel shade from image data
        float shade = (float)(data[index] + data[index+1] + data[index+2])/255.0f;
        if (shade > 0.7f && shade < 1.1f) {
          cbit1  = tilev[2];
          cbit2  = tilev[3];
        } else if (shade > 1.09f && shade < 1.30f) {
          cbit1  = tilev[4];
          cbit2  = tilev[5];
        } else if (shade > 1.29f) {
          cbit1  = tilev[6];
          cbit2  = tilev[7];
        }
        
        // store tile data
        tiledata[bytecount]   |= (cbit1 << shift);
        tiledata[bytecount+1] |= (cbit2 << shift);
        shift--;
      }
      bytecount+=2;
    }
    x_offset++;
    if (x_offset >= (w/TILE_WIDTH))
      x_offset = 0;
  }

  // print out the bytes
  int c = 0;
  printf("TILE_COUNT EQU %i \n", bytecount);
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

void convert_map(const char *file)
{
  int n, w, h;
  uint8_t *data = stbi_load(file, &w, &h, &n, 4);

  if (data == NULL) {
    printf("Failed to load file %s\n", file);
    return; 
  }

  // match each tile in the map to a tile in the tiles
  int maptilecount = floor(w/TILE_WIDTH)*floor(h/TILE_HEIGHT);
  int byte         = 0;
  int tile         = 0;
  int tilex        = 0;
  int tiley        = 0;
  int x, y;

  // clear map data
  int i;
  for (int i=0; i<maptilecount; i++)
    mapdata[i] = 0;

  for (int i=0; i<maptilecount; i++) {
    byte = 0;
    tile = 0;

    uint8_t singletile[16];
    for (int y=0; y<16; y++)
      singletile[y] = 0;
  
    /* get single tile data */
    for (int y=0; y<TILE_HEIGHT; y++) {
      int shift = 7;
      for (int x=0; x<TILE_WIDTH; x++) {
        int index = 4 * (((y+tiley) * w) + (x + tilex));
        uint8_t cbit1 = tilev[0];
        uint8_t cbit2 = tilev[1];

        // get pixel shade from image data
        float shade = (float)(data[index] + data[index+1] + data[index+2])/255.0f;
        if (shade > 0.7f && shade < 1.1f) {
          cbit1  = tilev[2];
          cbit2  = tilev[3];
        } else if (shade > 1.09f && shade < 1.30f) {
          cbit1  = tilev[4];
          cbit2  = tilev[5];
        } else if (shade > 1.29f) {
          cbit1  = tilev[6];
          cbit2  = tilev[7];
        }
        
        singletile[byte]   |= (cbit1 << shift);
        singletile[byte+1] |= (cbit2 << shift);
        shift--;
      }
      byte+=2;
    }

    tilex += TILE_WIDTH;
    if (tilex > w-1) {
      tiley += TILE_HEIGHT;
      tilex  = 0;
    } 

    // match against on in tilemap
    int c;
    for (int c=0; c<tilecount; c++) {
      int matchcount = 0;
      for (int x=0; x<16; x++) {
        if (singletile[x] == tiledata[x + (c*16)]) {
          matchcount++;
        }
      }
      if (matchcount == 16) {
        tile = c;
        break;
      }
    }
    mapdata[i] = tile;
  }

  // print out the map data
  printf("MAP_SIZE EQU 1024 \n"); 
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


