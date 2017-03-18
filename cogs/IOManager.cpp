#include "IOManager.h"

#include <fstream>
#include <filesystem>

// Namespace alias
namespace fs = std::tr2::sys;

namespace cogs
{
		bool IOManager::readFileToBuffer(const std::string & _filePath, std::vector<unsigned char>& _buffer)
		{
				std::ifstream file(_filePath, std::ios::binary);
				if (file.fail())
				{
						perror(_filePath.c_str());
						return false;
				}

				//seek to the end
				file.seekg(0, std::ios::end);

				//get the file size
				unsigned int fileSize = (unsigned int)file.tellg();
				file.seekg(0, std::ios::beg);

				//reduce the file size by any header bytes that might be present
				fileSize -= (unsigned int)file.tellg();

				//resize the buffer to the exact size we need it
				_buffer.resize(fileSize);

				//read the file to the buffer
				file.read((char *)&(_buffer[0]), fileSize);

				//close the file
				file.close();
				return true;
		}

		bool IOManager::readFileToBuffer(const std::string & _filePath, std::string & _buffer)
		{
				std::ifstream file(_filePath, std::ios::binary);
				if (file.fail())
				{
						perror(_filePath.c_str());
						return false;
				}

				//seek to the end
				file.seekg(0, std::ios::end);

				//Get the file size
				unsigned int fileSize = (unsigned int)file.tellg();
				file.seekg(0, std::ios::beg);

				//Reduce the file size by any header bytes that might be present
				fileSize -= (unsigned int)file.tellg();

				//resize the buffer to the exact size we need it
				_buffer.resize(fileSize);

				//read the file to the buffer
				file.read((char *)&(_buffer[0]), fileSize);

				//close the file
				file.close();

				return true;
		}

		bool IOManager::writeBufferToFile(const std::string & _buffer, const std::string & _filePath)
		{
				const std::string directory = _filePath.substr(0, _filePath.find_last_of('/'));
				openDirectory(directory);

				std::ofstream file(_filePath, std::ios::out | std::ios::app);
				if (file.fail())
				{
						perror(_filePath.c_str());
						return false;
				}

				file << _buffer;

				file.close();

				return true;
		}

		bool IOManager::getDirectoryEntries(const char * _path, std::vector<DirEntry>& _rvEntries)
		{
				auto dpath = fs::path(_path);
				// Must be directory
				if (!fs::is_directory(dpath)) return false;

				for (auto it = fs::directory_iterator(dpath); it != fs::directory_iterator(); ++it)
				{
						_rvEntries.emplace_back();
						_rvEntries.back().path = it->path().string();
						//check if it's a directory
						if (is_directory(it->path()))
						{
								_rvEntries.back().isDirectory = true;
						}
						else
						{
								_rvEntries.back().isDirectory = false;
						}
				}
				return true;
		}

		bool IOManager::makeDirectory(const char * _path)
		{
				return fs::create_directory(fs::path(_path));
		}

		void IOManager::openDirectory(const std::string& _filePath)
		{
				if (_filePath.find('/') != std::string::npos)
				{
						const std::string directory = _filePath.substr(0, _filePath.find_last_of('/'));

						openDirectory(directory);
				}

				auto dpath = fs::path(_filePath);

				if (!fs::exists(dpath))
				{
						makeDirectory(_filePath.c_str());
				}
		}
}