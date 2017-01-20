#ifndef RANDOM_H
#define RANDOM_H

#include <random>
#include <chrono>

namespace cogs
{
		namespace utils
		{
				using HighResClock = std::chrono::high_resolution_clock;

				/**
				* Static class for random number generation
				*/
				class Random
				{
				public:
						/**
						* Generate a random integer between [min, max]
						*/
						static int getRandInt(int _min, int _max);

						/**
						* Generate a random float between [min, max]
						*/
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