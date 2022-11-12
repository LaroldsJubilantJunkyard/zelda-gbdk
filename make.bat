@echo off

:: Cleanup
rmdir /s /q bin
rmdir /s /q dist

:: Cleanup
mkdir bin
mkdir dist


SET GBDK_HOME=C:/gbdk

SET LCC_COMPILE_BASE=%GBDK_HOME%\bin\lcc  -autobank -debug -Iheaders/main -Iheaders/gen -Wa-l -Wl-m -Wl-j -DUSE_SFR_FOR_REG
SET LCC_COMPILE=%LCC_COMPILE_BASE% -c -o 

:: Required to concatenate the "COMPILE_OBJECT_FILES" via a for loop
SETLOCAL ENABLEDELAYEDEXPANSION

SET "COMPILE_OBJECT_FILES="


call generate-graphics.bat


:: loop for all files in the default source folder
FOR /R "source/gen/" %%X IN (*.c) DO (
    echo Compiling %%~nX ...
    %LCC_COMPILE% bin/gen_%%~nX.o %%X
    SET "COMPILE_OBJECT_FILES=bin/gen_%%~nX.o !COMPILE_OBJECT_FILES!"

)


:: loop for all files in the default source folder
FOR /R "source/default/" %%X IN (*.c) DO (
    echo Compiling %%~nX ...
    %LCC_COMPILE% bin/%%~nX.o %%X
    SET "COMPILE_OBJECT_FILES=bin/%%~nX.o !COMPILE_OBJECT_FILES!"

)


:: Compile a .gb file from the compiled .o files
%LCC_COMPILE_BASE% -Wm-yC  -Wl-yt3 -Wl-yoA -Wl-yo2 -o dist/Zelda.gb !COMPILE_OBJECT_FILES!

endlocal

:: Use the romusage.exe to show the size of the ROM and what makes it up
"lib/romusage" dist\Zelda.cdb -a