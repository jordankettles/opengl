/*
 *  Quad.hpp
 *  Quad Class.
 *  Has a direct render method for RTT.
 */
#ifndef QUAD_HPP
#define QUAD_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/norm.hpp>

#include "Object.hpp"

//!  Quad.
/*!
 Basic quad class that represents a quad and defines it rendering.
 */
class Quad:  public Object{
    
    public:
        //! Default constructor
        /*! Create quad. */
        Quad();
        //! Destructor
        /*! Delete quad. */
        ~Quad();
        //! init
        /*! Initialise quad. */
        void init();
        //! render
        /*! Render quad. */
        void render(Camera* camera);
        //! directRender
        /*! Direct rendering function used for Render to Texture.*/
        void directRender();
    
    private:
        
        GLfloat g_vertex_buffer_data[18];
        GLuint vertexbuffer;
};





#endif
