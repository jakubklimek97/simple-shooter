#version 330 core
in vec3 ourColor;
in vec2 TexCoord;

out vec4 color;
// Texture samplers
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float gamma;




void main()
{
// Linearly interpolate between both textures (second texture is only slightly combined)
color = (mix(texture(texture1, TexCoord), texture(texture2, TexCoord), gamma) * vec4(ourColor, 1.0));
}