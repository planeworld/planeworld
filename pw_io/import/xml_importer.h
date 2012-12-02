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
/// \file       xml_importer.h
/// \brief      Prototype of class "CXMLImporter"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2011-02-15
///
////////////////////////////////////////////////////////////////////////////////

#ifndef XML_IMPORTER_H
#define XML_IMPORTER_H

//--- Standard header --------------------------------------------------------//

//--- Program header ---------------------------------------------------------//
#include "body.h"
#include "camera.h"
#include "planet_visuals.h"
#include "polyline_visuals.h"
#include "terrain_visuals.h"

//--- Misc header ------------------------------------------------------------//
#include "pugixml.hpp"

/// Specifies the mode for importing xml data
typedef enum
{
    IMPORT_MODE_UNIVERSE,
    IMPORT_MODE_OBJECT
} ImportModeType;

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class for importing world data from xml files
///
////////////////////////////////////////////////////////////////////////////////
class CXMLImporter 
{

    public:
   
        //--- Constructor/Destructor -----------------------------------------//
        CXMLImporter();
        ~CXMLImporter();

        //--- Constant Methods -----------------------------------------------//
        CCamera*                getCamera() const;
        Vector2d                getGravity() const;
        std::list<IObject*>     getObjects() const;
        std::list<IVisuals*>    getVisuals() const;
                
        //--- Methods --------------------------------------------------------//
        bool import(const std::string&,
                    const ImportModeType& = IMPORT_MODE_UNIVERSE);
        
    private:
        
        bool checkFile(const pugi::xml_node&);
        void createCamera(const pugi::xml_node&);
        void createGravity(const pugi::xml_node&);
        void createShapeCircle(CBody* const, const pugi::xml_node&);
        void createShapePlanet(CBody* const, const pugi::xml_node&);
        void createShapeTerrain(CBody* const, const pugi::xml_node&);
        void createShapePolyline(CBody* const, const pugi::xml_node&);
        void createVisualsCircle(CCircle* const, const pugi::xml_node&);
        void createVisualsPlanet(CPlanet* const, const pugi::xml_node&);
        void createVisualsTerrain(CTerrain* const, const pugi::xml_node&);
        void createVisualsPolyline(CPolyLine* const, const pugi::xml_node&);
        void createRigidBody(const pugi::xml_node&);
        void readObjectCore(IObject* const, const pugi::xml_node&);
        
        CCamera*                        m_pCamera;
        Vector2d                        m_vecGravity;
        std::string                     m_strCameraHook;
        std::map<std::string,IObject*>  m_Objects;
        std::list<IVisuals*>            m_Visuals;
        std::string                     m_strPath;
                
};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Return camera
///
/// \return Camera
///
////////////////////////////////////////////////////////////////////////////////
inline CCamera* CXMLImporter::getCamera() const
{
    METHOD_ENTRY("CXMLImporter::getCamera")
    return m_pCamera;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Return the vector of constant gravity (default is zero).
///
/// \return Constant gravity vector
///
////////////////////////////////////////////////////////////////////////////////
inline Vector2d CXMLImporter::getGravity() const
{
    METHOD_ENTRY("CXMLImporter::getGravity")
    return m_vecGravity;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Return imported objects
///
/// \return List of visuals
///
////////////////////////////////////////////////////////////////////////////////
inline std::list<IVisuals*> CXMLImporter::getVisuals() const
{
    METHOD_ENTRY("CXMLImporter::getVisuals")
    return m_Visuals;
}

#endif
