#include "Timing.h"
#include "Utils.h"

namespace cogs
{
		namespace utils
		{
				/*********** High-resolution Timer ***********/

				HRTimer::HRTimer()
				{
						start();
				}

				HRTimer::~HRTimer()
				{
				}

				void HRTimer::start(bool _reset /*= true*/)
				{
						if (_reset)
						{
								m_start = HRTimePoint::clock::now();
						}
						if (!m_isActive)
						{
								m_isActive = true;
						}
				}

				void HRTimer::stop()
				{
						m_stop = HRTimePoint::clock::now();
						m_isActive = false;
				}

				float HRTimer::seconds() const
				{
						if (m_isActive)
						{
								float elapsedMilli = std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(HRTimePoint::clock::now() - m_start).count();
								return elapsedMilli / 1000.0f;
						}
						float elapsedMilli = std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(m_stop - m_start).count();
						return elapsedMilli / 1000.0f;
				}

				float HRTimer::milli() const
				{
						if (m_isActive)
						{
								return std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(HRTimePoint::clock::now() - m_start).count();
						}
						return std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(m_stop - m_start).count();
				}

				/*********** FPS Limiter ***********/

				FpsLimiter::FpsLimiter(float _maxFPS /* = 60.0f */) : m_maxFPS(_maxFPS)
				{
						m_desiredMillis = 1000.0f / m_maxFPS;
				}

				FpsLimiter::~FpsLimiter()
				{
				}

				void FpsLimiter::setMaxFPS(float _maxFPS)
				{
						m_maxFPS = _maxFPS;
						m_desiredMillis = 1000.0f / m_maxFPS;
				}

				//called at the start of the frame
				void FpsLimiter::beginFrame()
				{
						m_startTicks = getTime();
						//m_timer.start();
				}

				//called at the end of the frame
				float FpsLimiter::endFrame()
				{
						calculateFPS();

						float frameTicks = getTime() - m_startTicks;
						//check if the time it took this frame to be completed is lower than the desired frame time
						if (m_desiredMillis > frameTicks)
						{
								//if the frame was completed too fast, delay it to limit the fps
								sleep(m_desiredMillis - frameTicks);
						}

						//if (m_desiredMillis > m_timer.milli())
						//{
						//		//if the frame was completed too fast, delay it to limit the fps
						//		sleep(m_desiredMillis - m_timer.milli());
						//}
						//m_timer.stop();

						//m_deltaTime = m_timer.milli() / 16.0f; //keep it close to 1 
						//m_fps = 1.0f / m_timer.seconds();
						
						return m_fps;
				}

				void FpsLimiter::calculateFPS()
				{
							//The number of frames to average
						static const int NUM_SAMPLES = 10;
						//Stores all the frametimes for each frame that we will average
						static float frameTimes[NUM_SAMPLES];
						//The current frame we are on
						static int currentFrame = 0;
						//the ticks of the previous frame
						static float previousTicks = getTime();
						//Ticks for the current frame
						float currentTicks = getTime();

						//Calculate the number of ticks (ms) for this frame
						float frameTime = currentTicks - previousTicks;

						m_deltaTime = frameTime / m_desiredMillis;
						//currentFrame % NUM_SAMPLES -> makes sure that current frame isn't higher then the number of samples
						frameTimes[currentFrame % NUM_SAMPLES] = frameTime;
						//current ticks is now previous ticks
						previousTicks = currentTicks;

						int count = 0;
						currentFrame++;

						if (currentFrame < NUM_SAMPLES)
						{
								count = currentFrame;
						}
						else
						{
								count = NUM_SAMPLES;
						}
						//Average all the frame times
						float frameTimeAvg = 0;
						for (int i = 0; i < count; i++)
						{
								frameTimeAvg += frameTimes[i];
						}
						frameTimeAvg /= count;
						//Calculate FPS
						if (frameTimeAvg > 0)
						{
								m_fps = 1000.0f / frameTimeAvg;
						}
						else
						{
								m_fps = 60.0f;
						}
						if (m_deltaTime == 0.0f)
						{
								m_deltaTime = 1.0f;
						}
				}
		}
}