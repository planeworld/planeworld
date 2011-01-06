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
/// \file       random_line.h
/// \brief      Prototype of class "CRandomLine"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2009-10-18
///
////////////////////////////////////////////////////////////////////////////////

#ifndef RANDOM_LINE_H
#define RANDOM_LINE_H

// Standard-header
#include <cmath>

// Program-header
#include "polyline.h"

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class representing a randomized polyline
///
/// This class creates a polyline. The only difference is, that the points can
/// be initialized randomly. Therefore, left- and rightmost points are selected
/// within a given interval. Recursively points are inserted on positions in
/// between, with a height, that decreases with every recursive level. In this
/// way, landscapes can be drawn with a given smoothness.
///
////////////////////////////////////////////////////////////////////////////////
class CRandomLine : public CPolyLine
{
    
    public:
    
        //--- Constructor/Destructor -----------------------------------------//
        CRandomLine();
        virtual ~CRandomLine();
        
        //--- Constant Methods -----------------------------------------------//
        void draw() const;

        //--- Methods --------------------------------------------------------//
        void init(const double&, const double&, const double&, const double&,
                    const double&, const int&, const int&);
        void transform(const double&, const Vector2d&);

    protected:
    
        //--- Protected Variables --------------------------------------------//
        double  m_fLeft0;                   ///< Initial left border
        double  m_fRight0;                  ///< Initial right border
        double  m_fLeft;                    ///< Left border
        double  m_fRight;                   ///< Right border

        double  m_fHeightMax0;              ///< Initial maximum height
        double  m_fHeightMin0;              ///< Initial minimum height
        double  m_fHeightMax;               ///< Maximum height
        double  m_fHeightMin;               ///< Minimum height
        
        //--- Protected Methods ----------------------------------------------//
        void subdivide(const double&, const double&, const double&,
                        const double&, const double&, const int&,
                        std::list<Vector2d>::iterator);
        double randomDouble(const double&, const double&);

};

#endif
