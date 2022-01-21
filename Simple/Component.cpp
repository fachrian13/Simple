#include <conio.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

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
		std::stringstream ss;

		ss << "\033[" << static_cast<int>(this->Foreground) + 0 << ";" << static_cast<int>(this->Background) + 10 << "m";
		return ss.str();
	}

	std::string ToString() const
	{
		std::stringstream ss;

		ss << "\033[" << static_cast<int>(this->Foreground) + 0 << ";" << static_cast<int>(this->Background) + 10 << "m";
		return ss.str();
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
		std::stringstream ss;

		ss << "\033[" << static_cast<int>(this->Y) << ";" << static_cast<int>(this->X) << "H";
		return ss.str();
	}

	std::string ToString() const
	{
		std::stringstream ss;

		ss << "\033[" << static_cast<int>(this->Y) << ";" << static_cast<int>(this->X) << "H";
		return ss.str();
	}
};

class ConsoleMenu
{
private:
	std::vector<std::string> Menu;
	Coordinate Position;
	size_t MaxLength;
	bool Fill;
	struct
	{
		int Begin;
		int Current;
		int End;
	} Index;

private:
	std::string Render(int limit)
	{
		std::stringstream ss;
		int indexLimit = limit + this->Index.Begin;

		if (this->Fill)
			for (int i = this->Index.Begin, y = this->Position.Y; i < indexLimit; i++, y++)
				this->Index.Current == i ? ss << Coordinate{ this->Position.X, y }.ToString() << ConsoleColor{ Color::White, Color::Black }.ToString() << this->Menu[i] << std::string(this->MaxLength - this->Menu[i].length(), ' ') << ConsoleColor{ Color::Default, Color::Default }.ToString() : ss << Coordinate{ this->Position.X, y } << this->Menu[i] << std::string(this->MaxLength - this->Menu[i].length(), ' ');
		else
			for (int i = this->Index.Begin, y = this->Position.Y; i < indexLimit; i++, y++)
				this->Index.Current == i ? ss << Coordinate{ this->Position.X, y }.ToString() << ConsoleColor{ Color::White, Color::Black }.ToString() << this->Menu[i] << ConsoleColor{ Color::Default, Color::Default }.ToString() << std::string(this->MaxLength - this->Menu[i].length(), ' ') : ss << Coordinate{ this->Position.X, y } << this->Menu[i] << std::string(this->MaxLength - this->Menu[i].length(), ' ');
		return ss.str();
	}

public:
	ConsoleMenu(std::initializer_list<std::string> menu, Coordinate position, bool fill = false) : Position(position), MaxLength(0), Fill(fill)
	{
		this->Index = { 0, 0, static_cast<int>(menu.size() - 1) };
		
		for (const auto& i : menu)
			this->MaxLength = i.length() > this->MaxLength ? i.length() : this->MaxLength;
	}

	void Run(int limit)
	{
		char ch;

		do
		{
			std::cout << this->Render(limit);

			ch = _getch();

			switch (ch)
			{
			case 'j':
				if (this->Index.Current != this->Index.End)
				{
					this->Index.Current++;
					if (this->Index.Current >= limit)
						this->Index.Begin++;
				}
				break;
			case 'k':
				if (this->Index.Current != 0)
				{
					this->Index.Current--;
					if (this->Index.Current < this->Index.Begin)
						this->Index.Begin--;
				}
				break;
			}
		} while (ch != '\r');
	}


};



int main()
{
	ConsoleMenu a
	(
		{
			"Helasdfasdforld 1",
			"Hello World 2",
			"Hello Woradfasdfsdfld 3",
			"Hello Wasddorld 4",
			"Hello Woasdfrld 5",
			"Hello Worlsddddddd 6",
			"Hello Woasdfasdfrld 7",
			"Hello Worasdfasdddadfld 8"

		},
		{ 3, 3 }
	);

	a.Run(5);
}