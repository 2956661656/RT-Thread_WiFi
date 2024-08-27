#include "ChipInfo.h"
#include "config.h"

uint32 ChipUID[3];

void GetChipID(void)
{
	ChipUID[0] = *(volatile uint32 *)0x1FFFF7F0;		//ID高32位
	ChipUID[1] = *(volatile uint32 *)0x1FFFF7EC;		//ID中间32位
	ChipUID[2] = *(volatile uint32 *)0x1FFFF7E8;		//ID低32位
	rt_kprintf("\nChipID:0x%08X-%08X-%08X\n", ChipUID[0], ChipUID[0], ChipUID[0]);
}
MSH_CMD_EXPORT(GetChipID, Get 96 bit unique chip ID);

void GetChipFlashCapacity(void)
{
	ChipUID[2] = *(volatile uint32 *)0x1FFFF7E8;		//ID低32位
	rt_kprintf("\nChip Flash Capacity:%dK \n", *(volatile uint16 *)0x1FFFF7E0);
}
MSH_CMD_EXPORT(GetChipFlashCapacity, Get chip flash capacity);
