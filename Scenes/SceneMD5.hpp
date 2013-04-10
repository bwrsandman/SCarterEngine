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
    bool CreateMD5VBO(void);
    
    virtual bool DestroyVBO(void);

    void renderJoints(const float);
    void renderWeights(const float);
    void renderMD5(const float);
    
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
        "#version 400\n\n"
        "layout (location = 0) in vec3 VertexPosition;\n"
        "layout (location = 1) in float Weight;\n"
        "layout (location = 2) in float JointIndex;\n"
    
        // To fragment shader
        "out float ex_weight;\n"
    
        "uniform float gTime;\n"  // Animation time
        "uniform mat4 gWorld;\n"
        "uniform vec3 jointPosition[16];\n"
        "uniform vec4 jointOrientation[16];\n"
   
        "vec4 quatMult(vec4 qa, vec4 qb) {\n"
        "  vec4 r;\n"
        "  r.w = (qa.w * qb.w) - (qa.x * qb.x) - (qa.y * qb.y) - (qa.z * qb.z);\n"
        "  r.x = (qa.x * qb.w) + (qa.w * qb.x) + (qa.y * qb.z) - (qa.z * qb.y);\n"
        "  r.y = (qa.y * qb.w) + (qa.w * qb.y) + (qa.z * qb.x) - (qa.x * qb.z);\n"
        "  r.z = (qa.z * qb.w) + (qa.w * qb.z) + (qa.x * qb.y) - (qa.y * qb.x);\n"
        "  return r;\n"
        "}\n\n"
        
        "vec3 quatRot(vec3 v, vec4 q) {\n"
        "  vec4 inverse = normalize(vec4(-q.xyz, q.w));\n"
        "  vec4 result = quatMult(q, vec4(v, 0.0));\n"
        "  result = quatMult(result, inverse);\n"
        "  return result.xyz;\n"
        "}\n"
    
        "void main( void ) {\n"
        "  uint jIndex = uint(JointIndex);\n"
        "  vec3 jointOffset = jointPosition[jIndex] * Weight;\n"
        "  vec4 jointQuaternion = jointOrientation[jIndex];\n"
        "  vec3 pos = jointOffset + quatRot(VertexPosition, jointQuaternion);\n"
        "  gl_Position = gWorld * vec4(pos, 1.0);\n"
        "  ex_weight = float(jIndex) * 0.25;\n"
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
    
    const char* MD5_VERTEX_SHADER =
        "#version 400\n\n"
        // w is weight value xyz is positions
        "layout (location = 0) in vec4 WeightPosition0;\n"
        "layout (location = 1) in vec4 WeightPosition1;\n"
        "layout (location = 2) in vec4 WeightPosition2;\n"
        "layout (location = 4) in vec4 WeightPosition3;\n"
        "layout (location = 5) in vec4 JointIndices;\n\n"
    
        // To fragment shader
        "out float ex_weight;\n\n"
    
        "uniform float gTime;\n"  // Animation time
        "uniform mat4 gWorld;\n"
        "uniform vec3 jointPosition[16];\n"
        "uniform vec4 jointOrientation[16];\n\n"
   
        "vec4 quatMult(vec4 qa, vec4 qb) {\n"
        "  vec4 r;\n"
        "  r.w = (qa.w * qb.w) - (qa.x * qb.x) - (qa.y * qb.y) - (qa.z * qb.z);\n"
        "  r.x = (qa.x * qb.w) + (qa.w * qb.x) + (qa.y * qb.z) - (qa.z * qb.y);\n"
        "  r.y = (qa.y * qb.w) + (qa.w * qb.y) + (qa.z * qb.x) - (qa.x * qb.z);\n"
        "  r.z = (qa.z * qb.w) + (qa.w * qb.z) + (qa.x * qb.y) - (qa.y * qb.x);\n"
        "  return r;\n"
        "}\n\n"
        
        "vec3 quatRot(vec3 v, vec4 q) {\n"
        "  vec4 inverse = normalize(vec4(-q.xyz, q.w));\n"
        "  vec4 result = quatMult(q, vec4(v, 0.0));\n"
        "  result = quatMult(result, inverse);\n"
        "  return result.xyz;\n"
        "}\n\n"
    
        "void main( void ) {\n"
        "  vec3 pos = vec3(0.0);\n"
        "  ex_weight = 0.0;\n"
        "  mat4 WeightPositions = mat4(WeightPosition0,\n"
        "                              WeightPosition1,\n"
        "                              WeightPosition2,\n"
        "                              WeightPosition3);\n"
        "  for (uint i = 0; i < 4; ++i) {\n"
        "    uint jIndex = uint(JointIndices[i]);\n"
        "    vec3 VertexPosition = WeightPositions[i].xyz;\n"
        "    float Weight = WeightPositions[i].w;\n"
        "    if (Weight <= 0.0)\n"
        "      continue;\n"
        "    vec3 jointOffset = jointPosition[jIndex];\n"
        "    vec4 jointQuaternion = jointOrientation[jIndex];\n"
        "    pos += Weight * (jointOffset + quatRot(VertexPosition, jointQuaternion));\n"
        "    ex_weight = Weight * float(jIndex)/4.0;\n"
        "  }\n"
        "  gl_Position = gWorld * vec4(pos, 1.0);\n"
        "}\n";

    const char* MD5_FRAGMENT_SHADER =
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
    float* jointOrientations = NULL;
    
    uint numJoints = 0;
    uint numWeights = 0;
    uint numMeshes = 0;
    uint numVertices = 0;
    
    GLuint jointsVAO;
    GLuint weightsVAO;
    GLuint md5VAO;
    
    const GLuint jointVAOIndex = 0;
    const GLuint weigthVAOIndex = 1;
    const GLuint md5VAOIndex = 2;
    
    GLuint vertexPtr;
    
    GLuint weightPositionPtr;
    GLuint weightValuePtr;
    GLuint weightJointPtr;
    
    GLuint jointPositionPtr;
    GLuint jointDepthPtr;
    
    GLuint md5WeightPositionPtr[4];
    GLuint md5JointPtr;
    
    /* Shaders */
    GLuint                JOINTSHVERT;
    GLuint                JOINTSHFRAG;
    GLuint                JOINTSHPROG;
    
    GLuint                WEIGHTSHVERT;
    GLuint                WEIGHTSHFRAG;
    GLuint                WEIGHTSHPROG;
    
    GLuint                MD5SHVERT;
    GLuint                MD5SHFRAG;
    GLuint                MD5SHPROG;
    
    /* Uniforms */
    GLuint gWeightJointLocation;
    GLuint gWeightJointOrientation;
    
    GLuint gMD5JointLocation;
    GLuint gMD5JointOrientation;
    
};

#endif	/* SCENEMD5_HPP */

