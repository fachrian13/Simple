#include "Simple.h"

using namespace Simple::System;
using namespace Simple::Utility;

constexpr ConsoleColor DefaultColor = { Color::Black, Color::White };

class Pixel
{
private:
	char Character;
	ConsoleColor Color;

public:
	friend bool operator==(const Pixel& first, const Pixel& second) { return first.Character == second.Character && first.Color == second.Color; }

	friend bool operator!=(const Pixel& first, const Pixel& second) { return !(first == second); }

public:
	Pixel(ConsoleColor color, char character = ' ') : Character(character), Color(color) {}

	std::string ToString() const { return this->Color.ToString() + this->Character; }

	void SetColor(ConsoleColor color) { this->Color = color; }
};

class Line
{
private:
	int Width;
	ConsoleColor Color;
	std::vector<Pixel> Pixels;

public:
	class : public ReadOnlyProperty<int> { friend class Line; } Size;

	friend bool operator==(const Line& first, const Line& second) { return first.Pixels == second.Pixels; }

	friend bool operator!=(const Line& first, const Line& second) { return !(first == second); }

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
public:
	int Width;
	int Height;
	ConsoleColor Color;

private:
	std::vector<Line> Lines;

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

public:
	Text(std::string value, ConsoleColor color = DefaultColor) : Content(value), Color(color) {}

	Canvas Render(int width) const override
	{
		std::vector<std::string> sContent = Tools::Split(this->Content, '\n');
		int contentSize = static_cast<int>(sContent.size());
		Canvas tCanvas(width, contentSize);
		Line tLine(width, this->Color);

		for (int i = 0; i < contentSize; i++)
		{
			for (size_t j = 0; j < (sContent[i].size() <= width ? sContent[i].size() : width); j++)
				tLine.PixelAt(j) = Pixel(this->Color, sContent[i][j]);
			tCanvas.LineAt(i) = tLine;
		}
		return tCanvas;
	}
};

class Dropdown : public Canvas
{
private:
	struct
	{
		int Begin;
		int Current;
		int End;
		int Limit;
	} Index;
	ConsoleColor Focus = { Color::White, Color::Black };
	std::vector<std::shared_ptr<Component>> Components;

private:
	void Compute()
	{
		int currentLine = 0;
		this->Index.Limit = this->Index.Begin + this->Height;

		for (int i = this->Index.Begin; i < this->Index.Limit; i++)
		{
			Canvas tCanvas = this->Components[i]->Render(this->Width);

			if (i == this->Index.Current)
				tCanvas.SetColor(this->Focus);

			for (int j = 0; j < tCanvas.Size; j++, currentLine++)
				if (this->LineAt(currentLine) != tCanvas.LineAt(j)) this->LineAt(currentLine) = tCanvas.LineAt(j);
		}
	}

public:
	Dropdown(int width, int height) : Canvas(width, height)
	{
		this->Index = { 0, 0, -1, height };
	}

	template<class T>
	void Add(T value)
	{
		this->Components.push_back(std::make_shared<T>(value));
		this->Index.End++;
	}

	void Draw()
	{
		this->Compute();
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

	layout.Add(Text("Hello Worasdfgasedfghsfghsfghsdfgaergasergsdgfhaerybseyvsertybsevvyse5yvse5yves5ybvae5ld1"));
	layout.Add(Text("Hello Worldae;rkfmgls;kryvser5yvedryvzw4ytbvwz45yvse5yvbae5vfgnhmkls;dfgnksdjfgn2"));
	layout.Add(Text("Hello Worlakjsdfbngjksdbfngjkbyase5yse5yvaewy4w45vyhsbdfnghd3", { Color::Red, Color::Blue }));
	layout.Add(Text("Hello Worrytgjdtghjmfdghjdawe54yse5vyev5yazew4yaffffffffffffffffffffddddddddddddfffffffffe5yae5yvtyjdryld4"));
	layout.Add(Text("Hello Wojedrtyjdtyjedtyjghj5vyae5vyes5vye5yes5ysfcghkjfghjcghjcfghjrld1"));
	layout.Add(Text("Hello Wobhklfuksyghjfdhjncghm5yes5yes5vyesvghjk,vhj,lgvhkj,rld2"));
	layout.Add(Text("Hello Worasrfgserhsrtghsrtgh5vyes5yse5vsertghsrtghdfghjdgfhld3"));
	layout.Add(Text("Hello Wozerdyxnrftunrtjucmfgyyes5yes5vyffffffffffffffffffffffffffffffffffffffffase5cyaw4uicmryikcrld4", { Color::Red, Color::Blue }));
	layout.Add(Text("Hello Wormtguyicng7iycr,6ix,tvaw4ctvaw4r 76int7icld1"));
	layout.Add(Text("Hello Wortm,uof7ymuofny7uidtbntae4tyuidtrm76idmt,7ift,c7iufmnt7ild2"));
	layout.Add(Text("Hello Wornft7inft7nift7e45ybves5vyaes5vycte5vcytae5nild3", { Color::Red, Color::Blue }));
	layout.Add(Text("Hello Worr7imft7ict7yaev5yave5ffffffffffffffffffffffffffffffffffyae5yaes5cae5vtaectaw4ctvae4vteld4"));
	layout.Add(Text("Hello Worictrm7ict745taw5e4vctae4te5teiftc7ictyld1"));
	layout.Add(Text("Hello Wornct7ift7yinmft7iusdfgsdfgfnt7idbt7inft7ild2"));
	layout.Add(Text("Hello Worixbudxrn5yze4m8uxsfgsfsfgsffffffffffffffffffffffffffffffffffffgsr56mucrftbzex5ybe5mucr6ticf ,yunbxr5ubld3", { Color::Red, Color::Blue }));
	layout.Add(Text("Hello Worbxr65ubrcx6ixr6budfgsdfgsdfgsdfgsdfgvr5urx6mjvzeld4"));
	layout.Add(Text("Hello Wor5uze5nudt6midt7ibsdfgsdfgsdxe5uzbe5unld1"));
	layout.Add(Text("Hello Worldr5udr5uyxer5uybxefgsdfgsdfgsdfgsdd5nuxr6isrvbusr5tnbusrx6d2", { Color::Red, Color::Blue }));
	layout.Add(Text("Hello Worrex65icntiycb6uxevfgsdfgsdfgsdfgsdfgsdfgsdfgsdfgsdfgsdfgsdfgsdfgsdfgsdf5buxer5ld3"));
	layout.Add(Text("Hello Wornixr6ubvex5uybxdrtufffffffffffffffffffffffffffffcsdfgsdfgdsfgsdfgsdfgsdfgsdfgsdfg6rtimcr6tubld4"));
	layout.Add(Text("Hello Woderyhdrnuhcmftuxe5bnuze45byaew4ymcr6i,xryxbv5yxdb4ryrdtrld1"));
	layout.Add(Text("Hello Worxrtnmuxcftbyuxe5byxdgsdfgsdfgsdfvyxed5ybdr6undr6ld2", { Color::Green, Color::Yellow }));
	layout.Add(Text("Hello Wor6ubndr6unbdr6unxr5ubdfgsdfgsdfgsdfgdasfgsdfgsdfgdsfgsdfgsdfgsdfgr6uld3"));
	layout.Add(Text("Hello Worburc56undr6ic6rtunr6cnudr56unxr56byuvdr5uybdr6bnucr6ubld4", { Color::Green, Color::Yellow }));
	layout.Add(Text("Hello Worasdfgasedfghsfghsfghsdfffffffffffffffgaergasergsdgfhaerybseyvsertybsevvyse5yvse5yves5ybvae5ld1"));
	layout.Add(Text("Hello Worldae;rkfmgls;kryvser5yvedryvzw4ytbvwz45yvse5yvbae5vfgnhmkls;dfgnksdjfgn2"));
	layout.Add(Text("Hello Worlakjsdfbngjksdbfngjkbyase5yse5yvaewy45vbwz$vyaew4ybvaew45vyhsbdfnghd3"));
	layout.Add(Text("Hello Worrytgjdtghjmfdghjdaffffffffffffffffffffffffffffffffwe54yse5vyev5yazew4yae5yae5yvtyjdryld4", { Color::Green, Color::Yellow }));
	layout.Add(Text("Hello Wojedrtyjdtyjedtyjghj5vyae5vyes5vye5yes5ysfcghkjfghjcghjcfghjrld1"));
	layout.Add(Text("Hello Wobhklfuksyghjfdhjncghfffffffffffffffffffm5yes5yes5vyesvghjk,vhj,lgvhkj,rld2", { Color::Green, Color::Yellow }));
	layout.Add(Text("Hello Worasrfgserhsrtghsrtgh5vyes5yse5vsertghsrtghdfghjdgfhld3"));
	layout.Add(Text("Hello Wozerdyxnrftunrtjucmfgyyes5yffffffffffffffffffffffffffffffes5vyase5cyaw4uicmryikcrld4"));
	layout.Add(Text("Hello Wormtguyicng7iycr,6ix,tvaw4ctvaw4r 76int7icld1"));
	layout.Add(Text("Hello Wortm,uof7ymuofny7uidtbntae4tyuidtrm76idmt,7ift,c7iufmnt7ild2", { Color::Green, Color::Yellow }));
	layout.Add(Text("Hello Wornft7inft7nift7e45ybves5vyaes5vycte5vcytae5nild3"));
	layout.Add(Text("Hello Worr7imft7ict7yaev5yave5yaffffffffffffffffffffffffffffffffe5yaes5cae5vtaectaw4ctvae4vteld4"));
	layout.Add(Text("Hello Worictrm7ict745taw5e4vctae4te5teiftc7ictyld1"));
	layout.Add(Text("Hello Wornct7ift7yinmft7iusdfgsdfgfnt7idbt7inft7ild2"));
	layout.Add(Text("Hello Worixbudxrn5yze4m8uxsfffffffffffffffffffffgsfsfgsfgsr56mucrftbzex5ybe5mucr6ticf ,yunbxr5ubld3", { Color::Green, Color::Yellow }));
	layout.Add(Text("Hello Worbxr65ubrcx6ixr6budfgsdfgsdfgsdfgsdfgvr5urx6mjvzeld4"));
	layout.Add(Text("Hello Wor5uze5nudt6midt7ibsdfgsdfgsdxe5uzbe5unld1"));
	layout.Add(Text("Hello Worldr5udr5uyxer5uybxefgsdfgsdfgsdfgsdd5nuxr6isrvbusr5tnbusrx6d2"));
	layout.Add(Text("Hello Worrex65icntiycb6uxevfgsdfgsdfgsdfgsdfgsdfgsdfgsdfgsdfgsdfgsdfgsdfgsdfgsdf5buxer5ld3", { Color::Green, Color::Yellow }));
	layout.Add(Text("Hello Wornixr6ubvex5uybxdrtucsdfgsdfgdsfgsdfgsdfgsdfgsdfgsdfg6rtimcr6tubld4"));
	layout.Add(Text("Hello Woderyhdrnuhcmftuxe5bnuze45byaew4ymcr6i,xryxbv5yxdb4ryrdtrld1"));
	layout.Add(Text("Hello Worxrtnmuxcftbyuxe5byxdgsdfgsdfgsdfvyxed5ybdr6undr6ld2"));
	layout.Add(Text("Hello Wor6ubndr6unbdr6unxr5ubdfgsdfgsdfgsdfgdasfgsdfgsdfgdsfgsdfgsdfgsdfgr6uld3"));
	layout.Add(Text("Hello Worburc56undr6ic6rtunr6cnudr56unxr56byuvdr5uybdr6bnucr6ubld4", { Color::Green, Color::Yellow }));
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