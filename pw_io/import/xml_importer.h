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
#include "planet.h"
#include "visuals_manager.h"

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

        //--- Constant Methods -----------------------------------------------//
        std::list<IObject*>     getObjects() const;
        
        //--- Methods --------------------------------------------------------//
        bool import(const std::string&);
        
        void setVisualsManager(CVisualsManager*);

    private:
        
        void createPlanetShape(CBody* const, const QDomNode&);
        void createPlanetVisuals(CPlanet* const, const QDomNode&);
        void createRigidBody(const QDomNode&);
        void readObjectCore(IObject* const, const QDomNode&);
        
        std::list<IObject*>     m_Objects;
        std::list<IVisuals*>    m_Visuals;
        CVisualsManager*        m_pVisualsManager;
        
};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Return imported objects
///
/// \return List of objects
///
////////////////////////////////////////////////////////////////////////////////
inline std::list<IObject*> CXMLImporter::getObjects() const
{
    METHOD_ENTRY("CXMLImporter::getObjects")

    METHOD_EXIT("CXMLImporter::getObjects")
    return m_Objects;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Pass the visuals manager
///
/// \param _pVM Instance of visuals manager
///
////////////////////////////////////////////////////////////////////////////////
inline void CXMLImporter::setVisualsManager(CVisualsManager* _pVM)
{
    METHOD_ENTRY("CXMLImporter::setVisualsManager")

    m_pVisualsManager = _pVM;

    METHOD_EXIT("CXMLImporter::setVisualsManager")
}

#endif
