// Just exploring some ideas, nothing used here yet for the main application.

#include "ShaderLibrary.h"

ShaderLibrary& ShaderLibrary::get() {
    static ShaderLibrary instance;
    return instance;
}

ShaderLibrary::ShaderLibrary() {
    registry["basic"] = {
        "src/shaders/basic/basic.vert",
        "src/shaders/basic/basic.frag"
    };
}

const ShaderLibrary::ShaderPaths& ShaderLibrary::getPaths(const std::string& name) {
    return registry.at(name);
}