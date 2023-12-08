#include "resources.hpp"

#include "vertices_data.hpp"
#include "utils.hpp"

const char* pathes[] ={
	"../textures/skybox/Daylight Box_Right.bmp",
	"../textures/skybox/Daylight Box_Left.bmp",
	"../textures/skybox/Daylight Box_Top.bmp",
	"../textures/skybox/Daylight Box_Bottom.bmp",
	"../textures/skybox/Daylight Box_Front.bmp",
	"../textures/skybox/Daylight Box_Back.bmp",
};

Resources::Resources()
	: ground(groundVertices, sizeof(groundVertices), groundIndexes, sizeof(groundIndexes))
	, river(riverVertices, sizeof(riverVertices), groundIndexes, sizeof(groundIndexes))
	, quad(quad2dVertices, sizeof(quad2dVertices), quad2dIndexes, sizeof(quad2dIndexes))
	, skybox(skyboxVertices, sizeof(skyboxVertices))
	, grassCount(0)
	, tesselationPlaneCount(0)
	, rock("../models/rock_smooth.obj")
	, shroom("../models/mushroom_smooth.obj")
	, tree("../models/tree_smooth.obj")
	, suzanne("../models/suzanne.obj")
	, glass("../models/glass.obj")
	, sphere("../models/icosphere.obj")
	, cube("../models/cube.obj")
	, spotlight("../models/spotlight.obj")
	, rockTexture("../models/rockTexture.png", GL_CLAMP_TO_EDGE)
	, shroomTexture("../models/mushroomTexture.png", GL_CLAMP_TO_EDGE)
	, treeTexture("../models/treeTexture.png", GL_CLAMP_TO_EDGE)
	, suzanneTexture("../models/suzanneTexture.png", GL_CLAMP_TO_EDGE)
	, suzanneShadelessTexture("../models/suzanneTextureShadeless.png", GL_CLAMP_TO_EDGE)
	, groundTexture("../textures/groundSeamless.jpg", GL_REPEAT)
	, riverTexture("../textures/waterSeamless.jpg", GL_REPEAT)
	, grassCluterTexture("../textures/grassAtlas.png", GL_CLAMP_TO_EDGE)
	, billboardTexture("../textures/treeBillboard.png", GL_CLAMP_TO_EDGE)
	, heartTexture("../textures/heart.png", GL_CLAMP_TO_EDGE)
	, glassTexture("../textures/glass.png", GL_CLAMP_TO_EDGE)
	, whiteTexture("../textures/white.png", GL_CLAMP_TO_EDGE)
	, diffuseMapTexture("../textures/metal_0029_color_1k.jpg", GL_CLAMP_TO_EDGE)
	, specularMapTexture("../textures/metal_0029_metallic_1k.jpg", GL_CLAMP_TO_EDGE)
	, heightmapTexture("../textures/heightmap.png", GL_CLAMP_TO_EDGE)
	, sandTexture("../textures/sand.jpg", GL_REPEAT)
	, snowTexture("../textures/snow.jpg", GL_REPEAT)
	, flameTexture("../textures/flame.png", GL_CLAMP_TO_EDGE)
	, skyboxTexture(pathes) {
	// Simple meshes
	ground.enableAttribute(0, 3, 5, 0);
	ground.enableAttribute(1, 2, 5, 3);

	river.enableAttribute(0, 3, 5, 0);
	river.enableAttribute(1, 2, 5, 3);

	quad.enableAttribute(0, 3, 5, 0);
	quad.enableAttribute(1, 2, 5, 3);

	skybox.enableAttribute(0, 3, 0, 0);

	const int N_GRASS = 500;
	std::vector<GLfloat> grassData;
	grassData.reserve(N_GRASS * 5);

	for (int i = 0; i < N_GRASS; ++i) {
		float randomX = -25.0f + rand01() * 50.0f;
		float randomZ = -25.0f + rand01() * 50.0f;
		double rval = rand01();
		int randomTexture = rval > 0.05f ? rval > 0.10f ? 0 : 1 : 2;
		for (int nVert = 0; nVert < 18; ++nVert) {
			float x = grassPosition[nVert * 3 + 0];
			float y = grassPosition[nVert * 3 + 1];
			float z = grassPosition[nVert * 3 + 2];
			float u = grassTexCoords[nVert * 2 + 0];
			float v = grassTexCoords[nVert * 2 + 1];

			grassData.push_back(x + randomX);
			grassData.push_back(y - 1.0f);
			grassData.push_back(z + randomZ);
			grassData.push_back(u / 3.0f + randomTexture / 3.0f);
			grassData.push_back(v);
		}
	}
	grassCount = grassData.size() / 5;
	grass.setData(grassData.data(), grassData.size() * sizeof(float));
	grass.enableAttribute(0, 3, 5, 0);
	grass.enableAttribute(1, 2, 5, 3);
	grassData.clear(); grassData.resize(0); // desalloc array

	const int N_DIVISIONS = 16;
	const int N_PRIMITIVES = N_DIVISIONS + 1;
	const int N_COMPONENTS = 3;
	const int N_PER_PRIMITIVE = 4;
	const int DATA_SIZE = N_PRIMITIVES * N_PRIMITIVES * N_PER_PRIMITIVE * N_COMPONENTS;
	const GLfloat SIZE = 256.0f;
	const GLfloat FACTOR = SIZE / N_PRIMITIVES;
	const GLfloat OFFSET = -SIZE / 2;
	GLfloat planeData[DATA_SIZE];
	tesselationPlaneCount = N_PRIMITIVES * N_PRIMITIVES * N_PER_PRIMITIVE;

	for (int i = 0; i < DATA_SIZE;) {
		int primitiveIndex = i / (N_PER_PRIMITIVE * N_COMPONENTS);
		int x = primitiveIndex % (N_PRIMITIVES);
		int y = primitiveIndex / (N_PRIMITIVES);

		int X = x + 1;
		int Y = y + 1;

		planeData[i++] = x * FACTOR + OFFSET;
		planeData[i++] = -1.0f;
		planeData[i++] = y * FACTOR + OFFSET;

		planeData[i++] = x * FACTOR + OFFSET;
		planeData[i++] = -1.0f;
		planeData[i++] = Y * FACTOR + OFFSET;

		planeData[i++] = X * FACTOR + OFFSET;
		planeData[i++] = -1.0f;
		planeData[i++] = Y * FACTOR + OFFSET;

		planeData[i++] = X * FACTOR + OFFSET;
		planeData[i++] = -1.0f;
		planeData[i++] = y * FACTOR + OFFSET;
	}

	tesselationPlane.setData(planeData, sizeof(planeData));
	tesselationPlane.enableAttribute(0, 3, 0, 0);

	// Textures
	groundTexture.enableMipmap();
	riverTexture.enableMipmap();

#define SHADER_LOAD_PRINT true
	// Model shader
	if (SHADER_LOAD_PRINT) printf("[Shader] Loading model shader\n");
	{
		model.init("shaders/model.vs.glsl", "shaders/model.fs.glsl");
		mvpLocationModel = model.getUniformLoc("mvp");
	}

	// Water shader
	if (SHADER_LOAD_PRINT) printf("[Shader] Loading water shader\n");
	{
		water.init("shaders/water.vs.glsl", "shaders/water.fs.glsl");

		mvpLocationWater = water.getUniformLoc("mvp");
		timeLocationWater = water.getUniformLoc("time");

		modelViewLocationWater = water.getUniformLoc("modelView");
		viewLocationWater = water.getUniformLoc("view");
		normalLocationWater = water.getUniformLoc("normalMatrix");

		water.use();
		glUniform1i(water.getUniformLoc("diffuseSampler"), 0);
		glUniform1i(water.getUniformLoc("specularSampler"), 1);
	}

	// Phong shader
	if (SHADER_LOAD_PRINT) printf("[Shader] Loading phong shader\n");
	{
		phong.init("shaders/phong.vs.glsl", "shaders/phong.fs.glsl");

		mvpLocationPhong = phong.getUniformLoc("mvp");
		modelViewLocationPhong = phong.getUniformLoc("modelView");
		viewLocationPhong = phong.getUniformLoc("view");
		normalLocationPhong = phong.getUniformLoc("normalMatrix");
		phong.use();
		glUniform1i(phong.getUniformLoc("diffuseSampler"), 0);
		glUniform1i(phong.getUniformLoc("specularSampler"), 1);
	}

	// Grass shader
	if (SHADER_LOAD_PRINT) printf("[Shader] Loading grass shader\n");
	{
		grassShader.init("shaders/grass.vs.glsl", "shaders/phong.fs.glsl");

		mvpLocationGrass = grassShader.getUniformLoc("mvp");
		timeLocationGrass = grassShader.getUniformLoc("time");

		modelViewLocationGrass = grassShader.getUniformLoc("modelView");
		viewLocationGrass = grassShader.getUniformLoc("view");
		normalLocationGrass = grassShader.getUniformLoc("normalMatrix");

		grassShader.use();
		glUniform1i(grassShader.getUniformLoc("diffuseSampler"), 0);
		glUniform1i(grassShader.getUniformLoc("specularSampler"), 1);
	}

	// Gouraud shader
	if (SHADER_LOAD_PRINT) printf("[Shader] Loading gouraud shader\n");
	{
		gouraud.init("shaders/gouraud.vs.glsl", "shaders/gouraud.fs.glsl");

		mvpLocationGouraud = gouraud.getUniformLoc("mvp");
		modelViewLocationGouraud = gouraud.getUniformLoc("modelView");
		viewLocationGouraud = gouraud.getUniformLoc("view");
		normalLocationGouraud = gouraud.getUniformLoc("normalMatrix");
		gouraud.use();
		glUniform1i(gouraud.getUniformLoc("diffuseSampler"), 0);
		glUniform1i(gouraud.getUniformLoc("specularSampler"), 1);
	}

	// Flat shader
	if (SHADER_LOAD_PRINT) printf("[Shader] Loading flat shader\n");
	{
		flat.init("shaders/flat.vs.glsl", "shaders/flat.gs.glsl", "shaders/gouraud.fs.glsl");

		mvpLocationFlat = flat.getUniformLoc("mvp");
		modelViewLocationFlat = flat.getUniformLoc("modelView");
		viewLocationFlat = flat.getUniformLoc("view");
		normalLocationFlat = flat.getUniformLoc("normalMatrix");
		flat.use();
		glUniform1i(flat.getUniformLoc("diffuseSampler"), 0);
		glUniform1i(flat.getUniformLoc("specularSampler"), 1);
	}

	// Simple shader
	if (SHADER_LOAD_PRINT) printf("[Shader] Loading simple shader\n");
	{
		simple.init("shaders/simple.vs.glsl", "shaders/simple.fs.glsl");

		mvpLocationSimple = simple.getUniformLoc("mvp");
		colorLocationSimple = simple.getUniformLoc("color");
	}

	// Skybox shader
	if (SHADER_LOAD_PRINT) printf("[Shader] Loading skybox shader\n");
	{
		skyboxShader.init("shaders/skybox.vs.glsl", "shaders/skybox.fs.glsl");

		mvpLocationSky = skyboxShader.getUniformLoc("mvp");
	}

	// Tessellation shader
	if (SHADER_LOAD_PRINT) printf("[Shader] Loading tessellation shader\n");
	{
		std::string vertexCode = readFile("shaders/tessellation.vs.glsl");
		std::string tessCtrlCode = readFile("shaders/tessellation.tcs.glsl");
		std::string tessEvalCode = readFile("shaders/tessellation.tes.glsl");
		std::string geometryCode = readFile("shaders/tessellation.gs.glsl");
		std::string fragmentCode = readFile("shaders/tessellation.fs.glsl");

		Shader vertex(GL_VERTEX_SHADER, vertexCode.c_str());
		Shader tessControl(GL_TESS_CONTROL_SHADER, tessCtrlCode.c_str());
		Shader tessEvaluation(GL_TESS_EVALUATION_SHADER, tessEvalCode.c_str());
		Shader geometry(GL_GEOMETRY_SHADER, geometryCode.c_str());
		Shader fragment(GL_FRAGMENT_SHADER, fragmentCode.c_str());
		tessellation.attachShader(vertex);
		tessellation.attachShader(tessControl);
		tessellation.attachShader(tessEvaluation);
		tessellation.attachShader(geometry);
		tessellation.attachShader(fragment);
		tessellation.link();

		mvpLocationTessellation = tessellation.getUniformLoc("mvp");
		modelViewLocationTessellation = tessellation.getUniformLoc("modelView");
		viewWireframeLocationTessellation = tessellation.getUniformLoc("viewWireframe");

		tessellation.use();
		glUniform1i(tessellation.getUniformLoc("heighmapSampler"), 0);
		glUniform1i(tessellation.getUniformLoc("groundSampler"), 1);
		glUniform1i(tessellation.getUniformLoc("sandSampler"), 2);
		glUniform1i(tessellation.getUniformLoc("snowSampler"), 3);
	}

	// Particule shader
	if (SHADER_LOAD_PRINT) printf("[Shader] Loading particule shader\n");
	{
		std::string vertexCode = readFile("shaders/particle.vs.glsl");
		std::string geometryCode = readFile("shaders/particle.gs.glsl");
		std::string fragmentCode = readFile("shaders/particle.fs.glsl");

		Shader vertex(GL_VERTEX_SHADER, vertexCode.c_str());
		Shader geometry(GL_GEOMETRY_SHADER, geometryCode.c_str());
		Shader fragment(GL_FRAGMENT_SHADER, fragmentCode.c_str());
		particule.attachShader(vertex);
		particule.attachShader(geometry);
		particule.attachShader(fragment);
		particule.link();

		modelViewLocationParticle = particule.getUniformLoc("modelView");
		projectionLocationParticle = particule.getUniformLoc("projection");
	}

	// Transform feedback shader
	if (SHADER_LOAD_PRINT) printf("[Shader] Loading transform feedback shader\n");
	{
		std::string vertexCode = readFile("shaders/transformFeedback.vs.glsl");

		Shader vertex(GL_VERTEX_SHADER, vertexCode.c_str());
		transformFeedback.attachShader(vertex);

		const GLchar* vars[] ={
			"positionMod",
			"velocityMod",
			"colorMod",
			"sizeMod",
			"timeToLiveMod"
		};
		transformFeedback.setTransformFeedbackVaryings(vars, sizeof(vars) / sizeof(vars[0]), GL_INTERLEAVED_ATTRIBS);
		transformFeedback.link();

		timeLocationTransformFeedback = transformFeedback.getUniformLoc("time");
		dtLocationTransformFeedback = transformFeedback.getUniformLoc("dt");
	}
}
