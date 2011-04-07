#ifndef _GPU_API_
#define _GPU_API_

#include "../psxcommon.h"
#include "../plugins.h"

typedef unsigned long BOOL;

#ifdef IPHONE
#define PSX4ALL_WIDTH 320
#define PSX4ALL_HEIGHT 240
#define PSX4ALL_PITCH 640
#elif defined(PANDORA)
#define PSX4ALL_WIDTH 320
#define PSX4ALL_HEIGHT 240
#define PSX4ALL_PITCH 640
#elif defined(ANDROID)
#define PSX4ALL_WIDTH 320
#define PSX4ALL_HEIGHT 240
#define PSX4ALL_PITCH 640
#else
#error define a system type and a width height and pitch
#endif
///////////////////////////////////////////////////////////////////////////////
//  GPU global definitions
#define	FRAME_BUFFER_SIZE	(1024*512*2)
#define	FRAME_WIDTH			  1024
#define	FRAME_HEIGHT		  512
#define	FRAME_OFFSET(x,y)	(((y)<<10)+(x))

///////////////////////////////////////////////////////////////////////////////
//  GPU defined interface for save states
#if 0
typedef struct  GPUFreeze
{
	u32 Version;
	u32 GP1;
	u32 Control[256];
	u8  FrameBuffer[0x100000];
	///////////////////////////////////////////////////////////////////////////////
	//  Display status
	s32	isPAL;
	u32 isDisplaySet;
	u32 DisplayArea   [8];
	u32 DirtyArea     [4];
	u32 LastDirtyArea [4];
	u32 CheckArea     [4];
	
	///////////////////////////////////////////////////////////////////////////////
	//  Rasterizer status
	u32 TextureWindow [4];
	u32 DrawingArea   [4];
	u32 DrawingOffset [2];
	u32 MaskU;
	u32 MaskV;
	
	///////////////////////////////////////////////////////////////////////////////
	//  Rasterizer status
	u8  Masking;
	u16 PixelMSB;
} GPUFreeze;
#endif

struct  GPUTumbnail
{
  u8  Data[128*96*3];
};

typedef struct GPUPacket
{
  union
  {
    u32 U4[16];
    s32 S4[16];
    u16 U2[32];
    s16 S2[32];
    u8  U1[64];
	  s8  S1[64];
  };
} GPUPacket;

#ifdef __cplusplus
extern "C" {
#endif
///////////////////////////////////////////////////////////////////////////////
//  GPU common data for fast access.
extern int  GPU_force_res_change;
extern BOOL GPU_IsInit;
extern u32  GPU_GP1;
#ifdef PSP
extern u16*  GPU_FrameBuffer;
#else
extern u16  GPU_FrameBuffer[FRAME_BUFFER_SIZE/2];    // FRAME_BUFFER_SIZE is defined in bytes
#endif
#if 1
extern GPUPacket PacketBuffer;
extern GPUPacket PacketBufferBackup;
#endif
extern int  vBlank;

#if 0
///////////////////////////////////////////////////////////////////////////////
//  GPU Pluggin Enumeration/selection
u32         GPU_GetNumPluggin();
const char* GPU_GetPluginName(u32 idx);
#ifdef __cplusplus
extern "C" {
#endif
BOOL        GPU_SelectPluggin(u32 idx);
#ifdef __cplusplus
}
#endif


///////////////////////////////////////////////////////////////////////////////
//  GPU inicialization/deinicialization functions
extern  BOOL  (*GPU_init)         (void);
extern  void  (*GPU_done)         (void);
extern  BOOL  (*GPU_freeze)       (BOOL bWrite , GPUFreeze *pFreeze);

///////////////////////////////////////////////////////////////////////////////
//  GPU Vsinc Notification
extern  void  (*GPU_vSinc)        (void);

///////////////////////////////////////////////////////////////////////////////
//  GPU DMA comunication
extern  void  (*GPU_dmaChain)     (u32* baseAddr,   u32 dmaVAddr);
extern  void  (*GPU_writeDataMem) (u32* dmaAddress, u32 dmaCount);
extern  void  (*GPU_readDataMem)  (u32* dmaAddress, u32 dmaCount);

///////////////////////////////////////////////////////////////////////////////
//  GPU Memory comunication
extern  void  (*GPU_writeData)    (u32  data);
extern  void  (*GPU_writeStatus)  (u32  data);
extern  u32   (*GPU_readData)     (void);
#endif
#if 0
inline  u32     GPU_readStatus    (void)  { return GP1; }
#endif

extern void (*GPU_updateLace)(void);
void GPUupdateLace(void);

#ifdef __cplusplus
}
#endif
///////////////////////////////////////////////////////////////////////////////
#endif //_GPU_API_
