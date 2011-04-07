#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>
#include <android/bitmap.h>
#include "emumedia.h"

#define LOG_TAG "libpsx EmuMedia"
#include <android/log.h>

extern int scaled_width;
extern int scaled_height;

unsigned char screenBuffer[1024*1024*2];

class EmuMediaImpl : public EmuMedia {
public:
	virtual bool init(JNIEnv *env);
	virtual void destroy(JNIEnv *env);

	virtual void setSurface(JNIEnv *env, jobject holder);
	virtual void setRenderer(JNIEnv *env, jobject holder);
	virtual void setSurfaceRegion(JNIEnv *env, int x, int y, int w, int h);
	virtual void setOverlay(JNIEnv *env, jobject pic);
	virtual bool lockSurface(JNIEnv *env, Surface *info, bool flip);
	virtual void unlockSurface(JNIEnv *env);
	virtual void setSurfaceSize(JNIEnv *env,
			int width, int height);
	virtual void needScreenshot(JNIEnv *env);
	virtual void restartEGL(JNIEnv *env);
			
	virtual bool audioCreate(JNIEnv *env,
			unsigned int rate, int bits, int channels);
	virtual void audioStart(JNIEnv *env);
	virtual void audioStop(JNIEnv *env);
	virtual void audioPause(JNIEnv *env);
	virtual void audioPlay(JNIEnv *env, void *data, int size);

private:
	void releaseVideoBuffer(JNIEnv *env);

	jclass jPeerClass;
	jclass jPeerClassView;
  jclass jPeerClassRenderer;
  jobject jPeerRenderer;
	jmethodID midGetRowBytes;
	jmethodID midDestroy;
	jmethodID midSetSurface;
	jmethodID midSetSurfaceRegion;
	jmethodID midSetOverlay;
	jmethodID midBitBlt;
	jmethodID midSetSurfaceSize;
  jmethodID midNeedScreenshot;
  jmethodID midRestartEGL;
	jmethodID midAudioCreate;
	jmethodID midAudioStart;
	jmethodID midAudioStop;
	jmethodID midAudioPause;
	jmethodID midAudioPlay;

	jobject jVideoBuffer;
	jbyteArray jAudioBuffer;
	int screenWidth;
	int screenHeight;
	int screenPitch;
	bool flipScreen;
};


void EmuMediaImpl::releaseVideoBuffer(JNIEnv *env)
{
	if (jVideoBuffer != NULL) {
		env->DeleteGlobalRef(jVideoBuffer);
		jVideoBuffer = NULL;
	}
	/*if (screenBuffer != NULL) {
		free(screenBuffer);
		screenBuffer = NULL;
	}*/
}

bool EmuMediaImpl::init(JNIEnv *env)
{
  jPeerRenderer = NULL;
	jVideoBuffer = NULL;
  jAudioBuffer = NULL;
	//screenBuffer = NULL;
	flipScreen = false;

	jPeerClass = env->FindClass("com/androidemu/EmuMedia");
	jPeerClass = (jclass) env->NewGlobalRef(jPeerClass);

	jPeerClassView = env->FindClass("com/androidemu/EmulatorViewRun");
	jPeerClassView = (jclass) env->NewGlobalRef(jPeerClassView);
	
	midSetSurfaceSize = env->GetStaticMethodID(jPeerClassView,
			"setSurfaceSize", "(II)V");

	jAudioBuffer = env->NewByteArray(32768);
	jAudioBuffer = (jbyteArray) env->NewGlobalRef(jAudioBuffer);

	midDestroy = env->GetStaticMethodID(jPeerClass, "destroy", "()V");
	midSetSurface = env->GetStaticMethodID(jPeerClass,
			"setSurface", "(Landroid/view/SurfaceHolder;)V");
	midSetSurfaceRegion = env->GetStaticMethodID(jPeerClass,
			"setSurfaceRegion", "(IIIIII)Landroid/graphics/Bitmap;");
	midSetOverlay = env->GetStaticMethodID(jPeerClass,
			"setOverlay", "(Landroid/graphics/Picture;)V");
	midBitBlt = env->GetStaticMethodID(jPeerClass,
			"bitBlt", "(Ljava/nio/Buffer;Z)V");
	midAudioCreate = env->GetStaticMethodID(jPeerClass,
			"audioCreate", "(III)V");
	midAudioStart = env->GetStaticMethodID(jPeerClass, "audioStart", "()V");
	midAudioStop = env->GetStaticMethodID(jPeerClass, "audioStop", "()V");
	midAudioPause = env->GetStaticMethodID(jPeerClass, "audioPause", "()V");
	midAudioPlay = env->GetStaticMethodID(jPeerClass, "audioPlay", "([BI)V");

	return true;
}

void EmuMediaImpl::destroy(JNIEnv *env)
{
	env->CallStaticVoidMethod(jPeerClass, midDestroy);
	env->DeleteGlobalRef(jAudioBuffer);
	env->DeleteGlobalRef(jPeerClass);
	env->DeleteGlobalRef(jPeerClassView);
  jPeerRenderer = NULL;
  
	releaseVideoBuffer(env);
	delete this;
}

void EmuMediaImpl::setSurface(JNIEnv *env, jobject holder)
{
	//env->CallStaticVoidMethod(jPeerClass, midSetSurface, holder);

	if (holder == NULL)
		releaseVideoBuffer(env);
}

void EmuMediaImpl::setRenderer(JNIEnv *env, jobject holder)
{
#if 0
	env->CallStaticVoidMethod(jPeerClass, midSetSurface, holder);

	if (holder == NULL)
		releaseVideoBuffer(env);
#endif
  if(jPeerRenderer == NULL)
  {
    jPeerRenderer = holder;
	  jPeerClassRenderer = env->GetObjectClass(holder);
	  midNeedScreenshot = env->GetMethodID(jPeerClassRenderer, "needScreenshot", "()V");
	  midRestartEGL = env->GetMethodID(jPeerClassRenderer, "restartEGL", "()V");
	}
}

void EmuMediaImpl::setSurfaceRegion(JNIEnv *env,
		int x, int y, int w, int h)
{
	releaseVideoBuffer(env);

	jobject bitmap = env->CallStaticObjectMethod(jPeerClass,
			midSetSurfaceRegion, x, y, w, h, scaled_width, scaled_height);

	screenWidth = scaled_width;
	screenHeight = scaled_height;
  screenPitch = screenWidth * 2; //env->CallIntMethod(bitmap, midGetRowBytes);
	const int size = screenPitch * scaled_height;
	//screenBuffer = malloc(size);
	jVideoBuffer = env->NewDirectByteBuffer(screenBuffer, size);
	jVideoBuffer = env->NewGlobalRef(jVideoBuffer);
}

void EmuMediaImpl::setOverlay(JNIEnv *env, jobject pic)
{
	env->CallStaticVoidMethod(jPeerClass, midSetOverlay, pic);
}

bool EmuMediaImpl::lockSurface(JNIEnv *env, Surface *info, bool flip)
{
  if(jVideoBuffer == NULL)
  {
    return false;
  }
  
	flipScreen = flip;

	info->bpr = screenPitch;
	info->w = screenWidth;
	info->h = screenHeight;
  info->bits = screenBuffer;
  
	return true;
}

void EmuMediaImpl::unlockSurface(JNIEnv *env)
{ 
	env->CallStaticVoidMethod(jPeerClass, midBitBlt,
			jVideoBuffer, flipScreen);
}

void EmuMediaImpl::setSurfaceSize(JNIEnv *env,
		int width, int height)
{
	env->CallStaticVoidMethod(jPeerClassView,
			midSetSurfaceSize, width, height);
}

void EmuMediaImpl::restartEGL(JNIEnv *env)
{ 
  env->CallVoidMethod(jPeerRenderer, midRestartEGL);
}

void EmuMediaImpl::needScreenshot(JNIEnv *env)
{ 
  env->CallVoidMethod(jPeerRenderer, midNeedScreenshot);
}

bool EmuMediaImpl::audioCreate(JNIEnv *env,
		unsigned int rate, int bits, int channels)
{
	env->CallStaticVoidMethod(jPeerClass,
			midAudioCreate, rate, bits, channels);
	return true;
}

void EmuMediaImpl::audioStart(JNIEnv *env)
{
	env->CallStaticVoidMethod(jPeerClass, midAudioStart);
}

void EmuMediaImpl::audioStop(JNIEnv *env)
{
	env->CallStaticVoidMethod(jPeerClass, midAudioStop);
}

void EmuMediaImpl::audioPause(JNIEnv *env)
{
	env->CallStaticVoidMethod(jPeerClass, midAudioPause);
}

void EmuMediaImpl::audioPlay(JNIEnv *env, void *src, int size)
{
	env->SetByteArrayRegion(jAudioBuffer, 0, size, (jbyte *) src);
	env->CallStaticVoidMethod(jPeerClass, midAudioPlay, jAudioBuffer, size);
}


EmuMedia *createEmuMedia()
{
	return new EmuMediaImpl;
}
