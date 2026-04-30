del /s /A:H *.suo
del /s *.bak
del /s Makefile
del /s Makefile.Debug
del /s Makefile.Release
del /s *.ilk
del /s *.pdb
del /s *.user
del /s *.idb
del /s *.ncb
del /s *.log
del /s *.obj
del /s *.tlog
del /s *.sln
del /s *.aps
del /s ui*.h
del /s moc*.cpp
del /s qrc_*.cpp
del /s icon.res
del /s .qmake.stash
del /s *pre1
del /s *user.22

rmdir /s /q GeneratedFiles
rmdir /s /q Release
rmdir /s /q Debug


:: @echo on
setlocal enabledelayedexpansion  
  
@REM 设置你想删除的目录  
set WHAT_SHOULD_BE_DELETED1=debug
  
for /r . %%a in (!WHAT_SHOULD_BE_DELETED1!) do (  
  if exist %%a (  
  echo "remove"%%a
  rd /s /q "%%a"  
 )  
)  


@REM 设置你想删除的目录  
set WHAT_SHOULD_BE_DELETED2=release
  
for /r . %%a in (!WHAT_SHOULD_BE_DELETED2!) do (  
  if exist %%a (  
  echo "remove"%%a
  rd /s /q "%%a"  
 )  
)  



@REM 设置你想删除的目录  
set WHAT_SHOULD_BE_DELETED2=.vs
  
for /r . %%a in (!WHAT_SHOULD_BE_DELETED2!) do (  
  if exist %%a (  
  echo "remove"%%a
  rd /s /q "%%a"  
 )  
)  


pause
