// Just exploring some ideas, nothing used here yet for the main application.

#pragma once

#include <unordered_map>
#include <string>

class ShaderLibrary {
public:
    struct ShaderPaths {
        std::string vert;
        std::string frag;
    };

    static ShaderLibrary& get();
    const ShaderPaths& getPaths(const std::string& name);

    ShaderLibrary(const ShaderLibrary&) = delete;
    ShaderLibrary& operator=(const ShaderLibrary&) = delete;

private:
    ShaderLibrary();
    std::unordered_map<std::string, ShaderPaths> registry;
};