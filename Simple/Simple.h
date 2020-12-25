/* Fahri Synyster
* Penjelasan:
*	Tujuan pembuatan library ini yaitu untuk pembelajaran.
*	Masih terdapat bug didalam library ini, jadi berhati-hatilah
*	saat menggunakannya. Tidak semua kode yang terdapat disini
*	buatan sendiri, namun ada juga yang diambil dari beberapa
*	sumber di internet. Jika ingin menambahkan, mengurangi, atau
*	memodifikasi kode silakan, saya harap dengan kontribusi anda
*	library ini menjadi lebih baik lagi.
* 
* Required C++17
* Written in Visual Studio 2019
*/

#pragma once
#ifndef _SIMPLE_
#define _SIMPLE_

#define NOMINMAX

#include <chrono>
#include <conio.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <string>
#include <thread>
#include <time.h>
#include <vector>
#include <windows.h>

namespace Simple {
	using BufferInfo	= CONSOLE_SCREEN_BUFFER_INFO;
	using Cstring		= const char*;
	using Cwstring		= const wchar_t*;
	using Coord			= COORD;
	using CursorInfo	= CONSOLE_CURSOR_INFO;
	using Dword			= DWORD;
	using FileStream	= std::fstream;
	using FontInfo		= CONSOLE_FONT_INFOEX;
	template<typename Type>
	using Greater		= std::greater<Type>;
	using Handle		= HANDLE;
	using Hwnd			= HWND;
	using InFileStream	= std::ifstream;
	template<typename Type>
	using Initializer	= std::initializer_list<Type>;
	using Int8			= char;
	using Int16			= short;
	using Int32			= int;
	using Int64			= long long;
	using InStream		= std::istream;
	template<typename Type>
	using Less			= std::less<Type>;
	template<typename type1, typename type2>
	using Map			= std::map<type1, type2>;
	using Mode			= std::ios;
	using OutFileStream	= std::ofstream;
	using OutStream		= std::ostream;
	using Path			= std::filesystem::path;
	using Rect			= RECT;
	using Regex			= std::regex;
	using SmallRect		= SMALL_RECT;
	using String		= std::string;
	using StringStream	= std::stringstream;
	using Uint8			= unsigned char;
	using Uint16		= unsigned short;
	using Uint32		= unsigned int;
	using Uint64		= unsigned long long;
	template<typename Type>
	using Vector		= std::vector<Type>;
}

namespace Simple::System {
	enum class Color {
		Black		= 0,
		DarkBlue	= 1,
		DarkGreen	= 2,
		DarkCyan	= 3,
		DarkRed		= 4,
		DarkMagenta	= 5,
		DarkYellow	= 6,
		Gray		= 7,
		DarkGray	= 8,
		Blue		= 9,
		Green		= 10,
		Cyan		= 11,
		Red			= 12,
		Magenta		= 13,
		Yellow		= 14,
		White		= 15
	};
}
namespace Simple::System {
	struct ConsoleColor {
		Color Background;
		Color Foreground;
	};
}
namespace Simple::System {
	class Exception {
	private:
		int $Line;
		Cstring $File;
		Cstring $Function;
		Cstring $Description;

	public:
		Exception(Cstring file, int line, Cstring function, Cstring description)
			:$File(file), $Line(line), $Function(function), $Description(description) {}
		virtual ~Exception() throw() {}
		Cstring Description() const { return $Description; }
		int Line() const { return $Line; }
		Cstring File() const { return $File; }
		Cstring Function() const { return $Function; }
	};

#define Error(d) throw Exception(__FILE__, __LINE__, __FUNCTION__, d)
}
namespace Simple::System {
	template<typename Type>
	class BinaryFile {
	private:
		Path $FileName;
		FileStream $File;

	public:
		BinaryFile() :$FileName("BinaryFile.bin") {
			$File.open($FileName.c_str(), Mode::app);
			$File.close();
		}
		BinaryFile(Cstring fileName) :$FileName(fileName) {
			if ($FileName.empty())
				$FileName = "BinaryFile.bin";
			else if (!$FileName.has_extension())
				$FileName += ".bin";
			else if ($FileName.extension() != ".bin")
				$FileName.replace_extension(".bin");

			$File.open($FileName.c_str(), Mode::app);
			$File.close();
		}
		void Delete(Uint32 position) {
			Vector<Type> stored = Read();
			stored.erase(stored.begin() + position);

			$File.open($FileName.c_str(), Mode::out | Mode::binary);
			if (!$File)
				Error("Gagal membuka file.");
			for (Type index : stored)
				$File.write((char*)&index, sizeof(Type));
			$File.close();
		}
		Vector<Type> Read() {
			Type temp;
			Vector<Type> stored;

			$File.open($FileName.c_str(), Mode::in | Mode::binary);
			if (!$File)
				Error("Gagal membuka file.");
			while ($File.read((char*)&temp, sizeof(Type)))
				stored.push_back(temp);
			$File.close();

			return stored;
		}
		void Update(Uint32 position, Type newData) {
			$File.open($FileName.c_str(), Mode::in | Mode::out | Mode::binary);
			if (!$File)
				Error("Gagal membuka file.");
			$File.seekp(position * sizeof(Type), Mode::beg);
			$File.write((char*)&newData, sizeof(Type));
			$File.close();
		}
		void Write(Type value) {
			$File.open($FileName.c_str(), Mode::out | Mode::binary | Mode::app);
			if (!$File)
				Error("Gagal membuka file.");
			$File.write((char*)&value, sizeof(Type));
			$File.close();
		}
	};
}
namespace Simple::System {
	class Console {
	protected:
		static BufferInfo $Buffer;
		static CursorInfo $Cursor;
		static Handle $Handle;
		static Hwnd $Hwnd;
		static Map<int, Color> $Color;

	public:
		static void Clear() {
			system("cls");
		}
		static void CursorVisible(bool visible) {
			if (!GetConsoleCursorInfo($Handle, &$Cursor))
				Error("Gagal mendapatkan informasi cursor.");
			$Cursor.bVisible = visible;
			if (!SetConsoleCursorInfo($Handle, &$Cursor))
				Error("Gagal mengatur ekstensi cursor.");
		}
		static void DisableCloseButton() {
			if (!EnableMenuItem(GetSystemMenu(GetConsoleWindow(), false), SC_CLOSE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED))
				Error("Gagal menonaktifkan tombol close.");
		}
		static void DisableMaximizeButton() {
			SetWindowLong($Hwnd, GWL_STYLE, GetWindowLong($Hwnd, GWL_STYLE) & ~WS_MAXIMIZEBOX);
		}
		static void DisableMinimizeButton() {
			SetWindowLong($Hwnd, GWL_STYLE, GetWindowLong($Hwnd, GWL_STYLE) & ~WS_MINIMIZEBOX);
		}
		static void DisableResize() {
			SetWindowLong($Hwnd, GWL_STYLE, GetWindowLong($Hwnd, GWL_STYLE) & ~WS_SIZEBOX);
		}
		static Int32 Get() {
			return std::cin.get();
		}
		static Coord GetBufferSize() {
			if (!GetConsoleScreenBufferInfo($Handle, &$Buffer))
				Error("Gagal mendapatkan informasi buffer.");

			return $Buffer.dwSize;
		}
		static Coord GetCursorPosition() {
			if (!GetConsoleScreenBufferInfo($Handle, &$Buffer))
				Error("Gagal mendapatkan informasi buffer.");

			return $Buffer.dwCursorPosition;
		}
		static Int32 GetKey() {
			return _getch();
		}
		static String GetLine() {
			String result;

			std::getline(std::cin, result);

			return result;
		}
		static ConsoleColor GetTextColor() {
			if (!GetConsoleScreenBufferInfo($Handle, &$Buffer))
				Error("Gagal mendapatkan informasi buffer.");

			return{
				$Color.find($Buffer.wAttributes / 16)->second,
				$Color.find($Buffer.wAttributes % 16)->second
			};
		}
		static void Sleep(Int64 milliseconds) {
			std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
		}
		template<typename... Type>
		static void Print(Type... value) {
			((std::cout << value), ...);
		}
		template<typename... Type>
		static void PrintLine(Type... value) {
			((std::cout << value), ...) << "\n";
		}
		static void SetBufferSize(Coord size) {
			if (!SetConsoleScreenBufferSize($Handle, size))
				Error("Gagal mengatur ukuran buffer.");
		}
		static void SetBufferSize(Int16 width, Int16 height) {
			if (!SetConsoleScreenBufferSize($Handle, { width, height }))
				Error("Gagal mengatur ukuran buffer.");
		}
		static void SetCursorPosition(Coord position) {
			if (!SetConsoleCursorPosition($Handle, position))
				Error("Gagal mengatur posisi cursor.");
		}
		static void SetCursorPosition(Int16 x, Int16 y) {
			if (!SetConsoleCursorPosition($Handle, { x, y }))
				Error("Gagal mengatur posisi cursor.");
		}
		static void SetCursorSize(Int16 size) {
			if (size < 0 || size>100)
				Error("Ukuran cursor diluar batas.");

			CursorInfo cursor;

			if (!GetConsoleCursorInfo($Handle, &cursor))
				Error("Gagal mendapatkan informasi cursor.");
			cursor.dwSize = size;
			if (!SetConsoleCursorInfo($Handle, &cursor))
				Error("Gagal mengatur ukuran cursor");
		}
		static void SetFont(Cwstring faceName, Int16 fontSize) {
			FontInfo font{
				sizeof(FontInfo),
				0,
				{0, fontSize},
				FF_DONTCARE,
				FW_NORMAL,
			};

			wcscpy_s(font.FaceName, faceName);
			if (!SetCurrentConsoleFontEx($Handle, false, &font))
				Error("Gagal mengatur gaya huruf.");
		}
		static void SetPositionToCenter() {
			Rect rScreen;
			Hwnd hScreen = GetDesktopWindow();
			if (!GetWindowRect(hScreen, &rScreen))
				Error("Gagal mendapatkan ukuran layar.");

			Rect rWindow;
			Hwnd hWindow = GetForegroundWindow();
			if (!GetWindowRect(hWindow, &rWindow))
				Error("Gagal mendapatkan ukuran window.");

			Int32 width		= rWindow.right - rWindow.left;
			Int32 height	= rWindow.bottom - rWindow.top;
			Int32 x			= ((rScreen.right - rScreen.left) / 2 - width / 2);
			Int32 y			= ((rScreen.bottom - rScreen.top) / 2 - height / 2);

			if (!SetWindowPos(hWindow, 0, x, y, width, height, SWP_NOSIZE | SWP_NOZORDER))
				Error("Gagal mengatur posisi console.");
		}
		static void SetSize(Coord size) {
			Coord max = GetLargestConsoleWindowSize($Handle);
			if (size.X > max.X)
				Error("Ukuran lebar diluar batas.");
			else if (size.Y > max.Y)
				Error("Ukuran tinggi diluar batas.");

			if (!GetConsoleScreenBufferInfo($Handle, &$Buffer))
				Error("Gagal mendapatkan informasi buffer.");

			SmallRect wInfo = $Buffer.srWindow;
			Coord wSize{
				wInfo.Right - wInfo.Left + 1,
				wInfo.Bottom - wInfo.Top + 1
			};

			if (wSize.X > size.X || wSize.Y > size.Y) {
				SmallRect window{
					0,
					0,
					size.X < wSize.X ? size.X - 1 : wSize.X - 1,
					size.Y < wSize.Y ? size.Y - 1 : wSize.Y - 1
				};

				if (!SetConsoleWindowInfo($Handle, true, &window))
					Error("Gagal mengatur ukuran window.");
			}

			SetBufferSize(size);

			SmallRect window{
				0,
				0,
				size.X - 1,
				size.Y - 1
			};

			if (!SetConsoleWindowInfo($Handle, true, &window))
				Error("Gagal mengatur ukuran window.");
		}
		static void SetSize(Int16 width, Int16 height) {
			Coord max = GetLargestConsoleWindowSize($Handle);
			if (width > max.X)
				Error("Ukuran lebar diluar batas.");
			else if (height > max.Y)
				Error("Ukuran tinggi diluar batas.");

			if (!GetConsoleScreenBufferInfo($Handle, &$Buffer))
				Error("Gagal mendapatkan informasi buffer.");

			SmallRect wInfo = $Buffer.srWindow;
			Coord wSize{
				wInfo.Right - wInfo.Left + 1,
				wInfo.Bottom - wInfo.Top + 1
			};

			if (wSize.X > width || wSize.Y > height) {
				SmallRect window{
					0,
					0,
					width < wSize.X ? width - 1 : wSize.X - 1,
					height < wSize.Y ? height - 1 : wSize.Y - 1
				};

				if (!SetConsoleWindowInfo($Handle, true, &window))
					Error("Gagal mengatur ukuran window.");
			}

			SetBufferSize(width, height);

			SmallRect window{
				0,
				0,
				width - 1,
				height - 1
			};

			if (!SetConsoleWindowInfo($Handle, true, &window))
				Error("Gagal mengatur ukuran window.");
		}
		static void SetTextColor(ConsoleColor color) {
			if (!SetConsoleTextAttribute($Handle, (Int16)color.Background << 4 | (Int16)color.Foreground))
				Error("Gagal mengatur warna teks.");
		}
		static void SetTextColor(Color background, Color foreground) {
			if (!SetConsoleTextAttribute($Handle, (Int16)background << 4 | (Int16)foreground))
				Error("Gagal mengatur warna teks.");
		}
	};

	BufferInfo Console::$Buffer{};
	CursorInfo Console::$Cursor{};
	Handle Console::$Handle = GetStdHandle(STD_OUTPUT_HANDLE);
	Hwnd Console::$Hwnd = GetConsoleWindow();
	Map<int, Color> Console::$Color{
		{ 0,	Color::Black		},
		{ 1,	Color::DarkBlue		},
		{ 2,	Color::DarkGreen	},
		{ 3,	Color::DarkCyan		},
		{ 4,	Color::DarkRed		},
		{ 5,	Color::DarkMagenta	},
		{ 6,	Color::DarkYellow	},
		{ 7,	Color::Gray			},
		{ 8,	Color::DarkGray		},
		{ 9,	Color::Blue			},
		{ 10,	Color::Green		},
		{ 11,	Color::Cyan			},
		{ 12,	Color::Red			},
		{ 13,	Color::Magenta		},
		{ 14,	Color::Yellow		},
		{ 15,	Color::White		}
	};
}
namespace Simple::System {
	struct DateTime {
		Int32 Second;
		Int32 Minute;
		Int32 Hour;
		Int32 DayOfTheWeek;
		Int32 DayOfTheMonth;
		Int32 DayOfTheYear;
		Int32 Month;
		Int32 Year;
		Int32 Dst;
		String DayShort;
		String DayLong;
		String MonthShort;
		String MonthLong;

		friend OutStream& operator<<(OutStream& out, DateTime value) {
			out << value.DayOfTheMonth << "/" << value.Month << "/" << value.Year << " ";
			value.Hour		< 10 ? out << 0 << value.Hour	<< ":"	: out << value.Hour		<< ":";
			value.Minute	< 10 ? out << 0 << value.Minute << ":"	: out << value.Minute	<< ":";
			value.Second	< 10 ? out << 0 << value.Second			: out << value.Second;

			return out;
		}
	};
}
namespace Simple::System {
	class Time final {
	private:
		static const String $Days[];
		static const String $Months[];

	public:
		static DateTime Current() {
			time_t now = time(0);
			tm time;

			localtime_s(&time, &now);

			return {
				time.tm_sec,
				time.tm_min,
				time.tm_hour,
				0,
				0,
				0,
				0,
				0,
				time.tm_isdst,
				"",
				"",
				"",
				""
			};
		}
		static DateTime Now() {
			time_t now = time(0);
			tm time;

			localtime_s(&time, &now);

			return {
				time.tm_sec,
				time.tm_min,
				time.tm_hour,
				time.tm_wday,
				time.tm_mday,
				time.tm_yday,
				time.tm_mon + 1,
				time.tm_year + 1900,
				time.tm_isdst,
				$Days[time.tm_wday].substr(0, 3),
				$Days[time.tm_wday],
				$Months[time.tm_mon].substr(0, 3),
				$Months[time.tm_mon]
			};
		}
		static DateTime Today() {
			time_t now = time(0);
			tm time;

			localtime_s(&time, &now);

			return {
				0,
				0,
				0,
				time.tm_wday,
				time.tm_mday,
				time.tm_yday,
				time.tm_mon + 1,
				time.tm_year + 1900,
				0,
				$Days[time.tm_wday].substr(0, 3),
				$Days[time.tm_wday],
				$Months[time.tm_mon].substr(0, 3),
				$Months[time.tm_mon]
			};
		}
	};

	const String Time::$Days[]{
		"Sunday"	, "Monday", "Tuesday", "Wednesday",
		"Thursday"	, "Friday", "Saturday"
	};
	const String Time::$Months[]{
		"January"	, "February"	, "March"		, "April",
		"May"		, "June"		, "July"		, "August",
		"September"	, "October"		, "November"	, "December"
	};
}

namespace Simple::Utility {
	template<typename Type1, typename Type2>
	struct Result {
		Type1 First;
		Type2 Second;
	};
}
namespace Simple::Utility {
	enum class Message {
		Information = 0,
		Warning		= 1,
		Danger		= 2
	};
}
namespace Simple::Utility {
	class Cipher final {
	public:
		static String XOR(String value, char key) {
			String res;

			for (char index : value)
				res += index ^ key;

			return res;
		}
		static String Caesar(String value, int key) {
			String res;

			for (char index : value)
				if (isupper(index))
					res += char(int(index + key - 65) % 26 + 65);
				else
					res += char(int(index + key - 97) % 26 + 97);

			return res;
		}
		static String Vigenere(String value, String key) {
			String res;
			Uint32 keySize = key.size();

			for (Uint32 i = 0;; i++) {
				if (keySize == i)
					i = 0;
				if (key.size() >= value.size())
					break;

				key += key[i];
			}

			for (Uint32 i = 0; i < value.size(); i++) {
				char x = (value[i] + key[i]) % 26;
				
				x += 'A';
				res += x;
			}

			return res;
		}
	};
}
namespace Simple::Utility {
	class Tools final : private System::Console {
	private:
		using Color = System::Color;
		using ConsoleColor = System::ConsoleColor;
		using Exception = System::Exception;

	public:
		static void Clear(Coord position, String& value) {
			if (!value.empty()) {
				DeleteText(position, value.length());
				value.clear();
			}
		}
		static void Clear(Int16 x, Int16 y, String& value) {
			if (!value.empty()) {
				DeleteText(x, y, value.length());
				value.clear();
			}
		}
		static void DeleteText(Coord position, Uint32 length) {
			Dword ch;

			if (!GetConsoleScreenBufferInfo($Handle, &$Buffer))
				Error("Gagal mendapatkan informasi buffer.");
			if (!FillConsoleOutputCharacter($Handle, ' ', length, position, &ch))
				Error("Gagal menghapus teks.");
			if (!GetConsoleScreenBufferInfo($Handle, &$Buffer))
				Error("Gagal mendapatkan informasi buffer.");
			if (!FillConsoleOutputAttribute($Handle, $Buffer.wAttributes, length, position, &ch))
				Error("Gagal mengisi attribute.");
		}
		static void DeleteText(Int16 x, Int16 y, Uint32 length) {
			Dword ch;

			if (!GetConsoleScreenBufferInfo($Handle, &$Buffer))
				Error("Gagal mendapatkan informasi buffer.");
			if (!FillConsoleOutputCharacter($Handle, ' ', length, { x, y }, &ch))
				Error("Gagal menghapus teks.");
			if (!GetConsoleScreenBufferInfo($Handle, &$Buffer))
				Error("Gagal mendapatkan informasi buffer.");
			if (!FillConsoleOutputAttribute($Handle, $Buffer.wAttributes, length, { x, y }, &ch))
				Error("Gagal mengisi attribute.");
		}
		static String GenerateRandomKey(Uint32 numberOfKey) {
			const char keyList[] = "QWERTYUIOPASDFGHJKLZXCVBNM1234567890";
			String key;

			srand((Uint32)time(0));
			for (Uint32 i = 0; i < numberOfKey; i++)
				key += keyList[rand() % 36];

			return key;
		}
		static String GetPassword() {
			String password;
			char key;

			while ((key = Console::GetKey()) != '\r') {
				switch (key) {
				case 0:
				case 224:
					GetKey();
					break;
				case '\b':
					if (password.size() > 0) {
						password.erase(password.end() - 1);
						Console::Print("\b \b");
					}
					break;
				default:
					password += key;
					Console::Print("*");
				}
			}

			return password;
		}
		static bool IsNumber(String& value) {
			return !value.empty() && std::find_if(value.begin(), value.end(), [](Uint16 ch) {return !std::isdigit(ch); }) == value.end();
		}
		template<typename Type>
		static Type Max(Type a, Type b) {
			return a > b ? a : b;
		}
		template<typename Type>
		static Type Min(Type a, Type b) {
			return a < b ? a : b;
		}
		template<typename... Type>
		static void Print(Coord position, Type... value) {
			SetCursorPosition(position);
			Console::Print(value...);
		}
		template<typename... Type>
		static void Print(Int16 x, Int16 y, Type... value) {
			SetCursorPosition(x, y);
			Console::Print(value...);
		}
		template<typename... Type>
		static void PrintColor(ConsoleColor color, Type... value) {
			ConsoleColor defaultColor = Console::GetTextColor();

			Console::SetTextColor(color);
			Console::Print(value...);
			Console::SetTextColor(defaultColor);
		}
		template<typename... Type>
		static void PrintColor(Color background, Color foreground, Type... value) {
			ConsoleColor defaultColor = Console::GetTextColor();

			Console::SetTextColor(background, foreground);
			Console::Print(value...);
			Console::SetTextColor(defaultColor);
		}
		template<typename... Type>
		static void PrintColor(Coord position, ConsoleColor color, Type... value) {
			ConsoleColor defaultColor = Console::GetTextColor();

			Console::SetTextColor(color);
			Console::SetCursorPosition(position);
			Console::Print(value...);
			Console::SetTextColor(defaultColor);
		}
		template<typename... Type>
		static void PrintColor(Int16 x, Int16 y, Color background, Color foreground, Type... value) {
			ConsoleColor defaultColor = Console::GetTextColor();

			Console::SetTextColor(background, foreground);
			Console::SetCursorPosition(x, y);
			Console::Print(value...);
			Console::SetTextColor(defaultColor);
		}
		template<typename... Type>
		static void PrintLineColor(ConsoleColor color, Type... value) {
			ConsoleColor defaultColor = Console::GetTextColor();

			Console::SetTextColor(color);
			Console::PrintLine(value...);
			Console::SetTextColor(defaultColor);
		}
		template<typename... Type>
		static void PrintLineColor(Color background, Color foreground, Type... value) {
			ConsoleColor defaultColor = Console::GetTextColor();

			Console::SetTextColor(background, foreground);
			Console::PrintLine(value...);
			Console::SetTextColor(defaultColor);
		}
		static void PrintMessage(Coord position, Message type, String message) {
			String level[]{ "[INFORMATION]", "[WARNING]", "[DANGER]" };

			switch (type) {
			case Message::Information:
				PrintColor(position, { Color::Cyan, Color::Black }, level[(Int16)type]);
				break;
			case Message::Warning:
				PrintColor(position, { Color::DarkYellow, Color::Black }, level[(Int16)type]);
				break;
			case Message::Danger:
				PrintColor(position, { Color::DarkRed, Color::Black }, level[(Int16)type]);
				break;
			}

			Console::Print(" ", message);
			Console::GetKey();
			DeleteText(position, level[(Int16)type].length() + message.length() + 1);
		}
		static void PrintMessage(Int16 x, Int16 y, Message type, String message) {
			String level[]{ "[INFORMATION]", "[WARNING]", "[DANGER]" };

			switch (type) {
			case Message::Information:
				PrintColor(x, y, Color::Cyan, Color::Black, level[(Int16)type]);
				break;
			case Message::Warning:
				PrintColor(x, y, Color::DarkYellow, Color::Black, level[(Int16)type]);
				break;
			case Message::Danger:
				PrintColor(x, y, Color::DarkRed, Color::Black, level[(Int16)type]);
				break;
			}

			Console::Print(" ", message);
			Console::GetKey();
			DeleteText(x, y, level[(Int16)type].length() + message.length() + 1);
		}
	};
}
#endif // !_SIMPLE_