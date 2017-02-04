#ifndef COLOR_H
#define COLOR_H

namespace cogs
{
		namespace graphics
		{
				using ubyte = unsigned char;

				struct Color
				{
						/**
						* A bunch of static colors for ease of creation
						*/
						static Color white;
						static Color black;
						static Color red;
						static Color green;
						static Color blue;
						static Color silver;
						static Color gray;
						static Color yellow;
						static Color olive;
						static Color lime;
						static Color aqua;
						static Color teal;
						static Color navy;
						static Color fuchsia;
						static Color purple;

						/**
						* default constructor
						*/
						Color() { }

						/**
						* constructor with the ability to set all values
						*/
						Color(ubyte _r, ubyte _g, ubyte _b, ubyte _a);

						/**
						* constructor with the ability to set rgb with the same value and seperate alpha
						*/
						Color(ubyte _rgb, ubyte _alpha);

						/**
						* Copy constructor
						*/
						Color(const Color& _other);

						ubyte r{ 0 };
						ubyte g{ 0 };
						ubyte b{ 0 };
						ubyte a{ 0 };
				};
		}
}

#endif // !COLOR_H