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
#include "egb.h"
#include "snd.h"
#include "io.h"

#define TSUGARU_BREAK _inline(0xE6,0xEA);

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

struct EGB_String
{
	short x,y;
	unsigned short len;
	char str[1];
};

char buf[256];

void SetString(struct EGB_String *egbStr,int x,int y,char *str)
{
	strcpy(egbStr->str,str);
	egbStr->len=strlen(str);
	egbStr->x=x;
	egbStr->y=y;
}

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

int Pad(int *x,int *y,unsigned int *zoomx,unsigned int *zoomy,unsigned int *style,int status)
{
	if(0==(status&1))
	{
		if(0==(status&0x10))
		{
			(*zoomx)++;
			(*zoomy)+=2;
		}
		else
		{
			(*y)--;
		}
		return 1;
	}
	if(0==(status&2))
	{
		if(0==(status&0x10))
		{
			(*zoomx)--;
			(*zoomy)-=2;
		}
		else
		{
			(*y)++;
		}
		return 1;
	}
	if(0==(status&4))
	{
		if(0==(status&0x10))
		{
			(*style)--;
		}
		else
		{
			(*x)--;
		}
		return 1;
	}
	if(0==(status&8))
	{
		if(0==(status&0x10))
		{
			(*style)++;
		}
		else
		{
			(*x)++;
		}
		return 1;
	}
	return 0;
}

int main(void)
{
	struct EGB_String *str=(struct EGB_String *)buf;

	EGB_init(EGB_work,EgbWorkSize);

	EGB_resolution(EGB_work,0,3);
	EGB_resolution(EGB_work,1,10);

	EGB_writePage(EGB_work,1);
	EGB_displayStart(EGB_work,2,2,2);
	EGB_displayStart(EGB_work,3,320,240);
	EGB_clearScreen(EGB_work);

	EGB_writeMode(EGB_work,EGB_PSET);

	char arrow[3]={0xEB,0xC8,0};

	EGB_writePage(EGB_work,0);
	EGB_clearScreen(EGB_work);
	EGB_color(EGB_work,EGB_FOREGROUND_COLOR,15);
	SetString(str,32,32,arrow);


	TSUGARU_BREAK;
	EGB_sjisString(EGB_work,str);
	TSUGARU_BREAK;


	return 0;
}
