# coding:utf-8

import sys
import os
import logging
import re
import json
import string
      
class uart:
    def __init__(self):
        self.uart_rx_port = ''
        self.uart_rx_pin = ''
        self.uart_rx_loc = ''
        self.uart_tx_port = ''
        self.uart_tx_pin = ''
        self.uart_tx_loc = ''
        self.uart_bandrate = 9600

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


def fimwareInfoGet():
    global name
    global description
    global version
    global bv_version
    global ic
    global ota_image_type
    global model_id
    global pid
    global manufacture_name
    global manufacture_id

    print("fimware information parse start...")
    file = open('package.json', 'rb');
    fileJson = json.load(file)
    name = fileJson['fimwareInfo']['name']
    description = fileJson['fimwareInfo']['description']
    version = fileJson['fimwareInfo']['version']
    bv_version = fileJson['fimwareInfo']['bv_version']
    ic = fileJson['fimwareInfo']['ic']
    ota_image_type = fileJson['fimwareInfo']['ota_image_type']
    model_id = fileJson['fimwareInfo']['model_id']
    pid = fileJson['fimwareInfo']['pid']
    manufacture_name = fileJson['fimwareInfo']['manufacture_name']
    manufacture_id = fileJson['fimwareInfo']['manufacture_id']

    print("fimware information parse success")


def versionConvert():
    global version_hex
    str = version.replace('.', '')
    versionLen = str.__len__()
    version_hex = ((int(str[0:1]) << 6) & 0xc0) + ((int(str[1:2]) << 2) & 0x30)
    if (version_hex > 3):
        version_hex = version_hex + (int(str[2:4]) & 0x0f)
    else:
        version_hex = version_hex + (int(str[2:3]) & 0x0f)
    # print(hex(version_hex))


def cfgWriteFile():
    print("config file write start...")
    fileObject = open("config.c", 'w')
    fileObject.writelines(
        "/*************************************************************************************/" + '\n')
    fileObject.writelines("/* Automatically-generated file. Do not edit! */" + '\n')
    fileObject.writelines(
        "/*************************************************************************************/" + '\n')
    fileObject.writelines("#include \"zigbee_sdk.h\"" + '\n')
    fileObject.writelines("#include \"config.h\"" + '\n\n\n')
    fileObject.writelines("char *g_firmware_name = FIRMWARE_NAME;" + '\n')
    fileObject.writelines("uint8_t g_firmware_ver = FIRMWARE_VER;" + '\n')
    fileObject.writelines("char *g_sdk_ver = SDK_VER;" + '\n')
    fileObject.writelines("void firmware_config(void)" + '\n')
    fileObject.writelines("{" + '\n')
    fileObject.writelines("    dev_register_base_info(MODEL_ID, PRODUCTOR_ID_PFEFIX, PRODUCTOR_ID);" + '\n')
    fileObject.writelines("}" + '\n')
    fileObject = open("config.h", 'w')
    fileObject.writelines(
        "/*************************************************************************************/" + '\n')
    fileObject.writelines("/* Automatically-generated file. Do not edit! */" + '\n')
    fileObject.writelines(
        "/*************************************************************************************/" + '\n')
    fileObject.writelines("#ifndef __CONFIG_H__" + '\n')
    fileObject.writelines("#define __CONFIG_H__" + '\n' + '\n')
    fileObject.writelines("/* firmware information! */" + '\n')
    fileObject.writelines("#define FIRMWARE_NAME".ljust(TABLE_SPACE) + '\"' + name + '\"' + '\n')
    fileObject.writelines("#define FIRMWARE_VER".ljust(TABLE_SPACE) + hex(version_hex) + '\n')
    fileObject.writelines("#define PRODUCTOR_ID_PFEFIX".ljust(TABLE_SPACE) + '\"' + manufacture_name + '\"' + '\n')
    fileObject.writelines("#define BV_VERSION".ljust(TABLE_SPACE) + '\"' + bv_version + '\"' + '\n')
    fileObject.writelines("#define IC".ljust(TABLE_SPACE) + '\"' + ic + '\"' + '\n')
    fileObject.writelines("#define OTA_IMAGE_TYPE".ljust(TABLE_SPACE) + ota_image_type + '\n')
    fileObject.writelines("#define MODEL_ID".ljust(TABLE_SPACE) + '\"' + model_id + '\"' + '\n')
    fileObject.writelines("#define PRODUCTOR_ID".ljust(TABLE_SPACE) + '\"' + pid + '\"' + '\n')
    fileObject.writelines("#define SDK_VER".ljust(TABLE_SPACE) + '\"' + bv_version + '\"' + '\n')

    fileObject_mk = open("config.mk", 'w')
    fileObject_mk.writelines("OBJ_DIR = ../smart_plug/obj" + '\n')
    fileObject_mk.writelines("TARGET = " + name + '\n')
    fileObject_mk.writelines("STR_VER = " + version + '\n')
    fileObject_mk.writelines("HEX_VER = " + hex(version_hex) + '\n')
    fileObject_mk.writelines("OTA_IMAGE_TYPE = " + ota_image_type + '\n')
    fileObject_mk.writelines("MANUF_ID = " + manufacture_id + '\n')

    fileObject_mk = open("firmware_ver_temp_file", 'w')
    fileObject_mk.writelines(version)
    fileObject_mk = open("firmware_name_temp_file", 'w')
    fileObject_mk.writelines(name)
    print("config file write success")


def uartCfgGet():
    print("uart config parse start...")
    index = 0
    file = open('package.json', 'rb');
    fileJson = json.load(file)
    uartEnable = fileJson['uartConfig']['uart_enable']
    uartNum = fileJson['uartConfig']['uart_num']

    if uartEnable == "true" and int(uartNum) > 0:
        fileObject = open("config.h", 'a+')
        fileObject.writelines('\n' + '\n')
        fileObject.writelines("/* uart config! */" + '\n')

        fileObject.writelines("#define UART_ENABLE".ljust(TABLE_SPACE) + uartEnable + '\n')
        fileObject.writelines("#define UART_NUM".ljust(TABLE_SPACE) + hex(uartNum) + '\n')
        fileObject.writelines('\n')

        uartArry = [uart()] * uartNum;
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

            fileObject.writelines(
                ("#define UART" + str(index) + "_RX_PORT").ljust(TABLE_SPACE) + uartArry[index].uart_rx_port + '\n')
            fileObject.writelines(
                ("#define UART" + str(index) + "_RX_PIN").ljust(TABLE_SPACE) + uartArry[index].uart_rx_pin + '\n')
            fileObject.writelines(
                ("#define UART" + str(index) + "_RX_LOC").ljust(TABLE_SPACE) + uartArry[index].uart_rx_loc + '\n')

            fileObject.writelines(
                ("#define UART" + str(index) + "_TX_PORT").ljust(TABLE_SPACE) + uartArry[index].uart_tx_port + '\n')
            fileObject.writelines(
                ("#define UART" + str(index) + "_TX_PIN").ljust(TABLE_SPACE) + uartArry[index].uart_tx_pin + '\n')
            fileObject.writelines(
                ("#define UART" + str(index) + "_TX_LOC").ljust(TABLE_SPACE) + uartArry[index].uart_tx_loc + '\n')

            fileObject.writelines(("#define UART" + str(index) + "_BANDRATE").ljust(TABLE_SPACE) + str(
                uartArry[index].uart_bandrate) + '\n')
            fileObject.writelines('\n')

    print("uart config parse success")


def ioCfgGet():
    print("io config parse start...")
    index = 0
    file = open('package.json', 'rb');
    fileJson = json.load(file)
    ledEnable = fileJson['ioConfig']['led_enable']
    ledNum = fileJson['ioConfig']['led_num']
    keyEnable = fileJson['ioConfig']['key_enable']
    keyNum = fileJson['ioConfig']['key_num']

    if ledEnable == "true" and int(ledNum) > 0:
        fileObject = open("./config.h", 'a+')
        fileObject.writelines('\n' + '\n');
        fileObject.writelines("/* io config! */" + '\n')
        fileObject.writelines("/* led config! */" + '\n')

        outArry = [in_out()] * ledNum
        for index in range(0, ledNum):
            led_out_port = "led" + str(index) + "_port"
            led_out_pin = "led" + str(index) + "_pin"
            led_out_mode = "led" + str(index) + "_mode"
            led_out = "led" + str(index) + "_out"
            led_driver = "led" + str(index) + "_driver"

            outArry[index].port = fileJson['ioConfig']['led'][index][str(led_out_port)]
            outArry[index].pin = fileJson['ioConfig']['led'][index][str(led_out_pin)]
            outArry[index].mode = fileJson['ioConfig']['led'][index][str(led_out_mode)]
            outArry[index].out = fileJson['ioConfig']['led'][index][str(led_out)]
            outArry[index].driver = fileJson['ioConfig']['led'][index][str(led_driver)]

            fileObject.writelines(
                ("#define LED" + str(index) + "_PORT").ljust(TABLE_SPACE) + outArry[index].port + '\n')
            fileObject.writelines(
                ("#define LED" + str(index) + "_PIN").ljust(TABLE_SPACE) + outArry[index].pin + '\n')
            fileObject.writelines(
                ("#define LED" + str(index) + "_MODE").ljust(TABLE_SPACE) + outArry[index].mode + '\n')
            fileObject.writelines(
                ("#define LED" + str(index) + "_DOUT").ljust(TABLE_SPACE) + outArry[index].out + '\n')
            fileObject.writelines(
                ("#define LED" + str(index) + "_DRIVE").ljust(TABLE_SPACE) + outArry[index].driver + '\n')

            fileObject.writelines('\n')

    if keyEnable == "true" and int(keyNum) > 0:
        fileObject = open("./config.h", 'a+')
        fileObject.writelines('\n');
        fileObject.writelines("/* key config! */" + '\n')

        keyArry = [in_out()] * keyNum
        for index in range(0, keyNum):
            key_port = "key" + str(index) + "_port"
            key_pin = "key" + str(index) + "_pin"
            key_mode = "key" + str(index) + "_mode"
            key_default = "key" + str(index) + "_out"
            key_driver = "key" + str(index) + "_driver"

            keyArry[index].port = fileJson['ioConfig']['key'][index][str(key_port)]
            keyArry[index].pin = fileJson['ioConfig']['key'][index][str(key_pin)]
            keyArry[index].mode = fileJson['ioConfig']['key'][index][str(key_mode)]
            keyArry[index].out = fileJson['ioConfig']['key'][index][str(key_default)]
            keyArry[index].driver = fileJson['ioConfig']['key'][index][str(key_driver)]

            fileObject.writelines(
                ("#define KEY" + str(index) + "_PORT").ljust(TABLE_SPACE) + keyArry[index].port + '\n')
            fileObject.writelines(
                ("#define KEY" + str(index) + "_PIN").ljust(TABLE_SPACE) + keyArry[index].pin + '\n')
            fileObject.writelines(
                ("#define KEY" + str(index) + "_MODE").ljust(TABLE_SPACE) + keyArry[index].mode + '\n')
            fileObject.writelines(
                ("#define KEY" + str(index) + "_DOUT").ljust(TABLE_SPACE) + keyArry[index].out + '\n')
            fileObject.writelines(
                ("#define KEY" + str(index) + "_DRIVE").ljust(TABLE_SPACE) + keyArry[index].driver + '\n')

            fileObject.writelines('\n')

    fileObject.writelines('\n' + "#endif" + '\n')
    print("io config parse success")


if __name__ == '__main__':
    fimwareInfoGet()
    versionConvert()
    cfgWriteFile()
    uartCfgGet()
    ioCfgGet()
