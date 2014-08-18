ECHO Debug Cleanup Started

:: DONT USE THIS, INSTEAD JUST BUILD AND MANUALLY START DEBUGGER

cd c:\wamp\www\voxelquest
:: SET name=%random%
:: ECHO "%name%"  > .\compiled\main.cpp

del /q .\bin\*.*
xcopy /s /y .\x64\Debug .\bin
del /q .\x64\Debug\*.*

ECHO running EXE
cd c:\wamp\www\voxelquest\bin
VoxelQuest.exe
