#include "header/initMaze.h"
Maze::Maze(){
    size = 1.2f;
    thk = 0.4f;
    count = 20;
    for (int i = 0; i < count * count; i++)
    {
        nodes.push_back(*new Cell(i, count,thk));
        
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
    nodes[0].conect();
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
    
}
std::vector<glm::vec2> Maze::makeLines(int tp,int  sd,int  p0,int  p1,int  p2,int  p3,bool  swch,float thin){
      std::vector<glm::vec2> lst;
      
        if (nodes[0].sides.end() == std::find(nodes[0].sides.begin(),nodes[0].sides.end(),tp)){
          
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
            
  
            
            if (nodes[i].sides.end() == std::find(nodes[i].sides.begin(),nodes[i].sides.end(),tp) && nodes[i].sides.end() == std::find(nodes[i].sides.begin(),nodes[i].sides.end(),sd))
            {
              lst.push_back(nodes[i].pnt[thin+p1]);
  
                if (i != nodes.size() - 1 && nodes[ii].sides.end() == std::find(nodes[ii].sides.begin(),nodes[ii].sides.end(),tp)){
                     lst.push_back(nodes[ii].pnt[thin+p0]);
                     }
            }
            if (nodes[i].sides.end() == std::find(nodes[i].sides.begin(),nodes[i].sides.end(),tp) && nodes[i].sides.end() != std::find(nodes[i].sides.begin(),nodes[i].sides.end(),sd))
            {
                if (nodes[ii].sides.end() != std::find(nodes[ii].sides.begin(),nodes[ii].sides.end(),tp))
                {
                  lst.push_back(nodes[ii].pnt[thin+p0]);
                    if (p2 != -1 && nodes[iii].sides.end() != std::find(nodes[iii].sides.begin(),nodes[iii].sides.end(),sd))
                    {
                     
                      lst.push_back(nodes[ii].pnt[thin+p0]);
                    
                      lst.push_back(nodes[iiii].pnt[thin+p2]);
                      
                      
  
                    }
                }
  
            }
            if (nodes[i].sides.end() != std::find(nodes[i].sides.begin(),nodes[i].sides.end(),tp) && nodes[i].sides.end() == std::find(nodes[i].sides.begin(),nodes[i].sides.end(),sd))
            {
             
              if (i != nodes.size() - 1 && nodes[ii].sides.end() == std::find(nodes[ii].sides.begin(),nodes[ii].sides.end(),tp)){
                 lst.push_back(nodes[ii].pnt[thin+p0]);
                 }
            }
            if (nodes[i].sides.end() != std::find(nodes[i].sides.begin(),nodes[i].sides.end(),tp) && nodes[i].sides.end() != std::find(nodes[i].sides.begin(),nodes[i].sides.end(),sd))
            {
                if (nodes[ii].sides.end() == std::find(nodes[ii].sides.begin(),nodes[ii].sides.end(),tp))
                {
                    if (p2 != -1 && nodes[iii].sides.end() != std::find(nodes[iii].sides.begin(),nodes[iii].sides.end(),sd))
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
        std::vector<Cell*>::iterator it = std::find(Conectablednodes.begin(),Conectablednodes.end(),sender);
         if (it != Conectablednodes.end())
        {
          Conectednodes.push_back(sender);
          Conectablednodes.erase(it);
        }
      conected = true;
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
            else if(std::rand() % 20 == 0|| asd)
            {
              Conectednodes.push_back(Conectablednodes[rnd]);
              (*Conectablednodes[rnd]).Conectednodes.push_back(this);
  
            }
            
          Conectablednodes.erase(Conectablednodes.begin() + rnd);
        
        } 
}
void Cell::set(){
    for (int i = 0; i < Conectednodes.size(); i++) { 
        
        if((*Conectednodes[i]).index == index + 1) sides.push_back(3);
        if((*Conectednodes[i]).index == index - 1) sides.push_back(2);
        if((*Conectednodes[i]).index == index + count) sides.push_back(0);
        if((*Conectednodes[i]).index == index - count) sides.push_back(1);
        
      }
}
void Cell::conect(){
      conected = true;
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
            else if(std::rand() % 20 == 0|| asd)
            {
              Conectednodes.push_back(Conectablednodes[rnd]);
              (*Conectablednodes[rnd]).Conectednodes.push_back(this);
  
            }
            
         Conectablednodes.erase(Conectablednodes.begin() + rnd);
        
        } 
}