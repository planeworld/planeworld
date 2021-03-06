////////////////////////////////////////////////////////////////////////////////
//
// This file is part of planeworld, a 2D simulation of physics and much more.
// Copyright (C) 2010-2018 Torsten Büschenfeld
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
////////////////////////////////////////////////////////////////////////////////

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
CBoundingBox::CBoundingBox() : IGridUser()
{
    METHOD_ENTRY("CBoundingBox::CBoundingBox")
    CTOR_CALL("CBoundingBox::CBoundingBox")
    
    m_vecLowerLeft.setZero();
    m_vecUpperRight.setZero();
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
        return true;
    }
    else
    {
        return false;
    }
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Tests two bounding boxes for overlap
///
/// This methods tests two bounding boxes for overlap. The second parameter
/// steers behaviour in the universe grid. The camera must be always tested
/// for overlap, independently of the cell. But physical objects, that may not
/// be larger than one cell, must be located in neighbouring cells or the same
/// cell to overlap. The second parameter sets this limit, negative values
/// will always test for overlap, the case of neighbouring cells implies 1 for
/// the cell limit.
///
/// \param _BBox Second bounding box for overlap test
/// \param _nCellLimit If cells differ too much, bounding boxes are not tested
///                    for overlap.
///
/// \return Do the bounding boxes overlap?
///
///////////////////////////////////////////////////////////////////////////////
bool CBoundingBox::overlaps(const CBoundingBox& _BBox,
                            const int& _nCellLimit) const
{
    METHOD_ENTRY("CBoundingBox::overlaps")

    // Test for overlapping bounding boxes
    if ((std::abs(m_vecCell[0] - _BBox.m_vecCell[0]) <= _nCellLimit &&
         std::abs(m_vecCell[1] - _BBox.m_vecCell[1]) <= _nCellLimit) ||
                                         _nCellLimit <  0)             
    {
    
        if ((
            std::abs(
                ((m_vecLowerLeft[0]+m_vecUpperRight[0]) / 2.0) -
                ((_BBox.getLowerLeft()[0]+_BBox.getUpperRight()[0]) / 2.0)+
                IGridUser::cellToDouble(m_vecCell-_BBox.m_vecCell)[0]
            )
            <
            (
                (std::abs(m_vecUpperRight[0]-m_vecLowerLeft[0]) / 2.0) +
                (std::abs(_BBox.getUpperRight()[0]-_BBox.getLowerLeft()[0]) / 2.0))
            )
            &&
            (
            std::abs(
                ((m_vecLowerLeft[1]+m_vecUpperRight[1]) / 2.0) -
                ((_BBox.getLowerLeft()[1]+_BBox.getUpperRight()[1]) / 2.0)+
                IGridUser::cellToDouble(m_vecCell-_BBox.m_vecCell)[1]
            )
            <
            (
                (std::abs(m_vecUpperRight[1]-m_vecLowerLeft[1]) / 2.0) +
                (std::abs(_BBox.getUpperRight()[1]-_BBox.getLowerLeft()[1]) / 2.0))
            )
            )
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else return false;
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
    
    m_vecCell = _BBox.m_vecCell;
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
}

SERIALIZE_IMPL(CBoundingBox,
    SERIALIZE("lower_left_x", m_vecLowerLeft[0])
    SERIALIZE("lower_left_y", m_vecLowerLeft[1])
    SERIALIZE("lower_left_x", m_vecUpperRight[0])
    SERIALIZE("lower_left_x", m_vecUpperRight[1])
    // From IGridUser:
    SERIALIZE("cell_x", m_vecCell[0])
    SERIALIZE("cell_y", m_vecCell[1])
)

    
