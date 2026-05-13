@echo off
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x64 >nul 2>&1
cd /d d:\Qt\EditPic\tests
if exist Makefile nmake clean
if exist Makefile.Release del Makefile.Release
if exist Makefile.Debug del Makefile.Debug
if exist .qmake.stash del .qmake.stash
qmake tst_editpic.pro
nmake
