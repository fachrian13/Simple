#include "Simple.h"

using namespace Simple::System;
using namespace Simple::Utility;

ConsoleColor DefaultColor = { Color::Black, Color::White };

class Pixel
{
private:
	char Character = ' ';
	ConsoleColor Color = DefaultColor;

public:
	Pixel(ConsoleColor color, char character = ' ') : Character(character), Color(color) {}

	std::string ToString() const
	{
		std::string tString;

		tString += this->Color;
		tString += this->Character;
		return tString;
	}
};

class Line
{
private:
	int Width;
	std::vector<Pixel> Pixels;
	ConsoleColor Color;

public:
	class : public ReadOnlyProperty<int> { friend class Line; } Size;

public:
	Line(int width, ConsoleColor color = DefaultColor) : Width(width), Color(color), Pixels(std::vector<Pixel>(width, Pixel(color)))
	{
		this->Size.Value = static_cast<int>(Pixels.size());
	}

	Pixel& PixelAt(int position)
	{
		return this->Pixels[position];
	}

	void Set(Line value)
	{
		for (int i = 0; i < value.Size; i++)
			this->Pixels[i] = value.Pixels[i];
	}

	std::string ToString() const
	{
		std::string tString;

		for (const auto& pixel : this->Pixels)
			tString += pixel.ToString();
		return tString;
	}
};

class Canvas
{
private:
	int Width;
	int Height;
	std::vector<Line> Lines;
	ConsoleColor Color;

public:
	class : public ReadOnlyProperty<int> { friend class Canvas; } Size;

public:
	Canvas(int width, int height, ConsoleColor color = DefaultColor) : Width(width), Height(height), Color(color), Lines(std::vector<Line>(height, Line(width, color)))
	{
		this->Size.Value = static_cast<int>(this->Lines.size());
	}

	Line& LineAt(int position)
	{
		return this->Lines[position];
	}

	std::string ToString() const
	{
		std::string tString;

		for (const auto& line : this->Lines)
			tString += line.ToString() + "\n";
		tString.pop_back();
		return tString;
	}
};

class Component
{
public:
	virtual Canvas Render() const PURE;
	virtual ~Component() {}
};

class Text : public Component
{
private:
	std::string Content;
	ConsoleColor Color;

public:
	Text(std::string value, ConsoleColor color = DefaultColor) : Content(value), Color(color) {}

	Canvas Render() const override
	{
		std::vector<std::string> contentSplited = Tools::Split(this->Content, '\n');
		Canvas tCanvas(120, contentSplited.size());

		for (size_t i = 0; i < contentSplited.size(); i++)
		{
			Line tLine(120, this->Color);

			for (size_t j = 0; j < contentSplited[i].size(); j++)
				tLine.PixelAt(j) = Pixel(this->Color, contentSplited[i][j]);

			tCanvas.LineAt(i).Set(tLine);
		}
		return tCanvas;
	}
};

class Layout : public Canvas
{
private:
	std::vector<std::shared_ptr<Component>> Components;

private:
	void Render()
	{
		int currentLine = 0;

		for (size_t i = 0; i < this->Components.size(); i++)
		{
			Canvas tCanvas = this->Components[i]->Render();

			for (int j = 0; j < tCanvas.Size; j++, currentLine++)
				this->LineAt(currentLine) = tCanvas.LineAt(j);
		}
	}

public:
	Layout(int width, int height, ConsoleColor color) : Canvas(width, height, color) {}

	template<class T>
	void Add(T value)
	{
		this->Components.push_back(std::make_shared<T>(value));
	}

	void Draw()
	{
		this->Render();
		Console::Write(this->ToString());
	}
};

int main()
{
	Console::EnableVirtualTerminal();
	Console::DisableResizeWindow();
	Console::SetWindowSize(120, 30);

	Layout layout(120, 30, { Color::Green, Color::Black });

	layout.Add(Text("Hello World", { Color::Magenta, Color::Black }));
	layout.Add(Text("Ini\nHello\nWorld\nDengan\nEscape\nCharacter", { Color::Red, Color::Blue }));

	layout.Draw();
	Console::ReadKey();
}