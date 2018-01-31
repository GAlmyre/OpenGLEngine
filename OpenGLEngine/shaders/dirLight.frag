#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gColorSpec;

uniform vec3 viewPos;
uniform float shininess;

struct Light {
	vec3 direction;
	vec3 color;
	float intensity;
};

uniform Light light;

vec3 phong()
{  
	// retrieve frag position, normal and colors
	vec3 FragPos = texture(gPosition, TexCoords).rgb;
	vec3 Normal = texture(gNormal, TexCoords).rgb;
	vec3 Diffuse = texture(gColorSpec, TexCoords).rgb;
	float Specular = texture(gColorSpec, TexCoords).a;

    // ambient
    vec3 ambient = light.color * Diffuse;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.color * diff * Diffuse;  
    
    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = light.color * spec * Specular;  
      
    return ambient + diffuse + specular;
}

void main()
{
	vec3 result = light.intensity*phong();

    FragColor = vec4(result, 1.0);
}	