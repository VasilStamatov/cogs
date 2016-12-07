#ifndef TIMING_H
#define TIMING_H

#include <chrono>

namespace cogs
{
		namespace utils
		{
				class HRTimer
				{
				public:
						HRTimer();
						~HRTimer();

						/**
								* /brief starts the clock (sets the flag to true) and resets it unless explicitly set not to
								*/
						void start(bool _reset = true);

						/**
								* /brief sets the stop time point to now() and turns the active flag to false
								*/
						void stop();

						/** 
								* /brief returns the elapsed time in seconds
								* (if the timer has been stopped it returns from the start to stop time)
								*/
						float seconds() const;

						/**
								* /brief returns the elapsed time in milliseconds
								* (if the timer has been stopped it returns from the start to stop time) 
						  */
						float milli() const;

						/**
								* /brief Returns the flag if the timer is active
								*/
						const bool& isActive() const noexcept { return m_isActive; }

				private:
						using HRTimePoint = std::chrono::time_point<std::chrono::high_resolution_clock>;

						HRTimePoint m_start;
						HRTimePoint m_stop;

						bool m_isActive{ false };
				};

				class FpsLimiter
				{
				public:
						FpsLimiter(float _maxFPS = 60.0f);
						~FpsLimiter();

						// Sets the desired max FPS
						void setMaxFPS(float _maxFPS);

						//set the startTicks to the current frame ticks
						void beginFrame();

						// end() will return the current FPS as a float
						float endFrame();

						float fps()						 const noexcept	{ return m_fps; }
						float deltaTime() const noexcept { return m_deltaTime; }

				private:
						float m_fps									 { 0.0f }; ///< the current fps the app is running at
						float m_deltaTime				{ 0.0f }; ///< the current deltaTime the app is running at
						float m_desiredMillis{ 0.0f }; ///< the desired milliseconds
						HRTimer m_timer; ///< the timer 
				};
		}
}

#endif // !TIMING_H
