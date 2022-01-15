#pragma once

#include <conio.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
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
#define SET(T) T& operator=(const T& value) override
#define GET(T) operator T () const override

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

			operator std::string const& () const
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

			operator std::string const& () const
			{
				return "\033[" + std::to_string(this->Y) + ";" + std::to_string(this->X) + "H";
			}
		};

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
			public:
				SET(bool)
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
			public:
				SET(bool)
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
				return std::cin.get();
			}

			/// <summary>
			/// Mengembalikan karakter yang diinput tanpa menekan enter.
			/// </summary>
			/// <returns>Karakter yang diinputkan.</returns>
			static char ReadKey()
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
			/// Membuat menu baru dengan menambahkan attribut menu yang dibutuhkan.
			/// </summary>
			/// <param name="menu">Menu yang akan dibuat.</param>
			/// <param name="position">Posisi saat ditampilkan pada console.</param>
			/// <param name="color">Warna cursor atau petunjuk pada menu.</param>
			/// <param name="fill">Menyamakan panjang cursor berdasarkan index menu terpanjang.</param>
			ConsoleMenu(std::initializer_list<std::string> menu, System::Coordinate position, System::ConsoleColor color, bool fill = false) : Menu{ menu, menu }, Position(position), Color(color)
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
				int ySize = System::Console::GetBufferSize().Y;
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
	}
}