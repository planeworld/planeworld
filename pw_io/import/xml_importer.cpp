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
#include "circle_visuals.h"
#include "planet_visuals.h"
#include "rigidbody.h"

//--- Misc header ------------------------------------------------------------//
#include <qxml.h>
#include <qdom.h>

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor
///
///////////////////////////////////////////////////////////////////////////////
CXMLImporter::CXMLImporter() : m_pCamera(0)
{
    METHOD_ENTRY("CXMLImporter::CXMLImporter")
    CTOR_CALL("CXMLImporter::CXMLImporter")
    
    m_vecGravity.setZero();

    METHOD_EXIT("CXMLImporter::CXMLImporter")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Destructor
///
///////////////////////////////////////////////////////////////////////////////
CXMLImporter::~CXMLImporter()
{
    METHOD_ENTRY("CXMLImporter::~CXMLImporter")
    DTOR_CALL("CXMLImporter::CXMLImporter")
   
    METHOD_EXIT("CXMLImporter::~CXMLImporter")
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Return imported objects
///
/// \return List of objects
///
////////////////////////////////////////////////////////////////////////////////
std::list<IObject*> CXMLImporter::getObjects() const
{
    METHOD_ENTRY("CXMLImporter::getObjects")
    
    std::map<std::string, IObject*>::const_iterator ci;
    std::list<IObject*> TmpList;
    for (ci = m_Objects.begin(); ci != m_Objects.end(); ++ci)
        TmpList.push_back((*ci).second);

    METHOD_EXIT("CXMLImporter::getObjects")
    return TmpList;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Import an xml file
///
/// \param _strFilename XML file to import
/// \param _Mode Import mode type to allow recursive reading of xml files
///
////////////////////////////////////////////////////////////////////////////////
bool CXMLImporter::import(const std::string& _strFilename,
                          const ImportModeType& _Mode)
{
    METHOD_ENTRY("CXMLImporter::import")
    
    QDomDocument Doc("PlaneworldML");
    QFile ImportFile(QString(_strFilename.c_str()));
    
    m_strPath=QString(_strFilename.c_str());
    m_strPath.truncate(m_strPath.lastIndexOf("/"));
    
    QString strFilename=QString(_strFilename.c_str());
    strFilename = strFilename.section('/',-1);
    
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
    
    QDomNode N;
    switch (_Mode)
    {
        case IMPORT_MODE_UNIVERSE:
            N  = Root.firstChild();
            break;
        case IMPORT_MODE_OBJECT:
            N = Root;
            break;
    }
    while (!N.isNull())
    {
        QDomElement E = N.toElement();
        if (!E.isNull())
        {
            if (E.tagName() == "object")
            {
                if (checkFile(E));
                else if (E.hasAttribute("type"))
                {
                    QString strType = E.attribute("type");
                    if (strType == "RigidBody")
                    {
                        this->createRigidBody(N.firstChild());
                    }
                }
            }
            else if (E.tagName() == "camera")
            {
                this->createCamera(N);
            }
            else if (E.tagName() == "gravity")
            {
                this->createGravity(N);
            }
        }
        
        N = N.nextSibling();
    }    

    if (_Mode == IMPORT_MODE_UNIVERSE)
    {
        // Camera hook must have been read by now
        // Set hook, if camera given. Otherwise a default cam is used.
        if (m_pCamera != 0)
        {
            INFO_MSG("XML Importer", "Camera Hook: " << m_strCameraHook)
            m_pCamera->setHook(m_Objects[m_strCameraHook]);
        }
        else
        {
            CRigidBody* pDefaultCam = new CRigidBody;
            MEM_ALLOC("pDefaultCam")
            pDefaultCam->setName("DefaultCamera");
            pDefaultCam->disableDynamics();
            pDefaultCam->disableGravitation();
            
            m_Objects.insert(std::pair<std::string,IObject*>("pDefaultCam", pDefaultCam));
            
            m_pCamera = new CCamera;
            m_pCamera->setHook(pDefaultCam);
            
            INFO_MSG("XML Importer", "Camera Hook: DefaultCamera")
        }
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Check Element for file attribute
///
/// \param _E Node element
///
/// \return Has attribute file?
///
////////////////////////////////////////////////////////////////////////////////
bool CXMLImporter::checkFile(const QDomElement& _E)
{
    METHOD_ENTRY("CXMLImporter::checkFile")
    
    if (_E.hasAttribute("file"))
    {
        this->import(m_strPath.toStdString()+"/"+_E.attribute("file").toStdString(),
                     IMPORT_MODE_OBJECT);
        METHOD_EXIT("CXMLImporter::checkFile")
        return true;
    }
 
    METHOD_EXIT("CXMLImporter::checkFile")
    return false;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Create a camera
///
/// \param _Node Current node in xml tree
///
////////////////////////////////////////////////////////////////////////////////
void CXMLImporter::createCamera(const QDomNode& _Node)
{
    METHOD_ENTRY("CXMLImporter::createCamera")
    
    INFO_MSG("XML Importer", "Creating camera.")
    
     // Free memory if pointer is already existent
    if (m_pCamera != 0)
    {
        delete m_pCamera;
        m_pCamera = 0;
        MEM_FREED("m_pCamera");
        NOTICE_MSG("XML Importer", "More than one camera, creating new.")
    }
    m_pCamera = new CCamera;
    MEM_ALLOC("pCamera")
    
    QDomElement E = _Node.toElement();
    
    m_strCameraHook = E.attribute("hook").toStdString();
    m_pCamera->setPosition(E.attribute("position_x").toDouble(),
                           E.attribute("position_y").toDouble());
    m_pCamera->setViewport(E.attribute("viewport_width").toDouble(),
                           E.attribute("viewport_height").toDouble());
    
    METHOD_EXIT("CXMLImporter::createCamera")
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Create constant gravity for the whole universe
///
/// \param _Node Current node in xml tree
///
////////////////////////////////////////////////////////////////////////////////
void CXMLImporter::createGravity(const QDomNode& _Node)
{
    METHOD_ENTRY("CXMLImporter::createGravity")
    
    INFO_MSG("XML Importer", "Setting constant gravity.")
    
    QDomElement E = _Node.toElement();
    
    m_vecGravity = Vector2d(E.attribute("vec_x").toDouble(),
                            E.attribute("vec_y").toDouble());
    
    METHOD_EXIT("CXMLImporter::createGravity")
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
                this->createShapePlanet(pRigidBody, N);
            }
            else if (strType == "Circle")
            {
                this->createShapeCircle(pRigidBody, N);
            }
            else if (strType == "Polyline")
            {
                this->createShapePolyline(pRigidBody, N);
            }
            else if (strType == "Terrain")
            {
                this->createShapeTerrain(pRigidBody, N);
            }
        }        
        
        N = N.nextSibling();
    }
    
    m_Objects.insert(std::pair<std::string,IObject*>(pRigidBody->getName(),pRigidBody));
    
    METHOD_EXIT("CXMLImporter::createRigidBody")
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Create a circle shape
///
/// \param _pBody Body to create the shape for
/// \param _Node Current node in xml tree
///
////////////////////////////////////////////////////////////////////////////////
void CXMLImporter::createShapeCircle(CBody* const _pBody, const QDomNode& _Node)
{
    METHOD_ENTRY("CXMLImporter::createShapeCircle")
    
    CCircle* pCircle = new CCircle;
    MEM_ALLOC("pCircle")
    
    QDomElement E = _Node.toElement();
    
    pCircle->setRadius(E.attribute("radius").toDouble());
    pCircle->setCenter(E.attribute("center_x").toDouble(),
                       E.attribute("center_y").toDouble());
        
    // The shape might have visuals
    if (_Node.hasChildNodes())
    {
        QDomNode N = _Node.firstChild();
        while (!N.isNull())
        {
            QDomElement E = N.toElement();
        
            if (E.tagName() == "visuals")
            {
                this->createVisualsCircle(pCircle, N);
            }
            N = N.nextSibling();
        }
    }
    
    _pBody->getGeometry()->addShape(pCircle);
    
    METHOD_EXIT("CXMLImporter::createShapeCircle")
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Create a planet shape
///
/// \param _pBody Body to create the shape for
/// \param _Node Current node in xml tree
///
////////////////////////////////////////////////////////////////////////////////
void CXMLImporter::createShapePlanet(CBody* const _pBody, const QDomNode& _Node)
{
    METHOD_ENTRY("CXMLImporter::createShapePlanet")
    
    CPlanet* pPlanet = new CPlanet;
    MEM_ALLOC("pPlanet")
    
    QDomElement E = _Node.toElement();
    
    pPlanet->setRadius(E.attribute("radius").toDouble());
    pPlanet->setCenter(E.attribute("center_x").toDouble(),
                        E.attribute("center_y").toDouble());
    pPlanet->setHeight(E.attribute("height_max").toDouble());
    pPlanet->setGroundResolution(E.attribute("ground_resolution").toDouble());
    pPlanet->setSeaLevel(E.attribute("sea_level").toDouble());
    pPlanet->initTerrain();
        
    // The shape might have visuals
    if (_Node.hasChildNodes())
    {
        QDomNode N = _Node.firstChild();
        while (!N.isNull())
        {
            QDomElement E = N.toElement();
        
            if (E.tagName() == "visuals")
            {
                this->createVisualsPlanet(pPlanet, N);
            }
            N = N.nextSibling();
        }
    }
    
    _pBody->getGeometry()->addShape(pPlanet);
    
    METHOD_EXIT("CXMLImporter::createShapePlanet")
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Create a polyline shape
///
/// \param _pBody Body to create the shape for
/// \param _Node Current node in xml tree
///
////////////////////////////////////////////////////////////////////////////////
void CXMLImporter::createShapePolyline(CBody* const _pBody, const QDomNode& _Node)
{
    METHOD_ENTRY("CXMLImporter::createShapePolyline")
    
    CPolyLine* pPolyline = new CPolyLine;
    MEM_ALLOC("pPolyline")
    
    QDomElement E = _Node.toElement();

    if (E.attribute("line_type") == "loop")
        pPolyline->setLineType(GRAPHICS_LINETYPE_LOOP);
    else if (E.attribute("line_type") == "strip")
        pPolyline->setLineType(GRAPHICS_LINETYPE_STRIP);
    else if (E.attribute("line_type") == "single")
        pPolyline->setLineType(GRAPHICS_LINETYPE_SINGLE);
    
    QStringList Points = E.attribute("points").simplified().split(";");
    for (int i=0; i<Points.size(); ++i)
    {
        QStringList Point = Points[i].simplified().split(",");
        pPolyline->addVertex(Point[0].toDouble(),Point[1].toDouble());
    }
    
    // The shape might have visuals
    if (_Node.hasChildNodes())
    {
        QDomNode N = _Node.firstChild();
        while (!N.isNull())
        {
            QDomElement E = N.toElement();
        
            if (E.tagName() == "visuals")
            {
                this->createVisualsPolyline(pPolyline, N);
            }
            N = N.nextSibling();
        }
    }
    
    _pBody->getGeometry()->addShape(pPolyline);
    
    METHOD_EXIT("CXMLImporter::createShapePolyline")
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Create a terrain shape
///
/// \param _pBody Body to create the shape for
/// \param _Node Current node in xml tree
///
////////////////////////////////////////////////////////////////////////////////
void CXMLImporter::createShapeTerrain(CBody* const _pBody, const QDomNode& _Node)
{
    METHOD_ENTRY("CXMLImporter::createShapeTerrain")
    
    CTerrain* pTerrain = new CTerrain;
    MEM_ALLOC("pTerrain")
    
    QDomElement E = _Node.toElement();
    
    pTerrain->setWidth (E.attribute("width").toDouble());
    pTerrain->setCenter(E.attribute("center_x").toDouble(),
                        E.attribute("center_y").toDouble());
    pTerrain->setHeight(E.attribute("height_max").toDouble());
    pTerrain->setDiversity(E.attribute("diversity").toDouble());
    pTerrain->setGroundResolution(E.attribute("ground_resolution").toDouble());
        
    // The shape might have visuals
    if (_Node.hasChildNodes())
    {
        QDomNode N = _Node.firstChild();
        while (!N.isNull())
        {
            QDomElement E = N.toElement();
        
            if (E.tagName() == "visuals")
            {
                this->createVisualsTerrain(pTerrain, N);
            }
            N = N.nextSibling();
        }
    }
    
    _pBody->getGeometry()->addShape(pTerrain);
    
    METHOD_EXIT("CXMLImporter::createShapeTerrain")
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Create visuals to a circle shape
///
/// \param _pCircle Circle to create visuals for
/// \param _Node Current node in xml tree
///
////////////////////////////////////////////////////////////////////////////////
void CXMLImporter::createVisualsCircle(CCircle* const _pCircle, const QDomNode& _Node)
{
    METHOD_ENTRY("CXMLImporter::createVisualsCircle")
    
    if (!_Node.isNull())
    {
        QDomElement E = _Node.toElement();
        
        if (E.attribute("type") == "Circle")
        {
            CCircleVisuals* pCircleVisuals = new CCircleVisuals(_pCircle);
            MEM_ALLOC("pCircleVisuals")
            
            m_Visuals.push_back(pCircleVisuals);
        }
            
    }
    
    METHOD_EXIT("CXMLImporter::createVisualsCircle")
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Create visuals to a planet shape
///
/// \param _pPlanet Planet to create visuals for
/// \param _Node Current node in xml tree
///
////////////////////////////////////////////////////////////////////////////////
void CXMLImporter::createVisualsPlanet(CPlanet* const _pPlanet, const QDomNode& _Node)
{
    METHOD_ENTRY("CXMLImporter::createVisualsPlanet")
    
    if (!_Node.isNull())
    {
        QDomElement E = _Node.toElement();
        
        if (E.attribute("type") == "Planet")
        {
            CPlanetVisuals* pPlanetVisuals = new CPlanetVisuals(_pPlanet);
            MEM_ALLOC("pPlanetVisuals")
            
            m_Visuals.push_back(pPlanetVisuals);
        }
            
    }
    
    METHOD_EXIT("CXMLImporter::createVisualsPlanet")
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Create visuals to a polyline shape
///
/// \param _pPolyline Polyline to create visuals for
/// \param _Node Current node in xml tree
///
////////////////////////////////////////////////////////////////////////////////
void CXMLImporter::createVisualsPolyline(CPolyLine* const _pPolyline, const QDomNode& _Node)
{
    METHOD_ENTRY("CXMLImporter::createVisualsPolyline")
    
    if (!_Node.isNull())
    {
        QDomElement E = _Node.toElement();
        
        if (E.attribute("type") == "Polyline")
        {
            CPolylineVisuals* pPolylineVisuals = new CPolylineVisuals(_pPolyline);
            MEM_ALLOC("pPlanetVisuals")
            
            m_Visuals.push_back(pPolylineVisuals);
        }
            
    }
    
    METHOD_EXIT("CXMLImporter::createVisualsPolyline")
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Create visuals to a terrain shape
///
/// \param _pTerrain Terrain to create visuals for
/// \param _Node Current node in xml tree
///
////////////////////////////////////////////////////////////////////////////////
void CXMLImporter::createVisualsTerrain(CTerrain* const _pTerrain, const QDomNode& _Node)
{
    METHOD_ENTRY("CXMLImporter::createVisualsTerrain")
    
    if (!_Node.isNull())
    {
        QDomElement E = _Node.toElement();
        
        if (E.attribute("type") == "Terrain")
        {
            CTerrainVisuals* pTerrainVisuals = new CTerrainVisuals(_pTerrain);
            MEM_ALLOC("pTerrainVisuals")
            
            m_Visuals.push_back(pTerrainVisuals);
        }
            
    }
    
    METHOD_EXIT("CXMLImporter::createVisualsTerrain")
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
