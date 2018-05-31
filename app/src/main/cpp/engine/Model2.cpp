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

#include "../Globals.hpp"
#include "Model2.h"
#include "../assimp/Importer.hpp"      // C++ importer interface
#include "../assimp/scene.h"           // Output data structure
#include "../assimp/postprocess.h"     // Post processing fla
#include "../android.h"
#include "GLIncludes.h"
#include "../DDLUtils.hpp"

void Model2::load(std::string filename, int vao)
{
#ifdef USE_ASSIMP

#if defined PLATFORM_OSX || defined PLATFORM_WINDOWS || defined PLATFORM_OPENVR
	char fullFilename[MAX_STR_LEN];
	printFullResourceFilename((char *)filename.c_str(), fullFilename);

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(fullFilename,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate
		| aiProcess_JoinIdenticalVertices
		| aiProcess_SortByPType
		| aiProcess_GenNormals
	);
#endif

#ifdef PLATFORM_ANDROID
	Assimp::Importer importer;

    char ffname[1024];
    sprintf(ffname, "%s/%s", g_externalFilesDir, filename.c_str());
	
	Log(ffname);

    const aiScene* scene = importer.ReadFile(ffname,
                                             aiProcess_CalcTangentSpace |
                                             aiProcess_Triangulate
                                             | aiProcess_JoinIdenticalVertices
                                             | aiProcess_SortByPType
                                             | aiProcess_GenNormals
    );
#endif

    if (scene == nullptr)
        return;

    char str[1024];

//    Model2 *model = new Model2();

    numMeshes = scene->mNumMeshes;

    for (int m = 0; m < scene->mNumMeshes; m++)
    {
        Mesh *outMesh = new Mesh();

		outMesh->index = m;
		outMesh->color = glm::vec4(1.0, 1.0, 1.0, 1.0);

        aiMesh *mesh = scene->mMeshes[m];

        outMesh->hasTexCoords = mesh->HasTextureCoords(0);

        // Vertices and texture coords

        if (mesh->HasPositions())
        {
            int vertexDataLength = 9;

            outMesh->numVertices = mesh->mNumVertices;
            outMesh->data = (float *)malloc(mesh->mNumVertices * vertexDataLength * sizeof(float));

            for (int v = 0; v < mesh->mNumVertices; v++)
            {
                outMesh->data[v * vertexDataLength] = mesh->mVertices[v].x;
                outMesh->data[v * vertexDataLength + 1] = mesh->mVertices[v].y;
                outMesh->data[v * vertexDataLength + 2] = mesh->mVertices[v].z;
                outMesh->data[v * vertexDataLength + 3] = 1.0;

                // Update min/max for scaling
                if (mesh->mVertices[v].x < minX)
                    minX = mesh->mVertices[v].x;
                if (mesh->mVertices[v].y < minY)
                    minY = mesh->mVertices[v].y;
                if (mesh->mVertices[v].z < minZ)
                    minZ = mesh->mVertices[v].z;

                if (mesh->mVertices[v].x > maxX)
                    maxX = mesh->mVertices[v].x;
                if (mesh->mVertices[v].y > maxY)
                    maxY = mesh->mVertices[v].y;
                if (mesh->mVertices[v].z > maxZ)
                    maxZ = mesh->mVertices[v].z;

                if (mesh->HasTextureCoords(0))
                {
                    outMesh->data[v * vertexDataLength + 4] = mesh->mTextureCoords[0][v].x;
                    outMesh->data[v * vertexDataLength + 5] = mesh->mTextureCoords[0][v].y;
                }
                else
                {
                    outMesh->data[v * vertexDataLength + 4] = 0;
                    outMesh->data[v * vertexDataLength + 5] = 0;
                }

                if (mesh->HasNormals())
                {
                    outMesh->data[v * vertexDataLength + 6] = mesh->mNormals[v].x;
                    outMesh->data[v * vertexDataLength + 7] = mesh->mNormals[v].y;
                    outMesh->data[v * vertexDataLength + 8] = mesh->mNormals[v].z;
                }
                else
                {
                    outMesh->data[v * vertexDataLength + 6] = 0;
                    outMesh->data[v * vertexDataLength + 7] = 0;
                    outMesh->data[v * vertexDataLength + 8] = 0;
                }
            }

            outMesh->dataLength = outMesh->numVertices * vertexDataLength;
        }

        // Indexes

        if (mesh->HasFaces())
        {
            int indexDataLength = 3;

            outMesh->numIndexes = mesh->mNumFaces * 3;

            outMesh->indexData = (int *)malloc(mesh->mNumFaces * 3 * indexDataLength * sizeof(int));
            outMesh->numPolys = mesh->mNumFaces;

            for (int f = 0; f < mesh->mNumFaces; f++)
            {
                outMesh->indexData[f * indexDataLength] = mesh->mFaces[f].mIndices[0];
                outMesh->indexData[f * indexDataLength + 1] = mesh->mFaces[f].mIndices[1];
                outMesh->indexData[f * indexDataLength + 2] = mesh->mFaces[f].mIndices[2];
            }

            //    }

            outMesh->indexDataLength = outMesh->numIndexes * indexDataLength;
        }

        // Bones

        if (mesh->HasBones())
        {
            for (int b = 0; b < mesh->mNumBones; b++)
            {
                aiBone * bone = mesh->mBones[b];
            }
        }

        // Add mesh to model
        meshes.push_back(outMesh);

		// Bind the VAO
		glBindVertexArray(vao);
		checkGLError("glBindVertexArray");

		// Make and bind the vertex and texcoords VBO
        glGenBuffers(1, (GLuint *)&outMesh->vbo);
        checkGLError("glGenBuffers");

        // Make and bind the index VBO
        glGenBuffers(1, (GLuint *)&outMesh->indexVBO);
        checkGLError("glGenBuffers");

        for (int m = 0; m < numMeshes; m++)
        {
            glBindBuffer(GL_ARRAY_BUFFER, outMesh->vbo);
            checkGLError("glBindBuffer");

            glBufferData(GL_ARRAY_BUFFER, sizeof(float) * outMesh->dataLength, outMesh->data, GL_STATIC_DRAW);
            checkGLError("glBufferData");

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, outMesh->indexVBO);
            checkGLError("glBindBuffer");

            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * outMesh->indexDataLength, outMesh->indexData, GL_STATIC_DRAW);
            checkGLError("FrameAnimRenderer glBufferData");
        }
    }

	// Calculate proportions
	float max = 0;
	float diffx = maxX - minX;
	float diffy = maxY - minY;
	float diffz = maxZ - minZ;

	if (diffx >= diffy && diffx >= diffz)
		max = diffx;
	if (diffy >= diffx && diffy >= diffz)
		max = diffy;
	if (diffz >= diffy && diffz >= diffx)
		max = diffz;

	proportionScale = glm::vec4(diffx / max, diffy / max, diffz / max, 1.0);

    name = filename;
    loaded = true;
#endif
}

void Model2::free()
{
    for (int i = 0; i < meshes.size(); i++)
    {
        glDeleteBuffers(1, (GLuint *)&meshes[i]->vbo);
        checkGLError("glDeleteBuffers");
    }
	
	meshes.clear();
}

void Model2::checkGLError(char *tag)
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
