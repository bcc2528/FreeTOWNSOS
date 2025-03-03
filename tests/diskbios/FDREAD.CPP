#include <stdio.h>
#include <conio.h>
#include <dos.h>
#include <i86.h>
#include "diskbios.h"

unsigned char buf[4096];


//   221                              <1> INT93H_FD_READ:
//   222 00000263 8B5E28              <1> 						MOV		BX,[BP+SAVED_DS]
//   223 00000266 8EDB                <1> 						MOV		DS,BX
//   224 00000268 8B7E02              <1> 						MOV		DI,[BP+SAVED_EDI]


// flicker screen by palette.
const unsigned char hackCode[]=
{
0xBA,0x48,0x04,0xB0,0x01,0xEE,0xBA,0x4A,0x04,0xB0,0x25,0xEE,0xBA,0x90,0xFD,0xB0,0x07,0xEE,0xBA,0x92,0xFD,0xB0,0xFF,0xEE,0xBA,0x94,0xFD,0xB0,0x00,0xEE,0xBA,0x96,0xFD,0xB0,0x00,0xEE,0xBA,0x48,0x04,0xB0,0x01,0xEE,0xBA,0x4A,0x04,0xB0,0x05,0xEE,0xBA,0x90,0xFD,0xB0,0x07,0xEE,0xBA,0x92,0xFD,0xB0,0xFF,0xEE,0xBA,0x94,0xFD,0xB0,0x00,0xEE,0xBA,0x96,0xFD,0xB0,0x00,0xEE,0xBA,0x48,0x04,0xB0,0x01,0xEE,0xBA,0x4A,0x04,0xB0,0x25,0xEE,0xBA,0x90,0xFD,0xB0,0x07,0xEE,0xBA,0x92,0xFD,0xB0,0x00,0xEE,0xBA,0x94,0xFD,0xB0,0xFF,0xEE,0xBA,0x96,0xFD,0xB0,0x00,0xEE,0xBA,0x48,0x04,0xB0,0x01,0xEE,0xBA,0x4A,0x04,0xB0,0x05,0xEE,0xBA,0x90,0xFD,0xB0,0x07,0xEE,0xBA,0x92,0xFD,0xB0,0x00,0xEE,0xBA,0x94,0xFD,0xB0,0xFF,0xEE,0xBA,0x96,0xFD,0xB0,0x00,0xEE,0xFB,0xE9,0x6C,0xFF
};

unsigned char far *INT93H(void)
{
	unsigned short far *idt;
	idt=(unsigned short far *)MK_FP(0,0);

	unsigned short cs=idt[0x93*2+1];
	unsigned char far *int93h;
	return (unsigned char far *)MK_FP(cs,0);
}

void HackMyBIOS(void)
{
	unsigned char far *int93h=INT93H();
	unsigned short addr=0x2D3;

	printf("%d bytes of hack code.\n",sizeof(hackCode));
	printf("%04x:%04x\n",_FP_SEG(int93h),addr);
	for(int i=0; i<16; ++i)
	{
		printf("%02x ",int93h[addr+i]);
	}
	printf("\n");

	for(int i=0; i<20000; ++i)// Let console refresh.
	{
		outp(0x6C,0);
	}

	unsigned char AL=inp(0xA8); // DMA Active Status.  Must be 0x20
	printf("A8H %02x\n",AL);
	outp(0xA8,0x20);
	outp(0xA1,0); // Select Channel 0
	AL=inp(0xA8); // DMA Active Status.  Must be 0x20
	printf("A8H %02x (After select ch0)\n",AL);

//	// Inject flicker.
//	for(int i=0; i<sizeof(hackCode); ++i)
//	{
//		int93h[addr+i]=hackCode[i];
//	}
}

void PrintInternalDebugInfo(void)
{
	unsigned char far *int93h=INT93H();

	//    22 000000E9 00                  <1> INT93H_FD_LAST_STATUS_BYTE	DB		0
	//    23 000000EA 00                  <1> INT93H_FD_LAST_CONTROL_BYTE	DB		0
	//    24 000000EB 00                  <1> INT93H_FD_LAST_SPEED_BYTE	DB		0
	//    25 000000EC 0000                <1> INT93H_FD_DMA_COUNT_BEFORE	DW		0
	//    26 000000EE 0000                <1> INT93H_FD_DMA_COUNT_AFTER	DW		0

	printf("Last Status Byte %02x\n",int93h[0xE9]);
	printf("Last Control Byte %02x\n",int93h[0xEA]);
	printf("Last Speed Byte %02x\n",int93h[0xEB]);
	printf("Last DMA Count Before Command %04x\n",*(unsigned short far *)(int93h+0xEC));
	printf("Last DMA Count After  Command %04x\n",*(unsigned short far *)(int93h+0xEE));
}

int main(void)
{
	int actual=0;
	printf("DKB_restore returns %04xH\n",DKB_restore(BIOS_FD0));

	HackMyBIOS();
	printf("DKB_read returns %04xH\n",DKB_read(BIOS_FD0,0,0,20, 1,(char *)buf,&actual));
	// The sector that does not exist.  Should return record not found.

	printf("DKB_restore returns %04xH\n",DKB_restore(BIOS_FD0));

	unsigned DMACount=inpw(0xA2);
	unsigned DMAAddrLow=inpw(0xA4);
	unsigned DMAAddrHigh=inpw(0xA6);
	printf("DMA Before Count=%04x  AddrLow=%04x  AddrHigh=%04x\n",DMACount,DMAAddrLow,DMAAddrHigh);

	printf("DKB_read returns %04xH\n",DKB_read(BIOS_FD0,0,0,1, 1,(char *)buf,&actual));
	printf("actual number of sectors read=%d\n",actual);

	DMACount=inpw(0xA2);
	DMAAddrLow=inpw(0xA4);
	DMAAddrHigh=inpw(0xA6);
	printf("DMA After  Count=%04x  AddrLow=%04x  AddrHigh=%04x\n",DMACount,DMAAddrLow,DMAAddrHigh);

	printf("Showing first 128 bytes.\n");
	for(int i=0; i<128; ++i)
	{
		printf("%02x ",buf[i]);
		if(15==i%16)
		{
			printf("\n");
		}
	}

	PrintInternalDebugInfo();

	return 0;
}
