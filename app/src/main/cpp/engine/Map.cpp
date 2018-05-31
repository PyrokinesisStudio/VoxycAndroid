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

#include "Map.h"
#include "GLIncludes.h"
#include "../platform.h"
#include "../DDLUtils.hpp"

void Map::init(TextureManager2 *texMan)
{
	this->texMan = texMan;

	const int len = 10000;

	char vertexShaderStr[len];
	char fragmentShaderStr[len];

#ifdef PLATFORM_WINDOWS
	// Compile shadow map program
	sprintf_s(vertexShaderStr, len, "#version 330 core\n%s", vertexShaderCodeDesktop);
	sprintf_s(fragmentShaderStr, len, "#version 330 core\n%s", fragmentShaderCodeDesktop);

	programMain = loadProgram(vertexShaderStr, fragmentShaderStr, true);
#endif

	loadVertices();
	draw();
}

void Map::tick()
{

}

void Map::draw()
{
	float width = PLAT_GetWindowWidth();
	float height = PLAT_GetWindowHeight();
	float aspect = width / height;

	bind();

	int numCoords = 0;
	int stride = 6 * sizeof(float);
	numCoords = 6 * 6;

	glEnable(GL_CULL_FACE);
	checkGLError("glEnable");
	glFrontFace(GL_CW);
	checkGLError("glFrontFace");
	glCullFace(GL_BACK);
	checkGLError("glCullFace");

	// Add program to OpenGL environment
	glUseProgram(programMain);
	checkGLError("glUseProgram");

	// Bind the VAO
	glBindVertexArray(vao);
	checkGLError("glBindVertexArray");

	// Bind the VBO
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	checkGLError("glBindBuffer");

	// Set the matrices
	glm::mat4 mvMatrix;
	glm::mat4 projectionMatrix;

	mvMatrix = glm::mat4();

	setMatrix(programMain, "mvMatrix", mvMatrix);

	setMatrix(programMain, "projectionMatrix", glm::perspective(VIEW_FOV, aspect, 0.001f, 1000.0f));

	// Set variables
	setUniform4f(programMain, "vColor", 1.0, 1.0, 1.0, 1.0);

	// Set texture
	int glTexID = -1;
	texMan->load("bkgrass1.png");
	Texture *tex = texMan->find("bkgrass1.png");

	if (tex != nullptr)
		glTexID = tex->glTexID;

	glActiveTexture(GL_TEXTURE0);
	checkGLError("glActiveTexture");
	glBindTexture(GL_TEXTURE_2D, glTexID);
	checkGLError("glBindTexture");

	// Set texture unit number
	int uTexture = glGetUniformLocation(programMain, "uTexture");
	checkGLError("glGetUniformLocation");
	glUniform1i(uTexture, 0);
	checkGLError("glUniform1i");

	// Set attributes
	setVertexAttrib(programMain, "vPosition", 4, GL_FLOAT, false, stride, 0);
	setVertexAttrib(programMain, "vTexCoords", 2, GL_FLOAT, false, stride, 4);

	// Draw
	glDrawArrays(GL_TRIANGLES, 0, numCoords);
	checkGLError("glDrawArrays");

	unbind();
}

void Map::bind()
{
#ifdef PLATFORM_WINDOWS

	const int size = 1024;

	// The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
	if (FramebufferName == -1);
	{
		glGenFramebuffers(1, &FramebufferName);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);

	// Depth texture. Slower than a depth buffer, but you can sample it later in your shader
	if (renderedTexture == -1);
	{
		glGenTextures(1, &renderedTexture);
	}

	glBindTexture(GL_TEXTURE_2D, renderedTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size, size, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

	// Poor filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// The depth buffer
	GLuint depthrenderbuffer;
	glGenRenderbuffers(1, &depthrenderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, size, size);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);

	// Set "renderedTexture" as our colour attachement #0
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderedTexture, 0);

	//// Depth texture alternative : 
	//glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture, 0);

	// Set the list of draw buffers.
	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

	// Always check that our framebuffer is ok
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		return;

	// Render to our framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
	glViewport(0, 0, size, size); // Render on the whole framebuffer, complete from the lower left corner to the upper right

#endif
}

void Map::unbind()
{
#ifdef PLATFORM_WINDOWS

	float width = PLAT_GetWindowWidth();
	float height = PLAT_GetWindowHeight();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, width, height); // Render on the whole framebuffer, complete from the lower left corner to the upper right}

#endif
}

void Map::loadVertices()
{
	// Generate VAO
	glGenVertexArrays(1, (GLuint *)&vao);
	checkGLError("glGenVertexArrays");
	glBindVertexArray(vao);
	checkGLError("glBindVertexArray");

	// Generate VBOs
	glGenBuffers(1, (GLuint *)&vbo);
	checkGLError("glGenBuffers");

	//
	// SHAPE_QUAD
	//

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	checkGLError("glBindBuffer");

	float quad[36];

	//   ______
	// |\\5   4|
	// |0\\    |
	// |  \\   |
	// |   \\  |
	// |    \\3|
	// |1__2_\\|

	// Triangle 1

	// Vertex 0
	quad[0] = -1.0f;
	quad[1] = 1.0f;
	quad[2] = 0.0f;
	quad[3] = 1.0f;

	quad[4] = 0.0f;
	quad[5] = 0.0f;

	// Vertex 1
	quad[6] = -1.0f;
	quad[7] = -1.0f;
	quad[8] = 0.0f;
	quad[9] = 1.0f;

	quad[10] = 0.0f;
	quad[11] = 1.0f;

	// Vertex 2
	quad[12] = 1.0f;
	quad[13] = -1.0f;
	quad[14] = 0.0f;
	quad[15] = 1.0f;

	quad[16] = 1.0f;
	quad[17] = 1.0f;

	// Triangle 2

	// Vertex 3
	quad[18] = 1.0f;
	quad[19] = -1.0f;
	quad[20] = 0.0f;
	quad[21] = 1.0f;

	quad[22] = 1.0f;
	quad[23] = 1.0f;

	// Vertex 4
	quad[24] = 1.0f;
	quad[25] = 1.0f;
	quad[26] = 0.0f;
	quad[27] = 1.0f;

	quad[28] = 1.0f;
	quad[29] = 0.0f;

	// Vertex 5
	quad[30] = -1.0f;
	quad[31] = 1.0f;
	quad[32] = 0.0f;
	quad[33] = 1.0f;

	quad[34] = 0.0f;
	quad[35] = 0.0f;

	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 36, quad, GL_STATIC_DRAW);
	checkGLError("glBufferData");
}

void Map::freeVertices()
{

}

void Map::checkGLError(char *tag)
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

