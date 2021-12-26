#pragma once

#include <conio.h>
#include <iostream>
#include <string>
#include <vector>
#include <windows.h>

namespace Simple
{
	namespace System
	{
		template<class T>
		class Property
		{
		protected:
			T Value;

		public:
			virtual ~Property() {}
			virtual T& operator=(const T& value) { return this->Value = value; }
			virtual operator T () const { return this->Value; }
		};

		template<class T>
		class WriteOnlyProperty
		{
		protected:
			T Value;

		public:
			virtual ~WriteOnlyProperty() {}
			virtual T& operator=(const T& value) { return this->Value = value; }
		};

		template<class T>
		class ReadOnlyProperty
		{
		protected:
			T Value;

		public:
			virtual ~ReadOnlyProperty() {}
			virtual operator T () const { return this->Value; }
		};

#define SET(T) T& operator=(const T& value) override
#define GET(T) operator T () const override

		enum class Color
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
			Gray = 90,
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
			Color Background;
			Color Foreground;

			friend std::ostream& operator<<(std::ostream& out, const ConsoleColor& value)
			{
				out << "\033[" << static_cast<int>(value.Foreground) + 0 << ";" << static_cast<int>(value.Background) + 10 << "m";
				return out;
			}

			operator std::string const& () const
			{
				return "\033[" + std::to_string(static_cast<int>(this->Foreground) + 0) + ";" + std::to_string(static_cast<int>(this->Background) + 10) + "m";
			}
		};

		struct Coordinate
		{
			int X;
			int Y;

			friend std::ostream& operator<<(std::ostream& out, const Coordinate& value)
			{
				out << "\033[" << value.Y << ";" << value.X << "H";
				return out;
			}

			operator std::string const& () const
			{
				return "\033[" + std::to_string(this->Y) + ";" + std::to_string(this->X) + "H";
			}
		};

		class Console final
		{
		public:
			inline static class : public WriteOnlyProperty<bool>
			{
			public:
				SET(bool)
				{
					if (value)
						Write("\033[?12h");
					else
						Write("\033[?12l");
					return this->Value = value;
				}
			} CursorBlinking;

			inline static class : public WriteOnlyProperty<bool>
			{
			public:
				SET(bool)
				{
					if (value)
						Write("\033[?25h");
					else
						Write("\033[?25l");
					return this->Value = value;
				}
			} CursorVisible;

		public:
			template<class... T>
			static void Write(T... value)
			{
				((std::cout << value), ...);
			}

			template<class... T>
			static void WriteLine(T... value)
			{
				Write(value..., "\n");
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

			static Coordinate GetSize()
			{
				HANDLE outputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
				CONSOLE_SCREEN_BUFFER_INFO csbi;
				GetConsoleScreenBufferInfo(outputHandle, &csbi);
				return
				{
					csbi.srWindow.Right - csbi.srWindow.Left + 1,
					csbi.srWindow.Bottom - csbi.srWindow.Top + 1
				};
			}
		};
	}

	namespace Utility
	{
		class Tools final
		{
		public:
			static void Clear(std::string& variable)
			{
				EraseCharacter(static_cast<int>(variable.size()));
				variable.clear();
			}

			static void EraseCharacter(int length)
			{
				System::Console::Write("\033[", length, "X");
			}

			static void EraseCharacter(System::Coordinate position, int length)
			{
				System::Console::Write(position, "\033[", length, "X");
			}
		};

		class ConsoleMenu final
		{
		private:
			int Limit;

			System::Coordinate Position;
			System::ConsoleColor Color;

			struct
			{
				int Begin;
				int Current;
				int End;
			} Index, Cursor;

			struct
			{
				std::vector<std::string> Front;
				std::vector<std::string> Back;
			} Menu;

		public:
			struct
			{
				class : public System::ReadOnlyProperty<int> { friend class ConsoleMenu; } Index;
				class : public System::ReadOnlyProperty<const char*> { friend class ConsoleMenu; } Value;
			} Selected;

			ConsoleMenu(std::initializer_list<std::string> menu, System::Coordinate position, System::ConsoleColor color, bool fill = false) : Menu{ menu, menu }, Position(position), Color(color)
			{
				if (fill)
				{
					std::size_t max = 0;

					for (const auto& index : this->Menu.Front) max = index.size() > max ? index.size() : max;
					for (auto& index : this->Menu.Front)index += std::string(max - index.size(), ' ');
				}

				this->Index = { 0, 0, static_cast<int>(this->Menu.Front.size() - 1) };
				this->Cursor = { this->Position.Y, this->Position.Y, this->Index.End + this->Position.Y + 1 };
				this->Limit = 0;
				this->Selected.Index.Value = 0;
				this->Selected.Value.Value = "";
			}

			void Clear()
			{
				int y = this->Position.Y;
				int index = this->Index.Begin;

				for (int i = 0; i < this->Limit; i++, y++, index++)
					Tools::EraseCharacter({ this->Position.X, y }, static_cast<int>(this->Menu.Front[index].size()));
			}

			void Run()
			{
				int ySize = System::Console::GetSize().Y;
				int last = static_cast<int>(this->Menu.Front.size());

				Run(last < ySize ? last : ySize);
			}

			void Run(int limit)
			{
				char ch;

				this->Limit = limit;
				this->Cursor.End = this->Position.Y + limit - 1;

				System::Console::CursorVisible = false;
				do
				{
					short y = this->Position.Y;
					short index = this->Index.Begin;

					for (int i = 0; i < limit; i++, y++, index++)
						System::Console::Write(System::Coordinate{ this->Position.X, y }, this->Menu.Front[index]);

					System::Console::Write(System::Coordinate{ this->Position.X, this->Cursor.Current }, this->Color, this->Menu.Front[this->Index.Current], System::ConsoleColor{ System::Color::Default, System::Color::Default });
					ch = System::Console::ReadKey();
					System::Console::Write(System::Coordinate{ this->Position.X, this->Cursor.Current }, this->Menu.Front[this->Index.Current]);

					if (ch == 'j')
					{
						if (this->Cursor.Current != this->Cursor.End)
							this->Cursor.Current++, this->Index.Current++;
						else if (this->Index.Current < this->Index.End)
							this->Index.Begin++, this->Index.Current++;
					}
					else if (ch == 'k')
					{
						if (this->Cursor.Current != this->Cursor.Begin)
							this->Cursor.Current--, this->Index.Current--;
						else if (this->Index.Current > 0)
							this->Index.Begin--, this->Index.Current--;
					}
				} while (ch != '\r');
				System::Console::CursorVisible = true;
				this->Selected.Index.Value = this->Index.Current;
				this->Selected.Value.Value = this->Menu.Back[this->Index.Current].c_str();
			}
		};
	}
}