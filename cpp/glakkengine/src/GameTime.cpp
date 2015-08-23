/* Copyright (c) 2010-2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#include "GameTime.hpp"

#include <GL/glfw.h>

using kke::GameTime;

GameTime::GameTime() : 
  totalElapsed(0),
  currentElapsed(0),
  fps(0),
  avgFps(0),
  totalAvgFps(0),
  avgFpsFrames(10),
  avgFpsCounter(0),
  frames(0),
  calcFps(false)
{

}

double GameTime::TotalElapsed() const
{
	return totalElapsed;
}

double GameTime::Elapsed() const
{
	return currentElapsed;
}

float GameTime::Fps() const
{
	return fps;
}

float GameTime::AverageFps() const
{
	return avgFps;
}

float GameTime::TotalFps() const
{
	return totalAvgFps;
}
  
const long unsigned int &GameTime::Frames() const
{
	return frames;
}
  
bool GameTime::FpsCalculation() const
{
	return calcFps;
}

// Static
float GameTime::GetTime()
{
	return glfwGetTime();
}

void GameTime::SetCalcFps(bool calc)
{
	calcFps = calc;
}

void GameTime::SetAvgFpsFrames(int frames)
{
	avgFpsFrames = frames;
}

void GameTime::Update(double theTotalElapsed)
{
	currentElapsed = theTotalElapsed - totalElapsed;
	totalElapsed = theTotalElapsed;
	
	if (calcFps)
	{
		fps = 1.0 / currentElapsed;
		frames++;
		
		if (++avgFpsCounter >= avgFpsFrames)
		{		
			avgFps = (totalElapsed - avgFpsCounter) / (totalElapsed - avgFpsTimer);
			avgFpsCounter = 0;
			avgFpsTimer = totalElapsed;
			totalAvgFps = frames / totalElapsed;
		}
	}
}