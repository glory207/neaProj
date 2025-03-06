
#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader.h"
#include <iostream>
#include<glm/gtc/matrix_transform.hpp>
#include<gl/GL.h>
#include"object.h"
#include "initMaze.h"
#include "Enemy.h"
#include "camera.h"
#include "PlayerClass.h"
#include "UIelement.h"
#include <random>
#include <thread>
#include <queue>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

unsigned int SCR_WIDTH = 1600;
unsigned int SCR_HEIGHT = 900;
unsigned int OFF_WIDTH = 0;
unsigned int OFF_HEIGHT = 0;
camera cam;
vector<Light> ligh;
Light tourch;
PlayerClass player;
PathFind pathf;
queue<PathFind*> pathfq;
vector<Enemy> enmi;
float resolution = 1.0f;
bool MouseLocked = false;
bool ELocked = false;
bool erere = false;
int adee = 1;
float adeee = 0;
vec2 LightSetings = vec2(1.1,0.25);
float ArmL = 0.27f;
float ArmL2 = 0.0f;
float ArmL3 = 1.25;
float deltaTime = 1.0f;
float UTime = 0.0f;
float sceeee = 1.0f;
Maze mz;
vec3 ray;
vec3 pathend;

// Define the random number generator and distribution
std::random_device rd;  // Seed generator
std::mt19937 gen(rd()); // Mersenne Twister engine
std::uniform_real_distribution<float> Rand(0.0f, 1.0f); // Range [0, 1]
GLFWwindow* window;




int main2()
{
#pragma region Start


    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "NEA", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

#pragma endregion
#pragma region Update
  
    glfwSwapInterval(1);
    
    while (!glfwWindowShouldClose(window))
    {
       

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

#pragma endregion
#pragma region End

    glfwTerminate();

    return 0;

#pragma endregion


}


























void foo(int Z)
{
    while (!glfwWindowShouldClose(window))
    {
        if (!pathfq.empty()) {

            PathFind* pt = pathfq.front();
            pt->path = mz.getpath(pt->startpos().x, pt->startpos().y, pt->startpos().z, pt->endpos().x, pt->endpos().y, pt->endpos().z
            );
            pt->OnPath = true;
            pt->WaitingForPath = false;
            pt->pathP = 0.0f;
            pathfq.pop();

        }
    }
}


int main()
{
    #pragma region Start


    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "NEA", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    GLuint lyrs[6] = { 0,0,0,0,0,0 };
    unsigned int shaderProgram = initShader("shaders/def.vert", "shaders/def.geom", "shaders/def.frag");
    unsigned int shaderLightProgram = initShader("shaders/def.vert", "shaders/def.geom", "shaders/light.frag");
    unsigned int shaderShadowProgram = initShader("shaders/shadow.vert", "shaders/shadow.geom", "shaders/shadow.frag");
    unsigned int shaderMazeProgram = initShader("shaders/maze.vert", "shaders/maze.geom", "shaders/maze.frag"); 
    unsigned int ShaderPost = initShader("shaders/cam.vert", "shaders/camPost.frag"); 
    unsigned int ShaderUI = initShader("shaders/cam.vert", "shaders/UI.frag"); 
    unsigned int ShaderDebug = initShader("shaders/debug.vert", "shaders/debug.geom", "shaders/maze.frag");
    createShadowFramebufferCube shBF = createShadowFramebufferCube(500);

    int rdi = 5;
    cout << "size" << endl;
    int c = 20;
    
    while (c <= 1 || c > 50)
    {
        if(c>50)
        {
            cout << "too large" << endl;
        }
        cout << "size" << endl;
        
        float cc;
        cin >> cc;
        c = int(cc);
    } 
    
    
    mz = Maze(&ligh,c);  
    cam = camera(glm::vec3(0.0f, 0.2f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)); 
    
    player = PlayerClass(vec3(0.0f, 0.0f, 0.5f), vec3(0.0f, -3.1415 / 2.0f, 0.0f));
    for (int i = 0; i < 30; i++)
    {
        enmi.push_back(Enemy(vec3(int(Rand(gen) * mz.count) * mz.size, 0.0f, int(Rand(gen) * mz.count) * mz.size), vec3(0.0f, -3.1415 / 2.0f, 0.0f)));
        mz.collide(&enmi[i].pos, nullptr, vec2(0.07f, 0.0f));
        enmi[i].pos.y = 0;
    } 
    
    float vertices[] = {
        0.0f, -0.01f, 0.0f,
        0.0f, 0.01f, 0.0f,
         -0.01f,0.0f, 0.0f,
         0.01f, 0.0f,0.0f
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);


    float sensitivity = 0.001f;
    double preTime = 0.0;
    double curTime = 0.0;
    double timeDif;
    unsigned int counter = 0;
    cam.updateSize(vec2(SCR_WIDTH, SCR_HEIGHT) * resolution);
    #pragma endregion
    #pragma region Update
    tourch = Light(vec3(0));
    glDepthFunc(GL_LEQUAL);
    glfwSwapInterval(1);
    SpObj testObj = SpObj(vec3(0), vec3(0), glm::vec3(0.09f), initCubeBuffer({9}), 27, 0);
    UImenue menue = UImenue();
    thread th3(foo, 3);
    while (!glfwWindowShouldClose(window))
    {
        curTime = glfwGetTime();
        timeDif = curTime - preTime;
        counter++;
        if (timeDif >= 0.1) {
            std::string FPS = std::to_string((int)((1.0 / timeDif) * counter));
            std::string ms = std::to_string((int)((timeDif / counter) * 1000));
            std::string title = "EPQ:  " + FPS + "FPS  " + ms + "ms " + std::to_string(curTime) + "curTime";
            glfwSetWindowTitle(window, title.c_str());
            preTime = curTime;
            deltaTime = (float)((1.0 / counter) * timeDif);

            counter = 0;
        }
        if (SCR_WIDTH > 0 && SCR_HEIGHT > 0 && timeDif < 1.0) {


            if (MouseLocked)
            {

                // Stores the coordinates of the cursor
                double mouseX;
                double mouseY;
                // Fetches the coordinates of the cursor
                glfwGetCursorPos(window, &mouseX, &mouseY);

                // Normalizes and shifts the coordinates of the cursor such that they begin in the middle of the screen
                // and then "transforms" them into degrees 
                cam.rot.x += sensitivity * (float)((SCR_HEIGHT / 2) + OFF_HEIGHT - mouseY);
                cam.rot.y += sensitivity * (float)((SCR_WIDTH / 2) + OFF_WIDTH - mouseX);
                // cout << sensitivity * (float)((SCR_HEIGHT / 2) - mouseY) / SCR_HEIGHT << " " << sensitivity * (float)((SCR_WIDTH / 2) - mouseX) / SCR_WIDTH << endl;
                player.inp->rot.y = cam.rot.y;

                // Sets mouse cursor to the middle of the screen so that it doesn't end up roaming around
                glfwSetCursorPos(window, (SCR_WIDTH / 2) + OFF_WIDTH, (SCR_HEIGHT / 2) + OFF_HEIGHT);


                processInput(window);
                player.update(deltaTime);
                for (int i = 0; i < enmi.size(); i++)enmi[i].update(deltaTime, cam.pos, &mz, &pathfq);
                vec3 ty = player.inp->acc;
                vec3 tp = player.inp->pos;
                player.inp->Grounded = mz.collide(&player.inp->pos, nullptr, vec2(0.075f, 0.0f));
                //for (int i = 0; i < enmi.size(); i++)mz.collide(&enmi[i].pos, nullptr, vec2(0.07f, 0.0f));
                if (pathf.pathP+1 >= pathf.path.size()) {
                    pathf.OnPath = false;
                    pathf.pathP = 0;
                }
                if (pathf.OnPath) {
                    vec2 psss1 = pathf.path[int(pathf.pathP)]->pos * mz.size;
                   vec2 psss2 = pathf.path[int(pathf.pathP+1)]->pos * mz.size;
                  // player.inp->pos = vec3(psss1.x, player.inp->pos.y, psss1.y) + ((vec3(psss2.x, player.inp->pos.y, psss2.y)) - vec3(psss1.x, player.inp->pos.y, psss1.y)) * (pathP-int(pathP));
                  // pathP += deltaTime * 3.0f;
                    player.inp->acc += 0.2f*((player.inp->pos-vec3(psss2.x, player.inp->pos.y, psss2.y)) / distance(psss2, vec2(player.inp->pos.x, player.inp->pos.z)));
                    if(distance(psss1,vec2(player.inp->pos.x, player.inp->pos.z)) > distance(psss2, vec2(player.inp->pos.x, player.inp->pos.z)))
                        pathf.pathP ++;

                }
                int playerIndex = int((player.inp->pos.x + 1) / mz.size) + int((player.inp->pos.z + 1) / mz.size) * mz.count;
                glDisable(GL_BLEND);


                glViewport(0, 0, 500, 500);

                if (UTime > 10) UTime = 0;
                float cpt = 0.1f;
                if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
                {
                    tourch.pos = cam.pos + vec3(0, 0.1, 0);
                    if (length(tourch.pos - cam.pos) < 7.0f) {

                        tourch.activate(true);
                        shBF.bind(true, 0, tourch.depthTex);
                        glUseProgram(shaderShadowProgram);
                        std::string ii[6];
                        glm::mat4 matr4[6];
                        for (int i = 0; i < 6; i++)
                        {
                            float fov = (0.5f * 3.14159265358979323846f);
                            float zNear = 0.01f;
                            float zFar = 100.0f;
                            glm::mat4 projectionMatrix;
                            glm::mat4 view;
                            projectionMatrix = glm::perspective(fov, 1.0f, zNear, zFar);
                            view = glm::lookAt(tourch.pos, tourch.pos + shBF.target[i], shBF.up[i]);
                            view = projectionMatrix * view;
                            matr4[i] = view;
                            ii[i] = "uProjectionMatrix[" + std::to_string(i) + "]";
                            glUniformMatrix4fv(glGetUniformLocation(shaderShadowProgram, ii[i].c_str()), 1, GL_FALSE, glm::value_ptr(matr4[i]));
                        }
                        glUniform3f(glGetUniformLocation(shaderShadowProgram, "lpos"), tourch.pos.x, tourch.pos.y, tourch.pos.z);
                        player.draw(curTime, shaderShadowProgram);
                        mz.obj.draw(shaderShadowProgram);
                        for (int j = 0; j < rdi * rdi; j++)
                        {
                            if (playerIndex + (j % rdi) - (rdi / 2) + (j / rdi - (rdi / 2)) * mz.count >= 0 &&
                                playerIndex + (j % rdi) - (rdi / 2) + (j / rdi - (rdi / 2)) * mz.count < mz.nodes.size()) {
                                for (Furniture furn : mz.nodes[playerIndex + (j % rdi) - (rdi / 2) + (j / rdi - (rdi / 2)) * mz.count].fur) {
                                    furn.obj.draw(shaderShadowProgram);
                                }
                            }

                        }

                    }
                    else {
                        tourch.activate(false);
                    }


                }



                for (int t = 0; t < ligh.size(); t++) {

                    if (length(ligh[t].pos - cam.pos) < 7.0f) {

                        ligh[t].activate(true);

                        if (ligh[t].active2 || UTime == t % 10) shBF.bind(true, 0, ligh[t].depthTexPre);
                        else
                        {
                            glBindTexture(GL_TEXTURE_CUBE_MAP, ligh[t].depthTexPre);
                            for (GLuint face = 0; face < 6; ++face) {
                                glCopyImageSubData(ligh[t].depthTexPre, GL_TEXTURE_CUBE_MAP, 0, 0, 0, face,
                                    ligh[t].depthTex, GL_TEXTURE_CUBE_MAP, 0, 0, 0, face,
                                    ligh[t].size, ligh[t].size, 1);
                            }
                            shBF.bind(false, 0, ligh[t].depthTex);

                        }
                        glUseProgram(shaderShadowProgram);
                        std::string ii[6];
                        glm::mat4 matr4[6];
                        for (int i = 0; i < 6; i++)
                        {
                            float fov = (0.5f * 3.14159265358979323846f);
                            float zNear = 0.01f;
                            float zFar = 100.0f;
                            glm::mat4 projectionMatrix;
                            glm::mat4 view;
                            projectionMatrix = glm::perspective(fov, 1.0f, zNear, zFar);
                            view = glm::lookAt(ligh[t].pos, ligh[t].pos + shBF.target[i], shBF.up[i]);
                            view = projectionMatrix * view;
                            matr4[i] = view;
                            ii[i] = "uProjectionMatrix[" + std::to_string(i) + "]";
                            glUniformMatrix4fv(glGetUniformLocation(shaderShadowProgram, ii[i].c_str()), 1, GL_FALSE, glm::value_ptr(matr4[i]));
                        }
                        glUniform3f(glGetUniformLocation(shaderShadowProgram, "lpos"), ligh[t].pos.x, ligh[t].pos.y, ligh[t].pos.z);
                        if (ligh[t].active2 || UTime == t % 50) {
                            mz.obj.draw(shaderShadowProgram);

                            for (int j = 0; j < rdi * rdi; j++)
                            {
                                if (playerIndex + (j % rdi) - (rdi / 2) + (j / rdi - (rdi / 2)) * mz.count >= 0 &&
                                    playerIndex + (j % rdi) - (rdi / 2) + (j / rdi - (rdi / 2)) * mz.count < mz.nodes.size()) {
                                    for (Furniture furn : mz.nodes[playerIndex + (j % rdi) - (rdi / 2) + (j / rdi - (rdi / 2)) * mz.count].fur) {
                                        furn.obj.draw(shaderShadowProgram);
                                    }
                                }

                            }
                        }
                        else
                        {
                            player.draw(curTime, shaderShadowProgram);
                            for (int i = 0; i < enmi.size(); i++)if(distance(player.inp->pos,enmi[i].pos)<7.0) enmi[i].draw(deltaTime, shaderShadowProgram);
                        }
                    }
                    else {
                        ligh[t].activate(false);


                       
                    }


                }

                UTime += deltaTime;
                // cout << std::to_string(nb1) << " " << std::to_string(nb2) << " " << std::to_string(nb3) << endl;
                cam.GFB.bind(true);
                glUseProgram(shaderProgram);
                mat4 ma = cam.matrix(float(SCR_WIDTH) / float(SCR_HEIGHT));
                glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "uProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(ma));
                glUniform3f(glGetUniformLocation(shaderProgram, "camPos"), cam.pos.x, cam.pos.y, cam.pos.z);
                glEnable(GL_CULL_FACE);
                glCullFace(GL_FRONT);
                mz.obj.draw(shaderProgram);
                glDisable(GL_CULL_FACE);

                player.draw(curTime, shaderProgram);
                for (int i = 0; i < enmi.size(); i++)if (distance(player.inp->pos, enmi[i].pos) < 7.0)enmi[i].draw(curTime, shaderProgram);

                for (int j = 0; j < rdi * rdi; j++)
                {
                    if (playerIndex + (j % rdi) - (rdi/2) + (j / rdi - (rdi/2)) * mz.count >= 0 &&
                        playerIndex + (j % rdi) - (rdi/2) + (j / rdi - (rdi/2)) * mz.count < mz.nodes.size()) {
                        for (Furniture furn : mz.nodes[playerIndex + (j % rdi) - (rdi / 2) + (j / rdi - (rdi / 2)) * mz.count].fur) {
                            furn.obj.draw(shaderProgram);
                        }
                    }

                }

                glUseProgram(shaderLightProgram);
                glUniformMatrix4fv(glGetUniformLocation(shaderLightProgram, "uProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(ma));
                for (int t = 0; t < ligh.size(); t++) {
                    if (ligh[t].active)
                    {
                        glUniform3f(glGetUniformLocation(shaderLightProgram, "color"), ligh[t].col.x, ligh[t].col.y, ligh[t].col.z);
                        glUniform1f(glGetUniformLocation(shaderLightProgram, "tim"), (int)(curTime * 10));
                        ligh[t].obj.rot.y = atan2(ligh[t].obj.pos.x - cam.pos.x, ligh[t].obj.pos.z - cam.pos.z);
                        ligh[t].obj.draw(shaderLightProgram);
                    }
                }
                    
                

                cam.FB.bind(true);

                glEnable(GL_BLEND); // Enable depth testing
                glDepthFunc(GL_LEQUAL); // Near things obscure far things
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                glBlendEquation(GL_FUNC_ADD);
                glBlendFunc(GL_ONE, GL_ONE);



                glUseProgram(cam.shader);
                glUniform4f(glGetUniformLocation(cam.shader, "textureMatrix"), 0, 0, 1, 1);
                glUniform3f(glGetUniformLocation(cam.shader, "camPos"), cam.pos.x, cam.pos.y, cam.pos.z);
                glUniform1i(glGetUniformLocation(cam.shader, "light"), 0);
                glUniform1i(glGetUniformLocation(cam.shader, "uSamplerS"), 4);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, cam.GFB.ColTex);
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, cam.GFB.PosTex);
                glActiveTexture(GL_TEXTURE2);
                glBindTexture(GL_TEXTURE_2D, cam.GFB.NomTex);
                glActiveTexture(GL_TEXTURE3);
                glBindTexture(GL_TEXTURE_2D, cam.GFB.NomFTex);

                std::string ii = " ";
                if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
                {
                    int t = 0;
                    //  if (tourch.active)
                    {
                        glUniformMatrix4fv(glGetUniformLocation(cam.shader, "rotcam"), 1, GL_FALSE, value_ptr(cam.rotationMatrix));

                        glActiveTexture(GL_TEXTURE4);
                        glBindTexture(GL_TEXTURE_CUBE_MAP, tourch.depthTex);

                        glUniform1f(glGetUniformLocation(cam.shader, "ConeAngle"), 0.68);
                        glUniform3f(glGetUniformLocation(cam.shader, "lightPos"), tourch.pos.x, tourch.pos.y, tourch.pos.z);
                        glUniform3f(glGetUniformLocation(cam.shader, "lightCol"), 1.0, 1.0, 1.0);
                        glUniform2f(glGetUniformLocation(cam.shader, "LightSetings"), 20.0, 0.3f);


                        cam.draw(cam.shader);

                    }


                }
                for (int t = 0; t < ligh.size(); t++) {

                    if (ligh[t].active)
                    {
                        glUniformMatrix4fv(glGetUniformLocation(cam.shader, "rotcam"), 1, GL_FALSE, value_ptr(cam.rotationMatrix));

                        glActiveTexture(GL_TEXTURE4);
                        glBindTexture(GL_TEXTURE_CUBE_MAP, ligh[t].depthTex);

                        glUniform1f(glGetUniformLocation(cam.shader, "ConeAngle"), 0);
                        glUniform3f(glGetUniformLocation(cam.shader, "lightPos"), ligh[t].pos.x, ligh[t].pos.y, ligh[t].pos.z);
                        glUniform3f(glGetUniformLocation(cam.shader, "lightCol"), ligh[t].col.x, ligh[t].col.y, ligh[t].col.z);
                        glUniform2f(glGetUniformLocation(cam.shader, "LightSetings"), LightSetings.x + sin(curTime) * 0.1f, LightSetings.y);


                        cam.draw(cam.shader);

                    }


                }


                glUniform1i(glGetUniformLocation(cam.shader, "light"), 1);
                glUniform1f(glGetUniformLocation(cam.shader, "brightness"), *menue.settings.brightness * 0.05);
                cam.draw(cam.shader);
                glDisable(GL_BLEND);


                cam.PFB1.bind(true);
                glUseProgram(ShaderPost);
                glUniform4f(glGetUniformLocation(ShaderPost, "textureMatrix"), 0, 0, 1, 1);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, cam.FB.ColTex);
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, cam.GFB.PosTex);
                cam.draw(ShaderPost);

                cam.MFB.bind(true);
                glUseProgram(shaderMazeProgram);

                if (glfwGetKey(window, GLFW_KEY_KP_7) == GLFW_PRESS)   sceeee *= 1.1f;
                if (glfwGetKey(window, GLFW_KEY_KP_8) == GLFW_PRESS)   sceeee /= 1.1f;
                glUniform1f(glGetUniformLocation(shaderMazeProgram, "sc"), sceeee);
                glUniform1f(glGetUniformLocation(shaderMazeProgram, "sc1"), 1.0);
                glUniform3f(glGetUniformLocation(shaderMazeProgram, "col"), 0.0f, 0.7f, 0.7f);
                glUniform2f(glGetUniformLocation(shaderMazeProgram, "campos"), player.inp->pos.x / mz.size, player.inp->pos.z / mz.size);
                glUniform1f(glGetUniformLocation(shaderMazeProgram, "rt"), cam.rot.y);
                glBindVertexArray(mz.VAO);
                glDrawElements(GL_LINES, mz.MapBuffers.length, GL_UNSIGNED_INT, 0);
                glBindVertexArray(VAO);

                glUniform3f(glGetUniformLocation(shaderMazeProgram, "col"), 1.0f, 1.0f, 1.0f);
                glUniform1f(glGetUniformLocation(shaderMazeProgram, "rt"), cam.rot.y);
                glUniform3f(glGetUniformLocation(shaderMazeProgram, "col"), 1.0f, 0.0f, 0.0f);

                glUniform1f(glGetUniformLocation(shaderMazeProgram, "sc"), sceeee);
                glUniform1f(glGetUniformLocation(shaderMazeProgram, "sc1"), 3.0);
                
                    for (int j = 0; j < 9; j++)
                    {
                        if (playerIndex + (j % 3) - 1 + (j / 3 - 1) * mz.count >= 0 &&
                            playerIndex + (j % 3) - 1 + (j / 3 - 1) * mz.count < mz.nodes.size()) {
                            for (int k = 0; k < 9 * 9; k++)
                            {
                                CellGrid* cgr = &mz.nodes[playerIndex + (j % 3) - 1 + (j / 3 - 1) * mz.count].grid[k % 9][k / 9];
                                if (cgr->obstruction)
                                    glUniform3f(glGetUniformLocation(shaderMazeProgram, "col"), 0.0f, 1.0f, 0.0f);
                                else
                                    glUniform3f(glGetUniformLocation(shaderMazeProgram, "col"), 0.0f, 0.0f, 1.0f);
                   
                                glUniform2f(glGetUniformLocation(shaderMazeProgram, "campos"), -cgr->pos.x + player.inp->pos.x / mz.size, -cgr->pos.y + player.inp->pos.z / mz.size);
                                glDrawArrays(GL_LINES, 0, 4);
                            }
                        }
                   
                    }

                    for (int j = pathf.pathP; j < pathf.path.size(); j++)
                    {

                        glUniform3f(glGetUniformLocation(shaderMazeProgram, "col"), 1.0f, 0.0f, 1.0f);

                        glUniform2f(glGetUniformLocation(shaderMazeProgram, "campos"), -pathf.path[j]->pos.x + player.inp->pos.x / mz.size, -pathf.path[j]->pos.y + player.inp->pos.z / mz.size);
                        glDrawArrays(GL_LINES, 0, 4);


                    }
                   

                    glUniform1f(glGetUniformLocation(shaderMazeProgram, "sc1"), 10);
                    for (Landmark* landm : mz.Camps)
                    {
                        if (dynamic_cast<EnCamp*>(landm) != nullptr)
                        {

                            glUniform3f(glGetUniformLocation(shaderMazeProgram, "col"), 1.0f, 0.0f, 0.0f);


                        }
                        else if (dynamic_cast<LootSpot*>(landm) != nullptr)
                        {
                            glUniform3f(glGetUniformLocation(shaderMazeProgram, "col"), 1.0f, 1.0f, 0.0f);


                        }

                        glUniform2f(glGetUniformLocation(shaderMazeProgram, "campos"), -landm->Pos.x * mz.count + player.inp->pos.x / mz.size, -landm->Pos.y * mz.count + player.inp->pos.z / mz.size);
                        glDrawArrays(GL_LINES, 0, 4);
                    }

                    glUniform1f(glGetUniformLocation(shaderMazeProgram, "sc1"), 50);
                    glUniform3f(glGetUniformLocation(shaderMazeProgram, "col"), 0.5f, 1.0f, 1.0f);
                    for (int i = 0; i < enmi.size(); i++) {
                        glUniform2f(glGetUniformLocation(shaderMazeProgram, "campos"), (-enmi[i].pos.x + player.inp->pos.x) / mz.size, (-enmi[i].pos.z + player.inp->pos.z) / mz.size);
                        glDrawArrays(GL_LINES, 0, 4);
                    }

                    glUniform1f(glGetUniformLocation(shaderMazeProgram, "sc"), 2);
                    glUniform1f(glGetUniformLocation(shaderMazeProgram, "sc1"), 4.0);
                    glUniform3f(glGetUniformLocation(shaderMazeProgram, "col"), 1.0f, 0.0f, 0.0f);
                    glUniform1f(glGetUniformLocation(shaderMazeProgram, "rt"), 0.0);
                    glUniform2f(glGetUniformLocation(shaderMazeProgram, "campos"), 0, 0);
                    glDrawArrays(GL_LINES, 0, 4);

                glBindFramebuffer(GL_FRAMEBUFFER, 0);

                glViewport(OFF_WIDTH, OFF_HEIGHT, SCR_WIDTH, SCR_HEIGHT);


                glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
                glEnable(GL_DEPTH_TEST);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



                glUseProgram(cam.ScreenShader);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, cam.PFB1.ColTex);
                glUniform4f(glGetUniformLocation(cam.ScreenShader, "textureMatrix"), 0, 0, 1, 1);
                cam.drawScreen();
                glUseProgram(cam.MapShader);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, cam.MFB.ColTex);
                glUniform1i(glGetUniformLocation(cam.MapShader, "ColT2"), 0);
                glUniform4f(glGetUniformLocation(cam.MapShader, "textureMatrix"), 0, 0, 1, 1);
                glUniform2f(glGetUniformLocation(cam.MapShader, "screen"), SCR_WIDTH, SCR_HEIGHT);
                cam.drawScreen();

                glUseProgram(ShaderDebug);
                glUniformMatrix4fv(glGetUniformLocation(ShaderDebug, "mtr"), 1, GL_FALSE, glm::value_ptr(ma));
                glBindVertexArray(VAO);
                glLineWidth(5);
                for (int t = 0; t < ligh.size(); t++)
                {
                    ray = ligh[t].pos;
                    if (distance(ligh[t].pos,player.obj.pos)<LightSetings.x) {
                        vec3 DirectionOfRay = -normalize(player.obj.pos - ray);
                        vec3 startOfRay = player.obj.pos;
                        vec3 PR2 = ray;

                        for (int j = 0; j < 9; j++)
                        {
                            if (playerIndex + (j % 3) - 1 + (j / 3 - 1) * mz.count >= 0 &&
                                playerIndex + (j % 3) - 1 + (j / 3 - 1) * mz.count < mz.nodes.size()) {
                                int rp = playerIndex + (j % 3) - 1 + (j / 3 - 1) * mz.count;
                                for (int j = 0; j < mz.nodes[rp].fur.size(); j++)
                                {
                                    if (ligh[t].perch != vec2(rp,j)) {

                                    mat4 modelViewMatrix;
                                    modelViewMatrix = rotate(modelViewMatrix, mz.nodes[rp].fur[j].obj.rot.z, glm::vec3(0, 0, 1));
                                    modelViewMatrix = rotate(modelViewMatrix, mz.nodes[rp].fur[j].obj.rot.y, glm::vec3(0, 1, 0));
                                    modelViewMatrix = rotate(modelViewMatrix, mz.nodes[rp].fur[j].obj.rot.x, glm::vec3(1, 0, 0));
                                    modelViewMatrix = scale(modelViewMatrix, mz.nodes[rp].fur[j].obj.sca);
                                    vec3 P[6] = {
                                        modelViewMatrix * vec4(1,0,0,1) ,
                                        modelViewMatrix * vec4(0,1,0,1) ,
                                        modelViewMatrix * vec4(0,0,1,1) ,
                                        modelViewMatrix * vec4(-1,0,0,1),
                                        modelViewMatrix * vec4(0,-1,0,1),
                                        modelViewMatrix * vec4(0,0,-1,1)
                                    };

                                    bool clip = false;
                                    for (int i = 0; i < 6; i++)
                                    {

                                        //for math
                                        vec3 PP1 = (P[i] + P[(i + 1) % 6] + P[(i + 2) % 6]) + mz.nodes[rp].fur[j].obj.pos;
                                        vec3 PP2 = (P[i] + P[(i + 1) % 6] - P[(i + 2) % 6]) + mz.nodes[rp].fur[j].obj.pos;
                                        vec3 PP3 = (P[i] - P[(i + 1) % 6] - P[(i + 2) % 6]) + mz.nodes[rp].fur[j].obj.pos;
                                        vec3 PP4 = (P[i] - P[(i + 1) % 6] + P[(i + 2) % 6]) + mz.nodes[rp].fur[j].obj.pos;

                                        float t = dot(normalize(P[i]), ((P[i] + mz.nodes[rp].fur[j].obj.pos) - startOfRay)) / dot(normalize(P[i]), DirectionOfRay);
                                        vec3 asd = startOfRay + DirectionOfRay * t;


                                        if (dot(normalize(PP1 - PP2), asd) > dot(normalize(PP1 - PP2), PP2) && dot(PP1 - PP2, asd) < dot(PP1 - PP2, PP1) &&
                                            dot(normalize(PP1 - PP4), asd) > dot(normalize(PP1 - PP4), PP4) && dot(PP1 - PP4, asd) < dot(PP1 - PP4, PP1) &&
                                            dot(normalize(DirectionOfRay), asd) > dot(normalize(DirectionOfRay), startOfRay))
                                        {

                                            glUniform3f(glGetUniformLocation(ShaderDebug, "p2"), asd.x, asd.y, asd.z);
                                            glUniform3f(glGetUniformLocation(ShaderDebug, "p1"), asd.x, asd.y + 0.01f, asd.z);

                                            glUniform3f(glGetUniformLocation(ShaderDebug, "col"), 0, 1, 0);
                                            glDrawArrays(GL_LINES, 0, 4);

                                            if (distance(startOfRay, asd) < distance(startOfRay, PR2))
                                            {
                                                PR2 = asd;

                                            }
                                        }

                                    }
                                }
                                }
                            }
                        }

                       glUniform3f(glGetUniformLocation(ShaderDebug, "p1"), PR2.x, PR2.y, PR2.z);
                       glUniform3f(glGetUniformLocation(ShaderDebug, "p2"), startOfRay.x, startOfRay.y, startOfRay.z);
                       glUniform3f(glGetUniformLocation(ShaderDebug, "col"), 1, 0, 0);
                       if(distance(PR2, ray)<0.05) glUniform3f(glGetUniformLocation(ShaderDebug, "col"), 0, 1, 0);
                       glDrawArrays(GL_LINES, 0, 4);
                    

                    }
                }

            }
            else {

                // Stores the coordinates of the cursor
                double mouseX;
                double mouseY;
                // Fetches the coordinates of the cursor
                glfwGetCursorPos(window, &mouseX, &mouseY);
                vec2 mousep = vec2((mouseX-OFF_WIDTH - (SCR_WIDTH- SCR_HEIGHT)/2)/ SCR_HEIGHT - 0.5,0.5- (mouseY-OFF_HEIGHT) / SCR_HEIGHT)*2.0f;

                if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && (abs(mousep.x) > 1 || abs(mousep.y) > 1))
                {
                    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
                    glfwSetCursorPos(window, (SCR_WIDTH / 2) + OFF_WIDTH, (SCR_HEIGHT / 2) + OFF_HEIGHT);
                    MouseLocked = true;
                }

                glBindFramebuffer(GL_FRAMEBUFFER, 0);

                glViewport(OFF_WIDTH, OFF_HEIGHT, SCR_WIDTH, SCR_HEIGHT);


                glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
                glEnable(GL_DEPTH_TEST);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



                glUseProgram(cam.ScreenShader);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, cam.PFB1.ColTex);
                glUniform4f(glGetUniformLocation(cam.ScreenShader, "textureMatrix"), 0, 0, 1, 1);
                cam.drawScreen();
                glUseProgram(cam.MapShader);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, cam.MFB.ColTex);
                glUniform1i(glGetUniformLocation(cam.MapShader, "ColT2"), 0);
                glUniform4f(glGetUniformLocation(cam.MapShader, "textureMatrix"), 0, 0, 1, 1);
                glUniform2f(glGetUniformLocation(cam.MapShader, "screen"), SCR_WIDTH, SCR_HEIGHT);
                cam.drawScreen();

                glViewport(OFF_WIDTH + (SCR_WIDTH - SCR_HEIGHT) / 2, OFF_HEIGHT, SCR_HEIGHT, SCR_HEIGHT);
                glUseProgram(ShaderUI);
                glActiveTexture(GL_TEXTURE0);
                GLuint qweewq = texture(30);
                glBindTexture(GL_TEXTURE_2D,qweewq);

                menue.draw(cam.VAO, ShaderUI,vec2(0),vec2(1), mousep, glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS);

                if (*menue.settings.frameLock) {
                    glfwSwapInterval(0);
                }
                else
                {
                    glfwSwapInterval(1);
                }
                resolution = pow(int((1.0-*menue.settings.resolution) * 10)+1, -2.0f);
                cam.updateSize(vec2(SCR_WIDTH, SCR_HEIGHT)* resolution);
            }
        }
        


        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    th3.join();
#pragma endregion
    #pragma region End


    glDeleteProgram(shaderProgram);
    glDeleteProgram(shaderLightProgram);
    glDeleteProgram(shaderShadowProgram);

    glfwTerminate();
   
    return 0;

#pragma endregion


}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        MouseLocked = false;
    }
    if (!ELocked && glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        ELocked = true;
        ligh.push_back(Light(cam.pos));
    }
    else if (glfwGetKey(window, GLFW_KEY_E) != GLFW_PRESS)
    {
        ELocked = false;
    }
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
    {
        ray = ligh[0].pos;
    }

    player.inp->SH = (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS);
    player.inp->SP = (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS);
   
    player.inp->inp = vec2(0);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        player.inp->inp.y = 1;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        player.inp->inp.x = -1;

    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        player.inp->inp.y = -1;

    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        player.inp->inp.x = 1;

    }
   
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)  LightSetings.x += 0.05;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)    LightSetings.x += -0.05;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)    LightSetings.y += 0.05;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)   LightSetings.y += -0.05;
    if (glfwGetKey(window, GLFW_KEY_KP_5) == GLFW_PRESS) {
        pathend = vec3(
        int(9.0 * (player.inp->pos.x / mz.size + mz.thk) / (mz.thk * 2.0)) - 10 * int((player.inp->pos.x + 1.0f) / mz.size),
        int(9.0 * (player.inp->pos.z / mz.size + mz.thk) / (mz.thk * 2.0)) - 10 * int((player.inp->pos.z + 1.0f) / mz.size),
            int((player.inp->pos.x + 1.0f) / mz.size) + int((player.inp->pos.z + 1.0f) / mz.size) * mz.count );
    }
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) pathend = vec3(5,5,int(Rand(gen) * mz.count * mz.count));
    if (glfwGetKey(window, GLFW_KEY_KP_6) == GLFW_PRESS) {
        pathf.FindPath(vec3(int(9.0 * (player.inp->pos.x / mz.size + mz.thk) / (mz.thk * 2.0)) - 10 * int((player.inp->pos.x + 1.0f) / mz.size),
            int(9.0 * (player.inp->pos.z / mz.size + mz.thk) / (mz.thk * 2.0)) - 10 * int((player.inp->pos.z + 1.0f) / mz.size),
            int((player.inp->pos.x + 1.0f) / mz.size) + int((player.inp->pos.z + 1.0f) / mz.size) * mz.count),
            pathend, &pathfq);
    }
    

    //LightSetings
    float armL = 0.27f;
    float armL2 = 0.05f;
    float armL3 = 1.25;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && player.inp->Grounded) {
        armL = 0.15f;
        armL3 = 0.8;
        armL2 = 0.02f;
    }
   // ArmL =  ArmL * (1.0 - deltaTime * 5.0) + (armL) * deltaTime * 5.0;
    ArmL =  ArmL + (armL - ArmL) * deltaTime * 5.0;
    ArmL2 = ArmL2 + (armL2 - ArmL2) * deltaTime * 5.0;
    ArmL3 = ArmL3 + (armL3 - ArmL3) * deltaTime * 5.0;

    cam.pos.x = player.inp->pos.x + sin(cam.rot.y) * ArmL * cos(cam.rot.x) +cos(cam.rot.y) * ArmL2;
    cam.pos.y = player.inp->pos.y + player.obj.sca.y * ArmL3 - sin(cam.rot.x) * ArmL;
    cam.pos.z = player.inp->pos.z +cos(cam.rot.y) * ArmL *cos(cam.rot.x) -sin(cam.rot.y) * ArmL2;
    player.inp->cam = cam.pos;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
   if (width * 9 > height * 16)
   {
       SCR_WIDTH = height * (16.0/ 9.0);
       OFF_HEIGHT = 0;
       OFF_WIDTH = (width -SCR_WIDTH)/2;
       SCR_HEIGHT = height;
   }
   else
    {
        SCR_WIDTH = width;
        OFF_WIDTH = 0;
        OFF_HEIGHT = (height - SCR_HEIGHT)/2;
        SCR_HEIGHT = width * (9.0/16.0);
    }
     
    cam.updateSize(vec2(SCR_WIDTH, SCR_HEIGHT) * resolution);
}