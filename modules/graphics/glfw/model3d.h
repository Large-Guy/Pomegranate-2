#ifndef POMEGRANATEENGINE_MODEL_H
#define POMEGRANATEENGINE_MODEL_H
#include <vector>
#include <vector3.h>
#include <vertex3d.h>
#include <glad/glad.h>

class Model3D {
private:
    std::vector<Vertex3D> _vertices;
    std::vector<uint> _indices;
    uint _VAO, _VBO, _EBO;
public:
    Model3D();
    [[nodiscard]] std::vector<Vertex3D> getVertices() const;
    [[nodiscard]] std::vector<uint> getIndices() const;
    void setVertices(const std::vector<Vertex3D>& vertices);
    void setIndices(const std::vector<uint>& indices);
    void addVertex(const Vertex3D& vertex);
    void addIndex(uint index);
    void regenerateBuffers();
    void draw() const;
};


#endif //POMEGRANATEENGINE_MODEL_H
