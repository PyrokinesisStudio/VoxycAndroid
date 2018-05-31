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

#ifndef VOXELS_H
#define VOXELS_H

#include <string>
#include <vector>
#include "../glm/glm.hpp"
#include <map>
#include "Mesh.h"

typedef struct Voxel
{
	char texture;
/*	char xofs;
	char yofs;
	char zofs;
	char r;
	char g;
	char b;*/
} Voxel;

typedef struct VoxelStack
{
	Voxel *voxels = nullptr;
	int height = 0;
} VoxelStack;

typedef struct VertexBuffer
{
	float *vertices = nullptr;
	int size = 0;
	int allocatedSize = 0;
	int cursor = 0;
};

typedef struct Vertex
{
	float x;
	float y;
	float z;
	float w;
	float u;
	float v;
};

class Voxels
{
public:
    void init(int size, std::map<int, std::string> *voxelTextures, std::map<std::string, std::string> *extraStrings);
	void set(int x, int y, int z, char value);
	char get(int x, int y, int z);
	VoxelStack *getStack(int x, int z);
	void setStackHeight(int x, int z, int height);
	void build(std::map<int, std::string> voxelTextures);
	void addQuad(std::string texture, Vertex ll, Vertex lr, Vertex ur, Vertex ul, int dir);
	void addVertex(std::string texture, float x, float y, float z, float w, float u, float v);
	std::vector<Mesh*> getMeshes();
	std::map<std::string, VertexBuffer*> getBuffers() { return buffers; };
	void clear();
	void checkGLError(char *tag);
	int getSize() { return size; };
	int save(std::string fname);
	int load(std::string fname);
	void calcMinMax();

	bool rebuild = false;

private:
    VoxelStack *stacks;
	int size = 0;
	bool initialized = false;
	int minx = 0;
	int maxx = 0;
	int miny = 0;
	int maxy = 0;
	int minz = 0;
	int maxz = 0;

	std::map<std::string, VertexBuffer*> buffers;
	std::map<int, std::string> *voxelTextures;
	std::map<std::string, std::string> *extraStrings;

//	static const int verticesSize = 1000000;
//  float vertices[verticesSize];
//	static int cursor;
};

#endif //FATELESS_TEXTUREMANAGER_H
