#ifndef IOMANAGER_H
#define IOMANAGER_H

#include <vector>
namespace cogs
{
		struct DirEntry
		{
				std::string path;
				bool isDirectory;
		};

		class IOManager
		{
		public:
				/*
						* \brief read the file to the buffer ( a vector of uchars, like a c-string )
						* \param[in] _filePath the filepath of the file that should be read from
						* \param[in] _buffer The buffer to write in
						* \param[out] bool return true if success of false if failed
						*/
				static bool readFileToBuffer(const std::string& _filePath, std::vector<unsigned char>& _buffer);

				/*
						* \brief read the file to the buffer ( a normal string )
						* \param[in] _filePath the filepath of the file that should be read from
						* \param[in] _buffer The buffer to write in
						* \param[out] bool return true if success of false if failed
						*/
				static bool readFileToBuffer(const std::string& _filePath, std::string& _buffer);

				static bool writeBufferToFile(const std::string& _buffer, const std::string& _filePath);

				/*
						* \brief Gets all directory entries in the directory specified by path and stores in rvEntries.
						* \param[in] _filePath the filepath of the file that should be read from
						* \param[in] _buffer The buffer to write in
						* \param[out] bool Returns false if path is not a directory.
						*/
				static bool getDirectoryEntries(const char* _path, std::vector<DirEntry>& _rvEntries);

				/*
						* \brief creates a directory in the specified path.
						* \param[in] _path the filepath where the directory should be created
						* \param[out] bool Returns false the creation failed.
						*/
				static bool makeDirectory(const char* _path);

		private:
				static void openDirectory(const std::string& _filePath);
		};
}
#endif // !IOMANAGER_H
