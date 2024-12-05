#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader.h"
#include <iostream>
#include "object.h"
#include "initMaze.h"
#include "camera.h"
#include "PlayerClass.h"
#include "Light.h"
#include<glm/gtc/matrix_transform.hpp>
#include<gl/GL.h>
#include <random>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
 unsigned int SCR_WIDTH = 800;
 unsigned int SCR_HEIGHT = 800;
 camera cam;
 vector<Light> ligh;
 PlayerClass player;
bool firstClick = false;
bool firstClicke = false;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    // build and compile our shader program
    // ------------------------------------
    
    unsigned int shaderProgram = initShader("shaders/def.vert", "shaders/def.frag");
    unsigned int shaderLightProgram = initShader("shaders/def.vert", "shaders/light.frag");
    unsigned int shaderShadowProgram = initShader("shaders/shadow.vert", "shaders/shadow.geom", "shaders/shadow.frag");
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    
    Maze mz = Maze();
     cam = camera(glm::vec3(0.0f,0.2f,0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
     // Define the random number generator and distribution
     std::random_device rd;  // Seed generator
     std::mt19937 gen(rd()); // Mersenne Twister engine
     std::uniform_real_distribution<float> Rand(0.0f, 1.0f); // Range [0, 1]

     // Generate a random float
     cout << Rand(gen) << endl;

      for (int index = 0; index < pow(mz.count, 2.0f); index++) 
      {
        if(Rand(gen)<0.5f)  ligh.push_back(Light(vec3((index%mz.count) * mz.size, 0.3f,(index/mz.count) * mz.size)));

      }
      player = PlayerClass(vec3(0.0f, 0.0f, 0.5f), vec3(0.0f, -3.1415 / 2.0f, 0.0f));
     createShadowFramebufferCube shBF = createShadowFramebufferCube(500);
    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

     float sensitivity = 1.0f;
     float deltaTime = 1.0f;
     double preTime = 0.0;
     double curTime = 0.0;
     double timeDif;
     unsigned int counter = 0;
    // render loop
    // -----------
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
       

       

        
        if (firstClick) {

            // Stores the coordinates of the cursor
            double mouseX;
            double mouseY;
            // Fetches the coordinates of the cursor
            glfwGetCursorPos(window, &mouseX, &mouseY);

            // Normalizes and shifts the coordinates of the cursor such that they begin in the middle of the screen
            // and then "transforms" them into degrees 
            cam.rot.x += sensitivity * (float)((SCR_HEIGHT / 2)- mouseY) / SCR_HEIGHT;
            cam.rot.y += sensitivity * (float)((SCR_WIDTH / 2)- mouseX) / SCR_WIDTH;
            player.inp->rot.y = cam.rot.y;

            // Sets mouse cursor to the middle of the screen so that it doesn't end up roaming around
            glfwSetCursorPos(window, (SCR_WIDTH / 2), (SCR_HEIGHT / 2));
        }

        // input
        // -----
        
        if(deltaTime < 1.0f){
        processInput(window);
        for (int t = 0; t < ligh.size(); t++) {
            mz.collide(&ligh[t].pos, &ligh[t].acc, vec2(0.04f));
            ligh[t].update(deltaTime);
        }
        player.inp->Grounded = mz.collide(&player.inp->pos, nullptr, vec2(0.05f, 0.0f));
        player.update(deltaTime);
        }
        // render
        // ------
        glDisable(GL_BLEND);
        glDisable(GL_CULL_FACE);

        glViewport(0, 0, 500,500);
        int nb = 0;
        for (int t = 0; t < ligh.size(); t++) {
            if (length(ligh[t].pos - cam.pos) < 2.5f) {
                nb++;
                shBF.bind(true, 0, ligh[t].depthTex);
                glUseProgram(shaderShadowProgram);
                for (int i = 0; i < 6; i++)
                {
                    float fov = (0.5f * 3.14159265358979323846f);
                    float zNear = 0.01f;
                    float zFar = 100.0f;
                    glm::mat4 projectionMatrix;
                    glm::mat4 view;
                    glm::mat4 rotationMatrix;
                    projectionMatrix = glm::perspective(fov, 1.0f, zNear, zFar);
                    view = glm::lookAt(ligh[t].obj.pos, ligh[t].pos + shBF.target[i], shBF.up[i]);
                    rotationMatrix = view;
                    view = projectionMatrix * view;
                    std::string ii = "uProjectionMatrix[" + std::to_string(i) + "]";
                    glUniformMatrix4fv(glGetUniformLocation(shaderShadowProgram, ii.c_str()), 1, GL_FALSE, glm::value_ptr(view));
                }
                glUniform3f(glGetUniformLocation(shaderShadowProgram, "lpos"), ligh[t].pos.x, ligh[t].pos.y, ligh[t].pos.z);
                mz.obj.draw(shaderShadowProgram);
                player.draw(curTime, shaderShadowProgram);
                // cout << ligh[t].pos.x << endl;
            }
        }


        //glViewport(0, 0, 1024, 1024);


        cam.GFB.bind(true);
        glUseProgram(shaderProgram);
        std::vector<glm::mat4> ma = cam.matrix(float(SCR_WIDTH) / float(SCR_HEIGHT));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "uProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(ma[0]));
        glUniform3f(glGetUniformLocation(shaderProgram, "camPos"), cam.pos.x, cam.pos.y, cam.pos.z);
        mz.obj.draw(shaderProgram);
        player.draw(curTime,shaderProgram);
        glUseProgram(shaderLightProgram);
        glUniformMatrix4fv(glGetUniformLocation(shaderLightProgram, "uProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(ma[0]));
        for (int t = 0; t < ligh.size(); t++) {
            glUniform3f(glGetUniformLocation(shaderLightProgram, "color"), ligh[t].col.x, ligh[t].col.y, ligh[t].col.z);
            ligh[t].obj.draw(shaderLightProgram);
        }








        cam.FB.bind(true);

        glEnable(GL_BLEND); // Enable depth testing
        glDepthFunc(GL_LEQUAL); // Near things obscure far things
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBlendEquation(GL_FUNC_ADD);
        glBlendFunc(GL_ONE, GL_ONE);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);

        glUseProgram(cam.shader);
        glUniform3f(glGetUniformLocation(cam.shader, "camPos"), cam.pos.x, cam.pos.y, cam.pos.z);
        glUniform1i(glGetUniformLocation(cam.shader, "light"), 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, cam.GFB.ColTex);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, cam.GFB.PosTex);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, cam.GFB.NomTex);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, cam.GFB.NomFTex);
        for (int t = 0; t < ligh.size(); t++) {

            glActiveTexture(GL_TEXTURE4);
           // if (length(ligh[t].pos - cam.pos) < 6.5f)
            {
                glBindTexture(GL_TEXTURE_CUBE_MAP, ligh[t].depthTex);


                glUniform3f(glGetUniformLocation(cam.shader, "lightPos"), ligh[t].pos.x, ligh[t].pos.y, ligh[t].pos.z);

                cam.draw();
            }
        }
        glUniform1i(glGetUniformLocation(cam.shader, "light"), 1);
        cam.draw();






        glBindFramebuffer(GL_FRAMEBUFFER,0);

        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
        

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glEnable(GL_DEPTH_TEST); // Enable depth testing
        
        // Clear the canvas before we start drawing on it.

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 



        glUseProgram(cam.ScreenShader);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, cam.FB.ColTex);
        cam.drawScreen();



        // draw our first triangle
       
       // tst.draw(shaderProgram); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        //glDrawArrays(GL_TRIANGLES, 0, 6);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        // glBindVertexArray(0); // no need to unbind it every time 

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    
    glDeleteProgram(shaderProgram);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
   // if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
       // glfwSetWindowShouldClose(window, true);

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        // Hides mouse cursor
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

            glfwSetCursorPos(window, (SCR_WIDTH / 2), (SCR_HEIGHT / 2));
            firstClick = true;

        

    }
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
          glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        firstClick = false;


    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        player.inp->acc.y += 0.5f;
    }
    if (!firstClicke && glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        firstClicke = true;
        ligh.push_back(Light(cam.pos));
    }
    else if (glfwGetKey(window, GLFW_KEY_E) != GLFW_PRESS)
    {
        firstClicke = false;

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
    /*
    float fs = 0.01f;
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        ligh[t].acc.x += -fs * sin(cam.rot.y);
        ligh[t].acc.z += -fs * cos(cam.rot.y);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        ligh[t].acc.x += fs * -cos(cam.rot.y);
        ligh[t].acc.z += fs * sin(cam.rot.y);

    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        ligh[t].acc.x += fs * sin(cam.rot.y);
        ligh[t].acc.z += fs * cos(cam.rot.y);

    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        ligh[t].acc.x += fs * cos(cam.rot.y);
        ligh[t].acc.z += fs * -sin(cam.rot.y);

    }
    */
    float armL = 0.27f;
    float armL2 = 0.04f;
    cam.pos.x = player.inp->pos.x + sin(cam.rot.y) * armL * cos(cam.rot.x) +cos(cam.rot.y) * armL2;
    cam.pos.y = player.inp->pos.y + player.obj.sca.y * 1.25 -sin(cam.rot.x) * armL;
    cam.pos.z = player.inp->pos.z +cos(cam.rot.y) * armL *cos(cam.rot.x) -sin(cam.rot.y) * armL2;
    player.inp->cam = cam.pos;





}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
 //   glViewport(0, 0, width, height);

     SCR_WIDTH = width;
    SCR_HEIGHT = height;
   
   


}