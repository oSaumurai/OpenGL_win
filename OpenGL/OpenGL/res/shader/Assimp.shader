#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoords;

//layout(location = 1) in vec3 aNormal;
//layout(location = 2) in vec2 aTexCoords;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 u_MVP;

void main()
{
    TexCoords = aTexCoords;
    gl_Position = u_MVP * vec4(aPos, 1.0);
}

#shader fragment
#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D u_Texture;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_diffuse1;

void main()
{
    FragColor = texture(u_Texture, TexCoords);
    //FragColor = texture(texture_diffuse1, TexCoords);
}