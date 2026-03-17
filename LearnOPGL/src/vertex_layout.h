//vertex_layout.h
#pragma once

//EVERY SINGLE SHADER MUST ABIDE BY THE VERTEX SLOT LOCATIONS SET! ELSE THE SHADERS WONT READ THE VALUES!
namespace VertexSlot {
    constexpr int Position = 0;
    constexpr int Color = 1;
    constexpr int UV0 = 2;
    constexpr int Normal = 3;
    //constexpr int Tangent = 4;
    //constexpr int UV1 = 5;
}

struct VertexAttribute {
    int slot;       // shader slot
    int size;       // number of components (2, 3, or 4)
    int offset;     // byte offset into one vertex  <-- make this explicit!!! THIS IS BYTES KEEP CONVENTION!
};

struct VertexLayout {
    std::vector<VertexAttribute> vertAttributes;
    int stride;
};

inline VertexLayout Layout_Pos3_Col3_UV2() {
    return {
        //std::vector<VertexAttribute> vertAttributes;
        {           //slot, size, offset 
            {VertexSlot::Position, 3, 0 },
            {VertexSlot::Color,    3, 12},
            {VertexSlot::UV0,      2, 24},
        },
        //int stride;
        32
    };
}

inline VertexLayout Layout_Pos3_Norm3() {
    return {
        //std::vector<VertexAttribute> vertAttributes;
        {
            {VertexSlot::Position, 3, 0 },
            {VertexSlot::Normal,    3, 12},
        },
        //int stride;
        24
    };
}