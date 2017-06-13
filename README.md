# GB-Convert
---
Gameboy-convert is a simple tool to convert png images, into a format supported by Gameboy assembly.


It currently takes in a png file, and outputs it in an assembly format you can use when developing for the
Gameboy.  Eventually this will be used in a Gameboy homebrew tutorial I am writing on my [website](http://exez.in).

The tiles conversion takes an arbitrarily sized image and outputs the assembly equivalent.  It splits the image up into 8x8 tiles.

The map conversion takes in a 256x256 image (32x32 tiles), and matches the tiles in the image, with the tiles from
your tilesheet, and outputs the assembly equivalent of your map for use on the Gameboy.

The -i flag inverts the color order, useful if you want to change the transparent color for sprite layers, etc

### Make
```
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
```


### Depends
- A modern C compiler.


### Todo
- [x] Better Support for arbitrary image sizes
- [ ] RLE/Various encoding/decoding methods
- [ ] The Gameboy assembly to encoded/decode data