#include "model2d.h"

Model2D::Model2D() {
    this->_vertices = std::vector<Vertex2D>();
    this->_indices = std::vector<unsigned int>();
    this->_VAO = 0;
    this->_VBO = 0;
    this->_EBO = 0;
    this->_buildIndexCount = 0;
    this->_buildVertexCount = 0;
}

Model2D::~Model2D() {
    if(this->_VAO != 0) {
        glDeleteVertexArrays(1, &this->_VAO);
        glDeleteBuffers(1, &this->_VBO);
        glDeleteBuffers(1, &this->_EBO);
    }
}

std::vector<Vertex2D>& Model2D::getVertices() {
    return this->_vertices;
}

std::vector<unsigned int>& Model2D::getIndices() {
    return this->_indices;
}

void Model2D::setVertices(const std::vector<Vertex2D>& vertices) {
    this->_vertices = vertices;
}

void Model2D::setIndices(const std::vector<unsigned int>& indices) {
    this->_indices = indices;
}

void Model2D::addVertex(const Vertex2D &vertex) {
    this->_vertices.push_back(vertex);
}

void Model2D::addIndex(uint index) {
    this->_indices.push_back(index);
}

void Model2D::regenerateBuffers() {
    //Clear previous buffers
    if(this->_VAO != 0) {
        glDeleteVertexArrays(1, &this->_VAO);
        glDeleteBuffers(1, &this->_VBO);
        glDeleteBuffers(1, &this->_EBO);
    }

    _buildVertexCount = _vertices.size();
    _buildIndexCount = _indices.size();

    std::vector<float> vertices;
    for (auto& vertex : this->_vertices) {
        vertices.push_back(vertex.position.x);
        vertices.push_back(vertex.position.y);

        vertices.push_back(vertex.color.x);
        vertices.push_back(vertex.color.y);
        vertices.push_back(vertex.color.z);

        vertices.push_back(vertex.texCoords.x);
        vertices.push_back(vertex.texCoords.y);
    }

    glGenVertexArrays(1, &this->_VAO);
    glGenBuffers(1, &this->_VBO);
    glGenBuffers(1, &this->_EBO);

    glBindVertexArray(this->_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, this->_VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(unsigned int), _indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0); // Position (stride 7)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(2 * sizeof(float))); // Color (stride 7, offset 2 floats)
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(5 * sizeof(float))); // TexCoords (stride 7, offset 5 floats)
    glEnableVertexAttribArray(2);

    //Shader will look like this
    //layout(location = 0) in vec2 position;
    //layout(location = 1) in vec3 color;
    //layout(location = 2) in vec2 texCoords;

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}



void Model2D::draw() const {
    glBindVertexArray(this->_VAO);
    glDrawElements(GL_TRIANGLES, _buildIndexCount, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

void Model2D::serialize(Archive &a) const {
    a << this->_vertices.size();
    for (auto& vertex : this->_vertices) {
        a << vertex;
    }
    a << this->_indices.size();
    for (auto& index : this->_indices) {
        a << index;
    }
}

void Model2D::deserialize(Archive &a) {
    unsigned long size;
    a >> &size;
    for (unsigned int i = 0; i < size; i++) {
        Vertex2D vertex;
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
