//
// Simple
// 
// Tujuan pembuatan library ini sebagai bahan pembelajaran.
// Masih terdapat bug atau penggunaan tidak tepat didalam library ini, jadi berhati-hatilah
// dalam menggunakannya. Tidak semua kode yang terdapat disini ditulis sendiri,
// namun ada beberapa yang diambil dari berbagai sumber. Jika ingin menambahkan, mengurangi, atau memodifikasi
// kode silakan, saya harap dengan kontribusi anda library ini menjadi lebih baik lagi.
// 
// Required C++17
// Written in Visual Studio 2019
// Fahri Synyster
//

#pragma once

#include <iostream>
#include <string>
#include <conio.h>
#include <windows.h>
#include <map>
#include <filesystem>
#include <fstream>
#include <vector>
#include <time.h>
#include <sstream>

namespace Simple {
	using Int8 = char;
	using Int16 = short;
	using Int32 = int;
	using Int64 = long long;
	using UInt8 = unsigned char;
	using UInt16 = unsigned short;
	using UInt32 = unsigned int;
	using UInt64 = unsigned long long;
	using CString = const char*;
	using String = std::string;
	using StringStream = std::stringstream;

	constexpr const char* NewLine = "\n";

	//
	// Terdapat kelas-kelas yang berhubungan dengan system(windows)
	//
	namespace System {
		//
		// Warna dasar pada console
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
		// Menulis warna pada console
		// 
		// @param out: Output stream untuk menulis kedalam console
		// @param color: Warna yang akan ditulis
		// 
		// @return std::ostream&: Output stream yang akan menulis warna
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
		// Struktur dasar warna pada console
		//
		struct ConsoleColor {
			Color Background;
			Color Foreground;
		};

		//
		// Kelas untuk menangkap error yang dilemparkan ketika program berjalan
		//
		class Exception {
		private:
			Int32 _Line;
			CString _File;
			CString _Function;
			CString _Description;

		public:
			Exception(CString file, Int32 line, CString function, CString description) :_File(file), _Line(line), _Function(function), _Description(description) {}
			const CString Description = _Description;
			const CString File = _File;
			const CString Function = _Function;
			const Int32 Line = _Line;
		};

	//
	// Untuk melempar Exception, dimana akan otomatis mengisi parameter sebagai berikut: 
	// throw Exception(__FILE__, __LINE__, __FUNCTION__, d)
	// 
	// @param d: Deskripsi/pesan error yang disampaikan
	//
	#define Error(d) throw Exception(__FILE__, __LINE__, __FUNCTION__, d)

		//
		// Baca/tulis kedalam file dengan mode binary berdasarkan objek
		//
		template<typename T>
		class BinaryFile {
		protected:
			std::filesystem::path _FileName;
			std::filesystem::path _FileTemp;
			std::fstream _File;
			std::fstream _Temp;

		public:
			BinaryFile() :_FileName("BinaryFile.bin") {
				_File.open(_FileName, std::ios::out | std::ios::app);
				if (!_File)
					Error("Gagal membuka file");
				_File.close();
			}

			BinaryFile(CString fileName) :_FileName(fileName) {
				if (_FileName.empty())
					_FileName = "BinaryFile.bin";
				else if (!_FileName.has_extension())
					_FileName += ".bin";
				else if (_FileName.extension() != ".bin")
					_FileName.replace_extension(".bin");

				_File.open(_FileName, std::ios::out | std::ios::app);
				if (!_File)
					Error("Gagal membuka file");
				_File.close();
			}
			
			//
			// Menghapus data dari file berdasarkan posisi/urutan saat ditulis
			//
			void Delete(size_t position) {
			//
			// Menghapus dengan cara ini akan menampung seluruh data yang ada di dalam file kedalam vector,
			// lalu index ke-posisi akan dihapus terlebih daluhu dari vector, selanjutnya vector akan ditulis ulang
			// kembali kedalam file secara satu-persatu
			//
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
					Error("Gagal membuka file");
				_Temp.open(_FileTemp, std::ios::out | std::ios::binary);
				if (!_Temp)
					Error("Gagal membuka file");

				for (size_t i = 0; _File.read((char*)&temp, sizeof T); i++)
					if (i != position)
						_Temp.write((char*)&temp, sizeof T);

				_File.close();
				_Temp.close();

				remove(_FileName);
				rename(_FileTemp, _FileName);
			#endif
			}

			//
			// Mengecek apakah file kosong atau tidak
			// 
			// @return bool: [True] jika file kosong, sebaliknya [False]
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
			// Membaca seluruh data dari dalam file, lalu menampungnya kedalam vector
			// 
			// @return std::vector: Data yang telah dibaca
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
			// Memperbarui data didalam file berdasarkan posisi/urutan saat ditulis kedalam file
			// 
			// @param position: Posisi data pada file
			// @param newData: Data baru yang akan ditulis
			//
			void Update(size_t position, T newData) {
				_File.open(_FileName, std::ios::in | std::ios::out | std::ios::binary);
				if (!_File)
					Error("Gagal membuka file.");
				_File.seekp(position * sizeof T, std::ios::beg);
				_File.write((char*)&newData, sizeof T);
				_File.close();
			}

			//
			// Menulis data kedalam file
			// 
			// @param value: Data yang akan ditulis
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
		// Terdapat method-method yang berhubungan dengan console
		//
		class Console {
		protected:
			inline static ConsoleColor _BaseColor = { Color::Black, Color::Gray };
			inline static CONSOLE_SCREEN_BUFFER_INFO _Buffer = {};
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
			inline static CONSOLE_CURSOR_INFO _Cursor = {};
			inline static HANDLE _Handle = GetStdHandle(STD_OUTPUT_HANDLE);

		public:
			//
			// Membersihkan buffer
			//
			static void Clear() {
				system("cls");
			}

			//
			// Menahan program hingga user menekan enter
			// Didalam fungsi ini terdapat method cin.get(), tapi karena dideklarasikan dengan attribut void jadi
			// fungsi ini tidak mereturn apa-apa
			// 
			// Gatau benar atau salah :D
			//
			static void Hold() {
				std::cin.get();
			}

			//
			// Menahan program hingga user menekan keyboard
			// Didalam fungsi ini terdapat method _getch(), tapi karena dideklarasikan dengan attribut void jadi
			// fungsi ini tidak mereturn apa-apa
			// 
			// Gatau benar atau salah :D
			//
			static void HoldKey() {
				_getch();
			}

			//
			// Menulis kedalam console
			// 
			// @param value: Nilai yang akan ditulis
			//
			template<typename... T>
			static void Write(T... value) {
				((std::cout << value), ...);
			}

			//
			// Menulis kedalam console diakhiri dengan baris baru
			// 
			// @param value: Nilai yang akan ditulis
			//
			template<typename... T>
			static void WriteLine(T... value) {
				((std::cout << value), ...) << "\n";
			}

		public:
			//
			// Mendapatkan atau mengatur warna buffer pada console
			//
			class {
			public:
				operator ConsoleColor() const {
					if (!GetConsoleScreenBufferInfo(_Handle, &_Buffer))
						Error("Gagal mendapatkan informasi buffer.");

					return {
						_Color.find(_Buffer.wAttributes / 16)->second,
						_Color.find(_Buffer.wAttributes % 16)->second
					};
				}

				ConsoleColor& operator=(ConsoleColor const color) {
					if (!SetConsoleTextAttribute(_Handle, static_cast<int>(color.Background) << 4 | static_cast<int>(color.Foreground)))
						Error("Gagal mengatur warna buffer.");

					return _BaseColor = color;
				}
			} inline static BufferColor;

			//
			// Mendapatkan atau mengatur ukuran buffer
			//
			class {
			public:
				operator COORD() const {
					if (!GetConsoleScreenBufferInfo(_Handle, &_Buffer))
						Error("Gagal mendapatkan informasi buffer.");

					return _Buffer.dwSize;
				}

				COORD& operator=(COORD const size) {
					if (!SetConsoleScreenBufferSize(_Handle, size))
						Error("Gagal mengatur ukuran buffer.");

					return _Buffer.dwSize = size;
				}
			} inline static BufferSize;

			//
			// Mendapatkan atau mengatur posisi cursor pada console
			//
			class {
			public:
				operator COORD() const {
					if (!GetConsoleScreenBufferInfo(_Handle, &_Buffer))
						Error("Gagal mendapatkan informasi buffer.");

					return _Buffer.dwCursorPosition;
				}

				COORD& operator=(COORD const size)
				{
					if (!SetConsoleCursorPosition(_Handle, size))
						Error("Gagal mengatur posisi cursor.");

					return _Buffer.dwCursorPosition = size;
				}
			} inline static CursorPosition;

			//
			// Mendapatkan atau mengatur ekstensi cursor
			//
			class {
			public:
				operator bool() const {
					if (!GetConsoleCursorInfo(_Handle, &_Cursor))
						Error("Gagal mendapatkan informasi cursor.");

					return _Cursor.bVisible;
				}

				bool& operator=(bool const value) {
					if (!GetConsoleCursorInfo(_Handle, &_Cursor))
						Error("Gagal mendapatkan informasi cursor.");
					_Cursor.bVisible = value;
					if (!SetConsoleCursorInfo(_Handle, &_Cursor))
						Error("Gagal mengatur ekstensi cursor.");
				}
			} inline static CursorVisible;

			//
			// Membaca inputan keyboard dengan menekan enter
			// 
			// @return char: Karakter yang diinputkan
			//
			class {
			public:
				operator char() const {
					return std::cin.get();
				}
			} inline static Read;

			//
			// Membaca inputan keyboard tanpa menekan enter
			// 
			// @return char: Karakter yang diinputkan
			//
			class {
			public:
				operator char() const {
					return _getch();
				}
			} inline static ReadKey;

			//
			// Membaca inputan kalimat
			// 
			// @return String: Kalimat yang diinputkan
			//
			class {
			public:
				operator String() const {
					String line;

					std::getline(std::cin, line);

					return line;
				}
			} inline static ReadLine;
		};

		//
		// Class yang didalamnya terdapat method untuk mengkonversi dua buah tipe data
		//
		class Convert final {
		public:
			//
			// Mengembalikan nilai Int32
			// 
			// @param value: Nilai yang akan dikonversi
			// 
			// @return Int32: Value yang telah dikonversi
			//
			static Int32 ToInt32(String value) {
				return std::stoi(value);
			}

			//
			// Mengembalikan nilai long
			// 
			// @param value: Nilai yang akan dikonversi
			// 
			// @return long: Value yang telah dikonversi
			//
			static long ToLong(String value) {
				return std::stol(value);
			}

			//
			// Mengembalikan nilai unsigned long
			// 
			// @param value: Nilai yang akan dikonversi
			// 
			// @return unsigned long: Value yang telah dikonversi
			//
			static unsigned long ToULong(String value) {
				return std::stoul(value);
			}

			//
			// Mengembalikan nilai Int64
			// 
			// @param value: Nilai yang akan dikonversi
			// 
			// @return Int64: Value yang telah dikonversi
			//
			static Int64 ToInt64(String value) {
				return std::stoll(value);
			}

			//
			// Mengembalikan nilai UInt64
			// 
			// @param value: Nilai yang akan dikonversi
			// 
			// @return UInt64: Value yang telah dikonversi
			//
			static UInt64 ToUInt64(String value) {
				return std::stoull(value);
			}

			//
			// Mengembalikan nilai float
			// 
			// @param value: Nilai yang akan dikonversi
			// 
			// @return float: Value yang telah dikonversi
			//
			static float ToFloat(String value) {
				return std::stof(value);
			}

			//
			// Mengembalikan nilai double
			// 
			// @param value: Nilai yang akan dikonversi
			// 
			// @return double: Value yang telah dikonversi
			//
			static double ToDouble(String value) {
				return std::stod(value);
			}

			//
			// Mengembalikan nilai long double
			// 
			// @param value: Nilai yang akan dikonversi
			// 
			// @return long double: Value yang telah dikonversi
			//
			static long double ToLDouble(String value) {
				return std::stold(value);
			}

			//
			// Mengembalikan nilai const char* / CString
			// 
			// @param value: Nilai yang akan dikonversi
			// 
			// @return CString: Value yang telah dikonversi
			//
			static CString ToCString(String& value) {
				return value.c_str();
			}

			//
			// Mengembalikan nilai string
			// 
			// @param value: Nilai yang akan dikonversi
			// 
			// @return String: Value yang telah dikonversi
			//
			static String ToString(Int32 value) {
				return std::to_string(value);
			}

			//
			// Mengembalikan nilai string
			// 
			// @param value: Nilai yang akan dikonversi
			// 
			// @return String: Value yang telah dikonversi
			//
			static String ToString(UInt32 value) {
				return std::to_string(value);
			}

			//
			// Mengembalikan nilai string
			// 
			// @param value: Nilai yang akan dikonversi
			// 
			// @return String: Value yang telah dikonversi
			//
			static String ToString(long value) {
				return std::to_string(value);
			}

			//
			// Mengembalikan nilai string
			// 
			// @param value: Nilai yang akan dikonversi
			// 
			// @return String: Value yang telah dikonversi
			//
			static String ToString(unsigned long value) {
				return std::to_string(value);
			}

			//
			// Mengembalikan nilai string
			// 
			// @param value: Nilai yang akan dikonversi
			// 
			// @return String: Value yang telah dikonversi
			//
			static String ToString(Int64 value) {
				return std::to_string(value);
			}

			//
			// Mengembalikan nilai string
			// 
			// @param value: Nilai yang akan dikonversi
			// 
			// @return String: Value yang telah dikonversi
			//
			static String ToString(UInt64 value) {
				return std::to_string(value);
			}

			//
			// Mengembalikan nilai string
			// 
			// @param value: Nilai yang akan dikonversi
			// 
			// @return String: Value yang telah dikonversi
			//
			static String ToString(double value) {
				return std::to_string(value);
			}

			//
			// Mengembalikan nilai string
			// 
			// @param value: Nilai yang akan dikonversi
			// 
			// @return String: Value yang telah dikonversi
			//
			static String ToString(float value) {
				return std::to_string(value);
			}

			//
			// Mengembalikan nilai string
			// 
			// @param value: Nilai yang akan dikonversi
			// 
			// @return String: Value yang telah dikonversi
			//
			static String ToString(long double value) {
				return std::to_string(value);
			}
		};

		//
		// Struktur untuk menampung waktu dalam system
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
		// Kelas yang digunakan untuk mengambil waktu dari system
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
			class {
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
			} inline static Current;

			//
			// Mengambil waktu dan tanggal saat ini
			// 
			// @return DateTime: Waktu dan tanggal saat ini
			//
			class {
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
			} inline static Now;

			//
			// Mengambil tanggal saat ini
			// 
			// @return DateTime: Tanggal saat ini
			//
			class {
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
			} inline static Today;
		};
	}

	//
	// Terdapat kelas-kelas buatan untuk mempermudah dalam menulis kode
	//
	namespace Utility {
		//
		// Standar struktur untuk menampung duah buah tipe data yang berbeda
		//
		template<typename T, typename T1>
		struct Result {
			T First;
			T1 Second;
		};

		//
		// Kode pesan untuk ditampilkan saat terjadi error
		//
		enum class Message {
			Information = 0,
			Warning = 1,
			Danger = 2
		};

		//
		// Terdapat method-method untuk mengenkripsi/mendekripsi sebuah string
		//
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
				size_t keySize = key.size();

				for (size_t i = 0;; i++) {
					if (key.size() >= value.size())
						break;

					key += key[i];
				}

				for (size_t i = 0; i < value.size(); i++) {
					char x = (value[i] + key[i]) % 26;

					_ += x += 'A';
				}

				return _;
			}
		};

		//
		// Terdapat custom-method untuk mempermudah menulis kode
		//
		class Tools final : private System::Console {
		private:
			using Color = System::Color;
			using ConsoleColor = System::ConsoleColor;
			using Convert = System::Convert;
			using Exception = System::Exception;

			inline static String _Message[] = { "[INFORMATION]", "[WARNING]", "[DANGER]" };

		public:
			//
			// Menghapus string yang telah tercetak kedalam console
			// 
			// @param position: Posisi string yang akan dihapus
			// @param value: String yang akan dihapus
			//
			static void Clear(COORD position, String& value) {
				if (!value.empty()) {
					Delete(position, value.length());
					value.clear();
				}
			}

			//
			// Menghapus nilai yang telah tercetak kedalam console
			// 
			// @param position: Koordinat nilai yang akan dihapus
			// @param length: Panjang nilai yang akan dihapus
			//
			static void Delete(COORD position, size_t length) {
				DWORD ch;
				
				if (!GetConsoleScreenBufferInfo(_Handle, &_Buffer))
					Error("Gagal mendapatkan informasi buffer.");
				if (!FillConsoleOutputCharacter(_Handle, ' ', (DWORD)length, position, &ch))
					Error("Gagal menghapus teks.");
				if (!GetConsoleScreenBufferInfo(_Handle, &_Buffer))
					Error("Gagal mendapatkan informasi buffer.");
				if (!FillConsoleOutputAttribute(_Handle, _Buffer.wAttributes, (DWORD)length, position, &ch))
					Error("Gagal mengisi attribute.");
			}

			//
			// Menghapus nilai yang telah tercetak kedalam console
			// 
			// @param x: Koordinat X nilai yang akan dihapus
			// @param y: Koordinat X nilai yang akan dihapus
			// @param length: Panjang nilai yang akan dihapus
			//
			static void Delete(Int16 x, Int16 y, size_t length) {
				DWORD ch;

				if (!GetConsoleScreenBufferInfo(_Handle, &_Buffer))
					Error("Gagal mendapatkan informasi buffer.");
				if (!FillConsoleOutputCharacter(_Handle, ' ', (DWORD)length, { x, y }, &ch))
					Error("Gagal menghapus teks.");
				if (!GetConsoleScreenBufferInfo(_Handle, &_Buffer))
					Error("Gagal mendapatkan informasi buffer.");
				if (!FillConsoleOutputAttribute(_Handle, _Buffer.wAttributes, (DWORD)length, { x, y }, &ch))
					Error("Gagal mengisi attribute.");
			}

			//
			// Membuat kata kunci acak
			// 
			// @param numberOfKey: Banyaknya karakter
			// 
			// @return String: String yang berisi karakter acak
			//
			static String GenerateRandomKey(UInt32 numberOfKey) {
				const char keyList[] = "QWERTYUIOPASDFGHJKLZXCVBNM1234567890";
				String key;

				srand((UInt32)time(0));
				for (UInt32 i = 0; i < numberOfKey; i++)
					key += keyList[rand() % 36];

				return key;
			}

			//
			// Mengecek string apakah berupa angka atau bukan
			// 
			// @param value: String yang akan dicek
			// 
			// @return bool: [True] Jika string berupa angka, sebaliknya [False]
			//
			static bool IsNumber(String& value) {
				return !value.empty() && std::find_if(value.begin(), value.end(), [](UInt16 ch) {return !isdigit(ch); }) == value.end();
			}

			//
			// Membandingkan dua buah nilai dengan tipe data yang sama
			// 
			// @param first: Nilai pertama
			// @param second: Nilai kedua
			// 
			// @return T: Nilai terbesar setelah dibandingkan
			//
			template<typename T>
			static T Max(T first, T second) {
				return first > second ? first : second;
			}

			//
			// Membandingkan dua buah nilai dengan tipe data yang sama
			// 
			// @param first: Nilai pertama
			// @param second: Nilai kedua
			// 
			// @return T: Nilai terkecil setelah dibandingkan
			//
			template<typename T>
			static T Min(T first, T second) {
				return first < second ? first : second;
			}

			//
			// Menulis nilai kedalam console dengan posisi yang spesifik
			// 
			// @param position: Koordinat penulisan nilai
			// @param value: Nilai yang akan ditulis
			//
			template<typename... T>
			static void Write(COORD position, T... value) {
				CursorPosition = position;
				Console::Write(value...);
			}

			//
			// Menulis nilai kedalam console dengan posisi yang spesifik
			// 
			// @param x: Koordinat X penulisan nilai
			// @param y: Koordinat Y penulisan nilai
			// @param value: Nilai yang akan ditulis
			//
			template<typename... T>
			static void Write(Int16 x, Int16 y, T... value) {
				CursorPosition = { x, y };
				Console::Write(value...);
			}

			//
			// Menulis nilai kedalam console dengan warna buffer yang spesifik
			// 
			// @param color: Warna buffer
			// @param value: Nilai yang akan ditulis
			//
			template<typename... T>
			static void Write(ConsoleColor color, T... value) {
				ConsoleColor defaultColor = BufferColor;

				BufferColor = color;
				Console::Write(value...);
				BufferColor = defaultColor;
			}

			//
			// Menulis milai kedalam console dengan warna buffer yang spesifik
			// 
			// @param background: Warna latar belakang buffer
			// @param foreground: Warna latar depan buffer
			// @param value: Nilai yang akan ditulis
			//
			template<typename... T>
			static void Write(Color background, Color foreground, T... value) {
				ConsoleColor defaultColor = BufferColor;

				BufferColor = { background, foreground };
				Console::Write(value...);
				BufferColor = defaultColor;
			}

			//
			// Menulis kedalam console dengan posisi dan warna buffer yang spesifik
			// 
			// @param position: Koordinat penulisan nilai
			// @param color: Warna buffer
			// @param value: Nilai yang akan ditulis
			//
			template<typename... T>
			static void Write(COORD position, ConsoleColor color, T... value) {
				ConsoleColor defaultColor = BufferColor;

				CursorPosition = position;
				BufferColor = color;
				Console::Write(value...);
				BufferColor = defaultColor;
			}

			//
			// Menulis kedalam console dengan posisi dan warna buffer yang spesifik
			// 
			// @param position: Koordinat penulisan nilai
			// @param background: Warna latar belakang buffer
			// @param foreground: Warna latar depan buffer
			// @param value: Nilai yang akan ditulis
			//
			template<typename... T>
			static void Write(COORD position, Color background, Color foreground, T... value) {
				ConsoleColor defaultColor = BufferColor;

				CursorPosition = position;
				BufferColor = { background, foreground };
				Console::Write(value...);
				BufferColor = defaultColor;
			}

			//
			// Menulis kedalam console dengan posisi dan warna buffer yang spesifik
			// 
			// @param x: Koordinat X penulisan nilai
			// @param y: Koordinat Y penulisan nilai
			// @param color: Warna buffer
			// @param value: Nilai yang akan ditulis
			//
			template<typename... T>
			static void Write(Int16 x, Int16 y, ConsoleColor color, T... value) {
				ConsoleColor defaultColor = BufferColor;

				CursorPosition = { x, y };
				BufferColor = color;
				Console::Write(value...);
				BufferColor = defaultColor;
			}

			//
			// Menulis kedalam console dengan posisi dan warna buffer yang spesifik
			// 
			// @param x: Koordinat X penulisan nilai
			// @param y: Koordinat Y penulisan nilai
			// @param background: Warna latar belakang buffer
			// @param foreground: Warna latar depan buffer
			// @param value: Nilai yang akan ditulis
			//
			template<typename... T>
			static void Write(Int16 x, Int16 y, Color background, Color foreground, T... value) {
				ConsoleColor defaultColor = BufferColor;

				CursorPosition = { x, y };
				BufferColor = { background, foreground };
				Console::Write(value...);
				BufferColor = defaultColor;
			}

			//
			// Menulis pesan kedalam console dengan kode pesan yang spesifik
			// 
			// @param type: Tipe pesan yang akan disampaikan
			// @param message: Pesan yang disampaikan
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
			// Menulis pesan kedalam console dengan posisi dan kode pesan yang spesifik
			//
			// @param position: Koordinat penulisan pesan
			// @param type: Tipe pesan yang akan disampaikan
			// @param message: Pesan yang disampaikan
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
				Console::HoldKey();
				Delete(position, _Message[static_cast<int>(type)].length() + message.length() + 1);
			}

			//
			// Menulis pesan kedalam console dengan posisi dan kode pesan yang spesifik
			// 
			// @param x: Koordinat X penulisan pesan
			// @param y: Koordinat Y penulisan pesan
			// @param type: Tipe pesan yang akan disampaikan
			// @param message: Pesan yang disampaikan
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
				Console::HoldKey();
				Delete(x, y, _Message[static_cast<int>(type)].length() + message.length() + 1);
			}

			//
			// Menulis nilai kedalam console dengan posisi yang spesifik, diakhiri dengan baris baru
			// 
			// @param position: Koordinat penulisan nilai
			// @param value: Nilai yang akan ditulis
			//
			template<typename... T>
			static void WriteLine(COORD position, T... value) {
				CursorPosition = position;
				Console::WriteLine(value...);
			}

			//
			// Menulis nilai kedalam console dengan posisi yang spesifik, diakhiri dengan baris baru
			// 
			// @param x: Koordinat X penulisan nilai
			// @param y: Koordinat Y penulisan nilai
			// @param value: Nilai yang akan ditulis
			//
			template<typename... T>
			static void WriteLine(Int16 x, Int16 y, T... value) {
				CursorPosition = { x, y };
				Console::WriteLine(value...);
			}

			//
			// Menulis nilai kedalam console dengan warna buffer yang spesifik, diakhiri dengan baris baru
			// 
			// @param color: Warna buffer
			// @param value: Nilai yang akan ditulis
			//
			template<typename... T>
			static void WriteLine(ConsoleColor color, T... value) {
				ConsoleColor defaultColor = BufferColor;

				BufferColor = color;
				Console::WriteLine(value...);
				BufferColor = defaultColor;
			}

			//
			// Menulis milai kedalam console dengan warna buffer yang spesifik, diakhiri dengan baris baru
			// 
			// @param background: Warna latar belakang buffer
			// @param foreground: Warna latar depan buffer
			// @param value: Nilai yang akan ditulis
			//
			template<typename... T>
			static void WriteLine(Color background, Color foreground, T... value) {
				ConsoleColor defaultColor = BufferColor;

				BufferColor = { background, foreground };
				Console::WriteLine(value...);
				BufferColor = defaultColor;
			}

			//
			// Menulis kedalam console dengan posisi dan warna buffer yang spesifik, diakhiri dengan baris baru
			// 
			// @param position: Koordinat penulisan nilai
			// @param color: Warna buffer
			// @param value: Nilai yang akan ditulis
			//
			template<typename... T>
			static void WriteLine(COORD position, ConsoleColor color, T... value) {
				ConsoleColor defaultColor = BufferColor;

				CursorPosition = position;
				BufferColor = color;
				Console::WriteLine(value...);
				BufferColor = defaultColor;
			}

			//
			// Menulis kedalam console dengan posisi dan warna buffer yang spesifik, diakhiri dengan baris baru
			// 
			// @param position: Koordinat penulisan nilai
			// @param background: Warna latar belakang buffer
			// @param foreground: Warna latar depan buffer
			// @param value: Nilai yang akan ditulis
			//
			template<typename... T>
			static void WriteLine(COORD position, Color background, Color foreground, T... value) {
				ConsoleColor defaultColor = BufferColor;

				CursorPosition = position;
				BufferColor = { background, foreground };
				Console::WriteLine(value...);
				BufferColor = defaultColor;
			}

			//
			// Menulis kedalam console dengan posisi dan warna buffer yang spesifik, diakhiri dengan baris baru
			// 
			// @param x: Koordinat X penulisan nilai
			// @param y: Koordinat Y penulisan nilai
			// @param color: Warna buffer
			// @param value: Nilai yang akan ditulis
			//
			template<typename... T>
			static void WriteLine(Int16 x, Int16 y, ConsoleColor color, T... value) {
				ConsoleColor defaultColor = BufferColor;

				CursorPosition = { x, y };
				BufferColor = color;
				Console::WriteLine(value...);
				BufferColor = defaultColor;
			}

			//
			// Menulis kedalam console dengan posisi dan warna buffer yang spesifik, diakhiri dengan baris baru
			// 
			// @param x: Koordinat X penulisan nilai
			// @param y: Koordinat Y penulisan nilai
			// @param background: Warna latar belakang buffer
			// @param foreground: Warna latar depan buffer
			// @param value: Nilai yang akan ditulis
			//
			template<typename... T>
			static void WriteLine(Int16 x, Int16 y, Color background, Color foreground, T... value) {
				ConsoleColor defaultColor = BufferColor;

				CursorPosition = { x, y };
				BufferColor = { background, foreground };
				Console::WriteLine(value...);
				BufferColor = defaultColor;
			}

			//
			// Menulis pesan kedalam console dengan kode pesan yang spesifik, diakhiri dengan baris baru
			// 
			// @param type: Tipe pesan yang akan disampaikan
			// @param message: Pesan yang disampaikan
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
			// Menulis pesan kedalam console dengan posisi dan kode pesan yang spesifik, diakhiri dengan baris baru
			//
			// @param position: Koordinat penulisan pesan
			// @param type: Tipe pesan yang akan disampaikan
			// @param message: Nilai yang akan ditulis
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
				Console::HoldKey();
				Delete(position, _Message[static_cast<int>(type)].length() + message.length() + 1);
			}

			//
			// Menulis pesan kedalam console dengan posisi dan kode pesan yang spesifik, diakhiri dengan baris baru
			// 
			// @param x: Koordinat X penulisan pesan
			// @param y: Koordinat Y penulisan pesan
			// @param type: Tipe pesan yang akan disampaikan
			// @param message: Nilai yang akan ditulis
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
				Console::HoldKey();
				Delete(x, y, _Message[static_cast<int>(type)].length() + message.length() + 1);
			}
		public:
			//
			// Membaca inputan berupa angka
			// 
			// @return Int32: angka yang diinputkan
			//
			class {
			public:
				operator Int32() const {
					String number;
					char key;

					while ((key = ReadKey) != '\r') {
						switch (key) {
						case '\b':
							if (number.size() > 0) {
								number.pop_back();
								Console::Write("\b \b");
							}
							break;
						default:
							if (isdigit(key)) {
								number += key;
								Console::Write(key);
							}
						}
					}

					return Convert::ToInt32(number);
				}
			} inline static ReadNumber;

			//
			// Mengubah setiap karakter yang diinputkan menjadi asterisk (*)
			// 
			// @return String: Kalimat yang diinputkan
			//
			class {
			public:
				operator String() const {
					String password;
					char key;

					while ((key = ReadKey) != '\r') {
						switch (key) {
						case '\b':
							if (password.size() > 0) {
								password.pop_back();
								Console::Write("\b \b");
							}
							break;
						default:
							password += key;
							Console::Write("*");
						}
					}

					return password;
				}
			} inline static ReadPassword;
		};

		//
		// Membuat menu dengan 'arrow-key' sebagai navigasi pemilihan
		//
		class ConsoleMenu final {
		private:
			System::ConsoleColor _Color;
			COORD _Coordinate;
			Int16 _Limit = 0;

			struct {
				std::vector<String> Back;
				std::vector<String> Front;
			} _Menu;

			struct {
				size_t Begin;
				size_t Current;
				size_t End;
			} _Index;

			struct {
				Int16 Begin;
				Int16 Current;
				Int16 End;
			} _Cursor;

		public:
			//
			// Tipe data untuk menampung hasil pilihan menu
			// Tipe data pertama untuk menampung index-ke dari menu yang dipilih
			// Tipe data kedua untuk menampung nama dari menu yang dipilih
			//
			using Selected = Result<size_t, String>;

			//
			// @param menu: Menu yang akan ditampilkan
			// @param cursorColor: Warna cursor saat pemilihan menu
			// @param position: Posisi menu saat ditampilkan
			// @param fill: [True] Cursor akan disamakan berdasarkan index menu terpanjang
			//
			ConsoleMenu(std::initializer_list<String> menu, System::ConsoleColor cursorColor, COORD position, bool fill = false) : _Menu({ menu, menu }), _Color(cursorColor), _Coordinate(position) {
				if (fill) {
					size_t max = 0;

					for (String index : menu)
						max = Tools::Max(index.length(), max);
					for (String& index : _Menu.Front)
						index += String(max - index.length(), ' ');
				}

				_Index = { 0, 0, menu.size() - 1 };
				_Cursor = { position.Y, position.Y, position.Y + static_cast<Int16>(menu.size()) - 1 };
			}

			//
			// Membersihkan menu dari dalam console
			//
			void Clear() {
				Int16 y = _Coordinate.Y;

				for (size_t i = 0, index = _Index.Begin; i < _Limit; i++, y++, index++)
					Tools::Delete(_Coordinate.X, y, _Menu.Front[index].length());
			}

			//
			// Menampilkan menu
			// 
			// @return Selected: Menu yang dipilih
			//
			Selected Print() {
				COORD buffer = System::Console::BufferSize;

				System::Console::CursorVisible = false;

				if ((_Coordinate.Y + _Menu.Back.size()) < buffer.Y) {
					Int16 y = _Coordinate.Y;
					char key;

					_Limit = static_cast<Int16>(_Menu.Back.size());
					for (size_t i = 0; i <= _Index.End; i++, y++)
						Tools::Write(_Coordinate.X, y, _Menu.Front[i]);

					do {
						Tools::Write(_Coordinate.X, _Cursor.Current, _Color, _Menu.Front[_Index.Current]);
						key = System::Console::ReadKey;
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
					char key;

					_Limit = buffer.Y - _Coordinate.Y;
					_Cursor.End = _Coordinate.Y + _Limit - 1;

					do {
						Int16 y = _Coordinate.Y;
						size_t index = _Index.Begin;

						for (size_t i = 0; i < _Limit; i++, y++, index++)
							Tools::Write(_Coordinate.X, y, _Menu.Front[index]);

						Tools::Write(_Coordinate.X, _Cursor.Current, _Color, _Menu.Front[_Index.Current]);
						key = System::Console::ReadKey;
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
				System::Console::CursorVisible = true;

				return { _Index.Current, _Menu.Back[_Index.Current] };
			}

			//
			// Menampilkan menu dengan membatasi menu yang ditampilkan
			// 
			// @param limit: Batas menu yang ditampilkan
			// 
			// @return Selection: Menu yang dipilih
			//
			Selected Print(Int16 limit) {
				Int16 key;

				_Limit = limit < static_cast<Int16>(_Menu.Back.size()) ? limit : static_cast<Int16>(_Menu.Back.size());
				_Cursor.End = _Coordinate.Y + _Limit - 1;
				System::Console::CursorVisible = false;

				do {
					Int16 y = _Coordinate.Y;
					size_t index = _Index.Begin;

					for (size_t i = 0; i < _Limit; i++, y++, index++)
						Tools::Write(_Coordinate.X, y, _Menu.Front[index]);

					Tools::Write(_Coordinate.X, y, _Color, _Menu.Front[_Index.Current]);
					key = System::Console::ReadKey;
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

				System::Console::CursorVisible = true;

				return{ _Index.Current, _Menu.Back[_Index.Current] };
			}

			//
			// Mendapatkan Ukuran menu
			// 
			// @return size_t: Ukuran menu
			//
			const size_t Size = _Menu.Back.size();
		};

		//
		// Membuat table yang terstuktur
		//
		class ConsoleTable final {
		private:
			using Header = std::vector<String>;
			using Row = std::vector<std::vector<String>>;
			using Width = std::vector<size_t>;
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
				for (size_t i = 0; i < header.size(); i++) {
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
				for (size_t i = 0; i < _Width.size(); i++) {
					for (size_t j = 0; j < (_Width[i] + _Padding + _Padding); j++)
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

			size_t _GetHeight() const {
				return _Row.size() + 5;
			}

			size_t _GetWidth() const {
				size_t size = 2;

				for (size_t length : _Width)
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
			// Untuk mengatur gaya border pada tabel
			//
			enum class Style {
				Basic = 0,
				Line = 1,
				DoubleLine = 2,
				Invisible = 3
			};

			//
			// Menginisialisasi tabel
			// 
			// @param header: Header dari tabel
			// @param padding: Jarak antara border ke cell (default 1)
			//
			ConsoleTable(std::initializer_list<String> header, UInt16 padding = 1) : _Header(header), _Padding(padding) {
				for (const String& index : header)
					_Width.push_back(index.length());
			}

			//
			// Menambahkan row kedalam tabel
			// 
			// @param row: Row yang akan ditambahkan
			//
			void Add(std::initializer_list<String> row) {
				if (row.size() > _Width.size())
					Error("Ukuran row harus sama dengan header.");

				auto res = std::vector<String>{ row };

				_Row.push_back(res);
				for (size_t i = 0; i < res.size(); i++)
					_Width[i] = Tools::Max(res[i].size(), _Width[i]);
			}

			//
			// Menampilkan tabel dengan mengatur buffer secara otomatis mengikuti ukuran tabel
			//
			void Print() {
				COORD buffer = System::Console::BufferSize;
				COORD cursor = System::Console::CursorPosition;

				size_t width = _GetWidth();
				size_t height = _GetHeight();

				System::Console::BufferSize = {
					static_cast<Int16>(width) > buffer.X ? static_cast<Int16>(width) : buffer.X,
					static_cast<Int16>(height) > buffer.Y ? static_cast<Int16>(height) : buffer.Y
				};
				System::Console::CursorPosition = { 0, cursor.Y };
				System::Console::Write(*this);
				System::Console::HoldKey();
				System::Console::Clear();
				System::Console::BufferSize = buffer;
			}

			//
			// Menghapus row dari tabel berdasarkan index row
			// 
			// @param index: Index row yang akan dihapus
			//
			void Remove(size_t index) {
				if (index > _Row.size())
					Error("Index row diluar batas.");

				_Row.erase(_Row.begin() + index);
			}

			//
			// Mengatur gaya border
			// 
			// @param style: Gaya border yang akan digunakan
			//
			void BorderStyle(Style style) {
				switch (style) {
				case Style::Basic:_Border = _Basic; break;
				case Style::Line:_Border = _Line; break;
				case Style::DoubleLine:_Border = _DoubleLine; break;
				case Style::Invisible:_Border = _Invisible; break;
				}
			}

			//
			// Memfilter row dari A ke Z
			// @param ascending: [True] row akan diurutkan dari A ke Z, sebaliknya [False]
			//
			void Sort(bool ascending) {
				if (ascending)
					std::sort(_Row.begin(), _Row.end(), std::less<std::vector<String>>());
				else
					std::sort(_Row.begin(), _Row.end(), std::greater<std::vector<String>>());
			}

			//
			// Mengupdate header berdasarkan index
			// 
			// @param index: Index header yang akan diupdate
			// @param newHeader: Header baru
			//
			void UpdateHeader(size_t index, String newHeader) {
				if (index > _Header.size())
					Error("Index header diluar batas");

				_Header[index] = newHeader;
				_Width[index] = newHeader.size() > _Width[index] ? newHeader.size() : _Width[index];
			}

			//
			// Mengupdate row berdasarkan index
			// 
			// @param rowIndex: Index dari row yang akan diupdate
			// @param headerIndex: Index dari header yang akan diupdate
			// @param newRow: Row baru
			//
			void UpdateRow(size_t rowIndex, size_t headerIndex, String newRow) {
				if (rowIndex > _Row.size())
					Error("Index row diluar batas.");
				else if (headerIndex > _Header.size())
					Error("Index header diluar batas.");

				_Row[rowIndex][headerIndex] = newRow;
				_Width[headerIndex] = newRow.size() > _Width[headerIndex] ? newRow.size() : _Width[headerIndex];
			}
		};
	}
}