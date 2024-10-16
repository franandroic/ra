#include "../headers/Mesh.h"

Mesh::Mesh(aiMesh *mesh) {

    if (!mesh) {
        std::cerr << "Error: mesh is null" << std::endl;
        return;
    }

    for (int i = 0; i < mesh->mNumVertices; i++) {
        vertices.push_back(glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z));
    }

    for (int i = 0; i < mesh->mNumFaces; i++) {
        for (int j = 0; j < mesh->mFaces[i].mNumIndices; j++) {
            indices.push_back(mesh->mFaces[i].mIndices[j]);
        }
    }

    if (mesh->HasNormals()) {
        for (int i = 0; i < mesh->mNumVertices; i++) {
            normals.push_back(glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z));
        }
    }

    if (mesh->HasTextureCoords(0)) {
        for (int i = 0; i < mesh->mNumVertices; i++) {
            uvCoords.push_back(glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y));
        }
    }

    //generiranje spremnika
	glGenVertexArrays(1, &(VAO));
	glGenBuffers(3, VBO);
	glGenBuffers(1, &(EBO));

};

void Mesh::applyTransform(glm::mat4 mat) {

    glm::vec4 tempVector;

    for (int i = 0; i < vertices.size(); i++) {
        tempVector = glm::vec4(vertices[i].x, vertices[i].y, vertices[i].z, 1.0);
        tempVector = mat * tempVector;
        vertices[i] = glm::vec3(tempVector.x, tempVector.y, tempVector.z);
    }
}

std::pair<glm::vec3, glm::vec3> Mesh::getBoundingBox() {

    glm::vec3 minCoords = glm::vec3(vertices[0].x, vertices[0].y, vertices[0].z);
    glm::vec3 maxCoords = glm::vec3(vertices[0].x, vertices[0].y, vertices[0].z);

    for (int i = 0; i < vertices.size(); i++) {
        if (vertices[i].x < minCoords.x) minCoords.x = vertices[i].x;
        else if (vertices[i].x > maxCoords.x) maxCoords.x = vertices[i].x;
        if (vertices[i].y < minCoords.y) minCoords.y = vertices[i].y;
        else if (vertices[i].y > maxCoords.y) maxCoords.y = vertices[i].y;
        if (vertices[i].z < minCoords.z) minCoords.z = vertices[i].z;
        else if (vertices[i].z > maxCoords.z) maxCoords.z = vertices[i].z;
    }

    return std::pair<glm::vec3, glm::vec3>(minCoords, maxCoords);
}

void Mesh::normalize() {

    std::pair<glm::vec3, glm::vec3> bounds = getBoundingBox();

    glm::vec3 center = glm::vec3((bounds.first.x + bounds.second.x) / 2, (bounds.first.y + bounds.second.y) / 2, (bounds.first.z + bounds.second.z) / 2);
    float range = std::max((bounds.second.x - bounds.first.x), (bounds.second.y - bounds.first.y));
    range = std::max(range, (bounds.second.z - bounds.first.z));

    glm::mat4 transform_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(2.0f / range, 2.0f / range, 2.0f / range)) * glm::translate(glm::mat4(1.0f), -center);

    applyTransform(transform_matrix);

    draw();
}

void Mesh::draw() {

    //priprema za crtanje
	glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &(vertices[0]), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
		glEnableVertexAttribArray(0);

        if (normals.size() > 0) {
            glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
            glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &(normals[0]), GL_STATIC_DRAW);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
            glEnableVertexAttribArray(1);
        }

        if (uvCoords.size() > 0) {
            glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
            glBufferData(GL_ARRAY_BUFFER, uvCoords.size() * sizeof(glm::vec2), &(uvCoords[0]), GL_STATIC_DRAW);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *) 0);
            glEnableVertexAttribArray(2);
        }

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), &(indices[0]), GL_STATIC_DRAW);
	
	glBindVertexArray(0);
}