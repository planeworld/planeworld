////////////////////////////////////////////////////////////////////////////////
//
// This file is part of planeworld, a 2D simulation of physics and much more.
// Copyright (C) 2013-2016 Torsten Büschenfeld
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
/// \file       double_buffered_shape.h
/// \brief      Prototype of class "CDoubleBufferedShape"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2013-01-15
///
////////////////////////////////////////////////////////////////////////////////

#ifndef DOUBLE_BUFFERED_SHAPE_H
#define DOUBLE_BUFFERED_SHAPE_H

//--- Standard header --------------------------------------------------------//

//--- Program header ---------------------------------------------------------//
#include "shape.h"

//--- Misc header ------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Abstract class representing a simple shape, which is double buffered
///        for collision detection and other purposes.
///
/// This class just wraps to regular shape interfaces to hide a double buffered
/// shape from the user.
///
////////////////////////////////////////////////////////////////////////////////
class CDoubleBufferedShape : public IUniqueIDUser
{
    
    public:
    
        //--- Constructor/Destructor -----------------------------------------//
        CDoubleBufferedShape();                         ///< Constructor
        ~CDoubleBufferedShape();                        ///< Destructor
        
        //--- Constant Methods -----------------------------------------------//
        CDoubleBufferedShape* clone() const;
        
        IShape* const getShapeBuf() const;
        IShape* const getShapeCur() const;
        void          updateBuffer() const;
                        
        //--- Methods --------------------------------------------------------//
        void buffer(IShape* const);
        void swapBuffer();
        
        //--- Friends --------------------------------------------------------//
        friend std::istream& operator>>(std::istream&, CDoubleBufferedShape&);
        friend std::ostream& operator<<(std::ostream&, CDoubleBufferedShape&);

    private:

        //--- Variables [private] --------------------------------------------//
        IShape*    m_pShapeBuf;                         ///< Buffered shape
        IShape*    m_pShapeCur;                         ///< Currently active shape
};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Get buffered shape
///
/// \return Buffered shape
///
////////////////////////////////////////////////////////////////////////////////
inline IShape* const CDoubleBufferedShape::getShapeBuf() const
{
    METHOD_ENTRY("CDoubleBufferedShape::getShapeBuf")
    return m_pShapeBuf;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Get the currently active shape
///
/// \return Currently active shape
///
////////////////////////////////////////////////////////////////////////////////
inline IShape* const CDoubleBufferedShape::getShapeCur() const
{
    METHOD_ENTRY("CDoubleBufferedShape::getShapeCur")
    return m_pShapeCur;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Copy shape information to buffer
///
///////////////////////////////////////////////////////////////////////////////
inline void CDoubleBufferedShape::updateBuffer() const
{
    METHOD_ENTRY ("CDoubleBufferedShape::updateBuffer")
    
    m_pShapeBuf->copy(m_pShapeCur);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Swaps the buffer and active shape.
///
////////////////////////////////////////////////////////////////////////////////
inline void CDoubleBufferedShape::swapBuffer()
{
    METHOD_ENTRY("CDoubleBufferedShape::swapBuffer")
    std::swap(m_pShapeCur,m_pShapeBuf);
}

#endif
