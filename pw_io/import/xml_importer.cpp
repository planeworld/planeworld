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
/// \file       xml_importer.cpp
/// \brief      Implementation of class "CXMLImporter"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2011-02-15
///
////////////////////////////////////////////////////////////////////////////////

#include "xml_importer.h"

//--- Program header ---------------------------------------------------------//
#include "log.h"
#include "planet_visuals.h"
#include "rigidbody.h"

//--- Misc header ------------------------------------------------------------//
#include <qxml.h>
#include <qdom.h>

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Import an xml file
///
/// \param _strFilename XML file to import
///
////////////////////////////////////////////////////////////////////////////////
bool CXMLImporter::import(const std::string& _strFilename)
{
    METHOD_ENTRY("CXMLImporter::import")
    
    QDomDocument Doc("PlaneworldML");
    QFile ImportFile(QString(_strFilename.c_str()));
    
    // Read the document
    if(!ImportFile.open(QIODevice::ReadOnly))
    {
        WARNING_MSG("XML Importer", "Cannot open xml file, skipping import.")
        METHOD_EXIT("CXMLImporter::import")
        return false;
    }
    if(!Doc.setContent(&ImportFile))
    {
        WARNING_MSG("XML Importer", "Content not valid, skipping import.")
        ImportFile.close();
        METHOD_EXIT("CXMLImporter::import")
        return false;
    }
    ImportFile.close();
    
    QDomElement Root = Doc.documentElement();
    
    QDomNode N  = Root.firstChild();
    while (!N.isNull())
    {
        QDomElement E = N.toElement();
        if (!E.isNull())
        {
            if (E.tagName() == "object")
            {
                QString strType = E.attribute("type");
                if (strType == "RigidBody")
                {
                    this->createRigidBody(N.firstChild());
                }
//                 QString strType = E.attribute("type");
//                 if (strType == "file")
            }
        }
        
        N = N.nextSibling();
    }    

    METHOD_EXIT("CXMLImporter::import")
    return true;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Create a rigid body
///
/// \param _Node Current node in xml tree
///
////////////////////////////////////////////////////////////////////////////////
void CXMLImporter::createRigidBody(const QDomNode& _Node)
{
    METHOD_ENTRY("CXMLImporter::createRigidBody")
    
    INFO_MSG("XML Importer", "Creating rigid body.");
    
    CRigidBody* pRigidBody = new CRigidBody;
    MEM_ALLOC("pRigidBody")
    
    QDomNode N = _Node;
    
    while (!N.isNull())
    {
        QDomElement E = N.toElement();
        
        if (E.tagName() == "core")
        {
            this->readObjectCore(pRigidBody, N);
        }
        else if (E.tagName() == "shape")
        {
            QString strType = E.attribute("type");
            if (strType == "Planet")
            {
                this->createPlanetShape(pRigidBody, N);
            }
        }        
        
        N = N.nextSibling();
    }
    
    m_Objects.push_back(pRigidBody);
    
    METHOD_EXIT("CXMLImporter::createRigidBody")
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Create a planet shape
///
/// \param _pBody Body to create the shape for
/// \param _Node Current node in xml tree
///
////////////////////////////////////////////////////////////////////////////////
void CXMLImporter::createPlanetShape(CBody* const _pBody, const QDomNode& _Node)
{
    METHOD_ENTRY("CXMLImporter::createPlanetShape")
    
    CPlanet* pPlanet = new CPlanet;
    MEM_ALLOC("pPlanet")
    
//     while (!_Node.isNull())
//     {
        QDomElement E = _Node.toElement();
        
        pPlanet->setRadius(E.attribute("radius").toDouble());
        pPlanet->setCenter(E.attribute("center_x").toDouble(),
                           E.attribute("center_y").toDouble());
        pPlanet->setHeight(E.attribute("height_max").toDouble());
        pPlanet->initTerrain();
        
//         _Node = _Node.nextSibling();
//     }

    // The shape might have visuals
    if (_Node.hasChildNodes())
    {
        QDomNode N = _Node.firstChild();
        while (!N.isNull())
        {
            QDomElement E = N.toElement();
        
            if (E.tagName() == "visuals")
            {
                this->createPlanetVisuals(pPlanet, N);
            }
            N = N.nextSibling();
        }
    }
    
    _pBody->getGeometry().addShape(pPlanet);
    
    METHOD_EXIT("CXMLImporter::createPlanetShape")
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Create visuals to a planet shape
///
/// \param _pBody Body to receive visuals ids
/// \param _pPlanet Planet to create visuals for
/// \param _Node Current node in xml tree
///
////////////////////////////////////////////////////////////////////////////////
void CXMLImporter::createPlanetVisuals(CPlanet* const _pPlanet, const QDomNode& _Node)
{
    METHOD_ENTRY("CXMLImporter::createPlanetVisuals")
    
    if (!_Node.isNull())
    {
        QDomElement E = _Node.toElement();
        
        if (E.attribute("type") == "Planet")
        {
            CPlanetVisuals* pPlanetVisuals = new CPlanetVisuals(_pPlanet);
            MEM_ALLOC("pPlanetVisuals")
            
            _pPlanet->setVisualsID(m_pVisualsManager->addVisuals(pPlanetVisuals));
        }
            
    }
    
    METHOD_EXIT("CXMLImporter::createPlanetVisuals")
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Read given objects core information
///
/// \param _pO Object to read information for
/// \param _Node Current node in xml tree
///
////////////////////////////////////////////////////////////////////////////////
void CXMLImporter::readObjectCore(IObject* const _pO, const QDomNode& _Node)
{
    METHOD_ENTRY("CXMLImporter::createRigidBody")
    
    QDomElement E = _Node.toElement();
    if (!E.isNull())
    {
        _pO->setName(E.attribute("name").toStdString());
        _pO->setMass(E.attribute("mass").toDouble());
        _pO->setOrigin(E.attribute("origin_x").toDouble(),
                    E.attribute("origin_y").toDouble());
                                
        if (E.attribute("gravity") == "true")
            _pO->enableGravitation();
        else
            _pO->disableGravitation();
        if (E.attribute("dynamics") == "true")
            _pO->enableDynamics();
        else
            _pO->disableDynamics();
    }
    
    METHOD_EXIT("CXMLImporter::createRigidBody")
}
