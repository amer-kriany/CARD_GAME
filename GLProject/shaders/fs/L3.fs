in vec2 TexCoord;

uniform sampler2D texture1;
uniform bool useTexture;
uniform vec3 objectColor;

out vec4 FragColor;

void main()
{
    if (useTexture)
    {
        FragColor = texture(texture1, TexCoord);
    }
    else
    {
        FragColor = vec4(objectColor, 1.0);
    }
}