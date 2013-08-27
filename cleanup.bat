ECHO Cleanup Started

cd c:\wamp\www\voxelquest
SET name=%random%
ECHO "%name%"  > .\compiled\main.cpp

del /q .\bin\*.*
xcopy /s /y .\Release .\bin
del /q .\Release\*.*

ECHO running EXE
cd c:\wamp\www\voxelquest\bin
VoxelQuest.exe
