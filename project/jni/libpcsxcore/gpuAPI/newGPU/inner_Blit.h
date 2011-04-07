#ifndef _INNER_BLIT_H_
#define _INNER_BLIT_H_
///////////////////////////////////////////////////////////////////////////////
//  GPU Blitting code

#define write2x(n) dst16[PSX4ALL_WIDTH+n] = dst16[n]

#define write(n) dst16[n]

#define bgr15torgb16(n) bgr2rgb[src16[n]]

#define rgb24torgb16(n, r, g, b) gp2x_video_RGB_color16( src8[r], src8[g], src8[b] )

#define LOOP_START_16_BASE(count)				\
		u16* src16 = (u16*)src + (i<<10);		\
		uCount = count;					\

#define LOOP_START_16(count)					\
	for(int i = 0; i < h1; i++)				\
	{							\
		LOOP_START_16_BASE(count)			\


#define LOOP_START_16_240(count)				\
	for(int i = 0; i < h1; i++)				\
	{							\
		LOOP_START_16_BASE(count)			\

#define LOOP_END_16						\
	}							\

#define LOOP_START_8_BASE(count)				\
		u8* src8 = (u8*)src + (i<<11);			\
		uCount = count;					\

#define LOOP_START_8(count)					\
	for(int i = 0; i < h1; i++)				\
	{							\
		LOOP_START_8_BASE(count)			\
		
#define LOOP_START_8_240(count)					\
	for(int i = 0; i < h1; i++)				\
	{							\
		LOOP_START_8_BASE(count)			\

#define LOOP_END_8						\
	}							\

void GPU_Blit_512_480_ROT90CW_2X(void* src, u16* dst16, u32 h1)
{ 
  u32 uCount;
  if((GPU_GP1 & 0x00200000) == 0)
  {
		LOOP_START_16(32)
    do{
    	write(0) = bgr15torgb16(0);
    	write(1) = bgr15torgb16(2);
    	write(2) = bgr15torgb16(4);
 			write(3) = bgr15torgb16(6);
 			write(4) = bgr15torgb16(7);
 			
    	write(5) = bgr15torgb16(8);
    	write(6) = bgr15torgb16(10);
    	write(7) = bgr15torgb16(12);
 			write(8) = bgr15torgb16(14);
 			write(9) = bgr15torgb16(15);
 			
      dst16 += 10;
      src16 += 16;
    }while(--uCount);
    i++;
	//dst16 += 160;
    LOOP_END_16
  }
  else
  {
    LOOP_START_8(32)
    do{
    	write(0) = rgb24torgb16(0, 0,  1,  2 );
    	write(1) = rgb24torgb16(1, 6,  7,  8 );
    	write(2) = rgb24torgb16(2, 12, 13,  14 );
 			write(3) = rgb24torgb16(3, 18,  19,  20 );
 			write(4) = rgb24torgb16(4, 21, 22, 23 );
 			
 			write(5) = rgb24torgb16(5, 24,  25,  26 );
    	write(6) = rgb24torgb16(6, 30,  31,  32 );
    	write(7) = rgb24torgb16(7, 36,  37,  38 );
 			write(8) = rgb24torgb16(8, 42,  43,  44 );
 			write(9) = rgb24torgb16(9, 45, 46, 47 );
 			
      dst16 += 10;
      src8 += 48;
    }while(--uCount);
    i++;
	//dst16 += 160;
    LOOP_END_8
  }
}

void GPU_Blit_384_480_ROT90CW_2X(void* src, u16* dst16, u32 h1)
{ 
  u32 uCount;
  if((GPU_GP1 & 0x00200000) == 0)
  {
    LOOP_START_16(32)
    do{
    	write(0) = bgr15torgb16(0);
    	write(1) = bgr15torgb16(1);
    	write(2) = bgr15torgb16(2);
    	write(3) = bgr15torgb16(3);
    	write(4) = bgr15torgb16(5);
    	write(5) = bgr15torgb16(6);
    	write(6) = bgr15torgb16(7);
    	write(7) = bgr15torgb16(9);
    	write(8) = bgr15torgb16(10);
    	write(9) = bgr15torgb16(11);
    	
      dst16 += 10;
      src16 += 12;
    }while(--uCount);
	//dst16 += 160;
	  i++;
    LOOP_END_16
  }
  else
  {
    LOOP_START_8(32)
    do{
    	write(0) = rgb24torgb16(0, 0,  1,  2 );
 			write(1) = rgb24torgb16(1, 3,  4,  5 );
 			write(2) = rgb24torgb16(2, 6,  7,  8 );
 			write(3) = rgb24torgb16(3, 9,  10,  11 );
 			write(4) = rgb24torgb16(4, 15,  16,  17 );
 			write(5) = rgb24torgb16(5, 18,  19,  20 );
 			write(6) = rgb24torgb16(6, 21,  22,  23 );
 			write(7) = rgb24torgb16(7, 27,  28,  29 );
 			write(8) = rgb24torgb16(8, 30,  31,  32 );
 			write(9) = rgb24torgb16(9, 33,  34,  35 );
 			
      dst16 += 10;
      src8 += 36;
    }while(--uCount);
	//dst16 += 160;
	  i++;
    LOOP_END_8
  }
}

void GPU_Blit_368_480_ROT90CW_2X(void* src, u16* dst16, u32 h1)
{ 
  u32 uCount;
  if((GPU_GP1 & 0x00200000) == 0)
  {
    LOOP_START_16(16)
    do{
    	write(0) = bgr15torgb16(0);
    	write(1) = bgr15torgb16(1);
    	write(2) = bgr15torgb16(2);
    	write(3) = bgr15torgb16(3);
    	write(4) = bgr15torgb16(5);
    	write(5) = bgr15torgb16(6);
    	write(6) = bgr15torgb16(7);
    	write(7) = bgr15torgb16(9);
    	write(8) = bgr15torgb16(10);
    	write(9) = bgr15torgb16(11);
    	write(10) = bgr15torgb16(12);
    	write(11) = bgr15torgb16(14);
    	write(12) = bgr15torgb16(15);
    	write(13) = bgr15torgb16(16);
    	write(14) = bgr15torgb16(17);
    	write(15) = bgr15torgb16(18);
    	write(16) = bgr15torgb16(19);
    	write(17) = bgr15torgb16(20);
    	write(18) = bgr15torgb16(21);
    	write(19) = bgr15torgb16(22);

      dst16 += 20;
      src16 += 23;
    }while(--uCount);
	//dst16 += 160;
	  i++;
    LOOP_END_16
  }
  else
  {
    LOOP_START_8(16)
    do{
    	write(0) = rgb24torgb16(0, 0,  1,  2 );
 			write(1) = rgb24torgb16(1, 3,  4,  5 );
 			write(2) = rgb24torgb16(2, 6,  7,  8 );
 			write(3) = rgb24torgb16(3, 9,  10,  11 );
 			write(4) = rgb24torgb16(4, 15,  16,  17 );
 			write(5) = rgb24torgb16(5, 18,  19,  20 );
 			write(6) = rgb24torgb16(6, 21,  22,  23 );
 			write(7) = rgb24torgb16(7, 27,  28,  29 );
 			write(8) = rgb24torgb16(8, 30,  31,  32 );
 			write(9) = rgb24torgb16(9, 33,  34,  35 );

    	write(10) = rgb24torgb16(10, 36,  37,  38 );
 			write(11) = rgb24torgb16(11, 39,  40,  41 );
 			write(12) = rgb24torgb16(12, 42,  43,  44 );
 			write(13) = rgb24torgb16(13, 45,  46,  47 );
 			write(14) = rgb24torgb16(14, 51,  52,  53 );
 			write(15) = rgb24torgb16(15, 54,  55,  56 );
 			write(16) = rgb24torgb16(16, 57,  58,  59 );
 			write(17) = rgb24torgb16(17, 60,  61,  62 );
 			write(18) = rgb24torgb16(18, 63,  64,  65 );
 			write(19) = rgb24torgb16(19, 66,  67,  68 );

      dst16 += 20;
      src8 += 69;
    }while(--uCount);
	//dst16 += 160;
	  i++;
    LOOP_END_8
  }
}

void GPU_Blit_256_480_ROT90CW_2X(void* src, u16* dst16, u32 h1)
{ 
  u32 uCount;
  if((GPU_GP1 & 0x00200000) == 0)
  {
    LOOP_START_16(32)
    do{
    	write(0) = bgr15torgb16(0);
    	write(2) = write(1) = bgr15torgb16(1);
    	write(3) = bgr15torgb16(2);
    	write(4) = bgr15torgb16(3);
    	write(5) = bgr15torgb16(4);
    	write(6) = bgr15torgb16(5);
    	write(8) = write(7) = bgr15torgb16(6);
    	write(9) = bgr15torgb16(7);

      dst16 += 10;
      src16 += 8;
    }while(--uCount);
	//dst16 += 160;
	  i++;
    LOOP_END_16
  }
  else
  {
    LOOP_START_8(32)
    do{
    	write(0) = rgb24torgb16(0, 0,  1,  2 );
 			write(2) = write(1) = rgb24torgb16(1, 3,  4,  5 );
 			write(3) = rgb24torgb16(2, 6,  7,  8 );
 			write(4) = rgb24torgb16(3, 9,  10,  11 );
 			write(5) = rgb24torgb16(4, 12,  13,  14 );
 			write(6) = rgb24torgb16(5, 15,  16,  17 );
 			write(8) = write(7) = rgb24torgb16(6, 18,  19,  20 );
 			write(9) = rgb24torgb16(7, 21,  22,  23 );
 			
      dst16 += 10;
      src8 += 24;
    }while(--uCount);
	//dst16 += 160;
	  i++;
    LOOP_END_8
  }
}

void GPU_Blit_320_480_ROT90CW_2X(void* src, u16* dst16, u32 h1)
{
  u32 uCount;
  if((GPU_GP1 & 0x00200000) == 0)
  {
    LOOP_START_16(32)
    do{
      write(0) = bgr15torgb16(0);
      write(1) = bgr15torgb16(1);
      write(2) = bgr15torgb16(2);
      write(3) = bgr15torgb16(3);
      write(4) = bgr15torgb16(4);
      write(5) = bgr15torgb16(5);
      write(6) = bgr15torgb16(6);
      write(7) = bgr15torgb16(7);
      write(8) = bgr15torgb16(8);
      write(9) = bgr15torgb16(9);
      
      dst16 += 10;
      src16 += 10;
    }while(--uCount);
	//dst16 += 160;
	  i++;
    LOOP_END_16
  }
  else
  {
    LOOP_START_8(32)
    do{

    	write(0) = rgb24torgb16(0, 0,  1,  2 );
 			write(1) = rgb24torgb16(1, 3,  4,  5 );
 			write(2) = rgb24torgb16(2, 6,  7,  8 );
 			write(3) = rgb24torgb16(3, 9,  10,  11 );
 			write(4) = rgb24torgb16(4, 12,  13,  14 );
 			write(5) = rgb24torgb16(5, 15,  16,  17 );
 			write(6) = rgb24torgb16(6, 18,  19,  20 );
 			write(7) = rgb24torgb16(7, 21,  22,  23 );
 			write(8) = rgb24torgb16(8, 24,  25,  26 );
 			write(9) = rgb24torgb16(9, 27,  28,  29 );

      dst16 += 10;
      src8 += 30;
    }while(--uCount);
	//dst16 += 160;
	  i++;
    LOOP_END_8
  }
}

void GPU_Blit_640_480_ROT90CW_2X(void* src, u16* dst16, u32 h1)
{
  u32 uCount;
  if((GPU_GP1 & 0x00200000) == 0)
  {
	  LOOP_START_16(40)
	  do
    {
      
		  write(0) = bgr15torgb16(0);
		  write(1) = bgr15torgb16(2);
		  write(2) = bgr15torgb16(4);
		  write(3) = bgr15torgb16(6);
		  write(4) = bgr15torgb16(8);
		  write(5) = bgr15torgb16(10);
		  write(6) = bgr15torgb16(12);
		  write(7) = bgr15torgb16(14);
			
      dst16 += 8;
      src16 += 16;
    }while(--uCount);
	//dst16 += 160;
	  i++;
    LOOP_END_16
  }
  else
  {
  	LOOP_START_8(40)
    do
    {
    	write(0) = rgb24torgb16(0, 0,  1,  2 );
    	write(1) = rgb24torgb16(1, 6,  7,  8 );
    	write(2) = rgb24torgb16(2, 12,  13,  14 );
    	write(3) = rgb24torgb16(3, 18, 19, 20 );
    	write(4) = rgb24torgb16(4, 24,  25,  26 );
    	write(5) = rgb24torgb16(5, 30,  31,  32 );
    	write(6) = rgb24torgb16(6, 36,  37,  38 );
    	write(7) = rgb24torgb16(7, 42, 43, 44 );

  		dst16 += 8;
  		src8 += 48;
    }while(--uCount);
	//dst16 += 160;
    i++;
    LOOP_END_8
  }
}

void GPU_Blit_640_240_ROT90CW_2X(void* src, u16* dst16, u32 h1)
{
  u32 uCount;
  if((GPU_GP1 & 0x00200000) == 0)
  {
	  LOOP_START_16(40)
	  do
    {
      
		  write(0) = bgr15torgb16(0);
		  write(1) = bgr15torgb16(2);
		  write(2) = bgr15torgb16(4);
		  write(3) = bgr15torgb16(6);
		  write(4) = bgr15torgb16(8);
		  write(5) = bgr15torgb16(10);
		  write(6) = bgr15torgb16(12);
		  write(7) = bgr15torgb16(14);
			
      dst16 += 8;
      src16 += 16;
    }while(--uCount);
	//dst16 += 160;
	//i++;
    LOOP_END_16
  }
  else
  {
  	LOOP_START_8(40)
    do
    {
    	write(0) = rgb24torgb16(0, 0,  1,  2 );
    	write(1) = rgb24torgb16(1, 6,  7,  8 );
    	write(2) = rgb24torgb16(2, 12,  13,  14 );
    	write(3) = rgb24torgb16(3, 18, 19, 20 );
    	write(4) = rgb24torgb16(4, 24,  25,  26 );
    	write(5) = rgb24torgb16(5, 30,  31,  32 );
    	write(6) = rgb24torgb16(6, 36,  37,  38 );
    	write(7) = rgb24torgb16(7, 42, 43, 44 );

  		dst16 += 8;
  		src8 += 48;
    }while(--uCount);
	//dst16 += 160;
  //  i++;
    LOOP_END_8
  }
}

void GPU_Blit_512_240_ROT90CW_2X(void* src, u16* dst16, u32 h1)
{ 
  u32 uCount;
  if((GPU_GP1 & 0x00200000) == 0)
  {
		LOOP_START_16(32)
    do{
    	write(0) = bgr15torgb16(0);
    	write(1) = bgr15torgb16(2);
    	write(2) = bgr15torgb16(4);
 			write(3) = bgr15torgb16(6);
 			write(4) = bgr15torgb16(7);
 			
    	write(5) = bgr15torgb16(8);
    	write(6) = bgr15torgb16(10);
    	write(7) = bgr15torgb16(12);
 			write(8) = bgr15torgb16(14);
 			write(9) = bgr15torgb16(15);
 			
      dst16 += 10;
      src16 += 16;
    }while(--uCount);
	//dst16 += 160;
    LOOP_END_16
  }
  else
  {
    LOOP_START_8(32)
    do{
    	write(0) = rgb24torgb16(0, 0,  1,  2 );
    	write(1) = rgb24torgb16(1, 6,  7,  8 );
    	write(2) = rgb24torgb16(2, 12, 13,  14 );
 			write(3) = rgb24torgb16(3, 18,  19,  20 );
 			write(4) = rgb24torgb16(4, 21, 22, 23 );
 			
 			write(5) = rgb24torgb16(5, 24,  25,  26 );
    	write(6) = rgb24torgb16(6, 30,  31,  32 );
    	write(7) = rgb24torgb16(7, 36,  37,  38 );
 			write(8) = rgb24torgb16(8, 42,  43,  44 );
 			write(9) = rgb24torgb16(9, 45, 46, 47 );
 			
      dst16 += 10;
      src8 += 48;
    }while(--uCount);
	//dst16 += 160;
    LOOP_END_8
  }
}

void GPU_Blit_384_240_ROT90CW_2X(void* src, u16* dst16, u32 h1)
{ 
  u32 uCount;
  if((GPU_GP1 & 0x00200000) == 0)
  {
    LOOP_START_16(32)
    do{
    	write(0) = bgr15torgb16(0);
    	write(1) = bgr15torgb16(1);
    	write(2) = bgr15torgb16(2);
    	write(3) = bgr15torgb16(3);
    	write(4) = bgr15torgb16(5);
    	write(5) = bgr15torgb16(6);
    	write(6) = bgr15torgb16(7);
    	write(7) = bgr15torgb16(9);
    	write(8) = bgr15torgb16(10);
    	write(9) = bgr15torgb16(11);
    	
      dst16 += 10;
      src16 += 12;
    }while(--uCount);
	//dst16 += 160;
    LOOP_END_16
  }
  else
  {
    LOOP_START_8(32)
    do{
    	write(0) = rgb24torgb16(0, 0,  1,  2 );
 			write(1) = rgb24torgb16(1, 3,  4,  5 );
 			write(2) = rgb24torgb16(2, 6,  7,  8 );
 			write(3) = rgb24torgb16(3, 9,  10,  11 );
 			write(4) = rgb24torgb16(4, 15,  16,  17 );
 			write(5) = rgb24torgb16(5, 18,  19,  20 );
 			write(6) = rgb24torgb16(6, 21,  22,  23 );
 			write(7) = rgb24torgb16(7, 27,  28,  29 );
 			write(8) = rgb24torgb16(8, 30,  31,  32 );
 			write(9) = rgb24torgb16(9, 33,  34,  35 );
 			
      dst16 += 10;
      src8 += 36;
    }while(--uCount);
	//dst16 += 160;
    LOOP_END_8
  }
}

void GPU_Blit_368_240_ROT90CW_2X(void* src, u16* dst16, u32 h1)
{ 
  u32 uCount;
  if((GPU_GP1 & 0x00200000) == 0)
  {
    LOOP_START_16(16)
    do{
    	write(0) = bgr15torgb16(0);
    	write(1) = bgr15torgb16(1);
    	write(2) = bgr15torgb16(2);
    	write(3) = bgr15torgb16(3);
    	write(4) = bgr15torgb16(5);
    	write(5) = bgr15torgb16(6);
    	write(6) = bgr15torgb16(7);
    	write(7) = bgr15torgb16(9);
    	write(8) = bgr15torgb16(10);
    	write(9) = bgr15torgb16(11);
    	write(10) = bgr15torgb16(12);
    	write(11) = bgr15torgb16(14);
    	write(12) = bgr15torgb16(15);
    	write(13) = bgr15torgb16(16);
    	write(14) = bgr15torgb16(17);
    	write(15) = bgr15torgb16(18);
    	write(16) = bgr15torgb16(19);
    	write(17) = bgr15torgb16(20);
    	write(18) = bgr15torgb16(21);
    	write(19) = bgr15torgb16(22);

      dst16 += 20;
      src16 += 23;
    }while(--uCount);
	//dst16 += 160;
    LOOP_END_16
  }
  else
  {
    LOOP_START_8(16)
    do{
    	write(0) = rgb24torgb16(0, 0,  1,  2 );
 			write(1) = rgb24torgb16(1, 3,  4,  5 );
 			write(2) = rgb24torgb16(2, 6,  7,  8 );
 			write(3) = rgb24torgb16(3, 9,  10,  11 );
 			write(4) = rgb24torgb16(4, 15,  16,  17 );
 			write(5) = rgb24torgb16(5, 18,  19,  20 );
 			write(6) = rgb24torgb16(6, 21,  22,  23 );
 			write(7) = rgb24torgb16(7, 27,  28,  29 );
 			write(8) = rgb24torgb16(8, 30,  31,  32 );
 			write(9) = rgb24torgb16(9, 33,  34,  35 );

    	write(10) = rgb24torgb16(10, 36,  37,  38 );
 			write(11) = rgb24torgb16(11, 39,  40,  41 );
 			write(12) = rgb24torgb16(12, 42,  43,  44 );
 			write(13) = rgb24torgb16(13, 45,  46,  47 );
 			write(14) = rgb24torgb16(14, 51,  52,  53 );
 			write(15) = rgb24torgb16(15, 54,  55,  56 );
 			write(16) = rgb24torgb16(16, 57,  58,  59 );
 			write(17) = rgb24torgb16(17, 60,  61,  62 );
 			write(18) = rgb24torgb16(18, 63,  64,  65 );
 			write(19) = rgb24torgb16(19, 66,  67,  68 );

      dst16 += 20;
      src8 += 69;
    }while(--uCount);
	//dst16 += 160;
    LOOP_END_8
  }
}

void GPU_Blit_256_240_ROT90CW_2X(void* src, u16* dst16, u32 h1)
{ 
  u32 uCount;
  if((GPU_GP1 & 0x00200000) == 0)
  {
    LOOP_START_16(32)
    do{
    	write(0) = bgr15torgb16(0);
    	write(2) = write(1) = bgr15torgb16(1);
    	write(3) = bgr15torgb16(2);
    	write(4) = bgr15torgb16(3);
    	write(5) = bgr15torgb16(4);
    	write(6) = bgr15torgb16(5);
    	write(8) = write(7) = bgr15torgb16(6);
    	write(9) = bgr15torgb16(7);

      dst16 += 10;
      src16 += 8;
    }while(--uCount);
	//dst16 += 160;
    LOOP_END_16
  }
  else
  {
    LOOP_START_8(32)
    do{
    	write(0) = rgb24torgb16(0, 0,  1,  2 );
 			write(2) = write(1) = rgb24torgb16(1, 3,  4,  5 );
 			write(3) = rgb24torgb16(2, 6,  7,  8 );
 			write(4) = rgb24torgb16(3, 9,  10,  11 );
 			write(5) = rgb24torgb16(4, 12,  13,  14 );
 			write(6) = rgb24torgb16(5, 15,  16,  17 );
 			write(8) = write(7) = rgb24torgb16(6, 18,  19,  20 );
 			write(9) = rgb24torgb16(7, 21,  22,  23 );
 			
      dst16 += 10;
      src8 += 24;
    }while(--uCount);
	//dst16 += 160;
    LOOP_END_8
  }
}

void GPU_Blit_320_240_ROT90CW_2X(void* src, u16* dst16, u32 h1)
{
  u32 uCount;
  if((GPU_GP1 & 0x00200000) == 0)
  {
    LOOP_START_16(32)
    do{
      write(0) = bgr15torgb16(0);
      write(1) = bgr15torgb16(1);
      write(2) = bgr15torgb16(2);
      write(3) = bgr15torgb16(3);
      write(4) = bgr15torgb16(4);
      write(5) = bgr15torgb16(5);
      write(6) = bgr15torgb16(6);
      write(7) = bgr15torgb16(7);
      write(8) = bgr15torgb16(8);
      write(9) = bgr15torgb16(9);
      
      dst16 += 10;
      src16 += 10;
    }while(--uCount);
	//dst16 += 160;
    LOOP_END_16
  }
  else
  {
    LOOP_START_8(32)
    do{

    	write(0) = rgb24torgb16(0, 0,  1,  2 );
 			write(1) = rgb24torgb16(1, 3,  4,  5 );
 			write(2) = rgb24torgb16(2, 6,  7,  8 );
 			write(3) = rgb24torgb16(3, 9,  10,  11 );
 			write(4) = rgb24torgb16(4, 12,  13,  14 );
 			write(5) = rgb24torgb16(5, 15,  16,  17 );
 			write(6) = rgb24torgb16(6, 18,  19,  20 );
 			write(7) = rgb24torgb16(7, 21,  22,  23 );
 			write(8) = rgb24torgb16(8, 24,  25,  26 );
 			write(9) = rgb24torgb16(9, 27,  28,  29 );

      dst16 += 10;
      src8 += 30;
    }while(--uCount);
	//dst16 += 160;
    LOOP_END_8
  }
}

#endif //_INNER_BLIT_H_
