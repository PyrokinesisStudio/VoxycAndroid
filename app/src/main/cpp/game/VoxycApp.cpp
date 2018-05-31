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

#include "VoxycApp.h"
#include "../DDLUtils.hpp"

void VoxycApp::init()
{
	Log("Voxyc");
	Log("*");
	Log("*");
	Log("*");
	
    g_engine2 = &engine;
	
	switch (module)
	{
		case 0:
			editor.init();
			break;
	    case 10:
			luaProgram.init();
			break;
	}
}

void VoxycApp::load()
{
	g_engine2 = &engine;

	switch (module)
	{
		case 0:
			editor.load();
			break;
		case 10:
			luaProgram.load();
			break;
	}
}

void VoxycApp::tick()
{
	if (loadingModule)
	{
		load();
		loadingModule = false;
		return;
	}
	
	switch (module)
	{
	case 0:
		editor.tick();
		break;
	case 10:
		luaProgram.tick();
		break;
	}
	
	engine.tick();
	
	// Switch module
	int switchModule = engine.getExtraInt("switchmodule");
	int nextModule = engine.getExtraInt("nextmodule");
	
	if (switchModule == 1)
	{
//		Log("nextmodule", nextModule);
		lastModule = module;
		module = nextModule;
		engine.clear();
		init();
//4		load();
		engine.setExtraInt("switchmodule", 0);
		loadingModule = true;
		skipFrame = true;
	}	
}
	
void VoxycApp::draw(int eye)
{
	if (skipFrame)
	{
		skipFrame = false;
		return;
	}
	
	engine.draw(eye);

//#ifdef PLATFORM_ANDROID
	calcFrameRate();
    makeUpLostFramesOrWait();
//#endif

//    Log("fps", fps);
}

void VoxycApp::calcFrameRate()
{
	// Calculate frame rate
	long long curTime = getTimeInMsec();

	if (lastFinishDrawTime != -1)
	{
		long long timeDiff = curTime - lastFinishDrawTime;

		fps = 0;

		if (timeDiff != 0)
			fps = 1000 / timeDiff;

		//	Log("FPS", timeDiff);
		//	Log("timeDiff", timeDiff);
	}

	lastFinishDrawTime = getTimeInMsec();
}

void VoxycApp::makeUpLostFramesOrWait()
{
	// Make up for lost frames

	const float goalTicksPerSec = 60;
	long long curTime = getTimeInMsec();
	static float totalMakeUpRatio = 0.0f;
	long long timeDiff = 0;
	float makeUpTicks = 0;

	if (lastFinishTickTime != -1)
	{
		timeDiff = curTime - lastFinishTickTime;

		float tps = 1000.0f / timeDiff;

		makeUpTicks = goalTicksPerSec - tps;

		float makeUpRatio = 0;

		if (makeUpTicks > 0)
		{
			float lagRatio = tps / goalTicksPerSec;
			makeUpRatio = 1 - lagRatio;
		}

		totalMakeUpRatio += makeUpRatio;
	}

	if (totalMakeUpRatio >= 1.0f)
	{
		tick();
		totalMakeUpRatio = 0.0f;
	}

	// Slow down if needed
	if (timeDiff < 16)
	{
		ddlsleep(abs(16 - timeDiff));
	}
}

void VoxycApp::free()
{
	engine.free();
}

