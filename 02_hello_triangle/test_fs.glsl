#version 410

in vec3 colour;
in vec2 texcoord;
in vec3 normal;
in vec3 FragPos;

out vec4 frag_colour;

uniform sampler2D ourTexture;
uniform vec3 lightPos; 

void main() {
	vec3 lightcolor = vec3(1.0,1.0,1.0);
	

    float ambientStrength = 0.3;
    vec3 ambient = ambientStrength * lightcolor;

	vec3 diffusecolor = vec3(1.0,1.0,1.0);  //multiply by texture
	vec3 specularcolor = vec3(1.0,1.0,1.0);

	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(lightPos - FragPos);  

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightcolor;

	vec3 result = (ambient + diffuse);
	//vec3 result = (ambient + diffuse)* colour;

	frag_colour = texture(ourTexture, texcoord) * vec4 (result, 1.0);
	//frag_colour = texture(ourTexture, texcoord);
}
