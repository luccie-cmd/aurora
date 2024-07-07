/*
 * Copyright (c) - All Rights Reserved.
 * 
 * See the LICENCE file for more information.
 */

#include "gsod.h"
#include <arch/fb/fb.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <math.h>

bool greenColor = true;

void GSOD(){
    uint64_t fb_width = FbGetWidth();
    uint64_t fb_heigth = FbGetHeight();
    seed(rand()*rand());
    for(uint64_t y = 0; y < fb_heigth; ++y){
        for(uint64_t x = 0; x < fb_width; ++x){
            if(x % 20 == 0) {
                greenColor = !greenColor;
            }
            uint32_t range = randRange(20);
            if(range == 0){
                range = 1;
            }
            if(y % range == 0){
                greenColor = !greenColor;
            }
            if(greenColor){
                FbPutPixel(x, y, 0, 128, 0);
            } else{
                FbPutPixel(x, y, 44, 44, 44);
            }
        }
    }
    InitFontSize(16);
    FbSetCursor(80, fb_heigth/5);
    uint64_t cursorPosX, cursorPosY;
    FbGetCursor(&cursorPosX, &cursorPosY);
    printf("System fault triggerd a green screen of death:");
    FbSetCursor(cursorPosX, cursorPosY+18);
    printf("Reason: %s", strerr(errno));
    FbSetCursor(cursorPosX, cursorPosY+36);
    if(errnoInfo != NULL){
        printf("Extra info: %s", errnoInfo);
    }
}