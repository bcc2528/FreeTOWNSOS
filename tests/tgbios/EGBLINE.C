/* LICENSE>>
Copyright 2020 Soji Yamakawa (CaptainYS, http://www.ysflight.com)

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

<< LICENSE */
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "egb.h"
#include "snd.h"
#include "io.h"

static char EGB_work[EgbWorkSize];

void SetScreenMode(int m1,int m2);

// Test 16-color mode

#define EGB_FOREGROUND_COLOR 0
#define EGB_BACKGROUND_COLOR 1
#define EGB_FILL_COLOR 2
#define EGB_TRANSPARENT_COLOR 3

#define EGB_PSET 0
#define EGB_PRESET 1
#define EGB_OR 2
#define EGB_AND 3
#define EGB_XOR 4
#define EGB_NOT 5
#define EGB_MATTE 6
#define EGB_PASTEL 7
#define EGB_OPAGUE 9
#define EGB_MASKSET 13
#define EGB_MASKRESET 14
#define EGB_MASKNOT 15

void WaitForPad(void)
{
	int status=0xFF;
	while(0x30==(status&0x30))
	{
		SND_joy_in_2(0,&status);
	}
	while(0x30!=(status&0x30))
	{
		SND_joy_in_2(0,&status);
	}
}



void Wait3Sec(void)
{
	unsigned int accum=0;
	clock_t t0=clock();
	while(accum<CLOCKS_PER_SEC*3)
	{
		clock_t t1=clock();
		accum+=t1-t0;
		t0=t1;
	}
}

void TestLINE(
	unsigned int lineColor,
	unsigned int scrnWid,
	unsigned int scrnHei,
	int mode)
{
	int i;
	short line[5];

	EGB_color(EGB_work,EGB_FOREGROUND_COLOR,lineColor);
	EGB_writeMode(EGB_work,mode);
	EGB_paintMode(EGB_work,0x02);
	for(i=-10; i<=10; ++i)
	{
		line[0]=2;
		line[1]=0+16*i;
		line[2]=0-16*i;
		line[3]=scrnWid+16*i;
		line[4]=scrnHei-16*i;
		EGB_connect(EGB_work,line);
	}

	for(i=-10; i<=10; ++i)
	{
		line[0]=2;
		line[1]=0+16*i;
		line[2]=scrnHei-16*i;
		line[3]=scrnWid+16*i;
		line[4]=0-16*i;
		EGB_connect(EGB_work,line);
	}

	for(i=-10; i<=10; ++i)
	{
		line[0]=2;
		line[1]=scrnWid/2-10*i;
		line[2]=-10;
		line[3]=scrnWid/3;
		line[4]=scrnHei+10;
		EGB_connect(EGB_work,line);
	}
}

void Test4Bit(void)
{
	EGB_resolution(EGB_work,0,3);
	EGB_resolution(EGB_work,1,3);

	EGB_writePage(EGB_work,1);
	EGB_clearScreen(EGB_work);

	EGB_writePage(EGB_work,0);
	EGB_clearScreen(EGB_work);

	TestLINE(15,640,480,EGB_PSET);

	Wait3Sec();
}

void Test8Bit(void)
{
	EGB_resolution(EGB_work,0,12);

	EGB_writePage(EGB_work,0);
	EGB_clearScreen(EGB_work);

	TestLINE(255,640,480,EGB_PSET);

	Wait3Sec();
}

void Test16Bit(void)
{
	EGB_resolution(EGB_work,0,10);
	EGB_resolution(EGB_work,1,10);

	EGB_writePage(EGB_work,1);
	EGB_clearScreen(EGB_work);

	EGB_writePage(EGB_work,0);
	EGB_clearScreen(EGB_work);

	TestLINE(32767,640,480,EGB_PSET);

	Wait3Sec();
}

int main(void)
{
	EGB_init(EGB_work,EgbWorkSize);

	Test4Bit();
	Test8Bit();
	Test16Bit();

	return 0;
}
