
echo off
setlocal enableextensions enabledelayedexpansion
SET allParams=%*
SET params=%allParams:' '=","%
SET params=%params:'="%
set /a count=0
for %%i in (%params%) do (
  set /a count+=1
  set param[!count!]=%%i
)
   

SET IAR_PROJECT_DIR=%param[4]:"=%
SET targetdir=%param[5]:"=%
SET firmware_name=%param[6]:"=%
SET STR_VER=%param[7]:"=%
SET ver_header_file=%IAR_PROJECT_DIR%\dev_version.h

::python %IAR_PROJECT_DIR%\..\..\..\..\tools\pre-build-iar.py
python %IAR_PROJECT_DIR%\pre-build-iar.py
move /Y config.c %IAR_PROJECT_DIR%\src\config.c
move /Y config.h %IAR_PROJECT_DIR%\include\config.h

