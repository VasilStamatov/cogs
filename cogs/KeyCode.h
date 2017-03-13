#ifndef KEYCODE_H
#define KEYCODE_H

#include <SDL\SDL_keycode.h>

namespace cogs
{
		/**
		* Enum class for all the supported key presses
		*/
		enum class KeyCode : unsigned int
		{
				UP = SDLK_UP,
				DOWN = SDLK_DOWN,
				RIGHT = SDLK_RIGHT,
				LEFT = SDLK_LEFT,
				SPACE = SDLK_SPACE,
				ESC = SDLK_ESCAPE,
				LSHIFT = SDLK_LSHIFT,
				RSHIFT = SDLK_RSHIFT,
				TAB = SDLK_TAB,
				Q = SDLK_q,
				W = SDLK_w,
				E = SDLK_e,
				R = SDLK_r,
				T = SDLK_t,
				Y = SDLK_y,
				U = SDLK_u,
				I = SDLK_i,
				O = SDLK_o,
				P = SDLK_p,
				A = SDLK_a,
				S = SDLK_s,
				D = SDLK_d,
				F = SDLK_f,
				G = SDLK_g,
				H = SDLK_h,
				J = SDLK_j,
				K = SDLK_k,
				L = SDLK_l,
				Z = SDLK_z,
				X = SDLK_x,
				C = SDLK_c,
				V = SDLK_v,
				B = SDLK_b,
				N = SDLK_n,
				M = SDLK_m,
				ALPHA0 = SDLK_0,
				ALPHA1 = SDLK_1,
				ALPHA2 = SDLK_2,
				ALPHA3 = SDLK_3,
				ALPHA4 = SDLK_4,
				ALPHA5 = SDLK_5,
				ALPHA6 = SDLK_6,
				ALPHA7 = SDLK_7,
				ALPHA8 = SDLK_8,
				ALPHA9 = SDLK_9,
				MOUSE0 = 99990,
				MOUSE1 = 99991,
				MOUSE2 = 99992
		};
}

#endif // !KEYCODE_H