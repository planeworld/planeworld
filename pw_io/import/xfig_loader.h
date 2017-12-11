////////////////////////////////////////////////////////////////////////////////
//
// This file is part of planeworld, a 2D simulation of physics and much more.
// Copyright (C) 2009-2017 Torsten Büschenfeld
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

//--- Standard header --------------------------------------------------------//
#include <vector>

//--- Program header ---------------------------------------------------------//
#include "parzival.h"
#include "shape.h"

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

        //--- Constant Operators ---------------------------------------------//
        
        //--- Operators ------------------------------------------------------//
        
        //--- Constant Methods -----------------------------------------------//

        //--- Methods --------------------------------------------------------//
        std::vector<IShape*>* getShapes();
        
        void load(const std::string&);

        //--- friends --------------------------------------------------------//

    private:

        //--- Private Variables ----------------------------------------------//
        std::vector<IShape*>      m_Shapes; ///< Stores the shapes
};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the shapes
///
/// \return Shape list
///
////////////////////////////////////////////////////////////////////////////////
inline std::vector<IShape*>* CXFigLoader::getShapes()
{
    METHOD_ENTRY("CXFigLoader::getShapes")
    return &m_Shapes;
}

#endif
