#version 450 core
layout (location = 0) in vec4 vertex;
uniform mat4 orthoProjection;

out vec2 textureCoords;

void main() {
    gl_Position = orthoProjection * vec4(vertex.xy, 0.0, 1.0);
    textureCoords = vertex.zw;
}