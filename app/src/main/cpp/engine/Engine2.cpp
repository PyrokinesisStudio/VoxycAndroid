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

#include "Engine2.h"
#include "../DDLUtils.hpp"
#include <algorithm>

#define CLICK_TIMER 100

Engine2 *g_engine2 = nullptr;

void Engine2::init()
{
	if (initialized)
		return;
	
#if defined PLATFORM_WINDOWS || defined PLATFORM_OSX
//	useShadowMap = false;
	useShadowMap = true;
#else
	useShadowMap = false;
#endif

	skyboxRenderer.init();
	shapeRenderer.init(&shadowMap, useShadowMap, &map, &mouseLook, &vehicle, &texMan);
	modelRenderer.init(&shadowMap, useShadowMap, &mouseLook, &vehicle);
    spriteRenderer2D.Init();
	textPrinter.init(&texMan, &spriteRenderer2D);
    gui.init(&texMan, &spriteRenderer2D, &textPrinter);
	
#ifdef USE_OZZ
	ozzRenderer.init();
#endif

    controls.init(&camera, NULL, &vehicle, &mouseLook, &texMan, &editorController);
	controls.setPlayerObj(playerObj);

	vehicle.init();
#ifndef USE_PLAYEROBJ
	player.init(&vehicle);
#endif
//	map.init(&texMan);
	audio.init();

#ifdef USE_PLAYEROBJ
    camera.init(NULL, &vehicle, &mouseLook);
	camera.setPlayerObj(playerObj);
#else
    camera.init(&player, &vehicle, &mouseLook);
#endif
    camera.position = glm::vec4(0.0, 0.0, 0.0, 0.0);
    camera.yaw = 0.0;
    camera.pitch = 0.0;
    camera.roll = 0.0;
	
	editorController.setPlayerObj(playerObj);

	sun.position = glm::vec4(0.0, 15.0, 15.0, 0.0);
	sun.yaw = 0.0;
	sun.pitch = -45.0;
	sun.roll = 0.0;
	
	initialized = true;
}

void Engine2::tick()
{
//	makeUpLostFramesOrWait();
#ifdef PLATFORM_WINDOWS
	// HACK
	ddlsleep(10);
#endif

    controls.tick();
	vehicle.tick();
#ifndef USE_PLAYEROBJ
	player.tick();
#endif
//	map.tick();
	audio.tick();

	moveObjectsSmoothly();
	moveObjectsByVelocity();
	limitPlayerRange();

	camera.tick();

	skyboxRenderer.tick();
	modelRenderer.tick();
	shapeRenderer.tick();

	editorController.tick();

//	spriteRenderer2D.tick();
}

void Engine2::draw(int eye)
{
	shadowMapReady = false;

	if (useShadowMap && !shadowMapReady)
	{
		drawShadowMap(eye);
		shadowMapReady = true;
	}

    if (skyboxGLTexID != -1)
	    skyboxRenderer.draw(&camera, skyboxGLTexID);

    glEnable(GL_DEPTH_TEST);
	glDepthRange(0.001, 1000.0);
//	glClearDepth(1.0);
    glClear(GL_DEPTH_BUFFER_BIT);
//	glDepthMask(true);

    shapeRenderer.draw(eye, objects, &camera, false, useShadowMap, &shadowMap, voxelTextures);
	modelRenderer.draw(eye, objects, &camera, false, useShadowMap, &shadowMap);

#ifdef USE_OZZ
	ozzRenderer.draw();
#endif

#ifdef PLATFORM_ANDROID
    if (controlsVisible)
	    controls.Draw2D(&spriteRenderer2D);
#endif

	textPrinter.draw();
	gui.draw();

	if (healthBarsVisible)
		drawHealthBars();

//	calcFrameRate();
}

void Engine2::drawShadowMap(int eye)
{
		shadowMap.bind(&camera);

		glClearDepth(1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0, 0.0, 0.0, 1.0);

		shapeRenderer.draw(eye, objects, &sun, true, false, &shadowMap, voxelTextures);
		modelRenderer.draw(eye, objects, &sun, true, false, &shadowMap);

		shadowMap.unbind();
}

void Engine2::drawHealthBars()
{
//	void DrawSprite(float xshift, float yshift, float scalex, float scaley, int glTexID, float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f);

	Texture *tex = texMan.find("bluecube.png");

	const float fullWidth = .4;

	float width1 = fullWidth * healthBar1Value;
	float width2 = fullWidth * healthBar2Value;

	spriteRenderer2D.DrawSprite(-.6, .8, fullWidth + 0.05, .06, tex->glTexID, 0.8, 0.9, 1.2, 0.1);
	spriteRenderer2D.DrawSprite(-.71 - ((fullWidth - width1) / 2.0) + width1 / 3.35, .8, width1, .04, tex->glTexID, 0.8, 0.9, 1.2, 0.5);

	spriteRenderer2D.DrawSprite(.59, .8, fullWidth + 0.05, .06, tex->glTexID, 0.8, 0.9, 1.2, 0.1);
	spriteRenderer2D.DrawSprite(.58 + ((fullWidth - width2) / 2.0) + (fullWidth - width2) / 3.35, .8, width2, .04, tex->glTexID, 0.8, 0.9, 1.2, 0.5);
}

void Engine2::free()
{
	objects.clear();

    for(const auto &pair: shapes)
    {
        Shape *shape = pair.second;
        shape->free();
    }
	
	shapes.clear();

    for(const auto &pair: models)
    {
        Model2 *model = pair.second;
        model->free();
    }

	models.clear();
	
	texMan.free();

	initialized = false;
}

void Engine2::addObject(std::string name)
{
//	Log("addObject " + name);
	
	if (findObj(name) != nullptr)
		return;

    Object *o = new Object();
    o->name = name;
    o->scale = glm::vec3(1.0);
    o->position = glm::vec4(0.0, 0.0, 0.0, 0.0);
	o->velocity = glm::vec4(0.0, 0.0, 0.0, 0.0);

    objects[name] = o;
}

void Engine2::setType(std::string name, ObjType type)
{
//	Log("setType " + name);
	
    Object *o = findObj(name);

    if (o == nullptr)
        return;

    o->type = type;
}

void Engine2::setShape(std::string name, ObjShapeType shape)
{
//	Log("setShape");
//	Log(name.c_str());
 
//	if (shape == SHAPE_QUAD)
//	  Log("setshape quad");
	
    Object *o = findObj(name);

    if (o == nullptr)
        return;

    o->shapeType = shape;
}

void Engine2::setShape(std::string name, std::string shapeName)
{
//	Log("setShape");
	
    Object *o = findObj(name);

    if (o == nullptr)
        return;

    o->shapeType = SHAPE_CUSTOM;

    Shape *s = findShape(shapeName);

    if (s == nullptr)
        return;

    o->shape = s;
}

void Engine2::setModel(std::string name, std::string modelName)
{
//	Log("setModel");
	
    Object *o = findObj(name);

    if (o == nullptr)
        return;

    o->type = OBJTYPE_MODEL;

    Model2 *m = findModel(modelName);

    if (m == nullptr) {
        m = new Model2();
        m->load(modelName, modelRenderer.getVAO());
        models[modelName] = m;
    }

    o->model = m;
}

void Engine2::setColor(std::string name, float r, float g, float b, float a)
{
//	Log("setColor");
	
    Object *o = findObj(name);
	
	if (o == nullptr)
		return;
	
    glm::vec4 color = glm::vec4(r, g, b, a);
    o->color = color;
}

void Engine2::setMeshColor(std::string name, int mesh, float r, float g, float b, float a)
{
//	Log("setMeshColor");
	
	Object *o = findObj(name);
	
	if (o == nullptr)
		return;

	glm::vec4 color = glm::vec4(r, g, b, a);

	if (o->type != OBJTYPE_MODEL)
		return;

	if (o->model == nullptr)
		return;

    if (mesh < o->model->meshes.size())
    	o->model->meshes[mesh]->color = color;
}

void Engine2::setTexture(std::string name, std::string textureName)
{
//	Log("setTexture");
	
    Texture *tex = texMan.find(textureName);

    if (tex == nullptr)
    {
        texMan.load(textureName);
    }

    Object *o = findObj(name);
	
	if (o == nullptr)
		return;

    if (o != nullptr) {
        Texture *t = texMan.find(textureName);

        if (t != nullptr) {
            o->textureName = textureName;
            o->glTexID = t->glTexID;
        }
    }
}

void Engine2::setTextureSpan(std::string name, float textureSpanX, float textureSpanY)
{
//	Log("setTextureSpan");
	
	Object *o = findObj(name);
	
	if (o == nullptr)
		return;

	if (o != nullptr) {
		o->texSpanX = textureSpanX;
		o->texSpanY = textureSpanY;
	}
}

void Engine2::setVisible(std::string name, bool visible)
{
	Object *o = findObj(name);
	
	if (o == nullptr)
		return;

	if (o != nullptr) {
		o->visible = visible;
	}
}

void Engine2::setMapTexture(std::string name)
{
	Object *o = findObj(name);
	
	if (o == nullptr)
		return;

	if (o != nullptr) {
		o->mapTexture = true;
	}
}

void Engine2::setSize(std::string name, float size)
{
    Object *o = findObj(name);
	
	if (o == nullptr)
		return;
	
    o->size = size;
}

void Engine2::setSize(std::string name, float sizex, float sizey, float sizez)
{
    Object *o = findObj(name);
	
	if (o == nullptr)
		return;
	
    glm::vec3 scale = glm::vec3(sizex, sizey, sizez);
    o->scale = scale;
}

void Engine2::setPos(std::string name, float x, float y, float z)
{
    Object *o = findObj(name);
	
	if (o == nullptr)
		return;
		
    glm::vec4 pos = glm::vec4(x, y, z, 1.0);

	if (!o->moveSmoothly)
		o->position = pos;

	o->nextPosition = pos;
}

glm::vec4 Engine2::getPos(std::string name)
{
	Object *o = findObj(name);
	
	if (o == nullptr)
		return glm::vec4(0.0, 0.0, 0.0, 0.0);
	
	return o->position;
}

void Engine2::setNextPos(std::string name, float x, float y, float z)
{
	Object *o = findObj(name);
	
	if (o == nullptr)
		return;
	
	glm::vec4 pos = glm::vec4(x, y, z, 1.0);

	o->nextPosition = pos;
}

glm::vec4 Engine2::getNextPos(std::string name)
{
	Object *o = findObj(name);
	
	if (o == nullptr)
		return glm::vec4(0.0, 0.0, 0.0, 0.0);
	
	return o->nextPosition;
}

void Engine2::setOrientation(std::string name, float yaw, float pitch, float roll)
{
    Object *o = findObj(name);
	
	if (o == nullptr)
		return;
	
    o->yaw = yaw;
    o->pitch = pitch;
    o->roll = roll;
}

void Engine2::setYaw(std::string name, float yaw)
{
    Object *o = findObj(name);
	
	if (o == nullptr)
		return;
	
    o->yaw = yaw;
}

void Engine2::setPitch(std::string name, float pitch)
{
    Object *o = findObj(name);
	
	if (o == nullptr)
		return;
	
    o->pitch = pitch;
}

void Engine2::setRoll(std::string name, float roll)
{
    Object *o = findObj(name);
	
	if (o == nullptr)
		return;
	
    o->roll = roll;
}

float Engine2::getYaw(std::string name)
{
    Object *o = findObj(name);
	
	if (o == nullptr)
		return 0.0;
	
    return o->yaw;
}

float Engine2::getPitch(std::string name)
{
    Object *o = findObj(name);
	
	if (o == nullptr)
		return 0.0;
	
    return o->pitch;
}

float Engine2::getRoll(std::string name)
{
    Object *o = findObj(name);
	
	if (o == nullptr)
		return 0.0;
	
    return o->roll;
}

void Engine2::setAlwaysFacePlayer(std::string name, bool value)
{
	Object *o = findObj(name);
	
	if (o == nullptr)
		return;
	
	o->alwaysFacePlayer = value;
}

void Engine2::setCamera(glm::vec4 position, glm::vec4 orientation)
{
}

void Engine2::setSun(glm::vec4 position, float yaw, float pitch, float roll)
{
	sun.position = position;
	sun.yaw = yaw;
	sun.pitch = pitch;
	sun.roll = roll;
}

void Engine2::setMoveSmoothly(std::string name, bool value)
{
	Object *o = findObj(name);
	
	if (o == nullptr)
		return;
	
	o->moveSmoothly = value;
}

void Engine2::setFade(std::string name, float near, float far)
{
	Object *o = findObj(name);
	
	if (o == nullptr)
		return;
	
	o->fadeNearDistance = near;
	o->fadeFarDistance = near;
}

class ObjectCompare
{
public:
	bool operator()(const Object*& first, const Object*& second) const
	{
		return first->name == second->name;
	}
};

Object *Engine2::findObj(std::string name)
{
    Object *obj = objects[name];

    return obj;
}

void Engine2::newShape(std::string name, ObjShapeType type, float sizeA, float sizeB, float sizeC, float sizeD)
{
//    Log("newShape");
//	Log(name.c_str(), sizeA);
	
	if (findShape(name) != nullptr)
		return;
	
    Shape *s = new Shape();
    s->name = name;
    s->type = type;
    s->sizeA = sizeA;
    s->sizeB = sizeB;
    s->sizeC = sizeC;
    s->sizeD = sizeD;
    s->type = type;

    s->generate(&voxelTextures, &extraStrings);

    shapes[name] = s;
}

Shape *Engine2::findShape(std::string name)
{
    Shape *shape = shapes[name];

	return shape;
}


void Engine2::loadModel(std::string fileName)
{
    Model2 *m = new Model2();
    m->load(fileName, modelRenderer.getVAO());
    models[fileName] = m;
}
	
Model2 *Engine2::findModel(std::string modelName)
{
    Model2 *model = models[modelName];

	return model;
}

void Engine2::setSkybox(std::string textureName)
{
    Texture *tex = texMan.find(textureName);

    if (tex == nullptr)
    {
        texMan.load(textureName);
    }

    Texture *tex2 = texMan.find(textureName);

    if (tex2 != nullptr) {
        skyboxGLTexID = tex2->glTexID;
    }
}

void Engine2::touchEvent(int count, int action1, float x1, float y1, int action2, float x2, float y2)
{
//	Log("x1", (int)(x1 * 1000), "y1", (int)(y1 * 1000));
	
	std::string extra1 = textPrinter.getOnClickExtraIfClicked(action1, x1, y1, 0, 1);
	extra1 = gui.getOnClickExtraIfClicked(action1, x1, y1, 0, 1);
	
	if (extra1 != "")
		setExtraInt(extra1, action1);
	
	std::string extra2 = textPrinter.getOnClickExtraIfClicked(action2, x2, y2, 0, 1);
	extra2 = gui.getOnClickExtraIfClicked(action2, x2, y2, 0, 1);

	if (action2 != 0 && extra2 != "")
		setExtraInt(extra2, action2);
	
	if (extra1 == "")
	{
		controls.touchEvent(count, action1, x1, y1, action2, x2, y2);
	}
	
	setExtraInt("touchaction1", action1);
	setExtraInt("touchaction2", action2);
	
/*	static int clickTimer = 0;
	
	if (action1 == 0 && extra != "" && clickTimer == 0)
	{
		std::string s = extra;
		Log(extra);
		setExtraInt(extra, 1);
//		clickTimer = CLICK_TIMER;
	}
	
	if (clickTimer > 0)
		clickTimer--;*/
}

void Engine2::setCameraPos(float x, float y, float z)
{
	glm::vec4 pos = glm::vec4(x, y, z, 1.0);
	camera.position = pos;
}

void Engine2::setCameraOrientation(float yaw, float pitch, float roll)
{
	camera.yaw = yaw;
	camera.pitch = pitch;
	camera.roll = roll;
}

void Engine2::setPlayerObj(std::string name)
{
#ifdef USE_PLAYEROBJ
	Object *o = findObj(name);
	playerObj = o;
	camera.setPlayerObj(o);
	controls.setPlayerObj(o);
	editorController.setPlayerObj(o);
#endif
}

void Engine2::setPlayerPos(float x, float y, float z)
{
	glm::vec4 pos = glm::vec4(x, y, z, 1.0);
	playerObj->position = pos;
}

void Engine2::setPlayerOrientation(float pitch, float yaw, float roll)
{
	playerObj->pitch = pitch;
	playerObj->yaw = yaw;
	playerObj->roll = roll;
	
#ifndef USE_PLAYEROBJ
	player.yaw = yaw;
	player.pitch = pitch;
	player.roll = roll;
#endif
}

void Engine2::setPlayerPitch(float pitch)
{
	playerObj->pitch = pitch;
}

void Engine2::setPlayerYaw(float yaw)
{
	playerObj->yaw = yaw;
}

void Engine2::setPlayerRoll(float roll)
{
	playerObj->roll = roll;
}

float Engine2::getPlayerPitch()
{
	return playerObj->pitch;
}

float Engine2::getPlayerYaw()
{
	return playerObj->yaw;
}

float Engine2::getPlayerRoll()
{
	return playerObj->roll;
}

void Engine2::setVehiclePos(float x, float y, float z)
{
	glm::vec4 pos = glm::vec4(x, y, z, 1.0);
	vehicle.position = pos;
}

void Engine2::setVehicleOrientation(float yaw, float pitch, float roll)
{
	vehicle.yaw = yaw;
	vehicle.pitch = pitch;
	vehicle.roll = roll;
}

void Engine2::setPlayerInVehicle(bool value)
{
#ifndef USE_PLAYEROBJ
	player.inVehicle = value;
#endif
}

void Engine2::setPartOfVehicle(std::string name, bool value)
{
	Object *o = findObj(name);

	if (o == nullptr)
		return;

	o->isPartOfVehicle = value;
}

void Engine2::setLargeScale(bool value)
{
	g_largeScale = value;
}

void Engine2::setControlScheme(ControlSchemes scheme)
{
	controls.setControlScheme(scheme);
}

void Engine2::setCameraScheme(CameraSchemes scheme)
{
	camera.setCameraScheme(scheme);
}

void Engine2::setUseShadowMap(bool value)
{
	useShadowMap = value;
}

void Engine2::moveObjectsZ(float fromZ, float fromRadius, float toZ)
{
    for(const auto &pair: objects)
    {
        Object *obj = pair.second;

		if (obj != nullptr && obj->position.z >= fromZ - fromRadius && obj->position.z <= fromZ + fromRadius)
		{
			float deltaZ = obj->position.z - fromZ;

			obj->position.z = toZ + deltaZ;
		}
	}
}

void Engine2::moveObjectsX(float fromX, float fromRadius, float toX)
{
    for(const auto &pair: objects)
    {
        Object *obj = pair.second;

        if (obj != nullptr && obj->position.x >= fromX - fromRadius && obj->position.x <= fromX + fromRadius)
		{
			float deltaX = obj->position.x - fromX;

			obj->position.x = toX + deltaX;
		}
	}
}

void Engine2::setTriggerAlwaysOn(bool value)
{
    controls.setTriggerAlwaysOn(value);
}

void Engine2::playSound(std::string name, bool stereo)
{
	audio.playSound(name, stereo);
}

void Engine2::playTrack(std::string name, bool stereo)
{
	audio.playTrack((char *)name.c_str(), stereo);
}

bool Engine2::checkCollision(Object *obj1, Object *obj2, float factor)
{
	float deltaX = abs(abs(obj1->position.x) - abs(obj2->position.x));
	float deltaY = abs(abs(obj1->position.y) - abs(obj2->position.y));
	float deltaZ = abs(abs(obj1->position.z) - abs(obj2->position.z));

	float avgSizeX = obj1->scale.x + obj2->scale.x / 2;
	float avgSizeY = obj1->scale.y + obj2->scale.y / 2;
	float avgSizeZ = obj1->scale.z + obj2->scale.z / 2;

	if (deltaX < avgSizeX * factor && deltaY < avgSizeY * factor && deltaZ < avgSizeZ * factor)
		return true;

	return false;
}

void Engine2::moveObjectsByVelocity()
{
	vehicle.moveByVelocity();

    for(const auto &pair: objects)
    {
        Object *obj = pair.second;

		if (obj != nullptr)
			obj->moveByVelocity();
	}
}

void Engine2::moveObjectsTowardsNextPosition()
{
    for(const auto &pair: objects)
    {
        Object *obj = pair.second;

		if (obj != nullptr)
			obj->moveTowardsNextPosition();
	}
}

void Engine2::moveObjectsSmoothly()
{
    for(const auto &pair: objects)
    {
        Object *obj = pair.second;

        if (obj != nullptr && obj->moveSmoothly)
			obj->moveTowardsNextPosition();
	}
}

void Engine2::setHitPoints(std::string name, int pts)
{
	Object *o = findObj(name);

	if (o == nullptr)
		return;

	o->hitPoints = pts;
}

void Engine2::addText(std::string name, std::string text, float x, float y, float size, std::string onClickExtra)
{
	glm::vec4 pos = glm::vec4(x, y, 1.0, 1.0);

	textPrinter.addText(name, text, pos, size, onClickExtra);
}

void Engine2::setText(std::string name, std::string text)
{
	textPrinter.setText(name, text);
}

void Engine2::setTextVisible(std::string name, bool visible)
{
	textPrinter.setVisible(name, visible);
}

void Engine2::setControlsEnabled(bool enabled)
{
	controls.setEnabled(enabled);
}

void Engine2::setHealthBar(int idx, float value)
{
	if (idx == 0)
		healthBar1Value = value;

	if (idx == 1)
		healthBar2Value = value;
}

void Engine2::setGlobalColor(glm::vec4 target)
{
	skyboxRenderer.setGlobalColor(target);
	shapeRenderer.setGlobalColor(target);
	modelRenderer.setGlobalColor(target);
//	spriteRenderer2D.fadeGlobalColor(target, fadeLength);
}

void Engine2::fadeGlobalColor(glm::vec4 target, int length)
{
    int fadeLength = length;

    skyboxRenderer.fadeGlobalColor(target, fadeLength);
    shapeRenderer.fadeGlobalColor(target, fadeLength);
    modelRenderer.fadeGlobalColor(target, fadeLength);
//	spriteRenderer2D.fadeGlobalColor(target, fadeLength);
}

void Engine2::setHealthBarsVisible(bool value)
{
	healthBarsVisible = value;
}

void Engine2::setControlsVisible(bool value)
{
	controlsVisible = value;
}

void Engine2::setExtraInt(std::string name, int value)
{
	extraInts[name] = value;
}

int Engine2::getExtraInt(std::string name)
{
	if (extraInts.count(name) == 0)
		return 0;

	return extraInts[name];
}

void Engine2::setExtraStr(std::string name, std::string value)
{
	extraStrings[name] = value;
}

std::string Engine2::getExtraStr(std::string name)
{
	if (extraStrings.count(name) == 0)
		return "";

	return extraStrings[name];
}

void Engine2::setVoxel(std::string shapeName, int x, int y, int z, char texture)
{
	Shape *s = findShape(shapeName);
	
	if (s == nullptr)
		return;

	if (s->type != SHAPE_VOXELS)
		return;

	Voxels *voxels = s->voxels;

	if (voxels == nullptr)
		return;

	voxels->set(x, y, z, texture);

	s->needsRebuild = true;
}

char Engine2::getVoxel(std::string shapeName, int x, int y, int z)
{
	Shape *s = findShape(shapeName);
	
	if (s == nullptr)
		return 0;

	if (s->type != SHAPE_VOXELS)
		return 0;

	Voxels *voxels = s->voxels;

	if (voxels == nullptr)
		return 0;

	return voxels->get(x, y, z);
}

void Engine2::setVoxelTexture(int voxel, std::string texture)
{
	voxelTextures[voxel] = texture;
}

std::string Engine2::getVoxelTexture(int voxel)
{
	return voxelTextures[voxel];
}

void Engine2::clearVoxels(std::string name)
{
	
}

void Engine2::rebuildShape(std::string shapeName)
{
    shapes[shapeName]->needsRebuild = true;
}

void Engine2::clear()
{
	objects.clear();

	for (const auto &pair : shapes)
	{
		Shape *shape = pair.second;
		if (shape != nullptr)
		    shape->free();
	}
	
	shapes.clear();
	
	textPrinter.clear();
}

void Engine2::limitPlayerRange()
{
	float minX = controls.getMinPlayerX();
	float maxX = controls.getMaxPlayerX();

	if (minX != 0.0 && playerObj->nextPosition.x < minX)
		playerObj->nextPosition.x = minX;
	if (maxX != 0.0 && playerObj->nextPosition.x > maxX)
		playerObj->nextPosition .x = maxX;
}

	
void Engine2::setEditVoxelsObj(std::string name)
{
	Object *obj = findObj(name);
	
	editorController.setEditVoxelsObj(obj);
}

void Engine2::addWg(std::string name, WidgetType type, std::string texture, std::string text, std::string onClickExtra, std::string group, float x, float y, float sizex, float sizey)
{
	gui.addWg(name, type, texture, text, onClickExtra, group, x, y, sizex, sizey);
}

void Engine2::setWgVisible(std::string name, bool val)
{
	gui.setWgVisible(name, val);
}

void Engine2::clearGUI()
{
	gui.clear();
}

void Engine2::save(std::string fname)
{
#ifdef PLATFORM_ANDROID
    if (fname == "")
	{
        char fname[1024];
        sprintf(fname, "%s/voxyc.vx", g_filesDir);	
	    editorController.save(fname);
	}
	else
	{
		editorController.save(fname);
	}
#endif
}

void Engine2::load(std::string fname)
{
#ifdef PLATFORM_ANDROID
    if (fname == "")
	{
        char fname[1024];
        sprintf(fname, "%s/voxyc.vx", g_filesDir);	
	    editorController.load(fname);
	}
	else
	{
		editorController.load(fname);
	}
#endif
}

void Engine2::writeObj(std::string name, std::string fname)
{
    Object *obj = objects[name];

    Voxels *voxels = obj->shape->getVoxels();

    objWriter.write(voxels->getBuffers(), fname);
}
