#pragma once

#include <conio.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <vector>
#include <windows.h>

namespace Simple
{
	namespace System
	{
		/// <summary>
		/// Kelas Property yang bisa diatur dan diambil datanya.
		/// </summary>
		/// <typeparam name="T">Type property.</typeparam>
		template<class T>
		class Property
		{
		protected:
			T Value;

		public:
			virtual ~Property() {}
			virtual T& operator=(const T& value) { return this->Value = value; }
			virtual operator T () const { return this->Value; }
		};

		/// <summary>
		/// Kelas property hanya bisa diambil datanya.
		/// </summary>
		/// <typeparam name="T">Type property.</typeparam>
		template<class T>
		class WriteOnlyProperty
		{
		protected:
			T Value;

		public:
			virtual ~WriteOnlyProperty() {}
			virtual T& operator=(const T& value) { return this->Value = value; }
		};

		/// <summary>
		/// Kelas property yang hanya bisa diatur datanya.
		/// </summary>
		/// <typeparam name="T">Type Property.</typeparam>
		template<class T>
		class ReadOnlyProperty
		{
		protected:
			T Value;

		public:
			virtual ~ReadOnlyProperty() {}
			virtual operator T () const { return this->Value; }
		};		
#define PROPERTYSET(T) T& operator=(const T& value) override
#define PROPERTYGET(T) operator T () const override
#define OUTSTREAM(T) friend std::ostream& operator<<(std::ostream& out, const T& value)

		/// <summary>
		/// Warna dasar pada console.
		/// </summary>
		enum class Color
		{
			Black = 30,
			Red = 31,
			Green = 32,
			Yellow = 33,
			Blue = 34,
			Magenta = 35,
			Cyan = 36,
			White = 37,
			Default = 39,
			Gray = 90,
			BrightRed = 91,
			BrightGreen = 92,
			BrightYellow = 93,
			BrightBlue = 94,
			BrightMagenta = 95,
			BrightCyan = 96,
			BrightWhite = 97
		};

		/// <summary>
		/// Struktur warna dasar pada console.
		/// </summary>
		struct ConsoleColor
		{
			Color Background;
			Color Foreground;

			friend std::ostream& operator<<(std::ostream& out, const ConsoleColor& value)
			{
				out << "\033[" << static_cast<int>(value.Foreground) + 0 << ";" << static_cast<int>(value.Background) + 10 << "m";
				return out;
			}

			friend bool operator==(const ConsoleColor& first, const ConsoleColor& second) { return first.Background == second.Background && first.Foreground == second.Foreground; }

			friend bool operator!=(const ConsoleColor& first, const ConsoleColor& second) { return !(first == second); }

			operator std::string() const
			{
				return "\033[" + std::to_string(static_cast<int>(this->Foreground) + 0) + ";" + std::to_string(static_cast<int>(this->Background) + 10) + "m";
			}

			std::string ToString() const
			{
				return "\033[" + std::to_string(static_cast<int>(this->Foreground) + 0) + ";" + std::to_string(static_cast<int>(this->Background) + 10) + "m";
			}
		};

		/// <summary>
		/// Struktur dasar untuk koordinat cursor.
		/// </summary>
		struct Coordinate
		{
			int X;
			int Y;

			friend std::ostream& operator<<(std::ostream& out, const Coordinate& value)
			{
				out << "\033[" << value.Y << ";" << value.X << "H";
				return out;
			}

			friend bool operator==(const Coordinate& first, const Coordinate& second) { return first.X == second.X && first.Y == second.Y; }

			friend bool operator!=(const Coordinate& first, const Coordinate& second) { return !(first == second); }

			operator std::string() const
			{
				return "\033[" + std::to_string(static_cast<int>(this->Y)) + ";" + std::to_string(static_cast<int>(this->X)) + "H";
			}

			std::string ToString() const
			{
				return "\033[" + std::to_string(static_cast<int>(this->Y)) + ";" + std::to_string(static_cast<int>(this->X)) + "H";
			}
		};

		/// <summary>
		/// Kelas area standar.
		/// </summary>
		struct Rectangle
		{
			int Top;
			int Bottom;
			int Left;
			int Right;
		};

		/// <summary>
		/// Kelas untuk menangani error pada program (tidak termasuk error yang dilemparkan dari std::exception).
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
#define THROW(v) throw Exception(__FILE__, __LINE__, __FUNCTION__, v)

		/// <summary>
		/// Kelas didalamnya terdapat method-method yang bisa digunakan untuk operasi didalam console.
		/// </summary>
		class Console final
		{
		public:
			/// <summary>
			/// Mengatur blinking pada cursor. (true = blinking, false = statis).
			/// </summary>
			inline static class : public WriteOnlyProperty<bool>
			{
			public: PROPERTYSET(bool)
				{
					if (value)
						Write("\033[?12h");
					else
						Write("\033[?12l");
					return this->Value = value;
				}
			} CursorBlinking;

			/// <summary>
			/// Mengatur eksistensi cursor pada console (true = terlihat, false = tidak terlihat).
			/// </summary>
			inline static class : public WriteOnlyProperty<bool>
			{
			public: PROPERTYSET(bool)
				{
					if (value)
						Write("\033[?25h");
					else
						Write("\033[?25l");
					return this->Value = value;
				}
			} CursorVisible;

		public:
			/// <summary>
			/// Membersihkan buffer.
			/// </summary>
			static void Clear()
			{
				system("cls");
			}

			/// <summary>
			/// Menonaktifkan tombol maximize.
			/// </summary>
			static void DisableMaximizeButton()
			{
				SetWindowLong(GetConsoleWindow(), GWL_STYLE, GetWindowLong(GetConsoleWindow(), GWL_STYLE) & ~WS_MAXIMIZEBOX);
			}

			/// <summary>
			/// Menonaktifkan tombol Minimize.
			/// </summary>
			static void DisableMinimizeButton()
			{
				SetWindowLong(GetConsoleWindow(), GWL_STYLE, GetWindowLong(GetConsoleWindow(), GWL_STYLE) & ~WS_MINIMIZEBOX);
			}

			/// <summary>
			/// Menonaktifkan Perubahan ukuran console.
			/// </summary>

			static void DisableResizeWindow()
			{
				SetWindowLong(GetConsoleWindow(), GWL_STYLE, GetWindowLong(GetConsoleWindow(), GWL_STYLE) & ~WS_SIZEBOX);
			}

			/// <summary>
			/// Mengaktifkan mode virtual pada console.
			/// </summary>
			/// <returns>'true' jika berhasil, sebaliknya 'false'.</returns>
			static bool EnableVirtualTerminal()
			{
				HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
				if (hOut == INVALID_HANDLE_VALUE)
					return false;

				DWORD dwMode = 0;
				if (!GetConsoleMode(hOut, &dwMode))
					return false;

				dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
				if (!SetConsoleMode(hOut, dwMode))
					return false;
				return true;
			}

			/// <summary>
			/// Mengambil ukuran buffer pada console.
			/// </summary>
			/// <returns>Ukuran buffer.</returns>
			static Coordinate GetBufferSize()
			{
				HANDLE outputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
				CONSOLE_SCREEN_BUFFER_INFO csbi;
				GetConsoleScreenBufferInfo(outputHandle, &csbi);
				return
				{
					csbi.srWindow.Right - csbi.srWindow.Left + 1,
					csbi.srWindow.Bottom - csbi.srWindow.Top + 1
				};
			}

			/// <summary>
			/// Menulis nilai kedalam console.
			/// </summary>
			/// <typeparam name="...T">Type nilai.</typeparam>
			/// <param name="...value">Nilai yang akan ditulis.</param>
			template<class... T>
			static void Write(T... value)
			{
				((std::cout << value), ...);
			}

			/// <summary>
			/// Menulis nilai kedalam console, diakhiri dengan baris baru.
			/// </summary>
			/// <typeparam name="...T">Type nilai.</typeparam>
			/// <param name="...value">Nilai yang akan ditulis.</param>
			template<class... T>
			static void WriteLine(T... value)
			{
				Write(value..., "\n");
			}

			/// <summary>
			/// Mengembalikan karakter yang diinput dengan menekan enter.
			/// </summary>
			/// <returns>Karakter yang diinputkan.</returns>
			static char Read()
			{
				char ch;

				ch = std::cin.get();
				return ch;
			}

			/// <summary>
			/// Mengembalikan karakter yang diinput tanpa menekan enter.
			/// </summary>
			/// <returns>Karakter yang diinputkan.</returns>
			static char ReadKey()
			{
				char ch;

				ch = _getch();
				return ch;
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
			/// Mengatur ukuran window.
			/// </summary>
			/// <param name="width">Lebar console (dalam karakter).</param>
			/// <param name="height">Tinggi console (dalam karakter).</param>
			static void SetWindowSize(short width, short height)
			{
				HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
				if (hOut == INVALID_HANDLE_VALUE)
					THROW("Unable to get stdout handle");

				/* Getting possible console size */
				{
					COORD cCoord = GetLargestConsoleWindowSize(hOut);
					if (width > cCoord.X)
						THROW("The width dimensions is too large");
					if (height > cCoord.Y)
						THROW("The height dimensions is too large");
				}

				CONSOLE_SCREEN_BUFFER_INFO csbInfo;
				if (!GetConsoleScreenBufferInfo(hOut, &csbInfo))
					THROW("Unable to retrieve screen buffer info");

				SMALL_RECT& wInfo = csbInfo.srWindow;
				COORD wSize{ wInfo.Right - wInfo.Left + 1, wInfo.Bottom - wInfo.Top + 1 };

				if (wSize.X > width || wSize.Y > height)
				{
					SMALL_RECT info{ 0, 0, width < wSize.X ? width - 1 : wSize.X - 1, height < wSize.Y ? height - 1 : wSize.Y - 1 };

					if (!SetConsoleWindowInfo(hOut, true, &info))
						THROW("Unable to resize window before resizing buffer");
				}

				COORD size{ width, height };
				if (!SetConsoleScreenBufferSize(hOut, size))
					THROW("Unable to resize screen buffer");

				SMALL_RECT info{ 0, 0, width - 1, height - 1 };
				if (!SetConsoleWindowInfo(hOut, true, &info))
					THROW("Unable to resize window after resizing buffer");
			}
		};

		/// <summary>
		/// Input output file dengan menggunakan metode binary.
		/// </summary>
		/// <typeparam name="T">Type file.</typeparam>
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
			BinaryFile(std::filesystem::path fileName) : FileName(fileName.empty() ? THROW("Nama file tidak boleh kosong") : fileName)
			{
				if (!this->FileName.has_extension()) this->FileName += ".bin";
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

			~BinaryFile() {}
		};
	}

	namespace Utility
	{
		/// <summary>
		/// Kelas didalamnya terdapat method-method pembantu untuk operasi didalam console.
		/// </summary>
		class Tools final
		{
		public:
			/// <summary>
			/// Membersihkan string dan menghapus nilai yang tercetak pada console.
			/// </summary>
			/// <param name="variable">String yang akan dibersihkan.</param>
			static void Clear(std::string& variable)
			{
				if (!variable.empty())
				{
					EraseCharacter(static_cast<int>(variable.size()));
					variable.clear();
				}
			}

			/// <summary>
			/// Menghapus nilai yang telah tercetak pada console.
			/// </summary>
			/// <param name="length">Panjang nilai.</param>
			/// <param name="method">Metode penghapusan berdasarkan Virtual Terminal.</param>
			static void EraseCharacter(int length, char method = 'X')
			{
				System::Console::Write("\033[", length, method);
			}

			/// <summary>
			/// Menghapus nilai yang telah tercetak pada console, dengan koordinat spesifik.
			/// </summary>
			/// <param name="position">Posisi nilai yang akan dihapus.</param>
			/// <param name="length">Panjang nilai.</param>
			/// <param name="method">Metode penghapusan berdasarkan Virtual Terminal.</param>
			static void EraseCharacter(System::Coordinate position, int length, char method = 'X')
			{
				System::Console::Write(position, "\033[", length, method);
			}

			/// <summary>
			/// Menghapus nilai yang telah tercetak pada console, dengan area yang spesifik.
			/// </summary>
			/// <param name="area">Area yang akan dihapus.</param>
			/// <param name="length">Panjang nilai terpanjang dalam area tersebut.</param>
			static void EraseCharacter(System::Rectangle area)
			{
				for (int i = area.Top; i <= area.Bottom; i++)
					EraseCharacter(System::Coordinate{ area.Left, i }, area.Right);
			}

			/// <summary>
			/// Mengecek apakah seluruh string berupa nomor atau tidak.
			/// </summary>
			/// <param name="value">String yang akan dicek.</param>
			/// <returns>'true' jika seluruh string berupa nomor, sebaliknya 'false'.</returns>
			static bool IsNumber(std::string& value)
			{
				return !value.empty() && std::find_if(value.begin(), value.end(), isdigit) == value.end();
			}

			/// <summary>
			/// Mengecek apakah seluruh string berupa huruf atau tidak.
			/// </summary>
			/// <param name="value">String yang akan dicek.</param>
			/// <returns>'true' jika seluruh string berupa huruf, sebaliknya 'false'.</returns>
			static bool IsAlpha(std::string& value)
			{
				return !value.empty() && std::find_if(value.begin(), value.end(), isalpha) == value.end();
			}

			/// <summary>
			/// Mengembalikan kalimat yang diinputkan dengan membatasinya.
			/// </summary>
			/// <param name="limit">Batas karakter yang dapat diinputkan.</param>
			/// <returns>Kalimat yang diinputkan.</returns>
			static std::string ReadLine(unsigned int limit)
			{
				char ch;
				std::string line;

				do
				{
					ch = System::Console::ReadKey();

					switch (ch)
					{
					case 0:
					case 224:
						System::Console::ReadKey();
						break;
					case '\b':
						if (!line.empty())
						{
							line.pop_back();
							System::Console::Write("\b \b");
						}
						break;
					case 13: break;
					default:
						if (line.size() < limit)
						{
							if (ch >= ' ' && ch <= '~')
							{
								line += ch;
								System::Console::Write(ch);
							}
						}
					}
				} while (ch != '\r');
				return line;
			}

			/// <summary>
			/// Mengembalikan nilai yang diinputkan.
			/// </summary>
			/// <returns>Kalimat yang diinputkan berupa angka.</returns>
			static std::string ReadNumber()
			{
				char ch;
				std::string number;

				do
				{
					ch = System::Console::ReadKey();

					switch (ch)
					{
					case 0:
					case 224:
						System::Console::ReadKey();
						break;
					case '\b':
						if (!number.empty())
						{
							number.pop_back();
							System::Console::Write("\b \b");
						}
						break;
					case 13: break;
					default:
						if (ch >= '0' && ch <= '9')
						{
							number += ch;
							System::Console::Write(ch);
						}
					}
				} while (ch != '\r');
				return number;
			}

			/// <summary>
			/// Mengembalikan nilai yang diinputkan dengan membatasinya.
			/// </summary>
			/// <param name="limit">Batas nilai yang bisa diinputkan.</param>
			/// <returns>Kalimat yang diinputkan berupa angka.</returns>
			static std::string ReadNumber(unsigned int limit)
			{
				char ch;
				std::string number;

				do
				{
					ch = System::Console::ReadKey();

					switch (ch)
					{
					case 0:
					case 224:
						System::Console::ReadKey();
						break;
					case '\b':
						if (!number.empty())
						{
							number.pop_back();
							System::Console::Write("\b \b");
						}
						break;
					case 13: break;
					default:
						if (number.size() < limit)
						{
							if (ch >= '0' && ch <= '9')
							{
								number += ch;
								System::Console::Write(ch);
							}
						}
					}
				} while (ch != '\r');
				return number;
			}

			/// <summary>
			/// Mengembalikan kalimat yang diinputkan, dan mengganti setiap karakter yang diinputkan dengan asterisk (*).
			/// </summary>
			/// <returns>Kalimat yang diinputkan.</returns>
			static std::string ReadPassword()
			{
				char ch;
				std::string password;

				do
				{
					ch = System::Console::ReadKey();

					switch (ch)
					{
					case 0:
					case 224:
						System::Console::ReadKey();
						break;
					case '\b':
						if (!password.empty())
						{
							password.pop_back();
							System::Console::Write("\b \b");
						}
						break;
					case 13: break;
					default:
						if (ch >= ' ' && ch <= '~')
						{
							password += ch;
							System::Console::Write("*");
						}
					}
				} while (ch != '\r');
				return password;
			}

			/// <summary>
			/// Mengembalikan kalimat yang diinputkan, dan mengganti setiap karakter yang diinputkan dengan asterisk (*) serta membatasinya.
			/// </summary>
			/// <param name="limit">Batas karakter yang dapat diinputkan.</param>
			/// <returns>Karakter yang diinputkan.</returns>
			static std::string ReadPassword(unsigned int limit)
			{
				char ch;
				std::string password;

				do
				{
					ch = System::Console::ReadKey();

					switch (ch)
					{
					case 0:
					case 224:
						System::Console::ReadKey();
						break;
					case '\b':
						if (!password.empty())
						{
							password.pop_back();
							System::Console::Write("\b \b");
						}
						break;
					case 13: break;
					default:
						if (password.size() < limit)
						{
							if (ch >= ' ' && ch <= '~')
							{
								password += ch;
								System::Console::Write("*");
							}
						}
					}
				} while (ch != '\r');
				return password;
			}

			static bool RegexMatch(std::string value, std::string expression)
			{
				return std::regex_match(value, std::regex(expression));
			}

			static bool RegexSearch(std::string value, std::string expression)
			{
				return std::regex_search(value, std::regex(expression));
			}

			/// <summary>
			/// Memisahkan string berdasarkan delimiter.
			/// </summary>
			/// <param name="value">String yang akan dipisahkan.</param>
			/// <param name="delim">Delimiter.</param>
			/// <returns>Vector berisi string yang sudah dipisahkan.</returns>
			static std::vector<std::string> Split(std::string value, char delim)
			{
				std::stringstream ss(value);
				std::string temp;
				std::vector<std::string> result;

				while (std::getline(ss, temp, delim))
					result.push_back(temp);
				return result;
			}

			/// <summary>
			/// Mengembalikan string yang telah dikonversi menjadi lowercase.
			/// </summary>
			/// <param name="value">String yang akan dikonversi.</param>
			/// <returns>String yang telah dikonversi menjadi lowercase.</returns>
			static std::string ToLower(std::string& value)
			{
				std::string lowercase = value;

				std::for_each(lowercase.begin(), lowercase.end(), tolower);
				return lowercase;
			}

			/// <summary>
			/// Mengembalikan string yang telah dikonversi menjadi uppercase.
			/// </summary>
			/// <param name="value">String yang akan dikonversi.</param>
			/// <returns>String yang telah dikonversi menjadi uppercase.</returns>
			static std::string ToUpper(std::string& value)
			{
				std::string uppercase = value;

				std::for_each(uppercase.begin(), uppercase.end(), toupper);
				return uppercase;
			}

			/// <summary>
			/// Menulis pesan kedalam console.
			/// </summary>
			/// <param name="position">Posisi pesan saat ditampilkan.</param>
			/// <param name="color">Warna tag.</param>
			/// <param name="tag">Judul pesan (Informasi, Warning, Danger, ...)</param>
			/// <param name="message">Pesan yang disampaikan.</param>
			static void WriteMessage(System::Coordinate position, System::ConsoleColor color, std::string tag, std::string message)
			{
				System::Console::Write(position, color, "[", tag, "]", System::ConsoleColor{System::Color::Default, System::Color::Default}, " ", message);
				System::Console::ReadKey();
				System::Console::Write("\033[1K");
			}
		};

		/// <summary>
		/// Membuat menu dengan 'j' sebagai navigasi kebawah dan 'k' sebagai navigasi keatas.
		/// </summary>
		class ConsoleMenu final
		{
		private:
			int Limit;

			System::Coordinate Position;
			System::ConsoleColor Color;

			struct
			{
				int Begin;
				int Current;
				int End;
			} Index, Cursor;

			struct
			{
				std::vector<std::string> Front;
				std::vector<std::string> Back;
			} Menu;

		public:
			/// <summary>
			/// Hasil dari pilihan menu.
			/// </summary>
			struct
			{
				class : public System::ReadOnlyProperty<int> { friend class ConsoleMenu; } Index;
				class : public System::ReadOnlyProperty<const char*> { friend class ConsoleMenu; } Value;
			} Selected;

			/// <summary>
			/// Untuk mengulang menu.
			/// </summary>
			class : public System::ReadOnlyProperty<bool> { friend class ConsoleMenu; } Running;

			struct
			{
				class : public System::ReadOnlyProperty<std::vector<int>>
				{
					friend class ConsoleMenu;

				public:
					int& operator[](int index)
					{
						return this->Value[index];
					}
				} X;

				class : public System::ReadOnlyProperty<std::vector<int>>
				{
					friend class ConsoleMenu;

				public:
					int& operator[](int index)
					{
						return this->Value[index];
					}
				} Y;
			} IndexPosition;

			/// <summary>
			/// Mengembalikan ukuran dari menu.
			/// </summary>
			class : public System::ReadOnlyProperty<int> { friend class ConsoleMenu; } Size;

			/// <summary>
			/// Membuat menu baru dengan menambahkan attribut menu yang dibutuhkan.
			/// </summary>
			/// <param name="menu">Menu yang akan dibuat.</param>
			/// <param name="position">Posisi saat ditampilkan pada console.</param>
			/// <param name="color">Warna cursor atau petunjuk pada menu.</param>
			/// <param name="fill">Menyamakan panjang cursor berdasarkan index menu terpanjang.</param>
			ConsoleMenu(std::vector<std::string> menu, System::Coordinate position, System::ConsoleColor color, bool fill = false) : Menu{ menu, menu }, Position(position), Color(color)
			{
				if (fill)
				{
					std::size_t max = 0;

					for (const auto& index : this->Menu.Front) max = index.size() > max ? index.size() : max;
					for (auto& index : this->Menu.Front)index += std::string(max - index.size(), ' ');
				}

				this->Index = { 0, 0, static_cast<int>(this->Menu.Front.size() - 1) };
				this->Cursor = { this->Position.Y, this->Position.Y, this->Index.End + this->Position.Y + 1 };
				this->Limit = 0;
				this->Selected.Index.Value = 0;
				this->Selected.Value.Value = "";
				this->Running.Value = true;
				for (size_t i = 0; i < this->Menu.Front.size(); i++)
				{
					IndexPosition.X.Value.push_back(static_cast<int>(this->Menu.Front[i].size()));
					IndexPosition.Y.Value.push_back(static_cast<int>(this->Cursor.Begin + i));
				}
				this->Size.Value = static_cast<int>(menu.size());
			}

			/// <summary>
			/// Membersihkan menu yang telah tercetak pada console.
			/// </summary>
			void Clear()
			{
				int y = this->Position.Y;
				int index = this->Index.Begin;

				for (int i = 0; i < this->Limit; i++, y++, index++)
					Tools::EraseCharacter({ this->Position.X, y }, static_cast<int>(this->Menu.Front[index].size()));
			}

			/// <summary>
			/// Mengembalikan true jika salah satu index diklik.
			/// </summary>
			/// <param name="value">Index menu berdasarkan value.</param>
			/// <returns>'true' jika index ditekan.</returns>
			bool Clicked(std::string value)
			{
				return strcmp(this->Selected.Value, value.c_str()) == 0;
			}

			/// <summary>
			/// Untuk keluar dari Property Loop.
			/// </summary>
			void Stop()
			{
				this->Running.Value = false;
			}

			/// <summary>
			/// Menampilkan dan mengoperasikan menu.
			/// </summary>
			void Run()
			{
				int ySize = System::Console::GetBufferSize().Y - this->Position.Y;
				int last = static_cast<int>(this->Menu.Front.size());

				Run(last < ySize ? last : ySize);
			}

			/// <summary>
			/// Menjalankan menu dengan membatasi berapa menu yang ditampilkan berdasarkan nilai yang diberikan.
			/// </summary>
			/// <param name="limit">Batas menampilkan menu.</param>
			void Run(int limit)
			{
				char ch;

				this->Limit = limit;
				this->Cursor.End = this->Position.Y + limit - 1;
				this->Running.Value = true;

				System::Console::CursorVisible = false;
				do
				{
					short y = this->Position.Y;
					short index = this->Index.Begin;

					for (int i = 0; i < limit; i++, y++, index++)
						System::Console::Write(System::Coordinate{ this->Position.X, y }, this->Menu.Front[index]);

					System::Console::Write(System::Coordinate{ this->Position.X, this->Cursor.Current }, this->Color, this->Menu.Front[this->Index.Current], System::ConsoleColor{ System::Color::Default, System::Color::Default });
					ch = System::Console::ReadKey();
					System::Console::Write(System::Coordinate{ this->Position.X, this->Cursor.Current }, this->Menu.Front[this->Index.Current]);

					if (ch == 'j')
					{
						if (this->Cursor.Current != this->Cursor.End)
							this->Cursor.Current++, this->Index.Current++;
						else if (this->Index.Current < this->Index.End)
							this->Index.Begin++, this->Index.Current++;
					}
					else if (ch == 'k')
					{
						if (this->Cursor.Current != this->Cursor.Begin)
							this->Cursor.Current--, this->Index.Current--;
						else if (this->Index.Current > 0)
							this->Index.Begin--, this->Index.Current--;
					}
				} while (ch != '\r');
				System::Console::CursorVisible = true;
				this->Selected.Index.Value = this->Index.Current;
				this->Selected.Value.Value = this->Menu.Back[this->Index.Current].c_str();
			}
		};

		namespace Menu
		{
			/// <summary>
			/// Base class untuk menu.
			/// </summary>
			/// <typeparam name="T">Type base.</typeparam>
			template<class T>
			class Base
			{
			protected:
				std::vector<T> Menu;
				System::Coordinate Position;
				size_t MaxLength;
				bool Fill;
				int Limit;
				int Size;
				struct
				{
					int Begin;
					int Current;
					int End;
					int Limit;
				} Index;

			protected:
				virtual std::string Render(int limit) PURE;

				virtual void Loop(int limit)
				{
					char ch;

					this->Running.Value = true;

					do
					{
						System::Console::Write(this->Render(limit));

						ch = System::Console::ReadKey();

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

			public:
				virtual ~Base() {}

			public:
				class : public System::ReadOnlyProperty<bool>
				{
					friend class Dropdown;
					friend class Input;
					friend class Base;
				} Running;

				class : public System::ReadOnlyProperty<System::Coordinate>
				{
					friend class Dropdown;
					friend class Input;

					std::string ToString() const { return this->Value; }
				} CursorPosition;

				struct
				{
					class : public System::ReadOnlyProperty<int>
					{
						friend class Dropdown;
						friend class Input;
					} Index;

					class : public System::ReadOnlyProperty<const char*>
					{
						friend class Dropdown;
						friend class Input;
					} Value;
				} Selected;
			};

			class Dropdown final : public Base<std::string>
			{
			private:
				std::string Render(int limit) override
				{
					std::stringstream ss;
					System::Coordinate position = this->Position;
					System::ConsoleColor focus = { System::Color::White, System::Color::Black };
					System::ConsoleColor normal = { System::Color::Default, System::Color::Default };

					this->Index.Limit = limit + this->Index.Begin;
					this->Limit = limit;

					if (this->Fill)
						for (int i = this->Index.Begin; i < this->Index.Limit; i++, position.Y++)
							this->Index.Current == i ? ss << position << focus << this->Menu[i] << std::string(this->MaxLength - this->Menu[i].length(), ' ') << normal : ss << position << this->Menu[i] << std::string(this->MaxLength - this->Menu[i].length(), ' ');
					else
						for (int i = this->Index.Begin; i < this->Index.Limit; i++, position.Y++)
							this->Index.Current == i ? ss << position << focus << this->Menu[i] << normal << std::string(this->MaxLength - this->Menu[i].length(), ' ') : ss << position << this->Menu[i] << std::string(this->MaxLength - this->Menu[i].length(), ' ');
					return ss.str();
				}

			public:
				Dropdown(std::initializer_list<std::string> menu, System::Coordinate position, bool fill = false)
				{
					this->Menu = menu;
					this->Position = position;
					this->Fill = fill;
					this->MaxLength = std::max_element(menu.begin(), menu.end(), [](std::string a, std::string b) { return a.size() < b.size(); })->length();
					this->Index = { 0, 0, static_cast<int>(menu.size() - 1), 0 };
					this->Selected.Index.Value = 0;
					this->Selected.Value.Value = 0;
					this->Running.Value = true;
					this->CursorPosition.Value = { 0, 0 };
					this->Size = static_cast<int>(menu.size());
				}

				void Clear()
				{
					System::Coordinate position = this->Position;
					std::string fill(this->MaxLength, ' ');
					std::stringstream ss;

					for (int i = 0; i < this->Limit; i++, position.Y++)
						ss << position << fill;
					System::Console::Write(ss.str());
				}

				bool Clicked(std::string value) const
				{
					return strcmp(this->Selected.Value, value.c_str()) == 0;
				}

				void Run(int limit)
				{
					System::Console::CursorVisible = false;
					this->Loop(limit);
					this->Selected.Index.Value = this->Index.Current;
					this->Selected.Value.Value = this->Menu[this->Index.Current].c_str();
					this->CursorPosition.Value = { this->Fill ? this->Position.X + static_cast<int>(this->MaxLength) : this->Position.X + static_cast<int>(this->Menu[this->Index.Current].length()), this->Position.Y + this->Index.Current - this->Index.Begin };
					System::Console::CursorVisible = false;
				}

				void Run()
				{
					int y = System::Console::GetBufferSize().Y - Position.Y + 1;

					Run(this->Size < y ? this->Size : y);
				}

				void Stop()
				{
					this->Running.Value = false;
				}
			};

			struct InputFormat
			{
				std::string Name;
				std::string* Source;

				InputFormat(std::string name, std::string* source = nullptr) : Name(name), Source(source) {}
			};

			class Input final : public Base<InputFormat>
			{
			private:
				std::string Render(int limit) override
				{
					std::stringstream ss;
					System::Coordinate position = this->Position;
					size_t maxWidth = System::Console::GetBufferSize().X - this->MaxLength;
					System::ConsoleColor focus = { System::Color::White, System::Color::Black };
					System::ConsoleColor normal = { System::Color::Default, System::Color::Default };

					this->Index.Limit = limit + this->Index.Begin;
					this->Limit = limit;

					if (this->Fill)
						for (int i = this->Index.Begin; i < this->Index.Limit; i++, position.Y++)
						{
							InputFormat current = this->Menu[i];
							
							this->Index.Current == i ? ss << position << focus << current.Name << std::string(this->MaxLength - current.Name.length(), ' ') << normal << (current.Source == nullptr ? std::string(maxWidth, ' ') : current.Source->empty() ? std::string(maxWidth, ' ') : " " + *current.Source + std::string(maxWidth - current.Source->length(), ' ')) : ss << position << current.Name << (current.Source == nullptr ? std::string(maxWidth, ' ') : current.Source->empty() ? std::string(maxWidth, ' ') : " " + *current.Source + std::string(maxWidth - current.Source->length(), ' ')) << std::string(this->MaxLength - current.Name.length(), ' ');
						}
					else
						for (int i = this->Index.Begin; i < this->Index.Limit; i++, position.Y++)
						{
							InputFormat current = this->Menu[i];

							this->Index.Current == i ? ss << position << focus << current.Name << normal << (current.Source == nullptr ? std::string(maxWidth, ' ') : current.Source->empty() ? std::string(maxWidth, ' ') : " " + *current.Source + std::string(maxWidth - current.Source->length(), ' ')) << std::string(this->MaxLength - current.Name.length(), ' ') : ss << position << current.Name << (current.Source == nullptr ? std::string(maxWidth, ' ') : current.Source->empty() ? std::string(maxWidth, ' ') : " " + *current.Source + std::string(maxWidth - current.Source->length(), ' ')) << std::string(this->MaxLength - current.Name.length(), ' ');
						}
					return ss.str();
				}

			public:
				Input(std::initializer_list<InputFormat> menu, System::Coordinate position, bool fill = false)
				{
					this->Menu = menu;
					this->Position = position;
					this->Fill = fill;
					this->MaxLength = std::max_element(menu.begin(), menu.end(), [](InputFormat a, InputFormat b) { return a.Name.length() < b.Name.length(); })->Name.length();
					this->Index = { 0, 0, static_cast<int>(menu.size() - 1), 0 };
					this->Selected.Index.Value = 0;
					this->Selected.Value.Value = "";
					this->Running.Value = true;
					this->CursorPosition.Value = { 0, 0 };
					this->Size = static_cast<int>(menu.size());
				}

				void Clear()
				{
					System::Coordinate position = this->Position;
					std::stringstream ss;

					size_t maxSource = 0;
					for (int i = this->Index.Begin; i < this->Index.End; i++)
						maxSource = this->Menu[i].Source->length() > maxSource ? this->Menu[i].Source->length() : maxSource;

					std::string fill(this->MaxLength + maxSource + 1, ' ');
					for (int i = 0; i < this->Limit; i++, position.Y++)
						ss << position << fill;
					System::Console::Write(ss.str());
				}

				void Run(int limit)
				{
					System::Console::CursorVisible = false;
					this->Loop(limit);
					this->Selected.Index.Value = this->Index.Current;
					this->Selected.Value.Value = this->Menu[this->Index.Current].Name.c_str();
					this->CursorPosition.Value = { this->Fill ? this->Position.X + static_cast<int>(this->MaxLength) : this->Position.X + static_cast<int>(this->Menu[this->Index.Current].Name.length()), this->Position.Y + this->Index.Current - this->Index.Begin };
					System::Console::Write(this->CursorPosition.Value);
					System::Console::CursorVisible = true;
				}

				void Run()
				{
					int y = System::Console::GetBufferSize().Y - Position.Y + 1;
					
					Run(this->Size < y ? this->Size : y);
				}

				void Stop()
				{
					this->Running.Value = false;
				}
			};
		}
	}
}