#ifndef _iap_h
#define _iap_h
#include "public.h"
#pragma pack(1)
struct BootConfig
{
	u32 app_addr[2];
	u8 app_load_index;
	u8 check_sum;
};
#pragma pack()

void iap_check();
#endif
