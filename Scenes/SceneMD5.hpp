/*
 * File:   SceneMD5.hpp
 * Author: sandy
 *
 * Created on March 14, 2013, 1:38 PM
 *
 */

#ifndef SCENEMD5_HPP
#define	SCENEMD5_HPP

class Joint;
class Mesh;

class SceneMD5 : public SceneBase
{
public:
    explicit SceneMD5();
    virtual ~SceneMD5();
    void load_md5_mesh(const char*);

protected:
    virtual void create_geom();
    virtual void create_light();
    virtual void set_perspective();
    virtual void render(const float);
    virtual void update(const float);
    virtual bool create_shaders(const char*, const char*); 
    bool create_shader (GLuint&, GLuint&, GLuint&, const char*, const char*);
    
    virtual bool CreateVBO(void);
    bool CreateJointVBO(void);
    virtual bool DestroyVBO(void);

    void renderJoints(const float);
    
    const char* JOINT_VERTEX_SHADER =
        "#version 400\n"
        "layout (location = 0) in vec3 VertexPosition;\n"
    
        // To fragment shader
        "out vec3 ex_Color;\n"
    
        "uniform float gTime;\n"  // Animation time
        "uniform mat4 gWorld;\n"
    
        "void main( void ) {\n"
        // Assume the initial position is (0,0,0).
        "  vec3 pos = VertexPosition;\n"

        "  gl_Position = gWorld * vec4(pos, 1.0);\n"
        "  ex_Color = clamp(pos, 0.0, 1.0);\n"
        "}\n";

    const char* JOINT_FRAGMENT_SHADER =
        "#version 400\n"
        "in  vec3 ex_Color;\n"
        "out vec4 out_Color;\n"
    
        "void main(void){\n"
        "       out_Color = vec4(ex_Color, 1.0);\n"
        "}\n";
    
private:
    Joint *joints = NULL;
    Mesh *meshes = NULL;
    uint numJoints = 0;
    uint numMeshes;
    
    GLuint points;
    uint numPoints;
    
    GLuint vertexPtr;
    GLuint weightPtr;
    GLuint jointPtr;
    
    /* Shaders */
    GLuint                JOINTSHVERT;
    GLuint                JOINTSHFRAG;
    GLuint                JOINTSHPROG;
    
};

#endif	/* SCENEMD5_HPP */

