
rmdir /s/q  "source\gen"
rmdir /s/q  "headers\gen"

mkdir "source\gen\default\graphics"
mkdir "headers\gen\graphics"


SET GBDK_HOME=C:/gbdk
SET PNG2ASSET=C:/gbdk/bin/png2asset
SET PNG2ASSET=C:\Users\larry\Documents\gbdk-2020\gbdk-support\png2asset\x64\Debug\png2asset

%PNG2ASSET% graphics\LinkSprites.png -c source\gen\default\graphics\LinkSprites.c -sw 16 -sh 16 -spr8x16 -keep_palette_order -noflip
%PNG2ASSET% graphics\LinkPushSprites.png -c source\gen\default\graphics\LinkPushSprites.c -sw 16 -sh 16 -spr8x16 -keep_palette_order -noflip
%PNG2ASSET% graphics\LinkShieldSprites.png -c source\gen\default\graphics\LinkShieldSprites.c -sw 16 -sh 16 -spr8x16 -keep_palette_order -noflip
%PNG2ASSET% graphics\LinkSwordSprites.png -c source\gen\default\graphics\LinkSwordSprites.c -sw 16 -sh 16 -spr8x16 -keep_palette_order -noflip
%PNG2ASSET% graphics\LinkActiveShieldSprites.png -c source\gen\default\graphics\LinkActiveShieldSprites.c -sw 16 -sh 16 -spr8x16 -keep_palette_order -noflip
%PNG2ASSET% graphics\SwordSlash.png -c source\gen\default\graphics\SwordSlash.c -sw 16 -sh 16 -spr8x16 -keep_palette_order -noflip
%PNG2ASSET% graphics\Moblin.png -c source\gen\default\graphics\Moblin.c -sw 16 -sh 16 -spr8x16 -keep_palette_order -noflip
%PNG2ASSET% graphics\NPCMarin.png -c source\gen\default\graphics\NPCMarin.c -sw 16 -sh 16 -spr8x16 -keep_palette_order -noflip

::%PNG2ASSET% graphics\Map-source.png -c source\gen\default\graphics\MapSource.c -map -use_map_attributes -is_source_tileset
%PNG2ASSET% graphics\Map.png -c source\gen\default\graphics\Map.c -map -use_map_attributes -noflip
%PNG2ASSET% graphics\HUD.png -c source\gen\default\graphics\HUD.c -map -use_map_attributes -noflip -keep_palette_order
%PNG2ASSET% graphics\AlphaNumeric.png -c source\gen\default\graphics\AlphaNumeric.c -map -use_map_attributes -noflip -keep_palette_order


:: move .h files to their proper location
FOR /R "source/gen/bank1/graphics/" %%X IN (*.h) DO (
    move "%%X" "%cd%/headers/gen/graphics/"
)

:: move .h files to their proper location
FOR /R "source/gen/default/graphics/" %%X IN (*.h) DO (
    move "%%X" "%cd%/headers/gen/graphics/"
)