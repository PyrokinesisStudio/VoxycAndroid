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

#include "../Globals.hpp"
#include "GLIncludes.h"
#include "TextureManager2.h"
#include "../platform.h"
#include "../DDLUtils.hpp"
#include "../lodepng.hpp"

Texture *TextureManager2::find(std::string name)
{
	Texture *tex = textures[name];

	if (tex == nullptr)
	{
		load(name);
		tex = find(name);
	}

	if (tex != nullptr)
		return tex;
	else
	    return nullptr;
}

void TextureManager2::load(std::string name, bool external)
{
//	Log(name);
	
    Texture *t = new Texture();
    t->name = name;

    int glTexID;

    glGenTextures(1, (GLuint *)&glTexID);

#ifdef PLATFORM_ANDROID
#ifdef USE_EXTERNAL_ASSETS
    PLAT_LoadTextureInJava(glTexID, (std::string)EXTERNAL_PROJECT_DIR + "/" + name, true);
#else
    PLAT_LoadTextureInJava(glTexID, name, external);
#endif
#endif

#if defined PLATFORM_OSX || defined PLATFORM_WINDOWS || defined PLATFORM_OPENVR
	glGenTextures(1, (GLuint *)&glTexID);

	char fullFilename[MAX_STR_LEN];

#ifdef USE_EXTERNAL_ASSETS
	printFullExternalResourceFilename((char *)name.c_str(), fullFilename);
#else
	printFullResourceFilename((char *)name.c_str(), fullFilename);
#endif

	// Load file and decode image.
	std::vector<unsigned char> image;
	unsigned width, height;
	unsigned error = lodepng::decode(image, width, height, fullFilename);

	// If there's an error, display it.
	if (error != 0)
	{
		Log("Error loading PNG: ", (char *)lodepng_error_text(error));

		printFullResourceFilename("bluecube.png", fullFilename);

		unsigned error = lodepng::decode(image, width, height, fullFilename);
	}

	glBindTexture(GL_TEXTURE_2D, glTexID);
	checkGLError("glBindTexture");
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	checkGLError("glBindTexture");
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	checkGLError("glBindTexture");

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);
	checkGLError("glTexImage2D");
#endif

    t->glTexID = glTexID;

	textures[name] = t;
}

void TextureManager2::free()
{
	for(const auto &pair: textures)
	{
		Texture *tex = pair.second;
        glDeleteTextures(1, (GLuint *)&tex->glTexID);
        checkGLError("glDeleteTextures");
    }
}

void TextureManager2::checkGLError(char *tag)
{
#ifdef USE_OPENGL
	GLenum err = glGetError();

	switch (err)
	{
	case GL_NO_ERROR:
		break;
	case GL_INVALID_ENUM:
		Log("GL error GL_INVALID_ENUM", tag);
		break;
	case GL_INVALID_VALUE:
		Log("GL error GL_INVALID_VALUE", tag);
		break;
	case GL_INVALID_OPERATION:
		Log("GL error GL_INVALID_OPERATION", tag);
		break;
#ifndef USE_OPENGL_3
	case GL_INVALID_FRAMEBUFFER_OPERATION:
		Log("GL error GL_INVALID_FRAMEBUFFER_OPERATION", str);
		break;
#endif
	case GL_OUT_OF_MEMORY:
		Log("GL error GL_OUT_OF_MEMORY", tag);
		break;
	}
#endif
}
