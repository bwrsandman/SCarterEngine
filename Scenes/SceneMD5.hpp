/*
 * File:   SceneMD5.hpp
 * Author: sandy
 *
 * Created on March 14, 2013, 1:38 PM
 *
 */

#ifndef SCENEMD5_HPP
#define	SCENEMD5_HPP

class md5;

class SceneMD5 : public SceneBase
{
public:
    explicit SceneMD5();
    virtual ~SceneMD5();
    void load_md5_mesh(const char*);
    void set_afps(float);
    void set_scale(float);
    void toggle_slerp(bool);
    void toggle_joints(bool);
    void toggle_wireframe(bool);
    void toggle_fill(bool);

protected:
    virtual void create_geom();
    virtual void create_light();
    virtual void set_perspective();
    virtual void render(const float);
    virtual void update(const float);
    virtual bool create_shaders(const char*, const char*) { return true; } 

private:
    float _angle = -70.0f;
    md5* model = NULL;
    bool fill = true;
    bool wireframe = true;
    float scale = 30.0f;
    
    
};

#endif	/* SCENEMD5_HPP */

