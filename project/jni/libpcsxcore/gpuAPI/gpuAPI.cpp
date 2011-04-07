#include "gpuAPI.h"


///////////////////////////////////////////////////////////////////////////////
//  GPU common data for fast access.
int  GPU_force_res_change = 0;
BOOL  GPU_IsInit = false;
u32   GPU_GP1;
#ifdef PSP
u16*   GPU_FrameBuffer = (u16*)(0x04000000 + 0x100000);
#else
u16   GPU_FrameBuffer[FRAME_BUFFER_SIZE/2];    // FRAME_BUFFER_SIZE is defined in bytes
#endif
#if 1
GPUPacket PacketBuffer;
GPUPacket PacketBufferBackup;
#endif

void  (*GPU_updateLace)        (void);

#if 0
///////////////////////////////////////////////////////////////////////////////
//  GPU inicialization/deinicialization functions
BOOL  (*GPU_init)         (void);
void  (*GPU_done)         (void);
BOOL  (*GPU_freeze)       (BOOL bWrite , GPUFreeze *pFreeze);

///////////////////////////////////////////////////////////////////////////////
//  GPU Vsinc Notification
void  (*GPU_vSinc)        (void);

///////////////////////////////////////////////////////////////////////////////
//  GPU DMA comunication
void  (*GPU_dmaChain)     (u32* baseAddr,   u32 dmaVAddr);
void  (*GPU_writeDataMem) (u32* dmaAddress, u32 dmaCount);
void  (*GPU_readDataMem)  (u32* dmaAddress, u32 dmaCount);

///////////////////////////////////////////////////////////////////////////////
//  GPU Memory comunication
void  (*GPU_writeData)    (u32  data);
void  (*GPU_writeStatus)  (u32  data);
u32   (*GPU_readData)     (void);



///////////////////////////////////////////////////////////////////////////////
//  GPU Pluggin Enumeration/selection
enum
{
  E_GPU_NEWGPU = 0,
  E_GPU_GL,
  E_GPU_PSP,
  E_GPU_DRHELL,
  E_GPU_DUMMY,
  E_GPU_COUNT
};

static  char  s_gpuNames[E_GPU_COUNT][256] =
{
  "newGpu",
  "glGpu",
  "pspGpu",
  "drhellGpu",
  "dummyGpu",
};

BOOL  register_NEWGPU   ()
#if (defined(PSP) && defined(PSP_GPU))// || ANDROID
{ return true;  }
#else
;
#endif
BOOL  register_GLGPU    ()    { return true;  }
BOOL  register_DUMMYGPU ()    { return true;  }
BOOL  register_PSPGPU   ()
#if defined(PSP) && defined(PSP_GPU)
;
#else
{ return true;  }
#endif
BOOL  register_DRHELLGPU   ()
#if 1 // !defined(ANDROID)
{ return true;  }
#else
;
#endif
BOOL  register_NOGPU    ();

typedef BOOL  (*PFI)();
static  PFI s_gpuRegisters[E_GPU_COUNT] =
{
  register_NEWGPU,
  register_GLGPU,
  register_PSPGPU,
  register_DRHELLGPU,
  register_DUMMYGPU
};


///////////////////////////////////////////////////////////////////////////////
u32         GPU_GetNumPluggin()
{
  return E_GPU_COUNT;
}


///////////////////////////////////////////////////////////////////////////////
const char* GPU_GetPluginName(u32 idx)
{
  if(idx<E_GPU_COUNT)
    return s_gpuNames[idx];
  return "unknownGPU";
}


///////////////////////////////////////////////////////////////////////////////
extern "C" BOOL        GPU_SelectPluggin(u32 idx)
{
  BOOL bInit = GPU_IsInit;
  BOOL bOk   = true;
  //GPUFreeze  local_freeze;
  if(bInit)
  {
    //bOk = GPU_freeze(1, &local_freeze);
    //GPU_done();
  }

  if(bOk && idx<E_GPU_COUNT)
    bOk = s_gpuRegisters[idx]();
  else
    bOk = false;

  if(!bOk)
    register_NOGPU();

  if(bInit && bOk)
  {
    //bOk = GPU_init();
    //bOk = bOk && GPU_freeze(0, &local_freeze);
  }

  return bOk;
}


///////////////////////////////////////////////////////////////////////////////
BOOL  register_NOGPU()
{
  //  GPU inicialization/deinicialization functions
  GPU_init    = 0;
  GPU_done    = 0;
  GPU_freeze  = 0;

  //  GPU Vsinc Notification
  GPU_vSinc = 0;

  //  GPU DMA comunication
  GPU_dmaChain      = 0;
  GPU_writeDataMem  = 0;
  GPU_readDataMem   = 0;

  //  GPU Memory comunication
  GPU_writeData   = 0;
  GPU_writeStatus = 0;
  GPU_readData    = 0;

  return true;
}
#endif
