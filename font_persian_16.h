/*
 * font_persian_16.h
 *
 * Header for the LVGL 8.4 native font "font_persian_16"
 * (Vazirmatn Regular, 16 px, 4 bpp), matching the pattern of
 * font_persian_14.h.
 */

#ifndef FONT_PERSIAN_16_H
#define FONT_PERSIAN_16_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

#ifndef VAZIRMATN_16
#define VAZIRMATN_16 1
#endif

#if VAZIRMATN_16 == 0 /* to avoid asserts */
#error "VAZIRMATN_16 is not enabled. Enable it in font_persian_16.c"
#endif

extern const lv_font_t font_persian_16;

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /* FONT_PERSIAN_16_H */
