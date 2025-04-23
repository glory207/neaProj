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
Maze::Maze(int c){
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
    for (int i = 0; i < 6; i++)
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
    //
    for (Landmark* landm : Camps)
    {
        // transforms the landmark from a 0-1 to the worlds scale 
        int posX = (int)((*landm).Pos.x * count);
        int posY = (int)((*landm).Pos.y * count);
        if (dynamic_cast<EnCamp*>(landm) != nullptr)
        {
            EnCamp* Camp = dynamic_cast<EnCamp*>(landm);

            for (Landmark* cage : Camp->cages) {
                int CposX = (int)( (cage->Pos.x * Camp->Size + Camp->Pos.x) * count);
                int CposY = (int)( (cage->Pos.y * Camp->Size + Camp->Pos.y) * count);
                
                nodes[CposX + CposY * count].cage = true;
                (*cage).Pos.x = CposX/ float(count);
                (*cage).Pos.y = CposY/ float(count);

            }
        }
        // adding a chest to loot spots
        else if (dynamic_cast<LootSpot*>(landm) != nullptr)
        {
            nodes[posX + posY * count].treasure = true;
        }
        (*landm).Pos.x = posX / float(count);
        (*landm).Pos.y = posY / float(count);
        // places the landmark into the center of its room
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
                  
                   nodes[i].prob = 0.8f;
               }

            }
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

        for (int j = 0; j < 9 * 9; j++)
        {
            // creats the grid used for path finding and sets the obstructions
            nodes[i].grid[j % 9][j / 9] = CellGrid(nodes[i].pnt(0, 0) + vec2((j % 9) - 4, (j / 9) - 4) * vec2(thk * 0.222f), j % 9, j / 9, i);
        }

        // blocks the path for objects but not path finding
        if (nodes[i].sidesIsPath[0]) nodes[i].grid[4][8].obstruction = 1;
        if (nodes[i].sidesIsPath[1]) nodes[i].grid[4][0].obstruction = 1;
        if (nodes[i].sidesIsPath[2]) nodes[i].grid[0][4].obstruction = 1;
        if (nodes[i].sidesIsPath[3]) nodes[i].grid[8][4].obstruction = 1;
        // add treasure chests
        if (nodes[i].treasure)
        {
            int furrot = floor(Rand(gen) * 4);
            Furniture::canFit(-2, furrot, &nodes[i].grid);
            Furniture ref = Furniture(-2, furrot, &nodes[i].grid, size, thk, ps);
            nodes[i].fur.push_back(ref);
        }
        // add cage
        if (nodes[i].cage)
        {
            int furrot = floor(Rand(gen) * 4);
            Furniture::canFit(-3, furrot, &nodes[i].grid);
            Furniture ref = Furniture(-3, furrot, &nodes[i].grid, size, thk, ps);
            nodes[i].fur.push_back(ref);
        }
        for (int j = 0; j < 6; j++) 
        {
            // randomizes the type and position of the furniture
            int furType = floor(Rand(gen) * 8);
            int furRot = floor(Rand(gen) * 4);
            
            if(Furniture::canFit(furType, furRot,&nodes[i].grid))
            {
                Furniture ref = Furniture(furType, furRot, &nodes[i].grid, size, thk, ps);
                
                nodes[i].fur.push_back(ref);
                // adds a light to go with the tourch
               if(furType == 4)
               {
                   Light l = Light(vec3(ref.obj.pos.x, ref.obj.pos.y + ref.obj.sca.y, ref.obj.pos.z));
                   l.perch = vec2(i, nodes[i].fur.size()-1);
                   nodes[i].ligh.push_back(l);
               }
               
            }
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
}
void Maze::distroy() {

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
    // get the distance from point xy to the furnitures center parralel to the side of the furniture
    if (side == 0) return x * cos(f.obj.rot.y) - y * sin(f.obj.rot.y);
    // uses the other side
    else  return x * sin(f.obj.rot.y) + y * cos(f.obj.rot.y);
}

bool Maze::collide(glm::vec3* pos, float leway) {
    // if the object is not in the air
    bool grnd = false;
    // scales the player to the maze
    leway /= -size;
    (*pos) /= size;
    // the node the object is within
    int nd = (int)((*pos).x + 0.5f) + (int)((*pos).z + 0.5f) * count;
    // validates they are within the maze
    if (nd >= 0 && nd < count * count)
    {
        
        // if the object is within the walls move it out
        if (!nodes[nd].sidesIsPath[0] && (int)((*pos).z + 0.5f) - (*pos).z < -(thk + leway)) {
            (*pos).z += (thk + leway) + (int)((*pos).z + 0.5f) - (*pos).z;
        }
        if (!nodes[nd].sidesIsPath[1] && (int)((*pos).z + 0.5f) - (*pos).z > (thk + leway)) {
            (*pos).z += (int)((*pos).z + 0.5f) - (*pos).z - (thk + leway);
        }

        if (!nodes[nd].sidesIsPath[2] && (int)((*pos).x + 0.5f) - (*pos).x > (thk + leway)) {
            (*pos).x += (int)((*pos).x + 0.5f) - (*pos).x - (thk + leway);
        }
        if (!nodes[nd].sidesIsPath[3] && (int)((*pos).x + 0.5f) - (*pos).x < -(thk + leway)) {
            (*pos).x += (int)((*pos).x + 0.5f) - (*pos).x + (thk + leway);
        }

        // the 4 corners
        if (
            // the player is standing in the corner
            nodes[nd].sidesIsPath[1] && (int)((*pos).z + 0.5f) - (*pos).z > (thk + leway) &&
            nodes[nd].sidesIsPath[3] && (int)((*pos).x + 0.5f) - (*pos).x < -(thk + leway) &&
            (
                // the two nodes beside it have made a corner
            !nodes[(int)((*pos).x + 0.5f) + 1.0 + (int)((*pos).z + 0.5f - 1.0f) * count].sidesIsPath[0] ||
            !nodes[(int)((*pos).x + 0.5f) + 1.0 + (int)((*pos).z + 0.5f - 1.0f) * count].sidesIsPath[2]
            )
            ) 
        {
            if (abs((int)((*pos).z + 0.5f) - (*pos).z - (thk + leway)) < abs((int)((*pos).x + 0.5f) - (*pos).x + (thk + leway)))
            {
                (*pos).z += (int)((*pos).z + 0.5f) - (*pos).z - (thk + leway);
            }
            else { (*pos).x += (int)((*pos).x + 0.5f) - (*pos).x + (thk + leway);  }

        }


        if (nodes[nd].sidesIsPath[1] && (int)((*pos).z + 0.5f) - (*pos).z > (thk + leway) &&
            nodes[nd].sidesIsPath[2] && (int)((*pos).x + 0.5f) - (*pos).x > (thk + leway) &&
            (
                !nodes[(int)((*pos).x + 0.5f) - 1.0 + (int)((*pos).z + 0.5f - 1.0f) * count].sidesIsPath[0] ||
                !nodes[(int)((*pos).x + 0.5f) - 1.0 + (int)((*pos).z + 0.5f - 1.0f) * count].sidesIsPath[3]
                )
            ) {

            if (abs((int)((*pos).z + 0.5f) - (*pos).z - (thk + leway)) < abs((int)((*pos).x + 0.5f) - (*pos).x - (thk + leway)))
            {
                (*pos).z += (int)((*pos).z + 0.5f) - (*pos).z - (thk + leway);
            }
            else { (*pos).x += (int)((*pos).x + 0.5f) - (*pos).x - (thk + leway); }

        }


        if (nodes[nd].sidesIsPath[0] && (int)((*pos).z + 0.5f) - (*pos).z < -(thk + leway) &&
            nodes[nd].sidesIsPath[3] && (int)((*pos).x + 0.5f) - (*pos).x < -(thk + leway) && (
                !nodes[(int)((*pos).x + 0.5f) + 1.0 + (int)((*pos).z + 0.5f + 1.0f) * count].sidesIsPath[1] ||
                !nodes[(int)((*pos).x + 0.5f) + 1.0 + (int)((*pos).z + 0.5f + 1.0f) * count].sidesIsPath[2]
                )
            ) {

            if (abs((thk + leway) + (int)((*pos).z + 0.5f) - (*pos).z) < abs((int)((*pos).x + 0.5f) - (*pos).x + (thk + leway)))
            {
                (*pos).z += (thk + leway) + (int)((*pos).z + 0.5f) - (*pos).z;
            }
            else { (*pos).x += (int)((*pos).x + 0.5f) - (*pos).x + (thk + leway);  }

        }


        if (nodes[nd].sidesIsPath[0] && (int)((*pos).z + 0.5f) - (*pos).z<-(thk + leway) &&
            nodes[nd].sidesIsPath[2] && (int)((*pos).x + 0.5f) - (*pos).x>(thk + leway) && (
                !nodes[(int)((*pos).x + 0.5f) - 1.0 + (int)((*pos).z + 0.5f + 1.0f) * count].sidesIsPath[1] ||
                !nodes[(int)((*pos).x + 0.5f) - 1.0 + (int)((*pos).z + 0.5f + 1.0f) * count].sidesIsPath[3]
                )
            ) {
            if (abs((thk + leway) + (int)((*pos).z + 0.5f) - (*pos).z) < abs((int)((*pos).x + 0.5f) - (*pos).x - (thk + leway)))
            {
                (*pos).z += (thk + leway) + (int)((*pos).z + 0.5f) - (*pos).z;
            }
            else { (*pos).x += (int)((*pos).x + 0.5f) - (*pos).x - (thk + leway); }

        }
        
    }
    else
    {
        //if outside the boundries of the game return them 
        (*pos).x = 0;
        (*pos).z = 0;
    }
    // scale the world
    (*pos) *= size;

    leway *= -size;

    for (int f = 0; f < nodes[nd].fur.size(); f++) {
        // the hight of the chair collider is half that of its model
        // so must be halfed
        if (nodes[nd].fur[f].ColorTexture == 24) {
            nodes[nd].fur[f].obj.pos.y *= 0.5f;
            nodes[nd].fur[f].obj.sca.y *= 0.5f;
        }

        // if close enough to collide with the furniture
        if (pow((*pos).x - nodes[nd].fur[f].obj.pos.x, 2.0) + pow((*pos).z - nodes[nd].fur[f].obj.pos.z, 2.0)
            < max(nodes[nd].fur[f].obj.sca.x, nodes[nd].fur[f].obj.sca.z) * 2.0)
        {

            vec2 Points[] = {
                    vec2(-1, 1),
                    vec2(-1, -1),
                    vec2(1, -1),
                    vec2(1, 1),
            };
            // gets the position of the 4 points at the base of the furniture
            for (int i = 0; i < 4; i++) {
                // scale
                Points[i].x *= nodes[nd].fur[f].obj.sca.x;
                Points[i].y *= nodes[nd].fur[f].obj.sca.z;
                // rotate
                vec2 PP = vec2(Points[i].x * cos(nodes[nd].fur[f].obj.rot.y) + Points[i].y * sin(nodes[nd].fur[f].obj.rot.y),
                    -Points[i].x * sin(nodes[nd].fur[f].obj.rot.y) + Points[i].y * cos(nodes[nd].fur[f].obj.rot.y));
                Points[i] = PP;
                // translate
                Points[i].x += nodes[nd].fur[f].obj.pos.x;
                Points[i].y += nodes[nd].fur[f].obj.pos.z;

            }

            bool inside1 = false;
            bool inside2 = false;
            float firstDist = 0.0;
            float secondDist = 0.0;

            // gets the magnitude of how deep within the furniture it is from one side
            float max = project(Points[2].x, Points[2].y, 0, nodes[nd].fur[f]);
            float min = project(Points[1].x, Points[1].y, 0, nodes[nd].fur[f]);

            float playProj = project((*pos).x, (*pos).z, 0, nodes[nd].fur[f]);
            if (playProj + leway > min && playProj - leway < max) {
                inside1 = true;
                if (playProj + leway - min > max - playProj - leway) firstDist = max - playProj + leway;
                else firstDist = min - playProj - leway;
            }

            // gets the magnitude of how deep within the furniture it is from the other side
            max = project(Points[0].x, Points[0].y, 1, nodes[nd].fur[f]);
            min = project(Points[1].x, Points[1].y, 1, nodes[nd].fur[f]);

            playProj = project((*pos).x, (*pos).z, 1, nodes[nd].fur[f]);
            if (playProj + leway > min && playProj - leway < max) {
                inside2 = true;
                if (playProj + leway - min > max - playProj - leway) secondDist = max - playProj + leway;
                else secondDist = min - playProj - leway;


            }

            // if inside it from both sides and the top then it must be moved out
            if (inside1 && inside2 && (*pos).y < nodes[nd].fur[f].obj.pos.y + nodes[nd].fur[f].obj.sca.y) {
                //choses the direction where the least movment nessesery is chosen
                if (abs(firstDist) > abs(nodes[nd].fur[f].obj.sca.y + nodes[nd].fur[f].obj.sca.y - (*pos).y) && abs(secondDist) > abs(nodes[nd].fur[f].obj.sca.y + nodes[nd].fur[f].obj.sca.y - (*pos).y)) {
                    (*pos).y = nodes[nd].fur[f].obj.pos.y + nodes[nd].fur[f].obj.sca.y - 0.001f;
                    // if standing on top of the furniture its not in the air
                    grnd = true;
                }
                else
                    if (abs(firstDist) > abs(secondDist))
                    {
                        (*pos).x += sin(nodes[nd].fur[f].obj.rot.y) * secondDist;
                        (*pos).z += cos(nodes[nd].fur[f].obj.rot.y) * secondDist;
                    }
                    else
                    {
                        (*pos).x += cos(nodes[nd].fur[f].obj.rot.y) * firstDist;
                        (*pos).z += -sin(nodes[nd].fur[f].obj.rot.y) * firstDist;
                    }

            }

        }


        if (nodes[nd].fur[f].ColorTexture == 24) {
            nodes[nd].fur[f].obj.pos.y *= 2.0f;
            nodes[nd].fur[f].obj.sca.y *= 2.0f;
        }
    }

    // if standing on the ground its not in the air
    if ((*pos).y < 0) {
        (*pos).y = -0.001f;


        grnd = true;
    }

    return grnd;
}


std::vector<CellGrid*> Maze::getpath(CellGrid* start, CellGrid* end) {

    for (int i = 0; i < nodes.size(); i++)
    {
        // clears the parent node and distance values for a new search
        nodes[i].setSearch(start);
    }
    bool found = false;
    CellGrid* CurentN = end;
    std::vector<CellGrid*> nodesToSearch;
    std::vector<CellGrid*> path; 
    while (!found)
    {
        if (CurentN == start) {
            CellGrid* cur = CurentN;
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
            CurentN->visited = true;
            // removes the current node as it has been searched
            if (!nodesToSearch.empty()) nodesToSearch.pop_back();

            // if the adjacent cell grid is not obstructed ts should be added to the list
            adjacentCellGrid(CurentN, 1, 0, &nodesToSearch);
            adjacentCellGrid(CurentN, 0, 1, &nodesToSearch);
            adjacentCellGrid(CurentN, -1, 0, &nodesToSearch);
            adjacentCellGrid(CurentN, 0, -1, &nodesToSearch);


            // sorts based of the distance traveled plus holistic
            std::sort(nodesToSearch.begin(), nodesToSearch.end(), order);
            if (nodesToSearch.size() == 0) {
                // faild search
                found = true;
            }
            else
            {
                // makes the smallest the nexted to be searched
                CurentN = nodesToSearch.back();
            }
        }
    }

    return path;
}

void Maze::adjacentCellGrid(CellGrid* CurentN,int px,int py, std::vector<CellGrid*>* nodesToSearch) {
    bool canAdd = true;
    int addingN[3] = { CurentN->x + px,CurentN->y + py,CurentN->p };
    // add the adjacent cell grid is ouside the origional grid its translated to fit onto the adjacent node
    // but only if there is a path to it
    if (addingN[1] == -1) {
        addingN[1] = 8;
        addingN[2] -= count;
        if (!nodes[CurentN->p].sidesIsPath[1]) canAdd = false;
    }
    if (addingN[1] == 9) {
        addingN[1] = 0;
        addingN[2] += count;
        if (!nodes[CurentN->p].sidesIsPath[0]) canAdd = false;
    }
    if (addingN[0] == -1) {
        addingN[0] = 8;
        addingN[2] -= 1;
        if (!nodes[CurentN->p].sidesIsPath[2]) canAdd = false;
    }
    if (addingN[0] == 9) {
        addingN[0] = 0;
        addingN[2] += 1;
        if (!nodes[CurentN->p].sidesIsPath[3]) canAdd = false;
    }
    // the cell grid is within the map and valid
    if (canAdd && addingN[2] >= 0 && addingN[2] < nodes.size() &&
        !nodes[addingN[2]].grid[addingN[0]][addingN[1]].visited) {

        // sets the parent of the added cellgrid to the current one
        nodes[addingN[2]].grid[addingN[0]][addingN[1]].parent = CurentN;

        // increments the distance traveled by one
        nodes[addingN[2]].grid[addingN[0]][addingN[1]].Distance =
            CurentN->Distance + (nodes[addingN[2]].grid[addingN[0]][addingN[1]].obstruction - CurentN->obstruction) + 1;
                                // adds the difference in height levels to the distance traveled
                       
        // so long as its not allready inside nodesToSearch it will be added to it
        std::vector<CellGrid*>::iterator it2 = std::find((*nodesToSearch).begin(), 
            (*nodesToSearch).end(), &nodes[addingN[2]].grid[addingN[0]][addingN[1]]);
        if (it2 == (*nodesToSearch).end())
        {
            nodesToSearch->push_back(&nodes[addingN[2]].grid[addingN[0]][addingN[1]]);
        }
    }

}

bool order(CellGrid* a, CellGrid* b) {
    // orders the nodesToSearch by the distance traveled plus holistic
    return a->Distance+a->Holistic > b->Distance + b->Holistic;
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
        grid[i % 9][i / 9].Holistic = abs(start->pos.x - grid[i % 9][i / 9].pos.x) 
            + abs(start->pos.y - grid[i % 9][i / 9].pos.y);
        grid[i % 9][i / 9].Distance = INFINITY;
    }
}


EnCamp::EnCamp(vec2 pos, float size) {

    // Define the random number generator and distribution
    std::random_device rd;  // Seed generator
    std::mt19937 gen(rd()); // Mersenne Twister engine
    std::uniform_real_distribution<float> Rand(0,1); // Range [0, 1]

    Pos = pos;
    Size = size * 0.03f + 0.05f;

    int numOfC = int(size)+1;
    for (int i = 0; i < numOfC; i++)
    {
        float ax = Rand(gen);
        float ay = Rand(gen);
        cages.push_back(new Landmark(vec2(ax,ay) -0.5f,0.1f));

    }
    Landmark().set(&cages, -0.5,0.5);


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
                if ((*(*marks)[i]).Pos.x + (*(*marks)[i]).Size > max) (*(*marks)[i]).Pos.x = max - (*(*marks)[i]).Size;
                if ((*(*marks)[i]).Pos.x - (*(*marks)[i]).Size < min) (*(*marks)[i]).Pos.x = min + (*(*marks)[i]).Size;
                if ((*(*marks)[i]).Pos.y - (*(*marks)[i]).Size < min) (*(*marks)[i]).Pos.y = min + (*(*marks)[i]).Size;
                if ((*(*marks)[i]).Pos.y + (*(*marks)[i]).Size > max) (*(*marks)[i]).Pos.y = max - (*(*marks)[i]).Size;
                if (j != i)
                {
                    if (glm::distance((*(*marks)[i]).Pos, (*(*marks)[j]).Pos) < (*(*marks)[i]).Size + (*(*marks)[j]).Size)
                    {
                        vec2 mv = (*(*marks)[i]).Pos - (*(*marks)[j]).Pos;
                        vec2 mv2 = (*(*marks)[j]).Pos;
                        vec2 mv3 = (*(*marks)[i]).Pos;
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


vec2 rott(vec2 v, float r)
{
    // rotates a vector v by angle r in radians
    return vec2(v.x * cos(r) + v.y * sin(r), -v.x * sin(r) + v.y * cos(r));
}

Furniture::Furniture(int type, int orientation, CellGrid(*grid)[9][9], float size, float thk, vec3 ps) {

    vec3 pos;
    vec3 sca;
    vec3 rot;
    int height = 20;
    ColorTexture = 2;
    NormalTexture = 12;

    vec2 furnPos;
    std::vector<vec2> Sectors;

    // gets the Furniture data based off its type
    switch (type)
    {
    case 0:
        //table
        furnPos = vec2(-1, -1);
        sca = vec3(0.25f, 0.08f, 0.25f);
        rot = vec3(0.0f);
        ColorTexture = 15;
        height = 20;

        Sectors.push_back(vec2(-4, -4));
        Sectors.push_back(vec2(-3, -4));
        Sectors.push_back(vec2(-2, -4));

        Sectors.push_back(vec2(-4, -3));
        Sectors.push_back(vec2(-3, -3));
        Sectors.push_back(vec2(-2, -3));

        Sectors.push_back(vec2(-4, -2));
        Sectors.push_back(vec2(-3, -2));
        Sectors.push_back(vec2(-2, -2));
        break;
    case 1:
        //pillar

        furnPos = vec2(0, 0);
        sca = vec3(0.2f, 0.5f, 0.2f);
        rot = vec3(0.0f, 3.1415f / 4.0f, 0.0f);
        ColorTexture = 26;
        height = 100;

        Sectors.push_back(vec2(0, 0));
        Sectors.push_back(vec2(-1, 0));
        Sectors.push_back(vec2(0, -1));
        Sectors.push_back(vec2(1, 0));
        Sectors.push_back(vec2(0, 1));
        break;
    case 2:
        //turned bookshelf

        furnPos = vec2(-1, -1);
        sca = vec3(0.4f, 0.4f, 0.075f);
        rot = vec3(0.0f, -3.1415f * 3 / 4.0f, 0.0f);
        ColorTexture = 25;
        height = 100;

        Sectors.push_back(vec2(-4, -2));
        Sectors.push_back(vec2(-3, -2));
        Sectors.push_back(vec2(-3, -3));
        Sectors.push_back(vec2(-2, -3));
        Sectors.push_back(vec2(-2, -4));
        break;
    case 3:
        //straight bookshelf

        furnPos = vec2(-1, 0);
        sca = vec3(0.7f, 0.5f, 0.075f);
        rot = vec3(0.0f, -3.1415f / 2.0f, 0.0f);
        ColorTexture = 25;
        height = 100;

        Sectors.push_back(vec2(-4, -3));
        Sectors.push_back(vec2(-4, -2));
        Sectors.push_back(vec2(-4, -1));
        Sectors.push_back(vec2(-4, 0));
        Sectors.push_back(vec2(-4, 1));
        Sectors.push_back(vec2(-4, 2));
        Sectors.push_back(vec2(-4, 3));
        break;
    case 4:
        //tourch

        furnPos = vec2(-0.7f, 0);
        sca = vec3(0.07f, 0.2f, 0.07f);
        rot = vec3(0.0f);
        ColorTexture = 23;
        height = 20;

        Sectors.push_back(vec2(-3, 0));

        break;
    case 5:
        //chair

        furnPos = vec2(-1, 0);
        sca = vec3(0.11f, 0.15f, 0.11f);
        rot = vec3(0.0f, -3.1415f / 2.0f, 0.0f);
        ColorTexture = 24;
        height = 20;

        Sectors.push_back(vec2(-4, 0));
        break;
    case -2:
        //chest

        furnPos = vec2(0, 0);
        sca = vec3(0.13f, 0.07f, 0.07f);
        rot = vec3(0.0f);
        ColorTexture = 13;
        height = 20;

        Sectors.push_back(vec2(0, 0));
        break;
    case -3:
        //cage

        furnPos = vec2(0, 0);
        sca = vec3(0.12f, 0.15f, 0.12f);
        rot = vec3(0.0f, 3.1415f / 4.0f, 0.0f);
        ColorTexture = 11;
        height = 20;

        Sectors.push_back(vec2(0, 0));
        break;
    case 7:
        //big bed

        furnPos = vec2(-1, -1);
        sca = vec3(0.18f, 0.09f, 0.4f);
        rot = vec3(0.0f, 3.1415f, 0.0f);
        ColorTexture = 16;
        height = 20;

        Sectors.push_back(vec2(-4, -4));
        Sectors.push_back(vec2(-3, -4));
        Sectors.push_back(vec2(-4, -3));
        Sectors.push_back(vec2(-3, -3));
        Sectors.push_back(vec2(-4, -2));
        Sectors.push_back(vec2(-3, -2));
        Sectors.push_back(vec2(-4, -1));
        Sectors.push_back(vec2(-3, -1));


        break;
    case 8:
        //small bed

        furnPos = vec2(-1, 0);
        sca = vec3(0.12f, 0.07f, 0.25f);
        rot = vec3(0.0f, -3.1415f / 2.0f, 0.0f);
        ColorTexture = 16;
        height = 20;

        Sectors.push_back(vec2(-4, 0));
        Sectors.push_back(vec2(-3, 0));
        Sectors.push_back(vec2(-2, 0));
        break;
    default:

        break;
    }

    for (int j = 0; j < Sectors.size(); j++)
    {
        // rotates the sector
        Sectors[j] = vec2(Sectors[j].x * cos((3.1415f / 2.0f) * orientation) + Sectors[j].y * sin((3.1415f / 2.0f) * orientation),
            -Sectors[j].x * sin((3.1415f / 2.0f) * orientation) + Sectors[j].y * cos((3.1415f / 2.0f) * orientation));

        // fills the space
        (*grid)[(int)round(Sectors[j].x + 4)][(int)round(Sectors[j].y + 4)].obstruction = height;
        (*grid)[(int)round(Sectors[j].x + 4)][(int)round(Sectors[j].y + 4)].hight = sca.y + 0.01;
    }

    // rotates the object
    furnPos = vec2(furnPos.x * cos((3.1415f / 2.0f) * orientation) + furnPos.y * sin((3.1415f / 2.0f) * orientation),
        -furnPos.x * sin((3.1415f / 2.0f) * orientation) + furnPos.y * cos((3.1415f / 2.0f) * orientation));

    rot.y += (3.1415f / 2.0f) * orientation;
    // scales the object
    sca *= size * thk;
    // the distance that must be shifted to be within the cell
    vec2 width = vec2(
        max(abs(rott(vec2(-1, 1) * vec2(sca.x, sca.z), rot.y).x), abs(rott(vec2(-1, -1) * vec2(sca.x, sca.z), rot.y).x)),
        max(abs(rott(vec2(-1, -1) * vec2(sca.x, sca.z), rot.y).y), abs(rott(vec2(1, -1) * vec2(sca.x, sca.z), rot.y).y))) + vec2(size * 0.02f, size * 0.02f);
    furnPos = (vec2(size * thk) - width) * furnPos;
    pos = vec3(furnPos.x, sca.y, furnPos.y) + ps;

    // initialises the object that will be dislpayed on screen
    obj = SpObj(pos, rot, sca, initCubeBuffer(), ColorTexture, NormalTexture);
    obj.textOff2 = glm::vec4(0.0f, 0.0f, 5.0f, 5.0f);
}

bool Furniture::canFit(int type, int orientation, CellGrid(*grid)[9][9]) {

    std::vector<vec2> Sectors;
    switch (type)
    {
    case 0:
        //table
        Sectors.push_back(vec2(-4, -4));
        Sectors.push_back(vec2(-3, -4));
        Sectors.push_back(vec2(-2, -4));

        Sectors.push_back(vec2(-4, -3));
        Sectors.push_back(vec2(-3, -3));
        Sectors.push_back(vec2(-2, -3));

        Sectors.push_back(vec2(-4, -2));
        Sectors.push_back(vec2(-3, -2));
        Sectors.push_back(vec2(-2, -2));
        break;
    case 1:
        //pillar
        Sectors.push_back(vec2(0, 0));
        Sectors.push_back(vec2(-1, 0));
        Sectors.push_back(vec2(0, -1));
        Sectors.push_back(vec2(1, 0));
        Sectors.push_back(vec2(0, 1));
        break;
    case 2:
        //turned bookshelf
        Sectors.push_back(vec2(-4, -2));
        Sectors.push_back(vec2(-3, -2));
        Sectors.push_back(vec2(-3, -3));
        Sectors.push_back(vec2(-2, -3));
        Sectors.push_back(vec2(-2, -4));
        break;
    case 3:
        //straight bookshelf
        Sectors.push_back(vec2(-4, -3));
        Sectors.push_back(vec2(-4, -2));
        Sectors.push_back(vec2(-4, -1));
        Sectors.push_back(vec2(-4, 0));
        Sectors.push_back(vec2(-4, 1));
        Sectors.push_back(vec2(-4, 2));
        Sectors.push_back(vec2(-4, 3));
        break;
    case 4:
        //tourch

        Sectors.push_back(vec2(-3, 0));

        break;
    case 5:
        //chair

        Sectors.push_back(vec2(-4, 0));
        break;
    case -2:
        //chest

        Sectors.push_back(vec2(0, 0));
        break;
    case -3:
        //cage

        Sectors.push_back(vec2(0, 0));
        break;
    case 7:
        //big bed

        Sectors.push_back(vec2(-4, -4));
        Sectors.push_back(vec2(-3, -4));
        Sectors.push_back(vec2(-4, -3));
        Sectors.push_back(vec2(-3, -3));
        Sectors.push_back(vec2(-4, -2));
        Sectors.push_back(vec2(-3, -2));
        Sectors.push_back(vec2(-4, -1));
        Sectors.push_back(vec2(-3, -1));

        break;
    case 8:
        //small bed
        Sectors.push_back(vec2(-4, 0));
        Sectors.push_back(vec2(-3, 0));
        Sectors.push_back(vec2(-2, 0));
        break;
    default:
        return false;

        break;
    }
    bool fit = true;
    for (int j = 0; j < Sectors.size(); j++)
    {
        // rotates the sector
        Sectors[j] = vec2(Sectors[j].x * cos((3.1415f / 2.0f) * orientation) + Sectors[j].y * sin((3.1415f / 2.0f) * orientation),
            -Sectors[j].x * sin((3.1415f / 2.0f) * orientation) + Sectors[j].y * cos((3.1415f / 2.0f) * orientation));

        if ((*grid)[(int)round(Sectors[j].x + 4)][(int)round(Sectors[j].y + 4)].obstruction >= 1)
        {
            // the space is taken
            fit = false;
            break;
        }
    }
    return fit;
}
Furniture::Furniture() {
}