#version 410

in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D scene;
uniform sampler2D bloomBlur;

void main()
{
	//float exposure = 1;
    //const float gamma = 2.2;
    //vec3 hdrColor = texture(scene, TexCoords).rgb;      
    // vec3 bloomColor = texture(bloomBlur, TexCoords).rgb;
    //hdrColor += bloomColor; // additive blending
    // tone mapping
   // vec3 result = vec3(1.0) - exp(-hdrColor * exposure);
    // also gamma correct while we're at it       
    //result = hdrColor; //pow(result, vec3(1.0 / gamma))
    FragColor = mix(texture(scene, TexCoords), texture(bloomBlur, TexCoords), 0.5); //vec4(result, 1.0);

}