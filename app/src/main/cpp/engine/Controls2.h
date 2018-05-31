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

#ifndef FATELESS_CONTROLS2_H
#define FATELESS_CONTROLS2_H

#include "../Globals.hpp"
#include "Object.h"
#include "Player.h"
#include "Vehicle.h"
#include "SpriteRenderer2D.hpp"
#include "TextureManager2.h"
#include "EditorController.h"

typedef enum ControlSchemes
{
	CTRL_FPS,
	CTRL_UFOSHOOTER,
	CTRL_UFOSHOOTER_360,
	CTRL_SCROLLINGSHOOTER,
	CTRL_EDITOR
};

class Controls2
{
public:
    void init(Object *camera, Player *player, Vehicle *vehicle, Object *mouseLook, TextureManager2 *texMan, EditorController *editor);
    void tick();
    void touchEvent(int count, int action1, float x1, float y1, int action2, float x2, float y2);
//    void touchEvent(int action, float x, float y, int finger, int count);
    void ArrowDown(float factor = 1.00);
    void ArrowUp(float factor = 1.00);
    void ArrowLeft(float factor = 1.00);
    void ArrowRight(float factor = 1.00);
	void MoveUp(float factor = 1.00);
	void MoveDown(float factor = 1.00);
	void MoveYaw(float amount);
    void MovePitch(float amount);
	void increaseThrottle();
	void decreaseThrottle();
	void rudderLeft();
	void rudderRight();
	float getMoveFactor();
	void setControlScheme(ControlSchemes scheme);
	void BtnDown(int which);
	void BtnUp(int which);
	void minus();
	void equals();
	void plus();
	void Draw2D(SpriteRenderer2D *r);
	float scrToGlX(float screenX);
	float scrToGlY(float screenY);
	float glToScrX(float glX);
	float glToScrY(float glY);
    void setTriggerAlwaysOn(bool value);
	void setEnabled(bool enabled);
	void moveForwardToYaw(float yaw, float factor);
	void setFireYaw(float yaw);
	float getFireYaw() { return fireYaw; };
	void setFireTriggerDown(bool down);
	void setPlayerObj(Object *playerObj);
	void setPlayerRangeX(float min, float max);
	float getMinPlayerX() { return minPlayerX; };
	float getMaxPlayerX() { return maxPlayerX; };
	void tap(); 
	void tap2();
	void save();
	void load();
	bool checkActionUp();

private:
	const char *defaultVoxelsFname = "C:/Users/dimit/voxels.vx";

    Object *camera;
#ifdef USE_PLAYEROBJ
    Object *playerObj;
#else
	Player *player;
#endif
	Vehicle *vehicle;
	Object *mouseLook;
    TextureManager2 *texMan;
	EditorController *editorController;

    int screenWidth = 0;
    int screenHeight = 0;

	bool jdown = false;
	float jlastdownx = 0;
    float jlastdowny = 0;
    float jlastmovex = 0;
    float jlastmovey = 0;
    bool ljdown = false;
    float rjlastdownx = 0;
    float rjlastdowny = 0;
    float rjlastmovex = 0;
    float rjlastmovey = 0;
    bool rjdown = false;
    float ljlastdownx = 0;
    float ljlastdowny = 0;
    float ljlastmovex = 0;
    float ljlastmovey = 0;

	float minPlayerX = 0.0;
	float maxPlayerX = 0.0;

	ControlSchemes controlScheme = CTRL_FPS;

    bool triggerAlwaysOn = false;
	bool enabled = true;

	float fireYaw = 0.0;
	
	int afterGestureTimer = 0;
	bool actionUp = false;
};

#endif //FATELESS_CONTROLS2_H
