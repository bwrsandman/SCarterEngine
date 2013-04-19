#ifndef TRIANGLE
#define TRIANGLE

class joint;
class weight;
class vertex;

class triangle
{
private:
    friend std::ostream& operator<< (std::ostream& os, const triangle& v);
    friend std::istream& operator>> (std::istream& is, triangle& v);
    int triIndex, vertIndex1, vertIndex2, vertIndex3;
public:
    void draw(const vertex *v, const weight *w, const joint *j);
    inline const unsigned int getIndex() const { return triIndex; }
    triangle();
    ~triangle();
};

#endif