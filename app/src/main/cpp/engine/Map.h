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

#ifndef Map_h
#define Map_h

#include "../Globals.hpp"
#include "GLIncludes.h"
#include "Renderer.h"
#include "TextureManager2.h"

class Map: public Renderer
{
public:

	//
	// VERTEX SHADER SHADOW MAP DESKTOP
	//

	const char *vertexShaderCodeDesktop =

		"precision highp float;" \

		"layout(location = 0) in vec4 vPosition;" \
		"uniform mat4 mvMatrix; " \
		"uniform mat4 projectionMatrix; " \
		"attribute vec2 vTexCoords;" \
		"varying lowp vec2 vTexCoordsOut; " \

		"void main() {" \
		"  gl_Position = projectionMatrix * mvMatrix * vPosition; "
		"  vTexCoordsOut = vTexCoords; " \
		"}\n";

	//
	// FRAGMENT SHADER SHADOW MAP DESKTOP
	//

	const char *fragmentShaderCodeDesktop =

		"precision highp float;" \

		"uniform sampler2D uTexture; " \
		"uniform lowp vec4 vColor; " \
		"varying lowp vec2 vTexCoordsOut; " \

		"void main() {" \
		"      gl_FragColor = texture(uTexture, vTexCoordsOut.st); " \
		"}\n";

	void init(TextureManager2 *texMan);
	void tick();
	void draw();
	void bind();
	void unbind();
	void loadVertices();
	void freeVertices();
	void checkGLError(char *tag);

	GLuint getRenderedTexture() { return renderedTexture; };
	glm::mat4 getProjectionMatrix() { return glm::ortho<float>(-100, 100, -100, 100, -100, 200); };

private:
	GLuint renderedTexture = -1;
	GLuint FramebufferName = -1;

	int programMain = -1;
	int vao = -1;
	int vbo = -1;

	TextureManager2 *texMan;
};

#endif