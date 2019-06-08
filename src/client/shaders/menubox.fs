#version 330 core

in vec3 TexCoords;

out vec4 FragColor;

uniform samplerCube skybox;
uniform vec3 fogColour;

const float lowerLimit = 0.0;
const float upperLimit = 0.5;

void main()
{    
    vec4 FinalColour = texture(skybox, TexCoords);

    float factor = (TexCoords.y - lowerLimit) / (upperLimit - lowerLimit);
    factor = clamp(factor,0.2,0.5);
    FragColor = mix(vec4(fogColour,0.1),FinalColour,factor);
}