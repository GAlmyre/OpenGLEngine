#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gColorSpec;

uniform vec3 viewPos;

struct Light {
	vec3 position;
	vec3 color;
	// uniforms to compute light's falloff
	float constant, linear, quadratic, intensity;
};

uniform float shininess;

uniform Light light;

vec3 phong()
{

	// retrieve frag position, normal and colors
	vec3 FragPos = texture(gPosition, TexCoords).rgb;
	vec3 Normal = texture(gNormal, TexCoords).rgb;
	vec3 Diffuse = texture(gColorSpec, TexCoords).rgb;
	float Specular = texture(gColorSpec, TexCoords).a;

	float distance    = length(light.position - FragPos);
	float attenuation = light.intensity / (light.constant + light.linear * distance + light.quadratic * (distance * distance));   

    // ambient
    vec3 ambient = light.color * Diffuse * attenuation;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.color * diff * Diffuse * attenuation;  
    
    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = light.color * spec * Specular * attenuation;  
        
    return ambient + diffuse + specular;
}

void main()
{
	vec3 result = phong();

    FragColor = vec4(result, 1.0);
}	