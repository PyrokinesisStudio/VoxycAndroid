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

#ifndef OVR_SDK_MOBILE_1_0_0_0_GVR_H
#define OVR_SDK_MOBILE_1_0_0_0_GVR_H

extern jclass g_objMainActivity;
extern jclass g_classMainActivity;
extern jmethodID g_open;
extern jmethodID g_getWidth;
extern jmethodID g_getHeight;
extern jmethodID g_getPixels;
extern jmethodID g_close;
extern jmethodID g_assetOpen;
extern jmethodID g_assetReadByte;
extern jmethodID g_assetReadInt;
extern jmethodID g_assetReadFloat;
extern jmethodID g_getAssetManager;
extern jmethodID g_loadSound;
extern jmethodID g_playSound;
extern jmethodID g_stopSound;
extern jmethodID g_playTrack;
extern jmethodID g_stopTrack;

#endif //OVR_SDK_MOBILE_1_0_0_0_GVR_H
