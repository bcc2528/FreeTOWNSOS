#include <fssimplewindow.h>
#include <algorithm>

#define VRAMW 640
#define VRAMH 480

unsigned char vram[VRAMH*VRAMW*4];

int duckywid=32;
int duckyhei=32;
// 15-bit RGBA
unsigned short duck[32*32]={
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0, 12672, 19008, 22176, 22176, 22176, 21120, 10560,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0, 16896, 23232, 26400, 27456, 28512, 27456, 26400, 23232, 19008,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0, 14784, 21120, 25344, 28512, 29568, 30624, 30624, 29568, 28512, 24288, 17952,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,  2112, 20064, 24288, 27456, 30624, 31680, 32736, 32736, 31680, 29568, 27456, 22176,
 14784,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0, 13728, 21120,    27,    27, 31681, 32744, 32745, 32737, 31680, 30624, 28512, 24288,
 19008,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0, 16896, 21120,    24,    27, 31681, 32748, 32755, 32749, 32741, 31680, 29568, 26400,
 20064,  8448,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0, 17952, 22176,    24,    26, 31681, 32748, 32759, 32760, 32749, 32738, 28512, 25344,
 20064, 12672,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,  5509,  8775, 23232, 24288, 26400, 28512, 31682, 32748, 32754, 32752, 32741, 27456, 24288,
 20064, 12672,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,  6630,  8808, 10985, 11050, 24288, 26400, 27456, 29569, 32745, 32743, 31685, 26400, 22176,
 17952,  9504,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,  5509,  6630,  7719,  8807,  9864,  9929, 24288, 26400, 26400, 27456, 26400, 25344, 23232, 19008,
 14784,  6336,     0,     0,     0,     0,     0,     0, 14784, 14784,     0,     0,     0,     0,     0,     0,
     0,     0,  3299,  4420,  5477,  6565,  6630,  7719, 21120, 22176, 23232, 24288, 23232, 22176, 20064, 15840,
 11616,     0,     0,     0,     0,     0,     0, 17952, 17952, 16896, 16896,     0,     0,     0,     0,     0,
     0,     0,     0,     0,  1121,  2210,  9504, 12672, 16896, 19008, 20064, 20064, 20064, 17952, 16896, 17952,
 14784, 14784, 17952, 17952, 17952, 17952, 17952, 17952, 17952, 17952, 17952, 17952,     0,     0,     0,     0,
     0,     0,     0,     0,     0,  4224,  8448, 12672, 14784, 16896, 17952, 20064, 22176, 23232, 24288, 24288,
 21120, 23232, 23232, 22176, 22176, 22176, 22176, 22176, 22176, 22176, 22176, 22176, 17952,     0,     0,     0,
     0,     0,     0,     0,     0,     0, 10560, 16896, 20064, 23232, 24288, 25344, 27456, 29568, 30624, 29568,
 28512, 28512, 28512, 28512, 27456, 26400, 26400, 26400, 26400, 25344, 25344, 23232, 21120,     0,     0,     0,
     0,     0,     0,     0,     0,     0, 12672, 19008, 24288, 27456, 29569, 29569, 30625, 32738, 32737, 31680,
 30624, 29568, 29568, 29568, 29568, 29568, 29568, 29568, 28512, 27456, 25344, 22176, 20064, 15840,     0,     0,
     0,     0,     0,     0,     0, 11616, 19008, 23232, 26400, 29568, 31682, 32738, 32739, 32739, 32738, 32737,
 32737, 31680, 31680, 31680, 30624, 30624, 30624, 29568, 28512, 27456, 25344, 22176, 20064, 16896,     0,     0,
     0,     0,     0,     0,  8448, 19008, 23232, 25344, 28512, 30625, 32739, 32740, 32740, 32740, 32739, 32738,
 32738, 32737, 32736, 32736, 31680, 30624, 29568, 29568, 28512, 27456, 25344, 22176, 20064, 16896, 13728,     0,
     0,     0,     0,     0, 17952, 22176, 24288, 27456, 30625, 32738, 32740, 32741, 32742, 32741, 32740, 32739,
 32738, 32737, 32736, 31680, 30624, 30624, 29568, 28512, 27456, 27456, 25344, 23232, 20064, 16896, 13728,     0,
     0,     0,     0,  2112, 20064, 23232, 26400, 28512, 30625, 32738, 32740, 32741, 32741, 32740, 32739, 32738,
 32737, 31680, 31680, 30624, 30624, 29568, 28512, 28512, 27456, 26400, 25344, 23232, 20064, 14784,  9504,     0,
     0,     0,     0, 13728, 19008, 22176, 25344, 28512, 29568, 31681, 32738, 32740, 32740, 32739, 32738, 32737,
 31680, 30624, 30624, 29568, 29568, 28512, 27456, 27456, 26400, 25344, 25344, 21120, 16896, 11616,     0,     0,
     0,     0,     0, 15840, 19008, 21120, 24288, 27456, 28512, 29568, 31681, 32738, 32739, 32738, 32737, 31680,
 30624, 29568, 29568, 28512, 28512, 27456, 26400, 26400, 24288, 23232, 22176, 17952, 12672,  7392,     0,     0,
     0,     0,     0, 14784, 17952, 21120, 23232, 26400, 27456, 28512, 29568, 31681, 32738, 31681, 30624, 30624,
 29568, 28512, 28512, 27456, 27456, 25344, 24288, 23232, 22176, 20064, 17952, 14784,  8448,  3168,     0,     0,
     0,     0,     0, 14784, 16896, 20064, 23232, 25344, 26400, 27456, 28512, 29568, 31681, 30624, 29568, 29568,
 28512, 27456, 26400, 25344, 24288, 23232, 22176, 20064, 19008, 16896, 14784, 10560,  5280,     0,     0,     0,
     0,     0,     0, 12672, 15840, 19008, 22176, 24288, 25344, 26400, 27456, 28512, 29568, 28512, 27456, 26400,
 25344, 24288, 23232, 22176, 21120, 20064, 19008, 16896, 14784, 13728, 12672,  8448,  3168,     0,     0,     0,
     0,     0,     0,  9504, 12672, 15840, 19008, 22176, 23232, 24288, 25344, 26400, 26400, 25344, 24288, 22176,
 22176, 21120, 20064, 20064, 19008, 16896, 15840, 13728, 12672, 11616,  9504,  5280,     0,     0,     0,     0,
     0,     0,     0,     0,  9504, 12672, 15840, 19008, 20064, 21120, 22176, 22176, 22176, 21120, 20064, 20064,
 19008, 17952, 16896, 15840, 14784, 13728, 12672, 11616, 10560,  8448,  7392,     0,     0,     0,     0,     0,
     0,     0,     0,     0,  6336,  9504, 12672, 15840, 17952, 17952, 19008, 19008, 19008, 17952, 16896, 15840,
 14784, 13728, 13728, 12672, 11616,  9504,  8448,  7392,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,  3168,  6336,  9504, 12672, 14784, 14784, 14784, 14784, 13728, 12672, 12672, 11616,
 10560, 10560,  9504,  8448,  7392,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,  3168,  5280,  7392,  9504, 10560, 10560,  9504,  8448,  7392,  6336,  5280,
  5280,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,  1056,  3168,  5280,  6336,  5280,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
};

void PutNoZoomNoClip(int x,int y,unsigned int bw,unsigned int bh,const unsigned short bmp[])
{
	int X,Y;
	for(X=0; X<bw; ++X)
	{
		for(Y=0; Y<bh; ++Y)
		{
			unsigned short pix=bmp[Y*bw+X];
			unsigned int sx=X+x;
			unsigned int sy=Y+y;
			unsigned char *pixPtr=vram+4*(VRAMW*sy+sx);

			unsigned int b=pix&0x1F;
			unsigned int r=(pix>>5)&0x1F;
			unsigned int g=(pix>>10)&0x1F;
			b=(b<<3)|(b>>2);
			r=(r<<3)|(r>>2);
			g=(g<<3)|(g>>2);

			pixPtr[0]=r;
			pixPtr[1]=g;
			pixPtr[2]=b;
			pixPtr[3]=255;
		}
	}
}

void PutWithZoomAndViewportClipping(
	int x0,int y0,int dx,int dy,unsigned int srcw,unsigned int srch,const unsigned short bmp[],
	int vx0,int vy0,int vx1,int vy1)
{
	int x1=x0+dx-1;
	int y1=y0+dy-1;
	int X,srcX,Y,srcY;

	int balanceX0=dx,balanceY=dy;
	int xStart=x0,yStart=y0,srcXStart=0,srcYStart=0;
	int xEnd=std::min(x1,vx1),yEnd=std::min(y1,vy1);

	if(0==dx || 0==dy) // Not visible.
	{
		return;
	}

	if(x0<vx0)
	{
		unsigned left=vx0-x0;

		xStart=vx0;
		srcXStart=srcw*left/dx;
		balanceX0=-srcw*(vx0-x0)+dx*(srcXStart+1);
	}
	if(y0<vy0)
	{
		unsigned up=vy0-y0;
		yStart=vy0;
		srcYStart=up*srch/dy;
		balanceY=-srch*(vy0-y0)+dy*(srcYStart+1);
	}

	srcY=srcYStart;
	for(Y=yStart; Y<=yEnd; ++Y)
	{
		int balanceX=balanceX0;
		srcX=srcXStart;
		for(X=xStart; X<=xEnd; ++X)
		{
			unsigned short pix=bmp[srcY*srcw+srcX];
			unsigned char *pixPtr=vram+4*(VRAMW*Y+X);

			unsigned int b=pix&0x1F;
			unsigned int r=(pix>>5)&0x1F;
			unsigned int g=(pix>>10)&0x1F;
			b=(b<<3)|(b>>2);
			r=(r<<3)|(r>>2);
			g=(g<<3)|(g>>2);

			pixPtr[0]=r;
			pixPtr[1]=g;
			pixPtr[2]=b;
			pixPtr[3]=255;

			balanceX-=srcw;
			while(balanceX<=0)
			{
				++srcX;
				balanceX+=dx;
			}
		}

		balanceY-=srch;
		while(balanceY<=0)
		{
			++srcY;
			balanceY+=dy;
		}
	}
}

int main(void)
{
	for(auto &v : vram)
	{
		v=0;
	}

	int zoomW=32,zoomH=32;
	int x=0,y=0;

	FsOpenWindow(0,0,VRAMW,VRAMH,1);
	for(;;)
	{
		FsPollDevice();
		auto key=FsInkey();
		if(key==FSKEY_ESC)
		{
			break;
		}

		if(FSKEY_Z==key)
		{
			zoomW+=4;
			zoomH+=4;
		}
		if(FSKEY_M==key)
		{
			zoomW-=4;
			zoomH-=4;
		}

		if(0!=FsGetKeyState(FSKEY_LEFT))
		{
			--x;
		}
		if(0!=FsGetKeyState(FSKEY_RIGHT))
		{
			++x;
		}
		if(0!=FsGetKeyState(FSKEY_UP))
		{
			--y;
		}
		if(0!=FsGetKeyState(FSKEY_DOWN))
		{
			++y;
		}

		//PutNoZoomNoClip(100,100,32,32,duck);

		PutWithZoomAndViewportClipping(x,y,zoomW,zoomH,32,32,duck,0,0,639,479);


		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		glPixelZoom(1,-1);
		glRasterPos2i(0,0);
		glDrawPixels(VRAMW,VRAMH,GL_RGBA,GL_UNSIGNED_BYTE,vram);
		FsSwapBuffers();
	}
}
