#include <GL/glew.h>
#include <GL/glu.h>

#include <gtkmm.h>
#include <gtkglmm.h>
#include <glm/glm.hpp>

#include <fstream>
#include <string>
#include <iostream>
#include <assert.h>

#include "../md5/Md5.hpp"

#include "SceneBase.hpp"
#include "SceneMD5.hpp"

SceneMD5::SceneMD5()
{
}

SceneMD5::~SceneMD5()
{
    delete fish;
    fish = NULL;
}

void SceneMD5::create_light()
{
    //Add ambient light
    GLfloat ambientColor[] = {0.2f, 0.2f, 0.2f, 1.0f};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);

    //Add positioned light
    GLfloat lightColor0 [] = {0.5f, 0.5f, 0.5f, 1.0f};
    GLfloat lightPos0 [] = {4.0f, 0.0f, 8.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);

    //Add directed light
    GLfloat lightColor1 [] = {0.5f, 0.2f, 0.2f, 1.0f};
    GLfloat lightPos1 [] = {-1.0f, 0.5f, 0.5f, 0.0f};
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
    glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
}

void SceneMD5::create_geom()
{
    fish = new md5("fish");
}

void SceneMD5::load_md5_mesh(const char* filename)
{
    delete(fish);
    fish = new md5("fish");
}

void SceneMD5::set_perspective()
{
    GLfloat h = (GLfloat)get_height();
    GLfloat w = (GLfloat)get_width();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)w / (double)h, 1.0, 200.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0f, -7.5f, -50.0f);
    glRotatef (-90.0f, 1.0, 0.0, 0.0);
    glScalef(30.0f, 30.0f, 30.0f);
}

void SceneMD5::render(const float dt)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    if (!fish)
        return;
    
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	//Add ambient light
	GLfloat ambientColor[] = {0.2f, 0.2f, 0.2f, 1.0f}; //Color (0.2, 0.2, 0.2)
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);

	//Add positioned light
	GLfloat lightColor0[] = {0.5f, 0.5f, 0.5f, 1.0f}; //Color (0.5, 0.5, 0.5)
	GLfloat lightPos0[] = {4.0f, 0.0f, 8.0f, 1.0f}; //Positioned at (4, 0, 8)
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);

	//Add directed light
	GLfloat lightColor1[] = {0.5f, 0.2f, 0.2f, 1.0f}; //Color (0.5, 0.2, 0.2)
	//Coming from the direction (-1, 0.5, 0.5)
	GLfloat lightPos1[] = {-1.0f, 0.5f, 0.5f, 0.0f};
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);

	glEnable(GL_LIGHTING);
	glTranslatef(0.0f, -7.5f, -50.0f);
	glRotatef(_angle, 0.0f, 1.0f, 0.0f);
  	glRotatef (-90.0f, 1.0, 0.0, 0.0);

	glScalef(30.0f, 30.0f, 30.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//fish->draw();
	glDisable(GL_LIGHTING);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	fish->draw();
}

void SceneMD5::update(const float dt)
{
    SceneBase::update(dt);
    _angle += dt;
    if (_angle > 360.0f)
        _angle -= 360.0f;
    if(fish)
        fish->animate(dt);
}