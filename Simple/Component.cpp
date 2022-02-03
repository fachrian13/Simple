#include <conio.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "Simple.h"

using namespace Simple;
using namespace Utility;

template<class T>
class MenuBase
{
protected:
	std::vector<T> Menu;
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

protected:
	virtual inline std::string Render(int limit) = 0;

public:
	class : public System::ReadOnlyProperty<bool> { friend class DropdownMenu; friend class InputMenu; } Running;

	class : public System::ReadOnlyProperty<System::Coordinate>
	{
		friend class DropdownMenu; friend class InputMenu;
		
		std::string ToString() const { return this->Value; }
	} CursorPosition;

	struct
	{
		class : public System::ReadOnlyProperty<int> { friend class DropdownMenu; friend class InputMenu; } Index;
		class : public System::ReadOnlyProperty<const char*> { friend class DropdownMenu; friend class InputMenu; } Value;
	} Selected;
};

class DropdownMenu final : public MenuBase<std::string>
{
private:
	inline std::string Render(int limit) override
	{
		std::stringstream ss;
		System::ConsoleColor focus{ System::Color::White, System::Color::Black };
		System::ConsoleColor normal{ System::Color::Default, System::Color::Default };

		this->Index.Limit = limit + this->Index.Begin;
		this->Limit = limit;

		if (this->Fill)
			for (int i = this->Index.Begin, y = this->Position.Y; i < this->Index.Limit; i++, y++)
				this->Index.Current == i ? ss << System::Coordinate{ this->Position.X, y } << focus << this->Menu[i] << std::string(this->MaxLength - this->Menu[i].length(), ' ') << normal : ss << System::Coordinate{ this->Position.X, y } << this->Menu[i] << std::string(this->MaxLength - this->Menu[i].length(), ' ');
		else
			for (int i = this->Index.Begin, y = this->Position.Y; i < this->Index.Limit; i++, y++)
				this->Index.Current == i ? ss << System::Coordinate{ this->Position.X, y } << focus << this->Menu[i] << normal << std::string(this->MaxLength - this->Menu[i].length(), ' ') : ss << System::Coordinate{ this->Position.X, y } << this->Menu[i] << std::string(this->MaxLength - this->Menu[i].length(), ' ');
		return ss.str();
	}

public:
	DropdownMenu(std::initializer_list<std::string> menu, System::Coordinate position, bool fill = false)
	{
		this->Menu = menu;
		this->Position = position;
		this->MaxLength = 0;
		this->Fill = fill;
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

struct InputMenuFormat
{
	std::string Name;
	std::string* Source;

	InputMenuFormat(std::string name, std::string* source = nullptr) : Name(name), Source(source) {}
};

class InputMenu final : public MenuBase<InputMenuFormat>
{
private:
	inline std::string Render(int limit) override
	{
		std::stringstream ss;
		size_t maxWidth = System::Console::GetBufferSize().X - this->MaxLength;
		System::ConsoleColor focus = System::ConsoleColor{ System::Color::White, System::Color::Black };
		System::ConsoleColor normal = System::ConsoleColor{ System::Color::Default, System::Color::Default };

		this->Index.Limit = limit + this->Index.Begin;
		this->Limit = limit;

		if (this->Fill)
			for (int i = this->Index.Begin, y = this->Position.Y; i < this->Index.Limit; i++, y++)
				this->Index.Current == i ? ss << System::Coordinate{ this->Position.X, y } << focus << this->Menu[i].Name << std::string(this->MaxLength - this->Menu[i].Name.length(), ' ') << normal << (this->Menu[i].Source == nullptr ? std::string(maxWidth, ' ') : this->Menu[i].Source->empty() ? std::string(maxWidth, ' ') : " " + *this->Menu[i].Source + std::string(maxWidth - this->Menu[i].Source->length(), ' ')) : ss << System::Coordinate{ this->Position.X, y } << this->Menu[i].Name << (this->Menu[i].Source == nullptr ? std::string(maxWidth, ' ') : this->Menu[i].Source->empty() ? std::string(maxWidth, ' ') : " " + *this->Menu[i].Source + std::string(maxWidth - this->Menu[i].Source->length(), ' ')) << std::string(this->MaxLength - this->Menu[i].Name.length(), ' ');
		else
			for (int i = this->Index.Begin, y = this->Position.Y; i < this->Index.Limit; i++, y++)
				this->Index.Current == i ? ss << System::Coordinate{ this->Position.X, y } << focus << this->Menu[i].Name << normal << (this->Menu[i].Source == nullptr ? std::string(maxWidth, ' ') : this->Menu[i].Source->empty() ? std::string(maxWidth, ' ') : " " + *this->Menu[i].Source + std::string(maxWidth - this->Menu[i].Source->length(), ' ')) << std::string(this->MaxLength - this->Menu[i].Name.length(), ' ') : ss << System::Coordinate{ this->Position.X, y } << this->Menu[i].Name << (this->Menu[i].Source == nullptr ? std::string(maxWidth, ' ') : this->Menu[i].Source->empty() ? std::string(maxWidth, ' ') : " " + *this->Menu[i].Source + std::string(maxWidth - this->Menu[i].Source->length(), ' ')) << std::string(this->MaxLength - this->Menu[i].Name.length(), ' ');
		return ss.str();
	}

public:
	InputMenu(std::initializer_list<InputMenuFormat> menu, System::Coordinate position, bool fill = false)
	{
		this->Menu = menu;
		this->Position = position;
		this->MaxLength = 0;
		this->Fill = fill;
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
		System::Console::Write(this->CursorPosition.Value);
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

void Register();

int main()
{
	Simple::System::Console::EnableVirtualTerminal();
	Simple::System::Console::SetWindowSize(120, 30);

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
		{ 3, 6 }
	};

	do
	{
		System::Console::Write(System::Coordinate{ 3, 2 }, "===============");
		System::Console::Write(System::Coordinate{ 3, 3 }, "   LOCALHOST");
		System::Console::Write(System::Coordinate{ 3, 4 }, "===============");

		dashboard.Run();

		if (strcmp(dashboard.Selected.Value, "[Kembali]") == 0)
			dashboard.Stop();
	} while (dashboard.Running);
	return 0;
}

void Register()
{

}