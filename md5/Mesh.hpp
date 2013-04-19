#ifndef MESH
#define MESH

class vertex;
class trangle;
class weight;

class mesh
{
private:
    friend std::ostream& operator<< (std::ostream& os, const mesh& m);
    friend std::istream& operator>> (std::istream& is, mesh& m);
    unsigned int numverts;
    unsigned int numtris;
    unsigned int numweights;
    std::string shader;
    vertex *verts;
    triangle *tris;
    weight *weights;


public:
    mesh();
    ~mesh();
    void draw(const joint *j);
};
#endif