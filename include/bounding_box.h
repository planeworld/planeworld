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
/// \file       bounding_box.h
/// \brief      Prototype of class "CBoundingBox"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2010-04-15
///
////////////////////////////////////////////////////////////////////////////////

#ifndef BOUNDING_BOX_H
#define BOUNDING_BOX_H

//--- Standard header --------------------------------------------------------//

//--- Program header ---------------------------------------------------------//
#include "log.h"

//--- Misc header ------------------------------------------------------------//
#include "eigen2/Eigen/Core"

using namespace Eigen;

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class representing an Axis Aligned Bounding Box
/// 
////////////////////////////////////////////////////////////////////////////////
class CBoundingBox
{
    
    public:
    
        //--- Constructor/Destructor -----------------------------------------//
        CBoundingBox();
        
        //--- Constant Methods -----------------------------------------------//
        const Vector2d& getLowerLeft() const;
        const Vector2d& getUpperRight() const;
        const double    getHeight() const;
        const double    getWidth() const;
        
        bool isInside(const Vector2d&) const;
        bool overlaps(const CBoundingBox&) const;

        //--- Methods --------------------------------------------------------//
        void setLowerLeft(const Vector2d&);
        void setUpperRight(const Vector2d&);
        
        void update(const CBoundingBox&);
        void update(const Vector2d&);

    protected:

        //--- Protected Variables --------------------------------------------//
        Vector2d m_vecLowerLeft;
        Vector2d m_vecUpperRight;
        
};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the lower left corner of Axis Aligned Bounding Box 
///
/// \return Returns lower left of bounding box
///
////////////////////////////////////////////////////////////////////////////////
inline const Vector2d& CBoundingBox::getLowerLeft() const
{
    METHOD_ENTRY("CBoundingBox::getLowerLeft")

    METHOD_EXIT("CBoundingBox::getLowerLeft")
    return m_vecLowerLeft;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the upper right corner of Axis Aligned Bounding Box 
///
/// \return Returns upper right of bounding box
///
////////////////////////////////////////////////////////////////////////////////
inline const Vector2d& CBoundingBox::getUpperRight() const
{
    METHOD_ENTRY("CBoundingBox::getUpperRight")

    METHOD_EXIT("CBoundingBox::getUpperRight")
    return m_vecUpperRight;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the height of Axis Aligned Bounding Box 
///
/// \return Returns height of bounding box
///
////////////////////////////////////////////////////////////////////////////////
inline const double CBoundingBox::getHeight() const
{
    METHOD_ENTRY("CBoundingBox::getHeight")

    METHOD_EXIT("CBoundingBox::getHeight")
    return m_vecUpperRight[1] - m_vecLowerLeft[1];
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the width of Axis Aligned Bounding Box 
///
/// \return Returns width of bounding box
///
////////////////////////////////////////////////////////////////////////////////
inline const double CBoundingBox::getWidth() const
{
    METHOD_ENTRY("CBoundingBox::getWidth")

    METHOD_EXIT("CBoundingBox::getWidth")
    return m_vecUpperRight[0] - m_vecLowerLeft[0];
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets the lower left corner of Axis Aligned Bounding Box 
///
/// \param _vecLL Lower left of bounding box
///
////////////////////////////////////////////////////////////////////////////////
inline void CBoundingBox::setLowerLeft(const Vector2d& _vecLL)
{
    METHOD_ENTRY("CBoundingBox::setLowerLeft")
    
    m_vecLowerLeft = _vecLL;

    METHOD_EXIT("CBoundingBox::setLowerLeft")
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets the upper right corner of Axis Aligned Bounding Box 
///
/// \param _vecUR Upper right of bounding box
///
////////////////////////////////////////////////////////////////////////////////
inline void CBoundingBox::setUpperRight(const Vector2d& _vecUR)
{
    METHOD_ENTRY("CBoundingBox::setUpperRight")
    
    m_vecUpperRight = _vecUR;

    METHOD_EXIT("CBoundingBox::setUpperRight")
}

#endif
