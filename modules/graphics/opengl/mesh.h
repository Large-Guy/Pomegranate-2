#ifndef POMEGRANATEENGINE_MESH_H
#define POMEGRANATEENGINE_MESH_H
#include <core/core.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "enumerations.h"

class MeshBase {
protected:
    unsigned int _vao, _vbo, _ebo;
public:
    virtual void apply() = 0;
    void bind();
    virtual size_t getVertexCount() = 0;
    virtual size_t getIndexCount() = 0;
    virtual ~MeshBase() {
        glDeleteVertexArrays(1, &_vao);
        glDeleteBuffers(1, &_vbo);
        glDeleteBuffers(1, &_ebo);
    }
};

template<typename Vertex, typename Index>
class Mesh : public MeshBase {
private:
    List<Vertex> _vertices;
    List<Index> _indices;
public:
    void apply() override {
        if (_vao != 0) {
            glDeleteVertexArrays(1, &_vao);
            glDeleteBuffers(1, &_vbo);
            glDeleteBuffers(1, &_ebo);
        }

        glGenVertexArrays(1, &_vao);
        glGenBuffers(1, &_vbo);
        glGenBuffers(1, &_ebo);

        // Bind VAO
        glBindVertexArray(_vao);

        // Load vertex data (VBO)
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * _vertices.size(), _vertices.data(), GL_STATIC_DRAW);

        // Load index data (EBO)
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Index) * _indices.size(), _indices.data(), GL_STATIC_DRAW);

        std::vector<VertexAttributeInfo> attributes = Vertex::getAttributeInfo();
        for (int i = 0; i < attributes.size(); i++) {
            GLsizei size = 0;
            GLenum type = 0;

            switch (attributes[i].type) {
                case ATTRIBUTE_TYPE_FLOAT:
                    size = 1;
                    type = GL_FLOAT;
                    break;
                case ATTRIBUTE_TYPE_FLOAT2:
                    size = 2;
                    type = GL_FLOAT;
                    break;
                case ATTRIBUTE_TYPE_FLOAT3:
                    size = 3;
                    type = GL_FLOAT;
                    break;
                case ATTRIBUTE_TYPE_FLOAT4:
                    size = 4;
                    type = GL_FLOAT;
                    break;
                case ATTRIBUTE_TYPE_INT:
                    size = 1;
                    type = GL_INT;
                    break;
                case ATTRIBUTE_TYPE_INT2:
                    size = 2;
                    type = GL_INT;
                    break;
                case ATTRIBUTE_TYPE_INT3:
                    size = 3;
                    type = GL_INT;
                    break;
                case ATTRIBUTE_TYPE_INT4:
                    size = 4;
                    type = GL_INT;
                    break;
                case ATTRIBUTE_TYPE_UNSIGNED_INT:
                    size = 1;
                    type = GL_UNSIGNED_INT;
                    break;
                case ATTRIBUTE_TYPE_UNSIGNED_INT2:
                    size = 2;
                    type = GL_UNSIGNED_INT;
                    break;
                case ATTRIBUTE_TYPE_UNSIGNED_INT3:
                    size = 3;
                    type = GL_UNSIGNED_INT;
                    break;
                case ATTRIBUTE_TYPE_UNSIGNED_INT4:
                    size = 4;
                    type = GL_UNSIGNED_INT;
                    break;
            }

            glVertexAttribPointer(i, size, type, GL_FALSE, sizeof(Vertex), (void*)attributes[i].offset);
            glEnableVertexAttribArray(i);
        }

        glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind VBO
        glBindVertexArray(0); // unbind VAO (keeps EBO bound to VAO)
    }

    Mesh() {
        _vertices = List<Vertex>();
        _indices = List<Index>();
        this->apply();
    }

    Mesh(List<Vertex> vertices, List<Index> indices) {
        _vertices = vertices;
        _indices = indices;
        this->apply();
    }

    void addVertex(Vertex vertex) {
        _vertices.push_back(vertex);
    }
    void addIndex(Index index) {
        _indices.push_back(index);
    }
    void setVertices(List<Vertex> vertices) {
        _vertices = vertices;
    }
    void setIndices(List<Index> indices) {
        _indices = indices;
    }
    size_t getVertexCount() override {
        return _vertices.size();
    }
    size_t getIndexCount() override {
        return _indices.size();
    }
};


#endif //POMEGRANATEENGINE_MESH_H
