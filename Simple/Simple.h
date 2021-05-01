#pragma once

#include <conio.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <time.h>
#include <windows.h>
#include <map>

namespace Simple {
	//
	// Mendeklarasikan kembali tipe data dengan nama lain.
	//
	using Int8 = char;
	using Int16 = short;
	using Int32 = int;
	using Int64 = long long;
	using UInt8 = unsigned char;
	using UInt16 = unsigned short;
	using UInt32 = unsigned int;
	using UInt64 = unsigned long long;
	using CString = const char*;
	using CWString = const wchar_t*;
	using String = std::string;
	using StringStream = std::stringstream;
	using StringWide = std::wstring;
	using SizeType = size_t;

	constexpr char NewLine = '\n';

	namespace System {
		//
		// Warna dasar pada console.
		//
		 const enum class Color {
			Black = 0,
			DarkBlue = 1,
			DarkGreen = 2,
			DarkCyan = 3,
			DarkRed = 4,
			DarkMagenta = 5,
			DarkYellow = 6,
			Gray = 7,
			DarkGray = 8,
			Blue = 9,
			Green = 10,
			Cyan = 11,
			Red = 12,
			Magenta = 13,
			Yellow = 14,
			White = 15
		};

		//
		// Menulis warna pada console.
		// 
		// @param out: Output stream untuk menulis kedalam console.
		// @param color: Warna yang akan ditulis.
		// 
		// @return std::ostream&: Output stream yang akan menulis warna.
		//
		std::ostream& operator<<(std::ostream& out, Color color) {
			switch (color)
			{
			case Color::Black: return out << "Black"; break;
			case Color::DarkBlue: return out << "DarkBlue"; break;
			case Color::DarkGreen: return out << "DarkGreen"; break;
			case Color::DarkCyan: return out << "DarkCyan"; break;
			case Color::DarkRed: return out << "DarkRed"; break;
			case Color::DarkMagenta: return out << "DarkMagenta"; break;
			case Color::DarkYellow: return out << "DarkYellow"; break;
			case Color::Gray: return out << "Gray"; break;
			case Color::DarkGray: return out << "DarkGray"; break;
			case Color::Blue: return out << "Blue"; break;
			case Color::Green: return out << "Green"; break;
			case Color::Cyan: return out << "Cyan"; break;
			case Color::Red: return out << "Red"; break;
			case Color::Magenta: return out << "Magenta"; break;
			case Color::Yellow: return out << "Yellow"; break;
			case Color::White: return out << "White"; break;
			default: return out << "Error";
			}
		}

		//
		// Struktur warna dasar pada console.
		//
		struct ConsoleColor {
			Color Background;
			Color Foreground;
		};

		//
		// Kelas untuk menangani error ketika program sedang berjalan.
		//
		class Exception {
		private:
			Int32 _Line;
			CString _File;
			CString _Function;
			CString _Description;

		public:
			Exception(CString file, Int32 line, CString function, CString description) : _File(file), _Line(line), _Function(function), _Description(description) {}

			//
			// Mengembalikan deskripsi error yang terjadi.
			// 
			// @return CString: Deskripsi.
			//
			CString Description() const {
				return _Description;
			}

			//
			// Mengembalikan nama file dimana error terjadi.
			// 
			// @return CString: Nama file.
			//
			CString File() const {
				return _File;
			}

			//
			// Mengembalikan nama fungsi dimana error terjadi.
			// 
			// @return CString: Nama fungsi.
			//
			CString Function() const {
				return _Function;
			}

			//
			// Mengembalikan baris dimana error terjadi.
			// 
			// @return Int32: Baris.
			//
			Int32 Line() const {
				return _Line;
			}
		};

		//
		// Untuk melempar Exception, dimana akan otomatis mengisi parameter sebagai berikut: 
		// throw Exception(__FILE__, __LINE__, __FUNCTION__, d).
		// 
		// @param d: Deskripsi/pesan error yang disampaikan.
		//
		#define Error(d) throw Exception(__FILE__, __LINE__, __FUNCTION__, d)

		//
		// Baca/tulis kedalam file menggunakan mode binary.
		//
		template<typename T>
		class BinaryFile {
		protected:
			std::filesystem::path _FileName;
			std::filesystem::path _FileTemp = "temp.dat";

			std::fstream _File;
			std::fstream _Temp;

		public:
			BinaryFile(CString fileName) : _FileName(fileName) {
				if (_FileName.empty())
					Error("Nama file tidak boleh kosong.");
				else if (!_FileName.has_extension())
					_FileName += ".bin";
				else if (_FileName.extension() != ".bin")
					_FileName.replace_extension(".bin");

				_File.open(_FileName, std::ios::out | std::ios::binary | std::ios::app);
				if (!_File)
					Error("Gagal membuka file.");
				_File.close();
			}

			//
			// Menghapus data dari file berdasarkan posisi/urutan saat ditulis.
			//
			void Delete(SizeType position) {
				#ifdef DeleteUsingVector
					std::vector<T> stored = Read(); stored.erase(stored.begin() + position);

					_File.open(_FileName, std::ios::out | std::ios::binary | std::ios::trunc);
					for (T index : stored)
						_File.write((char*)&index, sizeof T);

					_File.close();
				#else
					T temp;

					_File.open(_FileName, std::ios::in | std::ios::binary);
					if (!_File)
						Error("Gagal membuka file.");
					_Temp.open(_FileTemp, std::ios::out | std::ios::binary);
					if (!_Temp)
						Error("Gagal membuka temp file.");

					for (SizeType i = 0; _File.read((char*)&temp, sizeof T); i++)
						if (i != position)
							_Temp.write((char*)&temp, sizeof T);

					_File.close();
					_Temp.close();

					remove(_FileName);	
					rename(_FileTemp, _FileName);
				#endif
			}

			//
			// Mengecek apakah file dalam keadaan kosong atau tidak.
			// 
			// @return bool: [true] jika file kosong, sebaliknya [false].
			//
			bool Empty() {
				_File.open(_FileName, std::ios::in | std::ios::binary);
				if (!_File)
					Error("Gagal membuka file.");
				bool empty = _File.peek() == std::fstream::traits_type::eof();
				_File.close();

				return empty;
			}

			//
			// Membaca seluruh data dari dalam file, lalu menampungnya kedalam vector.
			// 
			// @return std::vector: Seluruh data yang telah dibaca.
			//
			std::vector<T> Read() {
				T temp;
				std::vector<T> stored;

				_File.open(_FileName, std::ios::in | std::ios::binary);
				if (!_File)
					Error("Gagal membuka file.");
				while (_File.read((char*)&temp, sizeof T))
					stored.push_back(temp);
				_File.close();

				return stored;
			}

			//
			// Memperbarui data didalam file berdasarkan posisi saat ditulis kedalam file.
			// 
			// @param position: Posisi data yang akan diupdate.
			// @param newData: Data baru yang akan ditulis.
			//
			void Update(SizeType position, T newData) {
				_File.open(_FileName, std::ios::in | std::ios::out | std::ios::binary);
				if (!_File)
					Error("Gagal membuka file.");
				_File.seekp(position * sizeof T, std::ios::beg);
				_File.write((char*)&newData, sizeof T);
				_File.close();
			}

			//
			// Menulis data kedalam file.
			// 
			// @param value: Data yang akan ditulis
			//
			void Write(T value) {
				_File.open(_FileName, std::ios::out | std::ios::binary | std::ios::app);
				if (_File)
					Error("Gagal membuka file");
				_File.write((char*)&value, sizeof T);
				_File.close();
			}
		};

		//
		// Terdapat method-method yang berhubungan dengan console.
		//
		class Console {
		protected:
			inline static CONSOLE_SCREEN_BUFFER_INFO _BufferInfo = {};
			inline static std::map<int, Color> _Color = {
				{ 0, Color::Black },
				{ 1, Color::DarkBlue },
				{ 2, Color::DarkGreen },
				{ 3, Color::DarkCyan },
				{ 4, Color::DarkRed },
				{ 5, Color::DarkMagenta },
				{ 6, Color::DarkYellow },
				{ 7, Color::Gray },
				{ 8, Color::DarkGray },
				{ 9, Color::Blue },
				{ 10, Color::Green },
				{ 11, Color::Cyan },
				{ 12, Color::Red },
				{ 13, Color::Magenta },
				{ 14, Color::Yellow },
				{ 15, Color::White}
			};
			inline static CONSOLE_CURSOR_INFO _CursorInfo = {};
			inline static HANDLE _Handle = GetStdHandle(STD_OUTPUT_HANDLE);
			inline static HWND _Hwnd = GetConsoleWindow();

		//
		// Ala-ala C# h3h3 
		//
		public:
			struct Buffer {
				//
				// Untuk mengatur warna buffer pada console.
				//
				inline static class {
				public:
					operator ConsoleColor() const {
						if (!GetConsoleScreenBufferInfo(_Handle, &_BufferInfo))
							Error("Gagal mendapatkan informasi buffer.");

						return {
							_Color.find(_BufferInfo.wAttributes / 16)->second,
							_Color.find(_BufferInfo.wAttributes % 16)->second
						};
					}

					void operator=(const ConsoleColor color) {
						if (!SetConsoleTextAttribute(_Handle, static_cast<unsigned short>(color.Background) << 4 | static_cast<unsigned short>(color.Foreground)))
							Error("Gagal mengatur attribut buffer.");
					}
				} Color;

				//
				// Untuk mengatur ukuran buffer pada console.
				//
				inline static class {
				public:
					operator COORD() const {
						if (!GetConsoleScreenBufferInfo(_Handle, &_BufferInfo))
							Error("Gagal mendapatkan informasi buffer.");

						return _BufferInfo.dwSize;
					}

					void operator=(const COORD size) {
						if (!SetConsoleScreenBufferSize(_Handle, size))
							Error("Gagal mengatur ukuran buffer.");
					}
				} Size;
			};

			struct Cursor {
				//
				// Untuk mengatur posisi cursor pada console.
				//
				inline static class {
				public:
					operator COORD() const {
						if (!GetConsoleScreenBufferInfo(_Handle, &_BufferInfo))
							Error("Gagal mendapatkan informasi buffer.");

						return _BufferInfo.dwCursorPosition;
					}

					void operator=(const COORD position) {
						if (!SetConsoleCursorPosition(_Handle, position))
							Error("Gagal mengatur posisi cursor.");
					}
				} Position;

				//
				// Untuk mengatur eksistensi cursor pada console.
				//
				inline static class {
				public:
					operator bool() const {
						if (!GetConsoleCursorInfo(_Handle, &_CursorInfo))
							Error("Gagal mendapatkan informasi buffer.");

						return _CursorInfo.bVisible;
					}

					void operator=(const bool visible) {
						if (!GetConsoleCursorInfo(_Handle, &_CursorInfo))
							Error("Gagal mendapatkan informasi buffer.");
						_CursorInfo.bVisible = visible;
						if (!SetConsoleCursorInfo(_Handle, &_CursorInfo))
							Error("Gagal mengatur informasi buffer.");
					}
				} Visible;
			};

			inline static class {
			public:
				operator COORD() const {
				}
				void operator=(const COORD size) {
					COORD max = GetLargestConsoleWindowSize(_Handle);
					if (size.X > max.X)
						Error("Ukuran lebar terlalu besar.");
					else if (size.Y > max.Y)
						Error("Ukuran tinggi terlalu besar");

					if(!GetConsoleScreenBufferInfo(_Handle, &_BufferInfo))
						Error("Gagal mendapatkan informasi buffer.");

					SMALL_RECT wInfo = _BufferInfo.srWindow;
					COORD wSize{
						wInfo.Right - wInfo.Left + 1,
						wInfo.Bottom - wInfo.Top + 1
					};

					if (wSize.X > size.X || wSize.Y > size.Y) {
						SMALL_RECT window{
							0,
							0,
							size.X < wSize.X ? size.X - 1 : wSize.X - 1,
							size.Y < wSize.Y ? size.Y - 1 : wSize.Y - 1
						};

						if (!SetConsoleWindowInfo(_Handle, true, &window))
							Error("Gagal mengatur ukuran console.");
					}

					Buffer::Size = size;

					SMALL_RECT window{
						0,
						0,
						size.X - 1,
						size.Y - 1
					};

					if (!SetConsoleWindowInfo(_Handle, true, &window))
						Error("Gagal mengatur ukuran console.");
				}
			} Size;

		public:
			//
			// Membersihkan buffer
			//
			static void Clear() {
				system("cls");
			}

			//
			// Menonaktifkan tombol close pada window frame
			//
			static void DisableCloseButton() {
				if (!EnableMenuItem(GetSystemMenu(GetConsoleWindow(), false), SC_CLOSE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED))
					Error("Gagal menonaktifkan tombol close.");
			}

			//
			// Menonaktifkan tombol maximize pada window frame
			//
			static void DisableMaximizeButton() {
				SetWindowLong(_Hwnd, GWL_STYLE, GetWindowLong(_Hwnd, GWL_STYLE) & ~WS_MAXIMIZEBOX);
			}

			//
			// Menonaktifkan tombol minimize pada window frame
			//
			static void DisableMinimizeButton() {
				SetWindowLong(_Hwnd, GWL_STYLE, GetWindowLong(_Hwnd, GWL_STYLE) & ~WS_MINIMIZEBOX);
			}

			//
			// Menonaktifkan perubahan ukuran pada window frame
			//
			static void DisableResize() {
				SetWindowLong(_Hwnd, GWL_STYLE, GetWindowLong(_Hwnd, GWL_STYLE) & ~WS_SIZEBOX);
			}

			//
			// Membaca inputan karakter dari keyboard dengan menekan enter.
			// 
			// @return char: Karakter yang diinputkan.
			//
			static char Read() {
				return std::cin.get();
			}

			//
			// Membaca inputan karakter dari keyboard tanpa menekan enter.
			// 
			// @return char: Karakter yang diinputkan.
			//
			static char ReadKey() {
				return _getch();
			}

			//
			// Membaca inputan kalimat dari keyboard.
			// 
			// @return String: Kalimat yang diinputkan.
			//
			String ReadLine() {
				String line;

				std::getline(std::cin, line);

				return line;
			}

			//
			// Mengatur gaya dan ukuran huruf pada console
			//
			static void SetFont(CWString faceName, Int16 size = 18) {
				CONSOLE_FONT_INFOEX font{
					sizeof CONSOLE_FONT_INFOEX,
					0,
					{ 0, size },
					FF_DONTCARE,
					FW_NORMAL
				};

				wcscpy_s(font.FaceName, faceName);
				if (!SetCurrentConsoleFontEx(_Handle, false, &font))
					Error("Gagal mengatur gaya huruf.");
			}

			//
			// Mengatur posisi console ke tengah layar
			//
			static void SetPositionToCenter() {
				RECT rScreen;
				HWND hScreen = GetDesktopWindow();
				if (!GetWindowRect(hScreen, &rScreen))
					Error("Gagal mendapatkan ukuran layar.");

				RECT rWindow;
				HWND hWindow = GetForegroundWindow();
				if (!GetWindowRect(hWindow, &rWindow))
					Error("Gagal mendapatkan ukuran window.");

				Int32 width = rWindow.right - rWindow.left;
				Int32 height = rWindow.bottom - rWindow.top;
				Int32 x = ((rScreen.right - rScreen.left) / 2 - width / 2);
				Int32 y = ((rScreen.bottom - rScreen.top) / 2 - height / 2);

				if (!SetWindowPos(hWindow, 0, x, y, width, height, SWP_NOSIZE | SWP_NOZORDER))
					Error("Gagal mengatur posisi console.");
			}

			//
			// Menulis kedalam console.
			// 
			// @param value: Nilai yang akan ditulis.
			//
			template<typename... T>
			static void Write(T... value) {
				((std::cout << value), ...);
			}

			//
			// Menulis kedalam console diakhiri dengan baris baru.
			// 
			// @param value: Nilai yang akan ditulis.
			//
			template<typename... T>
			static void WriteLine(T... value) {
				((std::cout << value), ...) << NewLine;
			}
		};

		//
		// Struktur standar untuk menampung waktu
		//
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

			friend std::ostream& operator<<(std::ostream& out, DateTime value) {
				out << value.DayOfTheMonth << "/" << value.Month << "/" << value.Year << " ";
				value.Hour < 10 ? out << 0 << value.Hour << ":" : out << value.Hour << ":";
				value.Minute < 10 ? out << 0 << value.Minute << ":" : out << value.Minute << ":";
				value.Second < 10 ? out << 0 << value.Second : out << value.Second;

				return out;
			}
		};

		//
		// Terdapat method untuk mengambil waktu dari system
		//
		class Time final {
		private:
			inline static time_t _Now = time(0);
			inline static tm _Time = {};
			inline static String _Day[] = {
				"Sunday", "Monday", "Tuesday", "Wednesday",
				"Thursday", "Friday", "Saturday"
			};
			inline static String _Month[] = {
				"January", "February", "March", "April",
				"May", "June", "July", "August",
				"September", "October", "November", "December"
			};

		public:
			//
			// Mengambil waktu saat ini
			// 
			// @return DateTime: Waktu saat ini
			//
			inline static class {
			public:
				operator DateTime() const {
					localtime_s(&_Time, &_Now);

					return {
						_Time.tm_sec,
						_Time.tm_min,
						_Time.tm_hour,
						0,
						0,
						0,
						0,
						0,
						_Time.tm_isdst,
						"",
						"",
						"",
						""
					};
				}
			} Current;

			//
			// Mengambil waktu dan tanggal saat ini
			// 
			// @return DateTime: Waktu dan tanggal saat ini
			//
			inline static class {
			public:
				operator DateTime() const {
					localtime_s(&_Time, &_Now);

					return {
						_Time.tm_sec,
						_Time.tm_min,
						_Time.tm_hour,
						_Time.tm_wday,
						_Time.tm_mday,
						_Time.tm_yday,
						_Time.tm_mon + 1,
						_Time.tm_year + 1900,
						_Time.tm_isdst,
						_Day[_Time.tm_wday].substr(0, 3),
						_Day[_Time.tm_wday],
						_Month[_Time.tm_mon].substr(0, 3),
						_Month[_Time.tm_mon]
					};
				}
			} Now;

			//
			// Mengambil tanggal saat ini
			// 
			// @return DateTime: Tanggal saat ini
			//
			inline static class {
			public:
				operator DateTime() const {
					localtime_s(&_Time, &_Now);

					return {
						0,
						0,
						0,
						_Time.tm_wday,
						_Time.tm_mday,
						_Time.tm_yday,
						_Time.tm_mon + 1,
						_Time.tm_year + 1900,
						0,
						_Day[_Time.tm_wday].substr(0, 3),
						_Day[_Time.tm_wday],
						_Month[_Time.tm_mon].substr(0, 3),
						_Month[_Time.tm_mon]
					};
				}
			} Today;
		};
	}

	namespace Utility {
		//
		// Struktur untuk menampung dua buah tipe data yang berbeda
		//
		template<typename T, typename T1>
		struct Result {
			T First;
			T1 Second;
		};

		class Cipher final {
		public:
			//
			// Metode enkripsi/dekripsi XOR
			// 
			// @param value: Nilai yang akan dienkripsi/didekripsi
			// @param key: Kunci yang digunakan untuk mengenkripsi/mendekripsi
			// 
			// @return String: Nilai yang telah terenkripsi/terdekripsi
			//
			static String XOR(String value, char key) {
				String _;

				for (char index : value)
					_ += index ^ key;

				return _;
			}

			//
			// Metode enkripsi/dekripsi Caesar
			// 
			// @param value: Nilai yang akan dienkripsi/didekripsi
			// @param key: Kunci yang digunakan untuk mengenkripsi/mendekripsi
			// 
			// @return String: Nilai yang telah terenkripsi/terdekripsi
			//
			static String Caesar(String value, int key) {
				String _;

				for (char index : value)
					if (isupper(index))
						_ += char(int(index + key - 65) % 26 + 65);
					else
						_ += char(int(index + key - 97) % 26 + 97);

				return _;
			}

			//
			// Metode enkripsi/dekripsi Vigenere
			// 
			// @param value: Nilai yang akan dienkripsi/didekripsi
			// @param key: Kunci yang digunakan untuk mengenkripsi/mendekripsi
			// 
			// @return String: Nilai yang telah terenkripsi/terdekripsi
			//
			static String Vigenere(String value, String key) {
				String _;
				SizeType keySize = key.size();

				for (SizeType i = 0;; i++) {
					if (key.size() >= value.size())
						break;

					key += key[i];
				}

				for (SizeType i = 0; i < value.size(); i++) {
					char x = (value[i] + key[i]) % 26;

					_ += x += 'A';
				}

				return _;
			}
		};

		//
		// Didalamnya terdapat method untuk menkonversi tipe data.
		//
		class Convert final {
		public:
			//
			// Mengkonversi String ke Int32.
			// 
			// @param value: Nilai yang akan dikonversi.
			// 
			// @return Int32: String yang telah dikonversi menjadi Int32.
			//
			static Int32 ToInt32(const String& value) {
				return std::stoi(value);
			}

			//
			// Mengkonversi String ke long.
			// 
			// @param value: Nilai yang akan dikonversi.
			// 
			// @return long: String yang telah dikonversi menjadi long.
			//
			static long ToLong(const String& value) {
				return std::stol(value);
			}

			//
			// Mengkonversi String ke unsigned long.
			// 
			// @param value: Nilai yang akan dikonversi.
			// 
			// @return unsigned long: String yang telah dikonversi menjadi unsigned long.
			//
			static unsigned long ToULong(const String& value) {
				return std::stoul(value);
			}

			//
			// Mengkonversi String ke Int64.
			// 
			// @param value: Nilai yang akan dikonversi.
			// 
			// @return Int64: String yang telah dikonversi menjadi Int64.
			//
			static Int64 ToInt64(const String& value) {
				return std::stoll(value);
			}

			//
			// Mengkonversi String ke UInt64.
			// 
			// @param value: Nilai yang akan dikonversi.
			// 
			// @return UInt64: String yang telah dikonversi menjadi UInt64.
			//
			static UInt64 ToUInt64(const String& value) {
				return std::stoull(value);
			}

			//
			// Mengkonversi String ke float.
			// 
			// @param value: Nilai yang akan dikonversi.
			// 
			// @return float: String yang telah dikonversi menjadi float.
			//
			static float ToFloat(const String& value) {
				return std::stof(value);
			}

			//
			// Mengkonversi String ke double.
			// 
			// @param value: Nilai yang akan dikonversi.
			// 
			// @return double: String yang telah dikonversi menjadi double.
			//
			static double ToDouble(const String& value) {
				return std::stod(value);
			}

			//
			// Mengkonversi String ke long double.
			// 
			// @param value: Nilai yang akan dikonversi.
			// 
			// @return long double: String yang telah dikonversi menjadi long double.
			//
			static long double ToLDouble(const String& value) {
				return std::stold(value);
			}

			//
			// Mengkonversi string menjadi huruf kecil/lowercase
			//
			static String ToLower(String value) {
				std::transform(value.begin(), value.end(), value.begin(), ::tolower);

				return value;
			}

			//
			// Mengkonversi string menjadi huruf kapital/uppercase
			//
			static String ToUpper(String value) {
				std::transform(value.begin(), value.end(), value.begin(), ::toupper);

				return value;
			}

			//
			// Mengkonversi String ke CString.
			// 
			// @param value: Nilai yang akan dikonversi.
			// 
			// @return CString: String yang telah dikonversi menjadi CString.
			//
			CString ToCString(const String& value) {
				return value.c_str();
			}

			//
			// Mengkonversi Int32 ke String.
			// 
			// @param value: Nilai yang akan dikonversi.
			// 
			// @return String: Int32 yang telah dikonversi menjadi String.
			//
			static String ToString(const Int32 value) {
				return std::to_string(value);
			}

			//
			// Mengkonversi UInt32 ke String.
			// 
			// @param value: Nilai yang akan dikonversi.
			// 
			// @return String: UInt32 yang telah dikonversi menjadi String.
			//
			static String ToString(const UInt32 value) {
				return std::to_string(value);
			}

			//
			// Mengkonversi long ke String.
			// 
			// @param value: Nilai yang akan dikonversi.
			// 
			// @return String: long yang telah dikonversi menjadi String.
			//
			static String ToString(const long value) {
				return std::to_string(value);
			}

			//
			// Mengkonversi unsigned long ke String.
			// 
			// @param value: Nilai yang akan dikonversi.
			// 
			// @return String: unsigned long yang telah dikonversi menjadi String.
			//
			static String ToString(const unsigned long value) {
				return std::to_string(value);
			}

			//
			// Mengkonversi Int64 ke String.
			// 
			// @param value: Nilai yang akan dikonversi.
			// 
			// @return String: Int64 yang telah dikonversi menjadi String.
			//
			static String ToString(const Int64 value) {
				return std::to_string(value);
			}

			//
			// Mengkonversi UInt64 ke String.
			// 
			// @param value: Nilai yang akan dikonversi.
			// 
			// @return String: UInt64 yang telah dikonversi menjadi String.
			//
			static String ToString(const UInt64 value) {
				return std::to_string(value);
			}

			//
			// Mengkonversi float ke String.
			// 
			// @param value: Nilai yang akan dikonversi.
			// 
			// @return String: float yang telah dikonversi menjadi String.
			//
			static String ToString(const float value) {
				return std::to_string(value);
			}

			//
			// Mengkonversi double ke String.
			// 
			// @param value: Nilai yang akan dikonversi.
			// 
			// @return String: double yang telah dikonversi menjadi String.
			//
			static String ToString(const double value) {
				return std::to_string(value);
			}

			//
			// Mengkonversi long double ke String.
			// 
			// @param value: Nilai yang akan dikonversi.
			// 
			// @return String: long double yang telah dikonversi menjadi String.
			//
			static String ToString(const long double value) {
				return std::to_string(value);
			}
		};

		class Tools final : System::Console {
		private:
			using Color = System::Color;
			using ConsoleColor = System::ConsoleColor;
			using Console = System::Console;
			using Exception = System::Exception;

			inline static String _Message[] = { "[INFORMATION]", "[WARNING]", "[DANGER]" };

		public:
			//
			// Tipe pesan
			//
			enum class Message {
				Information = 0,
				Warning = 1,
				Danger = 2
			};

			//
			// Membersihkan string serta menghapusnya dari console buffer.
			// 
			// @param position: Koordinat string yang akan dihapus.
			// @param value: String yang akan dihapus.
			//
			static void Clear(COORD position, String& value) {
				if (!value.empty()) {
					Delete(position, value.length());
					value.clear();
				}
			}

			//
			// Membersihkan string serta menghapusnya dari console buffer.
			// 
			// @param x: Koordinat X.
			// @param y: Koordinat Y.
			// @param value: String yang akan dihapus.
			//
			static void Clear(Int16 x, Int16 y, String& value) {
				if (!value.empty()) {
					Delete(x, y, value.length());
					value.clear();
				}
			}

			//
			// Menghapus nilai yang telah tercetak kedalam console.
			// 
			// @param position: Koordinat nilai yang akan dihapus.
			// @param length: Panjang nilai yang akan dihapus.
			//
			static void Delete(COORD position, SizeType length) {
				DWORD ch;

				if (!GetConsoleScreenBufferInfo(Console::_Handle, &Console::_BufferInfo))
					Error("Gagal mendapatkan informasi buffer.");
				if (!FillConsoleOutputCharacter(Console::_Handle, ' ', (DWORD)length, position, &ch))
					Error("Gagal menghapus teks.");
				if (!GetConsoleScreenBufferInfo(Console::_Handle, &Console::_BufferInfo))
					Error("Gagal mendapatkan informasi buffer.");
				if (!FillConsoleOutputAttribute(Console::_Handle, Console::_BufferInfo.wAttributes, (DWORD)length, position, &ch))
					Error("Gagal mengisi attribute.");
			}

			//
			// Menghapus nilai yang telah tercetak kedalam console.
			// 
			// @param x: Koordinat X.
			// @param y: Koordinat Y.
			// @param length: Panjang nilai yang akan dihapus.
			//
			static void Delete(Int16 x, Int16 y, SizeType length) {
				DWORD ch;

				if (!GetConsoleScreenBufferInfo(Console::_Handle, &Console::_BufferInfo))
					Error("Gagal mendapatkan informasi buffer.");
				if (!FillConsoleOutputCharacter(Console::_Handle, ' ', (DWORD)length, { x, y }, &ch))
					Error("Gagal menghapus teks.");
				if (!GetConsoleScreenBufferInfo(Console::_Handle, &Console::_BufferInfo))
					Error("Gagal mendapatkan informasi buffer.");
				if (!FillConsoleOutputAttribute(Console::_Handle, Console::_BufferInfo.wAttributes, (DWORD)length, { x, y }, &ch))
					Error("Gagal mengisi attribute.");
			}

			//
			// Membuat kata kunci acak.
			// 
			// @param numberOfKey: Banyaknya karakter.
			// 
			// @return String: String yang berisi karakter acak.
			//
			static String GenerateRandomKey(UInt16 numberOfKey) {
				const char keyList[] = "QWERTYUIOPASDFGHJKLZXCVBNM1234567890";
				String key;

				srand(static_cast<UInt32>(time(0)));
				for (UInt16 i = 0; i < numberOfKey; i++)
					key += keyList[rand() % 36];

				return key;
			}

			//
			// Mengecek String apakah berupa angka atau bukan.
			// 
			// @param value: String yang akan dicek.
			// 
			// @return bool: [true] Jika string berupa angka, sebaliknya [false].
			//
			static bool IsDigit(String& value) {
				return !value.empty() && std::find_if(value.begin(), value.end(), [](char ch) {return !isdigit(ch); }) == value.end();
			}

			//
			// Mengembalikan nilai terbesar.
			// 
			// @param first: Nilai pertama.
			// @param second: Nilai kedua.
			// 
			// @return T: Nilai terbesar.
			//
			template<typename T>
			static T Max(T first, T second) {
				return first > second ? first : second;
			}

			//
			// Mengembalikan nilai terkecil.
			// 
			// @param first: Nilai pertama.
			// @param second: Nilai kedua.
			// 
			// @return T: Nilai terkecil.
			//
			template<typename T>
			static T Min(T first, T second) {
				return first < second ? first : second;
			}

			//
			// Membaca inputan angka dari keyboard.
			// 
			// @return Int64: Angka yang diinputkan.
			//
			static Int64 ReadDigit() {
				char key;
				String line;

				while ((key = Console::ReadKey()) != '\r') {
					switch (key) {
					case '\b':
						if (!line.empty()) {
							line.pop_back();
							Console::Write("\b \b");
						}
						break;
					default:
						if (isdigit(key)) {
							line += key;
							Console::Write(key);
						}
					}
				}

				return Convert::ToInt64(line);
			}

			//
			// Membaca inputan keyboard dengan membatasi jumlah karakter yang diinputkan.
			// 
			// @param limit: Batas karakter yang diinputkan.
			// 
			// @return String: Karakter yang diinputkan.
			//
			static String ReadLine(SizeType limit) {
				char key;
				String line;

				while ((key = Console::ReadKey()) != '\r') {
					switch (key) {
					case '\b':
						if (!line.empty()) {
							line.pop_back();
							Console::Write("\b \b");
						}
						break;
					default:
						if (line.length() < limit) {
							line += key;
							Console::Write(key);
						}
					}
				}

				return line;
			}

			//
			// Membaca inputan kalimat dengan mengganti setiap karakter yang diinputkan menjadi asterisk (*).
			// 
			// @return String: Kalimat yang diinputkan.
			//
			static String ReadPassword() {
				char key;
				String line;

				while ((key = Console::ReadKey()) != '\r') {
					switch (key) {
					case '\b':
						if (!line.empty()) {
							line.pop_back();
							Console::Write("\b \b");
						}
						break;
					default:
						line += key;
						Console::Write("*");
					}
				}

				return line;
			}

			//
			// Membaca inputan kalimat dengan mengganti setiap karakter menjadi asterisk (*)
			// dan membatasi karakter yang diinputkan.
			// 
			// @param limit: Batas karakter yang diinputkan.
			// 
			// @return String: Kalimat yang diinputkan.
			//
			static String ReadPassword(SizeType limit) {
				char key;
				String line;

				while ((key = Console::ReadKey()) != '\r') {
					switch (key) {
					case '\b':
						if (!line.empty()) {
							line.pop_back();
							Console::Write("\b \b");
						}
						break;
					default:
						if (line.length() < limit) {
							line += key;
							Console::Write("*");
						}
					}
				}

				return line;
			}

			//
			// Menulis nilai kedalam console dengan posisi yang spesifik.
			// 
			// @param position: Koordinat penulisan nilai.
			// @param value: Nilai yang akan ditulis.
			//
			template<typename... T>
			static void Write(COORD position, T... value) {
				Console::Cursor::Position = position;
				Console::Write(value...);
			}

			//
			// Menulis nilai kedalam console dengan posisi yang spesifik.
			// 
			// @param x: Koordinat X penulisan nilai.
			// @param y: Koordinat Y penulisan nilai.
			// @param value: Nilai yang akan ditulis.
			//
			template<typename... T>
			static void Write(Int16 x, Int16 y, T... value) {
				Console::Cursor::Position = { x, y };
				Console::Write(value...);
			}

			//
			// Menulis nilai kedalam console dengan warna buffer yang spesifik.
			// 
			// @param color: Warna buffer.
			// @param value: Nilai yang akan ditulis.
			//
			template<typename... T>
			static void Write(ConsoleColor color, T... value) {
				ConsoleColor defaultColor = Console::Buffer::Color;

				Console::Buffer::Color = color;
				Console::Write(value...);
				Console::Buffer::Color = defaultColor;
			}

			//
			// Menulis milai kedalam console dengan warna buffer yang spesifik.
			// 
			// @param background: Warna latar belakang buffer.
			// @param foreground: Warna latar depan buffer.
			// @param value: Nilai yang akan ditulis.
			//
			template<typename... T>
			static void Write(Color background, Color foreground, T... value) {
				ConsoleColor defaultColor = Console::Buffer::Color;

				Console::Buffer::Color = { background, foreground };
				Console::Write(value...);
				Console::Buffer::Color = defaultColor;
			}

			//
			// Menulis kedalam console dengan posisi dan warna buffer yang spesifik.
			// 
			// @param position: Koordinat penulisan nilai.
			// @param color: Warna buffer.
			// @param value: Nilai yang akan ditulis.
			//
			template<typename... T>
			static void Write(COORD position, ConsoleColor color, T... value) {
				ConsoleColor defaultColor = Console::Buffer::Color;

				Console::Cursor::Position = position;
				Console::Buffer::Color = color;
				Console::Write(value...);
				Console::Buffer::Color = defaultColor;
			}

			//
			// Menulis kedalam console dengan posisi dan warna buffer yang spesifik.
			// 
			// @param position: Koordinat penulisan nilai.
			// @param background: Warna latar belakang buffer.
			// @param foreground: Warna latar depan buffer.
			// @param value: Nilai yang akan ditulis.
			//
			template<typename... T>
			static void Write(COORD position, Color background, Color foreground, T... value) {
				ConsoleColor defaultColor = Console::Buffer::Color;

				Console::Cursor::Position = position;
				Console::Buffer::Color = { background, foreground };
				Console::Write(value...);
				Console::Buffer::Color = defaultColor;
			}

			//
			// Menulis kedalam console dengan posisi dan warna buffer yang spesifik.
			// 
			// @param x: Koordinat X penulisan nilai.
			// @param y: Koordinat Y penulisan nilai.
			// @param color: Warna buffer.
			// @param value: Nilai yang akan ditulis.
			//
			template<typename... T>
			static void Write(Int16 x, Int16 y, ConsoleColor color, T... value) {
				ConsoleColor defaultColor = Console::Buffer::Color;

				Console::Cursor::Position = { x, y };
				Console::Buffer::Color = color;
				Console::Write(value...);
				Console::Buffer::Color = defaultColor;
			}

			//
			// Menulis kedalam console dengan posisi dan warna buffer yang spesifik.
			// 
			// @param x: Koordinat X penulisan nilai.
			// @param y: Koordinat Y penulisan nilai.
			// @param background: Warna latar belakang buffer.
			// @param foreground: Warna latar depan buffer.
			// @param value: Nilai yang akan ditulis.
			//
			template<typename... T>
			static void Write(Int16 x, Int16 y, Color background, Color foreground, T... value) {
				ConsoleColor defaultColor = Console::Buffer::Color;

				Console::Cursor::Position = { x, y };
				Console::Buffer::Color = { background, foreground };
				Console::Write(value...);
				Console::Buffer::Color = defaultColor;
			}

			//
			// Menulis pesan kedalam console dengan kode pesan yang spesifik.
			// 
			// @param type: Tipe pesan yang akan disampaikan.
			// @param message: Pesan yang disampaikan.
			//
			static void WriteMessage(Message type, String message) {
				switch (type) {
				case Message::Information:
					Write(Color::Cyan, Color::Black, _Message[static_cast<int>(type)]);
					break;
				case Message::Warning:
					Write(Color::DarkYellow, Color::Black, _Message[static_cast<int>(type)]);
					break;
				case Message::Danger:
					Write(Color::DarkRed, Color::Black, _Message[static_cast<int>(type)]);
					break;
				}

				Console::Write(" ", message);
			}

			//
			// Menulis pesan kedalam console dengan posisi dan kode pesan yang spesifik.
			//
			// @param position: Koordinat penulisan pesan.
			// @param type: Tipe pesan yang akan disampaikan.
			// @param message: Pesan yang disampaikan.
			//
			static void WriteMessage(COORD position, Message type, String message) {
				switch (type) {
				case Message::Information:
					Write(position, Color::Cyan, Color::Black, _Message[static_cast<int>(type)]);
					break;
				case Message::Warning:
					Write(position, Color::DarkYellow, Color::Black, _Message[static_cast<int>(type)]);
					break;
				case Message::Danger:
					Write(position, Color::DarkRed, Color::Black, _Message[static_cast<int>(type)]);
					break;
				}

				Console::Write(" ", message);
				Console::ReadKey();
				Delete(position, _Message[static_cast<int>(type)].length() + message.length() + 1);
			}

			//
			// Menulis pesan kedalam console dengan posisi dan kode pesan yang spesifik.
			// 
			// @param x: Koordinat X penulisan pesan.
			// @param y: Koordinat Y penulisan pesan.
			// @param type: Tipe pesan yang akan disampaikan.
			// @param message: Pesan yang disampaikan.
			//
			static void WriteMessage(Int16 x, Int16 y, Message type, String message) {
				switch (type) {
				case Message::Information:
					Write(x, y, Color::Cyan, Color::Black, _Message[static_cast<int>(type)]);
					break;
				case Message::Warning:
					Write(x, y, Color::DarkYellow, Color::Black, _Message[static_cast<int>(type)]);
					break;
				case Message::Danger:
					Write(x, y, Color::DarkRed, Color::Black, _Message[static_cast<int>(type)]);
					break;
				}
				Console::Write(" ", message);
				Console::ReadKey();
				Delete(x, y, _Message[static_cast<int>(type)].length() + message.length() + 1);
			}

			//
			// Menulis nilai kedalam console dengan posisi yang spesifik, diakhiri dengan baris baru.
			// 
			// @param position: Koordinat penulisan nilai.
			// @param value: Nilai yang akan ditulis.
			//
			template<typename... T>
			static void WriteLine(COORD position, T... value) {
				Console::Cursor::Position = position;
				Console::WriteLine(value...);
			}

			//
			// Menulis nilai kedalam console dengan posisi yang spesifik, diakhiri dengan baris baru.
			// 
			// @param x: Koordinat X penulisan nilai.
			// @param y: Koordinat Y penulisan nilai.
			// @param value: Nilai yang akan ditulis.
			//
			template<typename... T>
			static void WriteLine(Int16 x, Int16 y, T... value) {
				Console::Cursor::Position = { x, y };
				Console::WriteLine(value...);
			}

			//
			// Menulis nilai kedalam console dengan warna buffer yang spesifik, diakhiri dengan baris baru.
			// 
			// @param color: Warna buffer.
			// @param value: Nilai yang akan ditulis.
			//
			template<typename... T>
			static void WriteLine(ConsoleColor color, T... value) {
				ConsoleColor defaultColor = Console::Buffer::Color;

				Console::Buffer::Color = color;
				Console::WriteLine(value...);
				Console::Buffer::Color = defaultColor;
			}

			//
			// Menulis milai kedalam console dengan warna buffer yang spesifik, diakhiri dengan baris baru.
			// 
			// @param background: Warna latar belakang buffer.
			// @param foreground: Warna latar depan buffer.
			// @param value: Nilai yang akan ditulis.
			//
			template<typename... T>
			static void WriteLine(Color background, Color foreground, T... value) {
				ConsoleColor defaultColor = Console::Buffer::Color;

				Console::Buffer::Color = { background, foreground };
				Console::WriteLine(value...);
				Console::Buffer::Color = defaultColor;
			}

			//
			// Menulis kedalam console dengan posisi dan warna buffer yang spesifik, diakhiri dengan baris baru.
			// 
			// @param position: Koordinat penulisan nilai.
			// @param color: Warna buffer.
			// @param value: Nilai yang akan ditulis.
			//
			template<typename... T>
			static void WriteLine(COORD position, ConsoleColor color, T... value) {
				ConsoleColor defaultColor = Console::Buffer::Color;

				Console::Cursor::Position = position;
				Console::Buffer::Color = color;
				Console::WriteLine(value...);
				Console::Buffer::Color = defaultColor;
			}

			//
			// Menulis kedalam console dengan posisi dan warna buffer yang spesifik, diakhiri dengan baris baru.
			// 
			// @param position: Koordinat penulisan nilai.
			// @param background: Warna latar belakang buffer.
			// @param foreground: Warna latar depan buffer.
			// @param value: Nilai yang akan ditulis.
			//
			template<typename... T>
			static void WriteLine(COORD position, Color background, Color foreground, T... value) {
				ConsoleColor defaultColor = Console::Buffer::Color;

				Console::Cursor::Position = position;
				Console::Buffer::Color = { background, foreground };
				Console::WriteLine(value...);
				Console::Buffer::Color = defaultColor;
			}

			//
			// Menulis kedalam console dengan posisi dan warna buffer yang spesifik, diakhiri dengan baris baru.
			// 
			// @param x: Koordinat X penulisan nilai.
			// @param y: Koordinat Y penulisan nilai.
			// @param color: Warna buffer.
			// @param value: Nilai yang akan ditulis.
			//
			template<typename... T>
			static void WriteLine(Int16 x, Int16 y, ConsoleColor color, T... value) {
				ConsoleColor defaultColor = Console::Buffer::Color;

				Console::Cursor::Position = { x, y };
				Console::Buffer::Color = color;
				Console::WriteLine(value...);
				Console::Buffer::Color = defaultColor;
			}

			//
			// Menulis kedalam console dengan posisi dan warna buffer yang spesifik, diakhiri dengan baris baru.
			// 
			// @param x: Koordinat X penulisan nilai.
			// @param y: Koordinat Y penulisan nilai.
			// @param background: Warna latar belakang buffer.
			// @param foreground: Warna latar depan buffer.
			// @param value: Nilai yang akan ditulis.
			//
			template<typename... T>
			static void WriteLine(Int16 x, Int16 y, Color background, Color foreground, T... value) {
				ConsoleColor defaultColor = Console::Buffer::Color;

				Console::Cursor::Position = { x, y };
				Console::Buffer::Color = { background, foreground };
				Console::WriteLine(value...);
				Console::Buffer::Color = defaultColor;
			}

			//
			// Menulis pesan kedalam console dengan kode pesan yang spesifik, diakhiri dengan baris baru.
			// 
			// @param type: Tipe pesan yang akan disampaikan.
			// @param message: Pesan yang disampaikan.
			//
			static void WriteLineMessage(Message type, String message) {
				switch (type) {
				case Message::Information:
					Write(Color::Cyan, Color::Black, _Message[static_cast<int>(type)]);
					break;
				case Message::Warning:
					Write(Color::DarkYellow, Color::Black, _Message[static_cast<int>(type)]);
					break;
				case Message::Danger:
					Write(Color::DarkRed, Color::Black, _Message[static_cast<int>(type)]);
					break;
				}

				Console::WriteLine(" ", message);
			}

			//
			// Menulis pesan kedalam console dengan posisi dan kode pesan yang spesifik, diakhiri dengan baris baru.
			//
			// @param position: Koordinat penulisan pesan.
			// @param type: Tipe pesan yang akan disampaikan.
			// @param message: Nilai yang akan ditulis.
			//
			static void WriteLineMessage(COORD position, Message type, String message) {
				switch (type) {
				case Message::Information:
					Write(position, Color::Cyan, Color::Black, _Message[static_cast<int>(type)]);
					break;
				case Message::Warning:
					Write(position, Color::DarkYellow, Color::Black, _Message[static_cast<int>(type)]);
					break;
				case Message::Danger:
					Write(position, Color::DarkRed, Color::Black, _Message[static_cast<int>(type)]);
					break;
				}

				Console::WriteLine(" ", message);
				Console::ReadKey();
				Delete(position, _Message[static_cast<int>(type)].length() + message.length() + 1);
			}

			//
			// Menulis pesan kedalam console dengan posisi dan kode pesan yang spesifik, diakhiri dengan baris baru.
			// 
			// @param x: Koordinat X penulisan pesan.
			// @param y: Koordinat Y penulisan pesan.
			// @param type: Tipe pesan yang akan disampaikan.
			// @param message: Nilai yang akan ditulis.
			//
			static void WriteLineMessage(Int16 x, Int16 y, Message type, String message) {
				switch (type) {
				case Message::Information:
					Write(x, y, Color::Cyan, Color::Black, _Message[static_cast<int>(type)]);
					break;
				case Message::Warning:
					Write(x, y, Color::DarkYellow, Color::Black, _Message[static_cast<int>(type)]);
					break;
				case Message::Danger:
					Write(x, y, Color::DarkRed, Color::Black, _Message[static_cast<int>(type)]);
					break;
				}

				Console::WriteLine(" ", message);
				Console::ReadKey();
				Delete(x, y, _Message[static_cast<int>(type)].length() + message.length() + 1);
			}
		};

		//
		// Membuat menu dengan 'arrow-key' sebagai navigasi menu.
		// 
		// Penggunaan
		// ==========
		// Untuk membuat menu baru caranya sangat mudah, pertama inisialisasi ConsoleMenu dimana parameternya berisi
		// menu itu sendiri, warna cursor, koordinat menu saat ditampilkan, dan panjang cursor.
		// Lalu deklarasikan variable Selected, digunakan untuk menampung hasil dari pilihan menu.
		// Selanjutnya tampilkan menu dengan menggunakan method Print(), method ini juga akan mengembalikan
		// index berserta menu yang dipilih.
		// Apabila ingin menghapus menu dari console, gunakan method Clear().
		// 
		// Contoh
		// ======
		// -> Inisialisasi menu baru
		// ConsoleMenu mMenu  {
		//		{
		//			"Login"
		//			"Register"
		//		},
		//		{ Color::Green, Color::Black },
		//		{ 2, 6 },
		//		true
		// };
		// 
		// -> Variable selected untuk menampung hasil pilihan
		// ConsoleMenu::Selected rMenu;
		// 
		// -> Menampilkan menu
		// rMenu = mMenu.Print();
		// 
		class ConsoleMenu final {
		private:
			System::ConsoleColor _Color;
			COORD _Coordinate;
			Int16 _Limit = 0;

			//
			// Untuk menyimpan menu
			//
			struct {
				std::vector<String> Back; // Menu asli.
				std::vector<String> Front; // Menu yang telah dimodifikasi.
			} _Menu;

			//
			// Untuk menyimpan posisi index.
			//
			struct {
				SizeType Begin;
				SizeType Current;
				SizeType End;
			} _Index;

			//
			// Untuk menyimpan posisi cursor.
			//
			struct {
				Int16 Begin;
				Int16 Current;
				Int16 End;
			} _Cursor;

		public:
			//
			// Untuk menampung hasil pilihan menu.
			// First: Index-ke dari menu yang dipilih.
			// Second: Nama menu yang dipilih.
			//
			using Selected = Result<SizeType, String>;

			//
			// Menginisialisasi menu
			// 
			// @param menu: Menu yang akan dibuat.
			// @param cursorColor: Warna cursor.
			// @param position: koordinat menu saat ditampilkan.
			// @param fill: [true] cursor disamakan berdasarkan index menu terpanjang, sebaliknya [false]. Default [false].
			//
			ConsoleMenu(std::initializer_list<String> menu, System::ConsoleColor cursorColor, COORD position, bool fillSpace = false) : _Menu({ menu, menu }), _Color(cursorColor), _Coordinate(position) {
				if (fillSpace) {
					SizeType max = 0;

					for (String index : menu)
						max = Tools::Max(index.length(), max);
					for (String& index : _Menu.Front)
						index += (String(max - index.length(), ' '));
				}

				_Index = { 0, 0, menu.size() - 1 };
				_Cursor = { position.Y, position.Y, position.Y + static_cast<Int16>(menu.size()) - 1 };
			}

			//
			// Membersihkan menu yang telah tertulis kedalam console.
			//
			void Clear() {
				Int16 y = _Coordinate.Y;

				for (Int16 i = 0, index = static_cast<Int16>(_Index.Begin); i < _Limit; i++, y++, index++)
					Tools::Delete(_Coordinate.X, y, _Menu.Front[index].length());
			}

			//
			// Menampilkan menu.
			// 
			// @return Selected: Menu yang dipilih.
			//
			Selected Print() {
				char key;
				COORD buffer = System::Console::Buffer::Size;
				System::Console::Cursor::Visible = false;

				if ((_Coordinate.Y + static_cast<Int16>(_Menu.Back.size())) < buffer.Y) {
					Int16 y = _Coordinate.Y;

					_Limit = static_cast<Int16>(_Menu.Back.size());
					for (SizeType i = 0; i <= _Index.End; i++, y++) {
						Tools::Write(_Coordinate.X, y, _Menu.Front[i]);
					}

					do {
						Tools::Write(_Coordinate.X, _Cursor.Current, _Color, _Menu.Front[_Index.Current]);
						key = System::Console::ReadKey();
						Tools::Write(_Coordinate.X, _Cursor.Current, _Menu.Front[_Index.Current]);

						switch (key) {
						case 80:
							if (_Cursor.Current != _Cursor.End && _Index.Current < _Index.End)
								_Cursor.Current++, _Index.Current++;
							else
								_Cursor.Current = _Cursor.Begin, _Index.Current = _Index.Begin;
							break;
						case 72:
							if (_Cursor.Current != _Cursor.Begin && _Index.Current > _Index.Begin)
								_Cursor.Current--, _Index.Current--;
							else
								_Cursor.Current = _Cursor.End, _Index.Current = _Index.End;
							break;
						}
					} while (key != '\r');
				}
				else {
					_Limit = buffer.Y - _Coordinate.Y;
					_Cursor.End = _Coordinate.Y + _Limit - 1;

					do {
						Int16 y = _Coordinate.Y;
						SizeType index = _Index.Begin;

						for (SizeType i = 0; i < _Limit; i++, y++, index++)
							Tools::Write(_Coordinate.X, y, _Menu.Front[index]);

						Tools::Write(_Coordinate.X, _Cursor.Current, _Color, _Menu.Front[_Index.Current]);
						key = System::Console::ReadKey();
						Tools::Write(_Coordinate.X, _Cursor.Current, _Menu.Front[_Index.Current]);

						switch (key) {
						case 80:
							if (_Cursor.Current != _Cursor.End)
								_Cursor.Current++, _Index.Current++;
							else if (_Index.Current < _Index.End)
								_Index.Begin++, _Index.Current++;
							else
								_Cursor.Current = _Cursor.Begin, _Index.Current = _Index.Begin = 0;
							break;
						case 72:
							if (_Cursor.Current != _Cursor.Begin)
								_Cursor.Current--, _Index.Current--;
							else if (_Index.Current > 0)
								_Index.Begin--, _Index.Current--;
							else
								_Cursor.Current = _Cursor.End, _Index.Current = _Index.End, _Index.Begin = _Index.End - _Limit + 1;
							break;
						}
					} while (key != '\r');
				}
				System::Console::Cursor::Visible = true;

				return {
					_Index.Current,
					_Menu.Back[_Index.Current]
				};
			}
		};

		//
		// Membuat tabel yang terstuktur.
		// 
		// Penggunaan
		// ==========
		// Untuk membuat tabel baru, ConsoleTable harus diinisialisasi terlebih dahulu.
		// Selanjutnya, ukuran padding harus diatur. Ukuran padding mengatur jarak antara
		// teks di setiap cell dan batas cell (border). Nilai defaultnya adalah 1.
		// Untuk mengganti gaya border, gunakan method Border().
		// Untuk menambahkan baris baru kedalam tabel, gunakan operator += diikuti dengan daftar string.
		// Sebaliknya jika ingin menghapus row dari tabel gunakan operator -=.
		// Untuk memperbarui data didalam cell, gunakan method UpdateRow().
		// Hal yang sama juga berlaku untuk memperbarui header, cukup panggil UpdateHeader() yang diisi dengan
		// parameter yang diperlukan.
		// Untuk menulis tabel kedalam console anda bisa menggunakan method Print() atau juga dengan menggunakan output standar.
		// Jika ingin memfilter secara ascending atau sebaliknya, anda bisa menggunakan method Sort().
		// 
		// Contoh
		// ======
		// -> Inisialisasi tabel baru
		// ConsoleTable table{ "Country", "Capital", "Population", "Area", "Currency" };
		// 
		// -> Mengatur padding
		// table.Padding(2);
		// 
		// -> Mengatur gaya border
		// table.Border(Style::Line);
		// 
		// -> Menambahkan row baru
		// table += { "Germany", "Berlin", "82,800,000", "357,168 km2", "Euro" };
		// atau
		// table.Add({ "France", "Paris", "67,201,000", "640,679 km2 ", "Euro" });
		// 
		// -> Menghapus row dari tabel
		// table -= 2;
		// atau
		// table.Remove(2);
		// 
		// -> Untuk mengupdate row cukup
		// table.UpdateRow(5, 1, "Data baru");
		// 
		// -> Untuk mengupdate header
		// table.UpdateHeader(3, "Header baru");
		// 
		// -> Filter tabel
		// table.Sort(true)
		// 
		// -> Tampilkan tabel
		// table.Print();
		// atau
		// Console::Write(table);
		// atau
		// std::cout << table;
		// 
		// Original header: https://github.com/766F6964/ConsoleTable
		//
		class ConsoleTable final {
		private:
			using Header = std::vector<String>;
			using Row = std::vector<std::vector<String>>;
			using Width = std::vector<SizeType>;
			using Exception = System::Exception;

			Header _Header;
			Row _Row;
			Width _Width;
			UInt16 _Padding;

			struct RowType {
				String Left;
				String Intersect;
				String Right;
			};

			struct Border {
				String Horizontal;
				String Vertical;
				RowType Top;
				RowType Middle;
				RowType Bottom;
			};

			Border _Basic = { "-", "|", {"+", "+", "+"}, {"+", "+", "+"}, {"+", "+", "+"} };
			Border _Line = { "━", "┃", {"┏", "┳", "┓"}, {"┣", "╋", "┫"}, {"┗", "┻", "┛"} };
			Border _DoubleLine = { "═", "║", {"╔", "╦", "╗"}, {"╠", "╬", "╣"}, {"╚", "╩", "╝"} };
			Border _Invisible = { " ", " ", {" ", " ", " "}, {" ", " ", " "}, {" ", " ", " "} };
			Border _Border = _Basic;

			String _GetHeader(Header header) const {
				StringStream line;

				line << _Border.Vertical;
				for (SizeType i = 0; i < header.size(); i++) {
					String index = header[i];
					line << String(_Padding, ' ') + index + String((_Width[i] - index.length()), ' ') + String(_Padding, ' ');
					line << _Border.Vertical;
				}
				line << NewLine;

				return line.str();
			}

			String _GetLine(RowType position) const {
				StringStream line;

				line << position.Left;
				for (SizeType i = 0; i < _Width.size(); i++) {
					for (SizeType j = 0; j < (_Width[i] + _Padding + _Padding); j++)
						line << _Border.Horizontal;

					line << (i == _Width.size() - 1 ? position.Right : position.Intersect);
				}
				line << NewLine;

				return line.str();
			}

			String _GetRow(Row row) const {
				StringStream line;

				for (auto& index : row) {
					line << _Border.Vertical;

					for (size_t i = 0; i < index.size(); i++) {
						line << String(_Padding, ' ') + index[i] + String((_Width[i] - index[i].length()), ' ') + String(_Padding, ' ');
						line << _Border.Vertical;
					}
					line << NewLine;
				}

				return line.str();
			}

			SizeType _GetHeight() const {
				return _Row.size() + 5;
			}

			SizeType _GetWidth() const {
				SizeType size = 2;

				for (SizeType length : _Width)
					size += (length + _Padding + _Padding + 1);

				return size;
			}

			friend std::ostream& operator<<(std::ostream& out, const ConsoleTable& table) {
				return out
					<< table._GetLine(table._Border.Top)
					<< table._GetHeader(table._Header)
					<< table._GetLine(table._Border.Middle)
					<< table._GetRow(table._Row)
					<< table._GetLine(table._Border.Bottom);
			}

		public:
			//
			// Untuk mengatur gaya border
			//
			enum class Style {
				Basic = 0,
				Line = 1,
				DoubleLine = 2,
				Invisible = 3
			};

			//
			// Menginisialisasi tabel.
			// 
			// @param header: Header tabel.
			// @param padding: Jarak antara border ke cell. Default 1.
			//
			ConsoleTable(std::initializer_list<String> header, UInt16 padding = 1) :_Header(header), _Padding(padding) {
				for (const String& index : header)
					_Width.push_back(index.length());
			}

			ConsoleTable& operator+=(std::initializer_list<String> row) {
				Add(row);

				return *this;
			}

			ConsoleTable& operator-=(SizeType rowIndex) {
				Remove(rowIndex);

				return *this;
			}

			//
			// Menambahkan row kedalam tabel.
			// 
			// @param row: Row yang akan ditambahkan.
			//
			void Add(std::initializer_list<String> row) {
				if (row.size() > _Width.size())
					Error("Ukuran row harus sama dengan header.");

				auto res = std::vector<String>{ row };

				_Row.push_back(res);
				for (SizeType i = 0; i < res.size(); i++)
					_Width[i] = Tools::Max(res[i].size(), _Width[i]);
			}

			//
			// Mengatur gaya border.
			// 
			// @param style: Gaya border yang akan digunakan.
			//
			void Border(Style style) {
				switch (style) {
				case Style::Basic: _Border = _Basic; break;
				case Style::Line: _Border = _Line; break;
				case Style::DoubleLine: _Border = _DoubleLine; break;
				case Style::Invisible: _Border = _Invisible; break;
				}
			}

			//
			// Menulis tabel kedalam console dengan mengubah ukuran
			// buffer mengikuti ukuran tabel.
			//
			void Print() {
				COORD buffer = System::Console::Buffer::Size;
				COORD cursor = System::Console::Cursor::Position;
				SizeType width = _GetWidth();
				SizeType height = _GetHeight();

				System::Console::Buffer::Size = {
					static_cast<Int16>(width) > buffer.X ? static_cast<Int16>(width) : buffer.X,
					static_cast<Int16>(height) > buffer.Y ? static_cast<Int16>(height) : buffer.Y
				};
				System::Console::Cursor::Position = { 0, cursor.Y };
				System::Console::Write(*this);
				System::Console::ReadKey();
				System::Console::Clear();
				System::Console::Buffer::Size = buffer;
			}

			//
			// Menghapus row dari tabel berdasarkan index row.
			// 
			// @param index: Index row yang akan dihapus.
			//
			void Remove(size_t index) {
				if (index > _Row.size())
					Error("Index row diluar batas.");

				_Row.erase(_Row.begin() + index);
			}

			//
			// Memfilter row.
			// 
			// @param ascending: [true] tabel akan difilter secara ascending, sebaliknya [false]. Default [true].
			//
			void Sort(bool ascending = true) {
				if (ascending)
					std::sort(_Row.begin(), _Row.end(), std::less<std::vector<String>>());
				else
					std::sort(_Row.begin(), _Row.end(), std::greater<std::vector<String>>());
			}

			//
			// Mengupdate header berdasarkan index header.
			// 
			// @param index: Index header yang akan diupdate.
			// @param  newHeader: Header baru.
			//
			void UpdateHeader(SizeType index, String newHeader) {
				if (index > _Header.size())
					Error("Index header diluar batas.");

				_Header[index] = newHeader;
				_Width[index] = newHeader.size() > _Width[index] ? newHeader.size() : _Width[index];
			}

			//
			// Mengupdate row berdasarkan index row.
			// 
			// @param rowIndex: Index dari row yang akan diupdate.
			// @param headerIndex: Index header
			// @param newValue: Data baru
			//
			void UpdateRow(size_t rowIndex, size_t headerIndex, String newValue) {
				if (rowIndex > _Row.size())
					Error("Index row diluar batas.");
				else if (headerIndex > _Header.size())
					Error("Index header diluar batas.");

				_Row[rowIndex][headerIndex] = newValue;
				_Width[headerIndex] = newValue.size() > _Width[headerIndex] ? newValue.size() : _Width[headerIndex];
			}
		};
	}
}