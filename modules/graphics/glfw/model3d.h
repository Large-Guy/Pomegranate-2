#ifndef POMEGRANATEENGINE_MODEL3D_H
#define POMEGRANATEENGINE_MODEL3D_H
#include <vector>
#include <math/vector3.h>
#include <graphics/glfw/vertex3d.h>
#include <glad/glad.h>
#include <core/serializable.h>

class Model3D : public Serializable {
private:
    std::vector<Vertex3D> _vertices = {};
    std::vector<uint> _indices = {};
    uint _VAO = 0, _VBO = 0, _EBO = 0;
public:
    Model3D();
    ~Model3D();
    [[nodiscard]] std::vector<Vertex3D> getVertices() const;
    [[nodiscard]] std::vector<uint> getIndices() const;
    void setVertices(const std::vector<Vertex3D>& vertices);
    void setIndices(const std::vector<uint>& indices);
    void addVertex(const Vertex3D& vertex);
    void addIndex(uint index);
    void regenerateBuffers();
    void draw() const;

    void serialize(Archive& a) const override;
    void deserialize(Archive& a) override;
};


#endif //POMEGRANATEENGINE_MODEL3D_H
