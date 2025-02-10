
#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader.h"
#include <iostream>
#include "object.h"
#include "initMaze.h"
#include "camera.h"
#include "PlayerClass.h"
#include<glm/gtc/matrix_transform.hpp>
#include<gl/GL.h>
#include <random>

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
float resolution = 1.0f;
bool MouseLocked = false;
bool ELocked = false;
bool erere = false;
int adee = 1;
float adeee = 0;
vec2 LightSetings = vec2(1.1,1.2);
float ArmL = 0.27f;
float ArmL2 = 0.0f;
float ArmL3 = 1.25;
float deltaTime = 1.0f;
float UTime = 0.0f;
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

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
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

    unsigned int shaderInstaceProgram = initShader("shaders/defInst.vert", "shaders/defInst.geom", "shaders/defInst.frag");
    unsigned int shaderLightProgram = initShader("shaders/def.vert", "shaders/def.geom", "shaders/light.frag");
    unsigned int shaderShadowProgram = initShader("shaders/shadow.vert", "shaders/shadow.geom", "shaders/shadow.frag");
    unsigned int shaderInstaceShadowProgram = initShader("shaders/shadowInst.vert", "shaders/shadowInst.geom", "shaders/shadowInst.frag");
    unsigned int shaderMazeProgram = initShader("shaders/maze.vert", "shaders/maze.geom", "shaders/maze.frag"); 
    unsigned int ShaderPost = initShader("shaders/cam.vert", "shaders/camPost.frag"); 
    createShadowFramebufferCube shBF = createShadowFramebufferCube(500);

    // Define the random number generator and distribution
    std::random_device rd;  // Seed generator
    std::mt19937 gen(rd()); // Mersenne Twister engine
    std::uniform_real_distribution<float> Rand(0.0f, 1.0f); // Range [0, 1]
    cout << "size" << endl;
    int c = 30;
    
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
    
    
    Maze mz = Maze(&ligh,c);  
    cam = camera(glm::vec3(0.0f, 0.2f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)); 
    
    player = PlayerClass(vec3(0.0f, 0.0f, 0.5f), vec3(0.0f, -3.1415 / 2.0f, 0.0f));
    

    //for (int t = 0; t < pow(mz.count, 2.0f); t++)
    for (int t = 0; t < pow(mz.count, 2.0f); t++)
    {
        
        if (Rand(gen) < 0.4) {
        //    ligh.push_back(Light(vec3((t % mz.count) * mz.size, Rand(gen)* 0.4 + 0.3, (t / mz.count) * mz.size), shBF.size));

        }

    }
    
    float vertices[] = {
        0.0f, -0.1f, 0.0f,
        0.0f, 0.1f, 0.0f,
         -0.1f,0.0f, 0.0f,
         0.1f, 0.0f,0.0f
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


    float sensitivity = 1.0f;
    double preTime = 0.0;
    double curTime = 0.0;
    double timeDif;
    unsigned int counter = 0;
    cam.updateSize(vec2(SCR_WIDTH, SCR_HEIGHT) * resolution);
    #pragma endregion
    #pragma region Update
    tourch = Light(vec3(0));
    glfwSwapInterval(1);
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
           

            if (MouseLocked) {

                // Stores the coordinates of the cursor
                double mouseX;
                double mouseY;
                // Fetches the coordinates of the cursor
                glfwGetCursorPos(window, &mouseX, &mouseY);

                // Normalizes and shifts the coordinates of the cursor such that they begin in the middle of the screen
                // and then "transforms" them into degrees 
                cam.rot.x += sensitivity * (float)((SCR_HEIGHT / 2) - mouseY) / SCR_HEIGHT;
                cam.rot.y += sensitivity * (float)((SCR_WIDTH / 2) - mouseX) / SCR_WIDTH;
               // cout << sensitivity * (float)((SCR_HEIGHT / 2) - mouseY) / SCR_HEIGHT << " " << sensitivity * (float)((SCR_WIDTH / 2) - mouseX) / SCR_WIDTH << endl;
                player.inp->rot.y = cam.rot.y;

                // Sets mouse cursor to the middle of the screen so that it doesn't end up roaming around
                glfwSetCursorPos(window, (SCR_WIDTH / 2), (SCR_HEIGHT / 2));
            }

            // input

           {
                processInput(window);
              //  for (int t = 0; t < ligh.size(); t++) {
              //      //  mz.collide(&ligh[t].pos, &ligh[t].acc, vec2(0.075f));
              //    //  ligh[t].update(deltaTime);
              //  }
                player.update(deltaTime);
                vec3 ty = player.inp->acc;
                vec3 tp = player.inp->pos;
                player.inp->Grounded = mz.collide(&player.inp->pos, nullptr, vec2(0.075f, 0.0f));
               // cout << player.inp->Grounded << " " << player.inp->pos.y << endl;
                
               
            }
            // render
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
                    mz.draw(shaderShadowProgram);
                    glUseProgram(shaderInstaceShadowProgram);

                    for (int i = 0; i < 6; i++)glUniformMatrix4fv(glGetUniformLocation(shaderInstaceShadowProgram, ii[i].c_str()), 1, GL_FALSE, glm::value_ptr(matr4[i]));
                    glUniform3f(glGetUniformLocation(shaderInstaceShadowProgram, "lpos"), tourch.pos.x, tourch.pos.y, tourch.pos.z);
                    mz.furn.draw(shaderInstaceShadowProgram, mz.fur.size());
                }
                else {
                    tourch.activate(false);
                }


            }



            for (int t = 0; t < ligh.size(); t++) {
                
                if (length(ligh[t].pos - cam.pos) < 7.0f) {
                    
                    ligh[t].activate(true);
                   
                    if (ligh[t].active2||UTime == t%10) shBF.bind(true, 0, ligh[t].depthTexPre);
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
                       if (ligh[t].active2 || UTime == t % 10) {
                           mz.draw(shaderShadowProgram);
                           glUseProgram(shaderInstaceShadowProgram);

                           for (int i = 0; i < 6; i++)glUniformMatrix4fv(glGetUniformLocation(shaderInstaceShadowProgram, ii[i].c_str()), 1, GL_FALSE, glm::value_ptr(matr4[i]));
                           glUniform3f(glGetUniformLocation(shaderInstaceShadowProgram, "lpos"), ligh[t].pos.x, ligh[t].pos.y, ligh[t].pos.z);
                           mz.furn.draw(shaderInstaceShadowProgram, mz.fur.size());
                       }
                       else
                       {
                           player.draw(curTime, shaderShadowProgram);
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
            mz.draw(shaderProgram);
            glDisable(GL_CULL_FACE);

            player.draw(curTime, shaderProgram);


            glUseProgram(shaderInstaceProgram);
            glUniformMatrix4fv(glGetUniformLocation(shaderInstaceProgram, "uProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(ma));
            glUniform3f(glGetUniformLocation(shaderInstaceProgram, "camPos"), cam.pos.x, cam.pos.y, cam.pos.z);
            mz.furn.draw(shaderInstaceProgram, mz.fur.size());

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
                    glUniform3f(glGetUniformLocation(cam.shader, "lightCol"), 1.0,1.0,1.0);
                    glUniform2f(glGetUniformLocation(cam.shader, "LightSetings"),20.0, 1.1 + sin(curTime * 2) * 0.3f);
                    

                    cam.draw(cam.shader); 

                }


            }
            for (int t = 1; t < ligh.size(); t++) {

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
            cam.draw(cam.shader);
            glDisable(GL_BLEND);


            cam.PFB1.bind(true);
            glUseProgram(ShaderPost);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, cam.FB.ColTex); 
            glActiveTexture(GL_TEXTURE1); 
            glBindTexture(GL_TEXTURE_2D, cam.GFB.PosTex);
            cam.draw(ShaderPost);

            cam.MFB.bind(true);
            glUseProgram(shaderMazeProgram);
            glUniform2f(glGetUniformLocation(shaderMazeProgram, "campos"), cam.pos.x/mz.size, cam.pos.z / mz.size);
            glUniform1f(glGetUniformLocation(shaderMazeProgram, "rt"), cam.rot.y);
            glBindVertexArray(mz.VAO);
            glDrawElements(GL_LINES, mz.MapBuffers.length, GL_UNSIGNED_INT, 0);
            glBindVertexArray(VAO);
            glUniform1f(glGetUniformLocation(shaderMazeProgram, "rt"), 0.0);
            glUniform2f(glGetUniformLocation(shaderMazeProgram, "campos"), 0,0);
            glDrawArrays(GL_LINES, 0,4);

            

            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            glViewport(OFF_WIDTH, OFF_HEIGHT, SCR_WIDTH, SCR_HEIGHT);


            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glEnable(GL_DEPTH_TEST);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



            glUseProgram(cam.ScreenShader);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, cam.PFB1.ColTex);
            //glUniform1i(glGetUniformLocation(cam.ScreenShader, "ColT2"), 0);
            cam.drawScreen();
            glUseProgram(cam.MapShader);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, cam.MFB.ColTex);
            glUniform1i(glGetUniformLocation(cam.MapShader, "ColT2"), 0);

            glUniform2f(glGetUniformLocation(cam.MapShader, "screen"), SCR_WIDTH, SCR_HEIGHT);
            cam.drawScreen();

          
        }
        else
        {
           // cout << SCR_WIDTH << " " << SCR_HEIGHT << endl;
        }

        glfwPollEvents();
        glfwSwapBuffers(window);
    }
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
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        // Hides mouse cursor
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

        glfwSetCursorPos(window, (SCR_WIDTH / 2), (SCR_HEIGHT / 2));
        MouseLocked = true;
    }
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
        adee = 1;
    }
    else if (glfwGetKey(window, GLFW_KEY_F) != GLFW_PRESS)
    {
        adee = 2;
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
    
    if (glfwGetKey(window, GLFW_KEY_KP_ENTER) == GLFW_PRESS) cam.updateSize(vec2(SCR_WIDTH, SCR_HEIGHT) * resolution);
    if (glfwGetKey(window, GLFW_KEY_KP_0) == GLFW_PRESS)  resolution = 1.0f / pow(2.5f, 2.0f);
    if (glfwGetKey(window, GLFW_KEY_KP_1) == GLFW_PRESS)  resolution = 1.0f;
    if (glfwGetKey(window, GLFW_KEY_KP_2) == GLFW_PRESS)  resolution = 1.0f/pow(2.0f,2.0f);
    if (glfwGetKey(window, GLFW_KEY_KP_3) == GLFW_PRESS)  resolution = 1.0f/pow(3.0f,2.0f);
    if (glfwGetKey(window, GLFW_KEY_KP_4) == GLFW_PRESS)  resolution = 1.0f/pow(4.0f,2.0f);
    if (glfwGetKey(window, GLFW_KEY_KP_5) == GLFW_PRESS)  resolution = 1.0f/pow(5.0f,2.0f);
    if (glfwGetKey(window, GLFW_KEY_KP_6) == GLFW_PRESS)  resolution = 1.0f/pow(6.0f,2.0f);
    if (glfwGetKey(window, GLFW_KEY_KP_7) == GLFW_PRESS)  resolution = 1.0f/pow(7.0f,2.0f);
    if (glfwGetKey(window, GLFW_KEY_KP_8) == GLFW_PRESS)  resolution = 1.0f/pow(8.0f,2.0f);
    if (glfwGetKey(window, GLFW_KEY_KP_9) == GLFW_PRESS)  resolution = 1.0f/pow(9.0f,2.0f);
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