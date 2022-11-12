
rmdir /s/q  "source\gen"
rmdir /s/q  "headers\gen"

mkdir "source\gen\default\graphics"
mkdir "headers\gen\graphics"


SET GBDK_HOME=C:/gbdk
SET PNG2ASSET=C:/gbdk/bin/png2asset

:: Linkavitch
%PNG2ASSET% graphics\LinkSpritesRight.png -c source\gen\default\graphics\LinkSpritesRight.c -sw 16 -sh 16 -spr8x16 -keep_palette_order -noflip
%PNG2ASSET% graphics\LinkSpritesUp.png -c source\gen\default\graphics\LinkSpritesUp.c -sw 16 -sh 16 -spr8x16 -keep_palette_order -noflip
%PNG2ASSET% graphics\LinkSpritesDown.png -c source\gen\default\graphics\LinkSpritesDown.c -sw 16 -sh 16 -spr8x16 -keep_palette_order -noflip
%PNG2ASSET% graphics\LinkSpritesLeft.png -c source\gen\default\graphics\LinkSpritesLeft.c -sw 16 -sh 16 -spr8x16 -keep_palette_order -noflip

%PNG2ASSET% graphics\LinkShieldSpritesDown.png -c source\gen\default\graphics\LinkShieldSpritesDown.c -sw 16 -sh 16 -spr8x16 -keep_palette_order -noflip
%PNG2ASSET% graphics\LinkShieldSpritesUp.png -c source\gen\default\graphics\LinkShieldSpritesUp.c -sw 16 -sh 16 -spr8x16 -keep_palette_order -noflip
%PNG2ASSET% graphics\LinkShieldSpritesLeft.png -c source\gen\default\graphics\LinkShieldSpritesLeft.c -sw 16 -sh 16 -spr8x16 -keep_palette_order -noflip
%PNG2ASSET% graphics\LinkShieldSpritesRight.png -c source\gen\default\graphics\LinkShieldSpritesRight.c -sw 16 -sh 16 -spr8x16 -keep_palette_order -noflip

%PNG2ASSET% graphics\LinkSwordSpritesUp.png -c source\gen\default\graphics\LinkSwordSpritesUp.c -sw 16 -sh 16 -spr8x16 -keep_palette_order -noflip
%PNG2ASSET% graphics\LinkSwordSpritesRight.png -c source\gen\default\graphics\LinkSwordSpritesRight.c -sw 16 -sh 16 -spr8x16 -keep_palette_order -noflip
%PNG2ASSET% graphics\LinkSwordSpritesDown.png -c source\gen\default\graphics\LinkSwordSpritesDown.c -sw 16 -sh 16 -spr8x16 -keep_palette_order -noflip
%PNG2ASSET% graphics\LinkSwordSpritesLeft.png -c source\gen\default\graphics\LinkSwordSpritesLeft.c -sw 16 -sh 16 -spr8x16 -keep_palette_order -noflip

%PNG2ASSET% graphics\SwordSlashUp.png -c source\gen\default\graphics\SwordSlashUp.c -sw 16 -sh 16 -spr8x16 -keep_palette_order -noflip
%PNG2ASSET% graphics\SwordSlashRight.png -c source\gen\default\graphics\SwordSlashRight.c -sw 16 -sh 16 -spr8x16 -keep_palette_order -noflip
%PNG2ASSET% graphics\SwordSlashLeft.png -c source\gen\default\graphics\SwordSlashLeft.c -sw 16 -sh 16 -spr8x16 -keep_palette_order -noflip
%PNG2ASSET% graphics\SwordSlashDown.png -c source\gen\default\graphics\SwordSlashDown.c -sw 16 -sh 16 -spr8x16 -keep_palette_order -noflip

:: NPCs
%PNG2ASSET% graphics\NPCMarin.png -c source\gen\default\graphics\NPCMarin.c -sw 16 -sh 16 -spr8x16 -keep_palette_order -b 255
%PNG2ASSET% graphics\Chest.png -c source\gen\default\graphics\Chest.c -sw 16 -sh 16 -spr8x16 -keep_palette_order -b 255
%PNG2ASSET% graphics\SwordAlone.png -c source\gen\default\graphics\SwordAlone.c -sw 16 -sh 16 -spr8x16 -keep_palette_order -b 255

%PNG2ASSET% graphics\Explosion.png -c source\gen\default\graphics\Explosion.c -sw 16 -sh 16 -spr8x16 -keep_palette_order -noflip -b 255
:: Enemies
%PNG2ASSET% graphics\Moblin.png -c source\gen\default\graphics\Moblin.c -sw 16 -sh 16 -spr8x16 -keep_palette_order -noflip -b 255
:: %PNG2ASSET% graphics\Gel.png -c source\gen\default\graphics\Gel.c -sw 16 -sh 16 -spr8x16 -keep_palette_order -noflip
:: %PNG2ASSET% graphics\Octoroc.png -c source\gen\default\graphics\Octoroc.c -sw 16 -sh 16 -spr8x16 -keep_palette_order -noflip

:: Backgrounds & Maps
%PNG2ASSET% graphics\HUD.png -c source\gen\default\graphics\HUD.c -map -use_map_attributes -noflip -keep_palette_order -b 255
%PNG2ASSET% graphics\AlphaNumeric.png -c source\gen\default\graphics\AlphaNumeric.c -map -use_map_attributes -noflip -keep_palette_order  -b 255


%PNG2ASSET%  graphics\BasicMap.png -c source\gen\default\graphics\BasicMap.c -map -use_map_attributes -noflip  -b 255 -source_tileset graphics\Map-Source.png
%PNG2ASSET%  graphics\Map-Source.png -c source\gen\default\graphics\MapSource.c -map -use_map_attributes -noflip -keep_palette_order -b 255

:: move .h files to their proper location
FOR /R "source/gen/bank1/graphics/" %%X IN (*.h) DO (
    move "%%X" "%cd%/headers/gen/graphics/"
)

:: move .h files to their proper location
FOR /R "source/gen/default/graphics/" %%X IN (*.h) DO (
    move "%%X" "%cd%/headers/gen/graphics/"
)