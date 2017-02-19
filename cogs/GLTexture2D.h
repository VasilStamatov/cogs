#ifndef GLTEXTURE2D_H
#define GLTEXTURE2D_H

#include "Texture.h"
#include <string>

namespace cogs
{
		namespace graphics
		{
				/**
				* \brief class to handle opengl 2D textures
				*/
				class GLTexture2D : public Texture
				{
				public:
						GLTexture2D() { }
						/**
						* \brief construct the texture with a name and the filepath of the texture
						*/
						GLTexture2D(const std::string& _name, const std::string& _filePath, bool _alpha = true);
						~GLTexture2D();

						/**
						* \brief load the texture with a name and the filepath of the texture
						*/
						void load(const std::string& _name, const std::string& _filePath, bool _alpha = true);

						/**
						* \brief overriden bind/unbind functions from the base Texture class
						*/
						void bind()   const override;
						void unbind() const override;

						//operator overloads
						inline bool operator> (const GLTexture2D& _rhs) const
						{
								return m_id > _rhs.m_id;
						}
						inline bool operator!= (const GLTexture2D& _rhs) const
						{
								return m_id != _rhs.m_id;
						}

						//getters
						inline const std::string& getFilePath() const { return m_filePath; }
						inline const std::string& getName()					const { return m_name; }

				private:
						std::string m_name{ "" }; ///< the name of this texture
						std::string m_filePath{ "" }; ///< the filepath of the texture
				};
		}
}
#endif // !GLTEXTURE2D_H
