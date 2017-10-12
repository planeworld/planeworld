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
/// \file       xfig_loader.cpp
/// \brief      Implementation of class "CXFigLoader"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2009-10-18
///
////////////////////////////////////////////////////////////////////////////////

#include "xfig_loader.h"

#include "circle.h"
#include "polygon.h"

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Load data from XFig file
///
////////////////////////////////////////////////////////////////////////////////
void CXFigLoader::load(const std::string& _strFilename)
{
    METHOD_ENTRY("CXFigLoader::load")

    CCircle*            pCircle;
    CPolygon*           pPolygon;
    CParzival           File;
    std::string         strTmp;

    // Clear the list of possible last call
    m_Shapes.clear();
    
    //Open file if present
    if (!File.open(_strFilename))
    {
        WARNING_MSG("XFig Loader", "No Shape loaded.")
    };

    //--- Read the header, should be: #FIG X.Y -------------------------------//
    if (File.readString() != "#FIG")
    {
        ERROR_MSG("XFig Loader", "File: " << _strFilename << " doesn't seem to be a valid xfig-file.")
    }
    else
    {
        strTmp=File.readString();
        DEBUG_MSG("XFig Loader", "XFig Version " << strTmp << ".")
        strTmp=File.readLine();
    }
    strTmp=File.readString();
    DOM_VAR(DEBUG_MSG("XFig Loader", "Ignoring orientation: " << strTmp))
    strTmp=File.readString();
    DOM_VAR(DEBUG_MSG("XFig Loader", "Ignoring justification: " << strTmp))
    strTmp=File.readString();
    DOM_VAR(DEBUG_MSG("XFig Loader", "Ignoring units: " << strTmp))
    strTmp=File.readString();
    DOM_VAR(DEBUG_MSG("XFig Loader", "Ignoring papersize: " << strTmp))
    strTmp=File.readString();
    DOM_VAR(DEBUG_MSG("XFig Loader", "Ignoring magnification: " << strTmp))
    strTmp=File.readString();
    DOM_VAR(DEBUG_MSG("XFig Loader", "Ignoring multiple-page: " << strTmp))
    strTmp=File.readString();
    DOM_VAR(DEBUG_MSG("XFig Loader", "Ignoring transparent color: " << strTmp))
    strTmp=File.readLine();
    DOM_VAR(DEBUG_MSG("XFig Loader", "Ignoring resolution and coordinate system: "))
    File.readString(); File.readString();

    //--- Read the objects, mainloop -----------------------------------------//
    int nCase;
    int nCenterX = 0;
    int nCenterY = 0;
    int nRadiusX = 1;
    int nNPoints = 0;
    int nX, nY;
    
    while (!File.getStream().eof())
    {
        nCase = File.readInt();
        switch (nCase)
        {
            case 1:
                DOM_VAR(DEBUG_BLK(
                    double fTmp;
                    int nTmp;
                    int nDepth;
                    DEBUG_MSG("XFig Loader", "Ellipse")
                    nTmp=File.readInt();
                    DEBUG_MSG("XFig Loader", "Ellipse, ignoring subtype: " << nTmp)
                    nTmp=File.readInt();
                    DEBUG_MSG("XFig Loader", "Ellipse, ignoring linestyle: " << nTmp)
                    nTmp=File.readInt();
                    DEBUG_MSG("XFig Loader", "Ellipse, ignoring thickness: " << nTmp)
                    nTmp=File.readInt();
                    DEBUG_MSG("XFig Loader", "Ellipse, ignoring pencolor: " << nTmp)
                    nTmp=File.readInt();
                    DEBUG_MSG("XFig Loader", "Ellipse, ignoring fillcolor: " << nTmp)
                    nDepth = File.readInt();
                    DEBUG_MSG("XFig Loader", "Ellipse, depth: " << nDepth)
                    nTmp=File.readInt();
                    DEBUG_MSG("XFig Loader", "Ellipse, ignoring penstyle: " << nTmp)
                    nTmp=File.readInt();
                    DEBUG_MSG("XFig Loader", "Ellipse, ignoring area_fill: " << nTmp)
                    fTmp=File.readDouble();
                    DEBUG_MSG("XFig Loader", "Ellipse, ignoring style_val: " << fTmp)
                    nTmp=File.readInt();
                    DEBUG_MSG("XFig Loader", "Ellipse, ignoring direction: " << nTmp)
                    fTmp=File.readDouble();
                    DEBUG_MSG("XFig Loader", "Ellipse, ignoring angle: " << fTmp)
                    nCenterX = File.readInt();
                    DEBUG_MSG("XFig Loader", "Ellipse, center_x: " << nCenterX)
                    nCenterY = File.readInt();
                    DEBUG_MSG("XFig Loader", "Ellipse, center_y: " << nCenterY)
                    nRadiusX = File.readInt();
                    DEBUG_MSG("XFig Loader", "Ellipse, radius_x: " << nRadiusX)
                    nTmp = File.readInt();
                    DEBUG_MSG("XFig Loader", "Ellipse, radius_y: " << nTmp)
                    nTmp=File.readInt();
                    DEBUG_MSG("XFig Loader", "Ellipse, ignoring start_x: " << nTmp)
                    nTmp=File.readInt();
                    DEBUG_MSG("XFig Loader", "Ellipse, ignoring start_y: " << nTmp)
                    nTmp=File.readInt();
                    DEBUG_MSG("XFig Loader", "Ellipse, ignoring end_x: " << nTmp)
                    nTmp=File.readInt();
                    DEBUG_MSG("XFig Loader", "Ellipse, ignoring end_y: " << nTmp)
                ))
                
                // Add object to shapelist
                pCircle = new CCircle;
                MEM_ALLOC("pCircle")
                pCircle->setRadius(double(nRadiusX)/100.0);
                pCircle->setCenter(double(nCenterX)/100.0, double(-nCenterY)/100.0);
                pCircle->setDepths(SHAPE_DEPTH_ALL);
                m_Shapes.push_back(pCircle);
                
                break;
            case 2:
                DOM_VAR(DEBUG_BLK(
                    double fTmp;
                    int nTmp;
                    int nDepth;
                    DEBUG_MSG("XFig Loader", "Polygon")
                    nTmp=File.readInt();
                    DEBUG_MSG("XFig Loader", "Polygon, ignoring subtype: " << nTmp)
                    nTmp=File.readInt();
                    DEBUG_MSG("XFig Loader", "Polygon, ignoring linestyle: " << nTmp)
                    nTmp=File.readInt();
                    DEBUG_MSG("XFig Loader", "Polygon, ignoring thickness: " << nTmp)
                    nTmp=File.readInt();
                    DEBUG_MSG("XFig Loader", "Polygon, ignoring pencolor: " << nTmp)
                    nTmp=File.readInt();
                    DEBUG_MSG("XFig Loader", "Polygon, ignoring fillcolor: " << nTmp)
                    nDepth = File.readInt();
                    DEBUG_MSG("XFig Loader", "Polygon, depth: " << nDepth)
                    nTmp=File.readInt();
                    DEBUG_MSG("XFig Loader", "Polygon, ignoring penstyle: " << nTmp)
                    nTmp=File.readInt();
                    DEBUG_MSG("XFig Loader", "Polygon, ignoring area_fill: " << nTmp)
                    fTmp=File.readDouble();
                    DEBUG_MSG("XFig Loader", "Polygon, ignoring style_val: " << fTmp)
                    nTmp=File.readInt();
                    DEBUG_MSG("XFig Loader", "Polygon, ignoring join_style: " << nTmp)
                    nTmp=File.readInt();
                    DEBUG_MSG("XFig Loader", "Polygon, ignoring cap_style: " << nTmp)
                    nTmp=File.readInt();
                    DEBUG_MSG("XFig Loader", "Polygon, ignoring radius: " << nTmp)
                    nTmp=File.readInt();
                    DEBUG_MSG("XFig Loader", "Polygon, ignoring forward_arrow: " << nTmp)
                    nTmp=File.readInt();
                    DEBUG_MSG("XFig Loader", "Polygon, ignoring backward_arrow: " << nTmp)
                    nNPoints = File.readInt();
                    DEBUG_MSG("XFig Loader", "Polygon, number of points: " << nNPoints)
                ))
                pPolygon = new CPolygon;
                MEM_ALLOC("pPolygon")
                pPolygon->setPolygonType(PolygonType::LINE_LOOP);
                pPolygon->setDepths(SHAPE_DEPTH_ALL);
                for (int i=0; i<nNPoints; ++i)
                {
                    nX = File.readInt();
                    nY = File.readInt();
                    DOM_VAR(DEBUG_MSG("XFig Loader", "Polygon, point " << i+1 << ": " << nX << "," << nY))
                    pPolygon->addVertex(Vector2d(double(nX)/100.0,double(-nY)/100.0));
                }
                m_Shapes.push_back(pPolygon);
                break;
            default:
                DEBUG_MSG("XFig Loader", "Ignoring unknown shape. ")
                break;
        }
//      nTmp=File.readInt();
    }

    File.close();
}
