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

	InputManager inputManager(window, mWidth, mHeight, InputProfile::FlyingCamera);

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
	objPath.append("\\glava\\glava.obj");

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
	Camera camera(0.1f, 5.0f, 30.0f, (float)mWidth/(float)mHeight);
	camera.globalMove(glm::vec3(0.0, 0.5, 5.0));
	camera.rotate(glm::rotate(glm::mat4(1.0), glm::radians(180.0f), glm::vec3(0.0, 1.0, 0.0)));

	//stvaramo izvor svjetla
	Light light(glm::vec3(1.0, 1.0, 1.0), glm::vec3(1.0, 1.0, 1.0));
	light.globalMove(glm::vec3(0.0, 5.0, 5.0));

	//stvaramo reflektorski izvor svjetla
	ReflectorLight otherLight;
	otherLight.globalMove(glm::vec3(0.0, 0.5, 5.0));
	otherLight.rotate(glm::rotate(glm::mat4(1.0), glm::radians(180.0f), glm::vec3(0.0, 1.0, 0.0)));

	//inicijaliziramo crtaca
	Renderer renderer(&camera, &light);
	renderer.setReflectorLight(&otherLight);

	//spremamo ucitane podatke u objekt tipa Mesh
    aiMesh *aimesh = scene->mMeshes[0];
    Mesh mesh(aimesh);

	//ucitavamo materijal i teksturu objekta
	aiColor3D aK, dK, sK;
	float shiny;
	Material material;
	Texture diffuseTexture;

	if (scene->HasMaterials()) {

		scene->mMaterials[aimesh->mMaterialIndex]->Get(AI_MATKEY_COLOR_AMBIENT, aK);
		scene->mMaterials[aimesh->mMaterialIndex]->Get(AI_MATKEY_COLOR_DIFFUSE, dK);
		scene->mMaterials[aimesh->mMaterialIndex]->Get(AI_MATKEY_COLOR_SPECULAR, sK);
		scene->mMaterials[aimesh->mMaterialIndex]->Get(AI_MATKEY_SHININESS, shiny);

		material.ambientKoeficient = glm::vec3(aK.r, aK.g, aK.b);
		material.diffuseKoeficient = glm::vec3(dK.r, dK.g, dK.b);
		material.specularKoeficient = glm::vec3(sK.r, sK.g, sK.b);
		material.gloss = shiny;

		aiString naziv;
		scene->mMaterials[aimesh->mMaterialIndex]->Get(AI_MATKEY_NAME, naziv);
	
		aiString texturePosition;
		int width, height, nrChannels;
		unsigned char *data;
		if (AI_SUCCESS == scene->mMaterials[aimesh->mMaterialIndex]->Get(AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, 0), texturePosition)) {
			std::string texPath(resPath);
			texPath.append("\\glava\\");
			texPath.append(texturePosition.C_Str());

			data = stbi_load(texPath.c_str(), &width, &height, &nrChannels, 0);

			diffuseTexture.setTexWidth(width);
			diffuseTexture.setTexHeight(height);
			diffuseTexture.generateTexture(data);
			material.addDiffuseTexture(&diffuseTexture);

			stbi_image_free(data);
		}

	} else {
		std::cout << "No set materials." << std::endl;
	}

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

	//provodimo normalizaciju
	mesh.normalize();

	//od Shadera, Mesha i Materijala stvaramo objekte, transformiramo ih i dodajemo u crtaca
	Object object(&mesh, sjencar[1], nullptr, sjencar[3]);
	object.setScale(glm::vec3(0.5, 0.5, 0.5));
	object.globalMove(glm::vec3(0.0, -2.0, 0.0));
	renderer.registerObject(&object);

	//stvaramo generator cestica
	ParticleSpawner particleSpawner(1, 1, 200, 20, 5.0, glm::vec3(1.0, 0.0, 0.0));
	PaSpObject paspObject(glm::vec3(0.0, 0.0, 0.0), &particleSpawner, sjencar[4]);
	paspObject.globalMove(glm::vec3(0.0, -2.0, 0.0));
	paspObject.loadParticles();
	renderer.registerPaspObject(&paspObject);
	
	for (int i = 0; i < particleSpawner.countVertices(); i++) {
		std::cout << particleSpawner.getVertexAt(i).x << " " << particleSpawner.getVertexAt(i).y << " " << particleSpawner.getVertexAt(i).z << std::endl;
	}

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
	Animator animator(&object, pathmaker.getAnimationCurve(), pathmaker.getAnimationTangents(), pathmaker.getAnimationSecDer());

	inputManager.addAnimator(&animator);
	*/

	//glavna petlja
	while(glfwWindowShouldClose(window) == false) {

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		FPSManager.maintainFPS();

		inputManager.handleInput(renderer.camera, inFocus);

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

		renderer.renderPaspObjects();

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
