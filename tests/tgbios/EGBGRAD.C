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

/*
#include "snd.h"
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
} */

void GetRainbowColor(double rgb[3],double t)
{
	if(t<=0.0)
	{
		rgb[0]=0;
		rgb[1]=0;
		rgb[2]=1;
	}
	else if(1.0<=t)
	{
		rgb[0]=1;
		rgb[1]=0;
		rgb[2]=0;
	}
	else if(t<=0.25)
	{
		t/=0.25;
		rgb[0]=0;
		rgb[1]=t;
		rgb[2]=1;
	}
	else if(t<=0.5)
	{
		t=(t-0.25)/0.25;
		rgb[0]=0;
		rgb[1]=1;
		rgb[2]=1-t;
	}
	else if(t<=0.75)
	{
		t=(t-0.5)/0.25;
		rgb[0]=t;
		rgb[1]=1;
		rgb[2]=0;
	}
	else //if(t<=1.0)
	{
		t=(t-0.75)/0.25;
		rgb[0]=1;
		rgb[1]=1-t;
		rgb[2]=0;
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

#define EGB_SCALING 2
#define EGB_VIEWAREA 3

int main(void)
{
	EGB_init(EGB_work,EgbWorkSize);

	EGB_resolution(EGB_work,0,10);
	EGB_resolution(EGB_work,1,10);

	EGB_writePage(EGB_work,1);
	EGB_clearScreen(EGB_work);
	EGB_displayStart(EGB_work,EGB_SCALING,2,2);
	EGB_displayStart(EGB_work,EGB_VIEWAREA,320,240);

	EGB_writePage(EGB_work,0);
	EGB_clearScreen(EGB_work);
	EGB_displayStart(EGB_work,EGB_SCALING,2,2);
	EGB_displayStart(EGB_work,EGB_VIEWAREA,320,240);

	for(int x=0; x<320; ++x)
	{
		double t=(double)x/320.0;
		double rgb[3];
		GetRainbowColor(rgb,t);

		unsigned short col32k,r,g,b;
		r=(int)(rgb[0]*31.0);
		g=(int)(rgb[1]*31.0);
		b=(int)(rgb[2]*31.0);
		col32k=(g<<10)|(r<<5)|b;

		EGB_color(EGB_work,EGB_FOREGROUND_COLOR,col32k);

		short line[5];
		line[0]=2;			// 2 points
		line[1]=x;
		line[2]=0;
		line[3]=x;
		line[4]=239;
		EGB_connect(EGB_work,line);
	}

	Wait3Sec();

	return 0;
}
