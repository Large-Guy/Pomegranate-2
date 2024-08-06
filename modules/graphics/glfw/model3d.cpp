#include "model3d.h"

Model3D::Model3D() {
    this->_vertices = std::vector<Vertex3D>();
    this->_indices = std::vector<unsigned int>();
    this->_VAO = 0;
    this->_VBO = 0;
    this->_EBO = 0;
}

Model3D::~Model3D() {
    if(this->_VAO != 0) {
        glDeleteVertexArrays(1, &this->_VAO);
        glDeleteBuffers(1, &this->_VBO);
        glDeleteBuffers(1, &this->_EBO);
    }
}

std::vector<Vertex3D> Model3D::getVertices() const {
    return this->_vertices;
}

std::vector<unsigned int> Model3D::getIndices() const {
    return this->_indices;
}

void Model3D::setVertices(const std::vector<Vertex3D>& vertices) {
    this->_vertices = vertices;
}

void Model3D::setIndices(const std::vector<unsigned int>& indices) {
    this->_indices = indices;
}

void Model3D::addVertex(const Vertex3D &vertex) {
    this->_vertices.push_back(vertex);
}

void Model3D::addIndex(uint index) {
    this->_indices.push_back(index);
}

void Model3D::regenerateBuffers() {
    std::vector<float> vertices;
    for (auto& vertex : this->_vertices) {
        vertices.push_back(vertex.position.x);
        vertices.push_back(vertex.position.y);
        vertices.push_back(vertex.position.z);
    }

    glGenVertexArrays(1, &this->_VAO);
    glGenBuffers(1, &this->_VBO);
    glGenBuffers(1, &this->_EBO);

    glBindVertexArray(this->_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, this->_VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->_indices.size() * sizeof(unsigned int), this->_indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Model3D::draw() const {
    glBindVertexArray(this->_VAO);
    glDrawElements(GL_TRIANGLES, this->_indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Model3D::serialize(Archive &a) const {
    a << this->_vertices.size();
    for (auto& vertex : this->_vertices) {
        a << vertex;
    }
    a << this->_indices.size();
    for (auto& index : this->_indices) {
        a << index;
    }
}

void Model3D::deserialize(Archive &a) {
    unsigned long size;
    a >> &size;
    for (unsigned int i = 0; i < size; i++) {
        Vertex3D vertex;
        a >> &vertex;
        this->_vertices.push_back(vertex);
    }
    a >> &size;
    for (unsigned int i = 0; i < size; i++) {
        unsigned int index;
        a >> &index;
        this->_indices.push_back(index);
    }
    this->regenerateBuffers();
}
