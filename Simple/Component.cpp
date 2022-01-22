#include <conio.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "Simple.h"

using namespace Simple;

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
	/// <summary>
	/// Perulangan menu.
	/// </summary>
	class : public System::ReadOnlyProperty<bool> { friend class ConsoleMenu; } Running;

	/// <summary>
	/// Hasil pilihan menu.
	/// </summary>
	struct
	{
		class : public System::ReadOnlyProperty<int> { friend class ConsoleMenu; } Index;
		class : public System::ReadOnlyProperty<const char*> { friend class ConsoleMenu; } Value;
	} Selected;

public:
	ConsoleMenu(std::initializer_list<std::string> menu, Coordinate position, bool fill = false) : Menu(menu), Position(position), MaxLength(0), Fill(fill)
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

		this->Selected.Index.Value = this->Index.Current;
		this->Selected.Value.Value = this->Menu[this->Index.Current].c_str();
	}

	void Run()
	{
		int y = System::Console::GetBufferSize().Y;
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
	ConsoleMenu a
	(
		{
			"Helasdfasdfasdfasddtyjdtymdtymsasdforld 1",
			"Hello Wrymdtmstymasrymstryorld 2",
			"Hello Wormsrymsrymsrtymsrtymstcfyhmstymkdtadfasdfsdfld 3",
			"Hello Wasyumdtmstfymsrymsrynrdcnsryddorld 4",
			"Hello Wonjsrymksrymsrynmsrtjnaet6jhrsykutykmasr4kjaretgaedtjharedtgjasdfrld 5",
			"Hello Worlrstyjnsrdtgjnaedtjhaertjhartgejhnsddddddd 6",
			"Hello Woasrstgjnsrtfygjnsrtyjnaedthbxdegtjnhsrdftgjnsrtgyjnsrdfasdfrld 7",
			"Hello Worasdfdtgjhnaderthjnrsdtgasdddadfld 8",
			"Helasdfasdfasdfasddtyjdtymdtymsasdforld 1",
			"Hello Wrymdtmstymasrymstryorld 2",
			"Hello Wormsrymsrymsrtymsrtymstcfyhmstymkdtadfasdfsdfld 3",
			"Hello Wasyumdtmstfymsrymsrynrdcnsryddorld 4",
			"Hello Wonjsrymksrymsrynmsrtjnaet6jhrsykutykmasr4kjaretgaedtjharedtgjasdfrld 5",
			"Hello Worlrstyjnsrdtgjnaedtjhaertjhartgejhnsddddddd 6",
			"Hello Woasrstgjnsrtfygjnsrtyjnaedthbxdegtjnhsrdftgjnsrtgyjnsrdfasdfrld 7",
			"Hello Worasdfdtgjhnaderthjnrsdtgasdddadfld 8",
			"Helasdfasdfasdfasddtyjdtymdtymsasdforld 1",
			"Hello Wrymdtmstymasrymstryorld 2",
			"Hello Wormsrymsrymsrtymsrtymstcfyhmstymkdtadfasdfsdfld 3",
			"Hello Wasyumdtmstfymsrymsrynrdcnsryddorld 4",
			"Hello Wonjsrymksrymsrynmsrtjnaet6jhrsykutykmasr4kjaretgaedtjharedtgjasdfrld 5",
			"Hello Worlrstyjnsrdtgjnaedtjhaertjhartgejhnsddddddd 6",
			"Hello Woasrstgjnsrtfygjnsrtyjnaedthbxdegtjnhsrdftgjnsrtgyjnsrdfasdfrld 7",
			"Hello Worasdfdtgjhnaderthjnrsdtgasdddadfld 8",
			"Helasdfasdfasdfasddtyjdtymdtymsasdforld 1",
			"Hello Wrymdtmstymasrymstryorld 2",
			"Hello Wormsrymsrymsrtymsrtymstcfyhmstymkdtadfasdfsdfld 3",
			"Hello Wasyumdtmstfymsrymsrynrdcnsryddorld 4",
			"Hello Wonjsrymksrymsrynmsrtjnaet6jhrsykutykmasr4kjaretgaedtjharedtgjasdfrld 5",
			"Hello Worlrstyjnsrdtgjnaedtjhaertjhartgejhnsddddddd 6",
			"Hello Woasrstgjnsrtfygjnsrtyjnaedthbxdegtjnhsrdftgjnsrtgyjnsrdfasdfrld 7",
			"Hello Worasdfdtgjhnaderthjnrsdtgasdddadfld 8",
			"Helasdfasdfasdfasddtyjdtymdtymsasdforld 1",
			"Hello Wrymdtmstymasrymstryorld 2",
			"Hello Wormsrymsrymsrtymsrtymstcfyhmstymkdtadfasdfsdfld 3",
			"Hello Wasyumdtmstfymsrymsrynrdcnsryddorld 4",
			"Hello Wonjsrymksrymsrynmsrtjnaet6jhrsykutykmasr4kjaretgaedtjharedtgjasdfrld 5",
			"Hello Worlrstyjnsrdtgjnaedtjhaertjhartgejhnsddddddd 6",
			"Hello Woasrstgjnsrtfygjnsrtyjnaedthbxdegtjnhsrdftgjnsrtgyjnsrdfasdfrld 7",
			"Hello Worasdfdtgjhnaderthjnrsdtgasdddadfld 8",
			"Helasdfasdfasdfasddtyjdtymdtymsasdforld 1",
			"Hello Wrymdtmstymasrymstryorld 2",
			"Hello Wormsrymsrymsrtymsrtymstcfyhmstymkdtadfasdfsdfld 3",
			"Hello Wasyumdtmstfymsrymsrynrdcnsryddorld 4",
			"Hello Wonjsrymksrymsrynmsrtjnaet6jhrsykutykmasr4kjaretgaedtjharedtgjasdfrld 5",
			"Hello Worlrstyjnsrdtgjnaedtjhaertjhartgejhnsddddddd 6",
			"Hello Woasrstgjnsrtfygjnsrtyjnaedthbxdegtjnhsrdftgjnsrtgyjnsrdfasdfrld 7",
			"Hello Worasdfdtgjhnaderthjnrsdtgasdddadfld 8",
			"Helasdfasdfasdfasddtyjdtymdtymsasdforld 1",
			"Hello Wrymdtmstymasrymstryorld 2",
			"Hello Wormsrymsrymsrtymsrtymstcfyhmstymkdtadfasdfsdfld 3",
			"Hello Wasyumdtmstfymsrymsrynrdcnsryddorld 4",
			"Hello Wonjsrymksrymsrynmsrtjnaet6jhrsykutykmasr4kjaretgaedtjharedtgjasdfrld 5",
			"Hello Worlrstyjnsrdtgjnaedtjhaertjhartgejhnsddddddd 6",
			"Hello Woasrstgjnsrtfygjnsrtyjnaedthbxdegtjnhsrdftgjnsrtgyjnsrdfasdfrld 7",
			"Hello Worasdfdtgjhnaderthjnrsdtgasdddadfld 8",
			"Helasdfasdfasdfasddtyjdtymdtymsasdforld 1",
			"Hello Wrymdtmstymasrymstryorld 2",
			"Hello Wormsrymsrymsrtymsrtymstcfyhmstymkdtadfasdfsdfld 3",
			"Hello Wasyumdtmstfymsrymsrynrdcnsryddorld 4",
			"Hello Wonjsrymksrymsrynmsrtjnaet6jhrsykutykmasr4kjaretgaedtjharedtgjasdfrld 5",
			"Hello Worlrstyjnsrdtgjnaedtjhaertjhartgejhnsddddddd 6",
			"Hello Woasrstgjnsrtfygjnsrtyjnaedthbxdegtjnhsrdftgjnsrtgyjnsrdfasdfrld 7",
			"Hello Worasdfdtgjhnaderthjnrsdtgasdddadfld 8",
			"Helasdfasdfasdfasddtyjdtymdtymsasdforld 1",
			"Hello Wrymdtmstymasrymstryorld 2",
			"Hello Wormsrymsrymsrtymsrtymstcfyhmstymkdtadfasdfsdfld 3",
			"Hello Wasyumdtmstfymsrymsrynrdcnsryddorld 4",
			"Hello Wonjsrymksrymsrynmsrtjnaet6jhrsykutykmasr4kjaretgaedtjharedtgjasdfrld 5",
			"Hello Worlrstyjnsrdtgjnaedtjhaertjhartgejhnsddddddd 6",
			"Hello Woasrstgjnsrtfygjnsrtyjnaedthbxdegtjnhsrdftgjnsrtgyjnsrdfasdfrld 7",
			"Hello Worasdfdtgjhnaderthjnrsdtgasdddadfld 8",
			"Helasdfasdfasdfasddtyjdtymdtymsasdforld 1",
			"Hello Wrymdtmstymasrymstryorld 2",
			"Hello Wormsrymsrymsrtymsrtymstcfyhmstymkdtadfasdfsdfld 3",
			"Hello Wasyumdtmstfymsrymsrynrdcnsryddorld 4",
			"Hello Wonjsrymksrymsrynmsrtjnaet6jhrsykutykmasr4kjaretgaedtjharedtgjasdfrld 5",
			"Hello Worlrstyjnsrdtgjnaedtjhaertjhartgejhnsddddddd 6",
			"Hello Woasrstgjnsrtfygjnsrtyjnaedthbxdegtjnhsrdftgjnsrtgyjnsrdfasdfrld 7",
			"Hello Worasdfdtgjhnaderthjnrsdtgasdddadfld 8",
			"Helasdfasdfasdfasddtyjdtymdtymsasdforld 1",
			"Hello Wrymdtmstymasrymstryorld 2",
			"Hello Wormsrymsrymsrtymsrtymstcfyhmstymkdtadfasdfsdfld 3",
			"Hello Wasyumdtmstfymsrymsrynrdcnsryddorld 4",
			"Hello Wonjsrymksrymsrynmsrtjnaet6jhrsykutykmasr4kjaretgaedtjharedtgjasdfrld 5",
			"Hello Worlrstyjnsrdtgjnaedtjhaertjhartgejhnsddddddd 6",
			"Hello Woasrstgjnsrtfygjnsrtyjnaedthbxdegtjnhsrdftgjnsrtgyjnsrdfasdfrld 7",
			"Hello Worasdfdtgjhnaderthjnrsdtgasdddadfld 8",
			"Helasdfasdfasdfasddtyjdtymdtymsasdforld 1",
			"Hello Wrymdtmstymasrymstryorld 2",
			"Hello Wormsrymsrymsrtymsrtymstcfyhmstymkdtadfasdfsdfld 3",
			"Hello Wasyumdtmstfymsrymsrynrdcnsryddorld 4",
			"Hello Wonjsrymksrymsrynmsrtjnaet6jhrsykutykmasr4kjaretgaedtjharedtgjasdfrld 5",
			"Hello Worlrstyjnsrdtgjnaedtjhaertjhartgejhnsddddddd 6",
			"Hello Woasrstgjnsrtfygjnsrtyjnaedthbxdegtjnhsrdftgjnsrtgyjnsrdfasdfrld 7",
			"Hello Worasdfdtgjhnaderthjnrsdtgasdddadfld 8",
			"Helasdfasdfasdfasddtyjdtymdtymsasdforld 1",
			"Hello Wrymdtmstymasrymstryorld 2",
			"Hello Wormsrymsrymsrtymsrtymstcfyhmstymkdtadfasdfsdfld 3",
			"Hello Wasyumdtmstfymsrymsrynrdcnsryddorld 4",
			"Hello Wonjsrymksrymsrynmsrtjnaet6jhrsykutykmasr4kjaretgaedtjharedtgjasdfrld 5",
			"Hello Worlrstyjnsrdtgjnaedtjhaertjhartgejhnsddddddd 6",
			"Hello Woasrstgjnsrtfygjnsrtyjnaedthbxdegtjnhsrdftgjnsrtgyjnsrdfasdfrld 7",
			"Hello Worasdfdtgjhnaderthjnrsdtgasdddadfld 8",
			"Helasdfasdfasdfasddtyjdtymdtymsasdforld 1",
			"Hello Wrymdtmstymasrymstryorld 2",
			"Hello Wormsrymsrymsrtymsrtymstcfyhmstymkdtadfasdfsdfld 3",
			"Hello Wasyumdtmstfymsrymsrynrdcnsryddorld 4",
			"Hello Wonjsrymksrymsrynmsrtjnaet6jhrsykutykmasr4kjaretgaedtjharedtgjasdfrld 5",
			"Hello Worlrstyjnsrdtgjnaedtjhaertjhartgejhnsddddddd 6",
			"Hello Woasrstgjnsrtfygjnsrtyjnaedthbxdegtjnhsrdftgjnsrtgyjnsrdfasdfrld 7",
			"Hello Worasdfdtgjhnaderthjnrsdtgasdddadfld 8",
			"Helasdfasdfasdfasddtyjdtymdtymsasdforld 1",
			"Hello Wrymdtmstymasrymstryorld 2",
			"Hello Wormsrymsrymsrtymsrtymstcfyhmstymkdtadfasdfsdfld 3",
			"Hello Wasyumdtmstfymsrymsrynrdcnsryddorld 4",
			"Hello Wonjsrymksrymsrynmsrtjnaet6jhrsykutykmasr4kjaretgaedtjharedtgjasdfrld 5",
			"Hello Worlrstyjnsrdtgjnaedtjhaertjhartgejhnsddddddd 6",
			"Hello Woasrstgjnsrtfygjnsrtyjnaedthbxdegtjnhsrdftgjnsrtgyjnsrdfasdfrld 7",
			"Hello Worasdfdtgjhnaderthjnrsdtgasdddadfld 8",
			"Helasdfasdfasdfasddtyjdtymdtymsasdforld 1",
			"Hello Wrymdtmstymasrymstryorld 2",
			"Hello Wormsrymsrymsrtymsrtymstcfyhmstymkdtadfasdfsdfld 3",
			"Hello Wasyumdtmstfymsrymsrynrdcnsryddorld 4",
			"Hello Wonjsrymksrymsrynmsrtjnaet6jhrsykutykmasr4kjaretgaedtjharedtgjasdfrld 5",
			"Hello Worlrstyjnsrdtgjnaedtjhaertjhartgejhnsddddddd 6",
			"Hello Woasrstgjnsrtfygjnsrtyjnaedthbxdegtjnhsrdftgjnsrtgyjnsrdfasdfrld 7",
			"Hello Worasdfdtgjhnaderthjnrsdtgasdddadfld 8"
		},
		{ 3, 3 },
		true
	);

	a.Run(7);
}