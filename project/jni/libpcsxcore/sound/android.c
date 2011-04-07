#include "spu.h"
#include "externals.h"
#include "../minimal.h"

#ifndef NOSOUND
extern void render_audio(s16 *data, u32 size);
extern void start_audio();
extern void end_audio();
extern int is_paused();
#endif

u32 last_ticks;
u32 last_render_ticks;

static pthread_mutex_t sound_mutex;
static pthread_cond_t sound_cond;
pthread_t sound_thread=0;
volatile int sound_thread_exit=0;
unsigned int sound_buffer_bytes = 0;
unsigned int sound_new_buffer = 0;
static void *sound_thread_play(void *none);
static unsigned char sound_buffer[32768];
static unsigned char sound_buffer_silence[32768];

/* Start Sound Core */
void SetupSound(void)
{
#ifndef NOSOUND
	if( enable_audio == 0 ) return;
#if 0
  last_ticks = 0;
  last_render_ticks = 0;
#endif
#if 0
  sound_buffer_bytes = 0;
  sound_new_buffer = 0;
  sound_thread_exit = 0;
	pthread_create( &sound_thread, NULL, sound_thread_play, NULL);
#else
  memset(sound_buffer_silence, 0, 882 * 2);
#endif
#endif
}

/* Stop Sound Core */
void RemoveSound(void)
{
#ifndef NOSOUND
	if( enable_audio == 0 ) return;
#if 0
  sound_thread_exit = 1;
  pthread_cond_signal(&sound_cond);
#endif
#endif
}

static void *sound_thread_play(void *none)
{
#ifndef NOSOUND
#if 0
	if( enable_audio == 0 ) return NULL;
	pthread_mutex_init(&sound_mutex, NULL);
	pthread_cond_init(&sound_cond, NULL);
  start_audio();
  while(!sound_thread_exit && !bEndThread)
  {
  	pthread_mutex_lock(&sound_mutex);
  	pthread_cond_wait(&sound_cond, &sound_mutex);
  	pthread_mutex_unlock(&sound_mutex);
  	if(pSpuBuffer)
      render_audio((s16*)pSpuBuffer, sound_buffer_bytes);
  }
  sound_thread_exit = 0;
  pthread_cond_destroy(&sound_cond);
  pthread_mutex_destroy(&sound_mutex);
  end_audio();
#endif
  return NULL;
#endif
}

void CheckPauseSound()
{
  while(is_paused() && bEndThread == 0)
  {
    usleep(20000);
    render_audio((s16*)sound_buffer_silence, 882 * 2);
  }
}

/* Feed Sound Data */
void SoundFeedStreamData(unsigned char* pSound,long lBytes)
{
#ifndef NOSOUND
  //u32 render_ticks;
	if( enable_audio == 0 ) return;
  
  if(pSound == NULL || lBytes <= 0 || lBytes > 32768) // || sound_new_buffer != 0)
    return;

  //memcpy(sound_buffer, pSound, lBytes);
  //sound_buffer_bytes = lBytes;
  //pthread_cond_signal(&sound_cond);
  render_audio((s16*)pSound, lBytes);
  
#if 0
	if(last_render_ticks == 0)
	{
	  last_render_ticks = timeGetTime_spu();
	}
	
  render_ticks = timeGetTime_spu();
	if(render_ticks < last_render_ticks + 5)
	{
    u32 render_delta = (last_render_ticks + 5) - render_ticks;
    if(render_delta < 5)
    {
      usleep(render_delta * 1000);
    }
	}
	last_render_ticks = timeGetTime_spu();
#endif
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
