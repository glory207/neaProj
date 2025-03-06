#include "initMaze.h"
#include <string>
#include <random>

glm::mat4 Matrix(vec3 pos,vec3 sca, vec3 rot) {


    glm::mat4 modelViewMatrix;
    modelViewMatrix = translate(modelViewMatrix, pos);
   modelViewMatrix = rotate(modelViewMatrix, rot.z, glm::vec3(0, 0, 1));
   modelViewMatrix = rotate(modelViewMatrix, rot.y, glm::vec3(0, 1, 0));
   modelViewMatrix = rotate(modelViewMatrix, rot.x, glm::vec3(1, 0, 0));
    modelViewMatrix = scale(modelViewMatrix, sca);
    return modelViewMatrix;
}
Maze::Maze() {}
Maze::Maze(std::vector<Light>* ligh, int c){
    // Define the random number generator and distribution
    std::random_device rd;  // Seed generator
    std::mt19937 gen(rd()); // Mersenne Twister engine
    std::uniform_real_distribution<float> Rand(0.0f, 1.0f); // Range [0, 1]
    
    // the size of a room
    size = 2.0f;
    // the ratio of walkable space to wall / thickness of the path
    thk = 0.45f;
    // the number or rows and columns in the grid 
    count = c;

    // adds and spreads out the landmarks
    Camps.clear();
    for (int i = 0; i < 5; i++)
    {
        Camps.push_back(new EnCamp(vec2(Rand(gen), Rand(gen)), i));

    }
    for (int i = 0; i < count; i++)
    {
        Camps.push_back(new LootSpot(vec2(Rand(gen), Rand(gen))));

    }
    Landmark().set(&Camps,0,1);

    // adds the rooms
    for (int i = 0; i < count * count; i++)
    {
        nodes.push_back(*new Cell(i, count,thk));
    }
    // populates each room with landmarks
    for (int i = 0; i < count * count; i++)
    {
        // the probability of the path splitting 
        nodes[i].prob = 0.1f;
        for(Landmark* landm : Camps)
        {
            // transforms the landmark from a 0-1 to the worlds scale 
            int posX = (int)((*landm).Pos.x * count);
            int posY = (int)((*landm).Pos.y * count);
            if (dynamic_cast<EnCamp*>(landm) != nullptr)
            {
                EnCamp* Camp = dynamic_cast<EnCamp*>(landm);

               float sze = (*Camp).Size * count;
              // decreasing the number of walls in encampments
               if (glm::distance(vec2(posX, posY), vec2(nodes[i].x, nodes[i].y)) < sze)
               {
                  
                   nodes[i].prob = 0.7f;
               }
            }
            // adding a chest to loot spots
            else if (dynamic_cast<LootSpot*>(landm) != nullptr)
            {
                LootSpot* Camp = dynamic_cast<LootSpot*>(landm);
                if (nodes[i].index == posX + posY * count) {
                    nodes[i].treasure = true;
                }
            }
            (*landm).Pos.x = posX / float(count);
            (*landm).Pos.y = posY /float(count);
            // places the landmark into the center of its room
        }
        // connects the adjacent rooms together
        if (i != 0 && i % count != 0)
        {
            nodes[i].Conectablednodes.push_back(&nodes[i - 1]);
            nodes[i - 1].Conectablednodes.push_back(&nodes[i]);
        }
        if (i >= count)
        {
            nodes[i].Conectablednodes.push_back(&nodes[i - count]);
            nodes[i - count].Conectablednodes.push_back(&nodes[i]);
        }
    }

    // depth first search to create maze pattern
    std::vector<Cell*> nodeStack;
    nodeStack.push_back(&nodes[0]);
    Cell* curN = nodeStack.back();
    
    do{
        // sets the node as connected so it wont be visited again
        (*curN).connected = true;
        // the node has more branches to be searched
        if ((*curN).Conectablednodes.size() > 0)
        {
            // selects a random branch
            int rnd = floor(Rand(gen) * (*curN).Conectablednodes.size());

            // if the branch can be searched
            if (!(*(*curN).Conectablednodes[rnd]).connected || Rand(gen) <= (*curN).prob)
            {
                nodeStack.push_back((*curN).Conectablednodes[rnd]);
                (*curN).Connectednodes.push_back((*curN).Conectablednodes[rnd]);
                (*(*curN).Conectablednodes[rnd]).Connectednodes.push_back(curN);
            }

            // removes the branch from the node
            std::vector<Cell*>::iterator it = std::find((*(*curN).Conectablednodes[rnd]).Conectablednodes.begin(), (*(*curN).Conectablednodes[rnd]).Conectablednodes.end(), curN);
            if (it != (*(*curN).Conectablednodes[rnd]).Conectablednodes.end())
            {
                (*(*curN).Conectablednodes[rnd]).Conectablednodes.erase(it);
            }

            // removes the node from the branch
            it = std::find((*curN).Conectablednodes.begin(), (*curN).Conectablednodes.end(), (*curN).Conectablednodes[rnd]);
            if (it != (*curN).Conectablednodes.end())
            {
                (*curN).Conectablednodes.erase(it);
            }

        }
        else
        {
            // since the node has no more branches to be searched its poped from the stack
            nodeStack.pop_back();
        }
        // sets the current node as the one at the top of the stack 
        if(!nodeStack.empty())curN = nodeStack.back();
    } while (!nodeStack.empty());
    




    // populates each room with furniture

    for (int i = 0; i < nodes.size(); i++) {
        nodes[i].set();
        // the position of the node that the furniture will be relative to
        vec3 ps = vec3((i % count) * size, 0.001, (i / count) * size);
        // creates an empty room
        int grid[9][9] = {
            {0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0}
        };
        // blocks the path for objects but not path finding
        if (nodes[i].sidesIsPath[0]) grid[4][8] = 1;
        if (nodes[i].sidesIsPath[1]) grid[4][0] = 1;
        if (nodes[i].sidesIsPath[2]) grid[0][4] = 1;
        if (nodes[i].sidesIsPath[3]) grid[8][4] = 1;
        // add treasure chests
        if (nodes[i].treasure)
        {
            int furrot = floor(Rand(gen) * 4);
            Furniture::canFit(-2, furrot, &grid);
            Furniture ref = Furniture(-2, furrot, &grid, size, thk, ps);
            nodes[i].fur.push_back(ref);
        }
        for (int j = 0; j < 6; j++) 
        {
            // randomizes the type and position of the furniture
            int furType = floor(Rand(gen) * 8);
            int furRot = floor(Rand(gen) * 4);
            
            if(Furniture::canFit(furType, furRot,&grid))
            {
                Furniture ref = Furniture(furType, furRot, &grid, size, thk, ps);
                
                nodes[i].fur.push_back(ref);
                // adds a light to go with the tourch
               if(furType == 4)
               {
                   Light l = Light(vec3(ref.obj.pos.x, ref.obj.pos.y + ref.obj.sca.y, ref.obj.pos.z));
                   l.perch = vec2(i, nodes[i].fur.size()-1);
                   (*ligh).push_back(l);
               }
               
            }
        }
        for (int j = 0; j < 9*9; j++)
        {
            // creats the grid used for path finding and sets the obstructions
            nodes[i].grid[j % 9][j / 9] = CellGrid(nodes[i].pnt(0, 0) + vec2((j % 9) - 4,(j / 9) - 4) * vec2(thk * 0.222f), j % 9, j / 9, i);
            if (grid[j % 9][j / 9] >= 2)nodes[i].grid[j % 9][j / 9].obstruction = true;
        }

    }
    // creats the list of points for the 3D maze
    thk = 0.452f;
    std::vector<glm::vec2> lst1;
    std::vector<glm::vec2> lst2 = makeLines(0, 3, 2, 3, -1, -1, false,0);
    lst1.insert(lst1.end(),lst2.begin(),lst2.end());
    lst2 = makeLines(3, 0, 1, 3, -1, -1, true,0);
    std::reverse(lst2.begin(),lst2.end());
    lst1.insert(lst1.end(),lst2.begin(),lst2.end());
    lst2 = makeLines(1, 3, 0, 1, 2, 3, false,0);
    std::reverse(lst2.begin(),lst2.end());
    lst1.insert(lst1.end(),lst2.begin(),lst2.end());
    lst2 = makeLines(2, 0, 0, 2, 1, 3, true,0);
    lst1.insert(lst1.end(),lst2.begin(),lst2.end());
    lst1.push_back(nodes[0].pnt(0, thk));
    lst1.push_back(nodes[nodes.size() - 1].pnt(3, thk));
    // initialize the mazes object
    buffers = initMazeBuffers(lst1);
    obj = SpObj(glm::vec3(0), glm::vec3(0), glm::vec3(size),buffers,0,5);
    obj.textOff = glm::vec4(0.0f,0.0f, size * 2.2f, size * 2.2f);
    obj.textOff2 = glm::vec4(0.0f,0.0f, size * 2.2f, size * 2.2f);

    // creats the list of points for the 2D map

    thk = 0.5f;
    lst1.clear();
    lst1.push_back(nodes[0].pnt(0, thk));
    lst1.push_back(nodes[count-1].pnt(1, thk));
    lst1.push_back(nodes[0].pnt(0, thk));
    lst1.push_back(nodes[count*count- count].pnt(2, thk));


    lst2 = makeLines(0, 3, 2, 3, -1, -1, false, 0);
    lst1.insert(lst1.end(), lst2.begin(), lst2.end());
    lst2 = makeLines(3, 0, 1, 3, -1, -1, true, 0);
    std::reverse(lst2.begin(), lst2.end());
    lst1.insert(lst1.end(), lst2.begin(), lst2.end());


    thk = 0.45f;

    MapBuffers = initMaze2DBuffers(lst1);
    // as the map is simpler the extras that come with objects is unnecessary
    // just the Buffer is needed
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, MapBuffers.positions);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, MapBuffers.indices);


    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
std::vector<glm::vec2> Maze::makeLines(int tp,int  sd,int  p0,int  p1,int  p2,int  p3,bool  swch,float thin){
      std::vector<glm::vec2> lst;
      // turns the nodes into a list of points
        if (!nodes[0].sidesIsPath[tp]){
          
          lst.push_back(nodes[0].pnt(thin+p0, thk));
        } 
        for (int k = 0; k < nodes.size(); k++)
        {
            // pointers
            int node_Current = 0;
            int node_To_The_Side = 0;
            int node_Above = 0;
            int node_Above_To_The_Side = 0;
            if (swch)
            {
                // when vertical 
                node_Current = ((k % count) * count) + std::floor(k / count);
                node_To_The_Side = (((k + 1) % count) * count) + std::floor((k + 1) / count);
                node_Above = ((k % count) * count) + std::floor(k / count) - 1;
                node_Above_To_The_Side = ((k % count) * count) + std::floor(k / count) - 1 + count;
            }
            else
            {
                // when horizontal
                node_Current = k;
                node_To_The_Side = k + 1;
                node_Above = k - count;
                node_Above_To_The_Side = k - count + 1;
            }
            
  
            // dictates when a wall starts and ends
            if (!nodes[node_Current].sidesIsPath[tp] && !nodes[node_Current].sidesIsPath[sd])
            {
              lst.push_back(nodes[node_Current].pnt(thin+p1, thk));
  
                if (node_Current != nodes.size() - 1 && !nodes[node_To_The_Side].sidesIsPath[tp]){
                     lst.push_back(nodes[node_To_The_Side].pnt(thin+p0, thk));
                     }
            }
            if (!nodes[node_Current].sidesIsPath[tp] && nodes[node_Current].sidesIsPath[sd])
            {
                if (nodes[node_To_The_Side].sidesIsPath[tp])
                {
                  lst.push_back(nodes[node_To_The_Side].pnt(thin+p0, thk));
                    if (p2 != -1 && nodes[node_Above].sidesIsPath[sd])
                    {
                     
                      lst.push_back(nodes[node_To_The_Side].pnt(thin+p0, thk));
                    
                      lst.push_back(nodes[node_Above_To_The_Side].pnt(thin+p2, thk));
                      
                      
  
                    }
                }
  
            }
            if (nodes[node_Current].sidesIsPath[tp] && !nodes[node_Current].sidesIsPath[sd])
            {
             
              if (node_Current != nodes.size() - 1 && !nodes[node_To_The_Side].sidesIsPath[tp]){
                 lst.push_back(nodes[node_To_The_Side].pnt(thin+p0, thk));
                 }
            }
            if (nodes[node_Current].sidesIsPath[tp] && nodes[node_Current].sidesIsPath[sd])
            {
                if (!nodes[node_To_The_Side].sidesIsPath[tp])
                {
                    if (p2 != -1 && nodes[node_Above].sidesIsPath[sd])
                    {
                      lst.push_back(nodes[node_Above].pnt(thin+p3, thk));
  
                      lst.push_back(nodes[node_Current].pnt(thin+p1, thk));
                    }
  
                    lst.push_back(nodes[node_Current].pnt(thin+p1, thk));
  
  
                }
            }
                
        }
        return lst;
    }


float Maze::project(float x, float y, int side, Furniture f) {
    // get the distance from point xy to the furnitures center parralel to the edge of the furniture
    if (side == 0) return x * cos(f.obj.rot.y) - y * sin(f.obj.rot.y);
    // uses the other edge
    else  return x * sin(f.obj.rot.y) + y * cos(f.obj.rot.y);
}

bool Maze::collide(glm::vec3* pos, glm::vec3* acc, glm::vec2 leway) {
    glm::vec3 temp;
    if (acc == nullptr) acc = new glm::vec3(0);
    // if the object is not in the air
    bool grnd = false;
    // the minimun distance the object can be away from a wall
    leway.x /= -size;
    (*pos) /= size;
    // the node the object is within
    int nd = (int)((*pos).x + 0.5f) + (int)((*pos).z + 0.5f) * count;
    if (nd >= 0 && nd< count* count)
    {
        // if the object is within the walls move it out
        if (!nodes[nd].sidesIsPath[0] && (int)((*pos).z + 0.5f) - (*pos).z < -(thk + leway.x)) {
            // console.log((int)((*pos).x+0.5) -(*pos).x , (int)((*pos).x+0.5) - (*pos).x);
            (*pos).z += (thk + leway.x) + (int)((*pos).z +0.5f) - (*pos).z;
            (*acc).z *= -1;
            //std::cout << "00" << std::endl;
        }
        if (!nodes[nd].sidesIsPath[1] && (int)((*pos).z +0.5f) - (*pos).z > (thk + leway.x)) {
            // console.log((int)((*pos).x+0.5) -(*pos).x , (int)((*pos).x+0.5) - (*pos).x);
            (*pos).z += (int)((*pos).z +0.5f) - (*pos).z - (thk + leway.x);
            (*acc).z *= -1;
            //std::cout << "01" << std::endl;
        }

        if (!nodes[nd].sidesIsPath[2] && (int)((*pos).x +0.5f) - (*pos).x > (thk + leway.x)) {
            //console.log((int)((*pos).x+0.5) -(*pos).x , (int)((*pos).x+0.5) - (*pos).x);
            (*pos).x += (int)((*pos).x +0.5f) - (*pos).x - (thk + leway.x);
            (*acc).x *= -1;

            //std::cout << "02" << std::endl;
        }
        if (!nodes[nd].sidesIsPath[3] && (int)((*pos).x +0.5f) - (*pos).x < -(thk + leway.x)) {
            // console.log((int)((*pos).x+0.5) -(*pos).x , (int)((*pos).x+0.5) - (*pos).x);
            (*pos).x += (int)((*pos).x +0.5f) - (*pos).x + (thk + leway.x);
            (*acc).x *= -1;
            //std::cout << "03" << std::endl;
        }

        // the 4 corners

        if (nodes[nd].sidesIsPath[1] && (int)((*pos).z +0.5f) - (*pos).z > (thk + leway.x) &&
            nodes[nd].sidesIsPath[3] && (int)((*pos).x +0.5f) - (*pos).x < -(thk + leway.x) &&
            (
                !nodes[(int)((*pos).x +0.5f) + 1.0 + (int)((*pos).z +0.5f - 1.0f) * count].sidesIsPath[0] ||
                !nodes[(int)((*pos).x +0.5f) + 1.0 + (int)((*pos).z +0.5f - 1.0f) * count].sidesIsPath[2]
                )
            ) {
            if (abs((int)((*pos).z +0.5f) - (*pos).z - (thk + leway.x)) < abs((int)((*pos).x +0.5f) - (*pos).x + (thk + leway.x)))
            {
                (*pos).z += (int)((*pos).z +0.5f) - (*pos).z - (thk + leway.x); (*acc).z *= -1;
            }
            else { (*pos).x += (int)((*pos).x +0.5f) - (*pos).x + (thk + leway.x); (*acc).x *= -1; }

        }


        if (nodes[nd].sidesIsPath[1] && (int)((*pos).z +0.5f) - (*pos).z > (thk + leway.x) &&
            nodes[nd].sidesIsPath[2] && (int)((*pos).x +0.5f) - (*pos).x > (thk + leway.x) &&
            (
                !nodes[(int)((*pos).x +0.5f) - 1.0 + (int)((*pos).z +0.5f - 1.0f) * count].sidesIsPath[0] ||
                !nodes[(int)((*pos).x +0.5f) - 1.0 + (int)((*pos).z +0.5f - 1.0f) * count].sidesIsPath[3]
                )
            ) {

            if (abs((int)((*pos).z +0.5f) - (*pos).z - (thk + leway.x)) < abs((int)((*pos).x +0.5f) - (*pos).x - (thk + leway.x)))
            {
                (*pos).z += (int)((*pos).z +0.5f) - (*pos).z - (thk + leway.x); (*acc).z *= -1;
            }
            else { (*pos).x += (int)((*pos).x +0.5f) - (*pos).x - (thk + leway.x); (*acc).x *= -1; }

        }


        if (nodes[nd].sidesIsPath[0] && (int)((*pos).z +0.5f) - (*pos).z < -(thk + leway.x) &&
            nodes[nd].sidesIsPath[3] && (int)((*pos).x +0.5f) - (*pos).x < -(thk + leway.x) && (
                !nodes[(int)((*pos).x +0.5f) + 1.0 + (int)((*pos).z +0.5f + 1.0f) * count].sidesIsPath[1] ||
                !nodes[(int)((*pos).x +0.5f) + 1.0 + (int)((*pos).z +0.5f + 1.0f) * count].sidesIsPath[2]
                )
            ) {

            if (abs((thk + leway.x) + (int)((*pos).z +0.5f) - (*pos).z) < abs((int)((*pos).x +0.5f) - (*pos).x + (thk + leway.x)))
            {
                (*pos).z += (thk + leway.x) + (int)((*pos).z +0.5f) - (*pos).z; (*acc).z *= -1;
            }
            else { (*pos).x += (int)((*pos).x +0.5f) - (*pos).x + (thk + leway.x); (*acc).x *= -1; }

        }


        if (nodes[nd].sidesIsPath[0] && (int)((*pos).z +0.5f) - (*pos).z<-(thk + leway.x) &&
            nodes[nd].sidesIsPath[2] && (int)((*pos).x +0.5f) - (*pos).x>(thk + leway.x) && (
                !nodes[(int)((*pos).x +0.5f) - 1.0 + (int)((*pos).z +0.5f + 1.0f) * count].sidesIsPath[1] ||
                !nodes[(int)((*pos).x +0.5f) - 1.0 + (int)((*pos).z +0.5f + 1.0f) * count].sidesIsPath[3]
                )
            ) {
            if (abs((thk + leway.x) + (int)((*pos).z +0.5f) - (*pos).z) < abs((int)((*pos).x +0.5f) - (*pos).x - (thk + leway.x)))
            {
                (*pos).z += (thk + leway.x) + (int)((*pos).z +0.5f) - (*pos).z; (*acc).z *= -1;
            }
            else { (*pos).x += (int)((*pos).x +0.5f) - (*pos).x - (thk + leway.x); (*acc).x *= -1; }

        }
    }
    else
    {
        //if outside the boundries of the game return them 
        (*pos).x = 0;
        (*pos).z = 0;
    }
    leway.x *= -size;

    (*pos) *= size;


    for (Furniture f : nodes[nd].fur) {
        // if close enough to collide with the furniture
        if (pow((*pos).x - f.obj.pos.x, 2.0) + pow((*pos).z - f.obj.pos.z, 2.0) < max(f.obj.sca.x, f.obj.sca.z) * 2.0)
        {

            vec2 Points[] = {
                    vec2(-1, 1),
                    vec2(-1, -1),
                    vec2(1, -1),
                    vec2(1, 1),
            };
            // gets the position of the 4 points at the base of the furniture
            for (int i = 0; i < 4; i++) {
                Points[i].x *= f.obj.sca.x ;
                Points[i].y *= f.obj.sca.z ;

                vec2 PP = vec2(Points[i].x * cos(f.obj.rot.y) + Points[i].y * sin(f.obj.rot.y), -Points[i].x * sin(f.obj.rot.y) + Points[i].y * cos(f.obj.rot.y));
                Points[i] = PP;

                Points[i].x += f.obj.pos.x;
                Points[i].y += f.obj.pos.z;
                
            }

            bool inside1 = false;
            bool inside2 = false;
            float firstDist = 0.0;
            float secondDist = 0.0;

            // gets the magnitude of how deep within the furniture it is from one side
            float max = -90000.0;
            float min = 90000.0;
            for (int i = 0; i < 4; i++) {
                float test = project(Points[i].x, Points[i].y, 0,f);
                if (test > max)max = test;
                if (test < min)min = test;
            }
            float playProj = project((*pos).x, (*pos).z, 0,f);
            if (playProj + leway.x > min && playProj - leway.x < max) {
                inside1 = true;
                if (playProj + leway.x - min > max - playProj - leway.x) firstDist = max - playProj + leway.x;
                else firstDist = min - playProj - leway.x;
            }

            // gets the magnitude of how deep within the furniture it is from the other side
            max = -90000.0;
            min = 90000.0;
            for (int i = 0; i < 4; i++) {
                float test = project(Points[i].x, Points[i].y, 1,f);
                if (test > max)max = test;
                if (test < min)min = test;
            }
            playProj = project((*pos).x, (*pos).z, 1,f);
            if (playProj + leway.x > min && playProj - leway.x < max) {
                inside2 = true;
                if (playProj + leway.x - min > max - playProj - leway.x) secondDist = max - playProj + leway.x;
                else secondDist = min - playProj - leway.x;


            }
            // if inside it from both sides and the top then it must be moved out
            if (inside1 && inside2 && (*pos).y - leway.y < f.obj.pos.y + f.obj.sca.y) {
                //choses the direction where the least movment nessesery is chosen
                if (abs(firstDist) > abs(f.obj.sca.y + f.obj.sca.y + leway.y - (*pos).y) && abs(secondDist) > abs(f.obj.sca.y + f.obj.sca.y + leway.y - (*pos).y)) {
                    (*pos).y = f.obj.pos.y + f.obj.sca.y + leway.y-0.001f;
                    // if standing on top of the furniture its not in the air
                    grnd = true;
                    if ((*acc).y < 0)  (*acc).y *= -1.0;
                }
                else
                if (abs(firstDist) > abs(secondDist))
                {
                    // w = v - 2 * (v ∙ n) * n
                    vec3 acct = vec3((*acc).x - 2 * ((*acc).x * (sin(f.obj.rot.y)) + (*acc).z * (cos(f.obj.rot.y))) * (sin(f.obj.rot.y)),
                        (*acc).y,
                        (*acc).z - 2 * ((*acc).x * (sin(f.obj.rot.y)) + (*acc).z * (cos(f.obj.rot.y))) * (cos(f.obj.rot.y))
                    );
                   
                    (*acc).x = acct.x;
                    (*acc).y = acct.y;
                    (*acc).z = acct.z;
                    (*pos).x += sin(f.obj.rot.y) * secondDist;
                    (*pos).z += cos(f.obj.rot.y) * secondDist;
                }
                else
                {
                    vec3 acct = vec3((*acc).x - 2 * ((*acc).x * (cos(f.obj.rot.y)) + (*acc).z * -(sin(f.obj.rot.y))) * (cos(f.obj.rot.y)),
                        (*acc).y,
                        (*acc).z - 2 * ((*acc).x * (cos(f.obj.rot.y)) + (*acc).z * -(sin(f.obj.rot.y))) * -(sin(f.obj.rot.y))
                    );
                    (*acc).x = acct.x;
                    (*acc).y = acct.y;
                    (*acc).z = acct.z;
                    (*pos).x += cos(f.obj.rot.y) * firstDist;
                    (*pos).z += -sin(f.obj.rot.y) * firstDist;
                }
                
            }
           
        }
    }
    
    // if standing on the ground its not in the air
    if ((*pos).y < leway.y) {
        (*pos).y = leway.y - 0.001f;;
        if ( (*acc).y < 0.0f)  (*acc).y *= -1.0f;

        grnd = true;
    }

    return grnd;
}


std::vector<CellGrid*> Maze::getpath(int sx, int sy,int start, int ex, int ey, int end) {
    std::random_device rd;  // Seed generator
    std::mt19937 gen(rd()); // Mersenne Twister engine
    std::uniform_real_distribution<float> Rand(0.0f, 1.0f); // Range [0, 1]

    // validation for the starting and ending points
    if (sx > 8)sx = 8;
    if (sy > 8)sy = 8;
    if (sx < 0)sx = 0;
    if (sy < 0)sy = 0;
    if (ex > 8)ex = 8;
    if (ey > 8)ey = 8;
    if (ex < 0)ex = 0;
    if (ey < 0)ey = 0;
    if (start > nodes.size() - 1)start = nodes.size() - 1;
    if (start < 0)start = 0;
    if (end > nodes.size() - 1)end = nodes.size() - 1;
    if (end < 0)end = 0;
    std::vector<CellGrid*> path;
    for (int i = 0; i < nodes.size(); i++)
    {
        // clears the parent node and distance values for a new search
        nodes[i].setSearch(&nodes[start].grid[sx][sy]);
    }
    bool found = false;
    // the room x and y position and the nodes index
    int CurentN[3] = { ex,ey,end };
    std::vector<CellGrid*> nodesToSearch;
    while (!found)
    {
        if (CurentN[0] == sx && CurentN[1] == sy && CurentN[2] == start) {
            CellGrid* cur = &nodes[CurentN[2]].grid[CurentN[0]][CurentN[1]];
            // bactrack through the nodes parents to get the path
            while (cur != nullptr)
            {

                path.push_back(cur);
                cur = cur->parent;
            }

            found = true;
        }
        else
        {
            nodes[CurentN[2]].grid[CurentN[0]][CurentN[1]].visited = true;
            // removes the current node as it has been searched
            std::vector<CellGrid*>::iterator it = std::find(nodesToSearch.begin(), nodesToSearch.end(), &nodes[CurentN[2]].grid[CurentN[0]][CurentN[1]]);
            if (it != nodesToSearch.end())
            {
                nodesToSearch.erase(it);
            }
            // if the adjacent cell grid is not obstructed ts should be added to the list
            CellGrid* adjacent;
            adjacent = adjacentCellGrid(CurentN,-1,0,&nodesToSearch);
            if (adjacent != nullptr) {
                if (!adjacent->obstruction || adjacent == &nodes[start].grid[4][4] || adjacent == &nodes[end].grid[4][4]) {
                    nodesToSearch.push_back(adjacent);
                }

            }
            adjacent = adjacentCellGrid(CurentN,1,0,&nodesToSearch);
            if (adjacent != nullptr) {
                if (!adjacent->obstruction || adjacent == &nodes[start].grid[4][4] || adjacent == &nodes[end].grid[4][4]) {
                    nodesToSearch.push_back(adjacent);
                }

            }
            adjacent = adjacentCellGrid(CurentN,0,1,&nodesToSearch);
            if (adjacent != nullptr) {
                if (!adjacent->obstruction || adjacent == &nodes[start].grid[4][4] || adjacent == &nodes[end].grid[4][4]) {
                    nodesToSearch.push_back(adjacent);
                }

            }
            adjacent = adjacentCellGrid(CurentN,0,-1,&nodesToSearch);
            if (adjacent != nullptr) {
                if (!adjacent->obstruction || adjacent == &nodes[start].grid[4][4] || adjacent == &nodes[end].grid[4][4]) {
                    nodesToSearch.push_back(adjacent);
                }

            }
            // sorts based of the distance traveled plus holistic
            std::sort(nodesToSearch.begin(), nodesToSearch.end(), order);
            if (nodesToSearch.size() == 0) {
                // faild search
                found = true;
            }
            else
            {
                // makes the smallest the nexted to be searched
                CurentN[0] = nodesToSearch[0]->x;
                CurentN[1] = nodesToSearch[0]->y;
                CurentN[2] = nodesToSearch[0]->p;
            }
        }
    }

    return path;
}

CellGrid* Maze::adjacentCellGrid(int CurentN[3],int px,int py, std::vector<CellGrid*>* nodesToSearch) {
    bool canAdd = true;
    int addingN[3] = { CurentN[0] + px,CurentN[1] + py,CurentN[2] };
    // idd the adjacent cell grid is ouside the origional grid its translated to fit onto the adjacent node
    // but only if there is a path to it
    if (addingN[1] == -1) {
        addingN[1] = 8;
        addingN[2] -= count;
        if (!nodes[CurentN[2]].sidesIsPath[1]) canAdd = false;
    }
    if (addingN[1] == 9) {
        addingN[1] = 0;
        addingN[2] += count;
        if (!nodes[CurentN[2]].sidesIsPath[0]) canAdd = false;
    }
    if (addingN[0] == -1) {
        addingN[0] = 8;
        addingN[2] -= 1;
        if (!nodes[CurentN[2]].sidesIsPath[2]) canAdd = false;
    }
    if (addingN[0] == 9) {
        addingN[0] = 0;
        addingN[2] += 1;
        if (!nodes[CurentN[2]].sidesIsPath[3]) canAdd = false;
    }
    // the cell grid is within the map and valid
    if (canAdd && addingN[2] >= 0 && addingN[2] < nodes.size() && !nodes[addingN[2]].grid[addingN[0]][addingN[1]].visited) {
        nodes[addingN[2]].grid[addingN[0]][addingN[1]].parent = &nodes[CurentN[2]].grid[CurentN[0]][CurentN[1]];
        nodes[addingN[2]].grid[addingN[0]][addingN[1]].Distance = nodes[CurentN[2]].grid[CurentN[0]][CurentN[1]].Distance + 1;
        // so long as its not allready inside nodesToSearch it will be added to it
        std::vector<CellGrid*>::iterator it2 = std::find((*nodesToSearch).begin(), (*nodesToSearch).end(), &nodes[addingN[2]].grid[addingN[0]][addingN[1]]);
        if (it2 == (*nodesToSearch).end())
        {
            return &nodes[addingN[2]].grid[addingN[0]][addingN[1]];
        }
    }
    return nullptr;
}

bool order(CellGrid* a, CellGrid* b) {
    // orders the nodesToSearch by the distance traveled plus holistic
    return a->Distance+a->Holistic < b->Distance + b->Holistic;
}


Cell::Cell(int index, int count, float thk){

      this->index = index;
      this->count = count;
      this->x = index%count;
      this->y = glm::floor(index/count);
      this->connected = false;
      
  
  
      float posx = x;
      float posy = y;

     
}
glm::vec2 Cell::pnt(int i, float thk) {
    // the corners of the cell
       if(i == 0)return glm::vec2(x - thk, y - thk);
       else if(i == 1)return glm::vec2(x + thk, y - thk);
       else if(i == 2)return glm::vec2(x - thk, y + thk);
       else if(i == 3)return glm::vec2(x + thk, y + thk);
       else if(i == 4)return glm::vec2(x,y);
       else if(i == 5)return glm::vec2(x - 0.5, y - 0.5);
       else if(i == 6)return glm::vec2(x + 0.5, y - 0.5);
       else if(i == 7)return glm::vec2(x - 0.5, y + 0.5);
       else if(i == 8)return glm::vec2(x + 0.5, y + 0.5);
       else if(i == 9)return glm::vec2(x,y);
    
}

void Cell::set(){
    for (int i = 0; i < Connectednodes.size(); i++) { 
        
        if ((*Connectednodes[i]).index == index + count) { sides.push_back(0); 
        sidesIsPath[0] = true;
        }
        if((*Connectednodes[i]).index == index - count) {sides.push_back(1); 
        sidesIsPath[1] = true;
        }
        if ((*Connectednodes[i]).index == index - 1){ sides.push_back(2); 
        sidesIsPath[2] = true;
        }
        if ((*Connectednodes[i]).index == index + 1){ sides.push_back(3); 
        sidesIsPath[3] = true;
        }
        
      }
}


void Cell::setSearch(CellGrid* start) {

    for (int i = 0; i < 9*9; i++)
    {
        grid[i % 9][i / 9].visited = false;
        grid[i % 9][i / 9].parent = nullptr;
        grid[i % 9][i / 9].isPath = false;
        grid[i % 9][i / 9].Holistic = abs(start->pos.x - grid[i % 9][i / 9].pos.x) + abs(start->pos.y - grid[i % 9][i / 9].pos.y);
        grid[i % 9][i / 9].Distance = INFINITY;
        grid[i % 9][i / 9].DH = INFINITY;
    }
}


EnCamp::EnCamp(vec2 pos, float size) {

    // Define the random number generator and distribution
    std::random_device rd;  // Seed generator
    std::mt19937 gen(rd()); // Mersenne Twister engine
    std::uniform_real_distribution<float> Rand(0.0f, 1.0f); // Range [0, 1]

    Pos = pos;
    Size = size * 0.03f + 0.05f;

    int numOfC = int(size)+1;
    for (int i = 0; i < numOfC; i++)
    {
        cages.push_back(new Landmark((vec2(Rand(gen), Rand(gen)) - vec2(0.5f)) * 0.5f,0.01f));
    }
    Landmark().set(&cages, -0.5f, 0.5f);


}

LootSpot::LootSpot(vec2 pos) {
    Pos = pos;
    Size = 0.01f;
}

CellGrid::CellGrid() {

}
CellGrid::CellGrid(vec2 pos, int x, int y, int p) {
    this->pos = pos;
    bool visited = false;
    CellGrid* parent = nullptr;
    bool isPath = false;
    int Holistic = 0; 
    int Distance = INFINITY; 
    int DH = INFINITY; 
    this->x = x;
    this->y = y;
    this->p = p;
}

void Landmark::set(std::vector<Landmark*>* marks, float min, float max) {
    bool isfixed;
    do
    {
        isfixed = true;
        for (int i = 0; i < (*marks).size(); i++)
        {
            for (int j = 0; j < (*marks).size(); j++)
            {
                if ((*(*marks)[i]).Pos.x + (*(*marks)[i]).Size > 1) (*(*marks)[i]).Pos.x = 1 - (*(*marks)[i]).Size;
                if ((*(*marks)[i]).Pos.x - (*(*marks)[i]).Size < 0) (*(*marks)[i]).Pos.x = 0 + (*(*marks)[i]).Size;
                if ((*(*marks)[i]).Pos.y - (*(*marks)[i]).Size < 0) (*(*marks)[i]).Pos.y = 0 + (*(*marks)[i]).Size;
                if ((*(*marks)[i]).Pos.y + (*(*marks)[i]).Size > 1) (*(*marks)[i]).Pos.y = 1 - (*(*marks)[i]).Size;
                if (j != i)
                {
                    if (glm::distance((*(*marks)[i]).Pos, (*(*marks)[j]).Pos) < (*(*marks)[i]).Size + (*(*marks)[j]).Size)
                    {
                        vec2 mv = (*(*marks)[i]).Pos - (*(*marks)[j]).Pos;
                        mv = glm::normalize(mv) * (glm::distance((*(*marks)[i]).Pos, (*(*marks)[j]).Pos) - ((*(*marks)[i]).Size + (*(*marks)[j]).Size + 0.01f));


                        (*(*marks)[j]).Pos += mv * 0.5f;
                        (*(*marks)[i]).Pos -= mv * 0.5f;
                        isfixed = false;
                    }
                }

            }

        }
    } while (!isfixed);
}

Landmark::Landmark(vec2 P, float S) {
    Pos = P;
    Size = S;
}

Landmark::Landmark() {}