#ifndef _SIMPLE_
#define _SIMPLE_

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


	}
}

#endif