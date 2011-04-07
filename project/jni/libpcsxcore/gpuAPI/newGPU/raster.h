#ifndef RASTER_H
#define RASTER_H
///////////////////////////////////////////////////////////////////////////////
//  GPU Render Primitives


//  Images
void gpuLoadImage   (void);
void gpuStoreImage  (void);
void gpuMoveImage   (void);
void gpuClearImage  (void);

//  Lines
void gpuDrawLF(void);
void gpuDrawLG(void);

//  Sprite
void gpuDrawS(void);
void gpuDrawT(void);


//  Poligon
void  gpuDrawF3 ();
void  gpuDrawFT3();
void  gpuDrawG3 ();
void  gpuDrawGT3();

///////////////////////////////////////////////////////////////////////////////
//  GPU Raster Macros
#define	GPU_RGB16(rgb)        ((((rgb)&0xF80000)>>9)|(((rgb)&0xF800)>>6)|(((rgb)&0xF8)>>3))

#define GPU_EXPANDSIGN_POLY(x)  (((s32)(x)<<20)>>20)
#define GPU_EXPANDSIGN_SPRT(x)  (((s32)(x)<<21)>>21)

#define	GPU_TESTRANGE(x)      { if((u32)(x+1024) > 2047) return; }

#define	GPU_SWAP(a,b,t)	{(t)=(a);(a)=(b);(b)=(t);}

///////////////////////////////////////////////////////////////////////////////
#endif  //RASTER_H
