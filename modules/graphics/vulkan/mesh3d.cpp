#include "mesh3d.h"

Mesh3D::Mesh3D() {
    vertices = {};
    indices = {};
    this->_vertexBuffer = nullptr;
    this->_indexBuffer = nullptr;
}

Mesh3D::Mesh3D(List<Vertex3D> vertices, List<uint16_t> indices, Shader<Vertex3D>* shader)
{
    this->vertices = vertices;
    this->indices = indices;
    this->shader = shader;
    this->_vertexBuffer = nullptr;
    this->_indexBuffer = nullptr;
    apply();
}

void Mesh3D::apply() {
    _vertexBuffer = new Buffer<Vertex3D,BUFFER_TYPE_VERTEX>(vertices);
    _indexBuffer = new Buffer<uint16_t, BUFFER_TYPE_INDEX>(indices);
}
