# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

add_library( # Sets the name of the library.
             native-lib

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).
             src/main/cpp/native-lib.cpp )

add_library( # Sets the name of the library.
             hello-jni

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).
             # Associated headers in the same location as their source
             # file are automatically included.
             src/main/cpp/android.cpp
             src/main/cpp/DDLUtils.cpp
             src/main/cpp/Globals.cpp
             src/main/cpp/javagl.cpp
             src/main/cpp/lodepng.cpp
             src/main/cpp/main.cpp
             src/main/cpp/platform.cpp
             src/main/cpp/engine/Audio.cpp
             src/main/cpp/engine/AndroidAudio.cpp
             src/main/cpp/engine/BuildingWall.cpp
             src/main/cpp/engine/Camera.cpp
             src/main/cpp/engine/Controls2.cpp
             src/main/cpp/engine/Engine2.cpp
             src/main/cpp/engine/Map.cpp
             src/main/cpp/engine/Model2.cpp
             src/main/cpp/engine/ModelRenderer.cpp
             src/main/cpp/engine/Object.cpp
             src/main/cpp/engine/OzzRenderer.cpp
             src/main/cpp/engine/Player.cpp
             src/main/cpp/engine/Renderer.cpp
             src/main/cpp/engine/ShadowMap.cpp
             src/main/cpp/engine/Shape.cpp
             src/main/cpp/engine/ShapeRenderer.cpp
             src/main/cpp/engine/SkyboxRenderer.cpp
             src/main/cpp/engine/Terrain.cpp
             src/main/cpp/engine/TextureManager2.cpp
             src/main/cpp/engine/Vehicle.cpp
             src/main/cpp/engine/SpriteRenderer2D.cpp
             src/main/cpp/engine/TextPrinter.cpp
             src/main/cpp/engine/LuaBridge.cpp
             src/main/cpp/engine/Voxels.cpp
             src/main/cpp/engine/EditorController.cpp
             src/main/cpp/engine/GUI.cpp
             src/main/cpp/engine/OBJWriter.cpp
             src/main/cpp/game/VoxycApp.cpp
             src/main/cpp/game/LuaProgram.cpp
             src/main/cpp/game/Editor.cpp
             src/main/cpp/lua/lapi.c
             src/main/cpp/lua/lauxlib.c
             src/main/cpp/lua/lbaselib.c
             src/main/cpp/lua/lbitlib.c
             src/main/cpp/lua/lcode.c
             src/main/cpp/lua/lcorolib.c
             src/main/cpp/lua/lctype.c
             src/main/cpp/lua/ldblib.c
             src/main/cpp/lua/ldebug.c
             src/main/cpp/lua/ldo.c
             src/main/cpp/lua/ldump.c
             src/main/cpp/lua/lfunc.c
             src/main/cpp/lua/lgc.c
             src/main/cpp/lua/linit.c
             src/main/cpp/lua/liolib.c
             src/main/cpp/lua/llex.c
             src/main/cpp/lua/lmathlib.c
             src/main/cpp/lua/lmem.c
             src/main/cpp/lua/loadlib.c
             src/main/cpp/lua/lobject.c
             src/main/cpp/lua/lopcodes.c
             src/main/cpp/lua/loslib.c
             src/main/cpp/lua/lparser.c
             src/main/cpp/lua/lstate.c
             src/main/cpp/lua/lstring.c
             src/main/cpp/lua/lstrlib.c
             src/main/cpp/lua/ltable.c
             src/main/cpp/lua/ltablib.c
             src/main/cpp/lua/ltm.c
             src/main/cpp/lua/lua.c
             src/main/cpp/lua/luac.c
             src/main/cpp/lua/lundump.c
             src/main/cpp/lua/lutf8lib.c
             src/main/cpp/lua/lvm.c
             src/main/cpp/lua/lzio.c
             )

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )

# ASSIMP

add_library( assimp
             SHARED
             IMPORTED )

set_target_properties( # Specifies the target library.
                       assimp

                       # Specifies the parameter you want to define.
                       PROPERTIES IMPORTED_LOCATION

                       # Provides the path to the library you want to import.
                       ${PROJECT_SOURCE_DIR}/src/main/jniLibs/${ANDROID_ABI}/libassimp.so )

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
                       hello-jni

                       # Links the target library to the log library
                       # included in the NDK.
                       ${log-lib}
                       assimp
                       )