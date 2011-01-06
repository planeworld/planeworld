/*
Copyright (c) 2006-2007 Torsten Büschenfeld
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the documentation
      and/or other materials provided with the distribution.
    * Neither the name Torsten Büschenfeld nor the names of his contributors
      may be used to endorse or promote products derived from this software
      without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

////////////////////////////////////////////////////////////////////////////////
///
/// \file		graphics.h
/// \brief		prototype of class "CGraphics"
///
/// \author		bfeld
///
////////////////////////////////////////////////////////////////////////////////
#ifndef GRAPHICS_H
#define GRAPHICS_H

// Standard-header
#include <list>

// Program-header
#include "log.h"
#include "vector2d.h"
#include "vector3d.h"

// Misc-header
#include <QtOpenGL>

// Constants
const double GRAPHICS_FOV_DEFAULT = 45.0;				///< default fov
const double GRAPHICS_LEFT_DEFAULT = -160.0;			///< Default left plane
const double GRAPHICS_RIGHT_DEFAULT = 160.0;			///< Default right plane
const double GRAPHICS_BOTTOM_DEFAULT = -120.0;			///< Default bottom plane
const double GRAPHICS_TOP_DEFAULT = 120.0;				///< Default top plane
const double GRAPHICS_NEAR_DEFAULT = 0.1;				///< Default min depth
const double GRAPHICS_FAR_DEFAULT = 1000.0;				///< Default max depth

const double GRAPHICS_DYN_PEL_SIZE_DEFAULT = 10.0;		///< Default size of dyn-objects

const double GRAPHICS_PI = std::acos(-1.0);				///< Constant pi
const double GRAPHICS_DEG2RAD = GRAPHICS_PI / 180.0;	///< Converts degree to radiant values
const double GRAPHICS_RAD2DEG = GRAPHICS_PI * 180.0;	///< Converts radiant to degree values
const unsigned short GRAPHICS_WIDTH_DEFAULT = 1024u;	///< Default x-resolution
const unsigned short GRAPHICS_HEIGHT_DEFAULT = 768u;	///< Default y-resolution

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
/// CShape.
///
/// \todo Implement frustum culling
/// \todo Perhaps moving the camera towards z-axis is better than scaling, see
///			frustum culling.
///
////////////////////////////////////////////////////////////////////////////////
class CGraphics : virtual public CLogBase
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
		CVector2D		screen2World(const CVector2D&) const;
		CVector2D		screen2World(const double&, const double&) const;
		double			getDynPelSize() const;
		double			getResMPX() const;
		double			getResMPY() const;
		double			getResPMX() const;
		double			getResPMY() const;
		unsigned short	getWidthScr() const;
		unsigned short	getHeightScr() const;
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
		CVector2D	getCamPos() const;
		double		getCamAng() const;
		double		getCamZoom() const;
		
		//--- Methods --------------------------------------------------------//
		void applyCamMovement();
		void resetCam();
		void rotCamBy(const double&);
		void rotCamTo(const double&);
		void transCamBy(const CVector3D&);
		void transCamTo(const CVector3D&);
		void zoomCamBy(const double&);
		void zoomCamTo(const double&);

		//
		//--- Methods for drawing --------------------------------------------//
		//

		//--- Constant methods -----------------------------------------------//
		void circle(const CVector3D&, const double&) const;
		void dot(const CVector2D&) const;
		void showVec(const CVector2D&, const CVector2D&) const;

		//--- Methods --------------------------------------------------------//
		void addVertex(const CVector2D&);
		void addVertex(const double&, const double&);
		void beginLine(const LineType&, const double&);
		void endLine();

		
	private:
		
		//--- Variabels [private] --------------------------------------------//
		CVector3D				m_vecCamPos;	///< camera position
		double					m_fCamAng;		///< camera angle
		double					m_fCamZoom;		///< camera zoom
		double					m_fDepthMax;	///< maximum depth of levels
		double					m_fDepthMin;	///< minimum depth of levels

		double					m_fDynPelSize;	///< pel-size for dynamically sized shapes
		
		int						m_nVideoFlags;	///< videoflags like fullscreen, resolution
		unsigned short			m_unWidthScr;	///< Screen width
		unsigned short			m_unHeightScr;	///< Screen height

		unsigned int			m_unNrOfLines;	///< Total number of lines drawn
		
		std::list<CVector2D>	m_VertList;		///< list, containing the coordinates of vertices
		double					m_fDepth;		///< depth of lines in list
		

		//--- Constructor/destructor [private] -------------------------------//
		CGraphics();
		~CGraphics(){};

		//--- Operators [private] --------------------------------------------//
		CGraphics& operator=(const CGraphics&);	///< empty private operator=

		//--- Methods [private] ----------------------------------------------//
		void glSetPerspective(	const GLdouble&, const GLdouble&,
								const GLdouble&, const GLdouble&,
								const GLdouble&) const;

};

//--- Implementation goes here for inline reasons ----------------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Base class for graphics
///
/// This class is the base class for all classes using graphics. It just defines
/// a constructor that initializes the meyers-singleton for the graphics
/// instance.
/// 
////////////////////////////////////////////////////////////////////////////////
class CGraphicsBase : virtual public CLogBase
{
	protected:
		//--- Protected constructor ------------------------------------------//
		CGraphicsBase():m_Graphics(CGraphics::getInstance())
		{
			CTOR_CALL(m_Log, "GraphicsBase");
		};

		//--- Protected variables --------------------------------------------//
		CGraphics&	m_Graphics;					///< Instance of graphics class
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
	METHOD_ENTRY(m_Log, "CGraphics::getDynPelSize()");

	METHOD_EXIT(m_Log, "CGraphics::getDynPelSize()");
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
	METHOD_ENTRY(m_Log, "CGraphics::getResMPX()");

	METHOD_EXIT(m_Log, "CGraphics::getResMPX()");
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
	METHOD_ENTRY(m_Log, "CGraphics::getResMPY()");

	METHOD_EXIT(m_Log, "CGraphics::getResMPY()");
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
	METHOD_ENTRY(m_Log, "CGraphics::getWidthScr()");

	METHOD_EXIT(m_Log, "CGraphics::getWidthScr()");
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
	METHOD_ENTRY(m_Log, "CGraphics::getHeightScr()");

	METHOD_EXIT(m_Log, "CGraphics::getHeightScr()");
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
	METHOD_ENTRY(m_Log, "CGraphics::setColor(const double&, const double&, const double&)");

	glColor3d(_fR, _fG, _fB);

	METHOD_EXIT(m_Log, "CGraphics::setColor(const double&, const double&, const double&)");
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
	METHOD_ENTRY(m_Log, "CGraphics::setColor(const double&, const double&, const double&, const double&)");

	glColor4d(_fR, _fG, _fB, _fA);

	METHOD_EXIT(m_Log, "CGraphics::setColor(const double&, const double&, const double&, const double&)");
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
	METHOD_ENTRY(m_Log, "CGraphics::setWidthScr(const unsigned short&)");

	m_unWidthScr = _unWidthScr;

	METHOD_EXIT(m_Log, "CGraphics::setWidthScr(const unsigned short&)");
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
	METHOD_ENTRY(m_Log, "CGraphics::setHeightScr(const unsigned short&)");

	m_unHeightScr = _unHeightScr;

	METHOD_EXIT(m_Log, "CGraphics::setHeightScr(const unsigned short&)");
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the camera angle
///
/// \return The camera angle
///
///////////////////////////////////////////////////////////////////////////////
inline CVector2D CGraphics::getCamPos() const
{
	METHOD_ENTRY(m_Log, "CGraphics::getCamPos()");

	METHOD_EXIT(m_Log, "CGraphics::getCamPos()");
	return CVector2D(m_vecCamPos[0], m_vecCamPos[1]);
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
	METHOD_ENTRY(m_Log, "CGraphics::getCamAngle()");

	METHOD_EXIT(m_Log, "CGraphics::getCamAngle()");
	return m_fCamAng*GRAPHICS_DEG2RAD;
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
	METHOD_ENTRY(m_Log, "CGraphics::getCamZoom()");

	METHOD_EXIT(m_Log, "CGraphics::getCamZoom()");
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
inline void CGraphics::addVertex(const CVector2D& _vecV)
{
	METHOD_ENTRY(m_Log, "CGraphics::addVertex(const CVector2D&)");

	m_VertList.push_back(_vecV);
	++m_unNrOfLines;

	METHOD_EXIT(m_Log, "CGraphics::addVertex(const CVector2D&)");
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
	METHOD_ENTRY(m_Log, "CGraphics::addVertex(const double&, const double&)");

	m_VertList.push_back(CVector2D(_fX, _fY));
	++m_unNrOfLines;

	METHOD_EXIT(m_Log, "CGraphics::addVertex(const double&, const double&)");
}

#endif
