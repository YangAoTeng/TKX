#ifndef __OLED_UI_MENUDATA_H
#define __OLED_UI_MENUDATA_H
// 检测是否是C++编译器
#ifdef __cplusplus
extern "C" {
#endif
#include "OLED_UI.h"

//进行前置声明
extern MenuItem MainMenuItems[],SettingsMenuItems[],AboutThisDeviceMenuItems[],
AboutOLED_UIMenuItems[],MoreMenuItems[],Font8MenuItems[] ,Font12MenuItems[] ,
Font16MenuItems[] ,Font20MenuItems[],LongMenuItems[],SpringMenuItems[],LongListMenuItems[],SmallAreaMenuItems[];
extern MenuPage MainMenuPage,SettingsMenuPage,AboutThisDeviceMenuPage,
AboutOLED_UIMenuPage,MoreMenuPage,Font8MenuPage,Font12MenuPage,Font16MenuPage
,Font20MenuPage,LongMenuPage,SpringMenuPage,LongListMenuPage,SmallAreaMenuPage;
extern MenuPage StartPage;


#ifdef __cplusplus
}  // extern "C"
#endif

#endif
