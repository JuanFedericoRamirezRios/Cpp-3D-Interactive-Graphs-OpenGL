/*
Standard C++ 20
GLEW 2.3.1
GLFW 3.4
GLM 1.0.3
*/

// ******* This is a Fragment Shader file **********

#version 450 core // GLSL version -> 4.50 OpenGL version

in vec2 outTextureCoord; // Out from Assets/Shaders/LIT_TEXTURE_MODEL.vs
in vec3 outNormalVector; // Out from Assets/Shaders/LIT_TEXTURE_MODEL.fs
in vec3 outFragmentWorldPos; // Out from Assets/Shaders/LIT_TEXTURE_MODEL.fs. Position of the pixel in the world: world x localPosition

uniform vec3 cameraPos;
uniform vec3 lightPos;
uniform vec3 lightColor;

uniform float specularStrength;
uniform float ambientStrength;

// texture
uniform sampler2D uTexture; // sampler2D: sample the texture <- wrapping and filtering in TEXTURE.hpp

out vec4 color;

void main(){
    vec3 norm = normalize(outNormalVector);
    vec4 objColor = texture(uTexture, outTextureCoord);

    // ambient
    vec3 ambient = ambientStrength * lightColor;

    // diffuse
    vec3 lightDir = normalize(lightPos - outFragmentWorldPos); 
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // specular
    vec3 viewDir = normalize(cameraPos - outFragmentWorldPos);
    vec3 reflectionDir = reflect(-lightDir, norm); // reflect is a GLSL intrinsic function.
    float spec = pow(max(dot(viewDir, reflectionDir),0.0),128); // The value can be from 0 to 256. 
    vec3 specular = specularStrength * spec * lightColor;

    // lighting shading calculation
    vec3 totalColor = (ambient + diffuse + specular) * objColor.rgb;

    color = vec4(totalColor, 1.0f);
}