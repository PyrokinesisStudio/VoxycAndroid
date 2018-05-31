/*
Copyright (C) 2018 Dimitri Lozovoy

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "Globals.hpp"
#include "Engine.hpp"
#include "tilemap.h"
#include "entity.h"
#include "drawdata.h"
#include "transformer.h"
#include "platform.h"
#include "Engine.hpp"
#include "build3d.h"
#include <string.h>
#include <jni.h>
#include "main.h"
#include "Controls.hpp"
#include "Utils.hpp"

#define JNI_VERSION_1_6 0x00010006

using namespace std;

JavaVM *g_jvm = 0;

jclass g_objMainActivity = 0;
jclass g_classMainActivity = 0;
jmethodID g_open = 0;
jmethodID g_getWidth = 0;
jmethodID g_getHeight = 0;
jmethodID g_getPixels = 0;
jmethodID g_close = 0;
jmethodID g_assetOpen = 0;
jmethodID g_assetReadByte = 0;
jmethodID g_assetReadInt = 0;
jmethodID g_assetReadFloat = 0;
jmethodID g_getAssetManager = 0;
jmethodID g_loadSound = 0;
jmethodID g_playSound = 0;
jmethodID g_stopSound = 0;
jmethodID g_playTrack = 0;
jmethodID g_stopTrack = 0;

bool g_attached = false;

jint JNI_OnLoad(JavaVM *vm, void *reserved)
{
	g_jvm = vm;

	return JNI_VERSION_1_6;
}

//extern "C" JNIEXPORT jint JNICALL Java_oculus_MainActivity_attachThread(JNIEnv *env, jobject thiz)
//{
//}

extern "C" JNIEXPORT jint JNICALL Java_oculus_MainActivity_registerEnv(JNIEnv *env, jobject thiz)
{
//    AttachThread();

//    g_env = env;
}

/*extern "C" JNIEXPORT jint JNICALL Java_oculus_MainActivity_keyEvent(JNIEnv *env, jobject thiz, int action, int keyCode)
{
    // Action 0 = down
    // Action 1 = up

    Controls *controls = g_app->GetControls();

    if (action == 0)
    {
        switch (keyCode)
        {
            case KEYCODE_DPAD_UP:
                controls->MoveForward();
                break;
            case KEYCODE_DPAD_DOWN:
                controls->MoveBackward();
                break;
            case KEYCODE_DPAD_LEFT:
                controls->MoveLeft();
                break;
            case KEYCODE_DPAD_RIGHT:
                controls->MoveRight();
                break;
        }
    }
}*/

void AttachThread()
{
/*    if (!g_attached)
    {
        g_jvm->AttachCurrentThread((void**)&g_env, NULL);
        g_attached = true;
    }*/
}

void PLAT_LoadBitmap(int** out, unsigned* w, unsigned* h, char *path)
{
    JNIEnv *env;
    g_jvm->GetEnv((void**)&env, JNI_VERSION_1_6);

    jstring name = env->NewStringUTF(path);
    jobject png = env->CallObjectMethod(g_objMainActivity, g_open, name);
    env->DeleteLocalRef(name);
    jobject gpng = env->NewGlobalRef(png);

    // Get image dimensions
    int width = env->CallIntMethod(g_objMainActivity, g_getWidth, png);
    int height = env->CallIntMethod(g_objMainActivity, g_getHeight, png);

    // Get pixels
    jintArray array = env->NewIntArray(width * height);
    jobject garray = env->NewGlobalRef(array);
    env->CallVoidMethod(g_objMainActivity, g_getPixels, png, garray);

    jint *pixels = env->GetIntArrayElements((jintArray)garray, 0);

    char bytes[4];
    char bytesOut[4];

    for (int i = 0; i < width * height; i++)
    {
        int a = pixels[i];

        bytes[3] = a & 0xff;
        bytes[2] = (a >> 8) & 0xff;
        bytes[1] = (a >> 16) & 0xff;
        bytes[0] = (a >> 24) & 0xff;

        bytesOut[0] = bytes[0];
        bytesOut[1] = bytes[3];
        bytesOut[2] = bytes[2];
        bytesOut[3] = bytes[1];

        int b = int((unsigned char)(bytesOut[0]) << 24 |
        (unsigned char)(bytesOut[1]) << 16 |
        (unsigned char)(bytesOut[2]) << 8 |
        (unsigned char)(bytesOut[3]));

        pixels[i] = b;
    }

    *out = pixels;
    *w = width;
    *h = height;

    env->ReleaseIntArrayElements((jintArray)garray, pixels, 0);
    env->DeleteGlobalRef(garray);

    // Free image
    env->CallVoidMethod(g_objMainActivity, g_close, png);
    env->DeleteGlobalRef(gpng);
}

void PLAT_AssetOpen(char *filename)
{
    JNIEnv *env;
    g_jvm->GetEnv((void**)&env, JNI_VERSION_1_6);

    jstring jfilename = env->NewStringUTF(filename);
    env->CallVoidMethod(g_objMainActivity, g_assetOpen, jfilename);
    env->DeleteLocalRef(jfilename);
}

char PLAT_AssetReadByte()
{
    JNIEnv *env;
    g_jvm->GetEnv((void**)&env, JNI_VERSION_1_6);

    return env->CallCharMethod(g_objMainActivity, g_assetReadByte);
}

int PLAT_AssetReadInt()
{
    JNIEnv *env;
    g_jvm->GetEnv((void**)&env, JNI_VERSION_1_6);

    return env->CallIntMethod(g_objMainActivity, g_assetReadInt);
}

int PLAT_AssetReadFloat()
{
    JNIEnv *env;
    g_jvm->GetEnv((void**)&env, JNI_VERSION_1_6);

    return env->CallFloatMethod(g_objMainActivity, g_assetReadFloat);
}

jobject PLAT_GetAssetManager()
{
    JNIEnv *env;
    g_jvm->GetEnv((void**)&env, JNI_VERSION_1_6);

    return env->CallObjectMethod(g_objMainActivity, g_getAssetManager);
}

JNIEnv *PLAT_GetEnv()
{
    JNIEnv *env;
    g_jvm->GetEnv((void**)&env, JNI_VERSION_1_6);

    return env;
}

int PLAT_LoadSound(char *filename)
{
    JNIEnv *env;
    g_jvm->GetEnv((void**)&env, JNI_VERSION_1_6);

    jstring jfilename = env->NewStringUTF(filename);
    int sound = env->CallIntMethod(g_objMainActivity, g_loadSound, jfilename);
    env->DeleteLocalRef(jfilename);

    return sound;
}

int PLAT_PlaySound(int idx)
{
    JNIEnv *env;
    g_jvm->GetEnv((void**)&env, JNI_VERSION_1_6);

    return env->CallIntMethod(g_objMainActivity, g_playSound, idx);
}

void PLAT_StopSound(int stream)
{
    JNIEnv *env;
    g_jvm->GetEnv((void**)&env, JNI_VERSION_1_6);

    env->CallVoidMethod(g_objMainActivity, g_stopSound, stream);
}

void PLAT_PlayTrack(char *filename)
{
    JNIEnv *env;
    g_jvm->GetEnv((void**)&env, JNI_VERSION_1_6);

    jstring jfilename = env->NewStringUTF(filename);
    env->CallVoidMethod(g_objMainActivity, g_playTrack, jfilename);
    env->DeleteLocalRef(jfilename);
}

void PLAT_StopTrack()
{
    JNIEnv *env;
    g_jvm->GetEnv((void**)&env, JNI_VERSION_1_6);

    env->CallVoidMethod(g_objMainActivity, g_stopTrack);
}

int PLAT_GetWindowWidth()
{
    return 1024;
}

int PLAT_GetWindowHeight()
{
    return 768;
}

int PLAT_GetBytesRead()
{
    return 0;
}
