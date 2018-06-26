#version 410


in vec2 texcoord;
in vec3 normal;
in vec3 FragPos;

out vec4 frag_colour;

uniform sampler2D ourTexture;
uniform vec3 lightPos; 
uniform vec3 viewPos;

void main() {
	vec3 lightcolor = vec3(1.0,1.0,1.0);
	

    float ambientStrength = 0.3;
    vec3 ambient = ambientStrength * lightcolor;

	vec3 diffusecolor = vec3(1.0,1.0,1.0);  //multiply by texture

	float specularStrength = 0.3;
	vec3 specularcolor = vec3(1.0,1.0,1.0);

	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(lightPos - FragPos);  

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightcolor;

	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);  

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 50);
	vec3 specular = specularStrength * spec * lightcolor;  

	vec3 result = (ambient + diffuse);

	frag_colour = texture(ourTexture, texcoord) * vec4 (result, 1.0) + vec4(specular,1.0);
	//frag_colour = vec4(result,1.0);
}
