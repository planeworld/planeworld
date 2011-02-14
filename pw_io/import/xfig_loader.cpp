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
/// \file       xfig_loader.cpp
/// \brief      Implementation of class "CXFigLoader"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2009-10-18
///
////////////////////////////////////////////////////////////////////////////////

#include "xfig_loader.h"

////////////////////////////////////////////////////////////////////////////////
///
/// \brief constructor
///
////////////////////////////////////////////////////////////////////////////////
CXFigLoader::CXFigLoader()
{
    METHOD_ENTRY("CXFigLoader::CXFigLoader()")
    CTOR_CALL("CXFigLoader::CXFigLoader")
    METHOD_EXIT("CXFigLoader::CXFigLoader")
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Load data from XFig file
///
////////////////////////////////////////////////////////////////////////////////
void CXFigLoader::load(const std::string& _strFilename)
{
    METHOD_ENTRY("CXFigLoader::load")

    CCircle*            pCircle;
    CCircleVisuals*     pCircleVisuals;
    CPolyLine*          pPolyLine;
    CPolylineVisuals*   pPolylineVisuals;
    CParzival           File;
    std::string         strTmp;

    // Clear the list of possible last call
    m_ShapeList.clear();
    
    //Open file if present
    if (!File.open(_strFilename))
    {
        WARNING_MSG("XFig Loader", "No Shape loaded.")
        METHOD_EXIT("CXFigLoader::load")
    };

    //--- Read the header, should be: #FIG X.Y -------------------------------//
    if (File.readString() != "#FIG")
    {
        ERROR_MSG("XFig Loader", "File: " << _strFilename << " doesn't seem to be a valid xfig-file.")
        METHOD_EXIT("CXFigLoader::load")
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
    double fTmp;
    int nTmp;
    int nCase;
    int nDepth;
    int nNPoints;
    int nX, nY;
    
    while (!File.getStream().eof())
    {
        nCase = File.readInt();
        switch (nCase)
        {
            case 1:
                {
                DEBUG_MSG("XFig Loader", "Ellipse")
                nTmp=File.readInt();
                DOM_VAR(DEBUG_MSG("XFig Loader", "Ellipse, ignoring subtype: " << nTmp))
                nTmp=File.readInt();
                DOM_VAR(DEBUG_MSG("XFig Loader", "Ellipse, ignoring linestyle: " << nTmp))
                nTmp=File.readInt();
                DOM_VAR(DEBUG_MSG("XFig Loader", "Ellipse, ignoring thickness: " << nTmp))
                nTmp=File.readInt();
                DOM_VAR(DEBUG_MSG("XFig Loader", "Ellipse, ignoring pencolor: " << nTmp))
                nTmp=File.readInt();
                DOM_VAR(DEBUG_MSG("XFig Loader", "Ellipse, ignoring fillcolor: " << nTmp))
                nDepth = File.readInt();
                DOM_VAR(DEBUG_MSG("XFig Loader", "Ellipse, depth: " << nDepth))
                nTmp=File.readInt();
                DOM_VAR(DEBUG_MSG("XFig Loader", "Ellipse, ignoring penstyle: " << nTmp))
                nTmp=File.readInt();
                DOM_VAR(DEBUG_MSG("XFig Loader", "Ellipse, ignoring area_fill: " << nTmp))
                fTmp=File.readDouble();
                DOM_VAR(DEBUG_MSG("XFig Loader", "Ellipse, ignoring style_val: " << fTmp))
                nTmp=File.readInt();
                DOM_VAR(DEBUG_MSG("XFig Loader", "Ellipse, ignoring direction: " << nTmp))
                fTmp=File.readDouble();
                DOM_VAR(DEBUG_MSG("XFig Loader", "Ellipse, ignoring angle: " << fTmp))
                int nCenterX = File.readInt();
                DOM_VAR(DEBUG_MSG("XFig Loader", "Ellipse, center_x: " << nCenterX))
                int nCenterY = File.readInt();
                DOM_VAR(DEBUG_MSG("XFig Loader", "Ellipse, center_y: " << nCenterY))
                int nRadiusX = File.readInt();
                DOM_VAR(DEBUG_MSG("XFig Loader", "Ellipse, radius_x: " << nRadiusX))
                nTmp = File.readInt();
                DOM_VAR(DEBUG_MSG("XFig Loader", "Ellipse, radius_y: " << nTmp))
                nTmp=File.readInt();
                DOM_VAR(DEBUG_MSG("XFig Loader", "Ellipse, ignoring start_x: " << nTmp))
                nTmp=File.readInt();
                DOM_VAR(DEBUG_MSG("XFig Loader", "Ellipse, ignoring start_y: " << nTmp))
                nTmp=File.readInt();
                DOM_VAR(DEBUG_MSG("XFig Loader", "Ellipse, ignoring end_x: " << nTmp))
                nTmp=File.readInt();
                DOM_VAR(DEBUG_MSG("XFig Loader", "Ellipse, ignoring end_y: " << nTmp))

                // Add object to shapelist
                pCircle = new CCircle;
                MEM_ALLOC("pCircle")
                pCircleVisuals = new CCircleVisuals(pCircle);
                MEM_ALLOC("pCircleVisuals")
                pCircle->setRadius(double(nRadiusX)/100.0);
                pCircle->setCenter(double(nCenterX)/100.0, double(-nCenterY)/100.0);
                pCircle->setDepths(SHAPE_DEPTH_ALL);
                m_ShapeList.push_back(pCircle);
                m_VisualsList.push_back(pCircleVisuals);
                }
                break;
            case 2:
                DEBUG_MSG("XFig Loader", "Polyline")
                nTmp=File.readInt();
                DOM_VAR(DEBUG_MSG("XFig Loader", "Polyline, ignoring subtype: " << nTmp))
                nTmp=File.readInt();
                DOM_VAR(DEBUG_MSG("XFig Loader", "Polyline, ignoring linestyle: " << nTmp))
                nTmp=File.readInt();
                DOM_VAR(DEBUG_MSG("XFig Loader", "Polyline, ignoring thickness: " << nTmp))
                nTmp=File.readInt();
                DOM_VAR(DEBUG_MSG("XFig Loader", "Polyline, ignoring pencolor: " << nTmp))
                nTmp=File.readInt();
                DOM_VAR(DEBUG_MSG("XFig Loader", "Polyline, ignoring fillcolor: " << nTmp))
                nDepth = File.readInt();
                DOM_VAR(DEBUG_MSG("XFig Loader", "Polyline, depth: " << nDepth))
                nTmp=File.readInt();
                DOM_VAR(DEBUG_MSG("XFig Loader", "Polyline, ignoring penstyle: " << nTmp))
                nTmp=File.readInt();
                DOM_VAR(DEBUG_MSG("XFig Loader", "Polyline, ignoring area_fill: " << nTmp))
                fTmp=File.readDouble();
                DOM_VAR(DEBUG_MSG("XFig Loader", "Polyline, ignoring style_val: " << fTmp))
                nTmp=File.readInt();
                DOM_VAR(DEBUG_MSG("XFig Loader", "Polyline, ignoring join_style: " << nTmp))
                nTmp=File.readInt();
                DOM_VAR(DEBUG_MSG("XFig Loader", "Polyline, ignoring cap_style: " << nTmp))
                nTmp=File.readInt();
                DOM_VAR(DEBUG_MSG("XFig Loader", "Polyline, ignoring radius: " << nTmp))
                nTmp=File.readInt();
                DOM_VAR(DEBUG_MSG("XFig Loader", "Polyline, ignoring forward_arrow: " << nTmp))
                nTmp=File.readInt();
                DOM_VAR(DEBUG_MSG("XFig Loader", "Polyline, ignoring backward_arrow: " << nTmp))
                nNPoints = File.readInt();
                DOM_VAR(DEBUG_MSG("XFig Loader", "Polyline, number of points: " << nNPoints))

                pPolyLine = new CPolyLine;
                MEM_ALLOC("pPolyline")
                pPolylineVisuals = new CPolylineVisuals(pPolyLine);
                MEM_ALLOC("pPolylineVisuals")
                pPolyLine->setLineType(GRAPHICS_LINETYPE_LOOP);
                pPolyLine->setDepths(SHAPE_DEPTH_ALL);
                for (int i=0; i<nNPoints; ++i)
                {
                    nX = File.readInt();
                    nY = File.readInt();
                    DOM_VAR(DEBUG_MSG("XFig Loader", "Polyline, point " << i+1 << ": " << nX << "," << nY))
                    pPolyLine->addVertex(Vector2d(double(nX)/100.0,double(-nY)/100.0));
                }
                m_ShapeList.push_back(pPolyLine);
                m_VisualsList.push_back(pPolylineVisuals);
                break;
            default:
                DEBUG_MSG("XFig Loader", "Ignoring unknown shape. ")
                break;
        }
//      nTmp=File.readInt();
    }

    File.close();

    METHOD_EXIT("CXFigLoader::load")
}