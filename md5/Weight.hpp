#ifndef WEIGHT
#define WEIGHT

#include <glm/glm.hpp>

const glm::vec3 outColors [] = { glm::vec3(0.5f, 0.5f, 1.0f),
                                 glm::vec3(0.5f, 1.0f, 0.5f),
                                 glm::vec3(1.0f, 0.5f, 0.5f) };

class joint;

class weight
{
private:
    friend std::ostream& operator<< (std::ostream& os, const weight& w);
    friend std::istream& operator>> (std::istream& is, weight& w);
    int weightIndex, jointIndex; 
    float weightValue;
    glm::vec3 pos;
public:
    inline const unsigned int getIndex() { return weightIndex; }
    glm::vec3 getPos(const joint *j) const;
    weight();
    ~weight();
    static void draw(int numweights, const weight *w, const joint *j);
};

#endif