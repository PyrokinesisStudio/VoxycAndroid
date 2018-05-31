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

#include "GLIncludes.h"
#include "BuildingWall.h"
#include "../DDLUtils.hpp"
#include <vector>

int BuildingWall::vbo = 0;
int BuildingWall::numCoords = 0;
int BuildingWall::cursor = 0;
float BuildingWall::vertices[];
int verticesLength = 0;

void BuildingWall::generate(int width, int height, float windowSizeX, float windowSizeY)
{
    // Generate VBO
    glGenBuffers(1, (GLuint *)&vbo);
    checkGLError("glGenBuffers");

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    checkGLError("glBindBuffer");

    float textureSpan = 4.0;

    // Determine sizes
    int layers = (height * 2) + 1;
    int sections = (width * 2) + 1;

    float layerHeight = 2.0 / layers;
    float sectionWidth = 2.0 / sections;

    int worldBldWidth = sections * sectionWidth;
    int worldBldHeight = layers * layerHeight;

    bool windowSection = false;
    bool windowLayer = false;

    // Create wall
    float blockStartX = -1.0;
    float blockStartY = -1.0;

    // Window offsets
    float wox = -(windowSizeX * sectionWidth - sectionWidth) / 2;
    float wou = -wox / 2.0;
    float woy = -(windowSizeY * layerHeight - layerHeight) / 2;
    float wov = woy / 2.0;

    float winFrameUVSize = 0.01;

    BuildingWall::cursor = 0;

    for (int layer = 0; layer < layers; layer++)
    {
        if (!windowLayer)
        {
            // Solid wall

            // Front side

            float z = 1.0;

            //   ______
            // |\\5   4|
            // |0\\    |
            // |  \\   |
            // |   \\  |
            // |    \\3|
            // |1__2_\\|

            addVertex(-1.0, blockStartY + layerHeight + woy, z, 1.0,   0.0,               (1.0 - (worldBldHeight - (layerHeight * layer) + layerHeight) / worldBldHeight) * textureSpan);
            addVertex(-1.0, blockStartY - woy,               z, 1.0,   0.0,               (1.0 - (worldBldHeight - (layerHeight * layer)) / worldBldHeight) * textureSpan);
            addVertex(1.0, blockStartY - woy,                z, 1.0,   1.0 * textureSpan, (1.0 - (worldBldHeight - (layerHeight * layer)) / worldBldHeight) * textureSpan);

            addVertex(1.0, blockStartY - woy,                z, 1.0,   1.0 * textureSpan, (1.0 - (worldBldHeight - (layerHeight * layer)) / worldBldHeight) * textureSpan);
            addVertex(1.0, blockStartY + layerHeight + woy,  z, 1.0,   1.0 * textureSpan, (1.0 - (worldBldHeight - (layerHeight * layer)+ layerHeight) / worldBldHeight) * textureSpan);
            addVertex(-1.0, blockStartY + layerHeight + woy, z, 1.0,   0.0,               (1.0 - (worldBldHeight - (layerHeight * layer)+ layerHeight) / worldBldHeight) * textureSpan);

            // Back side

            z = -1.0;

            //   ______
            // |\\4   5|
            // |0\\    |
            // |  \\   |
            // |   \\  |
            // |    \\3|
            // |2__1_\\|

            addVertex(-1.0, blockStartY + layerHeight + woy, z, 1.0,   0.0,               (1.0 - (worldBldHeight - (layerHeight * layer) + layerHeight) / worldBldHeight) * textureSpan);
            addVertex(1.0, blockStartY - woy,                z, 1.0,   1.0 * textureSpan, (1.0 - (worldBldHeight - (layerHeight * layer)) / worldBldHeight) * textureSpan);
            addVertex(-1.0, blockStartY - woy,               z, 1.0,   0.0,               (1.0 - (worldBldHeight - (layerHeight * layer)) / worldBldHeight) * textureSpan);

            addVertex(1.0, blockStartY - woy,                z, 1.0,   1.0 * textureSpan, (1.0 - (worldBldHeight - (layerHeight * layer)) / worldBldHeight) * textureSpan);
            addVertex(-1.0, blockStartY + layerHeight + woy, z, 1.0,   0.0,               (1.0 - (worldBldHeight - (layerHeight * layer)+ layerHeight) / worldBldHeight) * textureSpan);
            addVertex(1.0, blockStartY + layerHeight + woy,  z, 1.0,   1.0 * textureSpan, (1.0 - (worldBldHeight - (layerHeight * layer)+ layerHeight) / worldBldHeight) * textureSpan);
        }
        else
        {
            float blockStartX = -1.0;
            windowSection = false;

            // Make windows
            for (int section = 0; section < sections; section++)
            {
                if (!windowSection)
                {
                    // Block in between windows

                    // Front

                    float z = 1.0;

                    //   ______
                    // |\\5   4|
                    // |0\\    |
                    // |  \\   |
                    // |   \\  |
                    // |    \\3|
                    // |1__2_\\|

                    addVertex(blockStartX - wox,                blockStartY + layerHeight - woy,  z, 1.0,   ((sectionWidth * section) / worldBldWidth) * textureSpan,                (1.0 - (worldBldHeight - (layerHeight * layer) + layerHeight) / worldBldHeight) * textureSpan);
                    addVertex(blockStartX - wox,                blockStartY + woy,                z, 1.0,   ((sectionWidth * section) / worldBldWidth) * textureSpan,                (1.0 - (worldBldHeight - (layerHeight * layer)) / worldBldHeight) * textureSpan);
                    addVertex(blockStartX + sectionWidth + wox, blockStartY + woy,                z, 1.0,   ((sectionWidth * section + sectionWidth) / worldBldWidth) * textureSpan, (1.0 - (worldBldHeight - (layerHeight * layer)) / worldBldHeight) * textureSpan);

                    addVertex(blockStartX + sectionWidth + wox, blockStartY + woy,                z, 1.0,   ((sectionWidth * section + sectionWidth) / worldBldWidth) * textureSpan, (1.0 - (worldBldHeight - (layerHeight * layer)) / worldBldHeight) * textureSpan);
                    addVertex(blockStartX + sectionWidth + wox, blockStartY + layerHeight - woy,  z, 1.0,   ((sectionWidth * section + sectionWidth) / worldBldWidth) * textureSpan, (1.0 - (worldBldHeight - (layerHeight * layer) + layerHeight) / worldBldHeight) * textureSpan);
                    addVertex(blockStartX - wox,                blockStartY + layerHeight - woy,  z, 1.0,   ((sectionWidth * section) / worldBldWidth) * textureSpan,                (1.0 - (worldBldHeight - (layerHeight * layer) + layerHeight) / worldBldHeight) * textureSpan);

                    // Back

                    z = -1.0;

                    //   ______
                    // |\\4   5|
                    // |0\\    |
                    // |  \\   |
                    // |   \\  |
                    // |    \\3|
                    // |2__1_\\|

                    addVertex(blockStartX - wox,                blockStartY + layerHeight - woy,  z, 1.0,   ((sectionWidth * section) / worldBldWidth) * textureSpan,                (1.0 - (worldBldHeight - (layerHeight * layer) + layerHeight) / worldBldHeight) * textureSpan);
                    addVertex(blockStartX + sectionWidth + wox, blockStartY + woy,                z, 1.0,   ((sectionWidth * section + sectionWidth) / worldBldWidth) * textureSpan, (1.0 - (worldBldHeight - (layerHeight * layer)) / worldBldHeight) * textureSpan);
                    addVertex(blockStartX - wox,                blockStartY + woy,                z, 1.0,   ((sectionWidth * section) / worldBldWidth) * textureSpan,                (1.0 - (worldBldHeight - (layerHeight * layer)) / worldBldHeight) * textureSpan);

                    addVertex(blockStartX + sectionWidth + wox, blockStartY + woy,                z, 1.0,   ((sectionWidth * section + sectionWidth) / worldBldWidth) * textureSpan, (1.0 - (worldBldHeight - (layerHeight * layer)) / worldBldHeight) * textureSpan);
                    addVertex(blockStartX - wox,                blockStartY + layerHeight - woy,  z, 1.0,   ((sectionWidth * section) / worldBldWidth) * textureSpan,                (1.0 - (worldBldHeight - (layerHeight * layer) + layerHeight) / worldBldHeight) * textureSpan);
                    addVertex(blockStartX + sectionWidth + wox, blockStartY + layerHeight - woy,  z, 1.0,   ((sectionWidth * section + sectionWidth) / worldBldWidth) * textureSpan, (1.0 - (worldBldHeight - (layerHeight * layer) + layerHeight) / worldBldHeight) * textureSpan);
                }
                else
                {
                    // Window frame

                    //   ______
                    // |\\5   4|
                    // |0\\    |
                    // |  \\   |
                    // |   \\  |
                    // |    \\3|
                    // |1__2_\\|

                    // Bottom

                    addVertex(blockStartX + wox,                blockStartY + woy,             -1.0, 1.0,   ((sectionWidth * section) / worldBldWidth) * textureSpan,                (1.0 - (worldBldHeight - (layerHeight * layer)) / worldBldHeight - winFrameUVSize) * textureSpan);
                    addVertex(blockStartX + wox,                blockStartY + woy,              1.0, 1.0,   ((sectionWidth * section) / worldBldWidth) * textureSpan,                (1.0 - (worldBldHeight - (layerHeight * layer)) / worldBldHeight) * textureSpan);
                    addVertex(blockStartX + sectionWidth - wox, blockStartY + woy,              1.0, 1.0,   ((sectionWidth * section + sectionWidth) / worldBldWidth) * textureSpan, (1.0 - (worldBldHeight - (layerHeight * layer)) / worldBldHeight) * textureSpan);

                    addVertex(blockStartX + sectionWidth - wox, blockStartY + woy,              1.0, 1.0,   ((sectionWidth * section + sectionWidth) / worldBldWidth) * textureSpan, (1.0 - (worldBldHeight - (layerHeight * layer)) / worldBldHeight) * textureSpan);
                    addVertex(blockStartX + sectionWidth - wox, blockStartY + woy,             -1.0, 1.0,   ((sectionWidth * section + sectionWidth) / worldBldWidth) * textureSpan, (1.0 - (worldBldHeight - (layerHeight * layer)) / worldBldHeight - winFrameUVSize) * textureSpan);
                    addVertex(blockStartX + wox,                blockStartY + woy,             -1.0, 1.0,   ((sectionWidth * section) / worldBldWidth) * textureSpan,                (1.0 - (worldBldHeight - (layerHeight * layer)) / worldBldHeight - winFrameUVSize) * textureSpan);

                    // Left

                    addVertex(blockStartX + wox,                blockStartY + layerHeight - woy, 1.0, 1.0,   ((sectionWidth * section) / worldBldWidth) * textureSpan,                (1.0 - (worldBldHeight - (layerHeight * layer) + layerHeight) / worldBldHeight) * textureSpan);
                    addVertex(blockStartX + wox,                blockStartY + woy,               1.0, 1.0,   ((sectionWidth * section) / worldBldWidth) * textureSpan,                (1.0 - (worldBldHeight - (layerHeight * layer)) / worldBldHeight) * textureSpan);
                    addVertex(blockStartX + wox,                blockStartY + woy,              -1.0, 1.0,   ((sectionWidth * section) / worldBldWidth + winFrameUVSize) * textureSpan,                (1.0 - (worldBldHeight - (layerHeight * layer)) / worldBldHeight) * textureSpan);

                    addVertex(blockStartX + wox,                blockStartY + woy,              -1.0, 1.0,   ((sectionWidth * section) / worldBldWidth + winFrameUVSize) * textureSpan,                (1.0 - (worldBldHeight - (layerHeight * layer)) / worldBldHeight) * textureSpan);
                    addVertex(blockStartX + wox,                blockStartY + layerHeight - woy,-1.0, 1.0,   ((sectionWidth * section) / worldBldWidth + winFrameUVSize) * textureSpan,                (1.0 - (worldBldHeight - (layerHeight * layer) + layerHeight) / worldBldHeight) * textureSpan);
                    addVertex(blockStartX + wox,                blockStartY + layerHeight - woy, 1.0, 1.0,   ((sectionWidth * section) / worldBldWidth) * textureSpan,                (1.0 - (worldBldHeight - (layerHeight * layer) + layerHeight) / worldBldHeight) * textureSpan);

                    // Right

                    addVertex(blockStartX + sectionWidth - wox, blockStartY + layerHeight - woy,-1.0, 1.0,   ((sectionWidth * section + sectionWidth) / worldBldWidth) * textureSpan, (1.0 - (worldBldHeight - (layerHeight * layer) + layerHeight) / worldBldHeight) * textureSpan);
                    addVertex(blockStartX + sectionWidth - wox, blockStartY + woy,              -1.0, 1.0,   ((sectionWidth * section + sectionWidth) / worldBldWidth) * textureSpan, (1.0 - (worldBldHeight - (layerHeight * layer)) / worldBldHeight) * textureSpan);
                    addVertex(blockStartX + sectionWidth - wox, blockStartY + woy,               1.0, 1.0,   ((sectionWidth * section + sectionWidth) / worldBldWidth + winFrameUVSize) * textureSpan, (1.0 - (worldBldHeight - (layerHeight * layer)) / worldBldHeight) * textureSpan);

                    addVertex(blockStartX + sectionWidth - wox, blockStartY + woy,               1.0, 1.0,   ((sectionWidth * section + sectionWidth) / worldBldWidth + winFrameUVSize) * textureSpan, (1.0 - (worldBldHeight - (layerHeight * layer)) / worldBldHeight) * textureSpan);
                    addVertex(blockStartX + sectionWidth - wox, blockStartY + layerHeight - woy, 1.0, 1.0,   ((sectionWidth * section + sectionWidth) / worldBldWidth + winFrameUVSize) * textureSpan, (1.0 - (worldBldHeight - (layerHeight * layer) + layerHeight) / worldBldHeight) * textureSpan);
                    addVertex(blockStartX + sectionWidth - wox, blockStartY + layerHeight - woy,-1.0, 1.0,   ((sectionWidth * section + sectionWidth) / worldBldWidth) * textureSpan, (1.0 - (worldBldHeight - (layerHeight * layer) + layerHeight) / worldBldHeight) * textureSpan);

                    // Top

                    addVertex(blockStartX + wox,                blockStartY + layerHeight - woy, 1.0, 1.0,   ((sectionWidth * section) / worldBldWidth) * textureSpan,                (1.0 - (worldBldHeight - (layerHeight * layer) + layerHeight) / worldBldHeight) * textureSpan);
                    addVertex(blockStartX + wox,                blockStartY + layerHeight - woy,-1.0, 1.0,   ((sectionWidth * section) / worldBldWidth) * textureSpan,                (1.0 - (worldBldHeight - (layerHeight * layer) + layerHeight) / worldBldHeight + winFrameUVSize) * textureSpan);
                    addVertex(blockStartX + sectionWidth - wox, blockStartY + layerHeight - woy,-1.0, 1.0,   ((sectionWidth * section + sectionWidth) / worldBldWidth) * textureSpan, (1.0 - (worldBldHeight - (layerHeight * layer) + layerHeight) / worldBldHeight + winFrameUVSize) * textureSpan);

                    addVertex(blockStartX + sectionWidth - wox, blockStartY + layerHeight - woy,-1.0, 1.0,   ((sectionWidth * section + sectionWidth) / worldBldWidth) * textureSpan, (1.0 - (worldBldHeight - (layerHeight * layer) + layerHeight) / worldBldHeight + winFrameUVSize) * textureSpan);
                    addVertex(blockStartX + sectionWidth - wox, blockStartY + layerHeight - woy, 1.0, 1.0,   ((sectionWidth * section + sectionWidth) / worldBldWidth) * textureSpan, (1.0 - (worldBldHeight - (layerHeight * layer) + layerHeight) / worldBldHeight) * textureSpan);
                    addVertex(blockStartX + wox,                blockStartY + layerHeight - woy, 1.0, 1.0,   ((sectionWidth * section) / worldBldWidth) * textureSpan,                (1.0 - (worldBldHeight - (layerHeight * layer) + layerHeight) / worldBldHeight) * textureSpan);
                }

                windowSection = !windowSection;
                blockStartX += sectionWidth;
            }
        }

        windowLayer = !windowLayer;
        blockStartY += layerHeight;
    }

    // Frame for the whole wall

    //   ______
    // |\\5   4|
    // |0\\    |
    // |  \\   |
    // |   \\  |
    // |    \\3|
    // |1__2_\\|

    // Left side

    addVertex(-1.0, 1.0, -1.0, 1.0,    0.0, 0.0);
    addVertex(-1.0, -1.0, -1.0, 1.0,   0.0, textureSpan);
    addVertex(-1.0, -1.0, 1.0, 1.0,    winFrameUVSize, textureSpan);

    addVertex(-1.0, -1.0, 1.0, 1.0,    winFrameUVSize, textureSpan);
    addVertex(-1.0, 1.0, 1.0, 1.0,     winFrameUVSize, 0.0);
    addVertex(-1.0, 1.0, -1.0, 1.0,    0.0, 0.0);

    // Right side

    addVertex(1.0, 1.0, 1.0, 1.0,    0.0, 0.0);
    addVertex(1.0, -1.0, 1.0, 1.0,   0.0, textureSpan);
    addVertex(1.0, -1.0, -1.0, 1.0,    winFrameUVSize, textureSpan);

    addVertex(1.0, -1.0, -1.0, 1.0,    winFrameUVSize, textureSpan);
    addVertex(1.0, 1.0, -1.0, 1.0,     winFrameUVSize, 0.0);
    addVertex(1.0, 1.0, 1.0, 1.0,    0.0, 0.0);

    // Top side
    addVertex(-1.0, 1.0, -1.0, 1.0,    0.0, 0.0);
    addVertex(-1.0, 1.0, 1.0, 1.0,   0.0, winFrameUVSize);
    addVertex(1.0, 1.0, 1.0, 1.0,    textureSpan, winFrameUVSize);

    addVertex(1.0, 1.0, 1.0, 1.0,    textureSpan, winFrameUVSize);
    addVertex(1.0, 1.0, -1.0, 1.0,     textureSpan, 0.0);
    addVertex(-1.0, 1.0, -1.0, 1.0,    0.0, 0.0);

    numCoords = cursor;

    // Send vertices to GPU memory
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * numCoords, vertices, GL_STATIC_DRAW);
    checkGLError("glBufferData");
}

void BuildingWall::addVertex(float x, float y, float z, float w, float u, float v)
{
    if (cursor >= verticesSize)
        return;

    vertices[cursor + 0] = x;
    vertices[cursor + 1] = y;
    vertices[cursor + 2] = z;
    vertices[cursor + 3] = 1.0f;

    vertices[cursor + 4] = u;
    vertices[cursor + 5] = v;

    cursor += 6;
};

void BuildingWall::checkGLError(char *tag)
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
