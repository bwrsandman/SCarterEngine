/* 
 * File:   SceneCubicContainer.hpp
 * Author: sandy
 *
 * Created on March 7, 2013, 1:40 AM
 */

#ifndef SCENECUBICCONTAINER_HPP
#define	SCENECUBICCONTAINER_HPP

class CubicGameState;

class SceneCubicContainer : public SceneBase
{
public:
    SceneCubicContainer();
    SceneCubicContainer(const SceneCubicContainer& orig);
    virtual ~SceneCubicContainer();
private:
    CubicGameState *gs;
    
    void reset_game ();

protected:
    virtual void create_geom();
    virtual void create_light();
    virtual void render(const float);
    virtual void update(const float);

};

#endif	/* SCENECUBICCONTAINER_HPP */

