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

// TODO: Réecrire avec shader init
Resources::Resources()
	: ground(groundVertices, sizeof(groundVertices), groundIndexes, sizeof(groundIndexes))
	, skybox(skyboxVertices, sizeof(skyboxVertices))
	, glass("../models/glass.obj")
	, suzanne("../models/suzanne.obj")
	, sphere("../models/icosphere.obj")
	, cube("../models/cube.obj")
	, spotlight("../models/spotlight.obj")
	, rock("../models/rock.obj")
	, glassTexture("../models/glass.png", GL_CLAMP_TO_EDGE)
	, suzanneTexture("../models/suzanneTextureShade.png", GL_CLAMP_TO_EDGE)
	, whiteTexture("../textures/white.png", GL_CLAMP_TO_EDGE)
	, groundTexture("../textures/grassSeamless.jpg", GL_REPEAT)
	, rockTexture("../models/rockTexture.png", GL_CLAMP_TO_EDGE)
	, skyboxTexture(pathes) {
	// Simple meshes
	ground.enableAttribute(0, 3, 5, 0);
	ground.enableAttribute(1, 2, 5, 3);

	skybox.enableAttribute(0, 3, 0, 0);

	// Textures
	groundTexture.enableMipmap();

	// Model shader
	{
		model.init("shaders/model.vs.glsl", "shaders/model.fs.glsl");
		mvpLocationModel = model.getUniformLoc("mvp");
	}

	// Phong shader
	{
		phong.init("shaders/phong.vs.glsl", "shaders/phong.fs.glsl");
		mvpLocationPhong = phong.getUniformLoc("mvp");
		modelViewLocationPhong = phong.getUniformLoc("modelView");
		viewLocationPhong = phong.getUniformLoc("view");
		normalLocationPhong = phong.getUniformLoc("normalMatrix");
	}

	// Gouraud shader
	{
		gouraud.init("shaders/gouraud.vs.glsl", "shaders/gouraud.fs.glsl");
		mvpLocationGouraud = gouraud.getUniformLoc("mvp");
		modelViewLocationGouraud = gouraud.getUniformLoc("modelView");
		viewLocationGouraud = gouraud.getUniformLoc("view");
		normalLocationGouraud = gouraud.getUniformLoc("normalMatrix");
	}

	// Flat shader
	{
		flat.init("shaders/flat.vs.glsl", "shaders/flat.gs.glsl", "shaders/flat.fs.glsl");
		mvpLocationFlat = flat.getUniformLoc("mvp");
		modelViewLocationFlat = flat.getUniformLoc("modelView");
		viewLocationFlat = flat.getUniformLoc("view");
		normalLocationFlat = flat.getUniformLoc("normalMatrix");
	}

	// Simple shader
	{
		simple.init("shaders/simple.vs.glsl", "shaders/simple.fs.glsl");
		mvpLocationSimple = simple.getUniformLoc("mvp");
		colorLocationSimple = simple.getUniformLoc("color");
	}

	// Skybox shader
	{
		skyboxShader.init("shaders/skybox.vs.glsl", "shaders/skybox.fs.glsl");
		mvpLocationSky = skyboxShader.getUniformLoc("mvp");
	}
}
