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
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Return imported objects
///
/// \return List of objects
///
////////////////////////////////////////////////////////////////////////////////
std::vector<IObject*> CXMLImporter::getObjects() const
{
    METHOD_ENTRY("CXMLImporter::getObjects")
    
    std::map<std::string, IObject*>::const_iterator ci;
    std::vector<IObject*> TmpList;
    for (ci = m_Objects.begin(); ci != m_Objects.end(); ++ci)
        TmpList.push_back((*ci).second);

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
    
    pugi::xml_document Doc;
    pugi::xml_parse_result Result = Doc.load_file(_strFilename.c_str());
    
    size_t Pos;
    Pos       = _strFilename.find_last_of("/");
    m_strPath = _strFilename.substr(0,Pos);
    
    if (Result)
    {
        INFO_MSG("XML Importer","XML file " << _strFilename << " parsed without errors.")
    }
    else
    {
        WARNING_MSG("XML Importer", "XML file " << _strFilename << " parsed with errors.")
        WARNING_MSG("XML Importer", "Error description: " << Result.description())
    }
    
    pugi::xml_node Root = Doc.first_child();
    pugi::xml_node N;
    switch (_Mode)
    {
        case IMPORT_MODE_UNIVERSE:
            N  = Root.first_child();
            
            if (std::string(Root.name()) == "universe")
            {
                this->createUniverse(Root);
            }
            break;
        case IMPORT_MODE_OBJECT:
            N = Root;
            break;
    }
    while (!N.empty())
    {
        if (std::string(N.name()) == "object")
        {
            if (checkFile(N));
            else if (N.attribute("type").value() != "")
            {
                std::string strType = N.attribute("type").as_string();
                if (strType == "RigidBody")
                {
                    this->createRigidBody(N.first_child());
                }
            }
        }
        else if (std::string(N.name()) == "camera")
        {
            this->createCamera(N);
        }
        else if (std::string(N.name()) == "gravity")
        {
            this->createGravity(N);
        }
        
        N = N.next_sibling();
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
/// \brief Check node for file attribute
///
/// \param _Node Current node in xml tree
///
/// \return Has attribute file?
///
////////////////////////////////////////////////////////////////////////////////
bool CXMLImporter::checkFile(const pugi::xml_node& _Node)
{
    METHOD_ENTRY("CXMLImporter::checkFile")
    
    if (_Node.attribute("file").value() != "")
    {
        this->import(m_strPath+"/"+_Node.attribute("file").value(),
                     IMPORT_MODE_OBJECT);
        return true;
    }

    return false;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Create a camera
///
/// \param _Node Current node in xml tree
///
////////////////////////////////////////////////////////////////////////////////
void CXMLImporter::createCamera(const pugi::xml_node& _Node)
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
    
    m_strCameraHook = _Node.attribute("hook").as_string();
    m_pCamera->setPosition(_Node.attribute("position_x").as_double(),
                           _Node.attribute("position_y").as_double());
    m_pCamera->setViewport(_Node.attribute("viewport_width").as_int(),
                           _Node.attribute("viewport_height").as_int());
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Create constant gravity for the whole universe
///
/// \param _Node Current node in xml tree
///
////////////////////////////////////////////////////////////////////////////////
void CXMLImporter::createGravity(const pugi::xml_node& _Node)
{
    METHOD_ENTRY("CXMLImporter::createGravity")
    
    INFO_MSG("XML Importer", "Setting constant gravity.")
    
    m_vecGravity = Vector2d(_Node.attribute("vec_x").as_double(),
                            _Node.attribute("vec_y").as_double());
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Create a rigid body
///
/// \param _Node Current node in xml tree
///
////////////////////////////////////////////////////////////////////////////////
void CXMLImporter::createRigidBody(const pugi::xml_node& _Node)
{
    METHOD_ENTRY("CXMLImporter::createRigidBody")
    
    INFO_MSG("XML Importer", "Creating rigid body.");
    
    CRigidBody* pRigidBody = new CRigidBody;
    MEM_ALLOC("pRigidBody")
    
    IObjectVisuals* pObjectVisuals = new IObjectVisuals(pRigidBody);
    
    pugi::xml_node N = _Node;
    
    while (!N.empty())
    {
        if (std::string(N.name()) == "core")
        {
            this->readObjectCore(pRigidBody, N);
        }
        else if (std::string(N.name()) == "shape")
        {
            std::string strType(N.attribute("type").as_string());
            if (strType == "Planet")
            {
                this->createShapePlanet(pRigidBody, pObjectVisuals, N);
            }
            else if (strType == "Circle")
            {
                this->createShapeCircle(pRigidBody, pObjectVisuals, N);
            }
            else if (strType == "Polyline")
            {
                this->createShapePolyline(pRigidBody, pObjectVisuals, N);
            }
            else if (strType == "Terrain")
            {
                this->createShapeTerrain(pRigidBody, pObjectVisuals, N);
            }
        }        
        
        N = N.next_sibling();
    }
    
    m_Objects.insert(std::pair<std::string,IObject*>(pRigidBody->getName(),pRigidBody));
    m_Visuals.push_back(pObjectVisuals);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Create a circle shape
///
/// \param _pBody Body to create the shape for
/// \param _pObjectVisuals Pointer to object visuals to refer shape visuals to
/// \param _Node Current node in xml tree
///
////////////////////////////////////////////////////////////////////////////////
void CXMLImporter::createShapeCircle(CBody* const _pBody,
                                     IObjectVisuals* const _pObjectVisuals,
                                     const pugi::xml_node& _Node)
{
    METHOD_ENTRY("CXMLImporter::createShapeCircle")
    
    CCircle* pCircle = new CCircle;
    MEM_ALLOC("pCircle")
    
    pCircle->setRadius(_Node.attribute("radius").as_double());
    pCircle->setCenter(_Node.attribute("center_x").as_double(),
                       _Node.attribute("center_y").as_double());
        
    // The shape might have visuals
    if (_Node.first_child())
    {
        pugi::xml_node N = _Node.first_child();
        while (!N.empty())
        {
            if (std::string(N.name()) == "visuals")
            {
                this->createVisualsCircle(pCircle, _pObjectVisuals, N);
            }
            N = N.next_sibling();
        }
    }
    
    _pBody->getGeometry()->addShape(pCircle);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Create a planet shape
///
/// \param _pBody Body to create the shape for
/// \param _pObjectVisuals Pointer to object visuals to refer shape visuals to
/// \param _Node Current node in xml tree
///
////////////////////////////////////////////////////////////////////////////////
void CXMLImporter::createShapePlanet(CBody* const _pBody,
                                     IObjectVisuals* const _pObjectVisuals,
                                     const pugi::xml_node& _Node)
{
    METHOD_ENTRY("CXMLImporter::createShapePlanet")
    
    CPlanet* pPlanet = new CPlanet;
    MEM_ALLOC("pPlanet")
    
    pPlanet->setRadius(_Node.attribute("radius").as_double());
    pPlanet->setCenter(_Node.attribute("center_x").as_double(),
                       _Node.attribute("center_y").as_double());
    pPlanet->setHeight(_Node.attribute("height_max").as_double());
    pPlanet->setGroundResolution(_Node.attribute("ground_resolution").as_double());
    pPlanet->setSeaLevel(_Node.attribute("sea_level").as_double());
    pPlanet->initTerrain();
        
    // The shape might have visuals
    if (_Node.first_child())
    {
        pugi::xml_node N = _Node.first_child();
        while (!N.empty())
        {
            if (std::string(N.name()) == "visuals")
            {
                this->createVisualsPlanet(pPlanet, _pObjectVisuals, N);
            }
            N = N.next_sibling();
        }
    }
    
    _pBody->getGeometry()->addShape(pPlanet);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Create a polyline shape
///
/// \param _pBody Body to create the shape for
/// \param _pObjectVisuals Pointer to object visuals to refer shape visuals to
/// \param _Node Current node in xml tree
///
////////////////////////////////////////////////////////////////////////////////
void CXMLImporter::createShapePolyline(CBody* const _pBody,
                                       IObjectVisuals* const _pObjectVisuals,
                                       const pugi::xml_node& _Node)
{
    METHOD_ENTRY("CXMLImporter::createShapePolyline")
    
    CPolyLine* pPolyline = new CPolyLine;
    MEM_ALLOC("pPolyline")
    
    if (std::string(_Node.attribute("line_type").as_string()) == "loop")
        pPolyline->setLineType(GRAPHICS_LINETYPE_LOOP);
    else if (std::string(_Node.attribute("line_type").as_string()) == "strip")
        pPolyline->setLineType(GRAPHICS_LINETYPE_STRIP);
    else if (std::string(_Node.attribute("line_type").as_string()) == "single")
        pPolyline->setLineType(GRAPHICS_LINETYPE_SINGLE);
    
    std::string strPoints = _Node.attribute("points").as_string();
    size_t Pos;

    Pos=strPoints.find_first_of(",");
    while (Pos != std::string::npos)
    {
        double fX;
        double fY;
        std::string strTmp;
        {
            strTmp    = strPoints.substr(0,Pos);
            strPoints = strPoints.substr(Pos+1);
            std::istringstream iss(strTmp);
            iss >> fX;
            Pos=strPoints.find_first_of(";");
        }
        {
            strTmp    = strPoints.substr(0,Pos);
            strPoints = strPoints.substr(Pos+1);
            std::istringstream iss(strTmp);
            iss >> fY;
            Pos=strPoints.find_first_of(",");
        }
        pPolyline->addVertex(fX,fY);
    }
    
    // The shape might have visuals
    if (!_Node.empty())
    {
        pugi::xml_node N = _Node.first_child();
        while (!N.empty())
        {
            if (std::string(N.name()) == "visuals")
            {
                this->createVisualsPolyline(pPolyline, _pObjectVisuals, N);
            }
            N = N.next_sibling();
        }
    }
    
    _pBody->getGeometry()->addShape(pPolyline);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Create a terrain shape
///
/// \param _pBody Body to create the shape for
/// \param _pObjectVisuals Pointer to object visuals to refer shape visuals to
/// \param _Node Current node in xml tree
///
////////////////////////////////////////////////////////////////////////////////
void CXMLImporter::createShapeTerrain(CBody* const _pBody,
                                      IObjectVisuals* const _pObjectVisuals,
                                      const pugi::xml_node& _Node)
{
    METHOD_ENTRY("CXMLImporter::createShapeTerrain")
    
    CTerrain* pTerrain = new CTerrain;
    MEM_ALLOC("pTerrain")
    
    pTerrain->setWidth (_Node.attribute("width").as_double());
    pTerrain->setCenter(_Node.attribute("center_x").as_double(),
                        _Node.attribute("center_y").as_double());
    pTerrain->setHeight(_Node.attribute("height_max").as_double());
    pTerrain->setDiversity(_Node.attribute("diversity").as_double());
    pTerrain->setGroundResolution(_Node.attribute("ground_resolution").as_double());
        
    // The shape might have visuals
    if (_Node.first_child())
    {
        pugi::xml_node N = _Node.first_child();
        while (!N.empty())
        {
            if (std::string(N.name()) == "visuals")
            {
                this->createVisualsTerrain(pTerrain, _pObjectVisuals, N);
            }
            N = N.next_sibling();
        }
    }
    
    _pBody->getGeometry()->addShape(pTerrain);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Create visuals to a circle shape
///
/// \param _pCircle Circle to create visuals for
/// \param _pObjectVisuals Pointer to object visuals to refer shape visuals to
/// \param _Node Current node in xml tree
///
////////////////////////////////////////////////////////////////////////////////
void CXMLImporter::createVisualsCircle(CCircle* const _pCircle,
                                       IObjectVisuals* const _pObjectVisuals,
                                       const pugi::xml_node& _Node)
{
    METHOD_ENTRY("CXMLImporter::createVisualsCircle")
    
    if (!_Node.empty())
    {
        if (std::string(_Node.attribute("type").as_string()) == "Circle")
        {
            CCircleVisuals* pCircleVisuals = new CCircleVisuals(_pCircle);
            MEM_ALLOC("pCircleVisuals")
            
            _pObjectVisuals->addVisuals(pCircleVisuals);
        }
            
    }
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Create visuals to a planet shape
///
/// \param _pPlanet Planet to create visuals for
/// \param _pObjectVisuals Pointer to object visuals to refer shape visuals to
/// \param _Node Current node in xml tree
///
////////////////////////////////////////////////////////////////////////////////
void CXMLImporter::createVisualsPlanet(CPlanet* const _pPlanet,
                                       IObjectVisuals* const _pObjectVisuals,
                                       const pugi::xml_node& _Node)
{
    METHOD_ENTRY("CXMLImporter::createVisualsPlanet")
    
    if (!_Node.empty())
    {
        if (std::string(_Node.attribute("type").as_string()) == "Planet")
        {
            CPlanetVisuals* pPlanetVisuals = new CPlanetVisuals(_pPlanet);
            MEM_ALLOC("pPlanetVisuals")
            
            _pObjectVisuals->addVisuals(pPlanetVisuals);
        }
            
    }
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Create visuals to a polyline shape
///
/// \param _pPolyline Polyline to create visuals for
/// \param _pObjectVisuals Pointer to object visuals to refer shape visuals to
/// \param _Node Current node in xml tree
///
////////////////////////////////////////////////////////////////////////////////
void CXMLImporter::createVisualsPolyline(CPolyLine* const _pPolyline,
                                         IObjectVisuals* const _pObjectVisuals,
                                         const pugi::xml_node& _Node)
{
    METHOD_ENTRY("CXMLImporter::createVisualsPolyline")
    
    if (!_Node.empty())
    {
        if (std::string(_Node.attribute("type").as_string()) == "Polyline")
        {
            CPolylineVisuals* pPolylineVisuals = new CPolylineVisuals(_pPolyline);
            MEM_ALLOC("pPlanetVisuals")
            
            _pObjectVisuals->addVisuals(pPolylineVisuals);
        }
            
    }
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Create visuals to a terrain shape
///
/// \param _pTerrain Terrain to create visuals for
/// \param _pObjectVisuals Pointer to object visuals to refer shape visuals to
/// \param _Node Current node in xml tree
///
////////////////////////////////////////////////////////////////////////////////
void CXMLImporter::createVisualsTerrain(CTerrain* const _pTerrain,
                                        IObjectVisuals* const _pObjectVisuals,
                                        const pugi::xml_node& _Node)
{
    METHOD_ENTRY("CXMLImporter::createVisualsTerrain")
    
    if (!_Node.empty())
    {
        if (std::string(_Node.attribute("type").as_string()) == "Terrain")
        {
            CTerrainVisuals* pTerrainVisuals = new CTerrainVisuals(_pTerrain);
            MEM_ALLOC("pTerrainVisuals")
            
            _pObjectVisuals->addVisuals(pTerrainVisuals);
        }
            
    }
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Creates the procedurally generated universe
///
/// \param _Node Current node in xml tree
///
////////////////////////////////////////////////////////////////////////////////
void CXMLImporter::createUniverse(const pugi::xml_node& _Node)
{
    METHOD_ENTRY("CXMLImporter::createUniverse")
    
    if (!_Node.empty())
    {
        m_Universe.generate(_Node.attribute("seed").as_int(),
                            _Node.attribute("number_of_stars").as_int());
    }
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Read given objects core information
///
/// \param _pO Object to read information for
/// \param _Node Current node in xml tree
///
////////////////////////////////////////////////////////////////////////////////
void CXMLImporter::readObjectCore(IObject* const _pO, const pugi::xml_node& _Node)
{
    METHOD_ENTRY("CXMLImporter::createRigidBody")
    
    if (!_Node.empty())
    {
        _pO->setName(_Node.attribute("name").as_string());
        _pO->setMass(_Node.attribute("mass").as_double());
        _pO->setOrigin(_Node.attribute("origin_x").as_double(),
                       _Node.attribute("origin_y").as_double());
        _pO->setCell(Vector2i(
                     _Node.attribute("cell_x").as_int(),
                     _Node.attribute("cell_y").as_int()));
                                
        if (_Node.attribute("gravity").as_bool() == true)
            _pO->enableGravitation();
        else
            _pO->disableGravitation();
        if (_Node.attribute("dynamics").as_bool() == true)
            _pO->enableDynamics();
        else
            _pO->disableDynamics();
    }
}
