#ifndef POMEGRANATEENGINE_MESH3D_H
#define POMEGRANATEENGINE_MESH3D_H
#include "shader.h"
#include "buffer.h"
#include "vertex3d.h"

struct Mesh3D {
private:
    BufferBase<BUFFER_TYPE_VERTEX>* _vertexBuffer;
    BufferBase<BUFFER_TYPE_INDEX>* _indexBuffer;
public:
    List<Vertex3D> vertices;
    List<uint16_t> indices;
    Shader<Vertex3D>* shader;
    Mesh3D();
    Mesh3D(List<Vertex3D> vertices, List<uint16_t> indices, Shader<Vertex3D>* shader);
    void apply();
    friend Window;
};


#endif //POMEGRANATEENGINE_MESH3D_H
