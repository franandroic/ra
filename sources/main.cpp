// Local Headers
#include "../headers/Shader.h"
#include "../headers/FPSManager.h"
#include "../headers/Mesh.h"
#include "../headers/Renderable.h"
#include "../headers/Object.h"
#include "../headers/Transformable.h"
#include "../headers/Renderer.h"
#include "../headers/Camera.h"
#include "../headers/Curve.h"
#include "../headers/Pathmaker.h"
#include "../headers/Light.h"
#include "../headers/ReflectorLight.h"
#include "../headers/Material.h"
#include "../headers/Texture.h"
#include "../headers/InputManager.h"
#include "../headers/Animator.h"
#include "../headers/ParticleSpawner.h"
#include "../headers/PaSpObject.h"
#include "../headers/SceneGraph.h"
#include "../headers/SGNode.h"

// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <assimp/Importer.hpp>      
#include <assimp/scene.h>           
#include <assimp/postprocess.h> 

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Standard Headers
#include <iostream>
#include <cstdlib>
#include <vector>
#include <chrono>

//dimenzije prozora
const int mWidth = 1280;
const int mHeight = 800;

//delta-time
double deltaTime = 0.0;

//fokus prozora
bool inFocus = true;

//upravljanje krivuljama
bool bSetToAnimate = false;
bool bAnimating = false;

//parametri za toon sjencanje
glm::vec3 darkest(0.25, 0.11, 0.00);
glm::vec3 darker(0.41, 0.18, 0.01);
glm::vec3 medium(0.63, 0.29, 0.07);
glm::vec3 lighter(0.86, 0.46, 0.22);
glm::vec3 lightest(0.94, 0.71, 0.57);

//parametar za hrapavost materijala
float roughness = 0.5;

GLFWwindow *setupContext() {

	GLFWwindow* window;

	glfwInit();
	gladLoadGL();
	glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
	window = glfwCreateWindow(mWidth, mHeight, "BabyEngine", nullptr, nullptr);
	if (window == nullptr) {
		fprintf(stderr, "Failed to Create OpenGL Context");
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		fprintf(stderr, "Failed to initialize GLAD");
		exit(-1);
	}
	fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glClearColor(0.15, 0.1, 0.1, 1);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	//glFrontFace(GL_CW);
	glEnable(GL_PROGRAM_POINT_SIZE);
	
	glfwSwapInterval(0);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	return window;
}

Shader* loadShader(char* path, char* naziv, bool noGeom) {
	std::string sPath(path);
	std::string pathVert;
	std::string pathGeom;
	std::string pathFrag;

	pathVert.append(path, sPath.find_last_of("\\/") + 1);
	pathGeom.append(path, sPath.find_last_of("\\/") + 1);
	pathFrag.append(path, sPath.find_last_of("\\/") + 1);
	if (pathFrag[pathFrag.size() - 1] == '/') {
		pathVert.append("shaders/");
		pathGeom.append("shaders/");
		pathFrag.append("shaders/");
	}
	else if (pathFrag[pathFrag.size() - 1] == '\\') {
		pathVert.append("shaders\\");
		pathGeom.append("shaders\\");
		pathFrag.append("shaders\\");
	}
	else {
		std::cerr << "nepoznat format pozicije shadera";
		exit(1);
	}

	pathVert.append(naziv);
	pathVert.append(".vert");
	pathGeom.append(naziv);
	pathGeom.append(".geom");
	pathFrag.append(naziv);
	pathFrag.append(".frag");

	if (noGeom) return new Shader(pathVert.c_str(), pathFrag.c_str());
	else return new Shader(pathVert.c_str(), pathGeom.c_str(), pathFrag.c_str());
}

void focus_callback(GLFWwindow *window, int focused) {

	if (focused) inFocus = true;
	else inFocus = false;
}

int main(int argc, char *argv[]) {

	//postavljanje openGL konteksta
    GLFWwindow* window = setupContext();

	//postavljanje callback funkcija
	glfwSetWindowFocusCallback(window, focus_callback);

	//postavljanje seed-a
	srand(time(0));

	//postavljanje upravitelja FPS-a
	FPSManager FPSManager(window, 60, "BabyEngine");
    
	//Ucitavanje modela iz .obj datoteke
	Assimp::Importer importer;

	std::string path(argv[0]);
	std::string dirPath(path, 0, path.find_last_of("\\/"));
	std::string resPath(dirPath);
	resPath.append("\\resources");
	std::string objPath(resPath);
	objPath.append("\\spaceship\\spaceship.obj");

	const aiScene* scene = importer.ReadFile(objPath.c_str(),
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType |
		aiProcess_FlipUVs |
		aiProcess_GenNormals
	);

	if (!scene) {
		std::cerr << importer.GetErrorString();
		return EXIT_FAILURE;
	}

    if (!(scene->HasMeshes())) {
        std::cerr << importer.GetErrorString();
        return EXIT_FAILURE;
    }

	//stvaramo i pozicioniramo kameru
	Camera camera(0.005f, 10.0f, 30.0f, (float)mWidth/(float)mHeight);

	//stvaramo izvor svjetla
	Light light(glm::vec3(0.1, 0.1, 0.1), glm::vec3(0.5, 0.5, 0.5));

	//stvaramo reflektorski izvor svjetla
	ReflectorLight otherLight;
	otherLight.setAmbientIntensity(glm::vec3(0.2, 0.2, 0.2));
	otherLight.setSourceIntensity(glm::vec3(0.9, 0.9, 0.9));

	//inicijaliziramo crtaca
	Renderer renderer(&camera, &light);
	renderer.setReflectorLight(&otherLight);

	//spremamo ucitane podatke u objekt tipa Mesh
	if (!scene->mMeshes[0]) std::cout << "No mesh 0!" << std::endl;
    //aiMesh *aimesh = scene->mMeshes[0];
    //Mesh mesh0(aimesh);
	Mesh meshRightWingBase(scene->mMeshes[0]);

	if (!scene->mMeshes[1]) std::cout << "No mesh 1!" << std::endl;
    //aiMesh *aimesh1 = scene->mMeshes[1];
    //Mesh mesh1(aimesh1);
	Mesh meshLeftWingBase(scene->mMeshes[1]);

	Mesh meshRightWing(scene->mMeshes[2]);
	Mesh meshLeftWing(scene->mMeshes[3]);
	Mesh meshBase(scene->mMeshes[4]);
	Mesh meshCockpit(scene->mMeshes[5]);
	Mesh meshGlass(scene->mMeshes[6]);
	Mesh meshPike(scene->mMeshes[7]);
	Mesh meshRightWingTip(scene->mMeshes[8]);
	Mesh meshLeftWingTip(scene->mMeshes[9]);
	Mesh meshAsteroid(scene->mMeshes[10]);

	//provodimo normalizaciju
	meshRightWingBase.normalize();
	meshLeftWingBase.normalize();

	meshRightWing.normalize();
	meshLeftWing.normalize();
	meshBase.normalize();
	meshCockpit.normalize();
	meshGlass.normalize();
	meshPike.normalize();
	meshRightWingTip.normalize();
	meshLeftWingTip.normalize();
	meshAsteroid.normalize();

	//ucitavamo materijal i teksturu objekta
	aiColor3D aK, dK, sK;
	float shiny;
	Material materialAsteroid;
	Material materialGlass;
	Material materialMetal;
	Material materialWing;

	scene->mMaterials[1]->Get(AI_MATKEY_COLOR_AMBIENT, aK);
	scene->mMaterials[1]->Get(AI_MATKEY_COLOR_DIFFUSE, dK);
	scene->mMaterials[1]->Get(AI_MATKEY_COLOR_SPECULAR, sK);
	scene->mMaterials[1]->Get(AI_MATKEY_SHININESS, shiny);
	materialAsteroid.ambientKoeficient = glm::vec3(aK.r, aK.g, aK.b);
	materialAsteroid.diffuseKoeficient = glm::vec3(dK.r, dK.g, dK.b);
	materialAsteroid.specularKoeficient = glm::vec3(sK.r, sK.g, sK.b);
	materialAsteroid.gloss = shiny;

	scene->mMaterials[2]->Get(AI_MATKEY_COLOR_AMBIENT, aK);
	scene->mMaterials[2]->Get(AI_MATKEY_COLOR_DIFFUSE, dK);
	scene->mMaterials[2]->Get(AI_MATKEY_COLOR_SPECULAR, sK);
	scene->mMaterials[2]->Get(AI_MATKEY_SHININESS, shiny);
	materialGlass.ambientKoeficient = glm::vec3(aK.r, aK.g, aK.b);
	materialGlass.diffuseKoeficient = glm::vec3(dK.r, dK.g, dK.b);
	materialGlass.specularKoeficient = glm::vec3(sK.r, sK.g, sK.b);
	materialGlass.gloss = shiny;

	scene->mMaterials[3]->Get(AI_MATKEY_COLOR_AMBIENT, aK);
	scene->mMaterials[3]->Get(AI_MATKEY_COLOR_DIFFUSE, dK);
	scene->mMaterials[3]->Get(AI_MATKEY_COLOR_SPECULAR, sK);
	scene->mMaterials[3]->Get(AI_MATKEY_SHININESS, shiny);
	materialMetal.ambientKoeficient = glm::vec3(aK.r, aK.g, aK.b);
	materialMetal.diffuseKoeficient = glm::vec3(dK.r, dK.g, dK.b);
	materialMetal.specularKoeficient = glm::vec3(sK.r, sK.g, sK.b);
	materialMetal.gloss = shiny;

	scene->mMaterials[4]->Get(AI_MATKEY_COLOR_AMBIENT, aK);
	scene->mMaterials[4]->Get(AI_MATKEY_COLOR_DIFFUSE, dK);
	scene->mMaterials[4]->Get(AI_MATKEY_COLOR_SPECULAR, sK);
	scene->mMaterials[4]->Get(AI_MATKEY_SHININESS, shiny);
	materialWing.ambientKoeficient = glm::vec3(aK.r, aK.g, aK.b);
	materialWing.diffuseKoeficient = glm::vec3(dK.r, dK.g, dK.b);
	materialWing.specularKoeficient = glm::vec3(sK.r, sK.g, sK.b);
	materialWing.gloss = shiny;

	/*
	Texture diffuseTexture;
	if (scene->HasMaterials()) {
		
		for (int i = 0; i < scene->mNumMeshes; i++) {

			scene->mMaterials[scene->mMeshes[i]->mMaterialIndex]->Get(AI_MATKEY_COLOR_AMBIENT, aK);
			scene->mMaterials[scene->mMeshes[i]->mMaterialIndex]->Get(AI_MATKEY_COLOR_DIFFUSE, dK);
			scene->mMaterials[scene->mMeshes[i]->mMaterialIndex]->Get(AI_MATKEY_COLOR_SPECULAR, sK);
			scene->mMaterials[scene->mMeshes[i]->mMaterialIndex]->Get(AI_MATKEY_SHININESS, shiny);

			material.ambientKoeficient = glm::vec3(aK.r, aK.g, aK.b);
			material.diffuseKoeficient = glm::vec3(dK.r, dK.g, dK.b);
			material.specularKoeficient = glm::vec3(sK.r, sK.g, sK.b);
			material.gloss = shiny;

			aiString naziv;
			scene->mMaterials[scene->mMeshes[i]->mMaterialIndex]->Get(AI_MATKEY_NAME, naziv);
		
			aiString texturePosition;
			int width, height, nrChannels;
			unsigned char *data;
			if (AI_SUCCESS == scene->mMaterials[scene->mMeshes[i]->mMaterialIndex]->Get(AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, 0), texturePosition)) {
				std::string texPath(resPath);
				texPath.append("\\spaceship\\");
				texPath.append(texturePosition.C_Str());

				data = stbi_load(texPath.c_str(), &width, &height, &nrChannels, 0);

				diffuseTexture.setTexWidth(width);
				diffuseTexture.setTexHeight(height);
				diffuseTexture.generateTexture(data);
				material.addDiffuseTexture(&diffuseTexture);

				stbi_image_free(data);
			}
		}

	} else {
		std::cout << "No set materials." << std::endl;
	}
	*/

	//ucitavanje sjencara
	Shader *sjencar[5];
	sjencar[0] = loadShader(argv[0], "shader0", true);
	sjencar[1] = loadShader(argv[0], "shader1", true);
	sjencar[2] = loadShader(argv[0], "shader2", true);
	sjencar[3] = loadShader(argv[0], "shader3", true);
	sjencar[4] = loadShader(argv[0], "shader4", true);

	GLint uniformEyePosLocation = glGetUniformLocation(sjencar[0]->ID, "eyePos");

	//toon sjencanje
	GLint uniformToonDarkest = glGetUniformLocation(sjencar[0]->ID, "darkest");
	GLint uniformToonDarker = glGetUniformLocation(sjencar[0]->ID, "darker");
	GLint uniformToonMedium = glGetUniformLocation(sjencar[0]->ID, "medium");
	GLint uniformToonLighter = glGetUniformLocation(sjencar[0]->ID, "lighter");
	GLint uniformToonLightest = glGetUniformLocation(sjencar[0]->ID, "lightest");

	glUseProgram(sjencar[0]->ID);
		glUniform3fv(uniformToonDarkest, 1, glm::value_ptr(darkest));
		glUniform3fv(uniformToonDarker, 1, glm::value_ptr(darker));
		glUniform3fv(uniformToonMedium, 1, glm::value_ptr(medium));
		glUniform3fv(uniformToonLighter, 1, glm::value_ptr(lighter));
		glUniform3fv(uniformToonLightest, 1, glm::value_ptr(lightest));
	glUseProgram(0);

	//sjencanje s hrapavoscu
	GLint uniformRoughness = glGetUniformLocation(sjencar[1]->ID, "roughness");

	glUseProgram(sjencar[1]->ID);
		glUniform1f(uniformRoughness, roughness);
	glUseProgram(0);

	std::cout << "So far so good..." << std::endl;

	//od Shadera, Mesha i Materijala stvaramo objekte i dodajemo ih u crtaca
	Object objectBase(&meshBase, sjencar[1], &materialMetal, sjencar[3]);
	renderer.registerObject(&objectBase);
	
	Object objectCockpit(&meshCockpit, sjencar[1], &materialMetal, sjencar[3]);
	renderer.registerObject(&objectCockpit);

	Object objectRightWingBase(&meshRightWingBase, sjencar[1], &materialWing, sjencar[3]);
	renderer.registerObject(&objectRightWingBase);

	Object objectLeftWingBase(&meshLeftWingBase, sjencar[1], &materialWing, sjencar[3]);
	renderer.registerObject(&objectLeftWingBase);

	Object objectPike(&meshPike, sjencar[1], &materialMetal, sjencar[3]);
	renderer.registerObject(&objectPike);

	Object objectGlass(&meshGlass, sjencar[1], &materialGlass, sjencar[3]);
	renderer.registerObject(&objectGlass);

	Object objectRightWing(&meshRightWing, sjencar[1], &materialWing, sjencar[3]);
	renderer.registerObject(&objectRightWing);

	Object objectLeftWing(&meshLeftWing, sjencar[1], &materialWing, sjencar[3]);
	renderer.registerObject(&objectLeftWing);

	Object objectRightWingTip(&meshRightWingTip, sjencar[1], &materialMetal, sjencar[3]);
	renderer.registerObject(&objectRightWingTip);

	Object objectLeftWingTip(&meshLeftWingTip, sjencar[1], &materialMetal, sjencar[3]);
	renderer.registerObject(&objectLeftWingTip);

	std::vector<Object *> objectsAsteroids;
	for (int i = 0; i < 100; i++) {
		objectsAsteroids.push_back(new Object(&meshAsteroid, sjencar[1], &materialAsteroid, sjencar[3]));
		renderer.registerObject(objectsAsteroids[i]);
	}

	//stvaramo generator cestica
	//width, height, maxNum, batchSize, batchSpawnFrequency, batchDuration, moveSpeed, moveID, baseColor
	//ParticleSpawner particleSpawner(0.5, 0.5, 1500, 200, 0.3, 1.5, 0.0005, 0, glm::vec3(1.0, 0.0, 0.0)); vatra
	ParticleSpawner particleSpawner(1.0, 1.0, 2000, 200, 0.1, 1.0, 0.001, 2, glm::vec3(1.0f, 1.0f, 0.0f));
	PaSpObject paspObject(glm::vec3(0.0, 0.0, 0.0), &particleSpawner, sjencar[4]);
	paspObject.loadParticles();
	renderer.registerPaspObject(&paspObject);

	//gradimo graf scene
	SceneGraph sceneGraph;

	SGNode spaceshipBase(&objectBase, "base", false);
	SGNode spaceshipCockpit(&objectCockpit, "cockpit", false);
	SGNode spaceshipRightWingBase(&objectRightWingBase, "right_wing_base", false);
	SGNode spaceshipLeftWingBase(&objectLeftWingBase, "left_wing_base", false);
	SGNode spaceshipPike(&objectPike, "pike", false);
	SGNode spaceshipGlass(&objectGlass, "glass", false);
	SGNode spaceshipRightWing(&objectRightWing, "right_wing", false);
	SGNode spaceshipLeftWing(&objectLeftWing, "left_wing", false);
	SGNode spaceshipRightWingTip(&objectRightWingTip, "right_wing_tip", false);
	SGNode spaceshipLeftWingTip(&objectLeftWingTip, "left_wing_tip", false);

	//SGNode asteroid0(objectsAsteroids[0], "asteroid_0", false);
	std::vector<SGNode *> asteroids;
	for (int i = 0; i < objectsAsteroids.size(); i++) {
		asteroids.push_back(new SGNode(objectsAsteroids[i], "asteroid_" + std::to_string(i), false));
	}

	SGNode particle1(&paspObject, "particles_1", true);

	SGNode camera1(&camera, "camera_1", false);

	SGNode reflector1(&otherLight, "reflector_1", false);

	//postavljamo input managera
	InputManager inputManager(window, mWidth, mHeight, &sceneGraph, &camera1, &spaceshipBase);

	sceneGraph.root.children.push_back(&spaceshipBase);
	sceneGraph.root.children[0]->children.push_back(&spaceshipCockpit);
	sceneGraph.root.children[0]->children.push_back(&spaceshipRightWingBase);
	sceneGraph.root.children[0]->children.push_back(&spaceshipLeftWingBase);
	sceneGraph.root.children[0]->children[0]->children.push_back(&spaceshipPike);
	sceneGraph.root.children[0]->children[0]->children.push_back(&spaceshipGlass);
	sceneGraph.root.children[0]->children[1]->children.push_back(&spaceshipRightWing);
	sceneGraph.root.children[0]->children[2]->children.push_back(&spaceshipLeftWing);
	sceneGraph.root.children[0]->children[1]->children[0]->children.push_back(&spaceshipRightWingTip);
	sceneGraph.root.children[0]->children[2]->children[0]->children.push_back(&spaceshipLeftWingTip);

	//sceneGraph.root.children.push_back(&asteroid0);
	float asteroidScaleFactor = 0.0f;
	for (int i = 0; i < asteroids.size(); i++) {
		sceneGraph.root.children.push_back(asteroids[i]);
		sceneGraph.moveSubtree(asteroids[i]->name, glm::vec3(((float) rand() / RAND_MAX) * 100.0f - 50.0f,
															 ((float) rand() / RAND_MAX) * 100.0f - 50.0f,
															 ((float) rand() / RAND_MAX) * 100.0f - 50.0f));
		
		asteroidScaleFactor = ((float) rand() / RAND_MAX) * 2.0f + 1.0f;
		sceneGraph.scaleSubtree(asteroids[i]->name, glm::vec3(asteroidScaleFactor + ((float) rand() / RAND_MAX),
															  asteroidScaleFactor + ((float) rand() / RAND_MAX),
															  asteroidScaleFactor + ((float) rand() / RAND_MAX)));

		sceneGraph.rotateSubtree(asteroids[i]->name, glm::vec3(1.0, 0.0, 0.0), ((float) rand() / RAND_MAX) * 90.0f);
		sceneGraph.rotateSubtree(asteroids[i]->name, glm::vec3(0.0, 1.0, 0.0), ((float) rand() / RAND_MAX) * 90.0f);
		sceneGraph.rotateSubtree(asteroids[i]->name, glm::vec3(0.0, 0.0, 1.0), ((float) rand() / RAND_MAX) * 90.0f);
	}

	sceneGraph.root.children.push_back(&particle1);

	sceneGraph.root.children[0]->children.push_back(&reflector1);

	sceneGraph.root.children.push_back(&camera1);

	sceneGraph.moveSubtree("base", glm::vec3(0.0, 0.0, -10.0));
	sceneGraph.moveSubtree("cockpit", glm::vec3(0.0, 0.0, 1.4));
	sceneGraph.moveSubtree("right_wing_base", glm::vec3(-0.45, -0.1, 0.0));
	sceneGraph.moveSubtree("left_wing_base", glm::vec3(0.45, -0.1, 0.0));
	sceneGraph.moveSubtree("pike", glm::vec3(0.0, 0.0, 1.5));
	sceneGraph.moveSubtree("glass", glm::vec3(0.0, 0.45, -0.2));
	sceneGraph.moveSubtree("right_wing", glm::vec3(-2.05, -0.1, 0.0));
	sceneGraph.moveSubtree("left_wing", glm::vec3(2.05, -0.1, 0.0));
	sceneGraph.moveSubtree("right_wing_tip", glm::vec3(-1.525, -0.15, 0.55));
	sceneGraph.moveSubtree("left_wing_tip", glm::vec3(1.525, -0.15, 0.55));

	sceneGraph.moveSubtree("reflector_1", glm::vec3(0.0, 0.0, 2.7));
	//sceneGraph.rotateSubtree("reflector_1", glm::vec3(0.0, 1.0, 0.0), 200.0f);
	inputManager.setReflector(&reflector1);

	sceneGraph.scaleSubtree("right_wing_base", glm::vec3(0.7, 0.7, 0.7));
	sceneGraph.scaleSubtree("left_wing_base", glm::vec3(0.7, 0.7, 0.7));
	sceneGraph.scaleSubtree("glass", glm::vec3(0.5, 0.5, 0.5));
	sceneGraph.scaleSubtree("right_wing", glm::vec3(1.4, 1.4, 1.4));
	sceneGraph.scaleSubtree("left_wing", glm::vec3(1.4, 1.4, 1.4));
	sceneGraph.scaleSubtree("right_wing_tip", glm::vec3(0.75, 0.75, 0.75));
	sceneGraph.scaleSubtree("left_wing_tip", glm::vec3(0.75, 0.75, 0.75));

	//sceneGraph.scaleSubtree("base", glm::vec3(0.2, 0.2, 0.2));

	camera1.item->setPosition(spaceshipBase.item->getPosition());
	sceneGraph.moveSubtree("camera_1", glm::vec3(2.5, 1.0, 5.0));
	sceneGraph.rotateSubtree("camera_1", glm::vec3(0.0, 1.0, 0.0), 200.0f);

	//stvaramo crtaca putanje
	
	/*
	Pathmaker pathmaker(sjencar[2], &camera);
	
	pathmaker.setControlPoint(glm::vec3(-1.0, 0.0, -1.0));
	pathmaker.setControlPoint(glm::vec3(-2.0, -1.0, -2.0));
	pathmaker.setControlPoint(glm::vec3(-2.0, -1.0, 0.0));
	pathmaker.setControlPoint(glm::vec3(0.0, 2.0, 1.0));
	pathmaker.setControlPoint(glm::vec3(1.0, 3.0, 1.0));
	pathmaker.setControlPoint(glm::vec3(3.0, 3.0, 3.0));
	pathmaker.setControlPoint(glm::vec3(3.0, 3.0, -1.0));
	pathmaker.setControlPoint(glm::vec3(-5.0, -5.0, -5.0));
	pathmaker.setControlPoint(glm::vec3(1.0, 1.0, 1.0));
	pathmaker.remakeCurves();

	//stvaramo animatora
	Animator animator(&paspObject, pathmaker.getAnimationCurve(), pathmaker.getAnimationTangents(), pathmaker.getAnimationSecDer());

	inputManager.addAnimator(&animator);
	*/

	//glavna petlja
	while(glfwWindowShouldClose(window) == false) {

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		deltaTime = FPSManager.maintainFPS();

		if (inputManager.currentInputProfile == InputProfile::FlyingCamera) inputManager.handleInput(&camera1, inFocus);
		else if (inputManager.currentInputProfile == InputProfile::VehicleControl) inputManager.handleInput(&spaceshipBase, inFocus);

		//iscrtavanje objekta
		glUseProgram(sjencar[0]->ID);
			glUniform3f(uniformEyePosLocation, renderer.camera->getEyePos().x, renderer.camera->getEyePos().y, renderer.camera->getEyePos().z);
		glUseProgram(0);
		
		glViewport(0, 0, renderer.refLight->DPwidth, renderer.refLight->DPheight);
		glBindFramebuffer(GL_FRAMEBUFFER, renderer.FBO);
			glClear(GL_DEPTH_BUFFER_BIT);
			renderer.renderShadowMap();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glViewport(0, 0, mWidth, mHeight);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		renderer.renderObjects();

		renderer.renderPaspObjects(deltaTime * 1000.0);

		//iscrtavanje krivulja
		//pathmaker.renderCurves(3, 4);

		glfwSwapBuffers(window);
		glfwPollEvents();

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

	}

	//brisanje resursa pri zavrsetku izvodenja programa
	delete sjencar;

	glfwTerminate();

    return EXIT_SUCCESS;
}
