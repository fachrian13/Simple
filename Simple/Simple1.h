#ifndef _SIMPLE1_
#define _SIMPLE1_

#include <iostream>
#include <sstream>
#include <conio.h>

namespace Simple1
{
	namespace System
	{
		enum class Color : uint8_t
		{
			Black = 30,
			Red = 31,
			Green = 32,
			Yellow = 33,
			Blue = 34,
			Magenta = 35,
			Cyan = 36,
			White = 37,
			Default = 39,
			BrightBlack = 90,
			BrightRed = 91,
			BrightGreen = 92,
			BrightYellow = 93,
			BrightBlue = 94,
			BrightMagenta = 95,
			BrightCyan = 96,
			BrightWhite = 97
		};

		struct ConsoleColor
		{
			Color Background = Color::Default;
			Color Foreground = Color::Default;

			friend std::ostream& operator<<(std::ostream& out, ConsoleColor& value)
			{
				return out << "\033[" << static_cast<uint8_t>(value.Foreground) + 0 << ";" << static_cast<uint8_t>(value.Background) + 10 << "m";
			}

			operator std::string const () const
			{
				return "\033[" + std::to_string(static_cast<uint8_t>(Foreground)) + ";" + std::to_string(static_cast<uint8_t>(Background) + 10) + "m";
			}
		};

		struct ConsoleCursor
		{
			short X;
			short Y;

			friend std::ostream& operator<<(std::ostream& out, const ConsoleCursor& value)
			{
				return out << "\033[" << value.Y << ";" << value.X << "H";
			}

			operator std::string const () const
			{
				return "\033[" + std::to_string(Y) + ";" + std::to_string(X) + "H";
			}
		};

		class Console
		{
		public:
			template<class... T>
			static void Write(T... value)
			{
				((std::cout << value), ...);
			}
			
			template<class... T>
			static void WriteLine(T... value)
			{
				((std::cout << value), ...) << "\n";
			}

			static char Read()
			{
				return std::cin.get();
			}

			static char ReadKey()
			{
				return _getch();
			}

			static std::string ReadLine()
			{
				std::string line;

				std::getline(std::cin, line);
				return line;
			}
		};
	}
}

#endif