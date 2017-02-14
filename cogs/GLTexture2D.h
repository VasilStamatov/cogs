#ifndef GLTEXTURE2D_H
#define GLTEXTURE2D_H

#include "Texture.h"
#include <string>

namespace cogs
{
		namespace graphics
		{
				class GLTexture2D : public Texture
				{
				public:
						GLTexture2D() { }
						GLTexture2D(const std::string& _name, const std::string& _filePath, bool _alpha = true);
						~GLTexture2D();

						void load(const std::string& _name, const std::string& _filePath, bool _alpha = true);

						void bind()   const override;
						void unbind() const override;

						inline bool operator> (const GLTexture2D& _rhs) const
						{
								return m_id > _rhs.m_id;
						}
						inline bool operator!= (const GLTexture2D& _rhs) const
						{
								return m_id != _rhs.m_id;
						}

						inline const std::string& getFilePath() const { return m_filePath; }
						inline const std::string& getName()					const { return m_name; }

				private:
						std::string m_name{ "" };
						std::string m_filePath{ "" };
				};
		}
}
#endif // !GLTEXTURE2D_H
