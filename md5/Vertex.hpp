#ifndef VERTEX
#define VERTEX

class weight;
class joint;

class vertex
{
private:
    friend std::ostream& operator<< (std::ostream& os, const vertex& v);
    friend std::istream& operator>> (std::istream& is, vertex& v);
    int vertIndex, weightIndex, weightElem;
    float texU, texV;
public:
    void draw(const weight *w, const joint *j) const;
    inline const unsigned int getIndex() const { return vertIndex; }
    vertex();
    ~vertex();
};

#endif