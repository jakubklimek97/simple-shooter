#version 330 core
out vec4 FragColor;

uniform vec3 lightColor;
uniform vec3 lightPos; 
uniform vec3 viewPos;

in vec3 FragPos;  
in vec2 TexCoords;
in vec3 Normal;

struct MaterialProperties {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
	float specularTexture;
};
uniform MaterialProperties material;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

void main()
{
	float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * (texture(texture_diffuse1, TexCoords).rgb + material.ambient); //swiecimy takim kolorem jak tekstura

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor * +(material.diffuse + texture(texture_diffuse1, TexCoords).rgb);

	vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = spec * lightColor * (material.specular + material.specularTexture * texture(texture_specular1, TexCoords).rgb);  

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0f);
}