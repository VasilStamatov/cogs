#ifndef GLTEXTURE2D_H
#define GLTEXTURE2D_H

#include "Texture.h"

#include <string>
#include <glm\vec2.hpp>
#include <glm\vec4.hpp>

namespace cogs
{
		enum class TextureType
		{
				SINGLE, //single texture
				MULTIPLE //multiple textures in the texture (texture atlast/spritesheet)
		};
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
				GLTexture2D(const std::string& _name, const std::string& _filePath);
				~GLTexture2D();

				/**
				* \brief load the texture with a name and the filepath of the texture
				*/
				void load(const std::string& _name, const std::string& _filePath);

				/**
				* \brief overriden bind/unbind functions from the base Texture class
				*/
				void bind()   const override;
				void unbind() const override;

				glm::vec4 getTexCoords(int _index);
				glm::vec2 getTexOffsets(int _index);

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
				inline const TextureType& getType()					const { return m_type; }
				inline const glm::ivec2& getDims()						const { return m_tileDims; }

				inline void setType(const TextureType& _type) { m_type = _type; }
				inline void setDims(const glm::ivec2& _tileDims) { m_tileDims = _tileDims; }

		private:
				std::string m_name{ "" }; ///< the name of this texture
				std::string m_filePath{ "" }; ///< the filepath of the texture
				glm::ivec2 m_tileDims{ 0,0 }; ///< the number of rows and cols
				TextureType m_type{ TextureType::SINGLE }; ///< type of the texture
		};
}
#endif // !GLTEXTURE2D_H
