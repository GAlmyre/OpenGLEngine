#version 330 core
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

out vec4 FragColor;

uniform vec3 viewPos;

struct Light {
	vec3 position;
	vec3 color;
	// uniforms to compute light's falloff
	float constant, linear, quadratic, intensity;
};

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    float shininess;
}; 
  
uniform Material material;
uniform Light light;

vec3 phong()
{

	float distance    = length(light.position - FragPos);
	float attenuation = light.intensity / (light.constant + light.linear * distance + light.quadratic * (distance * distance));   

    // ambient
    vec3 ambient = light.color * texture(material.texture_diffuse1, TexCoords).rgb * attenuation;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.color * diff * texture(material.texture_diffuse1, TexCoords).rgb * attenuation;  
    
    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.color * spec * texture(material.texture_specular1, TexCoords).rgb * attenuation;  
        
    return ambient + diffuse + specular;
}

void main()
{
	vec3 result = phong();

    FragColor = vec4(result, 1.0);
}	