#ifndef POMEGRANATEENGINE_MODEL2D_H
#define POMEGRANATEENGINE_MODEL2D_H
#include <vector>
#include <vector3.h>
#include <vertex2d.h>
#include <glad/glad.h>
#include <serializable.h>
#include <graphics_core.h>

class Model2D : public Serializable {
private:
    std::vector<Vertex2D> _vertices = {};
    std::vector<uint> _indices = {};
    uint _VAO = 0, _VBO = 0, _EBO = 0;
    int _buildVertexCount = 0;
    int _buildIndexCount = 0;
public:
    Model2D();
    ~Model2D();
    [[nodiscard]] std::vector<Vertex2D>& getVertices();
    [[nodiscard]] std::vector<uint>& getIndices();
    void setVertices(const std::vector<Vertex2D>& vertices);
    void setIndices(const std::vector<uint>& indices);
    void addVertex(const Vertex2D& vertex);
    void addIndex(uint index);
    void regenerateBuffers();
    void draw() const;

    void serialize(Archive& a) const override;
    void deserialize(Archive& a) override;
};

#endif //POMEGRANATEENGINE_MODEL2D_H
