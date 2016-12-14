#include "Utils.h"

#include <SDL\SDL_timer.h>
#include <FreeImage\FreeImage.h>


namespace cogs
{
		namespace utils
		{
				float getTime()
				{
						return static_cast<float>(SDL_GetTicks());
				}

				void sleep(const float _millis)
				{
						SDL_Delay(static_cast<Uint32>(_millis));
				}
		}
}