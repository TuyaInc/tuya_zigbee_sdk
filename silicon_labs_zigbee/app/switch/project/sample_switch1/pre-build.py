# coding:utf-8
'''
@Author: Leon
@email: 
@LastEditors  : Leon
@file name: per-build.py
@Description: auto generate files and set default config 
@Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
@Company: http://www.tuya.com
@Date: 2019-04-15 10:19:39
@LastEditTime : 2020-01-03 15:49:43
'''

import sys
import os
import logging
import re
import json
import string
import shutil
import collections

class uart_cfg:
    def __init__(self):
        self.uart_rx_port = ''
        self.uart_rx_pin = ''
        self.uart_rx_loc = ''
        self.uart_tx_port = ''
        self.uart_tx_pin = ''
        self.uart_tx_loc = ''
        self.uart_bandrate = 115200

class in_out:
    def __init__(self):
        self.port = ''
        self.pin = ''
        self.mode = ''
        self.out = ''
        self.driver = ''

# global variable
name = ""
description = ""
version = ""
bv_version = ""
ic = ""
ota_image_type = ""
model_id = ""
pid = ""
version_hex = ""
manufacture_name = ""
TABLE_SPACE = 40
manufacture_id = ""
module_name = ""

ledArry = list()
keyArry = list()
relayArry = list()
ledInfo = {"Name":'led', "Enable":'', "Num":''}
keyInfo = {"Name":'key', "Enable":'', "Num":''}
relayInfo = {"Name":'relay', "Enable":'', "Num":''}

current_path = os.getcwd()+'/'
callback_file_source_path = "../../../../../tools/code_template/callbacks.c"
callback_file_dest_path = "../src/"

# fimware info parser
def fimware_info_get():

    global name
    global description
    global version
    global ic
    global ota_image_type
    global model_id
    global pid
    global manufacture_name
    global manufacture_id
    global module_name

    print("fimware information parse start...")
    file = open('package.json', 'rb')
    fileJson = json.load(file, object_pairs_hook=collections.OrderedDict)
    name = fileJson['fimwareInfo']['name']
    description = fileJson['fimwareInfo']['description']
    version = fileJson['fimwareInfo']['version']
    ic = fileJson['fimwareInfo']['ic']
    ota_image_type = fileJson['fimwareInfo']['ota_image_type']
    model_id = fileJson['fimwareInfo']['model_id']
    pid = fileJson['fimwareInfo']['pid']
    manufacture_name = fileJson['fimwareInfo']['manufacture_name']
    manufacture_id = fileJson['fimwareInfo']['manufacture_id']
    module_name = fileJson['fimwareInfo']['module_name']
    file.close()
    print("fimware information parse success")

# version info convert
def version_info_convert():
    global version_hex
    str = version.replace('.', '')
    versionLen = str.__len__()
    version_hex = ((int(str[0:1]) << 6) & 0xc0) + ((int(str[1:2]) << 4) & 0x30)
    if (version_hex > 3):
        version_hex = version_hex + (int(str[2:4]) & 0x0f)
    else:
        version_hex = version_hex + (int(str[2:3]) & 0x0f)
    # print(hex(version_hex))


# write ifno to generate file
def ato_generate_func():
    def wrapper(func):
        def __wrapper(*args,**kwargs):
            print("file generate start...")
            fileObject = open(args[0], 'a+')
            fileObject.writelines('{'+ '\n')
            func(*args,**kwargs)
            fileObject.writelines('}' + '\n')
            fileObject.close()
        return __wrapper
    return wrapper


# write ifno to generate file
def ato_generate_file():
    def wrapper(func):
        def __wrapper(*args,**kwargs):
            print("file generate start...")
            fileObject = open(args[0], 'w')
            fileObject.writelines(
                "/*************************************************************************************/" + '\n')
            fileObject.writelines("/* Automatically-generated file. Do not edit! */" + '\n')
            fileObject.writelines(
                "/*************************************************************************************/" + '\n')
            fileObject.close()
            return func(*args,**kwargs)
        return __wrapper
    return wrapper


# generate config.c file
@ato_generate_file()
def ato_generate_config_c(*args,**kwargs):
    fileObject = open(args[0], 'a+')

    generate_firmware_func_config_c(fileObject)

    fileObject.close()
    print(args[0]+"config file write success")


# generat firmware function in config.c
def generate_firmware_func_config_c(fileObj):

    fileObj.writelines("#include \"tuya_zigbee_sdk.h\"" + '\n')
    fileObj.writelines("#include \"config.h\"" + '\n\n\n')
    fileObj.writelines("char *g_firmware_name = FIRMWARE_NAME;" + '\n')
    fileObj.writelines("uint8_t g_firmware_ver = FIRMWARE_VER;" + '\n' + '\n')
    # fileObj.writelines("char *g_sdk_ver = SDK_VER;" + '\n')
    global ledInfo, relayInfo, keyInfo

    fileObj.writelines("void firmware_config(void)" + '\n')
    fileObj.writelines("{" + '\n')
    fileObj.writelines("    dev_register_base_info(MODEL_ID, PRODUCTOR_ID_PFEFIX, PRODUCTOR_ID);" + '\n')
    fileObj.writelines("}" + '\n')


# generate config.h file
@ato_generate_file()
def ato_generate_config_h(*args,**kwargs):

    fileObject = open(args[0], 'a+')

    fileObject.writelines("#ifndef __CONFIG_H__" + '\n')
    fileObject.writelines("#define __CONFIG_H__" + '\n' + '\n')
    fileObject.writelines("#include \"tuya_zigbee_sdk.h\"" + '\n')
    fileObject.writelines("/* firmware information! */" + '\n')

    generate_fimware_macro_config_h(fileObject)
    generate_uartcfg_macro_config_h(fileObject)
    generate_iocfg_macro_config_h(fileObject)

    fileObject.writelines('\n' + "#endif" + '\n')
    fileObject.close()
    print(args[0]+"config file write success")

# generate fimware config
def generate_fimware_macro_config_h(fileObj):
    fileObj.writelines("#define FIRMWARE_INFO".ljust(TABLE_SPACE) + '\"' + name + "_" + version + '\"' + '\n')
    fileObj.writelines("#define FIRMWARE_NAME".ljust(TABLE_SPACE) + '\"' + name + '\"' + '\n')
    fileObj.writelines("#define FIRMWARE_VER".ljust(TABLE_SPACE) + hex(version_hex) + '\n')
    fileObj.writelines("#define PRODUCTOR_ID_PFEFIX".ljust(TABLE_SPACE) + '\"' + manufacture_name + '\"' + '\n')
    fileObj.writelines("#define IC".ljust(TABLE_SPACE) + '\"' + ic + '\"' + '\n')
    fileObj.writelines("#define OTA_IMAGE_TYPE".ljust(TABLE_SPACE) + ota_image_type + '\n')
    fileObj.writelines("#define MODULE_NAME".ljust(TABLE_SPACE)  + module_name + '\n')

    if len(model_id) < 6:
        fileObj.writelines("#define MODEL_ID".ljust(TABLE_SPACE) + "NULL" + '\n')
    else:
        fileObj.writelines("#define MODEL_ID".ljust(TABLE_SPACE) + '\"' + model_id + '\"' + '\n')

    if len(pid) < 8:
        fileObj.writelines("#define PRODUCTOR_ID".ljust(TABLE_SPACE) + "NULL" + '\n')
    else:
        fileObj.writelines("#define PRODUCTOR_ID".ljust(TABLE_SPACE) + '\"' + pid + '\"' + '\n')

# generate io config
def generate_iocfg_macro_config_h(fileObj):
    fileJs = get_io_base_info()
    get_io_config_info(ledInfo,fileObj,fileJs)
    get_io_config_info(relayInfo,fileObj,fileJs)
    get_io_config_info(keyInfo,fileObj,fileJs)
    
# generate config.mk file
def ato_generate_config_mk(*args,**kwargs):

    fileObject = open(args[0], 'a+')
    fileObject.writelines("OBJ_DIR = build/obj" + '\n')
    fileObject.writelines("TARGET = " + name + '\n')
    fileObject.writelines("TARGET_SECURITY = " + name + "_security" + '\n')
    fileObject.writelines("STR_VER = " + version + '\n')
    fileObject.writelines("HEX_VER = " + hex(version_hex) + '\n')
    fileObject.writelines("OTA_IMAGE_TYPE = " + ota_image_type + '\n')
    fileObject.writelines("MANUF_ID = " + manufacture_id + '\n')
    fileObject.writelines("PLATFORM = " + ic + '\n')
    fileObject.close()
    print(args[0]+"config file write success")


# generate uart config info 
def generate_uartcfg_macro_config_h(fileObj):

    print("uart config parse start...")
    index = 0
    file = open('package.json', 'rb')
    fileJson = json.load(file,object_pairs_hook=collections.OrderedDict)
    uartEnable = fileJson['uartConfig']['uart_enable']
    uartNum = fileJson['uartConfig']['uart_num']
    file.close()

    if uartEnable == "true" and int(uartNum) > 0:
        fileObj.writelines('\n' + '\n')
        fileObj.writelines("/* uart config! */" + '\n')

        fileObj.writelines("#define UART_ENABLE".ljust(TABLE_SPACE) + uartEnable + '\n')
        fileObj.writelines("#define UART_NUM".ljust(TABLE_SPACE) + hex(uartNum) + '\n')
        fileObj.writelines('\n')
        
        uartArry = [uart_cfg()] * uartNum
        for index in range(0, uartNum):
            uart_rx_port = "uart" + str(index) + "_rx_port"
            uart_rx_pin = "uart" + str(index) + "_rx_pin"
            uart_rx_loc = "uart" + str(index) + "_rx_loc"
            uart_tx_port = "uart" + str(index) + "_tx_port"
            uart_tx_pin = "uart" + str(index) + "_tx_pin"
            uart_tx_loc = "uart" + str(index) + "_tx_loc"
            uart_bandrate = "uart" + str(index) + "_bandrate"

            uartArry[index].uart_rx_port = fileJson['uartConfig']['uart'][index][str(uart_rx_port)]
            uartArry[index].uart_rx_pin = fileJson['uartConfig']['uart'][index][str(uart_rx_pin)]
            uartArry[index].uart_rx_loc = fileJson['uartConfig']['uart'][index][str(uart_rx_loc)]
            uartArry[index].uart_tx_port = fileJson['uartConfig']['uart'][index][str(uart_tx_port)]
            uartArry[index].uart_tx_pin = fileJson['uartConfig']['uart'][index][str(uart_tx_pin)]
            uartArry[index].uart_tx_loc = fileJson['uartConfig']['uart'][index][str(uart_tx_loc)]
            uartArry[index].uart_bandrate = fileJson['uartConfig']['uart'][index][str(uart_bandrate)]

            fileObj.writelines(
                ("#define UART" + str(index) + "_RX_PORT").ljust(TABLE_SPACE) + uartArry[index].uart_rx_port + '\n')
            fileObj.writelines(
                ("#define UART" + str(index) + "_RX_PIN").ljust(TABLE_SPACE) + uartArry[index].uart_rx_pin + '\n')
            fileObj.writelines(
                ("#define UART" + str(index) + "_RX_LOC").ljust(TABLE_SPACE) + uartArry[index].uart_rx_loc + '\n')
            fileObj.writelines(
                ("#define UART" + str(index) + "_TX_PORT").ljust(TABLE_SPACE) + uartArry[index].uart_tx_port + '\n')
            fileObj.writelines(
                ("#define UART" + str(index) + "_TX_PIN").ljust(TABLE_SPACE) + uartArry[index].uart_tx_pin + '\n')
            fileObj.writelines(
                ("#define UART" + str(index) + "_TX_LOC").ljust(TABLE_SPACE) + uartArry[index].uart_tx_loc + '\n')
            fileObj.writelines(
                ("#define UART" + str(index) + "_BANDRATE").ljust(TABLE_SPACE) + str(uartArry[index].uart_bandrate) + '\n')
            fileObj.writelines('\n')
    print("uart config parse success")


# get io base info
def get_io_base_info():

    global ledInfo, relayInfo, keyInfo
    file = open('package.json', 'rb')
    fileJson = json.load(file, object_pairs_hook=collections.OrderedDict)
    ioConfigJson = fileJson['ioConfig']
    ledInfo["Enable"] = ioConfigJson['led_enable'] if 'led_enable' in ioConfigJson else "false"
    # print(ledInfo)
    ledInfo["Enable"] = ioConfigJson['led_enable'] if 'led_enable' in ioConfigJson else "false"
    ledInfo["Num"] = ioConfigJson['led_num'] if 'led_enable' in ioConfigJson else "false"
    relayInfo["Enable"] = ioConfigJson['relay_enable'] if 'relay_enable' in ioConfigJson else "false"
    relayInfo["Num"] = ioConfigJson['relay_num'] if 'relay_num' in ioConfigJson else "false"
    keyInfo["Enable"] = ioConfigJson['key_enable'] if 'key_enable' in ioConfigJson else "false"
    keyInfo["Num"] = ioConfigJson['key_num'] if 'key_num' in ioConfigJson else "false"
    file.close()
    # print(ledInfo, relayInfo, keyInfo)
    return fileJson


# get io config info 
def get_io_config_info(modInfo,fileObj,fileJs):
    
    print("io config parse start...")

    if modInfo["Enable"] == "true" and int(modInfo["Num"]) > 0:
        fileObj.writelines('\n' + '\n')
        fileObj.writelines("/* io config! */" + '\n')
        fileObj.writelines("/* " + modInfo["Name"] + " config! */" + '\n')

        outArry = [in_out()] * modInfo["Num"]
        for index in range(0, modInfo["Num"]):

            port,pin,mode,out,driver = fileJs['ioConfig'][modInfo["Name"]][index]

            outArry[index].port = fileJs['ioConfig'][modInfo["Name"]][index][str(port)]
            outArry[index].pin = fileJs['ioConfig'][modInfo["Name"]][index][str(pin)]
            outArry[index].mode = fileJs['ioConfig'][modInfo["Name"]][index][str(mode)]
            outArry[index].out = fileJs['ioConfig'][modInfo["Name"]][index][str(out)]
            outArry[index].driver = fileJs['ioConfig'][modInfo["Name"]][index][str(driver)]

            fileObj.writelines(
                ("#define " + port.upper()).ljust(TABLE_SPACE) + outArry[index].port + '\n')
            fileObj.writelines(
                ("#define " + pin.upper()).ljust(TABLE_SPACE) + outArry[index].pin + '\n')
            fileObj.writelines(
                ("#define " + mode.upper()).ljust(TABLE_SPACE) + outArry[index].mode + '\n')
            fileObj.writelines(
                ("#define " + out.upper()).ljust(TABLE_SPACE) + outArry[index].out + '\n')
            fileObj.writelines(
                ("#define " + driver.upper()).ljust(TABLE_SPACE) + outArry[index].driver + '\n')

            fileObj.writelines('\n')
    print("io config parse success")


# generate _callback.c in src dir
def ato_generate_callback_c():
    print("callbacks file generate start!")
    if os.path.exists(callback_file_source_path):
        if os.path.isfile(callback_file_source_path):
            if os.path.exists(callback_file_dest_path + name + "_callback.c"):
               print("callback file already exit...") 
               print("callbacks file generate success!")
            else:
                shutil.copy(callback_file_source_path, callback_file_dest_path + name + "_callback.c")
                print("callbacks file generate success!")
    else:
        print("error, file not exit!")
        exit

def toolsConfig(root, target, files):
    if(os.path.exists(files)):
        return
    items = os.listdir(root)
    for item in items:
        path = os.path.join(root, item)
        if os.path.isdir(path):
            toolsConfig(path, target, files)
        elif path.split('/')[-1] == target:
            root_dir = os.getcwd()
            
            target_dir = path.replace(target, '')
            os.chdir(target_dir)
            tools_dir = os.getcwd()
            os.chdir(root_dir)
            fileObject_mk = open(files, 'w')
            fileObject_mk.writelines("GCC_TOOL_PATH = " + tools_dir + '\n')

            return
        else:
            pass

if __name__ == '__main__':
    fimware_info_get()
    version_info_convert()
    ato_generate_config_c("config.c")
    ato_generate_config_h("config.h")
    ato_generate_config_mk("config.mk")
    ato_generate_callback_c()
    toolsConfig('../../../../../tools/gcc-arm-none-eabi-9-2019-q4-major/', 'arm-none-eabi-gcc', "makefile.conf")
