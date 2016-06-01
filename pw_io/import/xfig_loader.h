////////////////////////////////////////////////////////////////////////////////
//
// This file is part of planeworld, a 2D simulation of physics and much more.
// Copyright (C) 2009-2016 Torsten Büschenfeld
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
/// \file       xfig_loader.h
/// \brief      prototype of class "CXFigLoader"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2009-10-18
///
/// \todo Adjust units when loading
///
////////////////////////////////////////////////////////////////////////////////

#ifndef XFIG_LOADER_H
#define XFIG_LOADER_H

// standard-header
// #include <fstream>
// #include <iostream>
// #include <list>
// #include <string>

// program-header
#include "circle_visuals.h"
// #include "log.h"
#include "parzival.h"
#include "polygon_visuals.h"

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class for importing xfig data
///
/// This class reads data from xfig files to define the objects shapes.
///
////////////////////////////////////////////////////////////////////////////////
class CXFigLoader
{

    public:

        //--- Constructor/Destructor -----------------------------------------//
        CXFigLoader();
        ~CXFigLoader(){};

        //--- Constant Operators ---------------------------------------------//
        
        //--- Operators ------------------------------------------------------//
        
        //--- Constant Methods -----------------------------------------------//
        const std::list<IShape*>&   getShapes() const;
        const std::list<IVisuals*>& getVisuals() const;
        

        //--- Methods --------------------------------------------------------//
        void load(const std::string&);

        //--- friends --------------------------------------------------------//

    private:

        //--- Private Variables ----------------------------------------------//
        std::list<IShape*>      m_ShapeList;            ///< Stores the shapes
        std::list<IVisuals*>    m_VisualsList;          ///< Stores the visuals
};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the shapes
///
/// \return Shape list
///
////////////////////////////////////////////////////////////////////////////////
inline const std::list<IShape*>& CXFigLoader::getShapes() const
{
    METHOD_ENTRY("CXFigLoader::getShapes")

    METHOD_EXIT("CXFigLoader::getShapes")
    return m_ShapeList;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the visuals
///
/// \return Visuals list
///
////////////////////////////////////////////////////////////////////////////////
inline const std::list<IVisuals*>& CXFigLoader::getVisuals() const
{
    METHOD_ENTRY("CXFigLoader::getVisuals")

    METHOD_EXIT("CXFigLoader::getVisuals")
    return m_VisualsList;
}

#endif
