#include "Random.h"

namespace cogs
{
		namespace utils
		{
				std::mt19937 Random::m_generator;
				bool Random::m_seeded = false;

				int Random::getRandInt(int _min, int _max)
				{
						if (!m_seeded)
						{
								m_generator.seed(HighResClock::now().time_since_epoch().count());
								m_seeded = true;
						}
						std::uniform_int_distribution<int> intDis(_min, _max);
						return intDis(m_generator);
				}

				float Random::getRandFloat(float _min, float _max)
				{
						if (!m_seeded)
						{
								m_generator.seed(HighResClock::now().time_since_epoch().count());
								m_seeded = true;
						}
						std::uniform_real_distribution<float> realDis(_min, _max);
						return realDis(m_generator);
				}
		}
}

