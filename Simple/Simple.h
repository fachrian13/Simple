#ifndef _SIMPLE_
#define _SIMPLE_

#include <filesystem>
#include <fstream>
#include <vector>
#include <windows.h>
#include <map>
#include <iostream>
#include <conio.h>
#include <string>
#include <thread>

namespace Simple
{
	namespace System
	{
		/// <summary>
		/// Warna dasar pada console.
		/// </summary>
		enum class Color
		{
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

		/// <summary>
		/// Struktur warna pada console.
		/// </summary>
		struct ConsoleColor
		{
			Color Background;
			Color Foreground;
		};

		/// <summary>
		/// 
		/// </summary>
		/// <typeparam name="T"></typeparam>
		template<class T>
		class ReadOnlyProperty
		{
		protected:
			T Value;

		public:
			virtual operator T() const { return this->Value; }
		};

		/// <summary>
		/// Kelas untuk menangani error pada program.
		/// (Tidak termasuk error yang diberikan dari std::exception)
		/// </summary>
		class Exception
		{
		public:
			class : public ReadOnlyProperty<int> { friend class Exception; } Line;
			class : public ReadOnlyProperty<const char*> { friend class Exception; } File;
			class : public ReadOnlyProperty<const char*> { friend class Exception; } Function;
			class : public ReadOnlyProperty<const char*> { friend class Exception; } Description;

			Exception(const char* file, int line, const char* function, const char* description)
			{
				this->Line.Value = line;
				this->File.Value = file;
				this->Function.Value = function;
				this->Description.Value = description;
			}
		};

/// <summary>
/// Untuk melempar Exception, otomatis mengisi parameter sebagai berikut
/// throw Exception(__FILE__, __LINE__, __FUNCTION__, v)
/// </summary>
#define THROW(v) throw Exception(__FILE__, __LINE__, __FUNCTION__, v)

		/// <summary>
		/// CRUD file dengan menggunakan mode binary.
		/// </summary>
		/// <typeparam name="T"></typeparam>
		template<class T>
		class BinaryFile
		{
		protected:
			std::filesystem::path FileName;
			std::filesystem::path FileTemp = "temp.dat";

			std::fstream File;
			std::fstream Temp;

		public:
			/// <summary>
			/// Inisialisasi kelas BinaryFile.
			/// </summary>
			/// <param name="fileName">Nama file yang akan dibuat.</param>
			BinaryFile(std::filesystem::path fileName) :
				FileName(fileName.empty() ? THROW("Nama file tidak boleh kosong") : fileName)
			{
				if (!this->FileName.has_extension) this->FileName += ".bin";
				else if (this->FileName.extension() != ".bin") this->FileName.replace_extension(".bin");

				this->File.open(this->FileName, std::ios::out | std::ios::binary | std::ios::app);
				if (!this->File) THROW("Gagal membuka file.");
				this->File.close();
			}

			/// <summary>
			/// Menghapus data dari file berdasarkan index data. Terdapat 2 metode penghapusan data.
			/// 
			/// 'true'
			/// Seluruh data akan ditampung kedalam vector, lalu index data akan dihapus. Kemudian vector ditulis
			/// kembali kedalam file secara berurutan.
			/// 
			/// 'false'
			/// Membuka dua buah file, file pertama sebagai source dan kedua sebagai temp. Data akan dibaca
			/// lalu ditulis langsung kedalam file temp, jika index sama maka akan dilewat.
			/// </summary>
			/// <param name="index">Index data yang akan dihapus</param>
			/// <param name="useVector">Metode penghapusan data. (Default 'true')</param>
			void Delete(size_t index, bool useVector = true)
			{
				if (useVector)
				{
					std::vector<T> stored = Read();
					stored.erase(stored.begin() + index);

					this->File.open(this->FileName, std::ios::out | std::ios::binary | std::ios::trunc);
					if (!this->File) THROW("Gagal membuka file.");

					for (T i : stored) this->File.write((char*)&i, sizeof T);
					this->File.close();
				}
				else
				{
					T temp;

					this->File.open(this->FileName, std::ios::in | std::ios::binary);
					if (!this->File) THROW("Gagal membuka file.");

					this->Temp.open(this->FileTemp, std::ios::out | std::ios::binary);
					if (!this->Temp) THROW("Gagal membuka file temp.");

					for (size_t i = 0; this->File.read((char*)&temp, sizeof T); i++)
						if (i != index) this->Temp.write((char*)&temp, sizeof T);

					this->File.close();
					this->Temp.close();

					remove(this->FileName);
					rename(this->FileTemp, this->FileName);
				}
			}

			/// <summary>
			/// Mengecek apakah file kosong atau tidak.
			/// </summary>
			/// <returns>'true' jika file kosong, sebaliknya 'false'.</returns>
			bool Empty()
			{
				this->File.open(FileName, std::ios::in | std::ios::binary);
				if (!this->File) THROW("Gagal membuka file.");
				bool empty = this->File.peek() == std::fstream::traits_type::eof();
				this->File.close();

				return empty;
			}

			/// <summary>
			/// Membaca seluruh data dari dalam file.
			/// </summary>
			/// <returns>Seluruh data yang telah dibaca.</returns>
			std::vector<T> Read()
			{
				T temp;
				std::vector<T> stored;

				this->File.open(this->FileName, std::ios::in | std::ios::binary);
				if (!this->File) THROW("Gagal membuka file.");

				while (this->File.read((char*)&temp, sizeof T)) stored.push_back(temp);
				this->File.close();

				return stored;
			}

			/// <summary>
			/// Memperbaru data didalam file berdasarkan index data.
			/// </summary>
			/// <param name="index">Index data yang akan diperbarui.</param>
			/// <param name="value">Data baru yang akan ditulis.</param>
			void Update(size_t index, T value)
			{
				this->File.open(this->FileName, std::ios::in | std::ios::out | std::ios::binary);
				if (!this->File) THROW("Gagal membuka file.");

				this->File.seekp(index * sizeof T, std::ios::beg);
				this->File.write((char*)&value, sizeof T);
				this->File.close();
			}

			/// <summary>
			/// Menulis data kedalam file.
			/// </summary>
			/// <param name="value">Data yang akan ditulis.</param>
			void Write(T value)
			{
				this->File.open(this->FileName, std::ios::out | std::ios::binary | std::ios::app);
				if (!this->File) THROW("Gagal membuka file.");

				this->File.write((char*)&value, sizeof T);
				this->File.close();
			}

			/// <summary>
			/// Menulis data kedalam file.
			/// </summary>
			/// <param name="value">Kumpulan data yang akan ditulis.</param>
			void Write(std::vector<T> value)
			{
				this->File.open(this->FileName, std::ios::out | std::ios::binary | std::ios::app);
				if (!this->File) THROW("Gagal membuka file.");

				for (T i : value) this->File.write((char*)&i, sizeof T);
				this->File.close();
			}
		};

		/// <summary>
		/// Kelas yang berisikan method-method yang bisa digunakan untuk operasi didalam console.
		/// </summary>
		class Console
		{
		protected:
			static CONSOLE_SCREEN_BUFFER_INFO BufferInfo;
			static HANDLE OutputHandle;
			static std::map<int, Color> Color;

		public:
			/// <summary>
			/// Membersihkan buffer.
			/// </summary>
			static void Clear()
			{
				system("cls");
			}

			/// <summary>
			/// Mendapatkan ukuran buffer.
			/// </summary>
			/// <returns>Ukuran buffer.</returns>
			static COORD GetBufferSize()
			{
				if (!GetConsoleScreenBufferInfo(OutputHandle, &BufferInfo)) THROW("Gagal mendapatkan informasi buffer.");

				return BufferInfo.dwSize;
			}

			/// <summary>
			/// Mendapatkan warna buffer.
			/// </summary>
			/// <returns>Warna buffer saat ini.</returns>
			static ConsoleColor GetColor()
			{
				if (!GetConsoleScreenBufferInfo(OutputHandle, &BufferInfo)) THROW("Gagal mendapatkan informasi buffer.");

				return
				{
					Color.find(BufferInfo.wAttributes / 16)->second,
					Color.find(BufferInfo.wAttributes % 16)->second
				};
			}

			/// <summary>
			/// Mendapatkan posisi cursor.
			/// </summary>
			/// <returns>Posisi cursor saat ini.</returns>
			static COORD GetCursorPosition()
			{
				if (!GetConsoleScreenBufferInfo(OutputHandle, &BufferInfo)) THROW("Gagal mendapatkan informasi buffer.");

				return BufferInfo.dwCursorPosition;
			}

			/// <summary>
			/// Mengembalikan karakter yang diinput dengan menekan enter.
			/// </summary>
			/// <returns>Karakter yang diinputkan.</returns>
			static int Read()
			{
				return std::cin.get();
			}

			/// <summary>
			/// Mengembalikan karakter yang diinput tanpa menekan enter.
			/// </summary>
			/// <returns>Karakter yang diinputkan.</returns>
			static int ReadKey()
			{
				return _getch();
			}

			/// <summary>
			/// Mengembalikan kalimat yang diinputkan.
			/// </summary>
			/// <returns>Kalimat yang diinputkan.</returns>
			static std::string ReadLine()
			{
				std::string line;

				std::getline(std::cin, line);
				return line;
			}

			/// <summary>
			/// Mengatur ukuran buffer.
			/// </summary>
			/// <param name="size">Ukuran buffer.</param>
			static void SetBufferSize(COORD size)
			{
				if (!SetConsoleScreenBufferSize(OutputHandle, size)) THROW("Gagal mengatur ukuran buffer.");
			}

			/// <summary>
			/// Mengatur warna buffer.
			/// </summary>
			/// <param name="color">Warna buffer.</param>
			static void SetColor(ConsoleColor color)
			{
				if (!SetConsoleTextAttribute(OutputHandle, static_cast<int>(color.Background)<< 4 | static_cast<int>(color.Foreground))) THROW("Gagal mengatur warna buffer.");
			}

			/// <summary>
			/// Mengatur posisi cursor.
			/// </summary>
			/// <param name="position">Posisi cursor.</param>
			static void SetCursorPosition(COORD position)
			{
				if (!SetConsoleCursorPosition(OutputHandle, position)) THROW("Gagal mengatur posisi cursor.");
			}

			/// <summary>
			/// Menjeda thread.
			/// </summary>
			/// <param name="milliseconds">Waktu jeda dalam milliseconds.</param>
			static void Sleep(unsigned long long milliseconds)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
			}

			/// <summary>
			/// Menulis nilai kedalam console.
			/// </summary>
			/// <typeparam name="...T">Tipe nilai.</typeparam>
			/// <param name="...value">Nilai yang akan ditulis.</param>
			template<class... T>
			static void Write(T... value)
			{
				((std::cout << value), ...);
			}

			/// <summary>
			/// Menulis nilai kedalam console diakhiri dengan baris baru.
			/// </summary>
			/// <typeparam name="...T">Tipe nilai.</typeparam>
			/// <param name="...value">Nilai yang akan ditulis.</param>
			template<class... T>
			static void WriteLine(T... value)
			{
				((std::cout << value), ...) << "\n";
			}
		};

		CONSOLE_SCREEN_BUFFER_INFO Console::BufferInfo = {};
		HANDLE Console::OutputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		std::map<int, Color> Console::Color =
		{
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
	}

	namespace Utility
	{
		class Convert
		{
		public:
			/// <summary>
			/// Mengkonversi warna menjadi string.
			/// </summary>
			/// <param name="value">Warna yang akan dikonversi.</param>
			/// <returns>Warna yang telah di konversi menjadi string.</returns>
			static std::string ToString(System::Color value)
			{
				switch (value) {
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

			/// <summary>
			/// Mengkonversi string menjadi lowercase.
			/// </summary>
			/// <param name="value">String yang akan dikonversi.</param>
			/// <returns>String yang telah dikonversi menjadi lowercase.</returns>
			static std::string ToLower(std::string value)
			{
				std::transform(value.begin(), value.end(), value.begin(), ::tolower);

				return value;
			}

			/// <summary>
			/// Mengkonversi string menjadi uppercase.
			/// </summary>
			/// <param name="value">String yang akan dikonversi.</param>
			/// <returns>String yang telah dikonversi menjadi uppercase.</returns>
			static std::string ToUpper(std::string value)
			{
				std::transform(value.begin(), value.end(), value.begin(), ::toupper);

				return value;
			}
		};

		/// <summary>
		/// Kelas yang berisi method-method pembantu
		/// </summary>
		class Tools final : private System::Console
		{
		private:
			using Exception = System::Exception;

		public:
			/// <summary>
			/// Mengosongkan string dan menghapus karakter yang telah tercetak pada buffer.
			/// </summary>
			/// <param name="position">Posisi karakter yang akan dihapus.</param>
			/// <param name="value">String yang akan dikosongkan.</param>
			static void Clear(COORD position, std::string& value)
			{
				if (!value.empty())
				{
					DeleteText(position, value.size());
					value.clear();
				}
			}

			/// <summary>
			/// Menghapus karakter yang telah tercetak pada console.
			/// </summary>
			/// <param name="position">Posisi karakter yang akan dihapus.</param>
			/// <param name="length">Panjang karakter.</param>
			static void DeleteText(COORD position, size_t length)
			{
				DWORD ch;

				if (!GetConsoleScreenBufferInfo(OutputHandle, &BufferInfo)) THROW("Gagal mendapatkan informasi buffer.");
				if (!FillConsoleOutputCharacter(OutputHandle, ' ', length, position, &ch)) THROW("Gagal menghapus text.");
				if (!GetConsoleScreenBufferInfo(OutputHandle, &BufferInfo)) THROW("Gagal mendapatkan informasi buffer.");
				if (!FillConsoleOutputAttribute(OutputHandle, BufferInfo.wAttributes, length, position, &ch)) THROW("Gagal mengisi attribute.");
			}

			/// <summary>
			/// Mengembalikan kalimat yang diinputkan, dan menyembunyikan karakter saat penginputan.
			/// </summary>
			/// <returns>Kalimat yang diinputkan.</returns>
			static std::string ReadPassword()
			{
				char ch;
				std::string password;

				do
				{
					ch = ReadKey();

					switch (ch)
					{
					case 0:
					case 224:
						ReadKey();
						break;
					case '\b':
						if (!password.empty())
						{
							password.pop_back();
							Write("\b \b");
						}
						break;
					case 13: break;
					default:
						password += ch;
						Write("*");
					}
				} while (ch != '\r');
				return password;
			}
			
			/// <summary>
			/// Mengembalikan kalimat yang diinputkan, dengan membatasi dan menyembunyikan karakter saat penginputan.
			/// </summary>
			/// <param name="limit">Batas karakter yang diinputkan</param>
			/// <returns>Kalimat yang diinputkan</returns>
			static std::string ReadPassword(int limit)
			{
				char ch;
				std::string password;

				do
				{
					ch = ReadKey();

					switch (ch)
					{
					case 0:
					case 224:
						ReadKey();
						break;
					case '\b':
						if (!password.empty())
						{
							password.pop_back();
							Write("\b \b");
						}
						break;
					case 13: break;
					default:
						if (password.size() < limit)
						{
							password += ch;
							Write("*");
						}
					}
				} while (ch != '\r');
				return password;
			}

			/// <summary>
			/// Mengecek apakah seluruh string hanya nomor atau tidak.
			/// </summary>
			/// <param name="value">String yang akan dicek.</param>
			/// <returns>'true' apabila string hanya nomor, sebaliknya 'false'.</returns>
			static bool IsNumber(std::string& value)
			{
				return !value.empty() && std::find_if(value.begin(), value.end(), [](char ch) { return !std::isdigit(ch); }) == value.end();
			}

			/// <summary>
			/// Menulis nilai kedalam console dan mengubah warna background dan foreground.
			/// </summary>
			/// <typeparam name="...T">Tipe nilai.</typeparam>
			/// <param name="color">Warna yang akan digunakan.</param>
			/// <param name="...value">Nilai yang akan ditulis.</param>
			template<class... T>
			static void WriteColor(System::ConsoleColor color, T... value)
			{
				ConsoleColor prevColor = GetColor();

				SetColor(color);
				Write(value...);
				SetColor(prevColor);
			}

			/// <summary>
			/// Menulis nilai kedalam console dan mengubah warna background dan foreground, diakhiri dengan baris baru.
			/// </summary>
			/// <typeparam name="...T">Tipe nilai.</typeparam>
			/// <param name="color">Warna yang akan digunakan.</param>
			/// <param name="...value">Nilai yang akan ditulis.</param>
			template<class... T>
			static void WriteLineColor(System::ConsoleColor color, T... value)
			{
				ConsoleColor prevColor = GetColor;

				SetColor(color);
				Write(value...);
				SetColor(prevColor);
			}
		};
	}
}

#endif