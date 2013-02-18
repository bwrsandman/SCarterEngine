/* 
 * File:   IKLine.hpp
 * Author: sandy
 *
 * Created on February 18, 2013, 8:53 AM
 */

#ifndef IKLINE_HPP
#define	IKLINE_HPP

#include <gtkmm.h>
#include <gtkglmm.h>
class Vector2f;

const float LEN = 0.2f;

class IKLine {
public:
    IKLine(float=LEN);
    IKLine(const IKLine& orig);
    virtual ~IKLine();
    
    inline void set_parent(IKLine* const p) { parent = p; }
    
    /* Vertex arrays */
    const GLfloat vertices[4];
    const GLubyte indices[2];
    float cum_orient() const;
    Vector2f cum_base() const;
    Vector2f cum_tip() const;
    
private:
    float len = LEN;
    IKLine* parent = NULL;
    float loc_orientation = 0.0f;
};

#endif	/* IKLINE_HPP */

