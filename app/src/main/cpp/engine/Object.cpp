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
#include "../DDLUtils.hpp"
#include "Object.h"

#ifdef PLATFORM_ANDROID
#include <cmath>
#include <cstdlib>
#endif

void Object::setDeltaXZ(float heading, float vel)
{
    delta.x = 0;
    delta.z = 0;

    if (vel < 0)
    {
        vel = -vel;
        heading -= 180.0;
    }

    while (heading < 0.0)
        heading = heading + 360.0;
    while (heading > 360.0)
        heading = heading - 360.0;

    if (heading == 0.0)
    {
        delta.x = 0;
        delta.z = -vel;
    }
    else if (heading == 180.0)
    {
        delta.x = 0;
        delta.z = vel;
    }
    else if (heading == 90.0)
    {
        delta.x = vel;
        delta.z = 0;
    }
    else if (heading == 270.0)
    {
        delta.x = -vel;
        delta.z = 0;
    }
    else if (heading >= 0.0 && heading < 90.0) {
        // x = opposite
        // y = adjacent
        // sin(head) = x / hyp
        // cos(head) = y / hyp

        delta.x = std::abs(vel * sin(glm::radians(heading)));
        delta.z = std::abs(vel * cos(glm::radians(heading)));

    } else if (heading >= 90.0 && heading < 180.0) {
        // y = opposite
        // x = adjacent
        // sin(head) = x / hyp
        // cos(head) = y / hyp

        delta.x = std::abs(vel * cos(glm::radians(heading - 90.0)));
        delta.z = -std::abs(vel * sin(glm::radians(heading - 90.0)));

    } else if (heading >= 180.0 && heading < 270.0) {
        // x = opposite
        // y = adjacent
        // sin(head) = x / hyp
        // cos(head) = y / hyp

        delta.x = -std::abs(vel * sin(glm::radians(heading - 180.0)));
        delta.z = -std::abs(vel * cos(glm::radians(heading - 180.0)));

    } else if (heading >= 270.0 && heading < 360.0) {
        // y = opposite
        // x = adjacent
        // sin(head) = x / hyp
        // cos(head) = y / hyp

        delta.x = -std::abs(vel * cos(glm::radians(heading - 270.0)));
        delta.z = std::abs(vel * sin(glm::radians(heading - 270.0)));
    }
}

void Object::setDeltaXYZ(float heading, float pitch, float vel)
{
	delta.x = 0;
	delta.y = 0;
	delta.z = 0;

	if (vel < 0)
	{
		vel = -vel;
		heading -= 180.0;
		pitch -= 180.0;
	}

	// Heading

	while (heading < 0.0)
		heading = heading + 360.0;
	while (heading > 360.0)
		heading = heading - 360.0;

	if (heading == 0.0)
	{
		delta.x = 0;
		delta.z = -vel;
	}
	else if (heading == 180.0)
	{
		delta.x = 0;
		delta.z = vel;
	}
	else if (heading == 90.0)
	{
		delta.x = vel;
		delta.z = 0;
	}
	else if (heading == 270.0)
	{
		delta.x = -vel;
		delta.z = 0;
	}
	else if (heading >= 0.0 && heading < 90.0) {
		// x = opposite
		// y = adjacent
		// sin(head) = x / hyp
		// cos(head) = y / hyp

		delta.x = std::abs(vel * sin(glm::radians(heading)));
		delta.z = -std::abs(vel * cos(glm::radians(heading)));

	}
	else if (heading >= 90.0 && heading < 180.0) {
		// y = opposite
		// x = adjacent
		// sin(head) = x / hyp
		// cos(head) = y / hyp

		delta.x = std::abs(vel * cos(glm::radians(heading - 90.0)));
		delta.z = std::abs(vel * sin(glm::radians(heading - 90.0)));

	}
	else if (heading >= 180.0 && heading < 270.0) {
		// x = opposite
		// y = adjacent
		// sin(head) = x / hyp
		// cos(head) = y / hyp

		delta.x = -std::abs(vel * sin(glm::radians(heading - 180.0)));
		delta.z = std::abs(vel * cos(glm::radians(heading - 180.0)));

	}
	else if (heading >= 270.0 && heading < 360.0) {
		// y = opposite
		// x = adjacent
		// sin(head) = x / hyp
		// cos(head) = y / hyp

		delta.x = -std::abs(vel * cos(glm::radians(heading - 270.0)));
		delta.z = -std::abs(vel * sin(glm::radians(heading - 270.0)));
	}

	// Pitch

	while (pitch < 0.0)
		pitch = pitch + 360.0;
	while (pitch> 360.0)
		pitch = pitch - 360.0;

	if (pitch == 0.0)
	{
		delta.y = 0;
	}
	else if (pitch == 180.0)
	{
		delta.y = 0;
	}
	else if (pitch == 90.0)
	{
		delta.y = vel;
	}
	else if (pitch == 270.0)
	{
		delta.y = -vel;
	}
	else if (pitch >= 0.0 && pitch < 90.0) {
		// x = opposite
		// y = adjacent
		// sin(head) = x / hyp
		// cos(head) = y / hyp

		delta.y = std::abs(vel * sin(glm::radians(pitch)));

	}
	else if (pitch >= 90.0 && pitch < 180.0) {
		// y = opposite
		// x = adjacent
		// sin(head) = x / hyp
		// cos(head) = y / hyp

		delta.y = std::abs(vel * cos(glm::radians(pitch - 90.0)));

	}
	else if (pitch >= 180.0 && pitch < 270.0) {
		// x = opposite
		// y = adjacent
		// sin(head) = x / hyp
		// cos(head) = y / hyp

		delta.y = -std::abs(vel * sin(glm::radians(pitch - 180.0)));

	}
	else if (pitch >= 270.0 && pitch < 360.0) {
		// y = opposite
		// x = adjacent
		// sin(head) = x / hyp
		// cos(head) = y / hyp

		delta.y = -std::abs(vel * cos(glm::radians(pitch - 270.0)));
	}
}

void Object::move()
{
	if (moveSmoothly)
		nextPosition += delta;
	else
		position += delta;
}

void Object::MoveBackward(float factor)
{
	float distance = moveDistance * factor;

	delta.y = 0;
	setDeltaXZ(-yaw, distance);
	move();
}

void Object::MoveForward(float factor)
{
	float distance = moveDistance * factor;

	delta.y = 0;
	setDeltaXZ(-yaw, -distance);
	move();
}

void Object::MoveLeft(float factor)
{
	float distance = moveDistance * factor;

	delta.y = 0;
	setDeltaXZ(-(yaw + 90), distance);
	move();
}

void Object::MoveRight(float factor)
{
	float distance = moveDistance * factor;

	delta.y = 0;
	setDeltaXZ(-(yaw - 90), distance);

	move();
}

void Object::MoveUp(float factor)
{
	float distance = moveDistance * factor;

	position.y += distance;
//	move();
}

void Object::MoveDown(float factor)
{
	float distance = moveDistance * factor;

	position.y -= distance;
//	move();
}

void Object::MoveYaw(float amount)
{
	float oldYaw = yaw;

	yaw += amount;

	if (yaw == 0.0 && oldYaw < 0.0)
		yaw = 0.1;

	if (yaw == 0.0 && oldYaw > 0.0)
		yaw = -0.1;
}

void Object::MovePitch(float amount)
{
//	if ((amount < 0 && pitch > -90) || (amount > 0 && pitch < 90))
		pitch += amount;
}

void Object::MoveRoll(float amount)
{
//	if ((amount < 0 && pitch > -90) || (amount > 0 && pitch < 90))
		roll += amount;
}

float Object::distanceTo(Object *other)
{
	if (other == nullptr)
		return 0;

	int deltaX = other->position.x - position.x;
	int deltaY = other->position.y - position.y;
	int deltaZ = other->position.z - position.z;

	return sqrt(deltaX * deltaX + deltaY * deltaY + deltaZ * deltaZ);
}

float Object::getYawTo(Object *dest)
{
/*	float result = 0;

	float deltax = position.x - dest->position.x;
	float deltaz = position.z - dest->position.z;

	result = ToDegrees(atan2(deltax, deltaz));*/

	float tox = dest->position.x;
	float toz = dest->position.z;

	double result = 0;

	double deltaX = 0;
	double deltaZ = 0;

	deltaX = std::abs(std::abs(tox) - std::abs(position.x));
	deltaZ = std::abs(std::abs(toz) - std::abs(position.z));

	double heading90 = 0;

	// Going north-east
	if (tox > position.x && toz > position.z) {
		heading90 = std::abs(ToDegrees(atan(deltaX / deltaZ)));
		result = heading90;
	}
	// Going south-east
	if (tox > position.x && toz < position.z) {
		heading90 = std::abs(ToDegrees(atan(deltaZ / deltaX)));
		result = 90 + heading90;
	}

	// Going south-west
	if (tox < position.x && toz < position.z) {
		heading90 = std::abs(ToDegrees(atan(deltaX / deltaZ)));
		result = 180 + heading90;
	}

	// Going north-west
	if (tox < position.x && toz > position.z) {
		heading90 = std::abs(ToDegrees(atan(deltaZ / deltaX)));
		result = 270 + heading90;
	}

	return result;
}

void Object::moveByVelocity()
{
	delta = velocity;
	move();
}

void Object::capVelocity()
{
	if (velocity.x >= maxVelocity)
		velocity.x = maxVelocity;
	if (velocity.y >= maxVelocity)
		velocity.y = maxVelocity;
	if (velocity.z >= maxVelocity)
		velocity.z = maxVelocity;

	if (velocity.x <= -maxVelocity)
		velocity.x = -maxVelocity;
	if (velocity.y <= -maxVelocity)
		velocity.y = -maxVelocity;
	if (velocity.z <= -maxVelocity)
		velocity.z = -maxVelocity;
}

void Object::moveTowardsNextPosition()
{
	glm::vec4 delta = nextPosition - position;

	position += delta / glm::vec4(4.0, 4.0, 4.0, 1.0);
}
