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

protected:
    virtual void create_geom();
    virtual void create_light();
    virtual void set_perspective();
    virtual void render(const float);
    virtual void update(const float);
    virtual bool create_shaders(const char*, const char*) { return true; } 

private:
    float _angle = -70.0f;
    md5* fish = NULL;
    bool fill = false;
    bool wireframe = true;
    
};

#endif	/* SCENEMD5_HPP */

