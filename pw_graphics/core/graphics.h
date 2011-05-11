/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

////////////////////////////////////////////////////////////////////////////////
///
/// \file       graphics.h
/// \brief      prototype of class "CGraphics"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2009-10-18
///
////////////////////////////////////////////////////////////////////////////////
#ifndef GRAPHICS_H
#define GRAPHICS_H

//--- Standard header --------------------------------------------------------//
#include <list>
#include <vector>

//--- Program header ---------------------------------------------------------//
#include "log.h"

//--- Misc header ------------------------------------------------------------//
#include "eigen2/Eigen/Core"
#include "eigen2/Eigen/Geometry"
#include "GL/gl.h"
#include "SDL.h"

using namespace Eigen;

// Constants
const double GRAPHICS_FOV_DEFAULT = 45.0;               ///< default fov
const double GRAPHICS_LEFT_DEFAULT = -160.0;            ///< Default left plane
const double GRAPHICS_RIGHT_DEFAULT = 160.0;            ///< Default right plane
const double GRAPHICS_BOTTOM_DEFAULT = -100.0;          ///< Default bottom plane
const double GRAPHICS_TOP_DEFAULT = 100.0;              ///< Default top plane
const double GRAPHICS_NEAR_DEFAULT = 0.1;               ///< Default min depth
const double GRAPHICS_FAR_DEFAULT = 1000.0;             ///< Default max depth

const double GRAPHICS_DYN_PEL_SIZE_DEFAULT = 10.0;      ///< Default size of dyn-objects

const double GRAPHICS_PI = std::acos(-1.0);             ///< Constant pi
const double GRAPHICS_DEG2RAD = GRAPHICS_PI / 180.0;    ///< Converts degree to radiant values
const double GRAPHICS_RAD2DEG = 180.0 / GRAPHICS_PI;    ///< Converts radiant to degree values
const unsigned short GRAPHICS_WIDTH_DEFAULT = 1440u;    ///< Default x-resolution
const unsigned short GRAPHICS_HEIGHT_DEFAULT = 900u;    ///< Default y-resolution

/// specifies the type of line to be drawn
typedef enum 
{
    GRAPHICS_LINETYPE_SINGLE,
    GRAPHICS_LINETYPE_LOOP,
    GRAPHICS_LINETYPE_STRIP
} LineType;

////////////////////////////////////////////////////////////////////////////////
///
/// \brief graphics-wrapper
///
/// This class is responsible for all graphical stuff. That is:
/// initialization and providing methods for drawing lines. The class is built
/// on top of OpenGL and the SDL. The class is implemented as a meyers-singleton
/// to provide easy access to its methods for graphics abstraction classes like
/// IShape.
///
/// \todo Implement frustum culling
/// \todo Perhaps moving the camera towards z-axis is better than scaling, see
///         frustum culling.
///
////////////////////////////////////////////////////////////////////////////////
class CGraphics
{

    public:
    
        /// Returns instance of meyers-singleton
        static CGraphics& getInstance()
        {
            static CGraphics Instance;
            return (Instance);
        };
        
        //
        //--- Methods for videomanagement ------------------------------------//
        //

        //--- Constant methods -----------------------------------------------//
        Vector2d        screen2World(const Vector2d&) const;
        Vector2d        screen2World(const double&, const double&) const;
        double          getDynPelSize() const;
        double          getResMPX() const;
        double          getResMPY() const;
        double          getResPMX() const;
        double          getResPMY() const;
        unsigned short  getWidthScr() const;
        unsigned short  getHeightScr() const;
        void setColor(const double&, const double&, const double&) const;
        void setColor(const double&, const double&, const double&, const double&) const;

        //--- Methods --------------------------------------------------------//
        bool init();
        bool resizeWindow(unsigned short, unsigned short);
        void setWidthScr(const unsigned short&);
        void setHeightScr(const unsigned short&);
        void swapBuffers();

        //
        //--- Methods for camera movement ------------------------------------//
        //

        //--- Constant methods -----------------------------------------------//
        Vector2d    getCamPos() const;
        double      getCamAng() const;
        double      getCamZoom() const;
        
        //--- Methods --------------------------------------------------------//
        void applyCamMovement();
        void resetCam();
        void rotCamBy(const double&);
        void rotCamTo(const double&);
        void transCamBy(const Vector2d&);
        void transCamTo(const Vector2d&);
        void zoomCamBy(const double&);
        void zoomCamTo(const double&);

        //
        //--- Methods for drawing --------------------------------------------//
        //

        //--- Constant methods -----------------------------------------------//
        void circle(const Vector2d&, const double&) const;
        void dot(const Vector2d&) const;
        void dots(const std::vector<Vector2d>&, const Vector2d& _vecOffset = Vector2d(0.0,0.0)) const;
        void filledRect(const Vector2d&, const Vector2d&) const;
        void polyline(const std::list<Vector2d>&, const LineType&, const Vector2d& _vecOffset = Vector2d(0.0,0.0)) const;
        void rect(const Vector2d&, const Vector2d&) const;
        void showVec(const Vector2d&, const Vector2d&) const;

        //--- Methods --------------------------------------------------------//
        void addVertex(const Vector2d&);
        void addVertex(const double&, const double&);
        void beginLine(const LineType&, const double&);
        void endLine();

        
    private:
        
        //--- Variabels [private] --------------------------------------------//
        Vector3d                m_vecCamPos;    ///< camera position
        double                  m_fCamAng;      ///< camera angle
        double                  m_fCamZoom;     ///< camera zoom
        double                  m_fDepthMax;    ///< maximum depth of levels
        double                  m_fDepthMin;    ///< minimum depth of levels

        double                  m_fDynPelSize;  ///< pel-size for dynamically sized shapes
        
        int                     m_nVideoFlags;  ///< videoflags like fullscreen, resolution
        unsigned short          m_unWidthScr;   ///< Screen width
        unsigned short          m_unHeightScr;  ///< Screen height

        unsigned int            m_unNrOfLines;  ///< Total number of lines drawn
        
        std::list<Vector2d>     m_VertList;     ///< list, containing the coordinates of vertices
        double                  m_fDepth;       ///< depth of lines in list
        

        //--- Constructor/destructor [private] -------------------------------//
        CGraphics();
        ~CGraphics(){};

        //--- Operators [private] --------------------------------------------//
        CGraphics& operator=(const CGraphics&); ///< empty private operator=

        //--- Methods [private] ----------------------------------------------//
        void glSetPerspective(  const GLdouble&, const GLdouble&,
                                const GLdouble&, const GLdouble&,
                                const GLdouble&) const;

};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Base class for graphics
///
/// This class is the base class for all classes using graphics. It just defines
/// a constructor that initializes the meyers-singleton for the graphics
/// instance.
/// 
////////////////////////////////////////////////////////////////////////////////
class CGraphicsBase
{
    protected:
        //--- Protected constructor ------------------------------------------//
        CGraphicsBase():m_Graphics(CGraphics::getInstance())
        {
            CTOR_CALL("GraphicsBase");
        };

        //--- Protected variables --------------------------------------------//
        CGraphics&  m_Graphics;                 ///< Instance of graphics class
};

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the element size of dynamically sized shapes
///
/// \return The element size set
///
///////////////////////////////////////////////////////////////////////////////
inline double CGraphics::getDynPelSize() const
{
    METHOD_ENTRY("CGraphics::getDynPelSize()");

    METHOD_EXIT("CGraphics::getDynPelSize()");
    return (m_fDynPelSize);
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns horizontal resolution
///
/// \return Horizontal resolution in m/pel
///
///////////////////////////////////////////////////////////////////////////////
inline double CGraphics::getResMPX() const
{
    METHOD_ENTRY("CGraphics::getResMPX()");

    METHOD_EXIT("CGraphics::getResMPX()");
    return ((GRAPHICS_RIGHT_DEFAULT-GRAPHICS_LEFT_DEFAULT) /
            (m_fCamZoom * m_unWidthScr));
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns vertical resolution
///
/// \return Vertical resolution in m/pel
///
///////////////////////////////////////////////////////////////////////////////
inline double CGraphics::getResMPY() const
{
    METHOD_ENTRY("CGraphics::getResMPY()");

    METHOD_EXIT("CGraphics::getResMPY()");
    return ((GRAPHICS_RIGHT_DEFAULT-GRAPHICS_LEFT_DEFAULT) /
            (m_fCamZoom * m_unWidthScr));
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the screen width
///
/// \return Screen width in pel
///
///////////////////////////////////////////////////////////////////////////////
inline unsigned short CGraphics::getWidthScr() const
{
    METHOD_ENTRY("CGraphics::getWidthScr()");

    METHOD_EXIT("CGraphics::getWidthScr()");
    return m_unWidthScr;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the screen height
///
/// \return Screen HeightScr in pel
///
///////////////////////////////////////////////////////////////////////////////
inline unsigned short CGraphics::getHeightScr() const
{
    METHOD_ENTRY("CGraphics::getHeightScr()");

    METHOD_EXIT("CGraphics::getHeightScr()");
    return m_unHeightScr;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Set RGB color
///
/// \param _fR Red value between 0.0 and 1.0
/// \param _fG Green value between 0.0 and 1.0
/// \param _fB Blue value between 0.0 and 1.0
///
///////////////////////////////////////////////////////////////////////////////
inline void CGraphics::setColor(const double& _fR, const double& _fG,
                                const double& _fB) const
{
    METHOD_ENTRY("CGraphics::setColor(const double&, const double&, const double&)");

    glColor3d(_fR, _fG, _fB);

    METHOD_EXIT("CGraphics::setColor(const double&, const double&, const double&)");
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Set RGB color with alpha value
///
/// \param _fR Red value between 0.0 and 1.0
/// \param _fG Green value between 0.0 and 1.0
/// \param _fB Blue value between 0.0 and 1.0
/// \param _fA Alpha value between 0.0 and 1.0
///
///////////////////////////////////////////////////////////////////////////////
inline void CGraphics::setColor(const double& _fR, const double& _fG,
                                const double& _fB, const double& _fA) const
{
    METHOD_ENTRY("CGraphics::setColor(const double&, const double&, const double&, const double&)");

    glColor4d(_fR, _fG, _fB, _fA);

    METHOD_EXIT("CGraphics::setColor(const double&, const double&, const double&, const double&)");
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Set the screen width
///
/// Actually, this method does not resize the screen, it just modifies the
/// variable. To resize the screen, it must be initialized again.
///
/// \param _unWidthScr Screen width in pel
///
///////////////////////////////////////////////////////////////////////////////
inline void CGraphics::setWidthScr(const unsigned short& _unWidthScr)
{
    METHOD_ENTRY("CGraphics::setWidthScr(const unsigned short&)");

    m_unWidthScr = _unWidthScr;

    METHOD_EXIT("CGraphics::setWidthScr(const unsigned short&)");
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Set the screen height
///
/// Actually, this method does not resize the screen, it just modifies the
/// variable. To resize the screen, it must be initialized again.
///
/// \param _unWidth Screen height in pel
///
///////////////////////////////////////////////////////////////////////////////
inline void CGraphics::setHeightScr(const unsigned short& _unHeightScr)
{
    METHOD_ENTRY("CGraphics::setHeightScr(const unsigned short&)");

    m_unHeightScr = _unHeightScr;

    METHOD_EXIT("CGraphics::setHeightScr(const unsigned short&)");
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the camera angle
///
/// \return The camera angle
///
///////////////////////////////////////////////////////////////////////////////
inline Vector2d CGraphics::getCamPos() const
{
    METHOD_ENTRY("CGraphics::getCamPos()");

    METHOD_EXIT("CGraphics::getCamPos()");
    return Vector2d(m_vecCamPos[0], m_vecCamPos[1]);
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the camera angle
///
/// \return The camera angle
///
///////////////////////////////////////////////////////////////////////////////
inline double CGraphics::getCamAng() const
{
    METHOD_ENTRY("CGraphics::getCamAngle()");

    METHOD_EXIT("CGraphics::getCamAngle()");
    return m_fCamAng;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the camera zoom
///
/// \return The camera zoom
///
///////////////////////////////////////////////////////////////////////////////
inline double CGraphics::getCamZoom() const
{
    METHOD_ENTRY("CGraphics::getCamZoom()");

    METHOD_EXIT("CGraphics::getCamZoom()");
    return m_fCamZoom;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Add vertex to linelist
///
/// The vertex is added at the current position in the linelist, specified by
/// the size-variable
///
/// \param _vecV Vertex
///
///////////////////////////////////////////////////////////////////////////////
inline void CGraphics::addVertex(const Vector2d& _vecV)
{
    METHOD_ENTRY("CGraphics::addVertex(const Vector2d&)");

    m_VertList.push_back(_vecV);
    ++m_unNrOfLines;

    METHOD_EXIT("CGraphics::addVertex(const Vector2d&)");
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Add vertex to linelist
///
/// The vertex is added at the current position in the linelist, specified by
/// the size-variable
///
/// \param _fX Vertex x-position
/// \param _fY Vertex y-position
///
///////////////////////////////////////////////////////////////////////////////
inline void CGraphics::addVertex(const double& _fX, const double& _fY)
{
    METHOD_ENTRY("CGraphics::addVertex(const double&, const double&)");

    m_VertList.push_back(Vector2d(_fX, _fY));
    ++m_unNrOfLines;

    METHOD_EXIT("CGraphics::addVertex(const double&, const double&)");
}

#endif
