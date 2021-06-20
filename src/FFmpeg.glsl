#type vertex

version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;
out vec3 ourColor;
out vec2 texCoord;
void main()
{
    gl_Position=vec4(aPos, 1.0);
    texCoord = aTexCoord;
    ourColor = aColor;
};

#type fragment
#version 330 core
out vec4 FragColor;   
in vec2 texCoord;
in vec3 ourColor;
uniform sampler2D image;
void main()
{
    vec4 texData = texture(image, texCoord);
    float Y = texData.x;
    float U = texData.y;
    float V = texData.z;
    float R = 1.164 * (Y - 0.0625) + 1.596 * (V - 0.5);
    float G = 1.164 * (Y - 0.0625) - 0.813 * (V - 0.5) - 0.391 * (U - 0.5);
    float B = 1.164 * (Y - 0.0625) + 2.018 * (U - 0.5);
    FragColor = vec4(R, G, B, 0.5f);
};