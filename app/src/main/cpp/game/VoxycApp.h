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

#ifndef FatelessApp_h
#define FatelessApp_h

#include <stdio.h>
#include "Editor.h"
#include "../engine/Engine2.h"
#include "LuaProgram.h"

class VoxycApp
{
public:
    void init();
	void load();
    void tick();
	void draw(int eye);
	void calcFrameRate();
	void makeUpLostFramesOrWait();
	void free();

private:
    Engine2 engine;
 	  
	int module = 0; // 10
	int lastModule = module;

	Editor editor;
	LuaProgram luaProgram;

	long long lastFinishDrawTime = 0.0;
	long long lastFinishTickTime = 0.0;
	float fps;
	
	bool skipFrame = false;
	bool loadingModule = false;
};

#endif
