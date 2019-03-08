#version 330 core
out vec4 FragColor;

uniform vec3 lightColor;
uniform vec3 lightPos; 

in vec3 FragPos;  
in vec2 TexCoords;
in vec3 Normal;

uniform sampler2D texture_diffuse1;

void main()
{
	float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(normalize(Normal), lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 result = (ambient + diffuse) * texture(texture_diffuse1, TexCoords).rgb;
    FragColor = vec4(result, 1.0f);
}