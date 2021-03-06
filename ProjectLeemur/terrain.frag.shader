#version 330 core

uniform struct LightSource {
	vec4 position;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec4 intensity;

	vec3 spotConeDirection;
	float spotCutoff;
	float spotExponent;

	float attenuationConstant;
	int type;
} licht;

uniform struct Material {
	vec4 emission;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;

	float shiny;
} grass, sand, snow, water;

uniform vec3 CameraPosition;
uniform mat3 NormalMatrix;
uniform mat4 model;

uniform samplerCube skybox;
uniform float ratio = 1.0 / 1.52;

in vec3 FragmentPosition;
in vec3 FragmentNormal;

out vec4 color;

void main() {
	vec3 position = FragmentPosition;
	Material material = grass;
	if (position.y > 7.6) material = snow;
	else if (position.y < 4 && position.y >= 2.2) material = sand;
	else if (position.y < 2.2) material = water;
	

	vec3 normal = normalize(NormalMatrix * FragmentNormal);
	vec3 surface = (model * vec4(position, 1.0f)).xyz;
	vec3 surfaceToCamera = normalize(CameraPosition - surface);
	
	vec3 intensity = licht.intensity.rgb;
	vec3 surfaceToLight;
	float attenuation;
	
	if (licht.type == 0) {
		// directional light
		surfaceToLight = normalize(licht.position.xyz);
		attenuation = 1.0;
	}
	else
	{
		// Point lights and Spotlights:
		float k = licht.attenuationConstant;
		float distToLight = length(licht.position.xyz - surface);
		surfaceToLight = normalize(licht.position.xyz - surface);
		attenuation = 1.0 / (1.0 + (k * distToLight * distToLight));

		if (licht.type == 2) {
			// is-a Spotlight
			
			// Get angle from point on surface to light with normal vector
			float angle = degrees(acos(
				dot(-surfaceToLight, normalize(licht.spotConeDirection))));
			
			attenuation = angle <= licht.spotCutoff
							? pow(attenuation, licht.spotExponent)
							: 0.0;

			// Blur the spotlight's cufoff edges
			float outerCutoff = licht.spotCutoff + (licht.spotCutoff / 20.0f);
			float epsilon = -licht.spotCutoff + (licht.spotCutoff / 20.0f);
			intensity *= clamp((angle - outerCutoff) / epsilon, 0.0, 1.0);
		}
	}
	
	float brightness = max(0.0, dot(normal, surfaceToLight));
	float specularity = 0.0;
	if (brightness > 0.0) {
		vec3 reflection = reflect(-surfaceToLight, normal);
		specularity = max(0.0, dot(surfaceToCamera, reflection));
		specularity = pow(specularity, material.shiny);
	}

    //Discretized Shading
    if (brightness < .25)        brightness = .25;
    else if (brightness < .5)    brightness = .5;
    else if (brightness < .75)   brightness = .75;

    if (specularity < .125)       specularity = .00;
    else if (specularity < .25)   specularity = .25;
    else if (specularity < .5)    specularity = .5;
    else if (specularity < .75)   specularity = .75;

	// Breakup into components and add them all together to form the final object color
	vec3 specular = specularity * intensity * vec3(material.specular) ;
	vec3 diffuse = brightness * intensity * vec3(material.diffuse) ;
	vec3 ambient = vec3(material.ambient);

	vec3 linearColor = ambient + attenuation * (diffuse + specular);
	vec3 gamma = vec3(1.0 / 2.2);
    color = vec4(pow(linearColor, gamma), 1.0);

    //Silhouette Edges
    float edge = max(0, dot(surfaceToLight, normal));
    if (edge < 0.01) {
        // in place of 1D texture to define edge ramp
        // use linear scaling for smoother transition
        if (edge > 0) {
            color = vec4(vec3(20 * edge), 1.f);
        }
        else
            color = vec4(0,0,0,1);
    }
}