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

#include "Vehicle.h"

void Vehicle::init()
{

}

void Vehicle::tick()
{
	setDeltaXYZ(yaw, pitch, 0.01 * throttle);
	move();
}

void Vehicle::ElevatorUp(float factor)
{
	pitch += factor;
	yaw += roll / 5000.0;
}

void Vehicle::ElevatorDown(float factor)
{
	pitch -= factor;
	yaw -= roll / 5000.0;
}

void Vehicle::AileronsLeft(float factor)
{
	MoveRoll(-factor * 5);
}

void Vehicle::AileronsRight(float factor)
{
	MoveRoll(factor * 5);
}

void Vehicle::increaseThrottle()
{
	if (throttle < 10.0)
		throttle += 0.001;
}

void Vehicle::decreaseThrottle()
{
	if (throttle > 0.0)
		throttle -= 0.001;

	if (throttle < 0.0)
		throttle = 0.0;
}

void Vehicle::rudderLeft()
{
	yaw -= 0.01;
	pitch += roll / 10000;
}

void Vehicle::rudderRight()
{
	yaw += 0.01;
	pitch -= roll / 10000;
}
