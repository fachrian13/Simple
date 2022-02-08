#include "Simple.h"

using namespace Simple::System;
using namespace Simple::Utility;

constexpr ConsoleColor DefaultColor = { Color::Black, Color::White };

class Pixel
{
private:
	char Character = ' ';
	ConsoleColor Color = DefaultColor;

public:
	friend bool operator==(const Pixel& first, const Pixel& second) { return first.Character == second.Character && first.Color == second.Color; }

	friend bool operator!=(const Pixel& first, const Pixel& second) { return !(first == second); }

public:
	Pixel(ConsoleColor color, char character = ' ') : Character(character), Color(color) {}

	void SetColor(ConsoleColor color)
	{
		this->Color = color;
	}

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

	friend bool operator==(const Line& first, const Line& second)
	{
		bool loop = true;

		for (int i = 0; loop; i++)
			loop = first.Pixels[i] == second.Pixels[i];
		return loop;
	}

	friend bool operator!=(const Line& first, const Line& second)
	{
		return !(first == second);
	}

	Line& operator=(const Line& value)
	{
		if (this == &value)
			return *this;

		this->Pixels = value.Pixels;
		return *this;
	}

public:
	Line(int width, ConsoleColor color = DefaultColor) : Width(width), Color(color), Pixels(std::vector<Pixel>(width, Pixel(color))) { this->Size.Value = width; }

	Pixel& PixelAt(int position) { return this->Pixels[position]; }

	void SetColor(ConsoleColor color)
	{
		for (auto& pixel : this->Pixels)
			pixel.SetColor(color);
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
	std::vector<Line> Lines;

public:
	int Width;
	int Height;
	ConsoleColor Color;

public:
	class : public ReadOnlyProperty<int> { friend class Canvas; } Size;

public:
	Canvas(int width, int height, ConsoleColor color = DefaultColor) : Width(width), Height(height), Color(color), Lines(std::vector<Line>(height, Line(width, color))) { this->Size.Value = height; }

	Line& LineAt(int position) { return this->Lines[position]; }

	void SetColor(ConsoleColor color)
	{
		for (auto& line : this->Lines)
			line.SetColor(color);
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
	virtual Canvas Render(int width) const PURE;
	virtual ~Component() {}
};

class Text final : public Component
{
private:
	std::string Content;
	ConsoleColor Color;
	std::vector<std::string> ContentSplited;

public:
	Text(std::string value, ConsoleColor color = DefaultColor) : Content(value), Color(color) { this->ContentSplited = Tools::Split(value, '\n'); }

	Canvas Render(int width) const override
	{
		Canvas tCanvas(width, this->ContentSplited.size());
		Line tLine(width, this->Color);

		for (size_t i = 0; i < this->ContentSplited.size(); i++)
		{
			for (size_t j = 0; j < (this->ContentSplited[i].size() <= width ? this->ContentSplited[i].size() : width); j++)
				tLine.PixelAt(j) = Pixel(this->Color, this->ContentSplited[i][j]);

			tCanvas.LineAt(i) = tLine;
		}
		return tCanvas;
	}
};

class Dropdown : public Canvas
{
private:
	ConsoleColor Focus = { Color::White, Color::Black };
	std::vector<std::shared_ptr<Component>> Components;
	std::vector<Canvas> Rendered;
	struct
	{
		int Begin;
		int Current;
		int End;
		int Limit;
	} Index;

private:
	void Render()
	{
		int currentLine = 0;

		this->Index.Limit = this->Index.Begin + this->Height;

		for (int i = this->Index.Begin; i < this->Index.Limit; i++)
		{
			if (i == this->Index.Current)
				this->Rendered[i].SetColor(this->Focus);
			else
				this->Rendered[i].SetColor(DefaultColor);
			
			for (int j = 0; j < this->Rendered[i].Size; j++, currentLine++)
				this->LineAt(currentLine) = this->Rendered[i].LineAt(j);
		}
	}

public:
	Dropdown(int width, int height) : Canvas(width, height) { this->Index = { 0, 0, -1, height }; }

	template<class T>
	void Add(T value)
	{
		std::shared_ptr<Component> tValue = std::make_shared<T>(value);

		this->Components.push_back(tValue);
		this->Rendered.push_back(tValue->Render(this->Width));
		this->Index.End++;
	}

	void Draw()
	{
		this->Render();
		Console::Write(this->ToString(), Coordinate{ 0, 0 });
	}

	void Run()
	{
		char ch;

		do
		{
			this->Draw();

			ch = Console::ReadKey();

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
	}
};

int main()
{
	Console::EnableVirtualTerminal();
	Console::DisableResizeWindow();
	Console::CursorVisible = false;
	Console::SetWindowSize(120, 30);

	Dropdown layout(120, 30);

	layout.Add(Text("Hello\nWorasdfgasedfghsfghsfghsdfgaergasergsdgfhaerybseyvsertybsevvyse5yvse5yves5ybvae5ld1"));
	layout.Add(Text("Hello Worldae;rkfmgls;kryvser5yvedryvzw4ytbvwz45yvse5yvbae5vfgnhmkls;dfgnksdjfgn2"));
	layout.Add(Text("Hello Worlakjsdfbngjksdbfngjkbyase5yse5yvaewy45vbwz$vyaew4ybvaew45vyhsbdfnghd3"));
	layout.Add(Text("Hello Worrytgjdtghjmfdghjdawe54yse5vyev5yazew4yae5yae5yvtyjdryld4"));
	layout.Add(Text("Hello Wojedrtyjdtyjedtyjghj5vyae5vyes5vye5yes5ysfcghkjfghjcghjcfghjrld1"));
	layout.Add(Text("Hello Wobhklfuksyghjfdhjncghm5yes5yes5vyesvghjk,vhj,lgvhkj,rld2"));
	layout.Add(Text("Hello Worasrfgserhsrtghsrtgh5vyes5yse5vsertghsrtghdfghjdgfhld3"));
	layout.Add(Text("Hello Wozerdyxnrftunrtjucmfgyyes5yes5vyase5cyaw4uicmryikcrld4"));
	layout.Add(Text("Hello Wormtguyicng7iycr,6ix,tvaw4ctvaw4r 76int7icld1"));
	layout.Add(Text("Hello Wortm,uof7ymuofny7uidtbntae4tyuidtrm76idmt,7ift,c7iufmnt7ild2"));
	layout.Add(Text("Hello Wornft7inft7nift7e45ybves5vyaes5vycte5vcytae5nild3"));
	layout.Add(Text("Hello Worr7imft7ict7yaev5yave5yae5yaes5cae5vtaectaw4ctvae4vteld4"));
	layout.Add(Text("Hello Worictrm7ict745taw5e4vctae4te5teiftc7ictyld1"));
	layout.Add(Text("Hello Wornct7ift7yinmft7iusdfgsdfgfnt7idbt7inft7ild2"));
	layout.Add(Text("Hello Worixbudxrn5yze4m8uxsfgsfsfgsfgsr56mucrftbzex5ybe5mucr6ticf ,yunbxr5ubld3"));
	layout.Add(Text("Hello Worbxr65ubrcx6ixr6budfgsdfgsdfgsdfgsdfgvr5urx6mjvzeld4"));
	layout.Add(Text("Hello Wor5uze5nudt6midt7ibsdfgsdfgsdxe5uzbe5unld1"));
	layout.Add(Text("Hello Worldr5udr5uyxer5uybxefgsdfgsdfgsdfgsdd5nuxr6isrvbusr5tnbusrx6d2"));
	layout.Add(Text("Hello Worrex65icntiycb6uxevfgsdfgsdfgsdfgsdfgsdfgsdfgsdfgsdfgsdfgsdfgsdfgsdfgsdf5buxer5ld3"));
	layout.Add(Text("Hello Wornixr6ubvex5uybxdrtucsdfgsdfgdsfgsdfgsdfgsdfgsdfgsdfg6rtimcr6tubld4"));
	layout.Add(Text("Hello Woderyhdrnuhcmftuxe5bnuze45byaew4ymcr6i,xryxbv5yxdb4ryrdtrld1"));
	layout.Add(Text("Hello Worxrtnmuxcftbyuxe5byxdgsdfgsdfgsdfvyxed5ybdr6undr6ld2"));
	layout.Add(Text("Hello Wor6ubndr6unbdr6unxr5ubdfgsdfgsdfgsdfgdasfgsdfgsdfgdsfgsdfgsdfgsdfgr6uld3"));
	layout.Add(Text("Hello Worburc56undr6ic6rtunr6cnudr56unxr56byuvdr5uybdr6bnucr6ubld4"));
	layout.Add(Text("Hello Worasdfgasedfghsfghsfghsdfgaergasergsdgfhaerybseyvsertybsevvyse5yvse5yves5ybvae5ld1"));
	layout.Add(Text("Hello Worldae;rkfmgls;kryvser5yvedryvzw4ytbvwz45yvse5yvbae5vfgnhmkls;dfgnksdjfgn2"));
	layout.Add(Text("Hello Worlakjsdfbngjksdbfngjkbyase5yse5yvaewy45vbwz$vyaew4ybvaew45vyhsbdfnghd3"));
	layout.Add(Text("Hello Worrytgjdtghjmfdghjdawe54yse5vyev5yazew4yae5yae5yvtyjdryld4"));
	layout.Add(Text("Hello Wojedrtyjdtyjedtyjghj5vyae5vyes5vye5yes5ysfcghkjfghjcghjcfghjrld1"));
	layout.Add(Text("Hello Wobhklfuksyghjfdhjncghm5yes5yes5vyesvghjk,vhj,lgvhkj,rld2"));
	layout.Add(Text("Hello Worasrfgserhsrtghsrtgh5vyes5yse5vsertghsrtghdfghjdgfhld3"));
	layout.Add(Text("Hello Wozerdyxnrftunrtjucmfgyyes5yes5vyase5cyaw4uicmryikcrld4"));
	layout.Add(Text("Hello Wormtguyicng7iycr,6ix,tvaw4ctvaw4r 76int7icld1"));
	layout.Add(Text("Hello Wortm,uof7ymuofny7uidtbntae4tyuidtrm76idmt,7ift,c7iufmnt7ild2"));
	layout.Add(Text("Hello Wornft7inft7nift7e45ybves5vyaes5vycte5vcytae5nild3"));
	layout.Add(Text("Hello Worr7imft7ict7yaev5yave5yae5yaes5cae5vtaectaw4ctvae4vteld4"));
	layout.Add(Text("Hello Worictrm7ict745taw5e4vctae4te5teiftc7ictyld1"));
	layout.Add(Text("Hello Wornct7ift7yinmft7iusdfgsdfgfnt7idbt7inft7ild2"));
	layout.Add(Text("Hello Worixbudxrn5yze4m8uxsfgsfsfgsfgsr56mucrftbzex5ybe5mucr6ticf ,yunbxr5ubld3"));
	layout.Add(Text("Hello Worbxr65ubrcx6ixr6budfgsdfgsdfgsdfgsdfgvr5urx6mjvzeld4"));
	layout.Add(Text("Hello Wor5uze5nudt6midt7ibsdfgsdfgsdxe5uzbe5unld1"));
	layout.Add(Text("Hello Worldr5udr5uyxer5uybxefgsdfgsdfgsdfgsdd5nuxr6isrvbusr5tnbusrx6d2"));
	layout.Add(Text("Hello Worrex65icntiycb6uxevfgsdfgsdfgsdfgsdfgsdfgsdfgsdfgsdfgsdfgsdfgsdfgsdfgsdf5buxer5ld3"));
	layout.Add(Text("Hello Wornixr6ubvex5uybxdrtucsdfgsdfgdsfgsdfgsdfgsdfgsdfgsdfg6rtimcr6tubld4"));
	layout.Add(Text("Hello Woderyhdrnuhcmftuxe5bnuze45byaew4ymcr6i,xryxbv5yxdb4ryrdtrld1"));
	layout.Add(Text("Hello Worxrtnmuxcftbyuxe5byxdgsdfgsdfgsdfvyxed5ybdr6undr6ld2"));
	layout.Add(Text("Hello Wor6ubndr6unbdr6unxr5ubdfgsdfgsdfgsdfgdasfgsdfgsdfgdsfgsdfgsdfgsdfgr6uld3"));
	layout.Add(Text("Hello Worburc56undr6ic6rtunr6cnudr56unxr56byuvdr5uybdr6bnucr6ubld4"));
	layout.Add(Text("Hello Worasdfgasedfghsfghsfghsdfgaergasergsdgfhaerybseyvsertybsevvyse5yvse5yves5ybvae5ld1"));
	layout.Add(Text("Hello Worldae;rkfmgls;kryvser5yvedryvzw4ytbvwz45yvse5yvbae5vfgnhmkls;dfgnksdjfgn2"));
	layout.Add(Text("Hello Worlakjsdfbngjksdbfngjkbyase5yse5yvaewy45vbwz$vyaew4ybvaew45vyhsbdfnghd3"));
	layout.Add(Text("Hello Worrytgjdtghjmfdghjdawe54yse5vyev5yazew4yae5yae5yvtyjdryld4"));
	layout.Add(Text("Hello Wojedrtyjdtyjedtyjghj5vyae5vyes5vye5yes5ysfcghkjfghjcghjcfghjrld1"));
	layout.Add(Text("Hello Wobhklfuksyghjfdhjncghm5yes5yes5vyesvghjk,vhj,lgvhkj,rld2"));
	layout.Add(Text("Hello Worasrfgserhsrtghsrtgh5vyes5yse5vsertghsrtghdfghjdgfhld3"));
	layout.Add(Text("Hello Wozerdyxnrftunrtjucmfgyyes5yes5vyase5cyaw4uicmryikcrld4"));
	layout.Add(Text("Hello Wormtguyicng7iycr,6ix,tvaw4ctvaw4r 76int7icld1"));
	layout.Add(Text("Hello Wortm,uof7ymuofny7uidtbntae4tyuidtrm76idmt,7ift,c7iufmnt7ild2"));
	layout.Add(Text("Hello Wornft7inft7nift7e45ybves5vyaes5vycte5vcytae5nild3"));
	layout.Add(Text("Hello Worr7imft7ict7yaev5yave5yae5yaes5cae5vtaectaw4ctvae4vteld4"));
	layout.Add(Text("Hello Worictrm7ict745taw5e4vctae4te5teiftc7ictyld1"));
	layout.Add(Text("Hello Wornct7ift7yinmft7iusdfgsdfgfnt7idbt7inft7ild2"));
	layout.Add(Text("Hello Worixbudxrn5yze4m8uxsfgsfsfgsfgsr56mucrftbzex5ybe5mucr6ticf ,yunbxr5ubld3"));
	layout.Add(Text("Hello Worbxr65ubrcx6ixr6budfgsdfgsdfgsdfgsdfgvr5urx6mjvzeld4"));
	layout.Add(Text("Hello Wor5uze5nudt6midt7ibsdfgsdfgsdxe5uzbe5unld1"));
	layout.Add(Text("Hello Worldr5udr5uyxer5uybxefgsdfgsdfgsdfgsdd5nuxr6isrvbusr5tnbusrx6d2"));
	layout.Add(Text("Hello Worrex65icntiycb6uxevfgsdfgsdfgsdfgsdfgsdfgsdfgsdfgsdfgsdfgsdfgsdfgsdfgsdf5buxer5ld3"));
	layout.Add(Text("Hello Wornixr6ubvex5uybxdrtucsdfgsdfgdsfgsdfgsdfgsdfgsdfgsdfg6rtimcr6tubld4"));
	layout.Add(Text("Hello Woderyhdrnuhcmftuxe5bnuze45byaew4ymcr6i,xryxbv5yxdb4ryrdtrld1"));
	layout.Add(Text("Hello Worxrtnmuxcftbyuxe5byxdgsdfgsdfgsdfvyxed5ybdr6undr6ld2"));
	layout.Add(Text("Hello Wor6ubndr6unbdr6unxr5ubdfgsdfgsdfgsdfgdasfgsdfgsdfgdsfgsdfgsdfgsdfgr6uld3"));
	layout.Add(Text("Hello Worburc56undr6ic6rtunr6cnudr56unxr56byuvdr5uybdr6bnucr6ubld4"));
	layout.Add(Text("Hello Worasdfgasedfghsfghsfghsdfgaergasergsdgfhaerybseyvsertybsevvyse5yvse5yves5ybvae5ld1"));
	layout.Add(Text("Hello Worldae;rkfmgls;kryvser5yvedryvzw4ytbvwz45yvse5yvbae5vfgnhmkls;dfgnksdjfgn2"));
	layout.Add(Text("Hello Worlakjsdfbngjksdbfngjkbyase5yse5yvaewy45vbwz$vyaew4ybvaew45vyhsbdfnghd3"));
	layout.Add(Text("Hello Worrytgjdtghjmfdghjdawe54yse5vyev5yazew4yae5yae5yvtyjdryld4"));
	layout.Add(Text("Hello Wojedrtyjdtyjedtyjghj5vyae5vyes5vye5yes5ysfcghkjfghjcghjcfghjrld1"));
	layout.Add(Text("Hello Wobhklfuksyghjfdhjncghm5yes5yes5vyesvghjk,vhj,lgvhkj,rld2"));
	layout.Add(Text("Hello Worasrfgserhsrtghsrtgh5vyes5yse5vsertghsrtghdfghjdgfhld3"));
	layout.Add(Text("Hello Wozerdyxnrftunrtjucmfgyyes5yes5vyase5cyaw4uicmryikcrld4"));
	layout.Add(Text("Hello Wormtguyicng7iycr,6ix,tvaw4ctvaw4r 76int7icld1"));
	layout.Add(Text("Hello Wortm,uof7ymuofny7uidtbntae4tyuidtrm76idmt,7ift,c7iufmnt7ild2"));
	layout.Add(Text("Hello Wornft7inft7nift7e45ybves5vyaes5vycte5vcytae5nild3"));
	layout.Add(Text("Hello Worr7imft7ict7yaev5yave5yae5yaes5cae5vtaectaw4ctvae4vteld4"));
	layout.Add(Text("Hello Worictrm7ict745taw5e4vctae4te5teiftc7ictyld1"));
	layout.Add(Text("Hello Wornct7ift7yinmft7iusdfgsdfgfnt7idbt7inft7ild2"));
	layout.Add(Text("Hello Worixbudxrn5yze4m8uxsfgsfsfgsfgsr56mucrftbzex5ybe5mucr6ticf ,yunbxr5ubld3"));
	layout.Add(Text("Hello Worbxr65ubrcx6ixr6budfgsdfgsdfgsdfgsdfgvr5urx6mjvzeld4"));
	layout.Add(Text("Hello Wor5uze5nudt6midt7ibsdfgsdfgsdxe5uzbe5unld1"));
	layout.Add(Text("Hello Worldr5udr5uyxer5uybxefgsdfgsdfgsdfgsdd5nuxr6isrvbusr5tnbusrx6d2"));
	layout.Add(Text("Hello Worrex65icntiycb6uxevfgsdfgsdfgsdfgsdfgsdfgsdfgsdfgsdfgsdfgsdfgsdfgsdfgsdf5buxer5ld3"));
	layout.Add(Text("Hello Wornixr6ubvex5uybxdrtucsdfgsdfgdsfgsdfgsdfgsdfgsdfgsdfg6rtimcr6tubld4"));
	layout.Add(Text("Hello Woderyhdrnuhcmftuxe5bnuze45byaew4ymcr6i,xryxbv5yxdb4ryrdtrld1"));
	layout.Add(Text("Hello Worxrtnmuxcftbyuxe5byxdgsdfgsdfgsdfvyxed5ybdr6undr6ld2"));
	layout.Add(Text("Hello Wor6ubndr6unbdr6unxr5ubdfgsdfgsdfgsdfgdasfgsdfgsdfgdsfgsdfgsdfgsdfgr6uld3"));
	layout.Add(Text("Hello Worburc56undr6ic6rtunr6cnudr56unxr56byuvdr5uybdr6bnucr6ubld4"));

	layout.Run();
	Console::ReadKey();
}