#include <conio.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "Simple.h"

using namespace Simple;

class DropdownMenu
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
		int Limit;
	} Index;

private:
	std::string Render(int limit)
	{
		std::stringstream ss;
		this->Index.Limit = limit + this->Index.Begin;
		this->Limit = limit;

		if (this->Fill)
			for (int i = this->Index.Begin, y = this->Position.Y; i < this->Index.Limit; i++, y++)
				this->Index.Current == i ? ss << System::Coordinate{ this->Position.X, y } << System::ConsoleColor{ System::Color::White, System::Color::Black } << this->Menu[i] << std::string(this->MaxLength - this->Menu[i].length(), ' ') << System::ConsoleColor{ System::Color::Default, System::Color::Default } : ss << System::Coordinate{ this->Position.X, y } << this->Menu[i] << std::string(this->MaxLength - this->Menu[i].length(), ' ');
		else
			for (int i = this->Index.Begin, y = this->Position.Y; i < this->Index.Limit; i++, y++)
				this->Index.Current == i ? ss << System::Coordinate{ this->Position.X, y } << System::ConsoleColor{ System::Color::White, System::Color::Black } << this->Menu[i] << System::ConsoleColor{ System::Color::Default, System::Color::Default } << std::string(this->MaxLength - this->Menu[i].length(), ' ') : ss << System::Coordinate{ this->Position.X, y } << this->Menu[i] << std::string(this->MaxLength - this->Menu[i].length(), ' ');
		return ss.str();
	}

public:
	class : public System::ReadOnlyProperty<bool> { friend class DropdownMenu; } Running;

	class : public System::ReadOnlyProperty<System::Coordinate>
	{
		friend class DropdownMenu;
		OUTSTREAM(System::Coordinate) { return out << value; }
	} CursorPosition;

	struct
	{
		class : public System::ReadOnlyProperty<int> { friend class DropdownMenu; } Index;
		class : public System::ReadOnlyProperty<const char*> { friend class DropdownMenu; } Value;
	} Selected;

public:
	DropdownMenu(std::initializer_list<std::string> menu, System::Coordinate position, bool fill = false) : Menu(menu), Position(position), MaxLength(0), Fill(fill)
	{
		for (const auto& i : menu)
			this->MaxLength = i.length() > this->MaxLength ? i.length() : this->MaxLength;
		this->Index = { 0, 0, static_cast<int>(menu.size() - 1), 0 };
		this->Selected.Index.Value = 0;
		this->Selected.Value.Value = "";
		this->Running.Value = true;
		this->CursorPosition.Value = { 0, 0 };
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
					if (this->Index.Current == this->Index.Limit)
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
		this->CursorPosition.Value = { this->Fill ? this->Position.X + static_cast<int>(this->MaxLength) : this->Position.X + static_cast<int>(this->Menu[this->Index.Current].length()), this->Position.Y + this->Index.Current - this->Index.Begin };
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

class InputMenu
{
public:
	struct Format
	{
		std::string Name;
		std::string* Source;

		Format(std::string name, std::string* source = nullptr) : Name(name), Source(source) {}
	};

private:
	std::vector<Format> Menu;
	System::Coordinate Position;
	size_t MaxLength;
	bool Fill;
	int Limit;
	struct
	{
		int Begin;
		int Current;
		int End;
		int Limit;
	} Index;

private:
	inline std::string Render(int limit)
	{
		std::stringstream ss;
		size_t maxWidth = System::Console::GetBufferSize().X - this->MaxLength;
		this->Index.Limit = limit + this->Index.Begin;
		this->Limit = limit;

		if (this->Fill)
			for (int i = this->Index.Begin, y = this->Position.Y; i < this->Index.Limit; i++, y++)
				this->Index.Current == i ? ss << System::Coordinate{ this->Position.X, y } << System::ConsoleColor{ System::Color::White, System::Color::Black } << this->Menu[i].Name << std::string(this->MaxLength - this->Menu[i].Name.length(), ' ') << System::ConsoleColor{ System::Color::Default, System::Color::Default } << (this->Menu[i].Source == nullptr ? std::string(maxWidth, ' ') : this->Menu[i].Source->empty() ? std::string(maxWidth, ' ') : " " + *this->Menu[i].Source + std::string(maxWidth - this->Menu[i].Source->length(), ' ')) : ss << System::Coordinate{ this->Position.X, y } << this->Menu[i].Name << (this->Menu[i].Source == nullptr ? std::string(maxWidth, ' ') : this->Menu[i].Source->empty() ? std::string(maxWidth, ' ') : " " + *this->Menu[i].Source + std::string(maxWidth - this->Menu[i].Source->length(), ' ')) << std::string(this->MaxLength - this->Menu[i].Name.length(), ' ');
		else
			for (int i = this->Index.Begin, y = this->Position.Y; i < this->Index.Limit; i++, y++)
				this->Index.Current == i ? ss << System::Coordinate{ this->Position.X, y } << System::ConsoleColor{ System::Color::White, System::Color::Black } << this->Menu[i].Name << System::ConsoleColor{ System::Color::Default, System::Color::Default } << (this->Menu[i].Source == nullptr ? std::string(maxWidth, ' ') : this->Menu[i].Source->empty() ? std::string(maxWidth, ' ') : " " + *this->Menu[i].Source + std::string(maxWidth - this->Menu[i].Source->length(), ' ')) << std::string(this->MaxLength - this->Menu[i].Name.length(), ' ') : ss << System::Coordinate{ this->Position.X, y } << this->Menu[i].Name << (this->Menu[i].Source == nullptr ? std::string(maxWidth, ' ') : this->Menu[i].Source->empty() ? std::string(maxWidth, ' ') : " " + *this->Menu[i].Source + std::string(maxWidth - this->Menu[i].Source->length(), ' ')) << std::string(this->MaxLength - this->Menu[i].Name.length(), ' ');
		return ss.str();
	}

public:
	class : public System::ReadOnlyProperty<bool> { friend class InputMenu; } Running;

	class : public System::ReadOnlyProperty<System::Coordinate>
	{
		friend class InputMenu;
		//OUTSTREAM(System::Coordinate) { return out << value; }
	} CursorPosition;

	struct
	{
		class : public System::ReadOnlyProperty<int> { friend class InputMenu; } Index;
		class : public System::ReadOnlyProperty<const char*> { friend class InputMenu; } Value;
	} Selected;

public:
	InputMenu(std::initializer_list<Format> menu, System::Coordinate position, bool fill = false) : Menu(menu), Position(position), MaxLength(0), Fill(fill)
	{
		for (const auto& i : menu)
			this->MaxLength = i.Name.length() > this->MaxLength ? i.Name.length() : this->MaxLength;
		this->Index = { 0, 0, static_cast<int>(menu.size() - 1), 0 };
		this->Selected.Index.Value = 0;
		this->Selected.Value.Value = "";
		this->Running.Value = true;
		this->CursorPosition.Value = { 0, 0 };
	}

	void Clear()
	{
		int y = this->Position.Y;
		std::stringstream ss;

		size_t maxSource = 0;
		for (int i = this->Index.Begin; i < this->Index.End; i++)
			maxSource = this->Menu[i].Source->length() > maxSource ? this->Menu[i].Source->length() : maxSource;

		std::string fill(this->MaxLength + maxSource + 1, ' ');
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
					if (this->Index.Current == this->Index.Limit)
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

		this->Selected.Index.Value = this->Index.Current;
		this->Selected.Value.Value = this->Menu[this->Index.Current].Name.c_str();
		this->CursorPosition.Value = { this->Fill ? this->Position.X + static_cast<int>(this->MaxLength) : this->Position.X + static_cast<int>(this->Menu[this->Index.Current].Name.length()), this->Position.Y + this->Index.Current - this->Index.Begin };
		System::Console::Write(this->CursorPosition);
		System::Console::CursorVisible = true;
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
	DropdownMenu dashboard
	{
		{
			"1. Registrasi Siswa",
			"2. Tampilkan seluruh data",
			"3. Cari data Siswa",
			"4. Perbarui data",
			"5. Hapus data siswa",
			"[Kembali]"
		},
		{ 3, 6 },
	};

	do
	{
		System::Console::Write(System::Coordinate{ 2, 2 }, "===============");
		System::Console::Write(System::Coordinate{ 2, 3 }, "   LOCALHOST");
		System::Console::Write(System::Coordinate{ 2, 4 }, "===============");

		dashboard.Run();
	} while (dashboard.Running);
	return 0;
}