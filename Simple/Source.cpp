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

	operator std::string const () const
	{
		std::stringstream ss;

		ss << "\033[" << std::to_string(static_cast<int>(this->Foreground) + 0) << ";" << std::to_string(static_cast<int>(this->Background) + 10) << "m";
		return ss.str();
	}

	std::string ToString()
	{
		std::stringstream ss;

		ss << "\033[" << std::to_string(static_cast<int>(this->Foreground) + 0) << ";" << std::to_string(static_cast<int>(this->Background) + 10) << "m";
		return ss.str();
	}
};

struct Coordinate
{
	int X;
	int Y;

	friend std::ostream& operator<<(std::ostream& out, const Coordinate& value)
	{
		out << "\033[" << value.Y + 1 << ";" << value.X + 1 << "H";
		return out;
	}

	operator std::string const () const
	{
		std::stringstream ss;

		ss << "\033[" << std::to_string(this->Y + 1) << ";" << std::to_string(this->X + 1) << "H";
		return ss.str();
	}

	std::string ToString()
	{
		std::stringstream ss;

		ss << "\033[" << std::to_string(this->Y + 1) << ";" << std::to_string(this->X + 1) << "H";
		return ss.str();
	}
};

class ConsoleMenu
{
private:
	std::vector<std::string> Menu;

	size_t Max;

	bool Fill;

	struct
	{
		int Begin;
		int Current;
		int End;
	} Index, Cursor;

private:
	std::string Render(int limit)
	{
		std::vector<std::string> temp;
		std::stringstream result;

		Cursor.End = Cursor.Begin + limit - 1;

		for (int i = 0, y = Cursor.Begin, index = Index.Begin; i < limit; i++, y++, index++)
			temp.push_back(Coordinate{ 0, y }.ToString() + Menu[index] + std::string(Max - Menu[index].size(), ' '));

		if (Fill)
			temp[Cursor.Current] = ConsoleColor{ Color::White, Color::Black }.ToString() + Coordinate{ 0, Cursor.Current }.ToString() + Menu[Index.Current] + std::string(Max - Menu[Index.Current].size(), ' ') + ConsoleColor{ Color::Default, Color::Default }.ToString();
		else
			temp[Cursor.Current] = ConsoleColor{ Color::White, Color::Black }.ToString() + Coordinate{ 0, Cursor.Current }.ToString() + Menu[Index.Current] + ConsoleColor{ Color::Default, Color::Default }.ToString() + std::string(Max - Menu[Index.Current].size(), ' ');

		std::copy(temp.begin(), temp.end(), std::ostream_iterator<std::string>(result));
		return result.str();
	}

public:
	ConsoleMenu(std::initializer_list<std::string> menu, bool fill = false) : Menu(menu), Max(0), Fill(fill)
	{
		Index = Cursor = { 0, 0, static_cast<int>(menu.size() - 1) };

		for (const auto& i : menu)
			Max = i.size() > Max ? i.size() : Max;
	}

	void Run()
	{
		char ch;

		do
		{
			std::cout << Render(5);

			ch = _getch();

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
	}
};

int main()
{
	ConsoleMenu mDashboard
	{
		{
			"1 laskdjf;alsdjf;",
			"2 sodufgbsktn",
			"3 dfjgnskjdtnh",
			"4 adrogujnsdklfjhnglkdsf",
			"5 sdkfjhnsrktlhbrfgt",
			"6 sdkjfghndkflgnh",
			"7 sdfkjgthndlfkgjhn",
			"8 sdglkjhndrtynh"
		},
		true
	};

	mDashboard.Run();
}

class Entity;

class Component
{
private:

public:
	Component() {}

	virtual ~Component() {}

	Entity* Entity;

	virtual bool Init() { return true; }

	virtual std::string Draw() {}

	virtual std::string Update() {}
};

class Entity
{
private:
	std::vector<std::unique_ptr<Component>> Components;

public:
	Entity() {}

	virtual ~Entity() {}

	template<class T, class... TArgs>
	inline T& AddComponent(TArgs... component)
	{
		T* comp(new T(std::forward<TArgs>(component...)));
		std::unique_ptr<Component> uPtr{ comp };

		this->Components.emplace_back(std::move(uPtr));
		comp->Entity = this;

	}
};