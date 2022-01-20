#include "Graphics\window.h"
#include "Camera\camera.h"
#include "Shaders\shader.h"
#include "Model Loading\mesh.h"
#include "Model Loading\texture.h"
#include "Model Loading\meshLoaderObj.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

void processKeyboardInput ();

// todo: refactor globals away (leave window and camera) 30min

bool toggleUI = true; // button press hides UI (makes this false)
float sliderValue; // sun pos modifier from UIi

float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

Window window("Game Engine", 1920, 1080);
GLFWwindow* pWindow = window.getWindow(); // todo: ew 
double mousePosX, mousePosY, mouseDeltaX, mouseDeltaY;
Camera camera;

glm::vec3 lightColor = glm::vec3(1.0f);
glm::vec3 lightPos = glm::vec3(-180.0f, 150.0f, -200.0f);

// rotate around a point <pt> away on <rotationAxis> axis
glm::mat4 rotateAroundPoint(glm::mat4 modelMatrix, const glm::vec3& pt, const glm::vec3& rotationAxis, float rotateSpeed, float t)
{   
	modelMatrix = glm::translate(modelMatrix, pt);
	modelMatrix = glm::rotate(modelMatrix, rotateSpeed * t, rotationAxis);
	return glm::translate(modelMatrix, -pt);
}

int main()
{
	glClearColor(0.2f, 0.8f, 1.0f, 1.0f);

	// imgui
	const char* glsl_version = "#version 400";

    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(pWindow, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
	ImVec4 clear_color = ImColor(0x49, 0xA6, 0x95);

	glEnable(GL_DEPTH_TEST);

	//building and compiling shader program
	Shader shader("Shaders/vertex_shader.glsl", "Shaders/fragment_shader.glsl");
	Shader skyShader("Shaders/sky_vertex_shader.glsl", "Shaders/sky_fragment_shader.glsl");
	Shader sunShader("Shaders/sun_vertex_shader.glsl", "Shaders/sun_fragment_shader.glsl");
	Shader mountainShader("Shaders/hill_vertex_shader.glsl", "Shaders/hill_fragment_shader.glsl");
	Shader oceanShader("Shaders/ocean_vertex_shader.glsl", "Shaders/ocean_fragment_shader.glsl");

	//Textures
	GLuint tex = loadBMP("Resources/Textures/wood.bmp");
	GLuint tex2 = loadBMP("Resources/Textures/rock.bmp");
	GLuint tex3 = loadBMP("Resources/Textures/water.bmp");
	GLuint texSky = loadBMP("Resources/Textures/sky2.bmp");
	GLuint texPenguin = loadBMP("Resources/Textures/penguin.bmp");
	GLuint texTree = loadBMP("Resources/Textures/tree.bmp");

	std::vector<Texture> textures;
	textures.push_back(Texture(tex, "texture_diffuse"));

	std::vector<Texture> textures2;
	textures2.push_back(Texture(tex2, "texture_diffuse"));

	std::vector<Texture> textures3;
	textures3.push_back(Texture(tex3, "texture_diffuse"));

    std::vector<Texture> textures4;
    textures4.push_back(Texture(texSky, "texture_diffuse"));

    std::vector<Texture> texturesPenguin;
    texturesPenguin.push_back(Texture(texPenguin, "texture_diffuse"));

    std::vector<Texture> texturesTree;
	texturesTree.push_back(Texture(texTree, "texture_diffuse"));

	// Create Obj files - easier :)
	// we can add here our textures :)
	MeshLoaderObj loader;
	Mesh sun = loader.loadObj("Resources/Models/sphere.obj");
	Mesh tree1 = loader.loadObj("Resources/Models/tree1.obj", texturesTree);
	Mesh tree2 = loader.loadObj("Resources/Models/tree2.obj", texturesTree);
	Mesh tree3 = loader.loadObj("Resources/Models/tree3.obj", texturesTree);
	Mesh penguin = loader.loadObj("Resources/Models/Penguin.obj", texturesPenguin);
	Mesh plane = loader.loadObj("Resources/Models/plane1.obj", textures2);
	Mesh ocean = loader.loadObj("Resources/Models/plane1.obj", textures3);
	Mesh box = loader.loadObj("Resources/Models/cube.obj", textures4);

	// TODO: refactor mess... idk 1-2h
	// Mesh loader::createMesh(objpath, texture-type from using above)

	glfwSetInputMode(pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// TODO: holding Z to deactivate this is stupid.. 

	//check if we close the window or press the escape button
	while (!window.isPressed(GLFW_KEY_ESCAPE) &&
		glfwWindowShouldClose(window.getWindow()) == 0)
	{
		window.clear();

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

        double oldMousePosX = mousePosX, oldMousePosY = mousePosY;
		
        glfwGetCursorPos(pWindow, &mousePosX, &mousePosY);
        mouseDeltaX = oldMousePosX - mousePosX;
        mouseDeltaY = oldMousePosY - mousePosY;
		// mouse camera handling

		processKeyboardInput();

		// TODO: refactor all this mess below... maybe
		// should only have transforms here and maybe imgui
		// poate de facut clasa uniformHandler(id) -> instanta skyHandler, oceanHandler, etc

		 //// Code for the light ////

		sunShader.use();

		glm::mat4 ProjectionMatrix = glm::perspective(90.0f, window.getWidth() * 1.0f / window.getHeight(), 0.1f, 10000.0f);
		glm::mat4 ViewMatrix = glm::lookAt(camera.getCameraPosition(), camera.getCameraPosition() + camera.getCameraViewDirection(), camera.getCameraUp());

		GLuint MatrixID = glGetUniformLocation(sunShader.getId(), "MVP");

		//Test for one Obj loading = light source

		glm::mat4 ModelMatrix = glm::mat4(1.0);
		ModelMatrix = glm::rotate(ModelMatrix, sliderValue * 90, glm::vec3(1.0f, 0.0f, 0.0f));
		ModelMatrix = glm::translate(ModelMatrix, lightPos);
		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

		sun.draw(sunShader);

		//// End code for the light ////

		///// skybox ////

		skyShader.use();

        GLuint MatrixID2 = glGetUniformLocation(skyShader.getId(), "MVP");
        GLuint ModelMatrixID = glGetUniformLocation(skyShader.getId(), "model");

		ModelMatrix = glm::mat4(1.0);
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-200.0f, -50.0f, -300.0f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(250.0f));
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
		glUniform3f(glGetUniformLocation(skyShader.getId(), "lightColor"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(skyShader.getId(), "lightPos"), lightPos.x, lightPos.y, lightPos.z);
		glUniform3f(glGetUniformLocation(skyShader.getId(), "viewPos"), camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		box.draw(skyShader);

        // trees draw

		// TODO omg refactor these IDs - 1h
        MatrixID2 = glGetUniformLocation(shader.getId(), "MVP");
        ModelMatrixID = glGetUniformLocation(shader.getId(), "model");

        ModelMatrix = glm::mat4(1.0);
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-85.0f, -5.0f, 90.0f));
        ModelMatrix = glm::scale(ModelMatrix, glm::vec3(0.25f));
        MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
        glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        glUniform3f(glGetUniformLocation(shader.getId(), "lightColor"), lightColor.x, lightColor.y, lightColor.z);
        glUniform3f(glGetUniformLocation(shader.getId(), "lightPos"), lightPos.x, lightPos.y, lightPos.z);
        glUniform3f(glGetUniformLocation(shader.getId(), "viewPos"), camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);
		tree1.draw(shader);
		// todo: disgusting.... 10min

		ModelMatrix = glm::mat4(1.0);
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-125.0f, -5.0f, -25.0f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(0.18f));
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
		tree2.draw(shader);
		// todo: disgusting....

		ModelMatrix = glm::mat4(1.0);
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(35.0f, -5.0f, 20.0f));
        ModelMatrix = glm::scale(ModelMatrix, glm::vec3(0.22f));
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
		tree3.draw(shader);
		// todo: disgusting....

        // penguin obj drawing

        shader.use();

        MatrixID2 = glGetUniformLocation(shader.getId(), "MVP");
        ModelMatrixID = glGetUniformLocation(shader.getId(), "model");

        ModelMatrix = glm::mat4(1.0);
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.0f, -55.0f, 400.0f));
        ModelMatrix = glm::scale(ModelMatrix, glm::vec3(10.0f));
        ModelMatrix = rotateAroundPoint(ModelMatrix, glm::vec3(0.0f, 2.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), -100.0f, currentFrame);

        MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
        glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        glUniform3f(glGetUniformLocation(shader.getId(), "lightColor"), lightColor.x, lightColor.y, lightColor.z);
        glUniform3f(glGetUniformLocation(shader.getId(), "lightPos"), lightPos.x, lightPos.y, lightPos.z);
        glUniform3f(glGetUniformLocation(shader.getId(), "viewPos"), camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);
        penguin.draw(shader);
		
		///// Test plane Obj file //////

		mountainShader.use();

        GLuint mountainMVPMatrixID = glGetUniformLocation(mountainShader.getId(), "MVP");
		GLuint mountainModelMatrixID = glGetUniformLocation(mountainShader.getId(), "model");

		ModelMatrix = glm::mat4(1.0);
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.0f, -20.0f, 0.0f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(0.3f));
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(mountainMVPMatrixID, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(mountainModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

		plane.draw(mountainShader);

        glUniform3f(glGetUniformLocation(mountainShader.getId(), "lightColor"), lightColor.x, lightColor.y, lightColor.z);
        glUniform3f(glGetUniformLocation(mountainShader.getId(), "lightPos"), lightPos.x, lightPos.y, lightPos.z);
        glUniform3f(glGetUniformLocation(mountainShader.getId(), "viewPos"), camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// ocean draw

		oceanShader.use(); // TODO: shader.use() and then getIDing on same shader - refactor 1h

        ModelMatrix = glm::mat4(1.0);
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.0f, -25.0f, 0.0f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(2.0f));
        MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
        glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

		ocean.draw(oceanShader); // refactor no oceanShader (or no oceanShader.use())

        glUniform3f(glGetUniformLocation(oceanShader.getId(), "lightColor"), lightColor.x, lightColor.y, lightColor.z);
        glUniform3f(glGetUniformLocation(oceanShader.getId(), "lightPos"), lightPos.x, lightPos.y, lightPos.z);
        glUniform3f(glGetUniformLocation(oceanShader.getId(), "viewPos"), camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);
		glUniform1f(glGetUniformLocation(oceanShader.getId(), "time"), currentFrame);

        // imgui ---->

		// deactivate mouse camera aiming if Z is held down
		// for being able to interact with ImGui
        if (window.isPressed(GLFW_KEY_Z))
        {
            glfwSetInputMode(pWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
        else
        {
            glfwSetInputMode(pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }

		if (toggleUI)
		{
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			{
				ImGui::Text("Player Position: %.2fx, %.2fy, %.2fz", camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);
				ImVec2 imSize(100, 20);
				ImGui::SetNextWindowSize(imSize);
				if (ImGui::Button("Hide Text"))
				{
					toggleUI = !toggleUI;
				}

				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
				const char* label = "Sun Example";
				const char* format = "%.2f";
				ImGui::SliderFloat("Sun Modifier", &sliderValue, 0.0f, 1.0f, "%.2f", 1.0f);
			}

			ImGui::Render();

			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

        // <---- end imgui

		window.update();
	}
}

// todo: sprint if ever do fps camera 
float speedRotate = 0.002f;
void processKeyboardInput()
{
	float cameraSpeed = 30 * deltaTime;

	//translation
	if (window.isPressed(GLFW_KEY_W))
		camera.keyboardMoveFront(cameraSpeed);
	if (window.isPressed(GLFW_KEY_S))
		camera.keyboardMoveBack(cameraSpeed);
	if (window.isPressed(GLFW_KEY_A))
		camera.keyboardMoveLeft(cameraSpeed);
	if (window.isPressed(GLFW_KEY_D))
		camera.keyboardMoveRight(cameraSpeed);

	camera.rotateOy(mouseDeltaX * speedRotate);
	camera.rotateOx(-mouseDeltaY * speedRotate * 2);
}

// todos not in main
// 2. fix camera class
// 3. fix shader folder -> glsls in shaders/glsl
// 4. texture loading not just .bmp
// 6. imgui move to dependencies
// 7. keyboard input to inputclass
// 8. includes in all files shouldn't be relative paths, ajust project settinsg

// move all these todos to github issues...