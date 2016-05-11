/*
    Nop90 - 2014

    swkbd 3DS - Custom Software Keyboard for 3DS
*/

#pragma once 

#include <3ds.h>

void swkbd_Init();
void swkbd_Exit();
int swkbd_AddSkin(u8* skinpath);
int swkbd_SetSkin(u8 skinnumber);
int swkbd_GetSkin();
void swkbd_SetFontColor(u32 color_0rfb);
void swkbd_SetBgColor(u32 color_0rfb);
void swkbd_SetTopScreenSourceBuf(void* buf);
void swkbd_GetStr(u8* buf, u32 buflen);


