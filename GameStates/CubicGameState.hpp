/* 
 * File:   CubicGameState.hpp
 * Author: sandy
 *
 * Created on March 7, 2013, 3:12 PM
 */

#ifndef CUBICGAMESTATE_HPP
#define	CUBICGAMESTATE_HPP

class GL1_Sphere;
class GL1_Cube;

const float sides = 3.0f;

class CubicGameState 
{
public:
    CubicGameState();
    CubicGameState(const CubicGameState& orig);
    virtual ~CubicGameState();

    // Accessors
    inline bool running(void) const { return _running; }
    inline void running(bool running)  { _running = running; }

    inline bool paused(void) const  { return _paused; }
    inline void pause(void)  { _paused = !_paused; }

    inline bool gravity(void) const  { return _gravity; }
    inline void togl_gravity(void)  { _gravity = !_gravity; }

    inline bool drag(void) const  { return _drag; }
    inline void togl_drag(void)  { _drag = !_drag; }

    inline bool kenergy(void) const  { return _kenergy; }
    inline void togl_kenergy(void)  { _kenergy = !_kenergy; }
    
    inline float angle (void) const { return _angle; }
    
    void update (float);
    void draw (void);
    
private:
    float _angle = 165.0f;
    bool _running = true;
    bool _paused  = false;
    bool _gravity = true;
    bool _drag    = true;
    bool _kenergy = true;
    GL1_Sphere *_ball;
    GL1_Cube *_box;

    bool collidesInX(GL1_Sphere& sphere, GL1_Cube& cube) const;
    bool collidesInY(GL1_Sphere& sphere, GL1_Cube& cube) const;
    bool collidesInZ(GL1_Sphere& sphere, GL1_Cube& cube) const;

};

#endif	/* CUBICGAMESTATE_HPP */

