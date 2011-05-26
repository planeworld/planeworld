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
/// \file       bounding_box.cpp
/// \brief      Implementation of class "CBoundingBox"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2010-04-15
///
////////////////////////////////////////////////////////////////////////////////

#include "bounding_box.h"

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor
///
///////////////////////////////////////////////////////////////////////////////
CBoundingBox::CBoundingBox()
{
    METHOD_ENTRY("CBoundingBox::CBoundingBox")
    CTOR_CALL("CBoundingBox::CBoundingBox")
    
    m_vecLowerLeft.setZero();
    m_vecUpperRight.setZero();
    
    METHOD_EXIT("CBoundingBox::CBoundingBox")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Tests a vector to be inside of bounding box
///
/// \param _vecV Vector to test for being inside of bounding box
///
/// \return Is the vector inside?
///
///////////////////////////////////////////////////////////////////////////////
bool CBoundingBox::isInside(const Vector2d& _vecV) const
{
    METHOD_ENTRY("CBoundingBox::isInside")

    if ((m_vecLowerLeft[0] <= _vecV[0]) &&
        (m_vecLowerLeft[1] <= _vecV[1]) &&
        (m_vecUpperRight[0] >= _vecV[0]) &&
        (m_vecUpperRight[1] >= _vecV[1])
       )
    {
        METHOD_EXIT("CBoundingBox::isInside")
        return true;
    }
    else
    {
        METHOD_EXIT("CBoundingBox::isInside")
        return false;
    }
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Tests two bounding boxes for overlap
///
/// \param _BBox Second bounding box for overlap test
///
/// \return Do the bounding boxes overlap?
///
///////////////////////////////////////////////////////////////////////////////
bool CBoundingBox::overlaps(const CBoundingBox& _BBox) const
{
    METHOD_ENTRY("CBoundingBox::overlaps")

    // Test for overlapping bounding boxes
    if ((
        std::abs(
        ((m_vecLowerLeft[0]+m_vecUpperRight[0]) / 2.0) -
        ((_BBox.getLowerLeft()[0]+_BBox.getUpperRight()[0]) / 2.0))
        <
        (
        (std::abs(m_vecUpperRight[0]-m_vecLowerLeft[0]) / 2.0) +
        (std::abs(_BBox.getUpperRight()[0]-_BBox.getLowerLeft()[0]) / 2.0))
        )
        &&
        (
        std::abs(
        ((m_vecLowerLeft[1]+m_vecUpperRight[1]) / 2.0) -
        ((_BBox.getLowerLeft()[1]+_BBox.getUpperRight()[1]) / 2.0))
        <
        (
        (std::abs(m_vecUpperRight[1]-m_vecLowerLeft[1]) / 2.0) +
        (std::abs(_BBox.getUpperRight()[1]-_BBox.getLowerLeft()[1]) / 2.0))
        )
        )
    {
        METHOD_EXIT("CBoundingBox::overlaps")
        return true;
    }
    else
    {
        METHOD_EXIT("CBoundingBox::overlaps")
        return false;
    }
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Update bounding box
///
/// This method updates the bounding box by a given bounding box. The resulting
/// bounding box is the maximum size/area of both bounding boxes.
///
/// \param _BBox Bounding box used for updating
///
///////////////////////////////////////////////////////////////////////////////
void CBoundingBox::update(const CBoundingBox& _BBox)
{
    METHOD_ENTRY("CBoundingBox::update")

    if (_BBox.getLowerLeft()[0] < m_vecLowerLeft[0]) m_vecLowerLeft[0]=_BBox.getLowerLeft()[0];
    if (_BBox.getLowerLeft()[1] < m_vecLowerLeft[1]) m_vecLowerLeft[1]=_BBox.getLowerLeft()[1];
    if (_BBox.getUpperRight()[0] > m_vecUpperRight[0]) m_vecUpperRight[0]=_BBox.getUpperRight()[0];
    if (_BBox.getUpperRight()[1] > m_vecUpperRight[1]) m_vecUpperRight[1]=_BBox.getUpperRight()[1]; 
    
    METHOD_EXIT("CBoundingBox::update")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Update bounding box
///
/// This method updates the bounding box by a given point. Every dimension of
/// the point will resize the bounding box if it is outside of it.
///
/// \param _vecPoint Point used for updating
///
///////////////////////////////////////////////////////////////////////////////
void CBoundingBox::update(const Vector2d& _vecPoint)
{
    METHOD_ENTRY("CBoundingBox::update")

    if      (_vecPoint[0] < m_vecLowerLeft[0]) m_vecLowerLeft[0] = _vecPoint[0];
    else if (_vecPoint[0] > m_vecUpperRight[0]) m_vecUpperRight[0] = _vecPoint[0];
    if      (_vecPoint[1] < m_vecLowerLeft[1]) m_vecLowerLeft[1] = _vecPoint[1];
    else if (_vecPoint[1] > m_vecUpperRight[1]) m_vecUpperRight[1] = _vecPoint[1];
    
    METHOD_EXIT("CBoundingBox::update")
}
    