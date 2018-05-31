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

#ifndef FATELESS_ENGINE2_H
#define FATELESS_ENGINE2_H

#include "../Globals.hpp"
#include "Object.h"
#include <vector>
#include <string>
#include "ShapeRenderer.h"
#include "ModelRenderer.h"
#include "TextureManager2.h"
#include "Shape.h"
#include "Model2.h"
#include "Controls2.h"
#include "SkyboxRenderer.h"
#include "ShadowMap.h"
#include "Player.h"
#include "Vehicle.h"
#include "Camera.h"
#include "Map.h"
#include "Audio.h"
#include "TextPrinter.h"
#include <map>

#ifdef USE_OZZ
#include "OzzRenderer.h"
#endif

#ifdef PLATFORM_ANDROID
#include "AndroidAudio.h"
#include "../android.h"
#endif

#include "GUI.h"
#include "OBJWriter.h"

class Engine2
{
public:
	void init();
    void tick();
	void draw(int eye);
	void drawShadowMap(int eye);
	void drawHealthBars();
	void free();

	void addObject(std::string name);
	void setType(std::string name, ObjType type);
	void setShape(std::string name, ObjShapeType shape);
    void setShape(std::string name, std::string shapeName);
	void setModel(std::string name, std::string modelName);
	void setColor(std::string name, float r, float g, float b, float a);
	void setMeshColor(std::string name, int mesh, float r, float g, float b, float a);
	void setTexture(std::string name, std::string textureName);
	void setTextureSpan(std::string name, float textureSpanX, float textureSpanY);
	void setVisible(std::string name, bool visible);
	void setMapTexture(std::string name);
	void setSize(std::string name, float size);
	void setSize(std::string name, float sizex, float sizey, float sizez);
	void setPos(std::string name, float x, float y, float z);
	glm::vec4 getPos(std::string name);
	void setNextPos(std::string name, float x, float y, float z);
	glm::vec4 getNextPos(std::string name);
	void setOrientation(std::string name, float yaw, float pitch, float roll);
	void setYaw(std::string name, float yaw);
    void setPitch(std::string name, float pitch);
    void setRoll(std::string name, float roll);
    float getYaw(std::string name);
    float getPitch(std::string name);
    float getRoll(std::string name);
	void setAlwaysFacePlayer(std::string name, bool value);
	void setCamera(glm::vec4 position, glm::vec4 orientation);
	void setSun(glm::vec4 position, float yaw, float pitch, float roll);
	void setMoveSmoothly(std::string name, bool value);
	void setFade(std::string name, float near, float far);
	Object *findObj(std::string name);

    void newShape(std::string name, ObjShapeType type, float sizeA, float sizeB, float sizeC = 1.0, float sizeD = 1.0);
    Shape *findShape(std::string name);

	void loadModel(std::string fileName);
	Model2 *findModel(std::string modelName);

    void setSkybox(std::string textureName);

    void touchEvent(int count, int action1, float x1, float y1, int action2, float x2, float y2);
//    void touchEvent(int action, float x, float y, int finger, int count);

	Controls2 *getControls() { return &controls; };
    EditorController *getEditorController() { return &editorController; };

	void setCameraPos(float x, float y, float z);
	void setCameraOrientation(float yaw, float pitch, float roll);

	void setPlayerObj(std::string name);
	void setPlayerPos(float x, float y, float z);
	void setPlayerOrientation(float pitch, float yaw, float roll);
	void setPlayerPitch(float pitch);
	void setPlayerYaw(float yaw);
	void setPlayerRoll(float roll);
    float getPlayerPitch();
	float getPlayerYaw();
	float getPlayerRoll();

	void setVehiclePos(float x, float y, float z);
	void setVehicleOrientation(float yaw, float pitch, float roll);

	void setPlayerInVehicle(bool value);
	void setPartOfVehicle(std::string name, bool value);

	void setLargeScale(bool value);
	void setControlScheme(ControlSchemes scheme);
	void setCameraScheme(CameraSchemes scheme);
	void setUseShadowMap(bool value);

	Vehicle *getVehicle() { return &vehicle; };
	Camera *getCamera() { return &camera; };

	void moveObjectsZ(float fromZ, float fromRadius, float toZ);
	void moveObjectsX(float fromX, float fromRadius, float toX);

    void setTriggerAlwaysOn(bool value);

	void playSound(std::string name, bool stereo);
	void playTrack(std::string name, bool stereo);

	bool checkCollision(Object *obj1, Object *obj2, float factor = 0.5);
	void moveObjectsByVelocity();
	void moveObjectsTowardsNextPosition();
	void moveObjectsSmoothly();

	void setHitPoints(std::string name, int pts);

	void addText(std::string name, std::string text, float x, float y, float size, std::string onClickExtra = "");
	void setText(std::string name, std::string text);
	void setTextVisible(std::string name, bool visible);

	void setControlsEnabled(bool enabled);

	void setHealthBar(int index, float value);
	void fadeGlobalColor(glm::vec4 target, int length);
    void setGlobalColor(glm::vec4 target);

	void setHealthBarsVisible(bool value);
	void setControlsVisible(bool value);
	
	void setExtraInt(std::string name, int value);
	int getExtraInt(std::string name);
	void setExtraStr(std::string name, std::string value);
	std::string getExtraStr(std::string name);

	void setVoxel(std::string shapeName, int x, int y, int z, char texture);
	char getVoxel(std::string shapeName, int x, int y, int z);
	void setVoxelTexture(int voxel, std::string texture);
	std::string getVoxelTexture(int voxel);
	void clearVoxels(std::string name);
    void rebuildShape(std::string shapeName);

	void clear();

	void limitPlayerRange();
	
	void setEditVoxelsObj(std::string name);
	
	void addWg(std::string name, WidgetType type, std::string texture, std::string text, std::string onClickExtra, std::string group, float x, float y, float sizex, float sizey);
    void setWgVisible(std::string name, bool val);
	void clearGUI();
	
	void save(std::string fname = "");
	void load(std::string fname = "");

	void writeObj(std::string name, std::string fname = "");
	
private:
	ShapeRenderer shapeRenderer;
    SkyboxRenderer skyboxRenderer;
	ModelRenderer modelRenderer;
    SpriteRenderer2D spriteRenderer2D;
	TextPrinter textPrinter;
	GUI gui;

#ifdef USE_OZZ
	OzzRenderer ozzRenderer;
#endif

	TextureManager2 texMan;
	ShadowMap shadowMap;

    std::map<std::string, Object*> objects;
    std::map<std::string, Shape*> shapes;
    std::map<std::string, Model2*> models;
	
    Camera camera;
	Object sun;
    Controls2 controls;

#ifdef USE_PLAYEROBJ
    Object defaultPlayerObj;
    Object *playerObj = &defaultPlayerObj;
#else
	Player player;
#endif
	Vehicle vehicle;
	Map map;
#ifdef PLATFORM_ANDROID
    AndroidAudio audio;
#else
	Audio audio;
#endif

    EditorController editorController;

    int skyboxGLTexID = -1;
	bool useShadowMap = false;
	bool shadowMapReady = false;

	float healthBar1Value = 0.0;
	float healthBar2Value = 0.0;

	Object mouseLook;

	bool healthBarsVisible = true;
	bool controlsVisible = true;
	
	std::map<std::string, int> extraInts;
	std::map<std::string, std::string> extraStrings;

	std::map<int, std::string> voxelTextures;

	bool initialized = false;

	OBJWriter objWriter;
};

extern Engine2 *g_engine2;

#endif //FATELESS_ENGINE2_H
