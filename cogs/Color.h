#ifndef COLOR_H
#define COLOR_H

namespace cogs
{
		namespace graphics
		{
				using byte = unsigned char;

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
						Color(byte _r, byte _g, byte _b, byte _a);

						/**
						* constructor with the ability to set rgb with the same value and seperate alpha
						*/
						Color(byte _rgb, byte _alpha);

						/**
						* Copy constructor
						*/
						Color(const Color& _other);

						bool operator==(const Color& _rhs) const { return (r == _rhs.r && g == _rhs.g && b == _rhs.b && a == _rhs.a); }

						byte r{ 0 };
						byte g{ 0 };
						byte b{ 0 };
						byte a{ 0 };
				};
		}
}

#endif // !COLOR_H