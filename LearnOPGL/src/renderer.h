#pragma once
#include "pch.h"
#include "./geometry.h"

struct MeshBuffers
{
    unsigned int VAO = 0;
    unsigned int VBO = 0;
    unsigned int EBO = 0;      // 0 means no EBO — use glDrawArrays
    unsigned int indexCount = 0;      // used when EBO exists
    unsigned int vertexCount = 0;      // used when no EBO

    bool hasEBO() const { return EBO != 0; }

    void destroy()
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        if (hasEBO()) glDeleteBuffers(1, &EBO);
    }
};

// Upload a GeometryData to the GPU and return its handles.
// Vertex layout expected: vec3 pos | vec3 color | vec2 uv  (8 floats)
MeshBuffers setupMesh(const GeometryData& geoData) {

    MeshBuffers mesh;
    mesh.vertexCount = static_cast<unsigned int>(geoData.vertices.size()) / 8; // 8 floats per vertex

    glGenVertexArrays(1, &mesh.VAO);		// reserve a VAO ID
    glGenBuffers(1, &mesh.VBO);			// reserve a VBO ID

    glBindVertexArray(mesh.VAO);			// start recording into VAO

    glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);	// select VBO as the active buffer
    glBufferData(GL_ARRAY_BUFFER, geoData.vertices.size() * sizeof(float), geoData.vertices.data(), GL_STATIC_DRAW);		// upload vertex data to GPU

    if (!geoData.indices.empty())
    {
        mesh.indexCount = static_cast<unsigned int>(geoData.indices.size());
        glGenBuffers(1, &mesh.EBO);			// reserve an EBO ID
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);	// select EBO as the active buffer
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, geoData.indices.size() * sizeof(float), geoData.indices.data(), GL_STATIC_DRAW);	// upload vertex data to GPU
    }

    //glVertexAttribPointer (SLOT, SIZE, TYPE, NORMALIZED, STRIDE, OFFSET) <<< this is how we tell OpenGL how to interpret the vertex data we just uploaded. We have to do this for each attribute in our vertex data (position, color, texture coords)
    // Slot 0: position (vec3)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);	// POSITION layout: slot 0, 3 floats, stride 12 bytes, offset 0
    glEnableVertexAttribArray(0);	// enable attribute slot 0 so the shader can read it
    // Slot 1: color (vec3)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));	// COLOR layout is now : slot 1, 3 floats, stride 12 bytes, offset 12 bytes (after the position data)
    glEnableVertexAttribArray(1);	// enable attribute slot 1 so the shader can read it
    // Slot 2: uv (vec2)
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));	// TEXTURE layout is now : slot 2, 2 floats, stride 8 bytes, offset 12 bytes (after the color data)
    glEnableVertexAttribArray(2);	// enable attribute slot 2 so the shader can read it

    glBindVertexArray(0);			// stop recording, VAO is saved DONT NEED TO PUT IN RENDER LOOP LIKE BEFORE, ITS UNNECESSARY TO BIND/UNBIND EVERY FRAME. When changing VAO VBO EBOS etc they are overwritten, so this is not needed.
    return mesh;
}

void drawMesh(const MeshBuffers& mesh)
{
    glBindVertexArray(mesh.VAO);

    if (mesh.hasEBO())
        glDrawElements(GL_TRIANGLES, mesh.indexCount, GL_UNSIGNED_INT, 0);
    else
        glDrawArrays(GL_TRIANGLES, 0, mesh.vertexCount);
}



