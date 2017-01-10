#ifndef GLTEXTURE_H
#define GLTEXTURE_H

#include "TextureData.h"

#include <string>
#include <map>
#include <memory>

namespace cogs
{
		namespace graphics
		{
				class GLTexture
				{
				public:
						GLTexture() { }
						GLTexture(const std::string& _name, const std::string& _filePath, bool _alpha = true);
						~GLTexture();

						void load(const std::string& _name, const std::string& _filePath, bool _alpha = true);

						inline void bind()   const { m_textureData.lock()->bind(); }
						inline void unbind() const { m_textureData.lock()->unbind(); }
						inline void dispose()      { m_textureData.reset(); }

						inline bool operator> (const GLTexture& _rhs) const
						{
								return m_textureData.lock() > _rhs.m_textureData.lock();
						}
						inline bool operator!= (const GLTexture& _rhs) const
						{
								return m_textureData.lock() != _rhs.m_textureData.lock();
						}

						inline unsigned int getWidth()										const { return m_textureData.lock()->getWidth(); }
						inline unsigned int getHeight()									const { return m_textureData.lock()->getHeight(); }
						inline unsigned int getID()													const { return m_textureData.lock()->getID(); }
						inline const std::string& getFilePath() const { return m_textureData.lock()->getFilePath(); }
						inline const std::string& getName()					const { return m_name; }

				private:
						static std::map<std::string, std::shared_ptr<TextureData>> s_resourceMap;

						std::weak_ptr<TextureData> m_textureData;
						std::string m_name{ "" };
				};
		}
}
#endif // !GLTEXTURE_H
