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
} material;

in vec3 FragmentPosition;
in vec3 FragmentNormal;

uniform samplerCube Skybox;
uniform vec3 CameraPosition;
uniform mat3 NormalMatrix;
uniform mat4 model;

out vec4 color;

void main() {
	vec3 normal = normalize(NormalMatrix * FragmentNormal);
	vec3 surface = (model * vec4(FragmentPosition, 1.0f)).xyz;
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
		specularity = pow(specularity, 3.0);
	}

	// Breakup into components and add them all together to form the final object color
	vec3 specular = specularity * intensity * vec3(material.specular) ;
	vec3 diffuse = brightness * intensity * vec3(material.diffuse) ;
	vec3 ambient = vec3(material.ambient);

	vec3 linearColor = ambient + attenuation * (diffuse + specular);
	vec3 gamma = vec3(1.0 / 2.2);
    color = vec4(pow(linearColor, gamma), 1.0);
}
