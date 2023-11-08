#version 450 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in ATTRIB_OUT {
	vec3 position;
	vec2 texCoords;
} attribIn[];

out ATTRIB_VS_OUT {
	vec2 texCoords;
	vec3 emission;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
} attribOut;

uniform mat4 view;
uniform mat4 modelView;
uniform mat3 normalMatrix;

struct Material {
	vec3 emission;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct UniversalLight {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 position;
	vec3 spotDirection;
};

layout (std140) uniform LightingBlock {
	Material mat;
	UniversalLight lights[3];
	vec3 lightModelAmbient;
	bool useBlinn;
	bool useSpotlight;
	bool useDirect3D;
	bool useTexture;
	float spotExponent;
	float spotOpeningAngle;
};

float computeSpot(in vec3 spotDir, in vec3 lightDir, in vec3 normal)
{
	float cos_delta = cos(radians(spotOpeningAngle));
	// light dir is already normalized
	float cos_gamma = dot(normalize(spotDir), lightDir);

	if (!useDirect3D) { // OpenGL
		if (cos_gamma > cos_delta) // Inside of spotlight cone
			return pow(cos_gamma, spotExponent);
		else // Outside of spotlight cone
			return 0.0;
	} else { // Direct3D
		float cos_inner = cos_delta;
		float cos_outer = pow(cos_delta, 1.01 + spotExponent / 2.0);

		if (cos_gamma > cos_inner) // Inside of spotlight cone
			return 1.0;
		else if (cos_gamma < cos_outer) // Outside of spotlight cone
			return 0.0;
		else // Inside fading zone
			return (cos_gamma - cos_outer) / (cos_inner - cos_outer);
	}
	return 0.0;
}

vec3 computeLight(in int lightIndex, in vec3 normal, in vec3 lightDir, in vec3 obsPos) {
	vec3 spotDir = mat3(view) * -lights[lightIndex].spotDirection;
	// Ambiant component
	attribOut.ambient += mat.ambient * lights[lightIndex].ambient;

	// Diffuse component (spotlight or not)
	float LdotN = dot(lightDir, normal);
	if (LdotN > 0.0) {
		float	spot = useSpotlight ?
			computeSpot(spotDir, lightDir, normal) :
			1.0f;
		attribOut.diffuse += spot * mat.diffuse * lights[lightIndex].diffuse * LdotN;

		// Specular component
		float spec = 0.0;
		if (useBlinn) { // Blinn
			vec3 halfVec = normalize(lightDir + obsPos);
			spec = dot(halfVec, normal);
		} else { // Phong
			vec3 reflectDir = reflect(-lightDir, normal);
			spec = dot(reflectDir, obsPos);
		}

		// No need to take the max between spec and 0.0 since we ignore the negative case
		if (spec > 0) {
			spec = pow(spec, mat.shininess); // Apply shininess to lighthen formula below
			attribOut.specular += mat.specular * lights[lightIndex].specular * spec;
		}
	}
	return vec3(0.0);
}


void main()
{

	vec3 Vertex0 = attribIn[0].position;
	vec3 Vertex1 = attribIn[1].position;
	vec3 Vertex2 = attribIn[2].position;

	vec3 side1 = (Vertex1 - Vertex0);
	vec3 side2 = (Vertex2 - Vertex0);
	vec3 normal = cross(side1, side2);

	vec3 center = (Vertex0 + Vertex1 + Vertex2) / 3.0;

	// Initializes colors components
	attribOut.emission = mat.emission;
	attribOut.ambient = mat.ambient * lightModelAmbient;
	attribOut.diffuse = vec3(0.0);
	attribOut.specular = vec3(0.0);

	vec3 pos = (modelView * vec4(center, 1.0)).xyz;
	vec3 lightDir[3];
	for (int i = 0; i < 3; ++i) {
		lightDir[i] = (view * vec4(lights[i].position, 1.0)).xyz - pos;
		computeLight(i,
			normalize(normalMatrix * normal),
			normalize(lightDir[i]),
			normalize(-pos)
		);
	}

	for (int i = 0; i < gl_in.length(); ++i) {
		gl_Position = gl_in[i].gl_Position;
		attribOut.texCoords = attribIn[i].texCoords;
		EmitVertex();
	}
	EndPrimitive();
}
