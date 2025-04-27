
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
#include "PlayerClass.h"
#include "UIelement.h"
#include <random>
#include <thread>
#include <queue>
void doLightNew(createShadowFramebufferCube* shadowFB, unsigned int shaderShadowProgram, Light* li);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void DrawingFunction();
void processInput(GLFWwindow* window);

unsigned int SCR_WIDTH = 1600;
unsigned int SCR_HEIGHT = 900;
unsigned int OFF_WIDTH = 0;
unsigned int OFF_HEIGHT = 0;
int playerIndex;
camera cam;
PlayerClass player;
vector<Enemy> enmi;
float resolution = 1.0f;
bool MouseLocked = false;
vec2 LightSetings = vec2(1.6,1.5);
float deltaTime = 1.0f;
double curTime = 0.0;
float mapScale = 1.0f;
Maze mz;
bool endGame = true;

// Define the random number generator and distribution
std::random_device rd;  // Seed generator
std::mt19937 gen(rd()); // Mersenne Twister engine
std::uniform_real_distribution<float> Rand(0.0f, 1.0f); // Range [0, 1]
GLFWwindow* window;
UImenue menue(5);


unsigned int VAO;

unsigned int FurnVAO;
unsigned int CageVAO;
unsigned int ChestVAO;
unsigned int FurnCount;


unsigned int shaderProgram;
unsigned int shaderLightProgram;
unsigned int shaderShadowProgram;
unsigned int shaderMazeProgram;
unsigned int shaderMazeFurnProgram;
unsigned int ShaderPost;
unsigned int ShaderUI;
unsigned int ShaderDebug;
createShadowFramebufferCube shadowFB;
vec2 mousep;

int lev = 0;
int inout = 0;
int leftright = 0;
int updown = 0;

int click = 0;



#pragma region shadow Functions

void doLightNew(createShadowFramebufferCube* shadowFB, unsigned int shaderShadowProgram, Light* li, bool isStatic) {
    // sets the dimentions of the screen to that of the lights frame buffer
    glViewport(0, 0, 500, 500);

    // tells the light it should a texture ready
    li->activate(true);

    // binds the frambufer that gets the closest position of objects from the lights perspective
    // if its the first time draw to the static objects texture
    if(!isStatic) shadowFB->bind(true, li->depthTex);
    else if (li->firstActive) shadowFB->bind(true, li->depthTexPre);
    else
    {
        // otherwise coppy the static objects texture to the moving object texture
        // then draw the moving objects over it
        glBindTexture(GL_TEXTURE_CUBE_MAP, li->depthTexPre);
        for (GLuint face = 0; face < 6; ++face) {
            glCopyImageSubData(li->depthTexPre, GL_TEXTURE_CUBE_MAP, 0, 0, 0, face,
                li->depthTex, GL_TEXTURE_CUBE_MAP, 0, 0, 0, face,
                li->size, li->size, 1);
        }
        shadowFB->bind(false, li->depthTex);

        li->activate(true);
    }


    std::string ii[6];
    glm::mat4 matr4[6];
    for (int i = 0; i < 6; i++)
    {
        // gets the matrix that translates obgects into screen space
        // there are 6 for each direction (up down left right)
        float fov = (0.5f * 3.14159265358979323846f); // half of pi 90 degrees
        float zNear = 0.01f;
        float zFar = 100.0f;
        glm::mat4 projectionMatrix;
        glm::mat4 view;
        projectionMatrix = glm::perspective(fov, 1.0f, zNear, zFar);
        view = glm::lookAt(li->pos, li->pos + shadowFB->target[i], shadowFB->up[i]);
        view = projectionMatrix * view;
        matr4[i] = view;
        // sets the name of the matrix to uProjectionMatrix[0-5] as they are stored in an array 
        ii[i] = "uProjectionMatrix[" + std::to_string(i) + "]";
        // binds the matrix to the GPU
        glUniformMatrix4fv(glGetUniformLocation(shaderShadowProgram, ii[i].c_str()), 1, GL_FALSE, glm::value_ptr(matr4[i]));
    }

    glUniform3f(glGetUniformLocation(shaderShadowProgram, "lpos"),
        li->pos.x, li->pos.y, li->pos.z);

        int Index = int((li->pos.x + 1) / mz.size) + int((li->pos.z + 1) / mz.size) * mz.count;
    // draws the objects to the lights framebuffer
    if (!isStatic) {
        // all the objects
        mz.obj.draw(shaderShadowProgram);
        int rdi = 3;

        for (int j = 0; j < rdi * rdi; j++)
        {
            int curIndex = Index + (j % rdi) - (rdi / 2) + (j / rdi - (rdi / 2)) * mz.count;
            if (curIndex >= 0 &&
                curIndex < mz.nodes.size()) {

                for (int tt = 0; tt < mz.nodes[curIndex].fur.size(); tt++) {
                    mz.nodes[curIndex].fur[tt].obj.draw(shaderShadowProgram);
                }
            }

        }
        glUniform1i(glGetUniformLocation(shaderShadowProgram, "cube"), 1);
        for (int i = 0; i < enmi.size(); i++)if (distance(player.inp->pos, enmi[i].inp->pos) < 7.0) enmi[i].draw(deltaTime, shaderShadowProgram);

        player.draw(curTime, shaderShadowProgram);
    }
    else if (li->firstActive) {
        // all the static objects
        mz.obj.draw(shaderShadowProgram);
        int rdi = 3;
        for (int j = 0; j < rdi * rdi; j++)
        {
            int curIndex = Index + (j % rdi) - (rdi / 2) + (j / rdi - (rdi / 2)) * mz.count;
            if (curIndex >= 0 &&
                curIndex < mz.nodes.size()) {

                for (int tt = 0; tt < mz.nodes[curIndex].fur.size(); tt++) {
                    mz.nodes[curIndex].fur[tt].obj.draw(shaderShadowProgram);
                }
            }

        }
   

    }
    else
    {
        // all the moving objects
        glUniform1i(glGetUniformLocation(shaderShadowProgram, "cube"), 1);
        for (int i = 0; i < enmi.size(); i++)if (distance(player.inp->pos, enmi[i].inp->pos) < 7.0) enmi[i].draw(deltaTime, shaderShadowProgram);

        player.draw(curTime, shaderShadowProgram);
    }


}



#pragma endregion



queue<PathFind*> pathfq; 
void PathFindingFunction()
{
    while (!glfwWindowShouldClose(window) && endGame)
    {
        if (!pathfq.empty()) {

            PathFind* pt = pathfq.front();
            pathfq.pop();

            // translates the possition to a grid node
            int start = int((pt->Startpos.x + 1.0f) / mz.size) + int((pt->Startpos.z + 1.0f) / mz.size) * mz.count;
            int sx = int(9.0 * (pt->Startpos.x / mz.size + mz.thk) / (mz.thk * 2.0)) - 10 * int((pt->Startpos.x + 1.0f) / mz.size);
            int sy = int(9.0 * (pt->Startpos.z / mz.size + mz.thk) / (mz.thk * 2.0)) - 10 * int((pt->Startpos.z + 1.0f) / mz.size);
            int end = int((pt->Endpos.x + 1.0f) / mz.size) + int((pt->Endpos.z + 1.0f) / mz.size) * mz.count;
            int ex = int(9.0 * (pt->Endpos.x / mz.size + mz.thk) / (mz.thk * 2.0)) - 10 * int((pt->Endpos.x + 1.0f) / mz.size);
            int ey = int(9.0 * (pt->Endpos.z / mz.size + mz.thk) / (mz.thk * 2.0)) - 10 * int((pt->Endpos.z + 1.0f) / mz.size);

            // validation for the starting and ending points
            if (sx > 8)sx = 8;  if (sy > 8)sy = 8;
            if (sx < 0)sx = 0;  if (sy < 0)sy = 0;
            if (ex > 8)ex = 8;  if (ey > 8)ey = 8;
            if (ex < 0)ex = 0;  if (ey < 0)ey = 0;
            if (start > mz.nodes.size() - 1)start = mz.nodes.size() - 1;
            if (start < 0)start = 0;
            if (end > mz.nodes.size() - 1)end = mz.nodes.size() - 1;
            if (end < 0)end = 0;

            // get the path
            pt->path = mz.getpath(&mz.nodes[start].grid[sx][sy],&mz.nodes[end].grid[ex][ey]);
            // respond the path has been found
            pt->OnPath = true;
            pt->WaitingForPath = false;
            pt->pathP = 0.0f;
        }
    }
}


void rayCastRoom(int node, vec3 startOfRay, vec3 DirectionOfRay, vec3 * rayPoint, int perch[2]) {

    vec3 wallPointx;
    vec3 wallPointz;
    vec3 xwallPoint;
    vec3 zwallPoint;
    float dx;
    float dz;
    float xd;
    float zd;
    // gets the plane intersection
    wallPointx.x = (mz.nodes[node].x + mz.thk) * mz.size;
    dx = (wallPointx.x - startOfRay.x) / DirectionOfRay.x;
    wallPointx = startOfRay + DirectionOfRay * dx;

    xwallPoint.x = (mz.nodes[node].x - mz.thk) * mz.size;
    xd = (xwallPoint.x - startOfRay.x) / DirectionOfRay.x;
    xwallPoint = startOfRay + DirectionOfRay * xd;

    wallPointz.z = (mz.nodes[node].y + mz.thk) * mz.size;
    dz = (wallPointz.z - startOfRay.z) / DirectionOfRay.z;
    wallPointz = startOfRay + DirectionOfRay * dz;

    zwallPoint.z = (mz.nodes[node].y - mz.thk) * mz.size;
    zd = (zwallPoint.z - startOfRay.z) / DirectionOfRay.z;
    zwallPoint = startOfRay + DirectionOfRay * zd;
    
    // gets the wall intersection
    if (dx > 0 && !mz.nodes[node].sidesIsPath[3] && wallPointx.z >= (mz.nodes[node].y - 0.5f) * mz.size &&
        wallPointx.z <= (mz.nodes[node].y + 0.5f) * mz.size) {
        // updates the rays intersection point
        if (distance(startOfRay, wallPointx) < distance(startOfRay, *rayPoint))
        {
            *rayPoint = wallPointx;
        }
    }
    if (xd > 0 && !mz.nodes[node].sidesIsPath[2] && xwallPoint.z >= (mz.nodes[node].y - 0.5f) * mz.size &&
        xwallPoint.z <= (mz.nodes[node].y + 0.5f) * mz.size) {
        // updates the rays intersection point
        if (distance(startOfRay, xwallPoint) < distance(startOfRay, *rayPoint))
        {
            *rayPoint = xwallPoint;
        }
    }
    if (dz > 0 && !mz.nodes[node].sidesIsPath[0] && wallPointz.x >= (mz.nodes[node].x - 0.5f) * mz.size &&
        wallPointz.x <= (mz.nodes[node].x + 0.5f) * mz.size) {
        // updates the rays intersection point
        if (distance(startOfRay, wallPointz) < distance(startOfRay, *rayPoint))
        {
           *rayPoint = wallPointz;
        }
    }
    if (zd > 0 && !mz.nodes[node].sidesIsPath[1] && zwallPoint.x >= (mz.nodes[node].x - 0.5f) * mz.size &&
        zwallPoint.x <= (mz.nodes[node].x + 0.5f) * mz.size) {
        // updates the rays intersection point
        if (distance(startOfRay, zwallPoint) < distance(startOfRay, *rayPoint))
        {
            *rayPoint = zwallPoint;
        }
    }
    
    
    if (mz.nodes[node].sidesIsPath[1] &&
        mz.nodes[node].sidesIsPath[2] && node > 0 && node + mz.count - 1 < mz.nodes.size()  &&
        (
            // the two nodes beside it have made a corner
            !mz.nodes[node + mz.count - 1].sidesIsPath[0] ||
            !mz.nodes[node + mz.count - 1].sidesIsPath[3]
            )
        ) {

        if (dz > 0 && wallPointz.x >= (mz.nodes[node].x - 0.5) * mz.size &&
            wallPointz.x <= (mz.nodes[node].x - mz.thk) * mz.size) {
            // updates the rays intersection point
            if (distance(startOfRay, wallPointz) < distance(startOfRay, *rayPoint))
            {
                *rayPoint = wallPointz;
            }
        }
        if (xd > 0 && xwallPoint.z <= (mz.nodes[node].y + 0.5) * mz.size &&
            xwallPoint.z >= (mz.nodes[node].y + mz.thk) * mz.size) {
            // updates the rays intersection point
            if (distance(startOfRay, xwallPoint) < distance(startOfRay, *rayPoint))
            {
                *rayPoint = xwallPoint;
            }
        }

    }
    
    if (mz.nodes[node].sidesIsPath[0] &&
        mz.nodes[node].sidesIsPath[2] && node > 0 && node - mz.count - 1 < mz.nodes.size() &&
        (
            // the two nodes beside it have made a corner
            !mz.nodes[node - mz.count - 1].sidesIsPath[1] ||
            !mz.nodes[node - mz.count - 1].sidesIsPath[3]
            )
        ) {

        if (xd > 0 && xwallPoint.z >= (mz.nodes[node].y - 0.5f) * mz.size &&
            xwallPoint.z <= (mz.nodes[node].y - mz.thk) * mz.size) {
            // updates the rays intersection point
            if (distance(startOfRay, xwallPoint) < distance(startOfRay, *rayPoint))
            {
                *rayPoint = xwallPoint;
            }
        }
        if (zd > 0 && zwallPoint.x >= (mz.nodes[node].x - 0.5f) * mz.size &&
            zwallPoint.x <= (mz.nodes[node].x - mz.thk) * mz.size) {
            // updates the rays intersection point
            if (distance(startOfRay, zwallPoint) < distance(startOfRay, *rayPoint))
            {
                *rayPoint = zwallPoint;
            }
        }


    }
    
    if (mz.nodes[node].sidesIsPath[0] &&
        mz.nodes[node].sidesIsPath[3] && node > 0 && node + mz.count + 1 < mz.nodes.size() &&
        (
            // the two nodes beside it have made a corner
            !mz.nodes[node + mz.count + 1].sidesIsPath[1] ||
            !mz.nodes[node + mz.count + 1].sidesIsPath[2]
            )
        ) {

        if (dx > 0 && wallPointx.z <= (mz.nodes[node].y + 0.5f) * mz.size &&
            wallPointx.z >= (mz.nodes[node].y + mz.thk) * mz.size) {
            // updates the rays intersection point
            if (distance(startOfRay, wallPointx) < distance(startOfRay, *rayPoint))
            {
                *rayPoint = wallPointx;
            }
        }

        if (dz > 0 && wallPointz.x <= (mz.nodes[node].x + 0.5f) * mz.size &&
            wallPointz.x >= (mz.nodes[node].x + mz.thk) * mz.size) {
            // updates the rays intersection point
            if (distance(startOfRay, wallPointz) < distance(startOfRay, *rayPoint))
            {
                *rayPoint = wallPointz;
            }
        }
    }
    
    if (mz.nodes[node].sidesIsPath[3] &&
        mz.nodes[node].sidesIsPath[1] && node > 0 && node - mz.count + 1 < mz.nodes.size() &&
        (
            // the two nodes beside it have made a corner
            !mz.nodes[node - mz.count + 1].sidesIsPath[0] ||
            !mz.nodes[node - mz.count + 1].sidesIsPath[2]
            )
        ) {
        if (dx > 0 && wallPointx.z >= (mz.nodes[node].y - 0.5f) * mz.size &&
            wallPointx.z <= (mz.nodes[node].y - mz.thk) * mz.size) {
            // updates the rays intersection point
            if (distance(startOfRay, wallPointx) < distance(startOfRay, *rayPoint))
            {
                *rayPoint = wallPointx;
            }
        }
        if (zd > 0 && zwallPoint.x <= (mz.nodes[node].x + 0.5f) * mz.size &&
            zwallPoint.x >= (mz.nodes[node].x + mz.thk) * mz.size) {
            // updates the rays intersection point
            if (distance(startOfRay, zwallPoint) < distance(startOfRay, *rayPoint))
            {
                *rayPoint = zwallPoint;
            }
        }

    }
    
    

    for (int j = 0; j < mz.nodes[node].fur.size(); j++)
    {
        if (!(perch[0] == node && perch[1] == j))
        {
            // transformes the rectangle into the furnatures size
            mat4 modelViewMatrix;
            modelViewMatrix = rotate(modelViewMatrix, mz.nodes[node].fur[j].obj.rot.z, glm::vec3(0, 0, 1));
            modelViewMatrix = rotate(modelViewMatrix, mz.nodes[node].fur[j].obj.rot.y, glm::vec3(0, 1, 0));
            modelViewMatrix = rotate(modelViewMatrix, mz.nodes[node].fur[j].obj.rot.x, glm::vec3(1, 0, 0));
            modelViewMatrix = scale(modelViewMatrix, mz.nodes[node].fur[j].obj.sca);
            // center of the 6 sides
            vec3 P[6] = {
                modelViewMatrix * vec4(1,0,0,1) ,
                modelViewMatrix * vec4(0,1,0,1) ,
                modelViewMatrix * vec4(0,0,1,1) ,
                modelViewMatrix * vec4(-1,0,0,1),
                modelViewMatrix * vec4(0,-1,0,1),
                modelViewMatrix * vec4(0,0,-1,1)
            };

            bool clip = false;
            // for each side
            for (int i = 0; i < 6; i++)
            {

                // the four corners of a side
                vec3 PP1 = (P[i] + P[(i + 1) % 6] + P[(i + 2) % 6]) + mz.nodes[node].fur[j].obj.pos;
                vec3 PP2 = (P[i] + P[(i + 1) % 6] - P[(i + 2) % 6]) + mz.nodes[node].fur[j].obj.pos;
                vec3 PP3 = (P[i] - P[(i + 1) % 6] - P[(i + 2) % 6]) + mz.nodes[node].fur[j].obj.pos;
                vec3 PP4 = (P[i] - P[(i + 1) % 6] + P[(i + 2) % 6]) + mz.nodes[node].fur[j].obj.pos;
                // the point of intersection with the plane
                float t = dot(normalize(P[i]), ((P[i] + mz.nodes[node].fur[j].obj.pos) - startOfRay)) / dot(normalize(P[i]), DirectionOfRay);
                vec3 asd = startOfRay + DirectionOfRay * t;

                // restricting the plane to the four corners
                if (t>0 && dot(normalize(PP1 - PP2), asd) > dot(normalize(PP1 - PP2), PP2) && dot(PP1 - PP2, asd) < dot(PP1 - PP2, PP1) &&
                    dot(normalize(PP1 - PP4), asd) > dot(normalize(PP1 - PP4), PP4) && dot(PP1 - PP4, asd) < dot(PP1 - PP4, PP1) &&
                    dot(normalize(DirectionOfRay), asd) > dot(normalize(DirectionOfRay), startOfRay))
                {
                    // updates the rays intersection point
                    if (distance(startOfRay, asd) < distance(startOfRay, *rayPoint))
                    {
                        *rayPoint = asd;

                    }
                }

            }
        }
    }
}

vec3 rayCast(vec3 startOfRay, vec3 endOfRay, int perch[2]) {

    vec3 DirectionOfRay = normalize(endOfRay - startOfRay);
    vec3 rayPoint = endOfRay;
    vec3 carryPoint = startOfRay / mz.size;

    // validation making sure the ray doesnt start on a a .5 decimal
    if (carryPoint.x - (int)carryPoint.x == 0.5f || carryPoint.x - (int)carryPoint.x == -0.5f ||
        carryPoint.z - (int)carryPoint.z == 0.5f || carryPoint.z - (int)carryPoint.z == -0.5f)
    {
        carryPoint -= DirectionOfRay * 0.01f;
    }
    int i = 0;
    vec3 centerPoint;
    vec3 xPoint;
    vec3 yPoint;
    float d1;
    float d2;
    do
    {
        centerPoint = vec3(round(carryPoint.x), 0, round(carryPoint.z));

        // marks the point logic would be preformed on
        int room = round(carryPoint.x) + round(carryPoint.z) * mz.count;
        if(room>=0 && room < mz.nodes.size()) rayCastRoom(room, startOfRay, DirectionOfRay, &rayPoint, perch);
          
        // gets the x intersection with the horizontal boundery
        xPoint.x = round(carryPoint.x);
        if (DirectionOfRay.x > 0) xPoint.x += 0.5f;
        else xPoint.x -= 0.5f;
        // use the x value and direction to calculate the point
        d1 = (xPoint.x - carryPoint.x) / DirectionOfRay.x;
        xPoint = carryPoint + DirectionOfRay * d1;

        // gets the y intersection with the vertical boundery
        yPoint.z = round(carryPoint.z);
        if (DirectionOfRay.z > 0) yPoint.z += 0.5f;
        else yPoint.z -= 0.5f;
        // use the y value and direction to calculate the point
        d2 = (yPoint.z - carryPoint.z) / DirectionOfRay.z;
        yPoint = carryPoint + DirectionOfRay * d2;

        // cheaks to see wich point is within the current nodes boundery
        // the next node will will continue from here
        if (round(carryPoint.z) == round(xPoint.z)) carryPoint += DirectionOfRay * d1 * 1.001f;
        else carryPoint += DirectionOfRay * d2 * 1.001f; // with a slight offset so its within the next  node

        // fail safe that stops infinate loops
        i++;
    } while (centerPoint != vec3(round(endOfRay.x / mz.size), 0, round(endOfRay.z / mz.size)) && i < 10);

    return rayPoint;
}
void UpdatingFunction() {

    playerIndex = int((player.inp->pos.x + 1) / mz.size) + int((player.inp->pos.z + 1) / mz.size) * mz.count;

#pragma region Inputs
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        player.inp->WH = true;
    }
    else if (glfwGetKey(window, GLFW_KEY_E) != GLFW_PRESS)
    {
        player.inp->WH = false;
    }

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        MouseLocked = false;
    }

    player.inp->SH = (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS);
    player.inp->SP = (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS);

    player.inp->inp = vec2(0);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        player.inp->inp.y += 1;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        player.inp->inp.x += -1;

    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        player.inp->inp.y += -1;

    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        player.inp->inp.x += 1;

    }

    bool interact = glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS; 

    GLFWgamepadstate state;
    if (glfwGetGamepadState(GLFW_JOYSTICK_1, &state))
    {
        if (state.buttons[GLFW_GAMEPAD_BUTTON_LEFT_BUMPER])
        {
            player.inp->WH = true;
        }
        if (state.buttons[GLFW_GAMEPAD_BUTTON_BACK])
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            MouseLocked = false;
        }
        player.inp->SH = player.inp->SH || (state.axes[GLFW_GAMEPAD_AXIS_LEFT_TRIGGER] > 0.1f);
        player.inp->SP = player.inp->SP || (state.buttons[GLFW_GAMEPAD_BUTTON_A]);

        player.inp->inp.x += state.axes[GLFW_GAMEPAD_AXIS_LEFT_X];
        player.inp->inp.y -= state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y];


        player.inp->lok.x -= (3 + 6 * (*menue.settings.controllerSensitivity - 0.5f)) * deltaTime * state.axes[GLFW_GAMEPAD_AXIS_RIGHT_X];
        player.inp->lok.y -= (3 + 6 * (*menue.settings.controllerSensitivity - 0.5f)) * deltaTime * state.axes[GLFW_GAMEPAD_AXIS_RIGHT_Y];


        interact = interact || state.buttons[GLFW_GAMEPAD_BUTTON_X];


        if (state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_UP])   mapScale *= 1.1f;
        if (state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_DOWN])   mapScale /= 1.1f;
    }


    // the out key or controller button is pressed

    if (interact) {
        // if its the first time set it to 1
        // if not set it to 2
        if (click > 0)click = 2;
        else {
            click = 1;
        }
    }
    else
    {
        click = 0;
    }


    if (glfwGetKey(window, GLFW_KEY_KP_7) == GLFW_PRESS)   mapScale *= 1.1f;
    if (glfwGetKey(window, GLFW_KEY_KP_8) == GLFW_PRESS)   mapScale /= 1.1f;



    player.inp->cam = &cam;
#pragma endregion




    player.update(deltaTime);

    for (int i = 0; i < enmi.size(); i++)enmi[i].update(deltaTime, cam.pos, &mz, &pathfq);

    player.inp->Grounded = mz.collide(&player.inp->pos, 0.03f);


#pragma region Ray Cast

    int rdi = 5;

    // if (*menue.settings.debug)
    bool playerLit = false;
    // all the cells around the player
    for (int t = 0; t < rdi * rdi; t++)
    {
        if (playerIndex + (t % rdi) - (rdi / 2) + (t / rdi - (rdi / 2)) * mz.count >= 0 &&
            playerIndex + (t % rdi) - (rdi / 2) + (t / rdi - (rdi / 2)) * mz.count < mz.nodes.size()) {
            int curIndex = playerIndex + (t % rdi) - (rdi / 2) + (t / rdi - (rdi / 2)) * mz.count;
            // the lights in those cells
            for (int tt = 0; tt < mz.nodes[curIndex].ligh.size(); tt++) {
                vec3 endOfRay = player.inp->obj.pos;
                vec3 startOfRay = mz.nodes[curIndex].ligh[tt].pos;
                // closer than the length of light
                if (distance(endOfRay, startOfRay) < LightSetings.x) {
                    if (rayCast(startOfRay, endOfRay, mz.nodes[curIndex].ligh[tt].perch) == endOfRay) {
                        playerLit = true;
                    }
                }
            }
        }
    }
    for (int tt = 0; tt < enmi.size(); tt++) {

        float playerFocus = player.inp->visible;
        if (playerLit) playerFocus *= 2;
        vec3 endOfRay = player.inp->obj.pos;
        vec3 startOfRay = enmi[tt].inp->pos + vec3(0, enmi[tt].inp->obj.sca.y, 0);
        if (distance(endOfRay, startOfRay) < 1.75) {
            int nullperamiter[2] = { -1,-1 };
            vec3 rayPoint = rayCast(startOfRay, endOfRay, nullperamiter);
            vec3 DirectionOfRay = normalize(endOfRay - startOfRay);
            bool seePlayer = false;
            if (rayPoint == endOfRay) {
                seePlayer = true;
            }
            // within the vision cone
            if (angle(DirectionOfRay, -enmi[tt].inp->look) < 0.35) {
                if (seePlayer)playerFocus *= 3;
            }
            // in front of the enemy
            else if (angle(DirectionOfRay, -enmi[tt].inp->look) < 1.57) {
                if (seePlayer)playerFocus *= 1.5f;
            }
            else {
                playerFocus = -3;
            }
            glDrawArrays(GL_LINES, 0, 4);
        }
        else {
            playerFocus = -10;
        }
        enmi[tt].inp->focus += playerFocus * deltaTime;
        if (enmi[tt].inp->focus < 0)enmi[tt].inp->focus = 0;
        if (enmi[tt].inp->focus > 7)enmi[tt].inp->focus = 7;

    }



#pragma endregion
}

void processInput(GLFWwindow* window) {
    bool up     = glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS;
    bool down   = glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS;
    bool left   = glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS;
    bool right  = glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS;
    bool in     = glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS;
    bool out    = glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS;
    

    GLFWgamepadstate state;
    if (glfwGetGamepadState(GLFW_JOYSTICK_1, &state))
    {
        up    = up || state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_UP];
        down = down || state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_DOWN];
        left = left || state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_LEFT];
        right = right || state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_RIGHT];
        in = in || state.buttons[GLFW_GAMEPAD_BUTTON_A];
        out = out || state.buttons[GLFW_GAMEPAD_BUTTON_B];

    }



    if (out) {
        // if its the first time set it to 1
        // if not set it to 2
        if (inout > 0)inout = 2;
        else {
            inout = 1;  lev--; // if its the first time 
            // decrease level
        }
    }// the in key or controller button is pressed
    else if (in) {
        // if its the first time set it to -1
        // if not set it to -2
        if (inout < 0)inout = -2;
        else {
            inout = -1; lev++;// if its the first time 
            // increase level
        }
    }
    else {
        // when nothing is pressed it should be 0
        inout = 0;
    }
    // validation
    if (lev < 0) lev = 0;
    
    if (up) {
        if (updown > 0)updown = 2;
        else {
            updown = 1;
        }
    }
    else if (down) {
            if (updown < 0)updown = -2;
            else {
                updown = -1;
            }
        }
    else { updown = 0; }
    if (left) {
        if (leftright < 0)leftright = -2;
        else leftright = -1;
    }
    else if (right) {
            if (leftright > 0)leftright = 2;
            else leftright = 1;
        }
    else {
            leftright = 0;
        }

}

void drawMap(bool center) {


    vec3 plPos = player.inp->pos;
    if(center)plPos = vec3(mz.count * mz.size * 0.5f);
    cam.MFB.bind(true);
    glUseProgram(shaderMazeProgram);

    if (center)glUniform1f(glGetUniformLocation(shaderMazeProgram, "thk"), 0.005f);
    else glUniform1f(glGetUniformLocation(shaderMazeProgram, "thk"), 0.01f);

    if (center)glUniform1f(glGetUniformLocation(shaderMazeProgram, "sc"), 3.5f / (mz.count * mz.size));
    else glUniform1f(glGetUniformLocation(shaderMazeProgram, "sc"), mapScale);

    glUniform1f(glGetUniformLocation(shaderMazeProgram, "sc1"), 1.0);
    glUniform3f(glGetUniformLocation(shaderMazeProgram, "campos"), plPos.x / mz.size, plPos.z / mz.size,0);

    if (center)glUniform1f(glGetUniformLocation(shaderMazeProgram, "rt"), 0);
    else glUniform1f(glGetUniformLocation(shaderMazeProgram, "rt"), cam.rot.y);

    glBindVertexArray(mz.MapBuffers.VAO);
    glDrawElements(GL_LINES, mz.MapBuffers.length, GL_UNSIGNED_INT, 0);




    if (!center){

        glUseProgram(shaderMazeFurnProgram);


        glUniform1f(glGetUniformLocation(shaderMazeFurnProgram, "sc1"), 0.05f);

        glBindVertexArray(FurnVAO);

        glUniform3f(glGetUniformLocation(shaderMazeFurnProgram, "campos"), plPos.x / mz.size, plPos.z / mz.size,0);
        glDrawArrays(GL_POINTS, 0, FurnCount);


        glBindVertexArray(VAO);

        glUniform1f(glGetUniformLocation(shaderMazeFurnProgram, "sc"), mapScale);
        glUniform1f(glGetUniformLocation(shaderMazeFurnProgram, "rt"), cam.rot.y);

        glUniform1f(glGetUniformLocation(shaderMazeFurnProgram, "sc1"), 0.02f);
        glUniform1f(glGetUniformLocation(shaderMazeFurnProgram, "thk"), 0.0075f * mapScale);
        glUniform3f(glGetUniformLocation(shaderMazeFurnProgram, "campos"), 0, 0, 50);
        glDrawArrays(GL_POINTS, 0, 1);

        glUniform1f(glGetUniformLocation(shaderMazeFurnProgram, "sc1"), 0.1f);
        for (int i = 0; i < enmi.size(); i++)
        {
            glUniform3f(glGetUniformLocation(shaderMazeFurnProgram, "campos"), ( - enmi[i].inp->pos.x + plPos.x) / mz.size, ( - enmi[i].inp->pos.z + plPos.z) / mz.size, 50);
            glDrawArrays(GL_POINTS, 0, 1);
        }
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

}


int hit = 0;
float run = 1;
float curP = 0;
float Pnt = 3.14f;
float speed = 3.5f;
int   direction = 1;
string message;
vec3 messageColor;

void skillCheck() {
    float score = mz.nodes[playerIndex].completion * 50.0f;
    // moves the point
    curP += speed * direction * deltaTime;
    // the input has been clicked
    if (click == 1)
    {
        // the point hit the target
        if (abs(curP - Pnt) < 0.5)
        {
            // transforms 1 2 3 to 1 3 5
            score += 2 * (int)run - 1;
            run += 0.25f;
            if (run > 3) run = 3;
            hit = 1;

            // the point hit the target perfectly
            if (abs(curP - Pnt) < 0.15)
            {
                score += 2 * (int)run - 1;
                run += 0.25f;
                if (run > 3) run = 3;
                hit = 2;
            }

            // gets a new target point 
            if (Rand(gen) >= 0.6f) direction *= -1;
            if (Rand(gen) >= 0.6f)
                speed = Rand(gen) * 3.6f + 2.4f;

                Pnt = curP + Rand(gen) * 3.141592f * 0.5f + 3.141592f * direction;
        }
        else
        {
            // the click was too early
            hit = 3;
            score--;
            run = 1;
        }
    }

    if ((curP - Pnt) * direction > 0.5)
    {
        // the click was too late
        // gets a new target point 
        if (Rand(gen) >= 0.6f) direction *= -1;
        if (Rand(gen) >= 0.6f) speed = Rand(gen) * 3.6f + 2.4f;
        Pnt = curP + Rand(gen) * 3.141592f * 0.5f + 3.141592f * direction;
        score--;
        run = 1;
        hit = 4;
    }


    if (hit == 1)
    {
        menue.prompt->children[2]->text = "good";
        menue.prompt->children[2]->back = vec4(0,1,0,1);
    }
    if (hit == 2)
    {
        menue.prompt->children[2]->text = "perfect";
        menue.prompt->children[2]->back = vec4(1, 0, 1, 1);
    }
    if (hit == 3)
    {
        menue.prompt->children[2]->text = "too early";
        menue.prompt->children[2]->back = vec4(1, 0, 0, 1);
    }
    if (hit == 4)
    {
        menue.prompt->children[2]->text = "too late";
        menue.prompt->children[2]->back = vec4(1, 0, 0, 1);
    }


    if ((int)run == 1) menue.prompt->children[3]->back = vec4(0.5, 0.2, 0.1, 1);
    if ((int)run == 2) menue.prompt->children[3]->back =vec4(0.6,0.6,0.6,1);
    if ((int)run == 3) menue.prompt->children[3]->back =vec4(1,0.9,0,1);
    menue.prompt->children[3]->text = "X" + to_string(2 * (int)run -1);

    menue.prompt->children[1]->children[0]->pos = vec2(sin(curP), cos(curP));
    menue.prompt->children[1]->children[1]->pos = vec2(sin(Pnt+0.5), cos(Pnt+0.5));
    menue.prompt->children[1]->children[2]->pos = vec2(sin(Pnt-0.5), cos(Pnt-0.5));
    menue.prompt->children[1]->children[3]->pos = vec2(sin(Pnt+0.15), cos(Pnt+0.15));
    menue.prompt->children[1]->children[4]->pos = vec2(sin(Pnt-0.15), cos(Pnt-0.15));
    mz.nodes[playerIndex].completion = score / 50.0f;

    // doesnt give anything to the player and can no longer be interacted with
    if (mz.nodes[playerIndex].completion < 0)
        mz.nodes[playerIndex].prize = 1;

    if (mz.nodes[playerIndex].completion > 1) {
        // chests add a key to the inventory 
        if (mz.nodes[playerIndex].treasure) {
            mz.nodes[playerIndex].prize = 3;
            player.inp->inventory.push_back(0);
        }
        // cages add to the soulStability
        if (mz.nodes[playerIndex].cage) {
            player.inp->soulStability += 20;
            mz.nodes[playerIndex].prize = 2;
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

    shaderProgram = initShader("shaders/def.vert", "shaders/def.geom", "shaders/def.frag");
    shaderLightProgram = initShader("shaders/def.vert", "shaders/def.geom", "shaders/light.frag");
    shaderShadowProgram = initShader("shaders/shadow.vert", "shaders/shadow.geom", "shaders/shadow.frag");
    shaderMazeProgram = initShader("shaders/maze.vert", "shaders/maze.geom", "shaders/maze.frag"); 
    shaderMazeFurnProgram = initShader("shaders/maze.vert", "shaders/mazeFurn.geom", "shaders/maze.frag"); 
    ShaderPost = initShader("shaders/cam.vert", "shaders/camPost.frag"); 
    ShaderUI = initShader("shaders/cam.vert", "shaders/UI.frag"); 
    ShaderDebug = initShader("shaders/debug.vert", "shaders/debug.geom", "shaders/maze.frag");
    shadowFB = createShadowFramebufferCube(500);

    int rdi = 5;

    float vertices[] = {
        0,0,0,
    };

    unsigned int VBO;
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

    cam = camera(vec3(0),vec3(0));
    menue = UImenue(texture(28));
    glDepthFunc(GL_LEQUAL);

    
    player = PlayerClass(vec3(0.0f, 1.0f, 0.5f), vec3(0.0f, -3.1415 / 2.0f, 0.0f));

    

    double preTime = 0.0;
    double curTime = 0.0;
    double timeDif;
    unsigned int counter = 0;
    cam.updateSize(vec2(SCR_WIDTH, SCR_HEIGHT) * resolution);


    thread PathFindingThread;

    SpObj inputPrompt = SpObj(vec3(0), vec3(0), vec3(0.016f,0.009f,0) * 3.0f, initSpriteBuffer(), 27, 0);
   
    #pragma endregion
    #pragma region Update
    while (!glfwWindowShouldClose(window))
    {

        curTime = glfwGetTime();
        timeDif = curTime - preTime;
        counter++;
        if (timeDif >= 0.1) {
            std::string FPS = std::to_string((int)((1.0 / timeDif) * counter));
            std::string ms = std::to_string((int)((timeDif / counter) * 1000));
            std::string title = "NEA:  " + FPS + "FPS  " + ms + "ms " + std::to_string(curTime) + "curTime";
            glfwSetWindowTitle(window, title.c_str());
            preTime = curTime;
            deltaTime = (float)((1.0 / counter) * timeDif);

            counter = 0;


            // Stores the coordinates of the cursor
            double mouseX;
            double mouseY;
            // Fetches the coordinates of the cursor
            glfwGetCursorPos(window, &mouseX, &mouseY);

            mousep = vec2((mouseX - OFF_WIDTH) / SCR_WIDTH - 0.5,
                0.5 - (mouseY - OFF_HEIGHT) / SCR_HEIGHT) * 2.0f;
        }
        if (SCR_WIDTH > 0 && SCR_HEIGHT > 0 && timeDif < 1.0) {

            if (!*menue.settings.gameStart) {

                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                // x button
                menue.fullBox->children[0]->children[0]->active = false;
                // main menu
                menue.fullBox->children[0]->children[1]->active = true;
                // pause
                menue.fullBox->children[0]->children[2]->active = false;
                // settings
                menue.fullBox->children[0]->children[3]->active = true;


                glBindFramebuffer(GL_FRAMEBUFFER, 0);


                menue.update(vec2(0), vec2(1), mousep,
                    glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS);

                processInput(window);

                menue.fullBox->children[0]->control(updown, leftright, inout, &lev, 0, deltaTime);
                glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
                glEnable(GL_DEPTH_TEST);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


                glViewport(OFF_WIDTH, OFF_HEIGHT, SCR_WIDTH, SCR_HEIGHT);
                glUseProgram(ShaderUI);
                glActiveTexture(GL_TEXTURE0);


                menue.draw(cam.VAO, ShaderUI, vec2(0), vec2(1),
                    glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS);

                if (*menue.settings.frameLock) {
                    glfwSwapInterval(0);
                }
                else
                {
                    glfwSwapInterval(1);
                }
                resolution = pow(int((1.0 - *menue.settings.resolution) * 10) + 1, -2.0f);
                cam.updateSize(vec2(SCR_WIDTH, SCR_HEIGHT) * resolution);

                if (*menue.settings.generate) {
                    *menue.settings.generate = false;

                    mz = Maze((int)(*menue.settings.gridSize * 100));

                    {
                        vector<float> vertices;
                        FurnCount = 0;
                        for (int i = 0; i < mz.nodes.size(); i++) {
                            int typ = 0;
                            if (mz.nodes[i].cage) typ = -5;
                            else if (mz.nodes[i].treasure) typ = 5;

                            for (int j = 0; j < 9 * 9; j++) {
                                if (mz.nodes[i].grid[j % 9][j / 9].obstruction > 1) {
                                    vertices.push_back(mz.nodes[i].grid[j % 9][j / 9].pos.y);
                                    vertices.push_back(mz.nodes[i].grid[j % 9][j / 9].pos.x);
                                    if (j == 40)vertices.push_back(typ);
                                    else vertices.push_back(0);
                                    FurnCount += 1;
                                }
                            }
                        }

                        unsigned int VBO;
                        glGenVertexArrays(1, &FurnVAO);
                        glGenBuffers(1, &VBO);
                        // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
                        glBindVertexArray(FurnVAO);

                        glBindBuffer(GL_ARRAY_BUFFER, VBO);
                        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

                        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
                        glEnableVertexAttribArray(0);

                        // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
                        glBindBuffer(GL_ARRAY_BUFFER, 0);

                        // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
                        // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
                        glBindVertexArray(0);
                    }



                    drawMap(true);

                    menue.fullBox->children[1]->children[0]->children[1]->texture = cam.MFB.ColTex;


                }
                if (*menue.settings.gameStart)
                {
                    // resets the players stats
                    player.inp->soulStability = 50;
                    player.inp->inventory.clear();

                    player.inp->pos = vec3(mz.count * mz.size * 0.5f, 0.1f, mz.count * mz.size * 0.5f);
                    dynamic_cast<UIDIV*>(menue.fullBox->children[0])->cur = 2;

                    PathFindingThread = thread(PathFindingFunction);
                    // removes the old enemies
                    enmi.clear();
                    // adds the new ones
                    for (int i = 0; i < (int)(*menue.settings.enemies * 100.0f); i++)
                    {
                        enmi.push_back(Enemy(&player,vec3(int(Rand(gen) * mz.count) * mz.size, 0.0f, int(Rand(gen) * mz.count) * mz.size), vec3(0.0f, -3.1415 / 2.0f, 0.0f)));
                    }
                    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
                    glfwSetCursorPos(window, (SCR_WIDTH / 2) + OFF_WIDTH, (SCR_HEIGHT / 2) + OFF_HEIGHT);
                    MouseLocked = true;

                    cam.updateSize(vec2(SCR_WIDTH, SCR_HEIGHT) * resolution);

                }
            }
            else if (MouseLocked)
            {

                lev = 0;
                thread UpdateThread(UpdatingFunction);

#pragma region Shadows

                glDisable(GL_BLEND);

                //shadows

                glUseProgram(shaderShadowProgram);
                glUniform1i(glGetUniformLocation(shaderShadowProgram, "cube"), 1);

                for (int curIndex = 0; curIndex < mz.nodes.size(); curIndex++)
                {
                    for (int tt = 0; tt < mz.nodes[curIndex].ligh.size(); tt++) {

                        if (distance(mz.nodes[curIndex].ligh[tt].pos, player.inp->pos) < 7.0) doLightNew(&shadowFB, shaderShadowProgram, &mz.nodes[curIndex].ligh[tt], true);
                        else
                        {
                            mz.nodes[curIndex].ligh[tt].activate(false);
                        }
                    }

                }

                for (int i = 0; i < enmi.size(); i++) {
                    glUniform1i(glGetUniformLocation(shaderShadowProgram, "cube"), 0);
                    if (distance(enmi[i].inp->vision.pos, player.inp->pos) < 7.0) doLightNew(&shadowFB, shaderShadowProgram, &enmi[i].inp->vision, false);
                    else
                    {
                        enmi[i].inp->vision.activate(false);
                    }
                }

#pragma endregion


#pragma region draw to interaction prompt
                cam.MFB.bind(true);
                 
                glUseProgram(ShaderUI);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, texture(30));
                // the object isn't locked out of interaction AND the player is close enough to interact with it
                if (mz.nodes[playerIndex].prize <= 0 && distance(vec2(player.inp->pos.x, player.inp->pos.z), vec2(mz.nodes[playerIndex].x, mz.nodes[playerIndex].y) * mz.size) < 0.3f) {

                    if (click == 1) {
                        // chest goes into interacting state
                        if (mz.nodes[playerIndex].prize == -1) {
                            menue.prompt->children[4]->active = false;
                            mz.nodes[playerIndex].prize = 0;
                        }
                        // cage can only go into interacting state when a key is heald
                        else if(mz.nodes[playerIndex].prize == -2 && menue.screen->children[3]->cur >= 0 && menue.screen->children[3]->cur < player.inp->inventory.size() &&
                            player.inp->inventory[menue.screen->children[3]->cur] == 0) {
                            // removes the key after use
                            player.inp->inventory.erase(player.inp->inventory.begin() + menue.screen->children[3]->cur);
                            menue.screen->children[3]->cur = -1;
                            menue.prompt->children[4]->active = false;
                            mz.nodes[playerIndex].prize = 0;
                        }
                    }
                }
                else 
                {
                    // reset interaction
                    run = 1;
                    curP = 0;
                    Pnt = 3.14f;
                    speed = 3.5f;
                    direction = 1;

                    menue.prompt->children[4]->active = true;
                    // displays text on top
                    if (mz.nodes[playerIndex].prize <= 0) {
                        if (mz.nodes[playerIndex].treasure) {
                            mz.nodes[playerIndex].prize = -1;
                            menue.prompt->children[4]->children[0]->text = "press F or X to start";
                        }
                        if (mz.nodes[playerIndex].cage) {
                            mz.nodes[playerIndex].prize = -2;
                            menue.prompt->children[4]->children[0]->text = "You NEED a KEY press F or X to start";
                        }
                    }
                    else if (mz.nodes[playerIndex].prize == 1) {
                        menue.prompt->children[4]->children[0]->text = "You have lost this prize";
                    }
                    else if (mz.nodes[playerIndex].prize == 2) {
                        menue.prompt->children[4]->children[0]->text = "You have gaind some lost soul";
                    }
                    else if (mz.nodes[playerIndex].prize == 3) {
                        menue.prompt->children[4]->children[0]->text = "You have gaind a key";
                    }
                }
                // links the objects compleation to the UI
                *menue.settings.completion = mz.nodes[playerIndex].completion;


                menue.prompt->update(vec2(0), vec2(1), player.inp->whe, false);
                menue.prompt->draw(cam.VAO, ShaderUI, vec2(0), vec2(1), false);

                inputPrompt.text1 = cam.MFB.ColTex;

#pragma endregion

#pragma region draw to Gbuffer
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
                for (int i = 0; i < enmi.size(); i++)if (distance(player.inp->pos, enmi[i].inp->pos) < 7.0)enmi[i].draw(curTime, shaderProgram);

                for (int j = 0; j < rdi * rdi; j++)
                {
                    if (playerIndex + (j % rdi) - (rdi / 2) + (j / rdi - (rdi / 2)) * mz.count >= 0 &&
                        playerIndex + (j % rdi) - (rdi / 2) + (j / rdi - (rdi / 2)) * mz.count < mz.nodes.size()) {
                        for (Furniture furn : mz.nodes[playerIndex + (j % rdi) - (rdi / 2) + (j / rdi - (rdi / 2)) * mz.count].fur) {
                            furn.obj.draw(shaderProgram);
                        }
                    }

                }

                glUseProgram(shaderLightProgram);
                glUniformMatrix4fv(glGetUniformLocation(shaderLightProgram, "uProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(ma));

                glUniform1f(glGetUniformLocation(shaderLightProgram, "tim"), (int)(curTime * 10));

                for (int t = 0; t < rdi * rdi; t++)
                {
                    if (playerIndex + (t % rdi) - (rdi / 2) + (t / rdi - (rdi / 2)) * mz.count >= 0 &&
                        playerIndex + (t % rdi) - (rdi / 2) + (t / rdi - (rdi / 2)) * mz.count < mz.nodes.size()) {
                        int curIndex = playerIndex + (t % rdi) - (rdi / 2) + (t / rdi - (rdi / 2)) * mz.count;
                        for (int tt = 0; tt < mz.nodes[curIndex].ligh.size(); tt++) {
                            glUniform3f(glGetUniformLocation(shaderLightProgram, "color"), mz.nodes[curIndex].ligh[tt].col.x, mz.nodes[curIndex].ligh[tt].col.y, mz.nodes[curIndex].ligh[tt].col.z);
                            mz.nodes[curIndex].ligh[tt].obj.rot.y = atan2(mz.nodes[curIndex].ligh[tt].obj.pos.x - cam.pos.x, mz.nodes[curIndex].ligh[tt].obj.pos.z - cam.pos.z);
                            mz.nodes[curIndex].ligh[tt].obj.draw(shaderLightProgram);
                        }
                    }
                }
                for (int i = 0; i < enmi.size(); i++)if (distance(player.inp->pos, enmi[i].inp->pos) < 7.0) {
                    glUniform3f(glGetUniformLocation(shaderLightProgram, "color"), enmi[i].inp->vision.col.x, enmi[i].inp->vision.col.y, enmi[i].inp->vision.col.z);
                    enmi[i].inp->vision.obj.pos = enmi[i].inp->vision.pos;
                    enmi[i].inp->vision.obj.draw(shaderLightProgram);
                }

                if (mz.nodes[playerIndex].treasure || mz.nodes[playerIndex].cage) {
                    // draw when the node is a treasure node or a cage node
                    glUniform1f(glGetUniformLocation(shaderLightProgram, "tim"), -5);
                    glUniform3f(glGetUniformLocation(shaderLightProgram, "color"), 1, 1, 1);
                    inputPrompt.pos = vec3(mz.nodes[playerIndex].x, 0.15f, mz.nodes[playerIndex].y) * mz.size;
                    inputPrompt.rot.y = atan2(inputPrompt.pos.x - player.inp->pos.x, inputPrompt.pos.z - player.inp->pos.z);
                    inputPrompt.pos -= normalize(inputPrompt.pos - player.inp->pos) * 0.13f * mz.size;
                    inputPrompt.draw(shaderLightProgram);
                    // prize 0 means its ready for interaction
                    if(mz.nodes[playerIndex].prize == 0)skillCheck();

                }

#pragma endregion

#pragma region Lighting

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


                for (int t = 0; t < rdi * rdi; t++)
                {
                    if (playerIndex + (t % rdi) - (rdi / 2) + (t / rdi - (rdi / 2)) * mz.count >= 0 &&
                        playerIndex + (t % rdi) - (rdi / 2) + (t / rdi - (rdi / 2)) * mz.count < mz.nodes.size()) {
                        int curIndex = playerIndex + (t % rdi) - (rdi / 2) + (t / rdi - (rdi / 2)) * mz.count;
                        for (int tt = 0; tt < mz.nodes[curIndex].ligh.size(); tt++) {
                            glUniform3f(glGetUniformLocation(cam.shader, "rotcam"), 1, 0, 0);

                            glActiveTexture(GL_TEXTURE4);
                            glBindTexture(GL_TEXTURE_CUBE_MAP, mz.nodes[curIndex].ligh[tt].depthTex);

                            glUniform1f(glGetUniformLocation(cam.shader, "ConeAngle"), 0);
                            glUniform3f(glGetUniformLocation(cam.shader, "lightPos"), mz.nodes[curIndex].ligh[tt].pos.x, mz.nodes[curIndex].ligh[tt].pos.y, mz.nodes[curIndex].ligh[tt].pos.z);
                            glUniform3f(glGetUniformLocation(cam.shader, "lightCol"), mz.nodes[curIndex].ligh[tt].col.x, mz.nodes[curIndex].ligh[tt].col.y, mz.nodes[curIndex].ligh[tt].col.z);
                            glUniform2f(glGetUniformLocation(cam.shader, "LightSetings"), LightSetings.x + sin(curTime) * 0.1f, LightSetings.y * *menue.settings.tourchBrightness);


                            cam.draw(cam.shader);

                        }
                    }
                }


                for (int i = 0; i < enmi.size(); i++)
                {
                    if (enmi[i].inp->vision.active) {
                        glActiveTexture(GL_TEXTURE4);
                        // shadow map
                        glBindTexture(GL_TEXTURE_CUBE_MAP, enmi[i].inp->vision.depthTex);

                        // the direction of the vision cone
                        glm::vec4 rotated = vec4(enmi[i].inp->look, 1.0f);
                        glUniform3f(glGetUniformLocation(cam.shader, "rotcam"), rotated.x, rotated.y, rotated.z);

                        // limits the ememys vision cone to 0.7 radians
                        glUniform1f(glGetUniformLocation(cam.shader, "ConeAngle"), 0.7);
                        glUniform3f(glGetUniformLocation(cam.shader, "lightPos"), enmi[i].inp->vision.pos.x, enmi[i].inp->vision.pos.y, enmi[i].inp->vision.pos.z);
                        glUniform3f(glGetUniformLocation(cam.shader, "lightCol"), enmi[i].inp->vision.col.x, enmi[i].inp->vision.col.y, enmi[i].inp->vision.col.z);
                        // the distance of vision and brightness of the cone
                        glUniform2f(glGetUniformLocation(cam.shader, "LightSetings"), 1.75, 5.0 * *menue.settings.tourchBrightness);


                        cam.draw(cam.shader);
                    }
                }



                glUniform1i(glGetUniformLocation(cam.shader, "light"), 1);
                glUniform1f(glGetUniformLocation(cam.shader, "brightness"), *menue.settings.ambientBrightness * 0.05);
                cam.draw(cam.shader);
                glDisable(GL_BLEND);


#pragma endregion

#pragma region Post Prossesing

                cam.PFB1.bind(true);
                glUseProgram(ShaderPost);
                glUniform4f(glGetUniformLocation(ShaderPost, "textureMatrix"), 0, 0, 1, 1);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, cam.FB.ColTex);
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, cam.GFB.PosTex);
                cam.draw(ShaderPost);
#pragma endregion

#pragma region HUD

                drawMap(false);


                glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
                glEnable(GL_DEPTH_TEST);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                glViewport(OFF_WIDTH, OFF_HEIGHT, SCR_WIDTH, SCR_HEIGHT);
                glUseProgram(ShaderUI);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, texture(30));

                menue.screen->children[0]->texture = cam.MFB.ColTex;
                menue.screen->texture = cam.PFB1.ColTex;

                // only opent the item wheel when pressed

                if (menue.screen->children[3]->children.size() != player.inp->inventory.size()) {
                    menue.screen->children[3]->children.clear();
                    for (int i = 0; i < player.inp->inventory.size(); i++) {
                        menue.screen->children[3]->children.push_back(new UIButton(vec2(0.2f), vec4(vec3(0.88f), 1.0f), vec4(0.0f),"key", true));
                    }
                }
                if (menue.screen->children[3]->cur >= 0 && menue.screen->children[3]->cur < player.inp->inventory.size()) {
                    menue.screen->children[2]->text = "Item: Key";
                }
                else {
                    menue.screen->children[2]->text = "no Item in hand";
                }
                menue.screen->children[3]->active = player.inp->WH;
                *menue.settings.soulStability = player.inp->soulStability/100.0f;
                player.inp->soulStability -= deltaTime * 0.05f;
                // sends the mouses offset
                menue.screen->update(vec2(0), vec2(1), player.inp->whe, false);
                menue.screen->draw(cam.VAO, ShaderUI, vec2(0), vec2(1), false);

#pragma endregion



                UpdateThread.join();



                // Stores the coordinates of the cursor
                double mouseX;
                double mouseY;
                // Fetches the coordinates of the cursor
                glfwGetCursorPos(window, &mouseX, &mouseY);
                player.inp->lok.x += (0.001f + 0.002f * (*menue.settings.mouseSensitivity - 0.5f)) * (float)((SCR_WIDTH / 2) + OFF_WIDTH - mouseX);
                player.inp->lok.y += (0.001f + 0.002f * (*menue.settings.mouseSensitivity - 0.5f)) * (float)((SCR_HEIGHT / 2) + OFF_HEIGHT - mouseY);


                // Sets mouse cursor to the middle of the screen so that it doesn't end up roaming around
                glfwSetCursorPos(window, (SCR_WIDTH / 2) + OFF_WIDTH, (SCR_HEIGHT / 2) + OFF_HEIGHT);

                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
                if (player.inp->soulStability < 0 || player.inp->soulStability > 100) {
                    lev = 0;
                    endGame = false;
                    if (PathFindingThread.joinable()) PathFindingThread.join();
                    endGame = true;

                    dynamic_cast<UIDIV*>(menue.fullBox->children[0])->cur = 1;
                    *menue.settings.quitToMain = false;
                    *menue.settings.gameStart = false;
                    *menue.settings.generate = true;
                    MouseLocked = true;
                }
            }
            else {

                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                drawMap(true);
                menue.fullBox->children[2]->children[0]->texture = cam.MFB.ColTex;

                // x button
                menue.fullBox->children[0]->children[0]->active = true;
                // main menu
                menue.fullBox->children[0]->children[1]->active = false;
                // pause
                menue.fullBox->children[0]->children[2]->active = true;
                // settings
                menue.fullBox->children[0]->children[3]->active = true;


                menue.update(vec2(0), vec2(1), mousep, glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS);

                processInput(window);

                menue.fullBox->children[0]->control(updown, leftright, inout, &lev, 0, deltaTime);

                glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
                glEnable(GL_DEPTH_TEST);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


                glViewport(OFF_WIDTH, OFF_HEIGHT, SCR_WIDTH, SCR_HEIGHT);
                glUseProgram(ShaderUI);
                glActiveTexture(GL_TEXTURE0);
                GLuint qweewq = texture(30);
                glBindTexture(GL_TEXTURE_2D, qweewq);

                menue.draw(cam.VAO, ShaderUI, vec2(0), vec2(1), glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS);


                if (*menue.settings.close)
                {

                    dynamic_cast<UIDIV*>(menue.fullBox->children[0])->cur = 2;
                    *menue.settings.close = false;
                    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
                    glfwSetCursorPos(window, (SCR_WIDTH / 2) + OFF_WIDTH, (SCR_HEIGHT / 2) + OFF_HEIGHT);
                    MouseLocked = true;
                }
                if (*menue.settings.quitToMain)
                {
                    lev = 0;
                    endGame = false;

                    if (PathFindingThread.joinable()) PathFindingThread.join();
                    endGame = true;

                    dynamic_cast<UIDIV*>(menue.fullBox->children[0])->cur = 1;
                    *menue.settings.quitToMain = false;
                    *menue.settings.gameStart = false;
                    *menue.settings.generate = true;
                    MouseLocked = true;
                }


                if (*menue.settings.frameLock) {
                    glfwSwapInterval(0);
                }
                else
                {
                    glfwSwapInterval(1);
                }
                resolution = pow(int((1.0 - *menue.settings.resolution) * 10) + 1, -2.0f);
                cam.updateSize(vec2(SCR_WIDTH, SCR_HEIGHT) * resolution);

            }

        }


        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    if(PathFindingThread.joinable()) PathFindingThread.join();
#pragma endregion
    #pragma region End


    glDeleteProgram(shaderProgram);
    glDeleteProgram(shaderLightProgram);
    glDeleteProgram(shaderShadowProgram);

    glfwTerminate();
   
    return 0;

#pragma endregion


}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // makes sure the screen is 16 by 9
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