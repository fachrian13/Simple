#include <conio.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "Simple.h"

using namespace Simple;

class ConsoleMenu
{
private:
	std::vector<std::string> Menu;
	System::Coordinate Position;
	size_t MaxLength;
	bool Fill;
	int Limit;
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
		this->Limit = limit;

		if (this->Fill)
			for (int i = this->Index.Begin, y = this->Position.Y; i < indexLimit; i++, y++)
				this->Index.Current == i ? ss << System::Coordinate{ this->Position.X, y }.ToString() << System::ConsoleColor{ System::Color::White, System::Color::Black }.ToString() << this->Menu[i] << std::string(this->MaxLength - this->Menu[i].length(), ' ') << System::ConsoleColor{ System::Color::Default, System::Color::Default }.ToString() : ss << System::Coordinate{ this->Position.X, y } << this->Menu[i] << std::string(this->MaxLength - this->Menu[i].length(), ' ');
		else
			for (int i = this->Index.Begin, y = this->Position.Y; i < indexLimit; i++, y++)
				this->Index.Current == i ? ss << System::Coordinate{ this->Position.X, y }.ToString() << System::ConsoleColor{ System::Color::White, System::Color::Black }.ToString() << this->Menu[i] << System::ConsoleColor{ System::Color::Default, System::Color::Default }.ToString() << std::string(this->MaxLength - this->Menu[i].length(), ' ') : ss << System::Coordinate{ this->Position.X, y } << this->Menu[i] << std::string(this->MaxLength - this->Menu[i].length(), ' ');
		return ss.str();
	}

public:
	class : public System::ReadOnlyProperty<bool> { friend class ConsoleMenu; } Running;

	struct
	{
		class : public System::ReadOnlyProperty<int> { friend class ConsoleMenu; } Index;
		class : public System::ReadOnlyProperty<const char*> { friend class ConsoleMenu; } Value;
	} Selected;

public:
	ConsoleMenu(std::initializer_list<std::string> menu, System::Coordinate position, bool fill = false) : Menu(menu), Position(position), MaxLength(0), Fill(fill)
	{
		for (const auto& i : menu)
			this->MaxLength = i.length() > this->MaxLength ? i.length() : this->MaxLength;
		this->Index = { 0, 0, static_cast<int>(menu.size() - 1) };
		this->Selected.Index.Value = 0;
		this->Selected.Value.Value = "";
		this->Running.Value = true;
	}

	void Clear()
	{
		int y = this->Position.Y;
		std::string fill(this->MaxLength, ' ');
		std::stringstream ss;

		for (int i = 0; i < this->Limit; i++, y++)
			ss << System::Coordinate{ this->Position.X, y } << fill;

		System::Console::Write(ss.str());
	}

	void Run(int limit)
	{
		char ch;

		this->Running.Value = true;
		System::Console::CursorVisible = false;

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

		System::Console::CursorVisible = true;
		this->Selected.Index.Value = this->Index.Current;
		this->Selected.Value.Value = this->Menu[this->Index.Current].c_str();
	}

	void Run()
	{
		int y = System::Console::GetBufferSize().Y - Position.Y + 1;
		int size = static_cast<int>(this->Menu.size());

		Run(size < y ? size : y);
	}

	void Stop()
	{
		this->Running.Value = false;
	}
};

int main()
{
	ConsoleMenu list
	{
		{
			"1 Amelia Noah",
			"2 Emma Liam",
			"3 Olivia Oliver",
			"4 Ava Elijah",
			"5 Luna Mateo",
			"6 Mia Lucas",
			"7 Isabella James",
			"8 Charlotte Levi",
			"9 Sophia Grayson",
			"10 Ella Asher",
			"11 Harper Sebastian",
			"12 Sofia Alexander",
			"13 Gianna Daniel",
			"14 Aurora Wyatt",
			"15 Scarlett Luca",
			"16 Nova William",
			"17 Violet Michael",
			"18 Evelyn Muhammad",
			"19 Zoey Kai",
			"20 Elizabeth Logan",
			"21 Hazel Ethan",
			"22 Avery Jaxon",
			"23 Willow Aiden",
			"24 Layla Ezekiel",
			"25 Camila Waylon",
			"26 Ellie Josiah",
			"27 Penelope Jackson",
			"28 Lily Ezra",
			"29 Isla Carter",
			"30 Elena Owen",
			"31 Emilia Anthony",
			"32 Eliana Benjamin",
			"33 Paisley Mason",
			"34 Leilani Luke",
			"35 Kinsley Julian",
			"36 Chloe Leo",
			"37 Hannah Jack",
			"38 Abigail Maverick",
			"39 Athena Elias",
			"40 Emily Samuel",
			"41 Everly Matthew",
			"42 Ivy John",
			"43 Alice Gabriel",
			"44 Delilah Jayden",
			"45 Eleanor Jace",
			"46 Hailey Hudson",
			"47 Autumn Kayden",
			"48 Mila David",
			"49 Aubrey Theo",
			"50 Stella Xavier",
			"51 Riley Thomas",
			"52 Raelynn Henry",
			"53 Claire Christopher",
			"54 Lucy Lincoln",
			"55 Grace Isaiah",
			"56 Aria Ryan",
			"57 Naomi Joseph",
			"58 Skylar Miles",
			"59 Arya Kingston",
			"60 Sophie Jameson",
			"61 Natalia Cooper",
			"62 Everleigh Greyson",
			"63 Nora Eli",
			"64 Natalie Adrian",
			"65 Zoe Jacob",
			"66 Ayla Isaac",
			"67 Jasmine Charlie",
			"68 Aaliyah Caleb",
			"69 Addison Bentley",
			"70 Genesis Bennett",
			"71 Bella Leon",
			"72 Serenity Joshua",
			"73 Oaklynn Jeremiah",
			"74 Rylee Andrew",
			"75 Maya Santiago",
			"76 Kehlani Axel",
			"77 Nevaeh Adam",
			"78 Madelyn Nolan",
			"79 Rose Colton",
			"80 Kaylee Luka",
			"81 Lydia Ian",
			"82 Anastasia Zion",
			"83 Leah Aaron",
			"84 Lillian Myles",
			"85 Eva Vincent",
			"86 Samantha Easton",
			"87 Melody Ayden",
			"88 Catalina Landon",
			"89 Victoria Amir",
			"90 Iris Theodore",
			"91 Maria Ace",
			"92 Valentina Silas",
			"93 Jade Leonardo",
			"94 Josie Lorenzo",
			"95 Alani Gold",
			"96 Savannah King",
			"97 Laila Atlas",
			"98 Adeline Bryson",
			"99 Nyla Beau",
			"100 Cora Legend"
		},
		{ 1, 10 },
		true
	};

	list.Run();

	return 0;
}