

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


%IAR_PROJECT_DIR%\..\..\..\..\tools\iar_tools\sdk_route-postbuild_exe.bat %* > %targetdir%\post_build.txt