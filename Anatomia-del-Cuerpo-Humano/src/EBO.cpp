#include "EBO.h"

EBO::EBO(const void* data, GLsizeiptr size) {
    glGenBuffers(1, &ID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

void EBO::Bind() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID); }
void EBO::Unbind() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }
void EBO::Delete() { glDeleteBuffers(1, &ID); }
