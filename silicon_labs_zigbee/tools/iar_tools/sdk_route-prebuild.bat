
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
SET chip=%param[6]:"=%


copy %IAR_PROJECT_DIR%\package.json %IAR_PROJECT_DIR%\..\package.json
cd ..
python %IAR_PROJECT_DIR%\..\pre-build-iar.py
move /Y config.c %IAR_PROJECT_DIR%\..\src\config.c
move /Y config.h %IAR_PROJECT_DIR%\..\include\config.h

