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

#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <regex>
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
	template<typename type>
	using Greater		= std::greater<type>;
	using Handle		= HANDLE;
	using Hwnd			= HWND;
	using InFileStream	= std::ifstream;
	template<typename type>
	using Initializer	= std::initializer_list<type>;
	using Int8			= char;
	using Int16			= short;
	using Int32			= int;
	using Int64			= long long;
	using InStream		= std::istream;
	template<typename type>
	using Less			= std::less<type>;
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
	template<typename type>
	using Vector		= std::vector<type>;
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
	template<typename type>
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
			Vector<type> stored = Read();
			stored.erase(stored.begin() + position);

			$File.open($FileName.c_str(), Mode::out | Mode::binary);
			if (!$File) Error("Gagal membuka file.");
			for (type index : stored) $File.write((char*)&index, sizeof(type));
			$File.close();
		}
		Vector<type> Read() {
			type temp;
			Vector<type> stored;

			$File.open($FileName.c_str(), Mode::in | Mode::binary);
			if (!$File) Error("Gagal membuka file.");
			while ($File.read((char*)&temp, sizeof(type)))stored.push_back(temp);
			$File.close();

			return stored;
		}
		void Update(Uint32 position, type newData) {
			$File.open($FileName.c_str(), Mode::in | Mode::out | Mode::binary);
			if (!$File) Error("Gagal membuka file.");
			$File.seekp(position * sizeof(type), Mode::beg);
			$File.write((char*)&newData, sizeof(type));
			$File.close();
		}
		void Write(type value) {
			$File.open($FileName.c_str(), Mode::out | Mode::binary | Mode::app);
			if (!$File) Error("Gagal membuka file.");
			$File.write((char*)&value, sizeof(type));
			$File.close();
		}
	};
}
#endif // !_SIMPLE_