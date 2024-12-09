#include "initMaze.h"
Maze::Maze(){
    size = 2.0f;
    thk = 0.4f;
    count = 20;
    for (int i = 0; i < count * count; i++)
    {
        nodes.push_back(*new Cell(i, count,thk));
        
        
    }
    for (int i = 0; i < count * count; i++)
    {
        
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

    nodes[0].conect(nullptr);
    for (int i = 0; i < nodes.size(); i++) nodes[i].set();

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

    lst1.push_back(nodes[0].pnt[0]);
    lst1.push_back(nodes[nodes.size()-1].pnt[3]);
    
    buffers = initBuffers(lst1);
    obj = SpObj(glm::vec3(0), glm::vec3(0), glm::vec3(size),buffers,0,5);
    obj.textOff = glm::vec4(0.0f,0.0f, size * 2.2f, size * 2.2f);
}
std::vector<glm::vec2> Maze::makeLines(int tp,int  sd,int  p0,int  p1,int  p2,int  p3,bool  swch,float thin){
      std::vector<glm::vec2> lst;
      
        if (!nodes[0].sidesi[tp]){
          
          lst.push_back(nodes[0].pnt[thin+p0]);
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
              lst.push_back(nodes[i].pnt[thin+p1]);
  
                if (i != nodes.size() - 1 && !nodes[ii].sidesi[tp]){
                     lst.push_back(nodes[ii].pnt[thin+p0]);
                     }
            }
            if (!nodes[i].sidesi[tp] && nodes[i].sidesi[sd])
            {
                if (nodes[ii].sidesi[tp])
                {
                  lst.push_back(nodes[ii].pnt[thin+p0]);
                    if (p2 != -1 && nodes[iii].sidesi[sd])
                    {
                     
                      lst.push_back(nodes[ii].pnt[thin+p0]);
                    
                      lst.push_back(nodes[iiii].pnt[thin+p2]);
                      
                      
  
                    }
                }
  
            }
            if (nodes[i].sidesi[tp] && !nodes[i].sidesi[sd])
            {
             
              if (i != nodes.size() - 1 && !nodes[ii].sidesi[tp]){
                 lst.push_back(nodes[ii].pnt[thin+p0]);
                 }
            }
            if (nodes[i].sidesi[tp] && nodes[i].sidesi[sd])
            {
                if (!nodes[ii].sidesi[tp])
                {
                    if (p2 != -1 && nodes[iii].sidesi[sd])
                    {
                      lst.push_back(nodes[iii].pnt[thin+p3]);
  
                      lst.push_back(nodes[i].pnt[thin+p1]);
                    }
  
                    lst.push_back(nodes[i].pnt[thin+p1]);
  
  
                }
            }
                
        }
        return lst;
    }
void Maze::drawMap(int programInfo) {

}
bool Maze::collide(glm::vec3* pos, glm::vec3* acc, glm::vec2 leway) {
    glm::vec3 temp;
    if (acc == nullptr) acc = new glm::vec3(0);
    bool grnd = false;
    leway.x /= -size;
    (*pos) /= size;

    if ((int)((*pos).x +0.5f) + (int)((*pos).z +0.5f) * count >= 0)
    {
        int nd = (int)((*pos).x +0.5f) + (int)((*pos).z +0.5f) * count;
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


    (*pos) *= size;
    leway.x *= -size;


    if ((*pos).y <= leway.y) {
        (*pos).y = leway.y;
        if ( (*acc).y < 0.0f)  (*acc).y *= -1.0f;

        grnd = true;
    }
    if ((*pos).y >= 0.5f - leway.y && (*acc).y > 0.0f) (*acc).y *= -1.0f;

    return grnd;
}



Cell::Cell(int index, int count, float thk){

      this->index = index;
      this->count = count;
      this->x = index%count;
      this->y = glm::floor(index/count);
      this->conected = false;
      
  
  
      float posx = x;
      float posy = y;
      pnt = {
       glm::vec2(posx - thk, posy - thk),
        glm::vec2(posx + thk, posy - thk),
        glm::vec2(posx - thk, posy + thk),
        glm::vec2(posx + thk, posy + thk),
        glm::vec2(posx,posy),
        glm::vec2(posx - 0.5, posy - 0.5),
        glm::vec2(posx + 0.5, posy - 0.5),
        glm::vec2(posx - 0.5, posy + 0.5),
        glm::vec2(posx + 0.5, posy + 0.5),
        glm::vec2(posx,posy)
      };

     
}
void Cell::conect(Cell* sender){


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
        
           int rnd = std::rand() % Conectablednodes.size();


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
            //else if(asd)
            else if(asd || rand() % 10 == 1)
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
