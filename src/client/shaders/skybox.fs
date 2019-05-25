#version 330 core

in vec3 TexCoords;

out vec4 FragColor;

uniform samplerCube skybox;
uniform vec3 fogColour;

const float lowerLimit = 0.0;
const float upperLimit = 1.0;

void main()
{    
    vec4 FinalColour = texture(skybox, TexCoords);

    float factor = (TexCoords.y - lowerLimit) / (upperLimit - lowerLimit);
    factor = clamp(factor,0.0,1.0);
    FragColor = mix(vec4(fogColour,1.0),FinalColour,factor);
}