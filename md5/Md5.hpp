#ifndef MD5
#define MD5

class mesh;
class joint;
class md5frame;

class md5
{
public:
    md5(std::string s);
    ~md5();
    void draw();
    void animate(float);
private:
    friend std::ostream& operator<< (std::ostream& os, md5& m);
    uint numFrames;
    uint numJoints;
    uint numMeshes;
    uint frameRate;
    uint numAnimatedComponents;
    float animTime;
    int* hierarchy;
    mesh *my_mesh;  
    joint *my_joints;   
    md5frame *my_frames;

    void loadmesh(std::string s);
    void loadanim(std::string s);
};
#endif