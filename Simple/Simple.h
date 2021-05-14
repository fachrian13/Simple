//
// Simple.h
// 
// 
// Description
// ===========
// Header yang dapat membantu anda untuk menulis kode.
// Tujuan pembuatan header ini hanya sebagai pembelajaran. Masih terdapat bug pada header ini,
// jadi pengguna diharapkan berhati-hati dalam menggunakannya.
// Tidak semua kode yang terdapat disini buatan sendiri, namun ada beberapa yang didapat
// dari berbagai sumber. Jika ingin menambahkan, mengurangi, atau memodifikasi kode silakan,
// saya harap dengan kontribusi anda header ini menjadi lebih baik lagi.
// 
// 
// Reference
// =========
// https://docs.microsoft.com/en-us/
// https://en.cppreference.com/w/
// https://stackoverflow.com/
// https://www.cplusplus.com/
// https://www.geeksforgeeks.org/
// https://github.com/
// https://www.youtube.com/user/TheChernoProject
// https://www.quora.com
// 
// 
// Last Update
// ===========
// [1.0.0] May 14, 2021
// - Perbaikan kode
// - [On Progress] Pembuatan kelas CommaSeparatedValue
// 
// 
// Required C++17
// Written in Visual Studio 2019
// Fahri Synyster
//

#pragma once
#include <chrono>
#include <conio.h>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <map>
#include <thread>
#include <time.h>
#include <windows.h>

namespace Simple {
	namespace System {
		//
		// Warna dasar pada console. 
		// Untuk mengkonversi menjadi string, anda bisa menggunakan method 
		// ToString() pada class Utility::Convert.
		//
		enum class Color {
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
		// Struktur warna dasar pada console.
		//
		struct ConsoleColor {
			Color Background;
			Color Foreground;
		};

		//
		// Kelas untuk menangani error pada program.
		// (Tidak termasuk error yang dilemparkan dari std::exception)
		//
		class Exception {
		private:
			int _Line;
			const char* _File;
			const char* _Function;
			const char* _Description;

		public:
			Exception(const char* file, int line, const char* function, const char* description) : _File(file), _Line(line), _Function(function), _Description(description) {}

			//
			// Mengembalikan deskripsi error yang terjadi.
			// 
			// @return const char*: Deskripsi.
			//
			const char* Description() const {
				return _Description;
			}

			//
			// Mengembalikan nama file dimana error terjadi.
			// 
			// @return const char*: Nama file.
			//
			const char* File() const {
				return _File;
			}

			//
			// Mengembalikan nama fungsi dimana error terjadi.
			// 
			// @return const char*: Nama fungsi.
			//
			const char* Function() const {
				return _Function;
			}

			//
			// Mengembalikan baris ke- dimana error terjadi.
			// 
			// @return int: Baris.
			//
			int Line() const {
				return _Line;
			}
		};

		//
		// Untuk melempar Exception, dimana akan otomatis mengisi parameter 
		// throw Exception(__FILE__, __LINE__, __FUNCTION__, value).
		// 
		// @param value: Deskripsi/pesan error yang disampaikan.
		//
		#define Error(value) throw Exception(__FILE__, __LINE__, __FUNCTION__, value)

		//
		// Baca/tulis kedalam file dengan menggunakan mode binary.
		//
		template<typename T>
		class BinaryFile {
		protected:
			std::filesystem::path _FileName;
			std::filesystem::path _FileTemp = "temp.dat";

			std::fstream _File;
			std::fstream _Temp;

		public:
			//
			// Inisialisasi kelas BinaryFile.
			// 
			// @param fileName: Nama file yang akan dibuat.
			//
			BinaryFile(std::filesystem::path fileName) : _FileName(fileName.empty() ? Error("Nama file tidak boleh kosong.") : fileName) {
				if (!_FileName.has_extension())
					_FileName += ".bin";
				else if (_FileName.extension() != ".bin")
					_FileName.replace_extension(".bin");

				_File.open(_FileName, std::ios::out | std::ios::binary | std::ios::app);
				if (!_File)
					Error("Gagal membuka file.");
				_File.close();
			}

			//
			// Menghapus data dari file berdasarkan index data. Terdapat 2 metode penghapusan data. 
			// Pertama dengan menggunakan vector, Seluruh data dari dalam file akan ditampung kedalam vector, 
			// lalu index data akan dihapus, selanjutnya vector ditulis kembali kedalam file secara berurutan. 
			// Kedua dengan membuka dua buah file, file pertama sebagai source dan file kedua sebagai temp. 
			// Data akan dibaca dari file source kemudian langsung ditulis kedalam file temp, jika index data sama 
			// dengan posisi pembacaan file, maka index tersebut akan dilewat.
			// 
			// @param index: index ke- data yang akan dihapus.
			// @param method: Metode penghapusan data. [true] Metode pertama, [false] Metode kedua. Default: [true]
			//
			void Delete(size_t index, bool method = true) {
				if (method) {
					std::vector<T> stored = Read();

					stored.erase(stored.begin() + index);
					if (!_File)
						Error("Gagal membuka file.");
					_File.open(_FileName, std::ios::out | std::ios::binary | std::ios::trunc);
					for (T index : stored)
						_File.write((char*)&index, sizeof T);
					_File.close();
				}
				else {
					T temp;

					_File.open(_FileName, std::ios::in | std::ios::binary);
					if (!_File)
						Error("Gagal membuka file.");
					_Temp.open(_FileName, std::ios::out | std::ios::binary);
					if (!_Temp)
						Error("Gagal membuka file temp.");
					for (size_t i = 0; _File.read((char*)&temp, sizeof T); i++)
						if (i != index)
							_Temp.write((char*)&temp, sizeof T);
					_File.close();
					_Temp.close();

					remove(_FileName);
					rename(_FileTemp, _FileName);
				}
			}

			//
			// Mengecek apakah file dalam keadaan kosong atau terisi.
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
			// Membaca seluruh data dari dalam file.
			// 
			// @return std::vector<T>: Seluruh data yang telah dibaca.
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
			// Memperbarui data didalam file berdasarkan index data.
			// 
			// @param index: Index ke- data yang akan diupdate.
			// @param newData: Data baru yang akan ditulis.
			//
			void Update(size_t index, T newData) {
				_File.open(_FileName, std::ios::in | std::ios::out | std::ios::binary);
				if (!_File)
					Error("Gagal membuka file.");
				_File.seekp(index * sizeof T, std::ios::beg);
				_File.write((char*)&newData, sizeof T);
				_File.close();
			}

			//
			// Menulis data kedalam file.
			// 
			// @param value: Data yang akan ditulis.
			//
			void Write(T value) {
				_File.open(_FileName, std::ios::out | std::ios::binary | std::ios::app);
				if (!_File)
					Error("Gagal membuka file.");
				_File.write((char*)&value, sizeof T);
				_File.close();
			}
		};

		//
		// Berisi method yang bisa digunakan untuk operasi didalam console.
		//
		class Console {
		protected:
			inline static CONSOLE_SCREEN_BUFFER_INFO _BufferInfo;
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
				{ 15, Color::White }
			};
			inline static CONSOLE_CURSOR_INFO _CursorInfo;
			inline static HANDLE _Handle = GetStdHandle(STD_OUTPUT_HANDLE);
			inline static HWND _Hwnd = GetConsoleWindow();

		public:
			struct Buffer {
				inline static struct {
					operator ConsoleColor() const {
						if (!GetConsoleScreenBufferInfo(_Handle, &_BufferInfo))
							Error("Gagal mendapatkan informasi buffer.");
						return {
							_Color.find(_BufferInfo.wAttributes / 16)->second,
							_Color.find(_BufferInfo.wAttributes % 16)->second
						};
					}

					void operator=(const ConsoleColor color) {
						if (!SetConsoleTextAttribute(_Handle, (short)color.Background << 4 | (short)color.Foreground))
							Error("Gagal mengatur warna buffer.");
					}
				} Color;

				inline static struct {
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
				inline static struct {
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

				inline static struct {
					operator bool() const {
						if (!GetConsoleCursorInfo(_Handle, &_CursorInfo))
							Error("Gagal mendapatkan informasi cursor.");
						return _CursorInfo.bVisible;
					}

					void operator=(const bool visible) {
						if (!GetConsoleCursorInfo(_Handle, &_CursorInfo))
							Error("Gagal mendapatkan informasi cursor");
						_CursorInfo.bVisible = visible;
						if (!SetConsoleCursorInfo(_Handle, &_CursorInfo))
							Error("Gagal mengatur ekstensi cursor.");
					}
				} Visible;
			};

			inline static struct {
			public:
				void operator=(const COORD size) {
					COORD max = GetLargestConsoleWindowSize(_Handle);
					if (size.X > max.X)
						Error("Ukuran lebar diluar batas.");
					else if (size.Y > max.Y)
						Error("Ukuran tinggi diluar batas.");

					if (!GetConsoleScreenBufferInfo(_Handle, &_BufferInfo))
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
			// Membersihkan buffer.
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
			// Mengembalikan karakter yang diinput dengan menekan enter.
			// 
			// @return char: Karakter yang diinputkan.
			//
			static char Read() {
				return std::cin.get();
			}

			//
			// Mengembalikan karakter yang diinput tanpa menekan enter.
			// 
			// @return char: Karakter yang diinputkan.
			//
			static char ReadKey() {
				return _getch();
			}

			//
			// Mengembalikan kalimat yang diinputkan dengan menekan enter.
			// 
			// @return std::string: Kalimat yang diinputkan.
			//
			std::string ReadLine() {
				std::string line;

				std::getline(std::cin, line);
				return line;
			}

			//
			// Mengatur gaya dan ukuran huruf pada console
			// 
			// @param faceName: Nama font yang akan digunakan.
			// @param size: Ukuran font. Default: 18.
			//
			static void SetFont(const wchar_t* faceName, short size = 18) {
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

				short width = rWindow.right - rWindow.left;
				short height = rWindow.bottom - rWindow.top;
				short x = rScreen.right / 2 - width / 2;
				short y = rScreen.bottom / 2 - height / 2;
				if (!SetWindowPos(_Hwnd, 0, x, y, width, height, SWP_NOZORDER | SWP_NOSIZE))
					Error("Gagal mengatur posisi console.");
			}

			//
			// Menghentikan thread berdasarkan waktu yang diberikan.
			// 
			// @param milisecond: Waktu berhenti dalam milisecond.
			//
			static void Sleep(unsigned long long milisecond) {
				std::this_thread::sleep_for(std::chrono::milliseconds(milisecond));
			}

			//
			// Menulis nilai kedalam console.
			// 
			// @param value: Nilai yang akan ditulis.
			//
			template<typename... T>
			static void Write(T... value) {
				((std::cout << value), ...);
			}

			//
			// Menulis nilai kedalam console diakhiri dengan baris baru.
			// 
			// @param value: Nilai yang akan ditulis.
			//
			template<typename... T>
			static void WriteLine(T... value) {
				Write(value..., "\n");
			}
		};

		//
		// Standar stuktur untuk menampung waktu.
		//
		class DateTime final {
		private:
			inline static time_t _Now = time(0);
			inline static tm _Time;
			inline static std::string _Day[] = {
				"Sunday", "Monday", "Tuesday", "Wednesday",
				"Thursday", "Friday", "Saturday"
			};
			inline static std::string _Month[] = {
				"January", "February", "March", "April",
				"May", "June", "July", "August",
				"September", "October", "November", "December"
			};

			friend std::ostream& operator<<(std::ostream& out, DateTime& value) {
				out << value.DayOfTheMonth << "/" << value.Month << "/" << value.Year << " ";
				value.Hour < 10 ? out << 0 << value.Hour << ":" : out << value.Hour << ":";
				value.Minute < 10 ? out << 0 << value.Minute << ":" : out << value.Minute << ":";
				value.Second < 10 ? out << 0 << value.Second : out << value.Second;
				return out;
			}

		public:
			int Second;
			int Minute;
			int Hour;
			int DayOfTheWeek;
			int DayOfTheMonth;
			int DayOfTheYear;
			int Month;
			int Year;
			int Dst;
			std::string DayShort;
			std::string DayLong;
			std::string MonthShort;
			std::string MonthLong;

			//
			// Mengambil waktu saat ini.
			// 
			// @return DateTime: Waktu saat ini.
			//
			static DateTime Current() {
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

			//
			// Mengambil waktu dan tanggal saat ini.
			// 
			// @return DateTime: Waktu dan tanggal saat ini.
			//
			static DateTime Now() {
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

			//
			// Mengambil tanggal saat ini.
			// 
			// @return DateTime: Tanggal saat ini.
			//
			static DateTime Today() {
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
		};
	}

	namespace Utility {
		//
		// Terdapat method untuk mengkonversi beberapa tipe data.
		//
		class Convert final {
		public:
			//
			// Menkonversi string menjadi int.
			// 
			// @param value: String yang akan dikonversi.
			// 
			// @return int: String yang telah dikonversi menjadi int.
			//
			static int ToInt32(const std::string& value) {
				return std::stoi(value);
			}

			//
			// Menkonversi string menjadi long.
			// 
			// @param value: String yang akan dikonversi.
			// 
			// @return long: String yang telah dikonversi menjadi long.
			//
			static long ToLong(const std::string& value) {
				return std::stol(value);
			}

			//
			// Menkonversi string menjadi unsigned long.
			// 
			// @param value: String yang akan dikonversi.
			// 
			// @return unsigned long: String yang telah dikonversi menjadi unsigned long.
			//
			static unsigned long ToULong(const std::string& value) {
				return std::stoul(value);
			}

			//
			// Menkonversi string menjadi long long.
			// 
			// @param value: String yang akan dikonversi.
			// 
			// @return long long: String yang telah dikonversi menjadi long long.
			//
			static long long ToInt64(const std::string& value) {
				return std::stoll(value);
			}

			//
			// Menkonversi string menjadi unsigned long long.
			// 
			// @param value: String yang akan dikonversi.
			// 
			// @return unsigned long long: String yang telah dikonversi menjadi unsigned long long.
			//
			static unsigned long long ToUInt64(const std::string& value) {
				return std::stoull(value);
			}

			//
			// Menkonversi string menjadi float.
			// 
			// @param value: String yang akan dikonversi.
			// 
			// @return float: String yang telah dikonversi menjadi float.
			//
			static float ToFloat(const std::string& value) {
				return std::stof(value);
			}

			//
			// Menkonversi string menjadi double.
			// 
			// @param value: String yang akan dikonversi.
			// 
			// @return double: String yang telah dikonversi menjadi double.
			//
			static double ToDouble(const std::string& value) {
				return std::stod(value);
			}

			//
			// Menkonversi string menjadi long double.
			// 
			// @param value: String yang akan dikonversi.
			// 
			// @return long double: String yang telah dikonversi menjadi long double.
			//
			static long double ToLDouble(const std::string& value) {
				return std::stold(value);
			}

			//
			// Mengkonversi int menjadi string.
			// 
			// @param value: int yang akan dikonversi.
			// 
			// @return std::string: int yang telah dikonversi menjadi string.
			//
			static std::string ToString(const int value) {
				return std::to_string(value);
			}

			//
			// Mengkonversi unsigned int menjadi string.
			// 
			// @param value: unsigned int yang akan dikonversi.
			// 
			// @return std::string: unsigned int yang telah dikonversi menjadi string.
			//
			static std::string ToString(const unsigned int value) {
				return std::to_string(value);
			}

			//
			// Mengkonversi long menjadi string.
			// 
			// @param value: long yang akan dikonversi.
			// 
			// @return std::string: long yang telah dikonversi menjadi string.
			//
			static std::string ToString(const long value) {
				return std::to_string(value);
			}

			//
			// Mengkonversi unsigned long menjadi string.
			// 
			// @param value: unsigned long yang akan dikonversi.
			// 
			// @return std::string: unsigned long yang telah dikonversi menjadi string.
			//
			static std::string ToString(const unsigned long value) {
				return std::to_string(value);
			}

			//
			// Mengkonversi long long menjadi string.
			// 
			// @param value: long long yang akan dikonversi.
			// 
			// @return std::string: long long yang telah dikonversi menjadi string.
			//
			static std::string ToString(const long long value) {
				return std::to_string(value);
			}

			//
			// Mengkonversi unsigned long long menjadi string.
			// 
			// @param value: unsigned long long yang akan dikonversi.
			// 
			// @return std::string: unsigned long long yang telah dikonversi menjadi string.
			//
			static std::string ToString(const unsigned long long value) {
				return std::to_string(value);
			}

			//
			// Mengkonversi float menjadi string.
			// 
			// @param value: float yang akan dikonversi.
			// 
			// @return std::string: float yang telah dikonversi menjadi string.
			//
			static std::string ToString(const float value) {
				return std::to_string(value);
			}

			//
			// Mengkonversi double menjadi string.
			// 
			// @param value: double yang akan dikonversi.
			// 
			// @return std::string: double yang telah dikonversi menjadi string.
			//
			static std::string ToString(const double value) {
				return std::to_string(value);
			}

			//
			// Mengkonversi long double menjadi string.
			// 
			// @param value: long double yang akan dikonversi.
			// 
			// @return std::string: long double yang telah dikonversi menjadi string.
			//
			static std::string ToString(const long double value) {
				return std::to_string(value);
			}

			//
			// Mengkonversi warna menjadi string.
			// 
			// @param value: Warna yang akan dikonversi.
			// 
			// @return std::string: Nama dari warna yang telah dikonversi menjadi string.
			//
			static std::string ToString(const System::Color value) {
				switch (value)				{
				case System::Color::Black: return "Black";
				case System::Color::DarkBlue: return "DarkBlue";
				case System::Color::DarkGreen: return "DarkGreen";
				case System::Color::DarkCyan: return "DarkCyan";
				case System::Color::DarkRed: return "DarkRed";
				case System::Color::DarkMagenta: return "DarkMagenta";
				case System::Color::DarkYellow: return "DarkYellow";
				case System::Color::Gray: return "Gray";
				case System::Color::DarkGray: return "DarkGray";
				case System::Color::Blue: return "Blue";
				case System::Color::Green: return "Green";
				case System::Color::Cyan: return "Cyan";
				case System::Color::Red: return "Red";
				case System::Color::Magenta: return "Magenta";
				case System::Color::Yellow: return "Yellow";
				case System::Color::White: return "White";
				default: return "Error";
				}
			}

			//
			// Mengkonversi string menjadi lowercase.
			// 
			// @param value: String yang akan dikonversi.
			// 
			// @return std::string: String yang telah dikonversi menjadi lowercase.
			//
			static std::string ToLower(std::string value) {
				std::transform(value.begin(), value.end(), value.begin(), ::tolower);
				return value;
			}

			//
			// Mengkonversi string menjadi uppercase.
			// 
			// @param value: String yang akan dikonversi.
			// 
			// @return std::string: String yang telah dikonversi menjadi uppercase.
			//
			static std::string ToUpper(std::string value) {
				std::transform(value.begin(), value.end(), value.begin(), ::toupper);
				return value;
			}
		};

		class Tools final : System::Console {
		private:
			using Exception = System::Exception;
			inline static std::string _Message[] = { "[INFORMATION]", "[WARNING]", "[DANGER]" };

		public:
			//
			// Tipe pesan
			//
			enum class MessageType {
				Information = 0,
				Warning = 1,
				Danger = 2
			};

			//
			// Membersihkan string serta menghapusnya dari buffer.
			// 
			// @param position: Koordinat string yang akan dihapus.
			// @param value: String yang akan dihapus.
			//
			static void Clear(COORD position, std::string& value) {
				if (!value.empty()) {
					Delete(position, value.size());
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
			static void Clear(short x, short y, std::string& value) {
				Clear({ x, y }, value);
			}

			//
			// Menghapus nilai yang telah tercetak kedalam console.
			// 
			// @param position: Koordinat nilai yang akan dihapus.
			// @param length: Panjang nilai yang akan dihapus.
			//
			static void Delete(COORD position, size_t length) {
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
			static void Delete(short x, short y, size_t length) {
				Delete({ x, y }, length);
			}

			//
			// Membuat kata kunci acak.
			// 
			// @param numberOfKey: Banyaknya karakter.
			// 
			// @return String: String yang berisi karakter acak.
			//
			static std::string GenerateRandomKey(short numberOfKey) {
				const char keyList[] = "QWERTYUIOPASDFGHJKLZXCVBNM1234567890";
				std::string key;

				srand(static_cast<unsigned int>(time(0)));
				for (short i = 0; i < numberOfKey; i++)
					key += keyList[rand() % 36];
				return key;
			}

			//
			// Mengecek string apakah berupa angka atau bukan.
			// 
			// @param value: String yang akan dicek.
			// 
			// @return bool: [true] Jika string berupa angka, sebaliknya [false].
			//
			static bool IsDigit(std::string& value) {
				return !value.empty() && std::find_if(value.begin(), value.end(), [](char ch) {return !isdigit(ch); }) == value.end();
			}

			//
			// Membaca inputan angka dari keyboard.
			// 
			// @return double: Angka yang diinputkan.
			//
			static double ReadDigit() {
				char key;
				std::string line;

				while ((key = System::Console::ReadKey()) != '\r') {
					switch (key) {
					case '\b':
						if (!line.empty()) {
							line.pop_back();
							System::Console::Write("\b \b");
						}
						break;
					default:
						if (isdigit(key) || key == '.') {
							line += key;
							System::Console::Write(key);
						}
					}
				}
				return Convert::ToDouble(line);
			}

			//
			// Membaca inputan dari keyboard dengan membatasi jumlah karakter yang diinputkan.
			// 
			// @param limit: Batas karakter yang diinputkan.
			// 
			// @return std::string: Nilai yang diinputkan.
			//
			static std::string ReadLine(size_t limit) {
				char key;
				std::string line;

				while ((key = System::Console::ReadKey()) != '\r') {
					switch (key) {
					case '\b':
						if (!line.empty()) {
							line.pop_back();
							System::Console::Write("\b \b");
						}
						break;
					default:
						if (line.size() < limit) {
							line += key;
							System::Console::Write(key);
						}
					}
				}
				return line;
			}

			//
			// Membaca inputan dari keyboard dengan menyamarkan karakter yang diinputkan.
			// 
			// @return std::string: Nilai yang diinputkan.
			//
			static std::string ReadPassword() {
				char key;
				std::string line;

				while ((key = System::Console::ReadKey()) != '\r') {
					switch (key) {
					case '\b':
						if (!line.empty()) {
							line.pop_back();
							System::Console::Write("\b \b");
						}
						break;
					default:
						line += key;
						System::Console::Write("*");
					}
				}
				return line;
			}

			//
			// Membaca inputan dari keyboard dengan menyamarkan dan membatasi karakter yang diinputkan.
			// 
			// @param limit: Batas karakter yang diinputkan.
			// 
			// @return std::string: Nilai yang diinputkan.
			//
			static std::string ReadPassword(size_t limit) {
				char key;
				std::string line;

				while ((key = System::Console::ReadKey()) != '\r') {
					switch (key) {
					case '\b':
						if (!line.empty()) {
							line.pop_back();
							System::Console::Write("\b \b");
						}
						break;
					default:
						if (line.size() < limit) {
							line += key;
							System::Console::Write("*");
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
				System::Console::Cursor::Position = position;
				System::Console::Write(value...);
			}

			//
			// Menulis nilai kedalam console dengan posisi yang spesifik.
			// 
			// @param x: Koordinat X penulisan nilai.
			// @param y: Koordinat Y penulisan nilai.
			// @param value: Nilai yang akan ditulis.
			//
			template<typename... T>
			static void Write(short x, short y, T... value) {
				Write({ x, y }, value...);
			}

			//
			// Menulis nilai kedalam console dengan warna buffer yang spesifik.
			// 
			// @param color: Warna buffer.
			// @param value: Nilai yang akan ditulis.
			//
			template<typename... T>
			static void Write(System::ConsoleColor color, T... value) {
				System::ConsoleColor defaultColor = System::Console::Buffer::Color;

				System::Console::Buffer::Color = color;
				System::Console::Write(value...);
				System::Console::Buffer::Color = defaultColor;
			}

			//
			// Menulis milai kedalam console dengan warna buffer yang spesifik.
			// 
			// @param background: Warna latar belakang buffer.
			// @param foreground: Warna latar depan buffer.
			// @param value: Nilai yang akan ditulis.
			//
			template<typename... T>
			static void Write(System::Color background, System::Color foreground, T... value) {
				Write({ background, foreground }, value...);
			}


			//
			// Menulis kedalam console dengan posisi dan warna buffer yang spesifik.
			// 
			// @param position: Koordinat penulisan nilai.
			// @param color: Warna buffer.
			// @param value: Nilai yang akan ditulis.
			//
			template<typename... T>
			static void Write(COORD position, System::ConsoleColor color, T... value) {
				System::ConsoleColor defaultColor = System::Console::Buffer::Color;

				System::Console::Cursor::Position = position;
				System::Console::Buffer::Color = color;
				System::Console::Write(value...);
				System::Console::Buffer::Color = defaultColor;
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
			static void Write(COORD position, System::Color background, System::Color foreground, T... value) {
				Write(position, { background, foreground }, value...);
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
			static void Write(short x, short y, System::ConsoleColor color, T... value) {
				Write({ x, y }, color, value...);
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
			static void Write(short x, short y, System::Color background, System::Color foreground, T... value) {
				Write({ x, y }, { background, foreground }, value...);
			}

			//
			// Menulis pesan kedalam console dengan kode pesan yang spesifik.
			// 
			// @param type: Tipe pesan yang akan disampaikan.
			// @param message: Pesan yang disampaikan.
			//
			static void WriteMessage(MessageType type, std::string message) {
				switch (type) {
				case MessageType::Information:
					Write(System::Color::Cyan, System::Color::Black, _Message[(int)type]);
					break;
				case MessageType::Warning:
					Write(System::Color::DarkYellow, System::Color::Black, _Message[(int)type]);
					break;
				case MessageType::Danger:
					Write(System::Color::DarkRed, System::Color::Black, _Message[(int)type]);
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
			static void WriteMessage(COORD position, MessageType type, std::string message) {
				switch (type) {
				case MessageType::Information:
					Write(position, System::Color::Cyan, System::Color::Black, _Message[(int)type]);
					break;
				case MessageType::Warning:
					Write(position, System::Color::DarkYellow, System::Color::Black, _Message[(int)type]);
					break;
				case MessageType::Danger:
					Write(position, System::Color::DarkRed, System::Color::Black, _Message[(int)type]);
					break;
				}

				Console::Write(" ", message);
				Console::ReadKey();
				Delete(position, _Message[(int)type].length() + message.length() + 1);
			}

			//
			// Menulis pesan kedalam console dengan posisi dan kode pesan yang spesifik.
			// 
			// @param x: Koordinat X penulisan pesan.
			// @param y: Koordinat Y penulisan pesan.
			// @param type: Tipe pesan yang akan disampaikan.
			// @param message: Pesan yang disampaikan.
			//
			static void WriteMessage(short x, short y, MessageType type, std::string message) {
				WriteMessage({ x, y }, type, message);
			}

			//
			// Menulis nilai kedalam console dengan posisi yang spesifik, diakhiri dengan baris baru.
			// 
			// @param position: Koordinat penulisan nilai.
			// @param value: Nilai yang akan ditulis.
			//
			template<typename... T>
			static void WriteLine(COORD position, T... value) {
				Write(position, value..., "\n");
			}

			//
			// Menulis nilai kedalam console dengan posisi yang spesifik, diakhiri dengan baris baru.
			// 
			// @param x: Koordinat X penulisan nilai.
			// @param y: Koordinat Y penulisan nilai.
			// @param value: Nilai yang akan ditulis.
			//
			template<typename... T>
			static void WriteLine(short x, short y, T... value) {
				WriteLine({ x, y }, value...);
			}

			//
			// Menulis nilai kedalam console dengan warna buffer yang spesifik, diakhiri dengan baris baru.
			// 
			// @param color: Warna buffer.
			// @param value: Nilai yang akan ditulis.
			//
			template<typename... T>
			static void WriteLine(System::ConsoleColor color, T... value) {
				Write(color, value..., "\n");
			}

			//
			// Menulis milai kedalam console dengan warna buffer yang spesifik, diakhiri dengan baris baru.
			// 
			// @param background: Warna latar belakang buffer.
			// @param foreground: Warna latar depan buffer.
			// @param value: Nilai yang akan ditulis.
			//
			template<typename... T>
			static void WriteLine(System::Color background, System::Color foreground, T... value) {
				WriteLine({ background, foreground }, value...);
			}

			//
			// Menulis kedalam console dengan posisi dan warna buffer yang spesifik, diakhiri dengan baris baru.
			// 
			// @param position: Koordinat penulisan nilai.
			// @param color: Warna buffer.
			// @param value: Nilai yang akan ditulis.
			//
			template<typename... T>
			static void WriteLine(COORD position, System::ConsoleColor color, T... value) {
				Write(position, color, value..., "\n");
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
			static void WriteLine(COORD position, System::Color background, System::Color foreground, T... value) {
				WriteLine(position, { background, foreground }, value...);
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
			static void WriteLine(short x, short y, System::ConsoleColor color, T... value) {
				WriteLine({ x, y }, color, value...);
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
			static void WriteLine(short x, short y, System::Color background, System::Color foreground, T... value) {
				WriteLine({ x, y }, { background, foreground }, value...);
			}

			//
			// Menulis pesan kedalam console dengan kode pesan yang spesifik, diakhiri dengan baris baru.
			// 
			// @param type: Tipe pesan yang akan disampaikan.
			// @param message: Pesan yang disampaikan.
			//
			static void WriteLineMessage(MessageType type, std::string message) {
				WriteMessage(type, message + "\n");
			}

			//
			// Menulis pesan kedalam console dengan posisi dan kode pesan yang spesifik, diakhiri dengan baris baru.
			//
			// @param position: Koordinat penulisan pesan.
			// @param type: Tipe pesan yang akan disampaikan.
			// @param message: Nilai yang akan ditulis.
			//
			static void WriteLineMessage(COORD position, MessageType type, std::string message) {
				WriteMessage(position, type, message + "\n");
			}

			//
			// Menulis pesan kedalam console dengan posisi dan kode pesan yang spesifik, diakhiri dengan baris baru.
			// 
			// @param x: Koordinat X penulisan pesan.
			// @param y: Koordinat Y penulisan pesan.
			// @param type: Tipe pesan yang akan disampaikan.
			// @param message: Nilai yang akan ditulis.
			//
			static void WriteLineMessage(short x, short y, MessageType type, std::string message) {
				WriteLineMessage({ x, y }, type, message);
			}
		};

		//
		// Membuat menu dengan arrow-key sebagai navigasi menu.
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
		// Note
		// ====
		// Karena buffer terbatas sebanyak SHRT_MAX maka menu yang bisa ditampilkan <= SHRT_MAX.
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
			COORD _Coordinate; // Untuk menyimpan posisi menu.
			short _Limit = 0; // Untuk menampung limit menu terhadap window frame.
			System::ConsoleColor _Color; // Untuk menyimpan warna cursor.

			//
			// Untuk menyimpan menu
			//
			struct {
				std::vector<std::string> Back; // Menu asli.
				std::vector<std::string> Front; // Menu yang telah dimodifikasi.
			} _Menu;

			//
			// Untuk menyimpan posisi cursor dan index.
			//
			struct {
				short Begin;
				short Current;
				short End;
			} _Cursor, _Index;

		public:
			//
			// Untuk menampung hasil pilihan menu. 
			// Index: Index ke- dari menu yang dipilih.
			// Value: Nama dari menu yang dipilih.
			//
			struct Selected {
				short Index;
				std::string Value;
			};

			//
			// Menginisialisasi menu.
			// 
			// @param menu: Menu yang akan dibuat.
			// @param cursorColor: Warna cursor yang akan digunakan.
			// @param position: Koordinat menu saat ditampilkan.
			// @param fill: [true] Panjang cursor mengikuti index menu terpanjang, sebaliknya [false]. Default: [false]
			//
			ConsoleMenu(std::initializer_list<std::string> menu, System::ConsoleColor cursorColor, COORD position, bool fill = false) : _Menu({ menu, menu }), _Color(cursorColor), _Coordinate(position) {
				if (fill) {
					size_t max = 0;

					for (std::string index : menu)
						max = index.size() > max ? index.size() : max;
					for (std::string& index : _Menu.Front)
						index += std::string(max - index.size(), ' ');
				}

				_Index = { 0, 0, (short)menu.size() - 1 };
				_Cursor = { position.Y, position.Y, position.Y + (short)menu.size() - 1 };
			}

			//
			// Membersihkan menu yang telah tertulis kedalam buffer.
			//
			void Clear() {
				short y = _Coordinate.Y;

				for (short i = 0, index = (short)_Index.Begin; i < _Limit; i++, index++)
					Tools::Delete(_Coordinate.X, y, _Menu.Front[index].size());
			}

			//
			// Menampilkan menu.
			// 
			// @return Selected: Menu yang dipilih.
			//
			Selected Print() {
				char key; // Untuk menampung arrow-key.
				COORD buffer = System::Console::Buffer::Size; // Ukuran buffer.

				System::Console::Cursor::Visible = false;
				if ((_Coordinate.Y + (short)_Menu.Back.size()) < buffer.Y) {
					short y = _Coordinate.Y; // Untuk menapung posisi y ketika menampilkan menu.

					_Limit = (short)_Menu.Back.size();
					for (short i = 0; i <= _Index.End; i++, y++)
						Tools::Write(_Coordinate.X, y, _Menu.Front[i]);

					//
					// Main looping.
					//
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

					//
					// Main looping.
					//
					do {
						short y = _Coordinate.Y; // Untuk menapung posisi y ketika menampilkan menu.
						short index = _Index.Begin; // Untuk menampung index awal

						for (short i = 0; i < _Limit; i++, y++, index++)
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
					_Menu.Front[_Index.Current]
				};
			}

			//
			// Menampilkan menu dengan membatasi jumlah menu yang ditampilkan.
			// 
			// @param limit: Batas menu yang ditampilkan.
			// 
			// @return Selected: Menu yang dipilih.
			//
			Selected Print(short limit) {
				char key; // Untuk menampung arrow-key.

				_Cursor.End = _Coordinate.Y + limit - 1;
				System::Console::Cursor::Visible = false;

				//
				// Main looping
				//
				do {
					short y = _Coordinate.Y; // Untuk menapung posisi y ketika menampilkan menu.
					short index = _Index.Begin; // Untuk menampung index awal

					for (short i = 0; i < limit; i++, y++, index++)
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
							_Cursor.Current = _Cursor.End, _Index.Current = _Index.End, _Index.Begin = _Index.End - limit + 1;
						break;
					}
				} while (key != '\r');
				System::Console::Cursor::Visible = true;
				return {
					_Index.Current,
					_Menu.Front[_Index.Current]
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
			using Exception = System::Exception;
			using Header = std::vector<std::string>;
			using Row = std::vector<std::vector<std::string>>;
			using Width = std::vector<size_t>;

			Header _Header;
			Row _Row;
			Width _Width;
			short _Padding;

			struct RowType {
				std::string Left;
				std::string Intersect;
				std::string Right;
			};

			struct Border {
				std::string Horizontal;
				std::string Vertical;
				RowType Top;
				RowType Middle;
				RowType Bottom;
			};

			Border _Basic = { "-", "|", {"+", "+", "+"}, {"+", "+", "+"}, {"+", "+", "+"} };
			Border _Line = { "━", "┃", {"┏", "┳", "┓"}, {"┣", "╋", "┫"}, {"┗", "┻", "┛"} };
			Border _DoubleLine = { "═", "║", {"╔", "╦", "╗"}, {"╠", "╬", "╣"}, {"╚", "╩", "╝"} };
			Border _Invisible = { " ", " ", {" ", " ", " "}, {" ", " ", " "}, {" ", " ", " "} };
			Border _Border = _Basic;

			std::string _GetHeader(Header header) const {
				std::stringstream line;

				line << _Border.Vertical;
				for (size_t i = 0; i < header.size(); i++) {
					std::string index = header[i];

					line << std::string(_Padding, ' ') + index + std::string((_Width[i] - index.size()), ' ') + std::string(_Padding, ' ');
					line << _Border.Vertical;
				}
				line << "\n";
				return line.str();
			}

			std::string _GetLine(RowType position) const {
				std::stringstream line;

				line << position.Left;
				for (size_t i = 0; i < _Width.size(); i++) {
					for (size_t j = 0; j < (_Width[i] + _Padding + _Padding); j++)
						line << _Border.Horizontal;
					line << (i == _Width.size() - 1 ? position.Right : position.Intersect);
				}
				line << "\n";
				return line.str();
			}

			std::string _GetRow(Row row) const {
				std::stringstream line;
				
				for (auto& index : row) {
					line << _Border.Vertical;
					for (size_t i = 0; i < index.size(); i++) {
						line << std::string(_Padding, ' ') + index[i] + std::string((_Width[i] - index[i].size()), ' ') + std::string(_Padding, ' ');
						line << _Border.Vertical;
					}
					line << "\n";
				}
				return line.str();
			}

			short _GetHeight() const {
				return (short)_Row.size() + 5;
			}

			short _GetWidth() const {
				size_t size = 2;

				for (size_t length : _Width)
					size += (length + _Padding + _Padding + 1);
				return (short)size;
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
			// @param padding: Jarak antara border ke cell. Default: 1.
			//
			ConsoleTable(std::initializer_list<std::string> header, short padding = 1) : _Header(header), _Padding(padding) {
				for (const std::string& index : header)
					_Width.push_back(index.size());
			}

			//
			// Menginisialisasi tabel.
			// 
			// @param header: Header tabel.
			// @param padding: Jarak antara border ke cell. Default: 1.
			//
			ConsoleTable(std::vector<std::string> header, short padding = 1) : _Header(header), _Padding(padding) {
				for (const std::string& index : header)
					_Width.push_back(index.size());
			}

			ConsoleTable& operator+=(std::initializer_list<std::string> row) {
				Add(row);
				return *this;
			}

			ConsoleTable& operator+=(std::vector<std::string> row) {
				Add(row);
				return *this;
			}

			ConsoleTable& operator-=(size_t rowIndex) {
				Remove(rowIndex);
				return *this;
			}

			//
			// Menambahkan row kedalam tabel.
			// 
			// @param row: Row yang akan ditambahkan.
			//
			void Add(std::initializer_list<std::string> row) {
				if (row.size() > _Width.size())
					Error("Ukuran row harus sama dengan header.");

				auto res = std::vector<std::string>{ row };
				_Row.push_back(res);
				for (size_t i = 0; i < res.size(); i++)
					_Width[i] = res[i].size() > _Width[i] ? res[i].size() : _Width[i];
			}

			//
			// Menambahkan row kedalam tabel.
			// 
			// @param row: Row yang akan ditambahkan.
			//
			void Add(std::vector<std::string> row) {
				if (row.size() > _Width.size())
					Error("Ukuran row harus sama dengan header.");

				_Row.push_back(row);
				for (size_t i = 0; i < row.size(); i++)
					_Width[i] = row[i].size() > _Width[i] ? row[i].size() : _Width[i];
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
			// Menulis kedalam console dengan mengubah ukuran buffer mengikuti ukuran tabel.
			//
			void Print() {
				COORD buffer = System::Console::Buffer::Size;
				COORD cursor = System::Console::Cursor::Position;
				short width = _GetWidth();
				short height = _GetHeight();

				System::Console::Buffer::Size = {
					width > buffer.X ? width : buffer.X,
					height > buffer.Y ? height : buffer.Y
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
			// @param ascending: [true] tabel akan difilter secara ascending, sebaliknya [false]. Default: [true]
			//
			void Sort(bool ascending = true) {
				if (ascending)
					std::sort(_Row.begin(), _Row.end(), std::less<std::vector<std::string>>());
				else
					std::sort(_Row.begin(), _Row.end(), std::greater<std::vector<std::string>>());
			}

			//
			// Mengupdate header berdasarkan index header.
			// 
			// @param index: Index header yang akan diupdate.
			// @param  newHeader: Header baru.
			//
			void UpdateHeader(size_t index, std::string newHeader) {
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
			void UpdateRow(size_t rowIndex, size_t headerIndex, std::string newValue) {
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