#include "Timing.h"
#include "Utils.h"

namespace cogs
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

		FpsLimiter::FpsLimiter(float _maxFPS) { setMaxFPS(_maxFPS); }

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

				m_deltaTime = m_timer.seconds();
				m_fps = 1.0f / m_deltaTime;

				return m_fps;
		}
}