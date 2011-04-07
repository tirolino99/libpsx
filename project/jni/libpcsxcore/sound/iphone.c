#include "spu.h"
#include "externals.h"
#include "../minimal.h"

u32 last_ticks;

#define AUDIO_BUFFERS 12
#define AUDIO_BUFFER_SIZE (NSSIZE*2)
/*
static pthread_mutex_t sound_mutex;
static pthread_cond_t sound_cond;
*/
//static unsigned char sound_buffer[AUDIO_BUFFERS][AUDIO_BUFFER_SIZE];
static int sound_buffer_index = 0;

extern void *MAINThread(void *arg);

/* Start Sound Core */
void SetupSound(void)
{
#ifndef NOSOUND
	if( enable_audio == 0 ) return;
  
  sound_buffer_index = 0;
  last_ticks = 0;
/*
  memset(sound_buffer, 0, AUDIO_BUFFER_SIZE*AUDIO_BUFFERS);

	pthread_mutex_init(&sound_mutex, NULL);
	pthread_cond_init(&sound_cond, NULL);
*/
  demuteSound();
#endif
}

/* Stop Sound Core */
void RemoveSound(void)
{
#ifndef NOSOUND
	if( enable_audio == 0 ) return;
/*
 pthread_cond_signal(&sound_cond);
*/  muteSound();
/*
 pthread_cond_signal(&sound_cond);
  usleep(100000);
  pthread_cond_destroy(&sound_cond);
  pthread_mutex_destroy(&sound_mutex);
*/
#endif
}

void renderSound(unsigned char* stream, int len, int index)
{
#ifndef NOSOUND
	if( enable_audio == 0 ) return;

  if(!bEndThread)
  {
    /*
    struct timespec tspec;
    tspec.tv_sec = 0;
    tspec.tv_nsec = 900000000; // 1000000000 / (44100 / (45 * 2 * 2)) / 1.25
  	pthread_mutex_lock(&sound_mutex);
  	pthread_cond_timedwait(&sound_cond, &sound_mutex, &tspec);
  	pthread_mutex_unlock(&sound_mutex);
    */
    
    MAINThread(0);
    memcpy(stream, pSpuBuffer, (len < (AUDIO_BUFFER_SIZE) ? len : (AUDIO_BUFFER_SIZE)));
    
    //memcpy(stream, sound_buffer[index], (len < (AUDIO_BUFFER_SIZE) ? len : (AUDIO_BUFFER_SIZE)));
  }
#endif
}

/* Feed Sound Data */
void SoundFeedStreamData(unsigned char* pSound,long lBytes)
{
#ifndef NOSOUND
#if 0
	if( enable_audio == 0 ) return;
  
  if(pSound == NULL || lBytes <= 0)
    return;
  
  memcpy(sound_buffer[sound_buffer_index], pSound, (lBytes > (AUDIO_BUFFER_SIZE) ? (AUDIO_BUFFER_SIZE) : lBytes));
  sound_buffer_index++;
  if(sound_buffer_index >= AUDIO_BUFFERS)
  {
    sound_buffer_index = 0;
  }
#endif
/*
 pthread_cond_signal(&sound_cond);
*/
#endif
}

unsigned long SoundGetBytesBuffered(void)
{
	if( enable_audio == 0 ) return TESTSIZE+1;
	
	if(last_ticks == 0)
	{
	  last_ticks = timeGetTime_spu();
	}
	
	if(timeGetTime_spu() >= last_ticks + 2)
	{
	  last_ticks = timeGetTime_spu();
    return 0;
	}
	
  return TESTSIZE+1;
}
