#include "iap.h"
#include "shell.h"
#include <string.h>

#define IAP_CONFIG_ADDR 0x0800C000U
#define IAP_CONFIG_SECTOR FLASH_SECTOR_3
const u8 check_sum_offset =0x27;

u8 check_sum_calc(struct BootConfig *bc)
{
	u8 check_sum=check_sum_offset;
	u8 *pbc= (u8*)bc;
	for(u8 i=0;i<sizeof(*bc)-1;i++)
	{
		check_sum ^=*(pbc+i); 
	}
	return check_sum;
}
void flash_sector_erase(u8 id)
{
	HAL_FLASH_Unlock();
	FLASH_EraseInitTypeDef f;
	f.TypeErase = FLASH_TYPEERASE_SECTORS;
	f.Sector = id;
	f.NbSectors=1;
	f.VoltageRange = VOLTAGE_RANGE_3;
	uint32_t PageError = 0;
	HAL_FLASHEx_Erase(&f, &PageError);
	HAL_FLASH_Lock();
}
bool is_erased(u32 addr,u32 len)
{
	for(u32 i=0;i<len;i++)
	{
		if(*(__IO u8*)addr != 0xff)
		{
			return FALSE;
		}
	}
	return TRUE;
}
void boot_config_save(struct BootConfig *bc)
{
	if(FALSE==is_erased(IAP_CONFIG_ADDR,sizeof(*bc)))
	{
		flash_sector_erase(IAP_CONFIG_SECTOR);
	}
	u8 *bc8=(u8*)bc;
	HAL_FLASH_Unlock();
	for(int i=0;i<sizeof(*bc);i++)
	{
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE,IAP_CONFIG_ADDR+i,*(bc8+i));
	}
	HAL_FLASH_Lock();
}
void boot_config_load(struct BootConfig *bc)
{
	memcpy(bc,(u8*)IAP_CONFIG_ADDR,sizeof(*bc));
}
bool entry_shell()
{
	
}
void iap_check()
{
	struct BootConfig bc;
	boot_config_load(&bc);
	struct BootConfig bc_default={
		{0x8010000,0x8020000},
		1,
		0x25
	};
	if(check_sum_calc(&bc)!=bc.check_sum)
	{
		//config invalid.
		boot_config_save(&bc_default);
	}
	u32 entry_ticks=HAL_GetTick();
	u32 time_out=3000;
	u8 state=0;
	bool exit=FALSE;
	while(FALSE==exit)
	{
		switch(state)
		{
			case 0:
				if(TRUE==entry_shell())
				{
					state=1;
				}
				if(HAL_GetTick()-entry_ticks>=time_out)
				{
					exit=TRUE;
				}
				break;
			case 1:
				if(FALSE==shell())
				{
					exit=TRUE;
				}
				break;
			default:
				break;
		}
	}
}
