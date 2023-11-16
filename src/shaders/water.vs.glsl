#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 inTexCoords;

out ATTRIB_VS_OUT {
	vec2 texCoords;
	vec3 normal;
	vec3 lightDir[3];
	vec3 spotDir[3];
	vec3 obsPos;
} attribOut;

uniform mat4 mvp;
uniform mat4 view;
uniform mat4 modelView;
uniform mat3 normalMatrix;

uniform float time;

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

void main() {
	vec2 tiledCoords = inTexCoords * vec2(2, 5);
	vec2 flow = vec2(0, time/10.0);

	attribOut.texCoords = tiledCoords + flow;

	vec4 modelPos = vec4(position.xyz, 1.0);
	gl_Position = mvp * modelPos;
	attribOut.normal = normalMatrix * vec3(0,1,0);

	vec4 viewPos = modelView * modelPos;
	for (int i = 0; i < 3; ++i) {
		attribOut.lightDir[i] = (view * vec4(lights[i].position, 1.0f)).xyz - viewPos.xyz;
		attribOut.spotDir[i] = mat3(view) * -lights[i].spotDirection;
	}
	attribOut.obsPos = -viewPos.xyz;
}
