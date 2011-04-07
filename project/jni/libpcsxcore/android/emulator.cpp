#include <sys/stat.h>
#include <errno.h>
#include <dlfcn.h>
#include <pthread.h>

#define LOG_TAG "libpsx Emulator"
#include <android/log.h>
#ifdef ANDROID_ARMV7
#include <cpu-features.h>
#endif

#include "emumedia.h"
#include "../psxcommon.h"
#include "../cdrom.h"
#include "../plugins.h"
#include "../sio.h"
#include "../misc.h"
#include "../r3000a.h"
//#include "../video/externals.h"
#include "minimal.h"

#define CONFIG_DIR		"/sdcard/.psx4droid"

#define SCREEN_W		emu_screen_w
#define SCREEN_H		emu_screen_h
#define SCREEN_PITCH	(emu_screen_pitch)

#include <GLES/glplatform.h>
#include <GLES/gl.h>
#include <GLES/glext.h>
#include <EGL/egl.h>

enum {
	EMUSTATE_PAUSED,
	EMUSTATE_RUNNING,
	EMUSTATE_LOADING,
	EMUSTATE_REQUEST_PAUSE
};

#ifdef ANDROID_ARMV7
int isARMv7 = 0;
int isARMneon = 0;
#endif

static pthread_t emuThread;
//static pthread_mutex_t emuStateMutex;
//static pthread_cond_t emuStateCond;
static int emuState = EMUSTATE_PAUSED;
static int savedState = 0;

static unsigned int keyStates;
static bool initialized;
static bool romLoaded = false;
static bool flipScreen;
static int refreshRate;

static unsigned int refreshTime;
static u64 lastFrameDrawnTime;

static EmuMedia *media;
static JavaVM *jvm;
static JNIEnv *emuThreadEnv;
static jobject jRenderer;

static JNIEnv *audioThreadEnv = NULL;

static struct {
	int key;
	int duration;
} trackballEvents[2];

int current_enable_audio = 0;
int global_enable_audio = 0;
int global_enable_cdda_audio = 0;
int __saved = 0;
int psx4all_emulating = 0;
char cfgfile[256];
long LoadCdBios;
char savestate_filename[1024] = {'\0'};
int global_spu_irq = 0;
int global_enable_frameskip = 1;
int global_rcnt_fix = 0;
unsigned int global_bias_cycle = 3;
int global_cdrom_precise = 0;
int scaled_width = 320;
int scaled_height = 240;
int global_scale_stretch = 0;

extern int stop;
	
extern BOOL displayFrameInfo;
extern BOOL enableAbbeyHack;
extern u32 DrawingCount[4];
  
extern EmuMedia *createEmuMedia();
extern "C" void setScreenSize(int width, int height);

extern "C" EGLDisplay display;
extern "C" EGLConfig  config;
extern "C" EGLContext context;
extern "C" EGLSurface surface;

extern "C" int iResX;
extern "C" int iResY;
extern "C" int updateAspect;
extern "C" long GPU_getScreenshot(u8 * pMem);

static void *loadSharedObject(const char *dir, const char *lib)
{
	char path[1024];
	snprintf(path, sizeof(path), "%s/lib%s.so", dir, lib);

	void *handle = dlopen(path, RTLD_NOW);
	if (handle == NULL) {
		__android_log_print(ANDROID_LOG_WARN, LOG_TAG,
				"Cannot load %s: %s", path, dlerror());
		return NULL;
	}

	void *(*createObject)() = (void *(*)()) dlsym(handle, "createObject");
	if (createObject == NULL) {
		dlclose(handle);
		return NULL;
	}
	return createObject();
}

static void pauseEmulator(JNIEnv *env)
{
  if(!romLoaded)
    return;
	//pthread_mutex_lock(&emuStateMutex);
	if (emuState == EMUSTATE_RUNNING) 
	{
		emuState = EMUSTATE_REQUEST_PAUSE;
		while (emuState == EMUSTATE_REQUEST_PAUSE)
		{
      usleep(100000);
	    //pthread_cond_wait(&emuStateCond, &emuStateMutex);
		}
	}
		
	//pthread_mutex_unlock(&emuStateMutex);
	if (current_enable_audio && media)
		media->audioPause(env);
}

static void resumeEmulator(int restart = 0)
{
  //if(!romLoaded)
  //  return;
  
	emuState = EMUSTATE_RUNNING;
}

static void unloadROM(JNIEnv *env)
{
	if (!initialized || !romLoaded)
		return;
	//pauseEmulator(env);
  stop = 1;
  psx4all_emulating = 0;
  emuState = EMUSTATE_RUNNING;
	while (psx4all_emulating == 0)
	{
    usleep(100000);
	}
  romLoaded = false;
  emuState = EMUSTATE_PAUSED;
  if(current_enable_audio && media)
	  media->audioStop(env);
	if(media)
	{
	  media->destroy(env);
	  media = NULL;
	}
	emuThreadEnv = NULL;
}

static int waitForStart()
{
	//pthread_mutex_lock(&emuStateMutex);
	while (emuState == EMUSTATE_PAUSED)
	{
    usleep(100000);
		//pthread_cond_wait(&emuStateCond, &emuStateMutex);
  }
	if (current_enable_audio && media)
		media->audioStart(emuThreadEnv);

	//pthread_mutex_unlock(&emuStateMutex);
	refreshTime = (refreshRate ? (1000 / refreshRate) : 0);
	lastFrameDrawnTime = 0;
	return 0;
}

static void *emuThreadProc(void *arg)
{
  //struct sched_param thread_param;
	
	//jvm->AttachCurrentThread(&emuThreadEnv, NULL);
  
  //thread_param.sched_priority = sched_get_priority_max(SCHED_RR);
  //pthread_setschedparam(pthread_self(), SCHED_RR, &thread_param);
        
  stop = 0;
  romLoaded = true;
	//waitForStart();
  //setScreenSize(320, 240);
  Config.SpuIrq = global_spu_irq;
  Config.RCntFix = global_rcnt_fix;
  Config.Cdda = (global_enable_cdda_audio ? 0 : 1);
  bias_cycle = global_bias_cycle;
  bias_vsync = 1; // TEMP
  cdrom_precise = global_cdrom_precise;
	if (current_enable_audio && media)
		media->audioStart(emuThreadEnv);
  emuState = EMUSTATE_RUNNING;
  
  if (SysInit() == -1) 
  {
    gp2x_deinit();
    emuState = EMUSTATE_PAUSED;
    return NULL;
  }

  if (LoadPlugins() == -1) 
  {
    gp2x_deinit();
    emuState = EMUSTATE_PAUSED;
    return NULL;
  }

  CheckCdrom();

  SysReset();

  if(psx4all_emulating == 0)
  {
    SysClose();
    gp2x_deinit();
    return NULL;
  }
  LoadCdBios = 0;

  if (LoadCdrom() == -1) 
  {
    SysClose();
    gp2x_deinit();
    emuState = EMUSTATE_PAUSED;
    return NULL;
  }

  if(psx4all_emulating)
  {
    psxCpu->Execute();
  }

  SysClose();
  gp2x_deinit();
	return NULL;
}

static void emu_config()
{
  //memset(&Config, 0, sizeof(PcsxConfig));
  Config.UseNet = 0;
  Config.HLE = 0;
  strcpy(cfgfile, CONFIG_DIR"/Pcsx.cfg");
  strcpy(Config.Net, _("Disabled"));

  strcpy(Config.Mcd1, CONFIG_DIR"/mcd001.mcr");
  strcpy(Config.Mcd2, CONFIG_DIR"/mcd002.mcr");
  Config.PsxAuto = 1;
  Config.Cdda = 1;
  Config.Xa = 0;
#ifdef DYNAREC
	Config.Cpu = CPU_DYNAREC;
#else
 	Config.Cpu = CPU_INTERPRETER;
#endif
	Config.Mdec = 0;
	Config.PsxOut = 0;
	Config.PsxType = 0;
	//Config.QKeys = 0;
	Config.RCntFix = 0;
	Config.Sio = 0;
	Config.SpuIrq = 0;
	Config.VSyncWA = 0;
}


static void render_frame(const EmuMedia::Surface &info)
{
	unsigned char *d = (unsigned char *) info.bits;
	unsigned char *s = (unsigned char *) screenBuffer;
	int h = SCREEN_H;

  if (info.bpr > 0) {
		while (--h >= 0) {
			memcpy(d, s, SCREEN_PITCH);
			d += info.bpr;
			s += SCREEN_PITCH;
		}
	} else {
		d += (h - 1) * -info.bpr + SCREEN_PITCH;
		while (--h >= 0) {
			unsigned int *src = (unsigned int *) s;
			unsigned int *dst = (unsigned int *) d;
			for (int w = SCREEN_W / 2; --w >= 0; src++)
				*--dst = (*src << 16) | (*src >> 16);

			d += info.bpr;
			s += SCREEN_PITCH;
		}
	}
}

extern "C" int is_paused()
{
	if (emuState == EMUSTATE_PAUSED || emuState == EMUSTATE_LOADING)
		return 1;
  return 0;
}

extern "C" int check_paused()
{
	//pthread_mutex_lock(&emuStateMutex);
	if (emuState == EMUSTATE_REQUEST_PAUSE) {
	  if(media && jRenderer)
	  {
      media->setRenderer(emuThreadEnv, jRenderer);
      media->needScreenshot(emuThreadEnv);
	  }
		SaveState((char *) CONFIG_DIR"/tmpSaveState");
		emuState = EMUSTATE_PAUSED;
		//pthread_cond_signal(&emuStateCond);
	}
	//pthread_mutex_unlock(&emuStateMutex);
  
	if (emuState == EMUSTATE_RUNNING)
	{
    if(savestate_filename[0] != '\0')
    {
  	  LoadState(savestate_filename);
  	  savestate_filename[0] = '\0';
      savedState = 0;
    }
    else if(savedState)
    {
  	  LoadState((char*) CONFIG_DIR"/tmpSaveState");
      savedState = 0;
    }
		return 0;
	}
		
	return waitForStart();
}

extern "C" u32 get_key_states()
{
	u32 states = keyStates;
	for (int i = 0; i < 2; i++) {
		if (trackballEvents[i].duration > 0) {
			trackballEvents[i].duration--;
			states |= trackballEvents[i].key;
		}
	}
	return states;
}

extern "C" void setScreenSize(int width, int height)
{
  if (media && romLoaded)
  {
    scaled_width = width;
    scaled_height = height;
	  media->setSurfaceSize(emuThreadEnv, width, height);
  }
}

extern "C" void flip_screen()
{
	if (refreshTime) {
		u64 now = gp2x_timer_read();
		if (now - lastFrameDrawnTime < refreshTime)
			return;
		lastFrameDrawnTime = now;
	}

	EmuMedia::Surface info;
	if (media && media->lockSurface(emuThreadEnv, &info, flipScreen)) {
#if 0
		render_frame(info);
#endif
		media->unlockSurface(emuThreadEnv);
	}
}

extern "C" void start_audio()
{
  jvm->AttachCurrentThread(&audioThreadEnv, NULL);
}

extern "C" void render_audio(s16 *data, u32 size)
{
  if(current_enable_audio && media)
	  media->audioPlay((audioThreadEnv == NULL ? emuThreadEnv : audioThreadEnv), data, size);
}

extern "C" void end_audio()
{
	jvm->DetachCurrentThread();
  audioThreadEnv = NULL;
}

extern "C" __attribute__((visibility("default")))
jboolean Java_com_androidemu_Emulator_initialize(
		JNIEnv *env, jobject self,
		jstring jlibdir, jstring jdatadir, jint sdk)
{
	flipScreen = false;
	initialized = false;
  scaled_width = 320;
  scaled_height = 240;
  
  emuThreadEnv = env;
  
	if (media == NULL)
		media = createEmuMedia();
	if (!media->init(env))
		return JNI_FALSE;

  current_enable_audio = global_enable_audio;
  
  if(current_enable_audio && media)
	  media->audioCreate(env, 44100, 16, 2);

  isARMv7 = ((android_getCpuFeatures() & ANDROID_CPU_ARM_FEATURE_ARMv7) != 0);
  isARMneon = ((android_getCpuFeatures() & ANDROID_CPU_ARM_FEATURE_NEON) != 0);

  // create config dir if not existing
  if (mkdir(CONFIG_DIR, 0755) != 0 && errno != EEXIST) {
    __android_log_print(ANDROID_LOG_WARN, LOG_TAG,
        "Cannot make config dir: %s", strerror(errno));
  }

  emu_config();
  gp2x_init(1000, 16, 11025, 16, 1, 60, 1);

	//pthread_mutex_init(&emuStateMutex, NULL);
	//pthread_cond_init(&emuStateCond, NULL);
  initialized = true;
  psx4all_emulating = 1;
  emuThreadProc(NULL);
  psx4all_emulating = -1;
	//pthread_create(&emuThread, NULL, emuThreadProc, NULL);
	return JNI_TRUE;
}

extern "C" __attribute__((visibility("default")))
void Java_com_androidemu_Emulator_cleanUp(JNIEnv *env, jobject self)
{
}

extern "C" __attribute__((visibility("default")))
void Java_com_androidemu_Emulator_setSurface(
		JNIEnv *env, jobject self, jobject surface)
{
  if(media)
	  media->setSurface(env, surface);
}

extern "C" __attribute__((visibility("default")))
void Java_com_androidemu_Emulator_setSurfaceRegion(
		JNIEnv *env, jobject self,
		int x, int y, int w, int h)
{
  if(media)
	  media->setSurfaceRegion(env, x, y, w, h);
	
  updateAspect = 1;
}

extern "C" __attribute__((visibility("default")))
void Java_com_androidemu_Emulator_step(
		JNIEnv *env, jobject self)
{
}

extern "C" __attribute__((visibility("default")))
void Java_com_androidemu_Emulator_setOverlay(
		JNIEnv *env, jobject self, jobject pic)
{
  if(media)
	  media->setOverlay(env, pic);
}

extern "C" __attribute__((visibility("default")))
void Java_com_androidemu_Emulator_setKeyStates(
		JNIEnv *env, jobject self, jint states)
{
	keyStates = states;
}

extern "C" __attribute__((visibility("default")))
void Java_com_androidemu_Emulator_processTrackball(
		JNIEnv *env, jobject self,
		jint key1, jint duration1, jint key2, jint duration2)
{
	const int key[] = { key1, key2 };
	const int duration[] = { duration1, duration2 };

	for (int i = 0; i < 2; i++) {
		if (key[i] == 0)
			continue;

		if (trackballEvents[i].key != key[i]) {
			trackballEvents[i].duration = 0;
			trackballEvents[i].key = key[i];
		}
		if ((trackballEvents[i].duration += duration[i]) > 80)
			trackballEvents[i].duration = 80;
	}
}

extern "C" __attribute__((visibility("default")))
void Java_com_androidemu_Emulator_setNativeOption(JNIEnv *env, jobject self,
		jstring jname, jstring jvalue)
{
	const char *name = env->GetStringUTFChars(jname, NULL);
	const char *value = NULL;
	if (jvalue != NULL)
		value = env->GetStringUTFChars(jvalue, NULL);

	if (strcmp(name, "refreshRate") == 0) {
		if (strcmp(value, "default") == 0) 
		{
		  refreshRate = 60;
		}
		else
		{
		  refreshRate = atoi(value);
    }
	} else if (strcmp(name, "soundEnabled") == 0) {
		global_enable_audio = (strcmp(value, "true") == 0);

	} else if (strcmp(name, "cddaEnabled") == 0) {
		global_enable_cdda_audio = (strcmp(value, "true") == 0);

	} else if (strcmp(name, "flipScreen") == 0) {
		flipScreen = (strcmp(value, "true") == 0);

	} else if (strcmp(name, "spuIrq") == 0) {
		global_spu_irq = (strcmp(value, "true") == 0);

	} else if (strcmp(name, "rcntFix") == 0) {
		global_rcnt_fix = (strcmp(value, "true") == 0);

  } else if (strcmp(name, "showFPS") == 0) {
  	displayFrameInfo = (strcmp(value, "true") == 0);

  } else if (strcmp(name, "scaleMode") == 0) {
  	global_scale_stretch = (strcmp(value, "true") == 0);

  } else if (strcmp(name, "cycleMult2") == 0) {
  	global_bias_cycle = atoi(value);

  } else if (strcmp(name, "cdromPrecise") == 0) {
  	global_cdrom_precise = (strcmp(value, "true") == 0);
  		
  } else if (strcmp(name, "enableFrameskip") == 0) {
  	global_enable_frameskip = (strcmp(value, "true") == 0);		
    if(global_enable_frameskip)
    {
      DrawingCount[0] = 4;
      DrawingCount[1] = 4;
    }
    else
    {
      DrawingCount[0] = 0;
      DrawingCount[1] = 0;
    }
	}

	env->ReleaseStringUTFChars(jname, name);
	if (jvalue != NULL)
		env->ReleaseStringUTFChars(jvalue, value);
}

extern "C" __attribute__((visibility("default")))
void Java_com_androidemu_Emulator_getScreenshot(
		JNIEnv *env, jobject self, jobject jbuffer)
{
	EmuMedia::Surface surface;
	surface.bits = env->GetDirectBufferAddress(jbuffer);
	surface.w = SCREEN_W;
	surface.h = SCREEN_H;
	surface.bpr = SCREEN_PITCH;
	render_frame(surface);
}

extern "C" __attribute__((visibility("default")))
void Java_com_androidemu_Emulator_reset(JNIEnv *env, jobject self)
{
  SysReset();
  CheckCdrom();
  LoadCdBios = 0;
  LoadCdrom();
}

extern "C" __attribute__((visibility("default")))
void Java_com_androidemu_Emulator_power(JNIEnv *env, jobject self)
{
	Java_com_androidemu_Emulator_reset(env, self);
}

extern "C" __attribute__((visibility("default")))
jboolean Java_com_androidemu_Emulator_loadBIOS(
		JNIEnv *env, jobject self, jstring jfile)
{
	const char *file = env->GetStringUTFChars(jfile, NULL);

	strcpy(Config.Bios, file);
	strcpy(Config.BiosDir, "/");

	env->ReleaseStringUTFChars(jfile, file);
	return JNI_TRUE;
}

extern "C" __attribute__((visibility("default")))
jboolean Java_com_androidemu_Emulator_isROMLoaded(JNIEnv *env, jobject self)
{
	return romLoaded;
}

extern "C" __attribute__((visibility("default")))
jboolean Java_com_androidemu_Emulator_isPaused(JNIEnv *env, jobject self)
{
	return (emuState == EMUSTATE_PAUSED);
}

extern "C" __attribute__((visibility("default")))
jboolean Java_com_androidemu_Emulator_loadROM(
		JNIEnv *env, jobject self, jstring jfile)
{
	//unloadROM(env);

	const char *file = env->GetStringUTFChars(jfile, NULL);
	jboolean rv = JNI_FALSE;

  SetIsoFile(file);
	
	memset(trackballEvents, 0, sizeof(trackballEvents));
	
	rv = JNI_TRUE;
error:
	env->ReleaseStringUTFChars(jfile, file);
	return rv;
}

extern "C" __attribute__((visibility("default")))
void Java_com_androidemu_Emulator_unloadROM(JNIEnv *env, jobject self)
{
	unloadROM(env);
}

extern "C" __attribute__((visibility("default")))
void Java_com_androidemu_Emulator_pause(JNIEnv *env, jobject self)
{
	pauseEmulator(env);
}

extern "C" __attribute__((visibility("default")))
void Java_com_androidemu_Emulator_resume(JNIEnv *env, jobject self)
{
	resumeEmulator();
}

extern "C" __attribute__((visibility("default")))
jboolean Java_com_androidemu_Emulator_saveState(
		JNIEnv *env, jobject self, jstring jfile)
{
  char syscmd[1024*2+64];
	const char *file = env->GetStringUTFChars(jfile, NULL);

  snprintf(syscmd, 1024*2+64, "\\cp -f %s %s", CONFIG_DIR"/tmpSaveState", file);
  system(syscmd);
  
	//SaveState((char *) file);

	env->ReleaseStringUTFChars(jfile, file);
	return JNI_TRUE;
}

extern "C" __attribute__((visibility("default")))
jboolean Java_com_androidemu_Emulator_loadState(
		JNIEnv *env, jobject self, jstring jfile)
{
	const char *file = env->GetStringUTFChars(jfile, NULL);
  snprintf(savestate_filename, 1024, "%s", (char*)file);
  env->ReleaseStringUTFChars(jfile, file);
  return JNI_TRUE;
}

extern "C" __attribute__((visibility("default")))
jboolean Java_com_androidemu_Emulator_ejectCD(
		JNIEnv *env, jobject self, jstring jfile)
{
	const char *file = env->GetStringUTFChars(jfile, NULL);

	CDR_close();
	SetCdOpenCaseTime(time(NULL) + 2);
	SetIsoFile(file);
	CDR_open();

	env->ReleaseStringUTFChars(jfile, file);
	return JNI_TRUE;
}

extern "C" __attribute__((visibility("default")))
void Java_com_androidemu_Emulator_setupGraphics(
		JNIEnv *env, jobject self, jobject thiz)
{
  jRenderer = thiz;
}

__attribute__((visibility("default")))
jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
    jvm = vm;
    return JNI_VERSION_1_4;
}


