[LaroldsJubilantJunkard.com](LaroldsJubilantJunkard.com) - [Re-creating Popular Games for Gameboy](https://laroldsjubilantjunkyard.com/tutorials/recreating-gameboy-games/)
# Zelda Clone 

Add objects to the map in the "objectspawning.c". Object positions are relative to next camera section they are in. each camera section is 160px wide, and 128px tall. 

I group camera sections based on what sprite data they need in VRAM.

To edit the tilemap, edit the BasicMap.tmx file in Tiled. Then export (BasicMap.png) as an image to the graphics folder. It should always be a multiple of 160x128.
To edit the tileset, simply edit the "graphics/Map-Source.png" image.

To add different objects you'll need to:
 - create update functions for them.
 - add their sprite data into VRAM in the objectspawning.c (if not already present)
 - spawn their objects with far pointers to their update functions, all in the objectspawning.c file.