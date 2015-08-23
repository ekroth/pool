/* Copyright (c) 2010-2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#ifndef KKE_GAMETIME_HPP
#define KKE_GAMETIME_HPP

#include "Interface/INoncopyable.hpp"

namespace kke
{
	class GameTime
	{
	public:
		GameTime();
		
		/**
		 * @brief Total elapsed time since start.
		 *
		 * @return double
		 **/
		double TotalElapsed() const;
		
		/**
		 * @brief Elapsed time since last update.
		 *
		 * @return double
		 **/
		double Elapsed() const;
		
		/**
		 * @brief FPS this frame.
		 *
		 * @return double
		 **/
		float Fps() const;
		
		/**
		 * @brief Average FPS, depending on @SetAvgFpsFrames.
		 *
		 * @return float
		 **/
		float AverageFps() const;
		
		/**
		 * @brief Average FPS since start.
		 *
		 * @return float
		 **/
		float TotalFps() const;
		
		
		/**
		 * @brief Frames elapsed since start.
		 *
		 * @return const long unsigned int&
		 **/
		const long unsigned int &Frames() const;  
		
		/**
		 * @brief If FPS calculation status.
		 *
		 * @return bool Enabled/Disabled.
		 **/
		bool FpsCalculation() const;
		
		/**
		 * @brief Frames per second calculation.
		 *
		 * @param calc ...
		 * @return void
		 **/
		void SetCalcFps(bool calc);
		
		/**
		 * @brief How many frames that may elapse before recalculating average FPS.
		 *
		 * @param frames ...
		 * @return void
		 **/
		void SetAvgFpsFrames(int frames);
		
		/**
		 * @brief Updates the time class.
		 *
		 * @param totalElapsed Total time elapsed.
		 * @return void
		 **/
		void Update(double totalElapsed);
		
	public:
		
		/**
		 * @brief Total elapsed time (glfwGetTime())
		 *
		 * @return float
		 **/
		static float GetTime();
		
	private:
		double totalElapsed,
				currentElapsed;
				
		float fps,
			avgFps,
			totalAvgFps;
			
		double avgFpsTimer;
		int avgFpsFrames,
			avgFpsCounter;

		long unsigned int frames;
		
		bool calcFps;
	};
}

#endif // KKE_GAMETIME_HPP
