// #ifndef LV_CONF_H
// #define LV_CONF_H
// #define LV_USE_PNG 1


// /*====================
//    Graphical settings
//  *====================*/
// #define LV_COLOR_DEPTH 16
// #define LV_HOR_RES_MAX 240
// #define LV_VER_RES_MAX 320

// /*====================
//    Memory settings
//  *====================*/
// #define LV_MEM_CUSTOM 0
// #define LV_USE_USER_DATA 1

// /*====================
//    Feature settings
//  *====================*/
// #define LV_USE_PERF_MONITOR 0
// #define LV_USE_LOG 0
// #define LV_USE_GPU_STM32_DMA2D 0

// /*====================
//    Tick settings
//  *====================*/
// #define LV_TICK_CUSTOM 0

// #define LV_USE_EXTRA 0  // Disable all extra modules


// /*====================
//    Font settings
//  *====================*/
// #define LV_FONT_MONTSERRAT_14 1  // For small text
// #define LV_FONT_MONTSERRAT_18 1  // For status text
// #define LV_FONT_MONTSERRAT_48 1  // For status icon
// #define LV_FONT_MONTSERRAT_8  0
// #define LV_FONT_MONTSERRAT_10 0
// #define LV_FONT_MONTSERRAT_12 0
// #define LV_FONT_MONTSERRAT_16 0
// #define LV_FONT_MONTSERRAT_20 0
// #define LV_FONT_MONTSERRAT_22 0
// #define LV_FONT_MONTSERRAT_24 0
// #define LV_FONT_MONTSERRAT_26 0
// #define LV_FONT_MONTSERRAT_28 0
// #define LV_FONT_MONTSERRAT_30 0
// #define LV_FONT_MONTSERRAT_32 0
// #define LV_FONT_MONTSERRAT_34 0
// #define LV_FONT_MONTSERRAT_36 0
// #define LV_FONT_MONTSERRAT_38 0
// #define LV_FONT_MONTSERRAT_40 0
// #define LV_FONT_MONTSERRAT_42 0
// #define LV_FONT_MONTSERRAT_44 0
// #define LV_FONT_MONTSERRAT_46 0

// // Default fonts
// #define LV_FONT_DEFAULT &lv_font_montserrat_14

// #endif
#ifndef LV_CONF_H
#define LV_CONF_H

/*====================
   Enable/Disable modules
 *====================*/
#define LV_USE_PNG 1

/* Disable all extra modules (this is key!) */
#define LV_USE_EXTRA 0

/*====================
   Graphical settings
 *====================*/
#define LV_COLOR_DEPTH 16
#define LV_HOR_RES_MAX 240
#define LV_VER_RES_MAX 320

/*====================
   Memory settings
 *====================*/
#define LV_MEM_CUSTOM 0
#define LV_USE_USER_DATA 1

/*====================
   Core features - DISABLE ALL
 *====================*/
#define LV_USE_PERF_MONITOR 0
#define LV_USE_LOG 0
#define LV_USE_GPU_STM32_DMA2D 0
#define LV_USE_ANIMATION 0
#define LV_USE_SHADOW 0
#define LV_USE_OPA_SCALE 0
#define LV_USE_GROUP 0
#define LV_USE_OBJ_REALIGN 0
#define LV_USE_FS_STDIO 0
#define LV_USE_FS_POSIX 0
#define LV_USE_FS_WIN32 0
#define LV_USE_FS_FATFS 0
#define LV_USE_FS_LITTLEFS 0

/*====================
   Widgets - ONLY WHAT YOU NEED
 *====================*/
#define LV_USE_BTN 1
#define LV_USE_LABEL 1
#define LV_USE_IMG 0
#define LV_USE_ARC 0
#define LV_USE_BAR 0
#define LV_USE_BTNMATRIX 0
#define LV_USE_CALENDAR 0
#define LV_USE_CANVAS 0
#define LV_USE_CHART 0
#define LV_USE_CHECKBOX 0
#define LV_USE_DROPDOWN 0
#define LV_USE_LINE 0
#define LV_USE_LIST 0
#define LV_USE_METER 0
#define LV_USE_ROLLER 0
#define LV_USE_SLIDER 0
#define LV_USE_SPINBOX 0
#define LV_USE_SPINNER 0
#define LV_USE_SWITCH 0
#define LV_USE_TABLE 0
#define LV_USE_TABVIEW 0
#define LV_USE_TEXTAREA 0
#define LV_USE_TILEVIEW 0
#define LV_USE_WIN 0
#define LV_USE_PAGE 0
#define LV_USE_CONT 0

/*====================
   Extra widgets - DISABLE ALL
 *====================*/
#define LV_USE_ANIMIMG 0
#define LV_USE_CALENDAR_HEADER_ARROW 0
#define LV_USE_CALENDAR_HEADER_DROPDOWN 0
#define LV_USE_CHART 0
#define LV_USE_COLORWHEEL 0
#define LV_USE_IMGBTN 0
#define LV_USE_KEYBOARD 0
#define LV_USE_LED 0
#define LV_USE_MENU 0
#define LV_USE_METER 0
#define LV_USE_MSGBOX 0
#define LV_USE_SPAN 0
#define LV_USE_SPINBOX 0
#define LV_USE_TABVIEW 0
#define LV_USE_TILEVIEW 0
#define LV_USE_WIN 0

/*====================
   Layouts - DISABLE
 *====================*/
#define LV_USE_FLEX 1
#define LV_USE_GRID 0

/*====================
   Themes - Keep minimal
 *====================*/
#define LV_USE_THEME_BASIC 0
#define LV_USE_THEME_DEFAULT 0
#define LV_USE_THEME_MONO 1

/*====================
   Font settings - MINIMAL
 *====================*/
#define LV_FONT_MONTSERRAT_14 1
#define LV_FONT_MONTSERRAT_18 1
#define LV_FONT_MONTSERRAT_48 1
#define LV_FONT_MONTSERRAT_8  0
#define LV_FONT_MONTSERRAT_10 0
#define LV_FONT_MONTSERRAT_12 0
#define LV_FONT_MONTSERRAT_16 0
#define LV_FONT_MONTSERRAT_20 0
#define LV_FONT_MONTSERRAT_22 0
#define LV_FONT_MONTSERRAT_24 0
#define LV_FONT_MONTSERRAT_26 0
#define LV_FONT_MONTSERRAT_28 0
#define LV_FONT_MONTSERRAT_30 0
#define LV_FONT_MONTSERRAT_32 0
#define LV_FONT_MONTSERRAT_34 0
#define LV_FONT_MONTSERRAT_36 0
#define LV_FONT_MONTSERRAT_38 0
#define LV_FONT_MONTSERRAT_40 0
#define LV_FONT_MONTSERRAT_42 0
#define LV_FONT_MONTSERRAT_44 0
#define LV_FONT_MONTSERRAT_46 0

/* Disable built-in fonts */
#define LV_FONT_DEJAVU_16_PERSIAN_HEBREW 0
#define LV_FONT_SIMSUN_16_CJK 0
#define LV_FONT_UNSCII_8 0

#define LV_FONT_DEFAULT &lv_font_montserrat_14

/*====================
   Tick settings
 *====================*/
#define LV_TICK_CUSTOM 0

#endif