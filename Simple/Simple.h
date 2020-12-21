/* Fahri Synyster
* Penjelasan:
*		Tujuan pembuatan library ini yaitu untuk pembelajaran.
*		Masih terdapat bug didalam library ini, jadi berhati-hatilah
*		saat menggunakannya. Tidak semua kode yang terdapat disini
*		buatan sendiri, namun ada juga yang diambil dari beberapa
*		sumber di internet. Jika ingin menambahkan, mengurangi, atau
*		memodifikasi kode silakan, saya harap dengan kontribusi anda
*		library ini menjadi lebih baik lagi.
* 
* Required C++17
* Written in Visual Studio 2019
*/

#pragma once
#ifndef _SIMPLE_
#define _SIMPLE_

#define NOMINMAX

#include <iostream>
#include <regex>
#include <vector>
#include <windows.h>

namespace Simple {
	using BufferInfo		= CONSOLE_SCREEN_BUFFER_INFO;
	using Cstring				= const char*;
	using Cwstring			= const wchar_t*;
	using Coord					= COORD;
	using CursorInfo		= CONSOLE_CURSOR_INFO;
	using Dword					= DWORD;
	using FileStream		= std::fstream;
	using FontInfo			= CONSOLE_FONT_INFOEX;
	template<typename type>
	using Greater				= std::greater<type>;
	using Handle				= HANDLE;
	using Hwnd					= HWND;
	using InFileStream	= std::ifstream;
	template<typename type>
	using Initializer		= std::initializer_list<type>;
	using Int8					= char;
	using Int16					= short;
	using Int32					= int;
	using Int64					= long long;
	using InStream			= std::istream;
	template<typename type>
	using Less					= std::less<type>;
	template<typename type1, typename type2>
	using Map						= std::map<type1, type2>;
	using Mode					= std::ios;
	using OutFileStream	= std::ofstream;
	using OutStream			= std::ostream;
	using Rect					= RECT;
	using Regex					= std::regex;
	using SmallRect			= SMALL_RECT;
	using String				= std::string;
	using StringStream	= std::stringstream;
	using Uint8					= unsigned char;
	using Uint16				= unsigned short;
	using Uint32				= unsigned int;
	using Uint64				= unsigned long long;
	template<typename type>
	using Vector				= std::vector<type>
}

#endif // !_SIMPLE_