#include "Shader.h"
#include "stb_image.h"
#include "Camera.h"
#include <vector>

//Globals

glm::vec3 playerDirection = glm::vec3(1, 0, 0);
glm::vec3 foodPosition = glm::vec3(2, 1, 2);
vector<glm::vec3> tailPositions = { glm::vec3(0, 1, 0), glm::vec3(-1, 1, 0) };
vector<glm::vec3> tailColors = { glm::vec3(1.0, 0.0, 1.0), glm::vec3(1.0, 0.1, 1.0) };


Camera camera(glm::vec3(4.29852, 10.0845, 14.0845));

#pragma region callbacks
void windowResizeCallback(GLFWwindow* window, int width, int height){
	glViewport(0, 0, width, height);
}

void mouseCallback(GLFWwindow* window, double x, double y){
    if(!camera.unlocked) return;
    camera.processMouseInput(x, y);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods){
    
    if(action != GLFW_PRESS) return;

    if (key == GLFW_KEY_UP && playerDirection != glm::vec3(0, 0, 1)) {
        playerDirection = glm::vec3(0, 0, -1);
        return;
    }

    if (key == GLFW_KEY_DOWN && playerDirection != glm::vec3(0, 0, -1)) {
        playerDirection = glm::vec3(0, 0, 1);
        return;

    }

    if (key == GLFW_KEY_RIGHT && playerDirection != glm::vec3(-1, 0, 0)) {
        playerDirection = glm::vec3(1, 0, 0);
        return;
    }

    if (key == GLFW_KEY_LEFT && playerDirection != glm::vec3(1, 0, 0)) {
        playerDirection = glm::vec3(-1, 0, 0);
        return;
    }
}

void processInput(GLFWwindow* window, float deltaTime){

    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }

    if(glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS){
        camera.unlocked = !camera.unlocked;
        if(!camera.unlocked){
            camera.position = glm::vec3(4.29852, 10.0845, 14.0845);
        }
    }

    if(!camera.unlocked) return;

    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        camera.processKeyboardInput(GLFW_KEY_W, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.processKeyboardInput(GLFW_KEY_S, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.processKeyboardInput(GLFW_KEY_A, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.processKeyboardInput(GLFW_KEY_D, deltaTime);
    }



}

#pragma endregion

#pragma region drawing

void drawFloor(Shader &floorShader){
    for (size_t x = 0; x < 10; x++)
    {
        for (size_t y = 0; y < 10; y++)
        {
            glm::mat4 transform = glm::mat4(1.0);

            transform = glm::translate(transform, glm::vec3(x, 0, y));

            floorShader.setMat4("model", transform);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
    }
}

void drawFood(Shader &foodShader, glm::mat4 viewMat, glm::mat4 projMat){
    //Drawing food
    foodShader.use();
    foodShader.setMat4("view", viewMat);
    foodShader.setMat4("projection", projMat);

    glm::mat4 foodTransform = glm::mat4(1.0);
    foodTransform = glm::translate(foodTransform, foodPosition);
    foodTransform = glm::scale(foodTransform, glm::vec3(0.5, 0.5, 0.5));

    foodShader.setMat4("model", foodTransform);

    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void drawSnake(Shader &snakeShader){
    for (int i = 0; i < tailPositions.size(); i++) {
        glm::mat4 tailTransform = glm::mat4(1.0);
        tailTransform = glm::translate(tailTransform, tailPositions[i]);
        snakeShader.setFloat3("inColor", tailColors[i].x, tailColors[i].y, tailColors[i].z);
        snakeShader.setMat4("model", tailTransform);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}

#pragma endregion

#pragma region updates and checks

void updateSnake(){
    //Update segment positions
    for (int i = tailPositions.size() - 1; i > 0; i--) {
        tailPositions[i] = tailPositions[i - 1];
    }

    tailPositions[0] += playerDirection;
}

void checkSnakeCollision(){
    //Check snake collision
    for (size_t i = 1; i < tailPositions.size(); i++)
    {
        if (tailPositions[0] == tailPositions[i]) {
            glfwTerminate();
        }
    }
}

void checkFoodCollision(){
    if (tailPositions[0] == foodPosition) {
        foodPosition.x = rand() % 10;
        foodPosition.z = rand() % 10;
        glm::vec3 lastSegment = tailPositions[tailPositions.size() - 1];
        glm::vec3 dir = glm::vec3(0, 0, 0);
        if (tailPositions.size() > 1) {
            dir = lastSegment - tailPositions[tailPositions.size() - 2];
        }
        tailPositions.push_back(lastSegment + dir);
        //glm::vec3 color = glm::vec3((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX);
        glm::vec3 color = glm::vec3(1.0, (float)tailPositions.size() / 10, 1.0);
        tailColors.push_back(color);
    }
}

void checkWallCollision(){
    //Check wall collision
    if (tailPositions[0].x >= 10 || tailPositions[0].z >= 10 || tailPositions[0].x < 0 || tailPositions[0].z < 0) {
        glfwTerminate();
    }
}

#pragma endregion



int main(){
	#pragma region Initialization, Window creation and callbacks
    //Initialize GLFW
	glfwInit();

	//Set hints
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Create the window
	GLFWwindow* window = glfwCreateWindow(800, 600, "Hello Triangle", NULL, NULL);

	//Error checking of window creation
	if(window == NULL){
		std::cerr<<"Error creating GLFW window"<<std::endl;
		return -1;
	}

	//FIRST make context current THEN init glad
	glfwMakeContextCurrent(window);

	//Init GLAD
	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
		std::cerr<<"Error initializing GLAD"<<std::endl;
	}

	//Set the viewport of the window
	glViewport(0, 0, 800, 600);

    //Disable cursor
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//set callbacks
	glfwSetFramebufferSizeCallback(window, windowResizeCallback);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetKeyCallback(window, keyCallback);

    #pragma endregion

    float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    Shader cubeShader("vertex.glsl", "fragment1.glsl", "Cube1");
    Shader foodShader("vertex.glsl", "fragment2.glsl", "Food1");
    
    #pragma region buffers
    //Vao
    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);


    //Vbo
    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //Vertex attrib pointers
    //Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    //Tex coords
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(2);
    #pragma endregion

    #pragma region texturing
    ///Texturing

    //Loading image 1
    int width, height, channels;
    unsigned char* data = stbi_load("container.jpg", &width, &height, &channels, 0);

    //Generating texture
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    
    //Settings
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    //Loading image 2
    int width2, height2, channels2;
    unsigned char* data2 = stbi_load("awesomeface.png", &width2, &height2, &channels2, 0);
        
    unsigned int texture2;
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width2, height2, 0, GL_RGBA, GL_UNSIGNED_BYTE, data2);
    glGenerateMipmap(GL_TEXTURE_2D);

    cubeShader.use();
    cubeShader.setInt("myTexture", 0);
    cubeShader.setInt("myTexture2", 1);

    #pragma endregion

    #pragma region Transformations

    //Transformations
    glm::mat4 view = glm::mat4(1.0);
    view = glm::translate(view, glm::vec3(0.0, 0.0, -3.0f));

    glm::mat4 proj = glm::mat4(1.0);
    proj = glm::perspective((float)(glm::radians(45.0f)), (float)800/600, 0.1f, 100.f);


    #pragma endregion

    glEnable(GL_DEPTH_TEST);

    float deltaTime = 0;
    float lastFrame = 0;
    float lastUpdate = -1;
    

	//Main loop
	while(!glfwWindowShouldClose(window)){

        float now = glfwGetTime();
        deltaTime = now - lastFrame;
        lastFrame = now;

		//Clearing
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //And setting only the first
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
		glBindVertexArray(vao);

        cubeShader.use();
        view = camera.getViewMatrix();
        cubeShader.setMat4("view", view);
        cubeShader.setMat4("projection", proj);

        //Drawing floor, setting the color to default, so the texture doesnt change
        cubeShader.setFloat3("inColor", 1, 1, 1);
        drawFloor(cubeShader);

        //Tick
        if(glfwGetTime() >= lastUpdate + 0.2f){

            checkSnakeCollision();
            checkFoodCollision();
            checkWallCollision();

            updateSnake();

            lastUpdate = glfwGetTime();
        }

        //Adding the second texture unit, and mixing to the snake
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        //Drawing segments
        drawSnake(cubeShader);

        //Then resetting the buffer so the floor will only use the first unit(0th)
        glBindTexture(GL_TEXTURE_2D, 0);

        drawFood(foodShader, view, proj);

        processInput(window, deltaTime);

		//Swap and pollevents
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//Terminate the window
	glfwTerminate();
}