#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;   // only Cube has real normals

out vec2 TexCoord;
out vec3 Normal;

uniform mat4 model;       // set per object (Table / Cards set this themselves)
uniform mat4 world;       // set once per group: lays the old XY table on the XZ floor
uniform mat4 view;        // set once per frame by the active camera
uniform mat4 projection;  // set once per frame

void main()
{
    mat4 placed = world * model;

    gl_Position = projection * view * placed * vec4(aPos, 1.0);
    TexCoord = aTexCoord;

    // Normals must not be stretched with the object, hence inverse-transpose.
    Normal = mat3(transpose(inverse(placed))) * aNormal;
}