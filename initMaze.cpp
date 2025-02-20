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

    size = 2.0f;
    thk = 0.45f;
    count = c;


    Camps.clear();
    bool isfixed = false;
    int iisfixed = 0;
    for (int i = 0; i < 5; i++)
    {
        Camps.push_back(new EnCamp(vec2(Rand(gen), Rand(gen)), i));

    }
    for (int i = 0; i < count; i++)
    {
        Camps.push_back(new LootSpot(vec2(Rand(gen), Rand(gen))));

    }
    float tyu = 0.05f;

    do
    {
        isfixed = true;
        for (int i = 0; i < Camps.size(); i++)
        {
            for (int j = 0; j < Camps.size(); j++)
            {
                if ((*Camps[i]).Pos.x + (*Camps[i]).Size > 1) (*Camps[i]).Pos.x = 1 - (*Camps[i]).Size;
                if ((*Camps[i]).Pos.x - (*Camps[i]).Size < 0) (*Camps[i]).Pos.x = 0 + (*Camps[i]).Size;
                if ((*Camps[i]).Pos.y - (*Camps[i]).Size < 0) (*Camps[i]).Pos.y = 0 + (*Camps[i]).Size;
                if ((*Camps[i]).Pos.y + (*Camps[i]).Size > 1) (*Camps[i]).Pos.y = 1 - (*Camps[i]).Size;
                if (j != i)
                {
                    if (glm::distance((*Camps[i]).Pos , (*Camps[j]).Pos) < (*Camps[i]).Size + (*Camps[j]).Size + tyu)
                    {
                        vec2 mv = (*Camps[i]).Pos - (*Camps[j]).Pos;
                        mv = glm::normalize(mv) * (glm::distance((*Camps[i]).Pos , (*Camps[j]).Pos) - ((*Camps[i]).Size + (*Camps[j]).Size + tyu * 1.1f));


                        (*Camps[j]).Pos += mv * 0.5f;
                        (*Camps[i]).Pos -= mv * 0.5f;
                        isfixed = false;
                    }
                }

            }

        }
        iisfixed++;
    } while (!isfixed);




    for (int i = 0; i < count * count; i++)
    {
        nodes.push_back(*new Cell(i, count,thk));
       
        
    }
    for (int i = 0; i < count * count; i++)
    {

        nodes[i].prob = 0.1f;
        for(Landmark* landm : Camps)
        {
            int posX = (int)((*landm).Pos.x * count);
            int posY = (int)((*landm).Pos.y * count);
            if (dynamic_cast<EnCamp*>(landm) != nullptr)
            {
                EnCamp* Camp = dynamic_cast<EnCamp*>(landm);

               float sze = (*Camp).Size * count;
              
               if (glm::distance(vec2(posX, posY), vec2(nodes[i].x, nodes[i].y)) < sze)
               {
                  
                   nodes[i].prob = 0.7f;
               }
            }
            else if (dynamic_cast<LootSpot*>(landm) != nullptr)
            {
                LootSpot* Camp = dynamic_cast<LootSpot*>(landm);
                if (nodes[i].index == posX + posY * count) {
                    nodes[i].treasure = true;
                }
            }
            (*landm).Pos.x = posX / float(count);
            (*landm).Pos.y = posY /float(count);
        }

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

    std::vector<Cell*> cellStack;
    cellStack.push_back(&nodes[0]);
    Cell* curC = cellStack.back();
    
    do{
        
        (*curC).conected = true;
        
        if ((*curC).Conectablednodes.size() > 0)
        {

            int rnd = floor(Rand(gen) * (*curC).Conectablednodes.size());


            bool asd = false;
            if (!(*(*curC).Conectablednodes[rnd]).conected || asd || Rand(gen) <= (*curC).prob)
            {
                cellStack.push_back((*curC).Conectablednodes[rnd]);
                (*curC).Conectednodes.push_back((*curC).Conectablednodes[rnd]);
                (*(*curC).Conectablednodes[rnd]).Conectednodes.push_back(curC);



                

            }


            std::vector<Cell*>::iterator it = std::find((*(*curC).Conectablednodes[rnd]).Conectablednodes.begin(), (*(*curC).Conectablednodes[rnd]).Conectablednodes.end(), curC);
            if (it != (*(*curC).Conectablednodes[rnd]).Conectablednodes.end())
            {
                (*(*curC).Conectablednodes[rnd]).Conectablednodes.erase(it);
            }

            it = std::find((*curC).Conectablednodes.begin(), (*curC).Conectablednodes.end(), (*curC).Conectablednodes[rnd]);
            if (it != (*curC).Conectablednodes.end())
            {
                (*curC).Conectablednodes.erase(it);
            }

        }
        else
        {
            cellStack.pop_back();

        }
        if(!cellStack.empty())curC = cellStack.back();
    } while (!cellStack.empty());
    





    std::vector<glm::mat4> mds;
    std::vector<float> txt;
    vec3 ps;
    for (int i = 0; i < nodes.size(); i++) {
        nodes[i].set();
        ps = vec3((i % count) * size, 0.001, (i / count) * size);
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
        
        if (nodes[i].sidesi[0]) grid[4][8] = 1;
        if (nodes[i].sidesi[1]) grid[4][0] = 1;
        if (nodes[i].sidesi[2]) grid[0][4] = 1;
        if (nodes[i].sidesi[3]) grid[8][4] = 1;

        if (nodes[i].treasure)
        {
            int furrot = floor(Rand(gen) * 4);
            Furniture::canFit(-2, furrot, &grid);
            Furniture ref = Furniture(-2, furrot, &grid, size, thk, ps);
            mds.push_back(Matrix(ref.pos, ref.sca, ref.rot));
            nodes[i].fur.push_back(ref);
            txt.push_back(ref.type1); 
            txt.push_back(ref.type2); 
        }
        for (int j = 0; j < 6; j++) 
        {
            int furt = floor(Rand(gen) * 8);
            int furrot = floor(Rand(gen) * 4);
            
            if(Furniture::canFit(furt, furrot,&grid))
            {
                Furniture ref = Furniture(furt, furrot, &grid, size, thk, ps);
                mds.push_back(Matrix(ref.pos, ref.sca, ref.rot));
                if (furt == 5)
                {
                    ref.sca.y *= 0.5f;
                    ref.pos.y = ref.sca.y;
                }
                nodes[i].fur.push_back(ref);
                txt.push_back(ref.type1);
                txt.push_back(ref.type2);
               if(furt == 4) 
               {
                   Light l = Light(vec3(ref.pos.x, ref.pos.y + ref.sca.y, ref.pos.z));
                   l.perch = vec2(i, nodes[i].fur.size()-1);
                   (*ligh).push_back(l);
               }
               
               
            }
        }
        for (int j = 0; j < 9*9; j++)
        {
            nodes[i].grid[j % 9][j / 9] = CellGrid(nodes[i].pnt(0, 0) + vec2((j % 9) - 4,(j / 9) - 4) * vec2(thk * 0.222f), j % 9, j / 9, i);
            if (grid[j % 9][j / 9] >= 2)nodes[i].grid[j % 9][j / 9].obstruction = true;
        }

    }
    furn = InsObj(initCubeBuffer({0,1,2,3,4,5, 6,7,8 }), 23, 12, initI(mds),initB(txt), mds.size());


    furn.textOff2 = glm::vec4(0.0f, 0.0f, 5.0f, 5.0f);
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

    buffers = initBuffers(lst1);
    obj = SpObj(glm::vec3(0), glm::vec3(0), glm::vec3(size),buffers,0,5);
    obj.textOff = glm::vec4(0.0f,0.0f, size * 2.2f, size * 2.2f);
    std::vector<glm::vec2> Mlst1;

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

    MapBuffers = initBuffers2(lst1);

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
      
        if (!nodes[0].sidesi[tp]){
          
          lst.push_back(nodes[0].pnt(thin+p0, thk));
        } 
        for (int k = 0; k < nodes.size(); k++)
        {
            int i = 0;
            int ii = 0;
            int iii = 0;
            int iiii = 0;
            if (swch)
            {
  
                i = ((k % count) * count) + std::floor(k / count);
                ii = (((k + 1) % count) * count) + std::floor((k + 1) / count);
                iii = ((k % count) * count) + std::floor(k / count) - 1;
                iiii = ((k % count) * count) + std::floor(k / count) - 1 + count;
            }
            else
            {
                i = k;
                ii = k + 1;
                iii = k - count;
                iiii = k - count + 1;
            }
            
  
            
            if (!nodes[i].sidesi[tp] && !nodes[i].sidesi[sd])
            {
              lst.push_back(nodes[i].pnt(thin+p1, thk));
  
                if (i != nodes.size() - 1 && !nodes[ii].sidesi[tp]){
                     lst.push_back(nodes[ii].pnt(thin+p0, thk));
                     }
            }
            if (!nodes[i].sidesi[tp] && nodes[i].sidesi[sd])
            {
                if (nodes[ii].sidesi[tp])
                {
                  lst.push_back(nodes[ii].pnt(thin+p0, thk));
                    if (p2 != -1 && nodes[iii].sidesi[sd])
                    {
                     
                      lst.push_back(nodes[ii].pnt(thin+p0, thk));
                    
                      lst.push_back(nodes[iiii].pnt(thin+p2, thk));
                      
                      
  
                    }
                }
  
            }
            if (nodes[i].sidesi[tp] && !nodes[i].sidesi[sd])
            {
             
              if (i != nodes.size() - 1 && !nodes[ii].sidesi[tp]){
                 lst.push_back(nodes[ii].pnt(thin+p0, thk));
                 }
            }
            if (nodes[i].sidesi[tp] && nodes[i].sidesi[sd])
            {
                if (!nodes[ii].sidesi[tp])
                {
                    if (p2 != -1 && nodes[iii].sidesi[sd])
                    {
                      lst.push_back(nodes[iii].pnt(thin+p3, thk));
  
                      lst.push_back(nodes[i].pnt(thin+p1, thk));
                    }
  
                    lst.push_back(nodes[i].pnt(thin+p1, thk));
  
  
                }
            }
                
        }
        return lst;
    }
void Maze::drawMap(int programInfo) {

}


float Maze::project(float x, float y, int side, Furniture f) {
    if (side == 0) return x * cos(f.rot.y) - y * sin(f.rot.y);
    else  return x * sin(f.rot.y) + y * cos(f.rot.y);
}

bool Maze::collide(glm::vec3* pos, glm::vec3* acc, glm::vec2 leway) {
    glm::vec3 temp;
    if (acc == nullptr) acc = new glm::vec3(0);
    bool grnd = false;
    leway.x /= -size;
    (*pos) /= size;

    int nd = (int)((*pos).x + 0.5f) + (int)((*pos).z + 0.5f) * count;
    if (nd >= 0 && nd< count* count)
    {
        //std::cout << nd << std::endl;
        if (!nodes[nd].sidesi[0] && (int)((*pos).z + 0.5f) - (*pos).z < -(thk + leway.x)) {
            // console.log((int)((*pos).x+0.5) -(*pos).x , (int)((*pos).x+0.5) - (*pos).x);
            (*pos).z += (thk + leway.x) + (int)((*pos).z +0.5f) - (*pos).z;
            (*acc).z *= -1;
            //std::cout << "00" << std::endl;
        }
        if (!nodes[nd].sidesi[1] && (int)((*pos).z +0.5f) - (*pos).z > (thk + leway.x)) {
            // console.log((int)((*pos).x+0.5) -(*pos).x , (int)((*pos).x+0.5) - (*pos).x);
            (*pos).z += (int)((*pos).z +0.5f) - (*pos).z - (thk + leway.x);
            (*acc).z *= -1;
            //std::cout << "01" << std::endl;
        }

        if (!nodes[nd].sidesi[2] && (int)((*pos).x +0.5f) - (*pos).x > (thk + leway.x)) {
            //console.log((int)((*pos).x+0.5) -(*pos).x , (int)((*pos).x+0.5) - (*pos).x);
            (*pos).x += (int)((*pos).x +0.5f) - (*pos).x - (thk + leway.x);
            (*acc).x *= -1;

            //std::cout << "02" << std::endl;
        }
        if (!nodes[nd].sidesi[3] && (int)((*pos).x +0.5f) - (*pos).x < -(thk + leway.x)) {
            // console.log((int)((*pos).x+0.5) -(*pos).x , (int)((*pos).x+0.5) - (*pos).x);
            (*pos).x += (int)((*pos).x +0.5f) - (*pos).x + (thk + leway.x);
            (*acc).x *= -1;
            //std::cout << "03" << std::endl;
        }

        if (nodes[nd].sidesi[1] && (int)((*pos).z +0.5f) - (*pos).z > (thk + leway.x) &&
            nodes[nd].sidesi[3] && (int)((*pos).x +0.5f) - (*pos).x < -(thk + leway.x) &&
            (
                !nodes[(int)((*pos).x +0.5f) + 1.0 + (int)((*pos).z +0.5f - 1.0f) * count].sidesi[0] ||
                !nodes[(int)((*pos).x +0.5f) + 1.0 + (int)((*pos).z +0.5f - 1.0f) * count].sidesi[2]
                )
            ) {
            if (abs((int)((*pos).z +0.5f) - (*pos).z - (thk + leway.x)) < abs((int)((*pos).x +0.5f) - (*pos).x + (thk + leway.x)))
            {
                (*pos).z += (int)((*pos).z +0.5f) - (*pos).z - (thk + leway.x); (*acc).z *= -1;
            }
            else { (*pos).x += (int)((*pos).x +0.5f) - (*pos).x + (thk + leway.x); (*acc).x *= -1; }

        }


        if (nodes[nd].sidesi[1] && (int)((*pos).z +0.5f) - (*pos).z > (thk + leway.x) &&
            nodes[nd].sidesi[2] && (int)((*pos).x +0.5f) - (*pos).x > (thk + leway.x) &&
            (
                !nodes[(int)((*pos).x +0.5f) - 1.0 + (int)((*pos).z +0.5f - 1.0f) * count].sidesi[0] ||
                !nodes[(int)((*pos).x +0.5f) - 1.0 + (int)((*pos).z +0.5f - 1.0f) * count].sidesi[3]
                )
            ) {

            if (abs((int)((*pos).z +0.5f) - (*pos).z - (thk + leway.x)) < abs((int)((*pos).x +0.5f) - (*pos).x - (thk + leway.x)))
            {
                (*pos).z += (int)((*pos).z +0.5f) - (*pos).z - (thk + leway.x); (*acc).z *= -1;
            }
            else { (*pos).x += (int)((*pos).x +0.5f) - (*pos).x - (thk + leway.x); (*acc).x *= -1; }

        }

        //////

        if (nodes[nd].sidesi[0] && (int)((*pos).z +0.5f) - (*pos).z < -(thk + leway.x) &&
            nodes[nd].sidesi[3] && (int)((*pos).x +0.5f) - (*pos).x < -(thk + leway.x) && (
                !nodes[(int)((*pos).x +0.5f) + 1.0 + (int)((*pos).z +0.5f + 1.0f) * count].sidesi[1] ||
                !nodes[(int)((*pos).x +0.5f) + 1.0 + (int)((*pos).z +0.5f + 1.0f) * count].sidesi[2]
                )
            ) {

            if (abs((thk + leway.x) + (int)((*pos).z +0.5f) - (*pos).z) < abs((int)((*pos).x +0.5f) - (*pos).x + (thk + leway.x)))
            {
                (*pos).z += (thk + leway.x) + (int)((*pos).z +0.5f) - (*pos).z; (*acc).z *= -1;
            }
            else { (*pos).x += (int)((*pos).x +0.5f) - (*pos).x + (thk + leway.x); (*acc).x *= -1; }

        }


        if (nodes[nd].sidesi[0] && (int)((*pos).z +0.5f) - (*pos).z<-(thk + leway.x) &&
            nodes[nd].sidesi[2] && (int)((*pos).x +0.5f) - (*pos).x>(thk + leway.x) && (
                !nodes[(int)((*pos).x +0.5f) - 1.0 + (int)((*pos).z +0.5f + 1.0f) * count].sidesi[1] ||
                !nodes[(int)((*pos).x +0.5f) - 1.0 + (int)((*pos).z +0.5f + 1.0f) * count].sidesi[3]
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
        (*pos).x = 0;
        (*pos).z = 0;
    }
    leway.x *= -size;

    (*pos) *= size;


    for (Furniture f : nodes[nd].fur) {
        
        if (pow((*pos).x - f.pos.x, 2.0) + pow((*pos).z - f.pos.z, 2.0) < max(f.sca.x, f.sca.z) * 2.0)
        {

            vec2 Points[] = {
                    vec2(-1, 1),
                    vec2(-1, -1),
                    vec2(1, -1),
                    vec2(1, 1),
            };

            for (int i = 0; i < 4; i++) {
                Points[i].x *= f.sca.x ;
                Points[i].y *= f.sca.z ;

                vec2 PP = vec2(Points[i].x * cos(f.rot.y) + Points[i].y * sin(f.rot.y), -Points[i].x * sin(f.rot.y) + Points[i].y * cos(f.rot.y));
                Points[i] = PP;

                Points[i].x += f.pos.x;
                Points[i].y += f.pos.z;

            }

            float max = -90000.0;
            float min = 90000.0;
            for (int i = 0; i < 4; i++) {
                float test = project(Points[i].x, Points[i].y, 0,f);
                if (test > max)max = test;
                if (test < min)min = test;
            }
            
            float playProj = project((*pos).x, (*pos).z, 0,f);
            bool assa = false;
            bool assa2 = false;
            float firstDist = 0.0;
            float secondDist = 0.0;
            if (playProj + leway.x > min && playProj - leway.x < max) {
                assa = true;
                if (playProj + leway.x - min > max - playProj - leway.x) firstDist = max - playProj + leway.x;
                else firstDist = min - playProj - leway.x;
            }

            max = -90000.0;
            min = 90000.0;
            for (int i = 0; i < 4; i++) {
                float test = project(Points[i].x, Points[i].y, 1,f);
                if (test > max)max = test;
                if (test < min)min = test;
            }
            playProj = project((*pos).x, (*pos).z, 1,f);
            if (playProj + leway.x > min && playProj - leway.x < max) {
                assa2 = true;
                if (playProj + leway.x - min > max - playProj - leway.x) secondDist = max - playProj + leway.x;
                else secondDist = min - playProj - leway.x;


            }
            
            if (assa2 && assa && (*pos).y - leway.y < f.pos.y + f.sca.y) {
                
                if (abs(firstDist) > abs(f.sca.y + f.sca.y + leway.y - (*pos).y) && abs(secondDist) > abs(f.sca.y + f.sca.y + leway.y - (*pos).y)) {
                    (*pos).y = f.pos.y + f.sca.y + leway.y-0.001f;
                    grnd = true;
                    if ((*acc).y < 0)  (*acc).y *= -1.0;
                }
                else {
                    if (abs(firstDist) > abs(secondDist))
                    {
                        // w = v - 2 * (v ∙ n) * n
                        vec3 acct = vec3((*acc).x - 2 * ((*acc).x * (sin(f.rot.y)) + (*acc).z * (cos(f.rot.y))) * (sin(f.rot.y)),
                            (*acc).y,
                            (*acc).z - 2 * ((*acc).x * (sin(f.rot.y)) + (*acc).z * (cos(f.rot.y))) * (cos(f.rot.y))
                        );
                       
                        (*acc).x = acct.x;
                        (*acc).y = acct.y;
                        (*acc).z = acct.z;
                        (*pos).x += sin(f.rot.y) * secondDist;
                        (*pos).z += cos(f.rot.y) * secondDist;
                    }
                    else
                    {
                        vec3 acct = vec3((*acc).x - 2 * ((*acc).x * (cos(f.rot.y)) + (*acc).z * -(sin(f.rot.y))) * (cos(f.rot.y)),
                            (*acc).y,
                            (*acc).z - 2 * ((*acc).x * (cos(f.rot.y)) + (*acc).z * -(sin(f.rot.y))) * -(sin(f.rot.y))
                        );
                        (*acc).x = acct.x;
                        (*acc).y = acct.y;
                        (*acc).z = acct.z;
                        (*pos).x += cos(f.rot.y) * firstDist;
                        (*pos).z += -sin(f.rot.y) * firstDist;
                    }
                }
            }
           
        }
    }
    

    if ((*pos).y < leway.y) {
        (*pos).y = leway.y - 0.001f;;
        if ( (*acc).y < 0.0f)  (*acc).y *= -1.0f;

        grnd = true;
    }
   // if ((*pos).y >= 0.5f - leway.y && (*acc).y > 0.0f) (*acc).y *= -1.0f;

    return grnd;
}

void Maze::draw(int programInfo) {
    
    obj.draw(programInfo);

}

std::vector<CellGrid*> Maze::getpath(int sx, int sy,int start, int ex, int ey, int end) {
    std::random_device rd;  // Seed generator
    std::mt19937 gen(rd()); // Mersenne Twister engine
    std::uniform_real_distribution<float> Rand(0.0f, 1.0f); // Range [0, 1]
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
        nodes[i].setSearch(&nodes[start].grid[sx][sy]);
    }
    bool found = false;
    int CurentN[3] = { ex,ey,end };
    int re = 0;
    std::vector<CellGrid*> nodesToSearch;
    while (!found)
    {
        re++;
        //std::cout << "x " << CurentN[0] << " y " << CurentN[1] << " p " << CurentN[2] << std::endl;
        if (CurentN[0] == sx && CurentN[1] == sy && CurentN[2] == start) {
            CellGrid* cur = &nodes[CurentN[2]].grid[CurentN[0]][CurentN[1]];
            while (cur != nullptr)
            {

                path.push_back(cur);
                cur = cur->parent;
            }

            found = true;
        }
        else
        {
            //nodes[CurentN].visit(&nodesToSearch);
            nodes[CurentN[2]].grid[CurentN[0]][CurentN[1]].visited = true;

            std::vector<CellGrid*>::iterator it = std::find(nodesToSearch.begin(), nodesToSearch.end(), &nodes[CurentN[2]].grid[CurentN[0]][CurentN[1]]);
            if (it != nodesToSearch.end())
            {
                nodesToSearch.erase(it);
            }
            CellGrid* thng;
            thng = doThing(CurentN,-1,0,&nodesToSearch);
            if (thng != nullptr) {
                if (!thng->obstruction || thng == &nodes[start].grid[4][4] || thng == &nodes[end].grid[4][4]) {
                    nodesToSearch.push_back(thng);
                }

            }
            thng = doThing(CurentN,1,0,&nodesToSearch);
            if (thng != nullptr) {
                if (!thng->obstruction || thng == &nodes[start].grid[4][4] || thng == &nodes[end].grid[4][4]) {
                    nodesToSearch.push_back(thng);
                }

            }
            thng = doThing(CurentN,0,1,&nodesToSearch);
            if (thng != nullptr) {
                if (!thng->obstruction || thng == &nodes[start].grid[4][4] || thng == &nodes[end].grid[4][4]) {
                    nodesToSearch.push_back(thng);
                }

            }
            thng = doThing(CurentN,0,-1,&nodesToSearch);
            if (thng != nullptr) {
                if (!thng->obstruction || thng == &nodes[start].grid[4][4] || thng == &nodes[end].grid[4][4]) {
                    nodesToSearch.push_back(thng);
                }

            }

            std::sort(nodesToSearch.begin(), nodesToSearch.end(), comp);
            if (nodesToSearch.size() == 0) {
                found = true;
                std::cout << "start " << start << " end " << end << " no path, count: " << re << std::endl;
            }
            else
            {

                CurentN[0] = nodesToSearch[0]->x;
                CurentN[1] = nodesToSearch[0]->y;
                CurentN[2] = nodesToSearch[0]->p;
            }
        }
    }

    return path;
}

CellGrid* Maze::doThing(int CurentN[3],int px,int py, std::vector<CellGrid*>* nodesToSearch) {
    bool canAdd = true;
    int addingN[3] = { CurentN[0] + px,CurentN[1] + py,CurentN[2] };
    if (addingN[1] == -1) {
        addingN[1] = 8;
        addingN[2] -= count;
        if (!nodes[CurentN[2]].sidesi[1]) canAdd = false;
    }
    if (addingN[1] == 9) {
        addingN[1] = 0;
        addingN[2] += count;
        if (!nodes[CurentN[2]].sidesi[0]) canAdd = false;
    }
    if (addingN[0] == -1) {
        addingN[0] = 8;
        addingN[2] -= 1;
        if (!nodes[CurentN[2]].sidesi[2]) canAdd = false;
    }
    if (addingN[0] == 9) {
        addingN[0] = 0;
        addingN[2] += 1;
        if (!nodes[CurentN[2]].sidesi[3]) canAdd = false;
    }

    if (canAdd && addingN[2] >= 0 && addingN[2] < nodes.size() && !nodes[addingN[2]].grid[addingN[0]][addingN[1]].visited) {
        nodes[addingN[2]].grid[addingN[0]][addingN[1]].parent = &nodes[CurentN[2]].grid[CurentN[0]][CurentN[1]];
        nodes[addingN[2]].grid[addingN[0]][addingN[1]].Distance = nodes[CurentN[2]].grid[CurentN[0]][CurentN[1]].Distance + 1;

        std::vector<CellGrid*>::iterator it2 = std::find((*nodesToSearch).begin(), (*nodesToSearch).end(), &nodes[addingN[2]].grid[addingN[0]][addingN[1]]);
        if (it2 == (*nodesToSearch).end())
        {
            return &nodes[addingN[2]].grid[addingN[0]][addingN[1]];
        }
    }
    return nullptr;
}

bool comp(CellGrid* a, CellGrid* b) {
    return a->Distance+a->Holistic < b->Distance + b->Holistic;
}


Cell::Cell(int index, int count, float thk){

      this->index = index;
      this->count = count;
      this->x = index%count;
      this->y = glm::floor(index/count);
      this->conected = false;
      
  
  
      float posx = x;
      float posy = y;

     
}
glm::vec2 Cell::pnt(int i, float thk) {
    
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

void Cell::conect(Cell* sender){
    std::random_device rd;  // Seed generator
    std::mt19937 gen(rd()); // Mersenne Twister engine
    std::uniform_real_distribution<float> Rand(0.0f, 1.0f); // Range [0, 1]


   // if (sender != nullptr)
    conected = true;
    {
        std::vector<Cell*>::iterator it = std::find(Conectablednodes.begin(), Conectablednodes.end(), sender);
        if (it != Conectablednodes.end())
        {
            Conectablednodes.erase(it);
            Conectednodes.push_back(sender);
        }
    }
      
        while (Conectablednodes.size() != 0) 
        {
        
           int rnd = floor(Rand(gen) * Conectablednodes.size());


           bool asd = false;
        //    for (var i = 0; i < cmp.length; i += 2)
        //    {
        //        var xx = cmp[i] % Cell::count;
        //        var yy = cmp[i] / Cell::count;
        //        var Distance = (Cell::x - xx) * (Cell::x - xx) + (Cell::y - yy) * (Cell::y - yy);
        //        if ( Distance< glm::pow(cmp[i + 1],2) ) 
        //         { asd = true;
        //  }
        //    }
  
            if (!(*Conectablednodes[rnd]).conected )
            {
              Conectednodes.push_back(Conectablednodes[rnd]);
              (*Conectablednodes[rnd]).conect(this);
            }
            else if(asd || Rand(gen) * 10 <= 1)
            {
              Conectednodes.push_back(Conectablednodes[rnd]);

              (*Conectablednodes[rnd]).Conectednodes.push_back(this);
  
            }
            
          Conectablednodes.erase(Conectablednodes.begin() + rnd);
        
        } 
}


void Cell::set(){
    for (int i = 0; i < Conectednodes.size(); i++) { 
        
        if ((*Conectednodes[i]).index == index + count) { sides.push_back(0); 
        sidesi[0] = true;
        }
        if((*Conectednodes[i]).index == index - count) {sides.push_back(1); 
        sidesi[1] = true;
        }
        if ((*Conectednodes[i]).index == index - 1){ sides.push_back(2); 
        sidesi[2] = true;
        }
        if ((*Conectednodes[i]).index == index + 1){ sides.push_back(3); 
        sidesi[3] = true;
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
        cages.push_back((vec2(Rand(gen), Rand(gen)) - vec2(0.5f)) * 0.5f);
    }
    bool isfixed = true;
    do
    {
        isfixed = true;
        for (int i = 0; i < cages.size(); i++)
        {
            for (int j = 0; j < cages.size(); j++)
            {
                if (glm::distance(cages[j], vec2(0)) > 0.5f) cages[j] = glm::normalize(cages[j]) * 0.5f;

                if (j != i)
                {
                    if (glm::distance(cages[i] , cages[j]) < 0.3f)
                    {
                        vec2 mv = cages[i] - cages[j];
                        mv = glm::normalize(mv) * (glm::distance(cages[i], cages[j]) - (0.4f));


                        cages[j] += mv * 0.5f;
                        cages[i] -= mv * 0.5f;
                        isfixed = false;
                    }
                }

            }

        }
    } while (!isfixed);


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