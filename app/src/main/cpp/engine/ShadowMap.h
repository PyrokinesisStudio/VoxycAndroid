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

#ifndef ShadowMap_h
#define ShadowMap_h

#include "../Globals.hpp"
#include "GLIncludes.h"
#include "Camera.h"

#define PROJMATSIZE 500

class ShadowMap
{
public:

	//
	// VERTEX SHADER SHADOW MAP DESKTOP
	//

	const char *vertexShaderCodeShadowMapDesktop =

		"precision highp float;" \

		"layout(location = 0) in vec4 vPosition;" \
		"uniform mat4 mvMatrix; " \
		"uniform mat4 projectionMatrix; " \

		"void main() {" \
		"  gl_Position = projectionMatrix * mvMatrix * vPosition; "
		"}\n";

	//
	// FRAGMENT SHADER SHADOW MAP DESKTOP
	//

	const char *fragmentShaderCodeShadowMapDesktop =

		"precision highp float;" \

		"in vec4 gl_FragCoord; " \
		"layout(location = 0) out float fragmentDepth; " \

		"void main() {" \
		"      fragmentDepth = gl_FragCoord.z; " \
		"}\n";

	void bind(Camera *camera);
	void unbind();
	void checkGLError(char *tag);

	GLuint getRenderedTexture() { return depthTexture; };
	glm::mat4 getProjectionMatrix();

private:
	GLuint depthTexture = -1;
	GLuint FramebufferName = -1;

	Camera *camera = nullptr;
};

#endif