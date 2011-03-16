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
#include <string>

//--- Program header ---------------------------------------------------------//
#include "body.h"
#include "camera.h"
#include "planet.h"

//--- Misc header ------------------------------------------------------------//
#include "QDomNode"

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
        std::list<IObject*>     getObjects() const;
        std::list<IVisuals*>    getVisuals() const;
                
        //--- Methods --------------------------------------------------------//
        bool import(const std::string&);
        
    private:
        
        bool checkFile(const QDomElement&);
        void createCamera(const QDomNode&);
        void createPlanetShape(CBody* const, const QDomNode&);
        void createPlanetVisuals(CPlanet* const, const QDomNode&);
        void createRigidBody(const QDomNode&);
        void readObjectCore(IObject* const, const QDomNode&);
        
        CCamera*                        m_pCamera;
        std::string                     m_strCameraHook;
        std::map<std::string,IObject*>  m_Objects;
        std::list<IVisuals*>            m_Visuals;
                
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

    METHOD_EXIT("CXMLImporter::getCamera")
    return m_pCamera;
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

    METHOD_EXIT("CXMLImporter::getVisuals")
    return m_Visuals;
}

#endif
