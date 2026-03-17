//renderer.h
#pragma once
#include "pch.h"
#include "./geometry.h"
#include "./vertex_layout.h"

enum class DrawMode { Arrays, Indexed };

struct MeshBuffers
{
    unsigned int VAO = 0;
    unsigned int VBO = 0;
    unsigned int EBO = 0;                   // 0 means no EBO — use glDrawArrays
    unsigned int indexCount = 0;            // used when EBO exists
    unsigned int vertexCount = 0;           // used when no EBO
    DrawMode drawMode = DrawMode::Arrays;   // <-- baked in at setup

    bool hasEBO() const { return EBO != 0; }

    void destroy()
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        if (hasEBO()) glDeleteBuffers(1, &EBO);
    }
};

// Upload a GeometryData to the GPU and return its handles.
// Layout defines the vertex layout of said GeometryData.
MeshBuffers setupMesh(const GeometryData& geoData, const VertexLayout& layout ) {

    MeshBuffers mesh;
    mesh.vertexCount = static_cast<unsigned int>((geoData.vertices.size() * sizeof(float)) / layout.stride);

    glGenVertexArrays(1, &mesh.VAO);		// reserve a VAO ID
    glGenBuffers(1, &mesh.VBO);			// reserve a VBO ID

    glBindVertexArray(mesh.VAO);			// start recording into VAO

    glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);	// select VBO as the active buffer
    glBufferData(GL_ARRAY_BUFFER, geoData.vertices.size() * sizeof(float), geoData.vertices.data(), GL_STATIC_DRAW);		// upload vertex data to GPU

    if (!geoData.indices.empty())
    {
        mesh.drawMode = DrawMode::Indexed;
        mesh.indexCount = static_cast<unsigned int>(geoData.indices.size());
        glGenBuffers(1, &mesh.EBO);			// reserve an EBO ID
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);	// select EBO as the active buffer
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, geoData.indices.size() * sizeof(unsigned int), geoData.indices.data(), GL_STATIC_DRAW);	// upload vertex data to GPU
    }

    //glVertexAttribPointer (SLOT, SIZE, TYPE, NORMALIZED, STRIDE, OFFSET) <<< this is how we tell OpenGL how to interpret the vertex data we just uploaded. We have to do this for each attribute in our vertex data (position, color, texture coords)
    
    // This handles vertex attribute pointers from the defined layout provided
    for (const VertexAttribute& va : layout.vertAttributes) {
        glVertexAttribPointer(va.slot, va.size, GL_FLOAT, GL_FALSE, layout.stride, (void*)(uintptr_t)va.offset);
        glEnableVertexAttribArray(va.slot);
    }
    
    glBindVertexArray(0);			// stop recording, VAO is saved DONT NEED TO PUT IN RENDER LOOP LIKE BEFORE, ITS UNNECESSARY TO BIND/UNBIND EVERY FRAME. When changing VAO VBO EBOS etc they are overwritten, so this is not needed.
    return mesh;
}

void drawMesh(const MeshBuffers& mesh)
{
    glBindVertexArray(mesh.VAO);
    if (mesh.drawMode == DrawMode::Indexed) //now is reading stored intent rather than inferring from a null check, not too big increase but whatever.
        glDrawElements(GL_TRIANGLES, mesh.indexCount, GL_UNSIGNED_INT, 0);
    else
        glDrawArrays(GL_TRIANGLES, 0, mesh.vertexCount);
}

void initGLStates() {
    //OpenGL State Management
    glEnable(GL_DEPTH_TEST);	// enable depth testing for correct z-ordering	
    glEnable(GL_CULL_FACE);		// enable culling
    glCullFace(GL_BACK);		// cull back faces //glCullFace(GL_FRONT); to see front face culling & test!
    glFrontFace(GL_CCW);		// define front face as counter-clockwise (this is default)
}

//To be deleted. expected vertex layout: vec3 pos | vec3 normal (how many floats? 6 floats)
MeshBuffers setupMesh33(const GeometryData& geoData) {

    MeshBuffers mesh;
    mesh.vertexCount = static_cast<unsigned int>(geoData.vertices.size()) / 6; // 6 floats per vertex

    glGenVertexArrays(1, &mesh.VAO);		// reserve a VAO ID
    glGenBuffers(1, &mesh.VBO);			// reserve a VBO ID

    glBindVertexArray(mesh.VAO);			// start recording into VAO

    glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);	// select VBO as the active buffer
    glBufferData(GL_ARRAY_BUFFER, geoData.vertices.size() * sizeof(float), geoData.vertices.data(), GL_STATIC_DRAW);		// upload vertex data to GPU

    if (!geoData.indices.empty())
    {
        mesh.drawMode = DrawMode::Indexed;
        mesh.indexCount = static_cast<unsigned int>(geoData.indices.size());
        glGenBuffers(1, &mesh.EBO);			// reserve an EBO ID
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);	// select EBO as the active buffer
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, geoData.indices.size() * sizeof(unsigned int), geoData.indices.data(), GL_STATIC_DRAW);	// upload vertex data to GPU
    }

    //glVertexAttribPointer (SLOT, SIZE, TYPE, NORMALIZED, STRIDE, OFFSET) <<< this is how we tell OpenGL how to interpret the vertex data we just uploaded. We have to do this for each attribute in our vertex data (position, color, texture coords)
    // Slot 0: position (vec3)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);	// POSITION layout: slot 0, 3 floats, stride 12 bytes, offset 0
    glEnableVertexAttribArray(0);	// enable attribute slot 0 so the shader can read it
    // Slot 1: normal (vec3)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));	// COLOR layout is now : slot 1, 3 floats, stride 12 bytes, offset 12 bytes (after the position data)
    glEnableVertexAttribArray(1);	// enable attribute slot 1 so the shader can read it

    glBindVertexArray(0);			// stop recording, VAO is saved DONT NEED TO PUT IN RENDER LOOP LIKE BEFORE, ITS UNNECESSARY TO BIND/UNBIND EVERY FRAME. When changing VAO VBO EBOS etc they are overwritten, so this is not needed.
    return mesh;
}