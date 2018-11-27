@echo off & setlocal enableextensions

:: Hidden files to remove
set Hidden=*.suo Thumbs.db
:: Files to remove
set Files=*.aps *.exp *.idb *.ilk *.ipch *.ncb *.obj *.pch *.pdb *.sbr *.sdf *.tlh *.tli *.user *.VC.db *.xdc
:: Directories to remove
set Directories=.vs bin ipch lib obj TestResults 


:: Remove hidden files
del /s /q /f /a:h %Hidden%
::Remove files
del /s /q /f %Files%
:: Remove directories
for /d /r . %%d in (%Directories%) do @if exist "%%d" echo "%%d" && rmdir /s /q "%%d"

:: That's the end
exit /b