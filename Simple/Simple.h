#ifndef _SIMPLE_
#define _SIMPLE_

#include <filesystem>
#include <fstream>
#include <vector>

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
			/// <returns>'true' jika file kosong</returns>
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
			/// <param name="index">Index data yang akan diperbarui</param>
			/// <param name="value">Data baru yang akan ditulis</param>
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
	}
}

#endif