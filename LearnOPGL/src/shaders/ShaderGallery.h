#pragma once

struct ShaderPaths {
    const char* vert;
    const char* frag;
};

struct ShaderGallery {
    static constexpr ShaderPaths basic = {
        "basic/basic-vs.glsl",
        "basic/basic-fs.glsl"
    };
};