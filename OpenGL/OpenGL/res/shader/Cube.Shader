#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normals;
layout(location = 2) in vec2 texCoord;

out vec2 v_TexCoord;
out vec3 v_normals;

uniform mat4 u_MVP;

void main()
{
    v_normals = normals;
    gl_Position = u_MVP * vec4(position, 1.0);
    v_TexCoord = texCoord;
};


#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in vec3 v_normals;

uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main()
{
    vec3 normal = normalize(v_normals);

    vec4 texColor = texture(u_Texture, v_TexCoord);
    color = texColor;
};