#version 450 core

in vec2 textureCoords;

uniform sampler2D texture_;
uniform vec3 colorVals;

out vec4 color;

void main(){
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(texture_, textureCoords).r); // 1.0, 1.0, 1.0: White color. alpha = texture(texture_, textureCoords).r -> Draw only the opaque part of the text
    color = vec4(colorVals, 1.0) * sampled; // Send out color information. White color is replaced with the text.
}