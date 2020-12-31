#version 440

out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos; 

struct PointLight {
    vec3 position;  
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	
    float constant;
    float linear;
    float quadratic;
}; 

uniform PointLight pointLight;

uniform sampler2D texture_diffuse1;
uniform vec3 ambientColor;
uniform vec3 lightColor;
uniform vec3 lightPos; 
uniform vec3 viewPos; 


void main()
{
	float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * pointLight.ambient;



	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos); 

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * pointLight.diffuse;



	float specularStrength = 0.5;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);  

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64);
	vec3 specular = specularStrength * spec * pointLight.specular;



	float distance    = length(pointLight.position - FragPos);
	float attenuation = 1.0 / (pointLight.constant + pointLight.linear * distance + 
    		    pointLight.quadratic * (distance * distance)); 

	ambient  *= attenuation; 
	diffuse  *= attenuation;
	specular *= attenuation; 
	

	vec3 result = (ambient + diffuse + specular);
	FragColor = texture(texture_diffuse1, TexCoord) * vec4(result, 1.0);
}