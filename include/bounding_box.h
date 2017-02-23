////////////////////////////////////////////////////////////////////////////////
//
// This file is part of planeworld, a 2D simulation of physics and much more.
// Copyright (C) 2010-2016 Torsten Büschenfeld
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
#include "grid_user.h"

//--- Misc header ------------------------------------------------------------//
#include <eigen3/Eigen/Core>

using namespace Eigen;

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class representing an Axis Aligned Bounding Box
/// 
////////////////////////////////////////////////////////////////////////////////
class CBoundingBox : public IGridUser
{
    
    public:
    
        //--- Constructor/Destructor -----------------------------------------//
        CBoundingBox();
        
        //--- Constant Methods -----------------------------------------------//
        const Vector2d& getLowerLeft() const;
        const Vector2d& getUpperRight() const;
        double          getHeight() const;
        double          getWidth() const;
        
        bool isInside(const Vector2d&) const;
        bool overlaps(const CBoundingBox&, const int& = -1) const;

        //--- Methods --------------------------------------------------------//
        void setLowerLeft(const Vector2d&);
        void setUpperRight(const Vector2d&);
        
        void update(const CBoundingBox&);
        void update(const Vector2d&);
        
        //--- friends --------------------------------------------------------//
        friend std::istream& operator>>(std::istream&, CBoundingBox&);
        friend std::ostream& operator<<(std::ostream&, CBoundingBox&);

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
    return m_vecUpperRight;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the height of Axis Aligned Bounding Box 
///
/// \return Returns height of bounding box
///
////////////////////////////////////////////////////////////////////////////////
inline double CBoundingBox::getHeight() const
{
    METHOD_ENTRY("CBoundingBox::getHeight")
    return m_vecUpperRight[1] - m_vecLowerLeft[1];
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the width of Axis Aligned Bounding Box 
///
/// \return Returns width of bounding box
///
////////////////////////////////////////////////////////////////////////////////
inline double CBoundingBox::getWidth() const
{
    METHOD_ENTRY("CBoundingBox::getWidth")
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
}

#endif
