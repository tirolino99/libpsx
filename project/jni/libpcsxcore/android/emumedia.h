#ifndef EMUMEDIA_H
#define EMUMEDIA_H

#include <jni.h>

class EmuMedia {
public:
	struct Surface {
		void *bits;
		int bpr;
		int w;
		int h;
	};

	virtual ~EmuMedia() {}

	virtual bool init(JNIEnv *env) = 0;
	virtual void destroy(JNIEnv *env) = 0;

	virtual void setSurface(JNIEnv *env, jobject holder) = 0;
	virtual void setRenderer(JNIEnv *env, jobject holder) = 0;
	virtual void setSurfaceRegion(JNIEnv *env,
			int x, int y, int w, int h) = 0;
	virtual void setOverlay(JNIEnv *env, jobject pic) = 0;
	virtual bool lockSurface(JNIEnv *env, Surface *info, bool flip) = 0;
	virtual void unlockSurface(JNIEnv *env) = 0;
  virtual void needScreenshot(JNIEnv *env) = 0;
  virtual void restartEGL(JNIEnv *env) = 0;
	virtual void setSurfaceSize(JNIEnv *env,
			int width, int height) = 0;
			
	virtual bool audioCreate(JNIEnv *env,
			unsigned int rate, int bits, int channels) = 0;
	virtual void audioStart(JNIEnv *env) = 0;
	virtual void audioStop(JNIEnv *env) = 0;
	virtual void audioPause(JNIEnv *env) = 0;
	virtual void audioPlay(JNIEnv *env, void *data, int size) = 0;
};

#endif

