#ifndef RANDOM_H
#define RANDOM_H

#include <random>
#include <chrono>

namespace cogs
{
		namespace utils
		{
				using HighResClock = std::chrono::high_resolution_clock;

				class Random
				{
				public:
						static int getRandInt(int _min, int _max);
						static float getRandFloat(float _min, float _max);

				private:
						Random() {}
						~Random() {}

				private:
						static std::mt19937 m_generator;
						static bool m_seeded;
				};
		}

}
#endif // !RANDOM_H