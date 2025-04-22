
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
Light tourch;
PlayerClass player;
PathFind pathf;
vector<Enemy> enmi;
float resolution = 1.0f;
bool MouseLocked = false;
bool ELocked = false;
bool erere = false;
int adee = 1;
float adeee = 0;
vec2 LightSetings = vec2(1.1,0.25);
float deltaTime = 1.0f;
double curTime = 0.0;
float mapScale = 1.0f;
Maze mz;
bool endGame = true;

vec3 pathend;
PlayerClassOld playerObj;
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
        for (int i = 0; i < enmi.size(); i++)if (distance(player.inp->pos, enmi[i].pos) < 7.0) enmi[i].draw(deltaTime, shaderShadowProgram);

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
        for (int i = 0; i < enmi.size(); i++)if (distance(player.inp->pos, enmi[i].pos) < 7.0) enmi[i].draw(deltaTime, shaderShadowProgram);

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


void UpdatingFunction() {

    playerIndex = int((player.inp->pos.x + 1) / mz.size) + int((player.inp->pos.z + 1) / mz.size) * mz.count;

#pragma region Inputs
    if (!ELocked && glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
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
    }




    if (glfwGetKey(window, GLFW_KEY_KP_7) == GLFW_PRESS)   mapScale *= 1.1f;
    if (glfwGetKey(window, GLFW_KEY_KP_8) == GLFW_PRESS)   mapScale /= 1.1f;

    if (glfwGetKey(window, GLFW_KEY_KP_5) == GLFW_PRESS) {
        pathend = player.inp->pos;

    }
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) pathend = vec3((Rand(gen) * mz.size * mz.count),0, (Rand(gen) * mz.size * mz.count));
    if (glfwGetKey(window, GLFW_KEY_KP_6) == GLFW_PRESS) {
        pathf.EndPath = true;
        pathf.FindPath(player.inp->pos,
            pathend, &pathfq);
    }



    player.inp->cam = &cam;
#pragma endregion




    player.update(deltaTime);

    for (int i = 0; i < enmi.size(); i++)enmi[i].update(deltaTime, cam.pos, &mz, &pathfq);

    player.inp->Grounded = mz.collide(&player.inp->pos, 0.03f);

    if (pathf.pathP + 1 >= pathf.path.size()) {
        pathf.OnPath = false;
        pathf.pathP = 0;
    }
    if (pathf.OnPath) {
        vec2 psss1 = pathf.path[int(pathf.pathP)]->pos * mz.size;
        vec2 psss2 = pathf.path[int(pathf.pathP + 1)]->pos * mz.size;
        //player.inp->pos = vec3(psss1.x, player.inp->pos.y, psss1.y) + ((vec3(psss2.x, player.inp->pos.y, psss2.y)) - vec3(psss1.x, player.inp->pos.y, psss1.y)) * (pathP-int(pathP));
        //pathP += deltaTime * 3.0f;
        player.inp->vel += 0.1f * ((player.inp->pos - vec3(psss2.x, player.inp->pos.y, psss2.y)) / distance(psss2, vec2(player.inp->pos.x, player.inp->pos.z)));
        if (pathf.path[pathf.pathP + 1]->obstruction > 3 && player.inp->pos.y < pathf.path[pathf.pathP + 1]->hight * mz.size) {
            cout << "sad" << endl;
            player.inp->Grounded = true;
            player.inp->pos.y += 1.0f * deltaTime; 
            if (player.inp->pos.y > pathf.path[pathf.pathP + 1]->hight * mz.size)player.inp->pos.y = pathf.path[pathf.pathP + 1]->hight * mz.size;
        }
        if (distance(psss1, vec2(player.inp->pos.x, player.inp->pos.z)) > distance(psss2, vec2(player.inp->pos.x, player.inp->pos.z)))pathf.pathP++;
        //if (int(pathf.pathP) > 0 && distance(psss1, vec2(player.inp->pos.x, player.inp->pos.z)) > distance(pathf.path[int(pathf.pathP - 1)]->pos * mz.size, vec2(player.inp->pos.x, player.inp->pos.z)))pathf.pathP++;
   
    }
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

    // the out key or controller button is pressed
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

        for (int i = 0; i < pathf.path.size(); i++)
        {
            glUniform3f(glGetUniformLocation(shaderMazeFurnProgram, "campos"), -pathf.path[i]->pos.x + plPos.x / mz.size, -pathf.path[i]->pos.y + plPos.z / mz.size, 50);
            glDrawArrays(GL_POINTS, 0, 1);
        }
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

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
    menue = UImenue(texture(30));
    glDepthFunc(GL_LEQUAL);

    
    player = PlayerClass(vec3(0.0f, 1.0f, 0.5f), vec3(0.0f, -3.1415 / 2.0f, 0.0f));

    

    double preTime = 0.0;
    double curTime = 0.0;
    double timeDif;
    unsigned int counter = 0;
    cam.updateSize(vec2(SCR_WIDTH, SCR_HEIGHT) * resolution);
    tourch = Light(vec3(0));


    thread PathFindingThread;

    SpObj testObj = SpObj(vec3(0), vec3(0), glm::vec3(0.09f), initSpriteBuffer(), 27, 0);
   
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


                menue.update( vec2(0), vec2(1), mousep,
                    glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS);

                processInput(window);

                menue.fullBox->children[0]->control(updown,leftright,inout,&lev,0,deltaTime);
                glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
                glEnable(GL_DEPTH_TEST);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


                //glViewport(OFF_WIDTH + (SCR_WIDTH - SCR_HEIGHT) / 2, OFF_HEIGHT, SCR_HEIGHT, SCR_HEIGHT);
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
                                   if(j==40)vertices.push_back(typ);
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
                    player.inp->pos = vec3(mz.count * mz.size * 0.5f,0.1f, mz.count * mz.size * 0.5f);
                    dynamic_cast<UIDIV*>(menue.fullBox->children[0])->cur = 2;

                    PathFindingThread = thread(PathFindingFunction);

                    enmi.clear();
                   // for (int i = 0; i < (int)(*menue.settings.enemies * 100); i++)
                   // {
                   //     enmi.push_back(Enemy(vec3(int(Rand(gen) * mz.count) * mz.size, 0.0f, int(Rand(gen) * mz.count) * mz.size), vec3(0.0f, -3.1415 / 2.0f, 0.0f)));
                   //     mz.collide(&enmi[i].pos, 0.07f);
                   //     enmi[i].pos.y = 0;
                   // }
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

                if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
                {
                    tourch.pos = cam.pos + vec3(0, 0.1, 0);
                    doLightNew(&shadowFB, shaderShadowProgram, &tourch, false);


                }
                for (int i = 0; i < enmi.size(); i++) {
                    glUniform1i(glGetUniformLocation(shaderShadowProgram, "cube"), 0);
                    if (distance(enmi[i].vision.pos, player.inp->pos) < 7.0) doLightNew(&shadowFB, shaderShadowProgram, &enmi[i].vision, false);
                    else
                    {
                        enmi[i].vision.activate(false);
                    }
                }

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
                for (int i = 0; i < enmi.size(); i++)if (distance(player.inp->pos, enmi[i].pos) < 7.0)enmi[i].draw(curTime, shaderProgram);

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


                for (int t = 0; t < rdi * rdi; t++)
                {
                    if (playerIndex + (t % rdi) - (rdi / 2) + (t / rdi - (rdi / 2)) * mz.count >= 0 &&
                        playerIndex + (t % rdi) - (rdi / 2) + (t / rdi - (rdi / 2)) * mz.count < mz.nodes.size()) {
                        int curIndex = playerIndex + (t % rdi) - (rdi / 2) + (t / rdi - (rdi / 2)) * mz.count;
                        for (int tt = 0; tt < mz.nodes[curIndex].ligh.size(); tt++) {
                            glUniform3f(glGetUniformLocation(shaderLightProgram, "color"), mz.nodes[curIndex].ligh[tt].col.x, mz.nodes[curIndex].ligh[tt].col.y, mz.nodes[curIndex].ligh[tt].col.z);
                            glUniform1f(glGetUniformLocation(shaderLightProgram, "tim"), (int)(curTime * 10));
                            mz.nodes[curIndex].ligh[tt].obj.rot.y = atan2(mz.nodes[curIndex].ligh[tt].obj.pos.x - cam.pos.x, mz.nodes[curIndex].ligh[tt].obj.pos.z - cam.pos.z);
                            mz.nodes[curIndex].ligh[tt].obj.draw(shaderLightProgram);
                        }
                    }
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
                glUniform1i(glGetUniformLocation(cam.shader, "gradient"), 1);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, cam.GFB.ColTex);
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, cam.GFB.PosTex);
                glActiveTexture(GL_TEXTURE2);
                glBindTexture(GL_TEXTURE_2D, cam.GFB.NomTex);
                glActiveTexture(GL_TEXTURE3);
                glBindTexture(GL_TEXTURE_2D, cam.GFB.NomFTex);

               // if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
               // {
               //     {
               //         glm::vec4 rotated = inverse(cam.rotationMatrix) * vec4(0.0f, 0.0f, 1.0f, 1.0f);
               //         glUniform3f(glGetUniformLocation(cam.shader, "rotcam"), rotated.x, rotated.y, rotated.z);
               //         glActiveTexture(GL_TEXTURE4);
               //         glBindTexture(GL_TEXTURE_CUBE_MAP, tourch.depthTex);
               //         glUniform1f(glGetUniformLocation(cam.shader, "ConeAngle"), 0.68);
               //         glUniform3f(glGetUniformLocation(cam.shader, "lightPos"), tourch.pos.x, tourch.pos.y, tourch.pos.z);
               //         glUniform3f(glGetUniformLocation(cam.shader, "lightCol"), 1.0, 1.0, 1.0);
               //         glUniform2f(glGetUniformLocation(cam.shader, "LightSetings"), 20.0, 0.3f);
               //
               //
               //         cam.draw(cam.shader);
               //
               //     }
               //
               //
               // }

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
                            glUniform2f(glGetUniformLocation(cam.shader, "LightSetings"), LightSetings.x + sin(curTime) * 0.1f, LightSetings.y);


                            cam.draw(cam.shader);

                        }
                    }
                }

                glUniform1i(glGetUniformLocation(cam.shader, "gradient"), 0);

                for (int i = 0; i < enmi.size(); i++)
                {
                    if (enmi[i].vision.active) {
                        glm::vec4 rotated = vec4(enmi[i].acc.x, 0.3f, enmi[i].acc.z, 1.0f);
                        glUniform3f(glGetUniformLocation(cam.shader, "rotcam"), rotated.x, rotated.y, rotated.z);

                        glActiveTexture(GL_TEXTURE4);
                        glBindTexture(GL_TEXTURE_CUBE_MAP, enmi[i].vision.depthTex);

                        glUniform1f(glGetUniformLocation(cam.shader, "ConeAngle"), 0.7);
                        glUniform3f(glGetUniformLocation(cam.shader, "lightPos"), enmi[i].vision.pos.x, enmi[i].vision.pos.y, enmi[i].vision.pos.z);
                        glUniform3f(glGetUniformLocation(cam.shader, "lightCol"), 1.0, 0.0, 0.0);
                        glUniform2f(glGetUniformLocation(cam.shader, "LightSetings"), 10.0, 1.3f);


                        cam.draw(cam.shader);
                    }
                }



                glUniform1i(glGetUniformLocation(cam.shader, "light"), 1);
                glUniform1f(glGetUniformLocation(cam.shader, "brightness"), *menue.settings.brightness * 0.05);
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

                glViewport(OFF_WIDTH, OFF_HEIGHT, SCR_WIDTH, SCR_HEIGHT);

                glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
                glEnable(GL_DEPTH_TEST);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                glViewport(OFF_WIDTH, OFF_HEIGHT, SCR_WIDTH, SCR_HEIGHT); 
                glUseProgram(ShaderUI); 
                glActiveTexture(GL_TEXTURE0); 
                GLuint qweewq = texture(30); 
                glBindTexture(GL_TEXTURE_2D, qweewq); 

                menue.screen->children[0]->texture = cam.MFB.ColTex;
                menue.screen->texture = cam.PFB1.ColTex;

                // only opent the item wheel when pressed
                menue.screen->children[3]->active = player.inp->WH;
                // sends the mouses offset
                menue.screen->update(vec2(0), vec2(1), player.inp->whe, false);
                menue.screen->draw(cam.VAO, ShaderUI, vec2(0), vec2(1), false);

#pragma endregion

#pragma region Ray Cast

                glUseProgram(ShaderDebug);
                glUniformMatrix4fv(glGetUniformLocation(ShaderDebug, "mtr"), 1, GL_FALSE, glm::value_ptr(ma));
                glBindVertexArray(VAO);
                glLineWidth(5);

                if (*menue.settings.debug)
                {
                    for (int t = 0; t < rdi * rdi; t++)
                    {
                        if (playerIndex + (t % rdi) - (rdi / 2) + (t / rdi - (rdi / 2)) * mz.count >= 0 &&
                            playerIndex + (t % rdi) - (rdi / 2) + (t / rdi - (rdi / 2)) * mz.count < mz.nodes.size()) {
                            int curIndex = playerIndex + (t % rdi) - (rdi / 2) + (t / rdi - (rdi / 2)) * mz.count;
                            for (int tt = 0; tt < mz.nodes[curIndex].ligh.size(); tt++) {
                                vec3 ray = player.inp->obj.pos;
                                vec3 startOfRay = mz.nodes[curIndex].ligh[tt].pos;
                                if (distance(ray, startOfRay) < LightSetings.x) {

                                    vec3 DirectionOfRay = normalize(ray - startOfRay);

                                    vec3 PR2 = ray;

                                    for (int j = 0; j < 9; j++)
                                    {
                                        if (playerIndex + (j % 3) - 1 + (j / 3 - 1) * mz.count >= 0 &&
                                            playerIndex + (j % 3) - 1 + (j / 3 - 1) * mz.count < mz.nodes.size()) {
                                            int rp = playerIndex + (j % 3) - 1 + (j / 3 - 1) * mz.count;
                                            for (int j = 0; j < mz.nodes[rp].fur.size(); j++)
                                            {
                                                if (mz.nodes[curIndex].ligh[tt].perch != vec2(rp, j)) {

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
                                    if (distance(PR2, ray) < 0.05) glUniform3f(glGetUniformLocation(ShaderDebug, "col"), 0, 1, 0);
                                    glDrawArrays(GL_LINES, 0, 4);


                                }

                            }
                        }
                    }

                    for (int tt = 0; tt < enmi.size(); tt++) {
                        vec3 ray = player.inp->obj.pos;
                        vec3 startOfRay = enmi[tt].pos + vec3(0, enmi[tt].obj.sca.y, 0);
                        if (distance(ray, startOfRay) < LightSetings.x) {

                            vec3 DirectionOfRay = normalize(ray - startOfRay);

                            vec3 PR2 = ray;

                            for (int j = 0; j < 9; j++)
                            {
                                if (playerIndex + (j % 3) - 1 + (j / 3 - 1) * mz.count >= 0 &&
                                    playerIndex + (j % 3) - 1 + (j / 3 - 1) * mz.count < mz.nodes.size()) {
                                    int rp = playerIndex + (j % 3) - 1 + (j / 3 - 1) * mz.count;
                                    for (int j = 0; j < mz.nodes[rp].fur.size(); j++)
                                    {
                                        {

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

                                                float t = dot(normalize(P[i]), P[i] + mz.nodes[rp].fur[j].obj.pos - startOfRay) / dot(normalize(P[i]), DirectionOfRay);
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
                            if (distance(PR2, ray) < 0.05) glUniform3f(glGetUniformLocation(ShaderDebug, "col"), 0, 1, 0);
                            glDrawArrays(GL_LINES, 0, 4);


                        }

                    }
                }


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

                menue.fullBox->children[0]->control(updown, leftright, inout, &lev, 0,deltaTime);

                glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
                glEnable(GL_DEPTH_TEST);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


                //glViewport(OFF_WIDTH + (SCR_WIDTH - SCR_HEIGHT) / 2, OFF_HEIGHT, SCR_HEIGHT, SCR_HEIGHT);
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