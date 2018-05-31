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

#ifndef Camera_h
#define Camera_h

#include "../Globals.hpp"
#include "Object.h"
#include "Player.h"
#include "Vehicle.h"

typedef enum CameraSchemes
{
	CAMERA_FPS,
	CAMERA_UFOSHOOTER,
	CAMERA_UFOSHOOTER_360,
	CAMERA_UFOSHOOTER_VR
};

class Camera: public Object
{
public:
	void init(Player *player, Vehicle *vehicle, Object *mouseLook);
	void tick();
	void setCameraScheme(CameraSchemes scheme);
	void setPlayer(Player *player);
	void setPlayerObj(Object *playerObj);

private:
#ifdef USE_PLAYEROBJ
    Object *playerObj;
#else
	Player *player;
#endif
	Vehicle *vehicle;
	Object *mouseLook;
	CameraSchemes cameraScheme;
};

#endif
