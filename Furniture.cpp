#include "Furniture.h"
#include <random>
vec2 rott(vec2 v, float r)
{
    return vec2(v.x * cos(r) + v.y * sin(r), -v.x * sin(r) + v.y * cos(r));
}

Furniture::Furniture(int i, int k, int(*grid)[9][9], float size, float thk, vec3 ps) {
	std::random_device rd;  // Seed generator
	std::mt19937 gen(rd()); // Mersenne Twister engine
	std::uniform_real_distribution<float> Rand(0.0f, 1.0f); // Range [0, 1]
    type1 = 2;
    type2 = 0;

    vec2 rectP;
    std::vector<vec2> cnt;
    /*
    switch (i) {
    case 0:
        rectP = vec2(-1, -1);
        sca = vec3(0.4f, 0.4f, 0.05f);
        rot = vec3(0.0f,-3.1415f*3 / 4.0f, 0.0f);
        type1 = 5;
        cnt.push_back(vec2(-1.0f, -1.0f));
        cnt.push_back(vec2(-1, 0));
        cnt.push_back(vec2(0, -1));
        break;
    case 1:
        rectP = vec2(-1, -1);
        sca = vec3(0.25f, 0.08f, 0.25f);
        rot = vec3(0.0f);
        cnt.push_back(vec2(-1, -1));
        type1 = 1;
        break;
    case 2:
        rectP = vec2(-1, 0);
        sca = vec3(0.15f, 0.2f, 0.15f);
        rot = vec3(0.0f, -3.1415f / 2.0f, 0.0f);
        cnt.push_back(vec2(-1, 0));
        type1 = 4;
        break;
    case 3:
        rectP = vec2(-1, 0);
        sca = vec3(0.7f, 0.5f, 0.075f);
        rot = vec3(0.0f,-3.1415f/2.0f,0.0f);
        cnt.push_back(vec2(-1, 0));
        cnt.push_back(vec2(-1, -1));
        cnt.push_back(vec2(-1, 1));
        type1 = 5;
        break;
    case 4:
        rectP = vec2(-0.8f, 0);
        sca = vec3(0.07f, 0.22f, 0.07f);
        rot = vec3(0.0f);
        cnt.push_back(vec2(-1, 0));
        type1 = 3;
        break;
    case 5:
        rectP = vec2(0, 0);
        sca = vec3(0.2f, 0.5f, 0.2f);
        rot = vec3(0.0f, 3.1415f / 4.0f, 0.0f);
        cnt.push_back(vec2(0, 0));
        type1 = 6;
        break;
    case 6:
        rectP = vec2(-1, 0);
        sca = vec3(0.2f, 0.09f, 0.35f);
        rot = vec3(0.0f, -3.1415f / 2.0f, 0.0f);
        cnt.push_back(vec2(-1, 0));
        cnt.push_back(vec2(0, 0));

        
        type1 = 2;
        break;
    case 7:
        rectP = vec2(-1, -1);
        sca = vec3(0.21f, 0.09f, 0.4f);
        rot = vec3(0.0f, 3.1415f, 0.0f);
        cnt.push_back(vec2(-1, -1));
        cnt.push_back(vec2(-1, 0));
        type1 = 2;
        break;
    case -2:
        rectP = vec2(0,0);
        sca = vec3(0.13f, 0.07f, 0.07f);
        rot = vec3(0.0f);
        cnt.push_back(vec2(0, 0));
        type1 = 7;
        break;
    default:
        

        break;
    }
    */
    switch (i)
    {
    case 0:
        //table
        rectP = vec2(-1, -1);
        sca = vec3(0.25f, 0.08f, 0.25f);
        rot = vec3(0.0f);
        type1 = 1;

        cnt.push_back(vec2(-4, -4));
        cnt.push_back(vec2(-3, -4));
        cnt.push_back(vec2(-2, -4));

        cnt.push_back(vec2(-4, -3));
        cnt.push_back(vec2(-3, -3));
        cnt.push_back(vec2(-2, -3));

        cnt.push_back(vec2(-4, -2));
        cnt.push_back(vec2(-3, -2));
        cnt.push_back(vec2(-2, -2));
        break;
    case 1:
        //pillar

        rectP = vec2(0, 0);
        sca = vec3(0.2f, 0.5f, 0.2f);
        rot = vec3(0.0f, 3.1415f / 4.0f, 0.0f);
        type1 = 6;

        cnt.push_back(vec2(0, 0));
        cnt.push_back(vec2(-1, 0));
        cnt.push_back(vec2(0, -1));
        cnt.push_back(vec2(1, 0));
        cnt.push_back(vec2(0, 1));
        break;
    case 2:
        //turned bookshelf

        rectP = vec2(-1, -1);
        sca = vec3(0.4f, 0.4f, 0.05f);
        rot = vec3(0.0f, -3.1415f * 3 / 4.0f, 0.0f);
        type1 = 5;

        cnt.push_back(vec2(-4, -1));
        cnt.push_back(vec2(-3, -2));
        cnt.push_back(vec2(-2, -3));
        cnt.push_back(vec2(-1, -4));
        cnt.push_back(vec2(-2, -4));
        cnt.push_back(vec2(-3, -3));
        cnt.push_back(vec2(-4, -2));
        break;
    case 3:
        //straight bookshelf

        rectP = vec2(-1, 0);
        sca = vec3(0.7f, 0.5f, 0.075f);
        rot = vec3(0.0f, -3.1415f / 2.0f, 0.0f);
        type1 = 5;

        cnt.push_back(vec2(-4, -3));
        cnt.push_back(vec2(-4, -2));
        cnt.push_back(vec2(-4, -1));
        cnt.push_back(vec2(-4, 0));
        cnt.push_back(vec2(-4, 1));
        cnt.push_back(vec2(-4, 2));
        cnt.push_back(vec2(-4, 3));
        break;
    case 4:
        //tourch

        rectP = vec2(-0.8f, 0);
        sca = vec3(0.07f, 0.22f, 0.07f);
        rot = vec3(0.0f);

        type1 = 3;

        cnt.push_back(vec2(-3, 0));

        break;
    case 5:
        //chair

        rectP = vec2(-1, 0);
        sca = vec3(0.11f, 0.15f, 0.11f);
        rot = vec3(0.0f, -3.1415f / 2.0f, 0.0f);

        type1 = 4;

        cnt.push_back(vec2(-4, 0));
        break;
    case -2:
        //chest

        rectP = vec2(0, 0);
        sca = vec3(0.13f, 0.07f, 0.07f);
        rot = vec3(0.0f);

        type1 = 7;

        cnt.push_back(vec2(0, 0));
        break;
    case 7:
        //big bed

        rectP = vec2(-1, -1);
        sca = vec3(0.21f, 0.09f, 0.4f);
        rot = vec3(0.0f, 3.1415f, 0.0f);
        type1 = 2;

        cnt.push_back(vec2(-4, -4));
        cnt.push_back(vec2(-3, -4));
        cnt.push_back(vec2(-4, -3));
        cnt.push_back(vec2(-3, -3));
        cnt.push_back(vec2(-4, -2));
        cnt.push_back(vec2(-3, -2));
        cnt.push_back(vec2(-4, -1));
        cnt.push_back(vec2(-3, -1));


        break;
    case 8:
        //small bed

        rectP = vec2(-1, 0);
        sca = vec3(0.12f, 0.07f, 0.25f);
        rot = vec3(0.0f, -3.1415f / 2.0f, 0.0f);
        type1 = 2;

        cnt.push_back(vec2(-4, 0));
        cnt.push_back(vec2(-3, 0));
        cnt.push_back(vec2(-2, 0));
        break;
    default:
       
        break;
    }
    for (int j = 0; j < cnt.size(); j++)
    {
        cnt[j] = vec2(cnt[j].x * cos((3.1415f / 2.0f) * k) + cnt[j].y * sin((3.1415f / 2.0f) * k),
            -cnt[j].x * sin((3.1415f / 2.0f) * k) + cnt[j].y * cos((3.1415f / 2.0f) * k));
        if ((*grid)[(int)round(cnt[j].x + 4)][ (int)round(cnt[j].y + 4)] == 1) (*grid)[(int)round(cnt[j].x + 4)][ (int)round(cnt[j].y + 4)] = 3;
        else (*grid)[(int)round(cnt[j].x + 4)][ (int)round(cnt[j].y + 4)] = 2;
    }
    
    {
        rectP = vec2(rectP.x * cos((3.1415f / 2.0f) * k) + rectP.y * sin((3.1415f / 2.0f) * k),
            -rectP.x * sin((3.1415f / 2.0f) * k) + rectP.y * cos((3.1415f / 2.0f) * k));
        rot.y += (3.1415f / 2.0f) * k;
    }

    sca *= size * thk;
    vec2 width = vec2(
        max(abs(rott(vec2(-1, 1) * vec2(sca.x, sca.z) ,rot.y).x), abs(rott(vec2(-1, -1) * vec2(sca.x, sca.z), rot.y).x)),
        max(abs(rott(vec2(-1, -1) * vec2(sca.x, sca.z),rot.y).y), abs(rott(vec2(1, -1) * vec2(sca.x, sca.z), rot.y).y))) +  vec2(size * 0.02f, size * 0.02f);
    vec2 rtP = (vec2(size * thk) - width) * rectP;
    pos = vec3(rtP.x,sca.y, rtP.y) + ps;
    

}

bool Furniture::canFit(int i, int k, int(*grid)[9][9]) {

    std::vector<vec2> cnt;
    switch (i)
    {
    case 0:
        //table
        cnt.push_back(vec2(-4, -4));
        cnt.push_back(vec2(-3, -4));
        cnt.push_back(vec2(-2, -4));

        cnt.push_back(vec2(-4, -3));
        cnt.push_back(vec2(-3, -3));
        cnt.push_back(vec2(-2, -3));

        cnt.push_back(vec2(-4, -2));
        cnt.push_back(vec2(-3, -2));
        cnt.push_back(vec2(-2, -2));
        break;
    case 1:
        //pillar
        cnt.push_back(vec2(0, 0));
        cnt.push_back(vec2(-1, 0));
        cnt.push_back(vec2(0, -1));
        cnt.push_back(vec2(1, 0));
        cnt.push_back(vec2(0, 1));
        break;
    case 2:
        //turned bookshelf
        cnt.push_back(vec2(-4, -1));
        cnt.push_back(vec2(-3, -2));
        cnt.push_back(vec2(-2, -3));
        cnt.push_back(vec2(-1, -4));
        cnt.push_back(vec2(-2, -4));
        cnt.push_back(vec2(-3, -3));
        cnt.push_back(vec2(-4, -2));
        break;
    case 3:
        //straight bookshelf
        cnt.push_back(vec2(-4, -3));
        cnt.push_back(vec2(-4, -2));
        cnt.push_back(vec2(-4, -1));
        cnt.push_back(vec2(-4, 0));
        cnt.push_back(vec2(-4, 1));
        cnt.push_back(vec2(-4, 2));
        cnt.push_back(vec2(-4, 3));
        break;
    case 4:
        //tourch

        cnt.push_back(vec2(-3, 0));

        break;
    case 5:
        //chair

        cnt.push_back(vec2(-4, 0));
        break;
    case -2:
        //chest

        cnt.push_back(vec2(0, 0));
        break;
    case 7:
        //big bed

        cnt.push_back(vec2(-4, -4));
        cnt.push_back(vec2(-3, -4));
        cnt.push_back(vec2(-4, -3));
        cnt.push_back(vec2(-3, -3));
        cnt.push_back(vec2(-4, -2));
        cnt.push_back(vec2(-3, -2));
        cnt.push_back(vec2(-4, -1));
        cnt.push_back(vec2(-3, -1));

        break;
    case 8:
        //small bed
        cnt.push_back(vec2(-4, 0));
        cnt.push_back(vec2(-3, 0));
        cnt.push_back(vec2(-2, 0));
        break;
    default:
        return false;

        break;
    }
    bool fit = true;
    for (int j = 0; j < cnt.size(); j++)
    {
        cnt[j] = vec2(cnt[j].x * cos((3.1415f / 2.0f) * k) + cnt[j].y * sin((3.1415f / 2.0f) * k),
            -cnt[j].x * sin((3.1415f / 2.0f) * k) + cnt[j].y * cos((3.1415f / 2.0f) * k));
        if ((*grid)[(int)round(cnt[j].x + 4)][ (int)round(cnt[j].y + 4)] >= 1)
        {
            fit = false;
            break;
        }
    }
    return fit;
}
Furniture::Furniture() {
}