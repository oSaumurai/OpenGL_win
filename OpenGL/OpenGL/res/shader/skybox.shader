#shader vertex
#version 330 core
layout(location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 u_MVP;

void main()
{
    TexCoords = aPos;
    vec4 pos = u_MVP * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
}

#shader fragment
#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;

void main()
{
    const float gamma = 2.6;
    vec3 hdrColor = texture(skybox, TexCoords).rgb;
    vec3 mapped = hdrColor / (hdrColor + vec3(1.0));
    mapped = pow(mapped, vec3(1.0 / gamma));

    //FragColor = vec4(0.0, 0.0, 0.0, 1.0);
    //FragColor.rgb = texture(skybox, TexCoords).rgb;
    FragColor = vec4(mapped, 1.0);
}