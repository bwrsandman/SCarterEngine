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
    bool CreateWeightVBO(void);
    virtual bool DestroyVBO(void);

    void renderJoints(const float);
    void renderWeights(const float);
    
    const char* JOINT_VERTEX_SHADER =
        "#version 400\n"
        "layout (location = 0) in vec3 VertexPosition;\n"
        "layout (location = 1) in float depth;\n"
    
        // To fragment shader
        "out float ex_depth;\n"
    
        "uniform float gTime;\n"  // Animation time
        "uniform mat4 gWorld;\n"
    
        "void main( void ) {\n"
        "  vec3 pos = VertexPosition;\n"

        "  gl_Position = gWorld * vec4(pos, 1.0);\n"
        "  ex_depth = depth;\n"
        "}\n";

    const char* JOINT_FRAGMENT_SHADER =
        "#version 400\n"
        "in  float ex_depth;\n"
        "out vec4 out_Color;\n"
    
        "const vec4 red = vec4(1.0, 0.0, 0.0, 1.0);\n"
        "const vec4 blue = vec4(0.0, 0.0, 1.0, 1.0);\n"
    
        "void main(void){\n"
        "  out_Color = mix(blue, red, clamp(ex_depth, 0.0, 1.0));\n"
        "}\n";
    
    const char* WEIGHT_VERTEX_SHADER =
        "#version 400\n"
        "layout (location = 0) in vec3 VertexPosition;\n"
        "layout (location = 1) in float Weight;\n"
        "layout (location = 2) in uint JointIndex;\n"
    
        // To fragment shader
        "out float ex_weight;\n"
    
        "uniform float gTime;\n"  // Animation time
        "uniform mat4 gWorld;\n"
        "uniform vec3 jointPosition[16];\n"
    
        "void main( void ) {\n"
        "  vec3 jointOffset = jointPosition[JointIndex];\n"
        "  vec3 pos = jointOffset + VertexPosition;\n"

        "  gl_Position = gWorld * vec4(pos, 1.0);\n"
        "  ex_weight = Weight;\n"
        "}\n";

    const char* WEIGHT_FRAGMENT_SHADER =
        "#version 400\n"
        "in  float ex_weight;\n"
        "out vec4 out_Color;\n"
    
        "const vec4 red = vec4(1.0, 0.0, 0.0, 1.0);\n"
        "const vec4 blue = vec4(0.0, 0.0, 1.0, 1.0);\n"
    
        "void main(void){\n"
        "  out_Color = mix(blue, red, clamp(ex_weight, 0.0, 1.0));\n"
        "}\n";
    
private:
    Joint *joints = NULL;
    Mesh *meshes = NULL;
    
    float* jointPositions = NULL;
    
    uint numJoints = 0;
    uint numWeights = 0;
    uint numMeshes = 0;
    
    GLuint jointsVAO;
    GLuint weightsVAO;
    
    const GLuint jointVAOIndex = 0;
    const GLuint weigthVAOIndex = 1;
    
    GLuint vertexPtr;
    
    GLuint weightPositionPtr;
    GLuint weightValuePtr;
    GLuint weightJointPtr;
    
    GLuint jointPositionPtr;
    GLuint jointDepthPtr;
    
    /* Shaders */
    GLuint                JOINTSHVERT;
    GLuint                JOINTSHFRAG;
    GLuint                JOINTSHPROG;
    
    GLuint                WEIGHTSHVERT;
    GLuint                WEIGHTSHFRAG;
    GLuint                WEIGHTSHPROG;
    
    /* Uniforms */
    GLuint gJointLocation;
    
};

#endif	/* SCENEMD5_HPP */

