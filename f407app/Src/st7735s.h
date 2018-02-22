#ifndef _st7735s_h
#define _st7735s_h
#include "public.h"
void lcd_clear(void);
void lcd_init(void);
void lcd_show(void);
void set_color(u8 r,u8 g,u8 b);
void lcd_point(u16 x,u16 y,u16 c);
void lcd_str(char * str,u16 x,u16 y);
void lcd_num(long n,u16 x,u16 y);
void lcd_vline(u8 x1,u8 y1, u8 x2,u8 y2);
void lcd_box(u8 x,u8 y,u8 w,u8 h);
#endif
