#include "Color.h"

namespace cogs
{
		namespace graphics
		{
				/**
				* A bunch of premade colors for ease of access
				* codes from http://htmlcolorcodes.com/
				*/
				Color Color::white		 = Color(255, 255, 255, 255);
				Color Color::black	  = Color(0, 0, 0, 255);
				Color Color::red				 = Color(255, 0, 0, 255);
				Color Color::green		 = Color(0, 128, 0, 255);
				Color Color::blue	   = Color(0, 0, 255, 255);
				Color Color::silver  = Color(192, 192, 192, 255);
				Color Color::gray			 = Color(128, 128, 128, 255);   
				Color Color::yellow  = Color(255, 255, 0, 255);
				Color Color::olive	  = Color(128, 128, 255, 255);
				Color Color::lime    = Color(0, 255, 0, 255);
				Color Color::aqua    = Color(0, 255, 255, 255);
				Color Color::teal    = Color(0, 128, 128, 255);
				Color Color::navy    = Color(0, 0, 128, 255);
				Color Color::fuchsia = Color(255, 0, 255, 255);
				Color Color::purple  = Color(128, 0, 128, 255);

				Color::Color(byte _r, byte _g, byte _b, byte _a) : r(_r), g(_g), b(_b), a(_a) { }

				Color::Color(byte _rgb, byte _alpha) : r(_rgb), g(_rgb), b(_rgb), a(_alpha) { }

				Color::Color(const Color & _other) : r(_other.r), g(_other.g), b(_other.b), a(_other.a)
				{
				}
		}
}