
set IAR_BIN_DIR="C:\Program Files (x86)\IAR Systems\Embedded Workbench 8.3\common\bin"
set IAR_PROJECT_NAME="tuya_sdk"
set IAR_PROJECT_PATH_NAME=%IAR_PROJECT_NAME:"=%.ewp

"%IAR_BIN_DIR:"=%"\IarBuild.exe %IAR_PROJECT_PATH_NAME% -build * -log all
cd ..

