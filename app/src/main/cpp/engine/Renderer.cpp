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

#include "Renderer.h"
#include "../DDLUtils.hpp"

void Renderer::setMatrix(int program, char *name, glm::mat4 matrix)
{
	float arr[16];

	const float *p = (const float *)glm::value_ptr(matrix);
	for (int i = 0; i < 16; i++)
		arr[i] = p[i];

	int handle = glGetUniformLocation(program, name);
	checkGLError("glGetUniformLocation");

	if (handle == -1)
	{
		Log(name, "is -1");
		return;
	}

	if (handle != -1)
	{
		glUniformMatrix4fv(handle, 1, false, arr);
		checkGLError("glUniformMatrix4fv");
	}
}

void Renderer::setUniform1f(int program, char *name, float x)
{
	int handle = glGetUniformLocation(program, name);
	checkGLError("glGetUniformLocation");

	if (handle == -1)
	{
		Log(name, "is -1");
		return;
	}

	glUniform1f(handle, x);
	checkGLError("glUniform1f");
}

void Renderer::setUniform2f(int program, char *name, float x, float y)
{
	int handle = glGetUniformLocation(program, name);
	checkGLError("glGetUniformLocation");

	if (handle == -1)
	{
		Log(name, "is -1");
		return;
	}

	glUniform2f(handle, x, y);
	checkGLError("glUniform2f");
}

void Renderer::setUniform4f(int program, char *name, float x, float y, float z, float w)
{
	int handle = glGetUniformLocation(program, name);
	checkGLError("glGetUniformLocation");

	if (handle == -1)
	{
		Log(name, "is -1");
		return;
	}

	glUniform4f(handle, x, y, z, w);
	checkGLError("glUniform4f");
}

void Renderer::setVertexAttrib(int program, char *name, int size, int type, bool normalized, int stride, int pointer)
{
	GLint handle = glGetAttribLocation(program, name);
	checkGLError("glGetAttribLocation");

	if (handle == -1)
	{
		Log(name, "is -1");
		return;
	}

	glEnableVertexAttribArray(handle);
	checkGLError("glEnableVertexAttribArray");

	glVertexAttribPointer(handle, size,
		type, normalized,
		stride, BUFFER_OFFSET(pointer * sizeof(float)));
	checkGLError("glVertexAttribPointer");
}

int Renderer::loadProgram(char *vertexShaderCode, char *fragmentShaderCode, bool shadowMap)
{
	int program = -1;
	int vertexShader;
	int fragmentShader;

	vertexShader = loadShader(GL_VERTEX_SHADER, vertexShaderCode);
	fragmentShader = loadShader(GL_FRAGMENT_SHADER, fragmentShaderCode);

	program = glCreateProgram();             // Create empty OpenGL ES Program
	checkGLError("glCreateProgram");
	glAttachShader(program, vertexShader);   // Add the vertex shader to program
	checkGLError("glAttachShader");
	glAttachShader(program, fragmentShader); // Add the fragment shader to program
	checkGLError("glAttachShader");

#if defined PLATFORM_OSX || defined PLATFORM_WINDOWS || defined PLATFORM_OPENVR
	/*	if (shadowMap)
	{
	glBindFragDataLocation(program, 0, "fragmentDepth");
	checkGLError("glBindFragDataLocation");
	}
	else
	{
	glBindFragDataLocation(program, 0, "finalColor");
	checkGLError("glBindFragDataLocation");
	}*/
#endif

	glLinkProgram(program);                  // Creates OpenGL ES program executables

											 // Throw exception if linking failed
	GLint status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE) {
		Log("Program linking failure: ");

		GLint infoLogLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* strInfoLog = new char[infoLogLength + 1];
		glGetProgramInfoLog(program, infoLogLength, NULL, strInfoLog);
		Log(strInfoLog);
		delete[] strInfoLog;

		glDeleteProgram(program);
		program = 0;
	}

	return program;
}

int Renderer::loadShader(int type, const char *shaderCode)
{
	int shader = glCreateShader(type);
	checkGLError("glCreateShader");

	// Add the source code to the shader and compile it
	char const *codeArr[1];
	codeArr[0] = shaderCode;
	glShaderSource(shader, 1, codeArr, NULL);
	checkGLError("glShaderSource");
	glCompileShader(shader);
	checkGLError("glShaderSource");

	GLint compile_ok = GL_FALSE;

	glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_ok);
	checkGLError("glGetShaderiv");

	if (compile_ok == GL_FALSE)
	{
		Log("Renderer shader compile error; type = ", type);
		char *errorMsg = (char *)malloc(MAX_STR_LEN);
		GLsizei len;
		glGetShaderInfoLog(shader, MAX_STR_LEN, &len, errorMsg);
		Log(errorMsg);
		Log("len =", len);
		free(errorMsg);
	}

	return shader;
}

void Renderer::setGlobalColor(glm::vec4 globalColor)
{
	globalColor = globalColor;
	targetGlobalColor = globalColor;
	fadeTimer = 0;
	fadeLength = 0;
}

void Renderer::fadeGlobalColor(glm::vec4 newGlobalColor, int ticks)
{
	lastGlobalColor = this->globalColor;
	targetGlobalColor = newGlobalColor;
	fadeTimer = ticks;
	fadeLength = ticks;
}

void Renderer::tick()
{
	if (fadeTimer > 0)
		fadeTimer--;

	if (fadeLength > 0)
	{
		float ratio = (float)fadeTimer / (float)fadeLength;

		if (globalColor != targetGlobalColor)
		{
			glm::vec4 diff = targetGlobalColor - globalColor;

			globalColor = lastGlobalColor + (diff * (1.0f - ratio));
		}
	}
}

void Renderer::checkGLError(char *tag)
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
