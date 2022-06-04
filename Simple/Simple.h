#pragma once

#define NOMINMAX

#include <algorithm>
#include <chrono>
#include <conio.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <random>
#include <source_location>
#include <sstream>
#include <string>
#include <thread>
#include <vector>
#include <windows.h>

using namespace std::chrono_literals;

namespace Simple {
	namespace System {
		enum class Color {
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

		struct ConsoleColor {
			Color Background;
			Color Foreground;

			operator std::string() const {
				return "\x1b[" + std::to_string((int)this->Foreground) + ";" + std::to_string((int)this->Background + 10) + "m";
			}
			std::string ToString() const {
				return (*this);
			}
			friend std::ostream& operator <<(std::ostream& out, const ConsoleColor& color) {
				return out << "\x1b[" << (int)color.Foreground << ";" << (int)color.Background + 10 << "m";
			}
			friend bool operator ==(const ConsoleColor& l, const ConsoleColor& r) {
				return l.Background == r.Background && l.Foreground == r.Foreground;
			}
			friend bool operator !=(const ConsoleColor& l, const ConsoleColor& r) {
				return !(l == r);
			}
		};

		struct Coordinate {
			short X;
			short Y;

			operator std::string() const {
				return "\x1b[" + std::to_string(this->Y) + ";" + std::to_string(this->X) + "H";
			}
			std::string ToString() const {
				return (*this);
			}
			friend std::ostream& operator <<(std::ostream& out, const Coordinate& coordinate) {
				return out << "\x1b[" << coordinate.Y << ";" << coordinate.X << "H";
			}
			friend bool operator ==(const Coordinate& l, const Coordinate& r) {
				return l.X == r.X && l.Y == r.Y;
			}
			friend bool operator !=(const Coordinate& l, const Coordinate& r) {
				return !(l == r);
			}
		};

		template<class T>
		class Property {
		protected:
			T value;

		public:
			virtual T& operator =(const T& value) {
				return this->value = value;
			}
			virtual operator T() const {
				return this->value;
			}
			virtual ~Property() {}
		};

		template<class T>
		class ReadOnlyProperty {
		protected:
			T value;

		public:
			virtual operator T() const {
				return this->value;
			}
			virtual ~ReadOnlyProperty() {}
		};

		template<class T>
		class WriteOnlyProperty {
		protected:
			T value;

		public:
			virtual T& operator =(const T& value) {
				return this->value = value;
			}
			virtual ~WriteOnlyProperty() {}
		};

#define propertyset(t) t& operator = (const t& value) override
#define propertyget(t) operator t () const override

		class Exception : std::exception {
		public:
			class : public ReadOnlyProperty<unsigned int> { friend class Exception; } Column;
			class : public ReadOnlyProperty<const char*> { friend class Exception; } File;
			class : public ReadOnlyProperty<const char*> { friend class Exception; } Function;
			class : public ReadOnlyProperty<unsigned int> { friend class Exception; } Line;
			class : public ReadOnlyProperty<const char*> { friend class Exception; } Message;

			Exception(const char* message, std::source_location path = std::source_location::current()) {
				this->Line.value = path.line();
				this->File.value = path.file_name();
				this->Column.value = path.column();
				this->Function.value = path.function_name();
				this->Message.value = message;
			}
			friend std::ostream& operator <<(std::ostream& out, const Exception& e) {
				return out
					<< "Terjadi kesalahan pada program!"
					<< "\nFile : " << e.File
					<< "\nLine : " << e.Line
					<< "\nColumn : " << e.Column
					<< "\nFunction : " << e.Function
					<< "\nMessage : " << e.Message;
			}
		};

		template<class T>
		class Range {
		private:
			const T start;
			const T stop;
			const T step;

		public:
			class Iterator {
			public:
				using IteratorCategory = std::forward_iterator_tag;
				using ValueType = T;
				using Reference = T&;
				using Pointer = T*;

			private:
				ValueType value;
				const T step;
				const T boundary;
				const bool positiveStep;

			public:
				Iterator(ValueType value, ValueType step, ValueType boundary) : value(value), step(step), boundary(boundary), positiveStep(step > 0) {}
				Iterator& operator ++() {
					this->value += this->step;

					return *this;
				}
				Iterator operator ++(int) {
					Iterator temp = *this;

					++(*this);

					return temp;
				}
				Reference operator *() {
					return this->value;
				}
				const Pointer operator ->() {
					return &this->value;
				}
				bool operator ==(const Iterator& rhs) {
					return this->positiveStep ? (this->value > rhs.value && this->value > this->boundary) : (this->value <= rhs.value && this->value < this->boundary);
				}
				bool operator !=(const Iterator& rhs) {
					return this->positiveStep ? (this->value < rhs.value&& this->value >= this->boundary) : (this->value > rhs.value && this->value <= this->boundary);
				}
			};

		public:
			Range(const T& start, const T& stop, const T& step) : start(start), stop(stop), step(step) {
				if (step == 0)
					throw Exception("Range step tidak boleh nol.");
				else if ((this->start > this->stop && this->step > 0) || this->start < this->stop && this->step < 0)
					throw Exception("Range argument harus mencapai terminasi.");
			}
			Range(const T& start, const T& stop) : start(start), stop(stop), step(start < stop ? (T)1 : (T)-1) {}
			Iterator begin() const {
				return Iterator(this->start, this->step, this->start);
			}
			Iterator end() const {
				return Iterator(this->stop, this->step, this->start);
			}
		};

		struct DateTime {
			int Second;
			int Minute;
			int Hour;
			int DayOfMonth;
			int Month;
			int Year;
			int DayOfWeek;
			int DayOfYear;
			int Daylight;

			static DateTime Now()
			{
				time_t now = time(0);
				tm time;
				DateTime res;

				localtime_s(&time, &now);

				res.Second = time.tm_sec;
				res.Minute = time.tm_min;
				res.Hour = time.tm_hour;
				res.DayOfWeek = time.tm_wday;
				res.DayOfMonth = time.tm_mday;
				res.DayOfYear = time.tm_yday;
				res.Month = time.tm_mon + 1;
				res.Year = time.tm_year + 1900;
				res.Daylight = time.tm_isdst;

				return res;
			}
			std::string ToString() const {
				return std::to_string(this->DayOfMonth) + "/" + std::to_string(this->Month) + "/" + std::to_string(this->Year) + " " + ((this->Hour < 10) ? "0" + std::to_string(this->Hour) + ":" : std::to_string(this->Hour) + ":") + ((this->Minute < 10) ? "0" + std::to_string(this->Minute) + ":" : std::to_string(this->Minute) + ":") + ((this->Second < 10) ? "0" + std::to_string(this->Second) : std::to_string(this->Second));
			}
			friend std::ostream& operator <<(std::ostream& out, const DateTime& dateTime) {
				return out << dateTime.ToString();
			}
		};

		template<class T>
		class BinaryFile {
		protected:
			std::filesystem::path fileName;
			std::fstream file;

		private:
			bool Open(std::ios_base::openmode mode) {
				this->file.open(this->fileName, mode);

				return this->file.is_open();
			}
			void Close() {
				this->file.close();
			}

		public:
			BinaryFile(std::filesystem::path fileName) {
				if (fileName.empty())
					throw Exception("Nama atau path file tidak boleh kosong.");
				else if (!fileName.has_extension())
					fileName.append(".bin");
				else if (fileName.extension() != ".bin")
					fileName.replace_extension(".bin");

				this->fileName = fileName;

				if (!std::filesystem::exists(this->fileName)) {
					if (this->Open(std::ios::out))
						this->Close();
					else
						throw Exception("Gagal membuat file baru.");
				}
			}
			virtual void Delete(size_t index, bool useVector = true) {
				if (useVector) {
					const auto fileData = Read();

					if (this->Open(std::ios::out | std::ios::binary | std::ios::trunc)) {
						for (auto i = 0; i < fileData.size(); i++)
							if (i != index)
								this->file.write((char*)&fileData[i], sizeof T);

						this->Close();
					}
					else
						throw Exception("Gagal membuka file");
				}
				else {
					T temp;
					std::fstream fileTemp;

					if (this->Open(std::ios::in | std::ios::binary)) {
						fileTemp.open("temp.bin", std::ios::out | std::ios::binary);

						for (auto i = 0; this->file.read((char*)&temp, sizeof T); i++)
							if (i != index)
								fileTemp.write((char*)&temp, sizeof T);

						this->Close();
						fileTemp.close();

						//std::remove(this->fileName.c_str());
						//std::rename("temp.bin", this->fileName.c_str());
					}
					else
						throw Exception("Gagal membuka file.");
				}
			}
			bool IsEmpty() {
				if (this->Open(std::ios::in)) {
					bool empty = this->file.peek() == std::fstream::traits_type::eof();

					this->Close();

					return empty;
				}
				else
					throw Exception("Gagal membuka file.");
			}
			virtual std::vector<T> Read() {
				T temp;
				std::vector<T> fileData;

				if (this->Open(std::ios::in | std::ios::binary)) {
					while (this->file.read((char*)&temp, sizeof T))
						fileData.push_back(temp);

					this->Close();

					return fileData;
				}
				else
					throw Exception("Gagal membuka file.");
			}
			virtual void Update(size_t index, const T& newValue) {
				if (this->Open(std::ios::in | std::ios::out | std::ios::binary)) {
					this->file.seekp(index * sizeof T, std::ios::beg);
					this->file.write((char*)&newValue, sizeof T);
					this->Close();
				}
				else
					throw Exception("Gagal membuka file.");
			}
			virtual void Write(const T& value) {
				if (this->Open(std::ios::out | std::ios::binary | std::ios::app)) {
					this->file.write((char*)&value, sizeof T);
					this->Close();
				}
				else
					throw Exception("Gagal membuka file.");
			}
			virtual void Write(const std::vector<T>& value) {
				if (this->Open(std::ios::out | std::ios::binary | std::ios::app)) {
					for (const auto& i : value)
						this->file.write((char*)&i, sizeof T);

					this->Close();
				}
			}
			virtual ~BinaryFile() {}
		};

		class Console final {
		public:
			inline static class : public WriteOnlyProperty<bool> {
			public:
				propertyset(bool) {
					Write("\x1b[?12" + value ? "h" : "l");
					
					return this->value = value;
				}
			} CursorBlinking;
			inline static class : public WriteOnlyProperty<bool> {
			public:
				propertyset(bool) {
					Write("\x1b[?25" + value ? "h" : "l");
					
					return this->value = value;
				}
			} CursorVisible;

		public:
			static void Clear() {
				system("cls");
			}
			static void DisableMaximizeButton() {
				SetWindowLong(GetConsoleWindow(), GWL_STYLE, GetWindowLong(GetConsoleWindow(), GWL_STYLE) & ~WS_MAXIMIZEBOX);
			}
			static void DisableMinimizeButton() {
				SetWindowLong(GetConsoleWindow(), GWL_STYLE, GetWindowLong(GetConsoleWindow(), GWL_STYLE) & ~WS_MINIMIZEBOX);
			}
			static void DisableResizeWindow() {
				SetWindowLong(GetConsoleWindow(), GWL_STYLE, GetWindowLong(GetConsoleWindow(), GWL_STYLE) & ~WS_SIZEBOX);
			}
			static void EnableVirtual() {
				HANDLE outputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
				if (outputHandle == INVALID_HANDLE_VALUE)
					throw Exception("Gagal mendapatkan handle output.");

				DWORD mode = 0;
				if (!GetConsoleMode(outputHandle, &mode))
					throw Exception("Gagal mendapatkan mode console.");

				mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
				if (!SetConsoleMode(outputHandle, mode))
					throw Exception("Gagal mengatur mode console.");
			}
			static Coordinate GetBufferSize() {
				HANDLE outputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
				if (outputHandle == INVALID_HANDLE_VALUE)
					throw Exception("Gagal mendapatkan handle output.");

				CONSOLE_SCREEN_BUFFER_INFO csbi;
				if (!GetConsoleScreenBufferInfo(outputHandle, &csbi))
					throw Exception("Gagal mendapatkan informasi buffer.");

				return { csbi.dwSize.X, csbi.dwSize.Y };
			}
			static Coordinate GetWindowSize() {
				HANDLE outputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
				if (outputHandle == INVALID_HANDLE_VALUE)
					throw Exception("Gagal mendapatkan handle output.");

				CONSOLE_SCREEN_BUFFER_INFO csbi;
				if (!GetConsoleScreenBufferInfo(outputHandle, &csbi))
					throw Exception("Gagal mendapatkan informasi buffer.");

				return { static_cast<short>(csbi.srWindow.Right - csbi.srWindow.Left + 1), static_cast<short>(csbi.srWindow.Bottom - csbi.srWindow.Top + 1) };
			}
			static int Read() {
				return std::cin.get();
			}
			static int ReadKey() {
				return _getch();
			}
			static std::string ReadLine() {
				std::string line;

				std::getline(std::cin, line);

				return line;
			}
			static void SetBufferSize(short width, short height) {
				HANDLE outputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
				if (outputHandle == INVALID_HANDLE_VALUE)
					throw Exception("Gagal mendapatkan handle output.");

				if (!SetConsoleScreenBufferSize(outputHandle, { width, height }))
					throw Exception("Gagal mengatur ukuran buffer.");
			}
			static void SetWindowSize(short width, short height) {
				HANDLE outputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
				if (outputHandle == INVALID_HANDLE_VALUE)
					throw Exception("Gagal mendapatkan handle output.");

				COORD largest = GetLargestConsoleWindowSize(outputHandle);
				if (width > largest.X)
					throw Exception("Ukuran lebar terlalu besar.");
				if (height > largest.Y)
					throw Exception("Ukuran tinggi terlalu besar.");

				Coordinate _ = GetBufferSize();
				COORD bufferSize{ _.X, _.Y };

				if (bufferSize.X > width || bufferSize.Y > height) {
					SMALL_RECT windowSizeBefore{ 0, 0, width < bufferSize.X ? width - 1 : bufferSize.X - 1, height < bufferSize.Y ? height - 1 : bufferSize.Y - 1 };

					if (!SetConsoleWindowInfo(outputHandle, true, &windowSizeBefore))
						throw Exception("Gagal mengatur ukuran window sebelum buffer.");
				}

				if (!SetConsoleScreenBufferSize(outputHandle, { width, height }))
					throw Exception("Gagal mengatur ukuran buffer.");

				SMALL_RECT windowSizeAfter{ 0, 0, width - 1, height - 1 };
				if (!SetConsoleWindowInfo(outputHandle, true, &windowSizeAfter))
					throw Exception("Gagal mengatur ukuran window setelah buffer.");
			}
			template<class R, class P>
			static void Sleep(const std::chrono::duration<R, P>& duration) {
				std::this_thread::sleep_for(duration);
			}
			template<class... T>
			static void Write(T... value) {
				((std::cout << value), ...);
			}
			template<class... T>
			static void WriteLine(T... value) {
				Write(value..., "\n");
			}
		};
	} /* !System */

	namespace Utility {
		class Random final {
		public:
			static char Character(char min, char max) {
				std::random_device random;
				std::mt19937 engine(random());
				std::uniform_int_distribution<> intDist(min, max);

				return intDist(engine);
			}
			template<class It>
			static It Element(It begin, It end) {
				std::random_device random;
				std::mt19937 engine(random());
				std::uniform_int_distribution<size_t> dist(0, std::distance(begin, end) - 1);

				std::advance(begin, dist(engine));

				return begin;
			}
			template<class T>
			static std::vector<T> Element(const std::vector<T>& value, unsigned int count) {
				std::random_device random;
				std::mt19937 engine(random());
				std::vector<T> result;

				std::sample(value.begin(), value.end(), std::back_inserter(result), count, engine);

				return result;
			}
			template<class T = int>
			static T Integer(T min, T max) {
				std::random_device random;
				std::mt19937 engine(random());
				std::uniform_int_distribution<T> intDist(min, max);

				return intDist(engine);
			}
			template<class T = double>
			static T FloatingPoint(T min, T max) {
				std::random_device random;
				std::mt19937 engine(random());
				std::uniform_real_distribution<T> intDist(min, max);

				return intDist(engine);
			}
		};

		class Tools final {
		public:
			static void Clear(std::string& str) {
				if (!str.empty()) {
					System::Console::Write(EraseCharacter(str.length()));

					str.clear();
				}
			}
			static bool Compare(const char* a, const char* b) {
				return strcmp(a, b) == 0;
			}
			static bool Compare(const std::string& a, const std::string& b) {
				return a == b;
			}
			static std::string EraseCharacter(size_t count) {
				return "\x1b[" + std::to_string(count) + "X";
			}
			static std::string EraseCharacter(System::Coordinate begin, System::Coordinate end) {
				std::string erase;

				for (short y = begin.Y; y < end.Y; y++)
					erase.append(System::Coordinate{ begin.X, y }.ToString() + EraseCharacter(end.X));

				return erase;
			}
			static bool IsAlpha(const std::string& value) {
				return !value.empty() && std::all_of(value.begin(), value.end(), ::isalpha);
			}
			static bool IsNumber(const std::string& value) {
				return !value.empty() && std::all_of(value.begin(), value.end(), ::isdigit);
			}
			static std::string ReadLine(unsigned int count = 128) {
				std::string line;
				int ch;

				do {
					ch = System::Console::ReadKey();

					switch (ch) {
					case 0:
					case 0xe0:
						System::Console::ReadKey();
						break;
					case 0x0d:
						break;
					case '\b':
						if (!line.empty()) {
							line.pop_back();
							System::Console::Write("\b \b");
						}
						break;
					default:
						if (line.size() < count) {
							if (ch > 0x1f && ch < 0x7f) {
								line.push_back(ch);
								System::Console::Write((char)ch);
							}
						}
					}
				} while (ch != 0x0d);

				return line;
			}
			static std::string ReadNumber(unsigned int count = 128) {
				std::string number;
				int ch;

				do {
					ch = System::Console::ReadKey();

					switch (ch) {
					case 0:
					case 0xe0:
						System::Console::ReadKey();
						break;
					case 0x0d:
						break;
					case 0x08:
						if (!number.empty()) {
							number.pop_back();
							System::Console::Write("\b \b");
						}
						break;
					default:
						if (number.size() < count) {
							if (ch > 0x2f && ch < 0x3a) {
								number.push_back(ch);
								System::Console::Write((char)ch);
							}
						}
					}
				} while (ch != 0x0d);

				return number;
			}
			static std::string ReadPassword(unsigned int count = 128) {
				std::string password;
				int ch;

				do {
					ch = System::Console::ReadKey();

					switch (ch) {
					case 0:
					case 0xe0:
						System::Console::ReadKey();
						break;
					case 0x0d:
						break;
					case '\b':
						if (!password.empty()) {
							password.pop_back();
							System::Console::Write("\b \b");
						}
						break;
					default:
						if (password.size() < count) {
							if (ch > 0x1f && ch < 0x7f) {
								password.push_back(ch);
								System::Console::Write("*");
							}
						}
					}
				} while (ch != 0x0d);

				return password;
			}
			static std::string ToLower(const std::string value) {
				std::string lowercase = value;

				std::for_each(lowercase.begin(), lowercase.end(), ::tolower);

				return lowercase;
			}
			static std::string ToUpper(const std::string value) {
				std::string uppercase = value;

				std::for_each(uppercase.begin(), uppercase.end(), ::toupper);

				return uppercase;
			}
			static std::string WriteColor(System::ConsoleColor color, std::string value) {
				return color.ToString() + value + System::ConsoleColor{ System::Color::Black, System::Color::White }.ToString();
			}
			static void WriteMessage(System::Coordinate position, std::string flag, std::string message, System::ConsoleColor color = { System::Color::Cyan, System::Color::Black }) {
				System::Console::Write(position, color, "[", flag, "]", System::ConsoleColor{ System::Color::Black, System::Color::White }, " " + message);
				System::Console::ReadKey();
				System::Console::Write("\x1b[2K");
			}
		};

		class Table final {
		private:
			std::vector<std::string> header;
			std::vector<std::vector<std::string>> row;
			std::vector<size_t> width;

			struct RowType {
				std::string left;
				std::string intersect;
				std::string right;
			};

			struct TableStyle {
				std::string horizontal;
				std::string vertical;
				RowType top;
				RowType middle;
				RowType bottom;
			};

			const TableStyle basic = { "-", "|", {"+", "+", "+"}, {"+", "+", "+"}, {"+", "+", "+"} };
			const TableStyle line = { "━", "┃", {"┏", "┳", "┓"}, {"┣", "╋", "┫"}, {"┗", "┻", "┛"} };
			const TableStyle doubleLine = { "═", "║", {"╔", "╦", "╗"}, {"╠", "╬", "╣"}, {"╚", "╩", "╝"} };
			const TableStyle invisible = { " ", " ", {" ", " ", " "}, {" ", " ", " "}, {" ", " ", " "} };

			TableStyle style = basic;

			unsigned padding = 1;

		private:
			std::string GetLine(RowType type) const {
				std::stringstream line;

				line << type.left;
				for (size_t i = 0; i < this->width.size(); i++) {
					for (size_t j = 0; j < (this->width[i] + (padding * 2)); j++)
						line << this->style.horizontal;

					line << (i == this->width.size() - 1 ? type.right : type.intersect);
				}

				return line.str() + "\n";
			}
			std::string GetHeader(std::vector<std::string> header) const {
				std::stringstream line;

				line << this->style.vertical;
				for (size_t i = 0; i < this->header.size(); i++) {
					std::string current = this->header[i];

					line << std::string(this->padding, ' ') + current + std::string(this->width[i] - current.length(), ' ') + std::string(this->padding, ' ');
					line << this->style.vertical;
				}
				line << "\n";

				return line.str();
			}
			std::string GetRow(std::vector<std::vector<std::string>> row) const {
				std::stringstream line;

				for (auto& r : row) {
					line << this->style.vertical;

					for (size_t i = 0; i < r.size(); i++) {
						std::string current = r[i];

						line << std::string(this->padding, ' ') + current + std::string(this->width[i] - current.length(), ' ') + std::string(this->padding, ' ');
						line << this->style.vertical;
					}

					line << "\n";
				}

				return line.str();
			}

		public:
			Table(std::initializer_list<std::string> header) : header(header) {
				for (const auto& i : header)
					this->width.push_back(i.length());
			}
			void Add(std::initializer_list<std::string> row) {
				if (row.size() > this->width.size())
					throw System::Exception("Ukuran row harus sama dengan header.");

				auto temp = std::vector<std::string>{ row };

				this->row.push_back(temp);
				for (size_t i = 0; i < temp.size(); i++)
					this->width[i] = std::max(temp[i].size(), this->width[i]);
			}
			void Remove(size_t index) {
				this->row.erase(this->row.begin() + index);
			}
			void SetPadding(unsigned padding) {
				this->padding = padding;
			}
			void SetStyle(unsigned style) {
				switch (style) {
				case 0:
					this->style = basic;
					break;
				case 1:
					this->style = line;
					break;
				case 2:
					this->style = doubleLine;
					break;
				case 3:
					this->style = invisible;
					break;
				default:
					this->style = basic;
				}
			}
			void Sort(bool ascending) {
				if (ascending)
					std::sort(this->row.begin(), this->row.end(), std::less<std::vector<std::string>>());
				else
					std::sort(this->row.begin(), this->row.end(), std::greater<std::vector<std::string>>());
			}
			void UpdateHeader(size_t header, std::string newHeader) {
				if (header > this->header.size() - 1)
					throw System::Exception("Index header diluar batas");

				this->header[header] = newHeader;
			}
			void UpdateRow(size_t row, size_t header, std::string newRow) {
				if (row > this->row.size() - 1)
					throw System::Exception("Index row diluar batas");
				if (header > this->header.size() - 1)
					throw System::Exception("Index header diluar batas");

				this->row[row][header] = newRow;
			}
			friend std::ostream& operator <<(std::ostream& out, const Table& value) {
				out
					<< value.GetLine(value.style.top)
					<< value.GetHeader(value.header)
					<< value.GetLine(value.style.middle)
					<< value.GetRow(value.row)
					<< value.GetLine(value.style.bottom);

				return out;
			}
		};

		namespace ConsoleMenu {
			constexpr auto EraseLine = "\x1b[2K";
			constexpr auto EraseLineBeforeCursor = "\x1b[1K";
			constexpr auto EraseLineAfterCursor = "\x1b[0K";
			constexpr auto EraseDisplay = "\x1b[2J";
			constexpr auto EraseDisplayBeforeCursor = "\x1b[1J";
			constexpr auto EraseDisplayAfterCursor = "\x1b[0J";

			class Input {
			public:
				struct Text {
					std::string Label;
					std::string* Source;
					std::string Placeholder;
					bool Password;

					Text(std::string label, std::string* source = nullptr, std::string placeholder = "", bool password = false) : Label(label), Source(source), Placeholder(placeholder), Password(password) {}
				};

				struct {
					class : public System::ReadOnlyProperty<size_t> { friend class Input; } Index;
					class : public System::ReadOnlyProperty<const char*> { friend class Input; } Value;
				} Selected;

				class : public System::ReadOnlyProperty<bool> { friend class Input; } Running;
				class : public System::ReadOnlyProperty<size_t> { friend class Input; } Size;

			private:
				std::vector<Text> menu;
				System::Coordinate position;

				struct {
					size_t begin;
					size_t current;
					size_t end;
					size_t limit;
				} index;

				struct {
					System::ConsoleColor focus = { System::Color::White, System::Color::Black };
					System::ConsoleColor normal = { System::Color::Black, System::Color::White };
					System::ConsoleColor placeholder = { System::Color::Black, System::Color::Gray };
				} color;

			private:
				inline std::string Render(unsigned short count) {
					std::string rendered;
					System::Coordinate position = this->position;

					this->index.limit = count + this->index.begin;

					for (auto i = this->index.begin; i < this->index.limit; i++, position.Y++) {
						Text current = this->menu[i];
						std::string temp = position.ToString() + EraseLine;

						i == this->index.current ? temp += this->color.focus.ToString() + current.Label + this->color.normal.ToString() + " " : temp += current.Label + " ";

						(current.Source == nullptr || current.Source->empty()) ? temp += this->color.placeholder.ToString() + current.Placeholder + this->color.normal.ToString() : current.Password ? temp += std::string(current.Source->length(), '*') : temp += *current.Source;

						rendered.append(temp);
					}

					return rendered;
				}
				void Loop(unsigned short count) {
					int ch;

					do {
						System::Console::Write(this->Render(count));

						ch = System::Console::ReadKey();

						switch (ch) {
						case 'j':
						case 'J':
							if (this->index.current != this->index.end)
							{
								this->index.current++;
								if (this->index.current == this->index.limit)
									this->index.begin++;
							}
							break;
						case 'k':
						case 'K':
							if (this->index.current != 0)
							{
								this->index.current--;
								if (this->index.current < this->index.begin)
									this->index.begin--;
							}
							break;
						}
					} while (ch != 0x0d);
				}

			public:
				Input(std::initializer_list<Text> menu, System::Coordinate position) : menu(menu), position(position) {
					this->index.begin = 0;
					this->index.current = 0;
					this->index.end = menu.size() - 1;
					this->index.limit = 0;
					this->Selected.Index.value = 0;
					this->Selected.Value.value = "";
					this->Running.value = false;
					this->Size.value = menu.size();
				}
				void Run(unsigned short count) {
					this->Running.value = true;
					this->Loop(count);
					this->Selected.Index.value = this->index.current;
					this->Selected.Value.value = this->menu[this->index.current].Label.c_str();

					System::Console::Write(System::Coordinate{ static_cast<short>(this->menu[this->index.current].Label.length() + this->position.X + 1), static_cast<short>(this->position.Y + this->index.current - this->index.begin) }, EraseLineAfterCursor);
				}
				void Run() {
					short y = System::Console::GetBufferSize().Y - this->position.Y + 1;
					short s = (int)this->Size;

					Run(s < y ? s : y);
				}
				void SetCursorColor(System::ConsoleColor color) {
					this->color.focus = color;
				}
				void Stop() {
					this->Running.value = false;
				}
			};

			class Selection {
			public:
				struct {
					class : public System::ReadOnlyProperty<size_t> { friend class Selection; } Index;
					class : public System::ReadOnlyProperty<const char*> { friend class Selection; } Value;
				} Selected;

				class : public System::ReadOnlyProperty<bool> { friend class Selection; } Running;
				class : public System::ReadOnlyProperty<size_t> { friend class Selection; } Size;

			private:
				std::vector<std::string> menu;
				System::Coordinate position;
				bool fill;
				size_t maxLength;

				struct {
					size_t begin;
					size_t current;
					size_t end;
					size_t limit;
				} index;

				struct {
					System::ConsoleColor focus = { System::Color::White, System::Color::Black };
					System::ConsoleColor normal = { System::Color::Black, System::Color::White };
				} color;

			private:
				inline std::string Render(unsigned short count) {
					std::string rendered;
					System::Coordinate position = this->position;

					this->index.limit = count + this->index.begin;

					for (auto i = this->index.begin; i < this->index.limit; i++, position.Y++) {
						std::string current = this->menu[i];
						std::string temp = position.ToString();

						if (this->fill)
							i == this->index.current ? temp += this->color.focus.ToString() + current + std::string(this->maxLength - current.length(), ' ') + this->color.normal.ToString() : temp += current + std::string(this->maxLength - current.length(), ' ');
						else
							i == this->index.current ? temp += this->color.focus.ToString() + current + this->color.normal.ToString() + std::string(this->maxLength - current.length(), ' ') : temp += current + std::string(this->maxLength - current.length(), ' ');

						rendered.append(temp);
					}

					return rendered;
				}
				void Loop(unsigned short count) {
					int ch;

					do {
						System::Console::Write(this->Render(count));

						ch = System::Console::ReadKey();

						switch (ch) {
						case 0x6a:
						case 0x4a:
							if (this->index.current != this->index.end)
							{
								this->index.current++;
								if (this->index.current == this->index.limit)
									this->index.begin++;
							}
							break;
						case 0x6b:
						case 0x4b:
							if (this->index.current != 0)
							{
								this->index.current--;
								if (this->index.current < this->index.begin)
									this->index.begin--;
							}
							break;
						}
					} while (ch != 0x0d);
				}

			public:
				Selection(std::initializer_list<std::string> menu, System::Coordinate position, bool fill = true) : menu(menu), position(position), fill(fill) {
					this->index.begin = 0;
					this->index.current = 0;
					this->index.end = menu.size() - 1;
					this->index.limit = 0;
					this->Selected.Index.value = 0;
					this->Selected.Value.value = "";
					this->Running.value = false;
					this->Size.value = menu.size();
					this->maxLength = std::max_element(menu.begin(), menu.end(), [](std::string a, std::string b) { return a.length() < b.length(); })->length();
				}
				Selection(std::vector<std::string> menu, System::Coordinate position, bool fill = true) : menu(menu), position(position), fill(fill) {
					this->index.begin = 0;
					this->index.current = 0;
					this->index.end = menu.size() - 1;
					this->index.limit = 0;
					this->Selected.Index.value = 0;
					this->Selected.Value.value = "";
					this->Running.value = false;
					this->Size.value = menu.size();
					this->maxLength = std::max_element(menu.begin(), menu.end(), [](std::string a, std::string b) { return a.length() < b.length(); })->length();
				}
				void Clear() {
					std::string clear;
					System::Coordinate position = this->position;

					for (auto i = this->index.begin; i < this->index.limit; i++, position.Y++)
						clear.append(position.ToString() + Tools::EraseCharacter(this->maxLength));

					System::Console::Write(clear);
				}
				void Run(unsigned short count) {
					this->Running.value = true;
					this->Loop(count);
					this->Selected.Index.value = this->index.current;
					this->Selected.Value.value = this->menu[this->index.current].c_str();

					System::Console::Write(System::Coordinate{ static_cast<short>(this->menu[this->index.current].length() + this->position.X), static_cast<short>(this->position.Y + this->index.current - this->index.begin) });
				}
				void Run() {
					short y = System::Console::GetBufferSize().Y - this->position.Y + 1;
					short s = (int)this->Size;

					Run(s < y ? s : y);
				}
				void SetCursorColor(System::ConsoleColor color) {
					this->color.focus = color;
				}
				void Stop() {
					this->Running.value = false;
				}
			};

			class Question {
			public:
				struct {
					class : public System::ReadOnlyProperty<size_t> { friend class Question; } Index;
					class : public System::ReadOnlyProperty<const char*> { friend class Question; } Value;
				} Selected;

				class : public System::ReadOnlyProperty<bool> { friend class Question; } Running;
				class : public System::ReadOnlyProperty<size_t> { friend class Question; } Size;

			private:
				std::vector<std::string> choice;
				System::Coordinate position;
				bool fill;
				size_t maxLength;
				std::string question;

				struct {
					size_t begin;
					size_t current;
					size_t end;
					size_t limit;
				} index;

				struct {
					System::ConsoleColor focus = { System::Color::White, System::Color::Black };
					System::ConsoleColor normal = { System::Color::Black, System::Color::White };
				} color;

			private:
				inline std::string Render(unsigned short count) {
					System::Coordinate position = this->position;
					std::string rendered = position.ToString() + this->question;

					this->index.limit = count + this->index.begin;
					position.Y++;

					for (auto i = this->index.begin; i < this->index.limit; i++, position.Y++) {
						std::string current = this->choice[i];
						std::string temp = position.ToString();

						if (this->fill)
							i == this->index.current ? temp += this->color.focus.ToString() + current + std::string(this->maxLength - current.length(), ' ') + this->color.normal.ToString() : temp += current + std::string(this->maxLength - current.length(), ' ');
						else
							i == this->index.current ? temp += this->color.focus.ToString() + current + this->color.normal.ToString() + std::string(this->maxLength - current.length(), ' ') : temp += current + std::string(this->maxLength - current.length(), ' ');

						rendered.append(temp);
					}

					return rendered;
				}
				void Loop(unsigned short count) {
					int ch;

					do {
						System::Console::Write(this->Render(count));

						ch = System::Console::ReadKey();

						switch (ch) {
						case 0x6a:
						case 0x4a:
							if (this->index.current != this->index.end)
							{
								this->index.current++;
								if (this->index.current == this->index.limit)
									this->index.begin++;
							}
							break;
						case 0x6b:
						case 0x4b:
							if (this->index.current != 0)
							{
								this->index.current--;
								if (this->index.current < this->index.begin)
									this->index.begin--;
							}
							break;
						}
					} while (ch != 0x0d);
				}

			public:
				Question(std::string question, std::initializer_list<std::string> choice, System::Coordinate position, bool fill = true) : question(question), choice(choice), position(position), fill(fill) {
					this->index.begin = 0;
					this->index.current = 0;
					this->index.end = choice.size() - 1;
					this->index.limit = 0;
					this->Selected.Index.value = 0;
					this->Selected.Value.value = "";
					this->Running.value = false;
					this->Size.value = choice.size();
					this->maxLength = std::max_element(choice.begin(), choice.end(), [](std::string a, std::string b) { return a.length() < b.length(); })->length();
				}
				void Clear() {
					System::Coordinate position = this->position;
					std::string clear = position.ToString() + Tools::EraseCharacter(this->question.length());

					position.Y++;
					for (auto i = this->index.begin; i < this->index.limit; i++, position.Y++)
						clear.append(position.ToString() + Tools::EraseCharacter(this->maxLength));

					System::Console::Write(clear);
				}
				void Run(unsigned short count) {
					this->Running.value = true;
					this->Loop(count);
					this->Selected.Index.value = this->index.current;
					this->Selected.Value.value = this->choice[this->index.current].c_str();

					System::Console::Write(System::Coordinate{ static_cast<short>(this->choice[this->index.current].length() + this->position.X), static_cast<short>(this->position.Y + this->index.current - this->index.begin) });
				}
				void Run() {
					short y = System::Console::GetBufferSize().Y - this->position.Y + 1;
					short s = (int)this->Size;

					Run(s < y ? s : y);
				}
				void SetCursorColor(System::ConsoleColor color) {
					this->color.focus = color;
				}
				void Stop() {
					this->Running.value = false;
				}
			};
		} /* !ConsoleMenu */
	} /* !Utility */
} /* !Simple */