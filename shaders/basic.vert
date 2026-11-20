#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 model;       // set per object (Table / Cards set this themselves)
uniform mat4 world;       // set once per group: lays the old XY table on the XZ floor
uniform mat4 view;        // set once per frame by the active camera
uniform mat4 projection;  // set once per frame

void main()
{
    gl_Position = projection * view * world * model * vec4(aPos, 1.0);
    TexCoord = aTexCoord;
}
