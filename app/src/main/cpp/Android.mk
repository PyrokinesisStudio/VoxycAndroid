# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
LOCAL_PATH := $(call my-dir)

ifneq ($(TARGET_ARCH_ABI),x86)

include $(CLEAR_VARS)

LOCAL_CPP_EXTENSION := .cpp .cc
LOCAL_MODULE    := hello-jni
LOCAL_SRC_FILES := lua/lapi.c \
lua/lauxlib.c \
lua/lbaselib.c \
lua/lbitlib.c \
lua/lcode.c \
lua/lcorolib.c \
lua/lctype.c \
lua/ldblib.c \
lua/ldebug.c \
lua/ldo.c \
lua/ldump.c \
lua/lfunc.c \
lua/lgc.c \
lua/linit.c \
lua/liolib.c \
lua/llex.c \
lua/lmathlib.c \
lua/lmem.c \
lua/loadlib.c \
lua/lobject.c \
lua/lopcodes.c \
lua/loslib.c \
lua/lparser.c \
lua/lstate.c \
lua/lstring.c \
lua/lstrlib.c \
lua/ltable.c \
lua/ltablib.c \
lua/ltm.c \
lua/lua.c \
lua/luac.c \
lua/lundump.c \
lua/lutf8lib.c \
lua/lvm.c \
lua/lzio.c \
hello-jni.cpp Globals.cpp main.cpp platform.cpp android.cpp javagl.cpp \
Audio.cpp AndroidAudio.cpp \
DDLUtils.cpp \
engine/Engine2.cpp \
engine/Object.cpp \
engine/ShapeRenderer.cpp \
engine/TextureManager2.cpp \
engine/Shape.cpp \
engine/Terrain.cpp \
engine/Controls2.cpp \
engine/SkyboxRenderer.cpp \
engine/BuildingWall.cpp \
engine/Model2.cpp \
engine/ModelRenderer.cpp \
engine/ShadowMap.cpp \
engine/Vehicle.cpp \
engine/Player.cpp \
engine/Camera.cpp \
engine/Renderer.cpp \
engine/Map.cpp \
engine/SpriteRenderer2D.cpp \
engine/TextPrinter.cpp \
engine/LuaBridge.cpp \
engine/Voxels.cpp \
engine/EditorController.cpp \
engine/GUI.cpp \
editor/EditorApp.cpp \
game/FatelessApp.cpp \
game/Warehouses.cpp \
game/Airport.cpp \
game/FlightSim.cpp \
game/SpacePort.cpp \
game/ShadowMapTest.cpp \
game/TrainStation.cpp \
game/Brooklyn.cpp \
game/Boats.cpp \
game/UFOShooter.cpp \
game/Editor.cpp \
game/LuaProgram.cpp

LOCAL_SHARED_LIBRARIES	:= android assimp squirrel
#LOCAL_SHARED_LIBRARIES	:= assimp GLESv3 EGL android
LOCAL_CFLAGS := -fpermissive -std=c++11 -lm
#LOCAL_ALLOW_UNDEFINED_SYMBOLS := true
LOCAL_LDLIBS := -L$(SYSROOT)/usr/lib -llog

ifeq ($(TARGET_ARCH_ABI),x86)
    LOCAL_CFLAGS += -ffast-math -mtune=atom -mssse3 -mfpmath=sse
endif

include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libassimp
LOCAL_SRC_FILES := libassimp.so
include $(PREBUILT_SHARED_LIBRARY)

#include $(CLEAR_VARS)
#LOCAL_MODULE := libGLESv3
#LOCAL_SRC_FILES := libGLESv3.so
#include $(PREBUILT_SHARED_LIBRARY)

#include $(CLEAR_VARS)
#LOCAL_MODULE := libEGL
#LOCAL_SRC_FILES := libEGL.so
#include $(PREBUILT_SHARED_LIBRARY)

#include $(CLEAR_VARS)
#LOCAL_MODULE := libandroid
#LOCAL_SRC_FILES := libandroid.so
#include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)

#LOCAL_CPP_EXTENSION := .cpp
#LOCAL_MODULE    := squirrel
#LOCAL_C_INCLUDES := $(LOCAL_PATH)/squirrel/
#LOCAL_SRC_FILES := squirrel/sqapi.cpp \
#squirrel/sqbaselib.cpp \
#squirrel/sqclass.cpp \
#squirrel/sqcompiler.cpp \
#squirrel/sqdebug.cpp \
#squirrel/sqfuncstate.cpp \
#squirrel/sqlexer.cpp \
#squirrel/sqmem.cpp \
#squirrel/sqobject.cpp \
#squirrel/sqstate.cpp \
#squirrel/sqtable.cpp \
#squirrel/sqvm.cpp
#LOCAL_SHARED_LIBRARIES := math
#LOCAL_STATIC_LIBRARIES := libm
#LOCAL_LDLIBS := -lm
#LOCAL_CFLAGS := -lm
#LOCAL_LDLIBS := -lGLESv1_CM -ldl -llog

#ifeq ($(TARGET_ARCH_ABI),x86)
#    LOCAL_CFLAGS += -ffast-math -mtune=atom -mssse3 -mfpmath=sse
#endif

#include $(BUILD_SHARED_LIBRARY)

endif

