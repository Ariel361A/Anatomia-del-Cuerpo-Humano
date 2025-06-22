#ifndef EBO_CLASS_H
#define EBO_CLASS_H

#include <glad/glad.h>

class EBO {
public:
    GLuint ID;
    // Igual que VBO pero para ELEMENT_ARRAY_BUFFER
    EBO(const void* data, GLsizeiptr size);

    void Bind();
    void Unbind();
    void Delete();
};

#endif // EBO_CLASS_H
