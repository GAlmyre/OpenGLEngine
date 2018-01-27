#version 330 core
in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 
  
uniform Material material;

vec3 phong()
{
	vec3 lightDir = normalize(lightPos - FragPos);
	vec3 norm = normalize(Normal);

	// specular
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = material.specular * spec * lightColor;

	// ambient
	vec3 ambient = lightColor * material.ambient;

	// diffuse
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = (diff * material.diffuse) * lightColor;

	return ambient + diffuse + specular;
}

void main()
{
	vec3 result = phong() * objectColor;

    FragColor = vec4(result, 1.0);
}	