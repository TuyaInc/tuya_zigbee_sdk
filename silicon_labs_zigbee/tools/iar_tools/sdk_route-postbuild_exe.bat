

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
::SET NAME=%param[6]:"=%
::SET VER=%param[8]:"=%


set /P STR_VER=<%IAR_PROJECT_DIR%\..\firmware_ver_temp_file
set /P NAME=<%IAR_PROJECT_DIR%\..\firmware_name_temp_file

for /f "delims=. tokens=1" %%i in ("%STR_VER%") do SET V1=%%i
for /f "delims=. tokens=2" %%j in ("%STR_VER%") do SET V2=%%j
for /f "delims=. tokens=3" %%k in ("%STR_VER%") do SET V3=%%k

SET /a var = %V1%*64+%V2%*16+%V3%
set code=0123456789ABCDEF
set /a tra=%var% %% 16
call set tra=%%code:~%tra%,1%%
set /a var/=16
call set var=%%code:~%var%,1%%

set HEX_VER=%var%%tra%


%IAR_PROJECT_DIR%\..\..\..\..\..\tools\iar_tools\ielftool.exe "%targetdir%/sdk_route.out" --srec --srec-s3only "%targetdir%/sdk_route.s37"
::cd D:\zigbee_project\sdk_router\app\builder\sdk_route_user\tools\
  %IAR_PROJECT_DIR%\..\..\..\..\..\tools\iar_tools\commander.exe gbl create "%targetdir%/sdk_route.gbl" --app "%targetdir%/sdk_route.s37" --device %chip%
::cd D:\zigbee_project\sdk_router\app\builder\sdk_route_user\tools\
%IAR_PROJECT_DIR%\..\..\..\..\..\tools\iar_tools\image-builder-windows.exe --create "%targetdir%/sdk_route.ota" --version 0x%HEX_VER% --manuf-id 0x1002 --image-type 0x1602 --tag-id 0x0000 --tag-file "%targetdir%/sdk_route.gbl" --string "EBL sdk_route"
::cd D:\zigbee_project\sdk_router\app\builder\sdk_route_user\tools\
  %IAR_PROJECT_DIR%\..\..\..\..\..\tools\iar_tools\commander.exe convert %IAR_PROJECT_DIR%\..\..\..\..\..\tools\iar_tools\%chip%_bootloader.s37 %targetdir%\sdk_route.s37 --address 0x0 --outfile %targetdir%\%NAME%_QIO_%STR_VER%.s37
copy %targetdir%\sdk_route.ota %targetdir%\%NAME%_OTA_%STR_VER%.bin
DEL %IAR_PROJECT_DIR%\..\firmware_ver_temp_file
DEL %IAR_PROJECT_DIR%\..\firmware_name_temp_file
DEL %IAR_PROJECT_DIR%\..\config.mk
DEL %IAR_PROJECT_DIR%\..\package.json