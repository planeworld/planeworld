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
/// \file       visuals.cpp
/// \brief      Implementation of interface "IVisuals"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2016-01-23
///
////////////////////////////////////////////////////////////////////////////////

#include "visuals.h"

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Input stream for game state information
///
/// \param _is  Source stream
/// \param _pVis IVisuals instance to stream
///
/// \return Remaining stream with game state information
///
////////////////////////////////////////////////////////////////////////////////
std::istream& operator>>(std::istream& _is, IVisuals* const _pVis)
{
    METHOD_ENTRY("IVisuals::operator>>")
    
    std::string strTmp;
    _is >> strTmp;
    
    _is >> _pVis->m_ShpRef;
    
    return _pVis->myStreamIn(_is);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Output stream for game state information
///
/// \param _os  Source stream
/// \param _pVis IVisuals instance to stream
///
/// \return Stream with game state information of IVisuals instance
///
////////////////////////////////////////////////////////////////////////////////
std::ostream& operator<<(std::ostream& _os, IVisuals* const _pVis)
{
    METHOD_ENTRY("IShape::operator<<")

    // ShapeVisualsType has to be the first information, since visuals creation
    // when loading depends on it.
    // Cast strongly typed enum ShapeVisualsType to streamable base type
    auto nShpVisType = static_cast<std::underlying_type<ShapeVisualsType>::type>(_pVis->getShapeVisualsType());
    _os << nShpVisType << std::endl;
    
    _os << "Visuals:" << std::endl;
    _os << _pVis->m_ShpRef << std::endl;
    
    return _pVis->myStreamOut(_os);
}
