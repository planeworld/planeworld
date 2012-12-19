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
/// \file       universe_scaled.h
/// \brief      Prototype of interface "IUniverseScaled"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2012-12-17
///
////////////////////////////////////////////////////////////////////////////////

#ifndef UNIVERSE_SCALED_H
#define UNIVERSE_SCALED_H

//--- Program header ---------------------------------------------------------//
#include "engine_common.h"
#include "log.h"

//--- Standard header --------------------------------------------------------//

//--- Misc header ---....-----------------------------------------------------//
#include "eigen2/Eigen/Geometry"

using namespace Eigen;

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Base class for everything that is in the universe scale.
///
/// This abstract class provides all basic methods and variables for objects
/// that are freely located within a whole universe. This mainly means a cell
/// in the universe grid and methods to have access to the cell.
/// 
////////////////////////////////////////////////////////////////////////////////
class IUniverseScaled
{
    
    public:
    
        //--- Constructor/Destructor -----------------------------------------//
        IUniverseScaled(){m_vecCell.setZero();}
        virtual ~IUniverseScaled(){};
        
        //--- Static methods -------------------------------------------------//
        static const Vector2d   cellToDouble(const Vector2i&);

        //--- Constant methods -----------------------------------------------//
        virtual const Vector2i  getCell() const;

        //--- Methods --------------------------------------------------------//
        virtual void            setCell(const Vector2i&);

    protected:

        //-- Variables [protected] -------------------------------------------//
        Vector2i                m_vecCell;  ///< Cell position in large scale grid

};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the grid cell as position in double values
///
/// The universe is organised in cells that hold a double valued position. The
/// cell is represented by an integer position index. This methods returns a
/// given cell as a double position
///
/// \param _vecCell Cell to be returned as a double position
///
/// \return The grid cell as position in double value
///
////////////////////////////////////////////////////////////////////////////////
inline const Vector2d IUniverseScaled::cellToDouble(const Vector2i& _vecCell)
{
    METHOD_ENTRY("IUniverseScaled::cellToDouble")
    return _vecCell.cast<double>() * DEFAULT_CELL_SIZE_2;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the grid cell of entity
///
/// The universe is organised in cells that hold a double valued position. The
/// cell is represented by an integer position index.
///
/// \return The grid cell of the entity
///
////////////////////////////////////////////////////////////////////////////////
inline const Vector2i IUniverseScaled::getCell() const
{
    METHOD_ENTRY("IUniverseScaled::getCell")
    return m_vecCell;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Set the grid cell of the entity
///
/// The universe is organised in cells that hold a double valued position. The
/// cell is represented by an integer position index.
///
/// \param _vecCell Grid cell of the entity
///
////////////////////////////////////////////////////////////////////////////////
inline void IUniverseScaled::setCell(const Vector2i& _vecCell)
{
    METHOD_ENTRY("IUniverseScaled::setCell")

    m_vecCell = _vecCell;
}


#endif
