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
				
				float FpsLimiter::m_fps{ 0.0f };
				float FpsLimiter::m_deltaTime{ 0.0f };
				float FpsLimiter::m_desiredMillis{ 0.0f };
				HRTimer FpsLimiter::m_timer;

				FpsLimiter::FpsLimiter() { }

				FpsLimiter::~FpsLimiter() { }

				void FpsLimiter::setMaxFPS(float _maxFPS)
				{
						m_desiredMillis = 1000.0f / _maxFPS;
				}

				//called at the start of the frame
				void FpsLimiter::beginFrame()
				{
						m_timer.start();
				}

				//called at the end of the frame
				float FpsLimiter::endFrame()
				{
						if (m_desiredMillis > m_timer.milli())
						{
								//if the frame was completed too fast, delay it to limit the fps
								sleep(m_desiredMillis - m_timer.milli());
						}
						m_timer.stop();

						m_deltaTime = m_timer.milli() / m_desiredMillis; //keep it close to 1 
						m_fps = 1.0f / m_timer.seconds();
						
						return m_fps;
				}
		}
}