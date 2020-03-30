/*
 * @Author: wls
 * @email: wuls@tuya.com
 * @LastEditors:   
 * @file name: light_toolkit.c
 * @Description: light common toolkit
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2019-05-06 10:57:08
 * @LastEditTime: 2019-05-24 20:34:29
 */

#include "light_types.h"
#include "light_tools.h"


/**
 * @berief: get the max value in 5 numbers
 * @param {UINT_T a
 *         UINT_T b
 *         UINT c
 *         UINT_T d
 *         UINT_T e
 *         5 numbers} 
 * @return: Max value
 * @retval: none
 */
UINT_T uiLightTool_GetMaxValue(UINT_T a, UINT_T b, UINT_T c, UINT_T d, UINT_T e)
{
    UINT_T x = a > b ? a : b;
    UINT_T y = c > d ? c : d;
    UINT_T z = x > y ? x : y;

    return (z > e ? z : e);

}

/**
 * @berief: get the absolute value
 * @param {value} 
 * @return: absolute value
 * @retval: none
 */
UINT_T uiLightTool_GetABSValue(INT_T value)
{
    return (value > 0 ? value : -value);

}

/**
 * @berief: ASSIC change to hex
 * @param {
 *          CHAR_T AscCode -> ASSIC code } 
 * @return: hex value
 * @retval: none
 */
UCHAR_T ucLightTool_Asc2Hex(CHAR_T AscCode)
{
    UCHAR_T ucResult = 0;

    if( '0' <= AscCode && AscCode <= '9' ) {
        ucResult = AscCode - '0';
    } else if ( 'a' <= AscCode && AscCode <= 'f' ) {
        ucResult = AscCode - 'a' + 10;
    } else if ( 'A' <= AscCode && AscCode <= 'F' ) {
        ucResult = AscCode - 'A' + 10;
    } else {
        ucResult = 0;
    }

    return ucResult;
}

/**
 * @berief: four unsigned char merge into unsigned short
 * @param {
 *              HH -> USHORT Hight hight 4bit
 *              HL -> USHORT Hight low 4bit
 *              HH -> USHORT low hight 4bit
 *              HL -> USHORT low low 4bit   } 
 * @return: USHORT value
 * @retval: none
 */
USHORT_T usLightTool_Str2Ushort(UCHAR_T HH, UCHAR_T HL, UCHAR_T LH, UCHAR_T LL)
{
    return ( (HH << 12) | (HL << 8) | (LH << 4) | (LL & 0x0F) );
}

/**
 * @berief: HSV change to RGB
 * @param {
 *              h -> 0~360
 *              s -> 0~1
 *              v -> 0~1
 *              *color_r -> R result
 *              *color_g -> G result
 *              *color_b -> B result 
 *              Range -> RGB range } 
 * @return: USHORT value
 * @retval: none
 */
VOID vLightTool_HSV2RGB(FLOAT_T h, FLOAT_T s, FLOAT_T v, USHORT_T *color_r, USHORT_T *color_g, USHORT_T *color_b, USHORT_T Range)
{
    FLOAT_T h60, f; 
    UINT_T h60f, hi;
    FLOAT_T p, q, t;
    FLOAT_T r, g, b;

    h60 = h / 60.0;
    h60f = h / 60;

    hi = (INT_T)h60f % 6;
    f = h60 - h60f;

    p = v * (1 - s);
    q = v * (1 - f * s);
    t = v * (1 - (1 - f) * s);

    r = g = b = 0;
    if(hi == 0) {
        r = v;          
        g = t;        
        b = p;
    } else if(hi == 1) {
        r = q;          
        g = v;        
        b = p;
    } else if(hi == 2) {
        r = p;          
        g = v;        
        b = t;
    } else if(hi == 3) {
        r = p;          
        g = q;        
        b = v;
    } else if(hi == 4) {
        r = t;          
        g = p;        
        b = v;
    } else if(hi == 5) {
        r = v;          
        g = p;        
        b = q;
    }

    r = (r * (FLOAT_T)Range);
    g = (g * (FLOAT_T)Range);
    b = (b * (FLOAT_T)Range);

    r *= 100;
    g *= 100;
    b *= 100;

    *color_r = (r + 50) / 100;
    *color_g = (g + 50) / 100;
    *color_b = (b + 50) / 100;
}

/**
 * @description: compare two string 
 * @param {IN} str1: string 1
 * @param {IN} str2: string 2
 * @return: 0: if these two string is not same,
 *          1: these two string is all same
 */
bool_t string_compare(IN char* str1,  IN char* str2)
{
    
    while(*str1 !='\0'&& *str2 != '\0')
    {
        if(*str1 != *str2){
            return 0;
        }
        str1++;
        str2++;
    }
    
    if(*str1 =='\0'&& *str2 == '\0')
      return 1;
    else
      return 0;
}