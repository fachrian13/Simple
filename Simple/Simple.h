#ifndef _SIMPLE_
#define _SIMPLE_

namespace Simple
{
	namespace System
	{
		/// <summary>
		/// Warna dasar pada console.
		/// </summary>
		enum class Color
		{
			Black = 0,
			DarkBlue = 1,
			DarkGreen = 2,
			DarkCyan = 3,
			DarkRed = 4,
			DarkMagenta = 5,
			DarkYellow = 6,
			Gray = 7,
			DarkGray = 8,
			Blue = 9,
			Green = 10,
			Cyan = 11,
			Red = 12,
			Magenta = 13,
			Yellow = 14,
			White = 15
		};

		/// <summary>
		/// Struktur warna pada console.
		/// </summary>
		struct ConsoleColor
		{
			Color Background;
			Color Foreground;
		};
	}
}

#endif