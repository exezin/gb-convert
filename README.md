# GB-Convert
---
Gameboy-convert is a simple tool to convert png images, into a format supported by Gameboy assembly.


It currently takes in a Nx8 size png file, and outputs it in an assembly format you can use when developing for the
Gameboy.  Eventually this will be used in a Gameboy homebrew tutorial I am writing on my website (https://siobyte.xyz).

The tiles conversion takes a 8 pixel high, and X pixels long image and outputs the assembly equivalent.

The map conversion takes in a 256x256 image (32x32 tiles), and matches the tiles in the image, with the tiles from
your tilesheet, and outputs the assembly equivalent of your map for use on the Gameboy.

The -i flag inverts the color order, useful if you want to change the transparent color for sprite layers, etc

### Make
```
mkdir build
mkdir obj
make
```

### Example usage
```
(mytiles.png = 32x8 )
(mymap.png   = 32x32)

./gb-convert -i -tiles mytiles.png -map mymap.png >> output.txt

    >> output.txt - 4 tiles - 16 bytes each
    TILE_DATA:
    DB $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
    DB $ff,$ff,$81,$ff,$bd,$c3,$a5,$c3,$a5,$c3,$bd,$c3,$81,$ff,$ff,$ff
    DB $00,$00,$44,$44,$00,$00,$00,$00,$44,$44,$38,$38,$00,$00,$00,$00
    DB $00,$00,$22,$22,$00,$00,$00,$00,$1c,$1c,$22,$22,$00,$00,$00,$00
	
    MAP_DATA:
    DB $01,$01,$00...etc

    Colors should be the following (or close to) beforehand:
    White - (255, 255, 255)
    Grey  - (150, 150, 150)
    DGrey - (80,  80,  80 )
    Black - (20,  20,  20 )
```


### Depends
- A modern C compiler.


### Todo
- Better Support for arbitrary image sizes
