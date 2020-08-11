#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UniformBufferObject {
    mat4 pvm;
    mat4 projection;
    mat4 view;
    mat4 model;
} ubo;

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec3 inColor;

layout(location = 0) out vec3 fragColor;

void main() {
    gl_Position = ubo.pvm * vec4(inPosition, 0.0, 1.0);
    fragColor = inColor;
}