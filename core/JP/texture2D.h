#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#include <string>
#include <iostream>

class Texture2D {
public:
    // Constructor 
    Texture2D(const char* filePath, int filterMode, int wrapMode, bool hasAlpha = false);

    // Destructor 
    ~Texture2D();

   
    void Bind(unsigned int slot = 0);
    unsigned int GetID() const;

private:
    unsigned int m_id; 
    int m_width;      
    int m_height;     
};

#endif // TEXTURE2D_H

