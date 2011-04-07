/***************************************************************************
 *   Copyright (C) 2007 Ryan Schultz, PCSX-df Team, PCSX team              *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02111-1307 USA.           *
 ***************************************************************************/

#ifndef __PLUGINS_H__
#define __PLUGINS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "psxcommon.h"

//#define ENABLE_SIO1API 1

#ifndef _WIN32

typedef void* HWND;
#define CALLBACK

long GPU_open(unsigned long *, char *, char *);
long SPU_open(void);
long PAD_open(unsigned long *);
long NET_open(unsigned long *);
long SIO1_open(unsigned long *);

#else

#include <windows.h>

typedef long (CALLBACK* GPUopen)(HWND);
typedef long (CALLBACK* SPUopen)(HWND);
typedef long (CALLBACK* PADopen)(HWND);
typedef long (CALLBACK* NETopen)(HWND);
typedef long (CALLBACK* SIO1open)(HWND);

#endif

#include "spu.h"

#include "psemu_plugin_defs.h"
#include "decode_xa.h"

int LoadPlugins();
void ReleasePlugins();
int OpenPlugins();
void ClosePlugins();
int ReloadCdromPlugin();

unsigned long PSEgetLibType(void);
unsigned long PSEgetLibVersion(void);
char *PSEgetLibName(void);

// GPU Functions
long GPU_init(void);
long GPU_shutdown(void);
long GPU_close(void);
void GPU_writeStatus(uint32_t);
void GPU_writeData(uint32_t);
void GPU_writeDataMem(uint32_t *, int);
uint32_t GPU_readStatus(void);
uint32_t GPU_readData(void);
void GPU_readDataMem(uint32_t *, int);
long GPU_dmaChain(uint32_t *,uint32_t);
long GPU_configure(void);
long GPU_test(void);
void GPU_about(void);
void GPU_makeSnapshot(void);
void GPU_keypressed(int);
void GPU_displayText(char *);
#if 0
typedef struct {
	uint32_t ulFreezeVersion;
	uint32_t ulStatus;
	uint32_t ulControl[256];
	unsigned char psxVRam[1024*512*2];
} GPUFreeze_t;
long GPU_freeze(uint32_t, GPUFreeze_t *);
#else
typedef struct  GPUFreeze_t
{
	u32 ulFreezeVersion;
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
  int vBlank;
} GPUFreeze_t;
long GPU_freeze(uint32_t, GPUFreeze_t *);
#endif
long GPU_getScreenPic(unsigned char *);
long GPU_showScreenPic(unsigned char *);
void GPU_clearDynarec(void (CALLBACK *callback)(void));
void GPU_vBlank(int);
void GPU_registerCallback(void (CALLBACK *callback)(int));
void GPU_idle(void);

// CD-ROM Functions
long CDR_init(void);
long CDR_shutdown(void);
long CDR_open(void);
long CDR_close(void);
long CDR_getTN(unsigned char *);
long CDR_getTD(unsigned char, unsigned char *);
long CDR_readTrack(unsigned char *);
unsigned char* CDR_getBuffer(void);
unsigned char* CDR_getBufferSub(void);
long CDR_configure(void);
long CDR_test(void);
void CDR_about(void);
long CDR_play(unsigned char *);
long CDR_stop(void);
long CDR_setfilename(char *);
struct CdrStat {
	uint32_t Type;
	uint32_t Status;
	unsigned char Time[3];
};
long CDR_getStatus(struct CdrStat *);
char* CDR_getDriveLetter(void);
struct SubQ {
	char res0[12];
	unsigned char ControlAndADR;
	unsigned char TrackNumber;
	unsigned char IndexNumber;
	unsigned char TrackRelativeAddress[3];
	unsigned char Filler;
	unsigned char AbsoluteAddress[3];
	unsigned char CRC[2];
	char res1[72];
};
long CDR_readCDDA(unsigned char, unsigned char, unsigned char, unsigned char *);
long CDR_getTE(unsigned char, unsigned char *, unsigned char *, unsigned char *);

// SPU Functions
long SPU_init(void);				
long SPU_shutdown(void);	
long SPU_close(void);			
void SPU_playSample(unsigned char);		
void SPU_writeRegister(unsigned long, unsigned short);
unsigned short SPU_readRegister(unsigned long);
void SPU_writeDMA(unsigned short);
unsigned short SPU_readDMA(void);
void SPU_writeDMAMem(unsigned short *, int);
void SPU_readDMAMem(unsigned short *, int);
void SPU_playADPCMchannel(xa_decode_t *);
void SPU_registerCallback(void (CALLBACK *callback)(void));
long SPU_configure(void);
long SPU_test(void);
void SPU_about(void);
/*
typedef struct {
	unsigned char PluginName[8];
	uint32_t PluginVersion;
	uint32_t Size;
	unsigned char SPUPorts[0x200];
	unsigned char SPURam[0x80000];
	xa_decode_t xa;
	unsigned char *SPUInfo;
} SPUFreeze_t;
*/
/*typedef struct
{
 char          szSPUName[8];
 unsigned long ulFreezeVersion;
 unsigned long ulFreezeSize;
 unsigned char cSPUPort[0x200];
 unsigned char cSPURam[0x80000];
 xa_decode_t   xaS;     
 u32           Addr;
} SPUFreeze_t;*/
typedef struct
{
 char          szSPUName[8];
 uint32_t ulFreezeVersion;
 uint32_t ulFreezeSize;
 unsigned char cSPUPort[0x200];
 unsigned char cSPURam[0x80000];
 xa_decode_t   xaS;     
 unsigned char *SPUInfo;
} SPUFreeze_t;
long SPU_freeze(uint32_t, SPUFreeze_t *);
void SPU_async(uint32_t);
void SPU_playCDDAchannel(short *, int);

// PAD Functions
long PAD1_configure(void);
void PAD1_about(void);
long PAD1_init(long);
long PAD1_shutdown(void);	
long PAD1_test(void);		
long PAD1_close(void);
long PAD1_query(void);
long PAD1_readPort1(PadDataS*);
long PAD1_readPort2(PadDataS*);
long PAD1_keypressed(void);
unsigned char PAD1_startPoll(int);
unsigned char PAD1_poll(unsigned char);
void PAD1_setSensitive(int);
long PAD1_open(void);

// PAD Functions
long PAD2_configure(void);
void PAD2_about(void);
long PAD2_init(long);
long PAD2_shutdown(void);	
long PAD2_test(void);		
long PAD2_close(void);
long PAD2_query(void);
long PAD2_readPort1(PadDataS*);
long PAD2_readPort2(PadDataS*);
long PAD2_keypressed(void);
unsigned char PAD2_startPoll(int);
unsigned char PAD2_poll(unsigned char);
void PAD2_setSensitive(int);
long PAD2_open(void);

// NET Functions
long NET_init(void);
long NET_shutdown(void);
long NET_close(void);
long NET_configure(void);
long NET_test(void);
void NET_about(void);
void NET_pause(void);
void NET_resume(void);
long NET_queryPlayer(void);
long NET_sendData(void *, int, int);
long NET_recvData(void *, int, int);
long NET_sendPadData(void *, int);
long NET_recvPadData(void *, int);

typedef struct {
	char EmuName[32];
	char CdromID[9];	// ie. 'SCPH12345', no \0 trailing character
	char CdromLabel[11];
	void *psxMem;
	//GPUshowScreenPic GPU_showScreenPic;
	//GPUdisplayText GPU_displayText;
	//PADsetSensitive PAD_setSensitive;
	char GPUpath[256];	// paths must be absolute
	char SPUpath[256];
	char CDRpath[256];
	char MCD1path[256];
	char MCD2path[256];
	char BIOSpath[256];	// 'HLE' for internal bios
	char Unused[1024];
} netInfo;

void NET_setInfo(netInfo *);
void NET_keypressed(int);

#ifdef ENABLE_SIO1API

// SIO1 Functions (link cable)
long SIO1_init(void);
long SIO1_shutdown(void);
long SIO1_close(void);
long SIO1_configure(void);
long SIO1_test(void);
void SIO1_about(void);
void SIO1_pause(void);
void SIO1_resume(void);
long SIO1_keypressed(int);
void SIO1_writeData8(unsigned char);
void SIO1_writeData16(unsigned short);
void SIO1_writeData32(unsigned long);
void SIO1_writeStat16(unsigned short);
void SIO1_writeStat32(unsigned long);
void SIO1_writeMode16(unsigned short);
void SIO1_writeMode32(unsigned long);
void SIO1_writeCtrl16(unsigned short);
void SIO1_writeCtrl32(unsigned long);
void SIO1_writeBaud16(unsigned short);
void SIO1_writeBaud32(unsigned long);
unsigned char SIO1_readData8(void);
unsigned short SIO1_readData16(void);
unsigned long SIO1_readData32(void);
unsigned short SIO1_readStat16(void);
unsigned long SIO1_readStat32(void);
unsigned short SIO1_readMode16(void);
unsigned long SIO1_readMode32(void);
unsigned short SIO1_readCtrl16(void);
unsigned long SIO1_readCtrl32(void);
unsigned short SIO1_readBaud16(void);
unsigned long SIO1_readBaud32(void);
void SIO1_registerCallback(void (CALLBACK *callback)(void));

#endif

void CALLBACK clearDynarec(void);

void CALLBACK GPU_busy( int ticks );

void SetIsoFile(const char *filename);
const char *GetIsoFile(void);
boolean UsingIso(void);
void SetCdOpenCaseTime(s64 time);

#ifdef __cplusplus
}
#endif
#endif
