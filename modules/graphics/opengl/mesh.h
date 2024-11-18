#ifndef POMEGRANATEENGINE_MESH_H
#define POMEGRANATEENGINE_MESH_H
#include <core/core.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "enumerations.h"
#include <algorithm>
#include <math/math.h>

class MeshBase : public Asset {
protected:
    unsigned int _vao, _vbo, _ebo;
public:
    MeshBase() : Asset() {
        _vao = 0;
        _vbo = 0;
        _ebo = 0;
    }
    MeshBase(std::string path, std::string name) : Asset(path, name) {
        _vao = 0;
        _vbo = 0;
        _ebo = 0;
    }
    virtual void apply() = 0;
    virtual size_t getVertexCount() = 0;
    virtual size_t getIndexCount() = 0;
    virtual ~MeshBase() {
        glDeleteVertexArrays(1, &_vao);
        glDeleteBuffers(1, &_vbo);
        glDeleteBuffers(1, &_ebo);
    }
    friend class Window;
};

template<typename Vertex, typename Index>
class Mesh : public MeshBase {
private:
    std::vector<Vertex> _vertices;
    std::vector<Index> _indices;

    struct IndexInfo {
        Index index;
        Index uvIndex;
        Index normalIndex;
    };

    std::vector<std::string> split(const std::string& str, const std::string& delimiter) {
        std::vector<std::string> parts;
        size_t start = 0;
        size_t end = str.find(delimiter);
        while(end != std::string::npos) {
            parts.push_back(str.substr(start, end - start));
            start = end + delimiter.length();
            end = str.find(delimiter, start);
        }
        parts.push_back(str.substr(start, end));
        return parts;
    }

    int vertexAttributeIndex(AttributeType type) {
        std::vector<VertexAttributeInfo> attributes = Vertex::getAttributeInfo();
        for(int i = 0; i < attributes.size(); i++) {
            if(attributes[i].type == type) {
                return i;
            }
        }
        return -1;
    }

    void loadOBJ(const std::string& path) {
        File file(path);
        file.open();
        if (!file.exists()) {
            return;
        }

        std::vector<VertexAttributeInfo> attributes = Vertex::getAttributeInfo();


        std::vector<std::string> lines = split(file.readText(), "\n");

        std::vector<int> attributeCount(attributes.size(), 0);

        std::vector<Vector3> vertices;
        std::vector<IndexInfo> indices;
        std::vector<Vector3> normals;
        std::vector<Vector2> uvs;

        for(auto& line : lines) {

            int positionIndex = vertexAttributeIndex(ATTRIBUTE_TYPE_POSITION);
            if(positionIndex != -1) {
                if (line.find("v ") != std::string::npos) {
                    std::vector<std::string> parts = split(line, " ");
                    Vector3 position = {std::stof(parts[1]), std::stof(parts[2]), std::stof(parts[3])};

                    vertices.push_back(position);
                    attributeCount[positionIndex]++;
                }
            }

            int normalIndex = vertexAttributeIndex(ATTRIBUTE_TYPE_NORMAL);
            if(normalIndex != -1) {
                if (line.find("vn ") != std::string::npos) {
                    std::vector<std::string> parts = split(line, " ");
                    Vector3 normal = {std::stof(parts[1]), std::stof(parts[2]), std::stof(parts[3])};

                    normals.push_back(normal);

                    attributeCount[normalIndex]++;
                }
            }

            int uvIndex = vertexAttributeIndex(ATTRIBUTE_TYPE_TEXCOORD);
            if(uvIndex != -1) {
                if (line.find("vt ") != std::string::npos) {
                    std::vector<std::string> parts = split(line, " ");
                    Vector2 uv = {std::stof(parts[1]), std::stof(parts[2])};

                    uvs.push_back(uv);

                    attributeCount[uvIndex]++;
                }
            }

            if(line.find("f ") != std::string::npos) {
                List<std::string> parts = split(line, " ");

                for(int i = 1; i < parts.size(); i++) {
                    std::vector<std::string> ind = split(parts[i], "/");

                    Index index = 0;
                    if(ind.size() > 0 && !ind[0].empty())
                        index = std::stoi(ind[0]) - 1;

                    Index uv = 0;
                    if(ind.size() > 1 && !ind[1].empty())
                        uv = std::stoi(ind[1]) - 1;

                    Index normal = 0;
                    if(ind.size() > 2 && !ind[2].empty())
                        normal = std::stoi(ind[2]) - 1;

                    indices.push_back({index, uv, normal});
                }
            }
        }

        for(auto& index : indices) {
            Vertex vertex{};

            int positionIndex = vertexAttributeIndex(ATTRIBUTE_TYPE_POSITION);
            if(positionIndex != -1 && attributeCount[positionIndex] > 0) {
                //Memcpy the position
                int offset = attributes[positionIndex].offset;
                memcpy(&vertex + offset, &vertices[index.index], sizeof(Vector3));
            }
            int normalIndex = vertexAttributeIndex(ATTRIBUTE_TYPE_NORMAL);
            if(normalIndex != -1 && attributeCount[normalIndex] > 0) {
                //Memcpy the normal
                size_t offset = attributes[normalIndex].offset;
                memcpy(((char*)&vertex) + offset, &normals[index.normalIndex], sizeof(Vector3));
            }
            int uvIndex = vertexAttributeIndex(ATTRIBUTE_TYPE_TEXCOORD);
            if(uvIndex != -1 && attributeCount[uvIndex] > 0) {
                //Memcpy the uv
                size_t offset = attributes[uvIndex].offset;
                memcpy(((char*)&vertex) + offset, &uvs[index.uvIndex], sizeof(Vector2));
            }
            _vertices.push_back(vertex);
        }

        for(int i = 0; i < _vertices.size(); i++) {
            _indices.push_back(i);
        }

        //Merge vertices
        for(int i = 0; i < _vertices.size(); i++) {
            for(int j = i + 1; j < _vertices.size(); j++) {
                if(_vertices[i] == _vertices[j]) {
                    _indices[j] = _indices[i];
                }
            }
        }

        file.close();
    }
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

            switch (attributes[i].format) {
                case ATTRIBUTE_FORMAT_FLOAT:
                    size = 1;
                    type = GL_FLOAT;
                    break;
                case ATTRIBUTE_FORMAT_FLOAT2:
                    size = 2;
                    type = GL_FLOAT;
                    break;
                case ATTRIBUTE_FORMAT_FLOAT3:
                    size = 3;
                    type = GL_FLOAT;
                    break;
                case ATTRIBUTE_FORMAT_FLOAT4:
                    size = 4;
                    type = GL_FLOAT;
                    break;
                case ATTRIBUTE_FORMAT_INT:
                    size = 1;
                    type = GL_INT;
                    break;
                case ATTRIBUTE_FORMAT_INT2:
                    size = 2;
                    type = GL_INT;
                    break;
                case ATTRIBUTE_FORMAT_INT3:
                    size = 3;
                    type = GL_INT;
                    break;
                case ATTRIBUTE_FORMAT_INT4:
                    size = 4;
                    type = GL_INT;
                    break;
                case ATTRIBUTE_FORMAT_UNSIGNED_INT:
                    size = 1;
                    type = GL_UNSIGNED_INT;
                    break;
                case ATTRIBUTE_FORMAT_UNSIGNED_INT2:
                    size = 2;
                    type = GL_UNSIGNED_INT;
                    break;
                case ATTRIBUTE_FORMAT_UNSIGNED_INT3:
                    size = 3;
                    type = GL_UNSIGNED_INT;
                    break;
                case ATTRIBUTE_FORMAT_UNSIGNED_INT4:
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
        _vertices = std::vector<Vertex>();
        _indices = std::vector<Index>();
        this->apply();
    }

    Mesh(std::vector<Vertex> vertices, std::vector<Index> indices) {
        _vertices = vertices;
        _indices = indices;
        this->apply();
    }

    Mesh(std::string path = "", std::string name = "") : MeshBase(path, name) {
        _vertices = std::vector<Vertex>();
        _indices = std::vector<Index>();
        loadOBJ(path);
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
