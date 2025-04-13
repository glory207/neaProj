#include "Furniture.h"
#include <random>
vec2 rott(vec2 v, float r)
{
    // rotates a vector v by angle r in radians
    return vec2(v.x * cos(r) + v.y * sin(r), -v.x * sin(r) + v.y * cos(r));
}

Furniture::Furniture(int type, int orientation, int(*grid)[9][9], float size, float thk, vec3 ps) {

    vec3 pos;
    vec3 sca;
    vec3 rot;

    ColorTexture = 2;
    NormalTexture = 12;

    vec2 furnPos;
    std::vector<vec2> Sectors;
    /*
    switch (i) {
    case 0:
        furnPos = vec2(-1, -1);
        sca = vec3(0.4f, 0.4f, 0.05f);
        rot = vec3(0.0f,-3.1415f*3 / 4.0f, 0.0f);
        ColorTexture = 5;
        Sectors.push_back(vec2(-1.0f, -1.0f));
        Sectors.push_back(vec2(-1, 0));
        Sectors.push_back(vec2(0, -1));
        break;
    case 1:
        furnPos = vec2(-1, -1);
        sca = vec3(0.25f, 0.08f, 0.25f);
        rot = vec3(0.0f);
        Sectors.push_back(vec2(-1, -1));
        ColorTexture = 1;
        break;
    case 2:
        furnPos = vec2(-1, 0);
        sca = vec3(0.15f, 0.2f, 0.15f);
        rot = vec3(0.0f, -3.1415f / 2.0f, 0.0f);
        Sectors.push_back(vec2(-1, 0));
        ColorTexture = 4;
        break;
    case 3:
        furnPos = vec2(-1, 0);
        sca = vec3(0.7f, 0.5f, 0.075f);
        rot = vec3(0.0f,-3.1415f/2.0f,0.0f);
        Sectors.push_back(vec2(-1, 0));
        Sectors.push_back(vec2(-1, -1));
        Sectors.push_back(vec2(-1, 1));
        ColorTexture = 5;
        break;
    case 4:
        furnPos = vec2(-0.8f, 0);
        sca = vec3(0.07f, 0.22f, 0.07f);
        rot = vec3(0.0f);
        Sectors.push_back(vec2(-1, 0));
        ColorTexture = 3;
        break;
    case 5:
        furnPos = vec2(0, 0);
        sca = vec3(0.2f, 0.5f, 0.2f);
        rot = vec3(0.0f, 3.1415f / 4.0f, 0.0f);
        Sectors.push_back(vec2(0, 0));
        ColorTexture = 6;
        break;
    case 6:
        furnPos = vec2(-1, 0);
        sca = vec3(0.2f, 0.09f, 0.35f);
        rot = vec3(0.0f, -3.1415f / 2.0f, 0.0f);
        Sectors.push_back(vec2(-1, 0));
        Sectors.push_back(vec2(0, 0));

        
        ColorTexture = 2;
        break;
    case 7:
        furnPos = vec2(-1, -1);
        sca = vec3(0.21f, 0.09f, 0.4f);
        rot = vec3(0.0f, 3.1415f, 0.0f);
        Sectors.push_back(vec2(-1, -1));
        Sectors.push_back(vec2(-1, 0));
        ColorTexture = 2;
        break;
    case -2:
        furnPos = vec2(0,0);
        sca = vec3(0.13f, 0.07f, 0.07f);
        rot = vec3(0.0f);
        Sectors.push_back(vec2(0, 0));
        ColorTexture = 7;
        break;
    default:
        

        break;
    }
    */
    // gets the Furniture data based off its type
    switch (type)
    {
    case 0:
        //table
        furnPos = vec2(-1, -1);
        sca = vec3(0.25f, 0.08f, 0.25f);
        rot = vec3(0.0f);
        ColorTexture = 15;

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

        Sectors.push_back(vec2(0, 0));
        Sectors.push_back(vec2(-1, 0));
        Sectors.push_back(vec2(0, -1));
        Sectors.push_back(vec2(1, 0));
        Sectors.push_back(vec2(0, 1));
        break;
    case 2:
        //turned bookshelf

        furnPos = vec2(-1, -1);
        sca = vec3(0.4f, 0.4f, 0.05f);
        rot = vec3(0.0f, -3.1415f * 3 / 4.0f, 0.0f);
        ColorTexture = 25;

        Sectors.push_back(vec2(-4, -1));
        Sectors.push_back(vec2(-3, -2));
        Sectors.push_back(vec2(-2, -3));
        Sectors.push_back(vec2(-1, -4));
        Sectors.push_back(vec2(-2, -4));
        Sectors.push_back(vec2(-3, -3));
        Sectors.push_back(vec2(-4, -2));
        break;
    case 3:
        //straight bookshelf

        furnPos = vec2(-1, 0);
        sca = vec3(0.7f, 0.5f, 0.075f);
        rot = vec3(0.0f, -3.1415f / 2.0f, 0.0f);
        ColorTexture = 25;

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

        Sectors.push_back(vec2(-3, 0));

        break;
    case 5:
        //chair

        furnPos = vec2(-1, 0);
        sca = vec3(0.11f, 0.15f, 0.11f);
        rot = vec3(0.0f, -3.1415f / 2.0f, 0.0f);

        ColorTexture = 24;

        Sectors.push_back(vec2(-4, 0));
        break;
    case -2:
        //chest

        furnPos = vec2(0, 0);
        sca = vec3(0.13f, 0.07f, 0.07f);
        rot = vec3(0.0f);

        ColorTexture = 13;

        Sectors.push_back(vec2(0, 0));
        break;
    case 7:
        //big bed

        furnPos = vec2(-1, -1);
        sca = vec3(0.18f, 0.09f, 0.4f);
        rot = vec3(0.0f, 3.1415f, 0.0f);
        ColorTexture = 16;

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
        if ((*grid)[(int)round(Sectors[j].x + 4)][ (int)round(Sectors[j].y + 4)] == 1) (*grid)[(int)round(Sectors[j].x + 4)][ (int)round(Sectors[j].y + 4)] = 3;
        else (*grid)[(int)round(Sectors[j].x + 4)][ (int)round(Sectors[j].y + 4)] = 2;
    }

    // rotates the object
    furnPos = vec2(furnPos.x * cos((3.1415f / 2.0f) * orientation) + furnPos.y * sin((3.1415f / 2.0f) * orientation),
        -furnPos.x * sin((3.1415f / 2.0f) * orientation) + furnPos.y * cos((3.1415f / 2.0f) * orientation));

    rot.y += (3.1415f / 2.0f) * orientation;
    // scales the object
    sca *= size * thk;
    // the distance that must be shifted to be within the cell
    vec2 width = vec2(
        max(abs(rott(vec2(-1, 1) * vec2(sca.x, sca.z) ,rot.y).x), abs(rott(vec2(-1, -1) * vec2(sca.x, sca.z), rot.y).x)),
        max(abs(rott(vec2(-1, -1) * vec2(sca.x, sca.z),rot.y).y), abs(rott(vec2(1, -1) * vec2(sca.x, sca.z), rot.y).y))) +  vec2(size * 0.02f, size * 0.02f);
    furnPos = (vec2(size * thk) - width) * furnPos;
    pos = vec3(furnPos.x,sca.y, furnPos.y) + ps;

    // initialises the object that will be dislpayed on screen
    obj = SpObj(pos,rot,sca, initCubeBuffer(),ColorTexture,NormalTexture);
    obj.textOff2 = glm::vec4(0.0f, 0.0f, 5.0f, 5.0f);
}

bool Furniture::canFit(int type, int orientation, int(*grid)[9][9]) {

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
        Sectors.push_back(vec2(-4, -1));
        Sectors.push_back(vec2(-3, -2));
        Sectors.push_back(vec2(-2, -3));
        Sectors.push_back(vec2(-1, -4));
        Sectors.push_back(vec2(-2, -4));
        Sectors.push_back(vec2(-3, -3));
        Sectors.push_back(vec2(-4, -2));
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

        if ((*grid)[(int)round(Sectors[j].x + 4)][ (int)round(Sectors[j].y + 4)] >= 1)
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