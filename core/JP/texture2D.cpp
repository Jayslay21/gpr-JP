#include "texture2d.h"
#include "../ew/external/glad.h"
#include "../ew/external/stb_image.h"

// Constructor that initializes texture properties
Texture2D::Texture2D(const char* path, int filterMode, int wrapMode, bool hasAlpha)
{
    // Set texture wrapping modes
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);

    // Define border color for textures
    static const float borderColor[4] = { 1.0f, 1.0f, 0.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    // Create and bind a new texture ID
    glGenTextures(1, &m_id);
    glBindTexture(GL_TEXTURE_2D, m_id);

    // Apply filtering options for the texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMode);

    // Load image data
    stbi_set_flip_vertically_on_load(true);
    int width, height, channels;
    unsigned char* imageData = stbi_load(path, &width, &height, &channels, 0);

    if (imageData)
    {
        // Determine how to upload the texture based on transparency
        if (hasAlpha)
        {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
        }
        else
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
        }

        // Generate mipmaps for better texture quality
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cerr << "Error loading texture from " << path << std::endl;
    }

    m_width = width;
    m_height = height;

    stbi_image_free(imageData);
}

Texture2D::~Texture2D()
{
}

void Texture2D::Bind(unsigned int unit)
{
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, m_id);
}

// Getter method to retrieve the texture ID
unsigned int Texture2D::GetID() const
{
    return m_id;
}
