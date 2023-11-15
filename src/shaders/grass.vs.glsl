#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoords;

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
	vec3 pos = position;
	pos.x += texCoords.y*sin(time/2.0 + pos.x + pos.z)*0.05;
	pos.z += texCoords.y*sin(time/2.0 + pos.x + pos.z)*0.05;

	vec4 modelPos = vec4(pos.xyz, 1.0);
	gl_Position = mvp * modelPos;
	attribOut.texCoords = texCoords;
	attribOut.normal = normalMatrix * vec3(0,1,0);

	vec4 viewPos = modelView * modelPos;
	for (int i = 0; i < 3; ++i) {
		attribOut.lightDir[i] = ( view * vec4(lights[i].position, 1.0f) ).xyz - viewPos.xyz;
		attribOut.spotDir[i] = mat3(view) * -lights[i].spotDirection;
	}
	attribOut.obsPos = -viewPos.xyz;
}
