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
#include "circle.h"
#include "circle_visuals.h"
// #include "log.h"
#include "parzival.h"
#include "polyline.h"
#include "polyline_visuals.h"

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
