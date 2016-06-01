////////////////////////////////////////////////////////////////////////////////
//
// This file is part of planeworld, a 2D simulation of physics and much more.
// Copyright (C) 2011-2016 Torsten Büschenfeld
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
/// \file       xml_importer.cpp
/// \brief      Implementation of class "CXMLImporter"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2011-02-15
///
/// \todo Implement suffix "_ref" to manipulate single parameters with respect
///       to kinematics reference.
///
////////////////////////////////////////////////////////////////////////////////

#include "xml_importer.h"

//--- Program header ---------------------------------------------------------//
#include "log.h"
#include "rigidbody.h"

#include "circle.h"
#include "circle_visuals.h"
#include "physics_manager.h"
#include "planet.h"
#include "planet_visuals.h"
#include "polygon.h"
#include "polygon_visuals.h"
#include "terrain.h"
#include "terrain_visuals.h"
#include "visuals_manager.h"

//--- Misc header ------------------------------------------------------------//

//--- Constants --------------------------------------------------------------//
const bool XML_IMPORTER_DO_NOT_NOTICE = false;


///////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor
///
///////////////////////////////////////////////////////////////////////////////
CXMLImporter::CXMLImporter() : m_pCurrentEmitter(nullptr),
                               m_pCurrentBody(nullptr),
                               m_pCurrentObjectVisuals(nullptr),
                               m_pCurrentDoubleBufferedShape(nullptr),
                               m_pCamera(nullptr),
                               m_strPath(""),
                               m_strFont(""),
                               m_strLuaPhysicsInterface(""),
                               m_fDebrisFrequency(PHYSICS_DEBRIS_DEFAULT_FREQUENCY),
                               m_fLuaFrequency(PHYSICS_LUA_DEFAULT_FREQUENCY),
                               m_fPhysicsFrequency(PHYSICS_DEFAULT_FREQUENCY),
                               m_fVisualsFrequency(VISUALS_DEFAULT_FREQUENCY)
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

    if (Result)
    {
        DOM_FIO(INFO_MSG("XML Importer","XML file " << _strFilename << " parsed without errors."))
    }
    else
    {
        WARNING_MSG("XML Importer", "XML file " << _strFilename << " parsed with errors.")
        WARNING_MSG("XML Importer", "Error description: " << Result.description())
    }
    
    // Save path -- kind of stack push, when recursively calling import
    std::string strPath = m_strPath;
    
    // First, devide given filename into path and filename. Be sure to create
    // local path if no path is included ("planeworld example.xml", resulting
    // in path "." and filename "example.xml").
    size_t Pos;
    Pos = _strFilename.find_last_of("/");
    if (Pos != _strFilename.npos)
        m_strPath = _strFilename.substr(0,Pos);
    else
        m_strPath = ".";
    
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
            else
            {
                ERROR_MSG("XML Importer", "Cannot import sub config. Please start with xml root file.")
                return false;
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
            if (!checkFile(N))
            {
                if (!N.attribute("type").empty())
                {
                    std::string strType = N.attribute("type").as_string();
                    if (strType == "rigidbody")
                    {
                        this->createRigidBody(N.first_child());
                    }
                }
            } // if (!checkFile(N))
        }
        else if (std::string(N.name()) == "camera")
        {
            this->createCamera(N);
        }
        else if (std::string(N.name()) == "component")
        {
            this->createComponent(N);
        }
        else if (std::string(N.name()) == "config")
        {
            if (!checkFile(N))
            {
                std::string strLuaPhysicsInterface = checkAttributeString(N, "physics_interface", m_strLuaPhysicsInterface);
                if (strLuaPhysicsInterface == "") m_strLuaPhysicsInterface = "";
                else m_strLuaPhysicsInterface = m_strPath+"/"+strLuaPhysicsInterface;
                m_strFont = m_strPath+"/"+checkAttributeString(N, "font", m_strFont);
                m_fLuaFrequency  = checkAttributeDouble(N, "lua_frequency", m_fLuaFrequency);
                m_fDebrisFrequency  = checkAttributeDouble(N, "debris_frequency", m_fDebrisFrequency);
                m_fPhysicsFrequency = checkAttributeDouble(N, "physics_frequency", m_fPhysicsFrequency);
                m_fVisualsFrequency = checkAttributeDouble(N, "visuals_frequency", m_fVisualsFrequency);
                
                if (m_fLuaFrequency > m_fPhysicsFrequency)
                {
                    m_fLuaFrequency=m_fPhysicsFrequency;
                    NOTICE_MSG("XML Importer", "Lua frequency to high. Resetting to " << m_fLuaFrequency << "Hz.")
                }
                if (m_fDebrisFrequency > m_fPhysicsFrequency)
                {
                    m_fDebrisFrequency=m_fPhysicsFrequency;
                    NOTICE_MSG("XML Importer", "Lua frequency to high. Resetting to " << m_fDebrisFrequency << "Hz.")
                }
                if (m_fVisualsFrequency > m_fPhysicsFrequency)
                {
                     m_fVisualsFrequency=m_fPhysicsFrequency;
                    NOTICE_MSG("XML Importer", "Lua frequency to high. Resetting to " << m_fVisualsFrequency << "Hz.")
                }
            }
            
        }        
        else if (std::string(N.name()) == "emitter")
        {
            this->createEmitter(N);
        }
        else if (std::string(N.name()) == "gravity")
        {
            this->createGravity(N);
        }
        else if (std::string(N.name()) == "shape")
        {
            if (!checkFile(N))
            {
                std::string strType = checkAttributeString(N, "type", "no_type");
                if (strType == "shape_circle")
                {
                    this->createShapeCircle(m_pCurrentBody, m_pCurrentObjectVisuals, N);
                }
                else if (strType == "shape_planet")
                {
                    this->createShapePlanet(m_pCurrentBody, m_pCurrentObjectVisuals, N);
                }
                else if (strType == "shape_terrain")
                {
                    this->createShapeTerrain(m_pCurrentBody, m_pCurrentObjectVisuals, N);
                }
                else if (strType == "shape_polygon")
                {
                    this->createShapePolygon(m_pCurrentBody, m_pCurrentObjectVisuals, N);
                }
                else if (strType == "no_type")
                {
                    WARNING_MSG("XML Importer", "No type given for shape.")
                }
            }
        }
        else if (std::string(N.name()) == "shape_visuals_circle")
        {
            this->createVisualsCircle(m_pCurrentDoubleBufferedShape, m_pCurrentObjectVisuals, N);
        }
        else if (std::string(N.name()) == "shape_visuals_planet")
        {
            this->createVisualsPlanet(m_pCurrentDoubleBufferedShape, m_pCurrentObjectVisuals, N);
        }
        else if (std::string(N.name()) == "shape_visuals_terrain")
        {
            this->createVisualsTerrain(m_pCurrentDoubleBufferedShape, m_pCurrentObjectVisuals, N);
        }
        else if (std::string(N.name()) == "shape_visuals_polygon")
        {
            this->createVisualsPolygon(m_pCurrentDoubleBufferedShape, m_pCurrentObjectVisuals, N);
        }
        
        N = N.next_sibling();
    }    

    if (_Mode == IMPORT_MODE_UNIVERSE)
    {
        // Camera hook must have been read by now
        // Set hook, if camera given. Otherwise a default cam is used.
        if (m_pCamera == nullptr)
        {
            m_pCamera = new CCamera;
            MEM_ALLOC("CCamera")
            m_pCamera->setViewport(600.0,400.0);
            
            INFO_MSG("XML Importer", "Default Camera created.")
        }
        // Handle all hooks
        for (auto ci = m_Hooks.cbegin(); ci != m_Hooks.cend(); ++ci)
        {
            std::string strHookable = (*ci).first;
            if (strHookable != "no_hook")
            {
                auto it = m_pDataStorage->getDynamicObjects().find(strHookable);
                if (it != m_pDataStorage->getDynamicObjects().end())
                {
                    (*ci).second->hook((*it).second);
                }
                else
                {
                    it = m_pDataStorage->getStaticObjects().find(strHookable);
                    if (it != m_pDataStorage->getStaticObjects().end())
                    {
                        (*ci).second->hook((*it).second);
                    }
                    else
                    {
                        WARNING_MSG("XML Importer", "Hook to unknown hookable: " << strHookable)
                    }
                }
            }
        }
        for (auto ci = m_ThrusterHooks.cbegin(); ci != m_ThrusterHooks.cend(); ++ci)
        {
            std::string strHookable = (*ci).second;
            if (strHookable != "no_hook")
            {
                auto it = m_pDataStorage->getDynamicObjects().find(strHookable);
                if (it != m_pDataStorage->getDynamicObjects().end())
                {
                    (*ci).first->IObjectReferrer::attachTo((*it).second);
                }
                else
                {
                    it = m_pDataStorage->getStaticObjects().find(strHookable);
                    if (it != m_pDataStorage->getStaticObjects().end())
                    {
                        (*ci).first->IObjectReferrer::attachTo((*it).second);
                    }
                    else
                    {
                        WARNING_MSG("XML Importer", "Hook to unknown hookable: " << strHookable)
                    }
                }
            }
        }
        for (auto ci = m_KinematicsStateReferences.cbegin(); ci != m_KinematicsStateReferences.cend(); ++ci)
        {
            std::string strRef = (*ci).second;
            if (strRef != "no_ref")
            {
                auto it = m_pDataStorage->getDynamicObjects().find(strRef);
                if (it != m_pDataStorage->getDynamicObjects().end())
                {
                    (*ci).first->getKinematicsState().referTo((*it).second->getKinematicsState());
                }
                else
                {
                    it = m_pDataStorage->getStaticObjects().find(strRef);
                    if (it != m_pDataStorage->getStaticObjects().end())
                    {
                        (*ci).first->getKinematicsState().referTo((*it).second->getKinematicsState());
                    }
                    else
                    {
                        WARNING_MSG("XML Importer", "Reference to unknown kinematics state user: " << strRef)
                    }
                }
            }
        }
    }
    
    // Restore path -- kind of stack pop, when recursively calling import
    m_strPath = strPath;

    return true;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Check node for attribute
///
/// The method will check a given attribute for its existence in the xml file.
/// If existent, it will be returned, otherwise, a default value is given.
///
/// \param _Node Current node in xml tree
/// \param _strAttr Attribute
/// \param _bDef Default value
/// \param _bNotice Give notice if attribute not given
///
/// \return Attribute
///
////////////////////////////////////////////////////////////////////////////////
const bool CXMLImporter::checkAttributeBool(const pugi::xml_node& _Node, 
                                          const std::string& _strAttr,
                                          const bool& _bDef,
                                          const bool _bNotice) const
{
    METHOD_ENTRY("CXMLImporter::checkAttributeBool")
    if (_Node.attribute(_strAttr.c_str()).empty())
    {
        if (_bNotice)
        {
            NOTICE_MSG("XML Importer", "Attribute " << _strAttr << " not found. Using"
                                       " default value " << _bDef << ".")
        }
        else
        {
            DEBUG_MSG("XML Importer", "Attribute " << _strAttr << " not found. Using"
                                      " default value " << _bDef << ".")
        }
        return _bDef;
    }
    else
    {
        bool bRet = _Node.attribute(_strAttr.c_str()).as_bool();
        DOM_VAR(DEBUG_MSG("XML Importer", "Attribute " << _strAttr << " = " << bRet << "."))
        return bRet;
    }
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Check node for attribute
///
/// The method will check a given attribute for its existence in the xml file.
/// If existent, it will be returned, otherwise, a default value is given.
///
/// \param _Node Current node in xml tree
/// \param _strAttr Attribute
/// \param _fDef Default value
/// \param _bNotice Give notice if attribute not given
///
/// \return Attribute
///
////////////////////////////////////////////////////////////////////////////////
const double CXMLImporter::checkAttributeDouble(const pugi::xml_node& _Node, 
                                          const std::string& _strAttr,
                                          const double& _fDef,
                                          const bool _bNotice) const
{
    METHOD_ENTRY("CXMLImporter::checkAttributeDouble")
    
    bool        bFound = false;
    double      fUnitConversion = 1.0;
    std::string strUnit("");
    
    
    if (!_Node.attribute((_strAttr+"__deg").c_str()).empty())
    {
        bFound = true;
        fUnitConversion = M_PI / 180.0;
        strUnit = "__deg";
    }
    if (!_Node.attribute(_strAttr.c_str()).empty())
        bFound = true;
    
    if (!bFound)
    {
        if (_bNotice)
        {
            NOTICE_MSG("XML Importer", "Attribute " << _strAttr << " not found. Using"
                                       " default value " << _fDef << ".")
        }
        else
        {
            DEBUG_MSG("XML Importer", "Attribute " << _strAttr << " not found. Using"
                                      " default value " << _fDef << ".")
        }
        return _fDef;
    }
    else
    {
        double fRet = _Node.attribute((_strAttr+strUnit).c_str()).as_double()*fUnitConversion;
        DOM_VAR(DEBUG_MSG("XML Importer", "Attribute " << _strAttr << " = " << fRet << "."))
        return fRet;
    }
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Check node for attribute
///
/// The method will check a given attribute for its existence in the xml file.
/// If existent, it will be returned, otherwise, a default value is given.
///
/// \param _Node Current node in xml tree
/// \param _strAttr Attribute
/// \param _nDef Default value
/// \param _bNotice Give notice if attribute not given
///
/// \return Attribute
///
////////////////////////////////////////////////////////////////////////////////
const int CXMLImporter::checkAttributeInt(const pugi::xml_node& _Node, 
                                          const std::string& _strAttr,
                                          const int& _nDef,
                                          const bool _bNotice) const
{
    METHOD_ENTRY("CXMLImporter::checkAttributeInt")
    if (_Node.attribute(_strAttr.c_str()).empty())
    {
        if (_bNotice)
        {
            NOTICE_MSG("XML Importer", "Attribute " << _strAttr << " not found. Using"
                                       " default value " << _nDef << ".")
        }
        else
        {
            DEBUG_MSG("XML Importer", "Attribute " << _strAttr << " not found. Using"
                                      " default value " << _nDef << ".")
        }
        return _nDef;
    }
    else
    {
        int nRet = _Node.attribute(_strAttr.c_str()).as_int();
        DOM_VAR(DEBUG_MSG("XML Importer", "Attribute " << _strAttr << " = " << nRet << "."))
        return nRet;
    }
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Check node for attribute
///
/// The method will check a given attribute for its existence in the xml file.
/// If existent, it will be returned, otherwise, a default value is given.
///
/// \param _Node Current node in xml tree
/// \param _strAttr Attribute
/// \param _strDef Default value
/// \param _bNotice Give notice if attribute not given
///
/// \return Attribute
///
////////////////////////////////////////////////////////////////////////////////
const std::string CXMLImporter::checkAttributeString(const pugi::xml_node& _Node, 
                                          const std::string& _strAttr,
                                          const std::string& _strDef,
                                          const bool _bNotice) const
{
    METHOD_ENTRY("CXMLImporter::checkAttributeString")
    if (_Node.attribute(_strAttr.c_str()).empty())
    {
        if (_bNotice)
        {
            NOTICE_MSG("XML Importer", "Attribute " << _strAttr << " not found. Using"
                                       " default value " << _strDef << ".")
        }
        else
        {
            DEBUG_MSG("XML Importer", "Attribute " << _strAttr << " not found. Using"
                                       " default value " << _strDef << ".")
        }
        return _strDef;
    }
    else
    {
        std::string strRet = _Node.attribute(_strAttr.c_str()).as_string();
        DOM_VAR(DEBUG_MSG("XML Importer", "Attribute " << _strAttr << " = " << strRet << "."))
        return strRet;
    }
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
    
    if (!_Node.attribute("file").empty())
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
 
    if (!checkFile(_Node))
    {
        INFO_MSG("XML Importer", "Creating camera.")
    
        // Free memory if pointer is already existent
        if (m_pCamera != nullptr)
        {
            delete m_pCamera;
            m_pCamera = 0;
            MEM_FREED("CCamera");
            NOTICE_MSG("XML Importer", "More than one camera, creating new one, deleting old one.")
        }
        m_pCamera = new CCamera;
        MEM_ALLOC("CCamera")
        
        m_strCameraHook = _Node.attribute("hook").as_string();
        m_pCamera->setPosition(_Node.attribute("position_x").as_double(),
                              _Node.attribute("position_y").as_double());
        m_pCamera->getKinematicsState().setAngle(checkAttributeDouble(_Node,"angle", 0.0, false));
        m_pCamera->setViewport(_Node.attribute("viewport_width").as_int(),
                              _Node.attribute("viewport_height").as_int());
        m_pCamera->zoomTo(GRAPHICS_PX_PER_METER/checkAttributeDouble(_Node, "m_per_px", 0.5, false));
//         if (checkAttributeBool(_Node, "enable_angle_hook", true)) m_pCamera->enableAngleHook();
//         else m_pCamera->disableAngleHook();
        
        m_Hooks.insert(std::pair<std::string,IKinematicsStateUser*>(
            checkAttributeString(_Node, "hook", "no_hook", XML_IMPORTER_DO_NOT_NOTICE),
            m_pCamera
        ));
    } // if (!checkFile(_Node))
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Create an emitter
///
/// \param _Node Current node in xml tree
///
////////////////////////////////////////////////////////////////////////////////
void CXMLImporter::createEmitter(const pugi::xml_node& _Node)
{
    METHOD_ENTRY("CXMLImporter::createEmitter")
    
    if (!checkFile(_Node))
    {
        if (!_Node.empty())
        {
            std::string strType = checkAttributeString(_Node, "type", mapEmitterToString.at(EMITTER_DEFAULT_TYPE));
            if (strType == "object_emitter")
            {
                INFO_MSG("XML Importer", "Creating object emitter.")
                
                CObjectEmitter* pObjEmitter = new CObjectEmitter;
                MEM_ALLOC("CObjectEmitter")
                
                pObjEmitter->setName(checkAttributeString(_Node, "name", pObjEmitter->getName()));
                
                std::string strMode = checkAttributeString(_Node, "mode", mapEmitterModeToString.at(EMITTER_DEFAULT_MODE));
                if (strMode == "emit_once")
                {
                    pObjEmitter->setMode(EMITTER_MODE_EMIT_ONCE);
                }
                else if (strMode == "timed")
                {
                    pObjEmitter->setMode(EMITTER_MODE_TIMED);
                    pObjEmitter->setFrequency(checkAttributeDouble(_Node, "frequency", EMITTER_DEFAULT_FREQUENCY));
                }
                else
                {
                    WARNING_MSG("XML Importer", "Unknown mode " << strMode << ". Using default mode: " << 
                                                mapEmitterModeToString.at(EMITTER_DEFAULT_MODE))
                    WARNING(
                        std::cout << "  Known modes: " << std::endl;
                        for (auto it = mapEmitterModeToString.cbegin(); it != mapEmitterModeToString.cend(); ++it)
                        {
                            std::cout << "    " << (*it).second << std::endl;
                        }
                    )
                }
                
                std::string strDist = checkAttributeString(_Node, "distribution", mapEmitterDistributionToString.at(EMITTER_DEFAULT_DISTRIBUTION));
                if (strDist == "circular_field")
                {
                    NOTICE_MSG("XML Importer", "Circular distribution not yet implemented.")
                    pObjEmitter->setDistribution(EMITTER_DISTRIBUTION_CIRCULAR_FIELD);
                }
                else if (strDist == "point_source")
                {
                    pObjEmitter->setDistribution(EMITTER_DISTRIBUTION_POINT_SOURCE);
                    pObjEmitter->setOrigin(Vector2d(
                                            checkAttributeDouble(_Node, "origin_x", 0.0),
                                            checkAttributeDouble(_Node, "origin_y", 0.0)));
                    pObjEmitter->setVelocity(checkAttributeDouble(_Node, "velocity", EMITTER_DEFAULT_VELOCITY));
                    pObjEmitter->setVelocityStd(checkAttributeDouble(_Node, "velocity_std", EMITTER_DEFAULT_VELOCITY_STD));
                    pObjEmitter->setAngle((checkAttributeDouble(_Node, "angle", EMITTER_DEFAULT_ANGLE)));
                    pObjEmitter->setAngleStd((checkAttributeDouble(_Node, "angle_std", EMITTER_DEFAULT_ANGLE_STD)));
                }
                else if (strDist == "rectangular_field")
                {
                    pObjEmitter->setDistribution(EMITTER_DISTRIBUTION_RECTANGULAR_FIELD);
                    pObjEmitter->setOrigin(Vector2d(
                                            checkAttributeDouble(_Node, "origin_x", 0.0),
                                            checkAttributeDouble(_Node, "origin_y", 0.0)));
                    pObjEmitter->setLimits(checkAttributeDouble(_Node, "limit_x_min", EMITTER_DEFAULT_LIMIT_MIN_X),
                                          checkAttributeDouble(_Node, "limit_x_max", EMITTER_DEFAULT_LIMIT_MAX_X),
                                          checkAttributeDouble(_Node, "limit_y_min", EMITTER_DEFAULT_LIMIT_MIN_Y),
                                          checkAttributeDouble(_Node, "limit_y_max", EMITTER_DEFAULT_LIMIT_MAX_Y));
                    pObjEmitter->setNumber(_Node.attribute("number").as_uint());
                }
                else
                {
                    WARNING_MSG("XML Importer", "Unknown distribution " << strDist << ". Using default distribution: " <<
                                                mapEmitterDistributionToString.at(EMITTER_DEFAULT_DISTRIBUTION))
                    WARNING(
                        std::cout << "  Known distributions: " << std::endl;
                        for (auto it = mapEmitterDistributionToString.cbegin(); it != mapEmitterDistributionToString.cend(); ++it)
                        {
                            std::cout << "    " << (*it).second << std::endl;
                        }
                    )
                }
                m_Hooks.insert(std::pair<std::string,IKinematicsStateUser*>(
                    checkAttributeString(_Node, "hook", "no_hook", XML_IMPORTER_DO_NOT_NOTICE),
                    pObjEmitter
                ));
                
                m_pCurrentEmitter = pObjEmitter;
                m_Emitters.insert(std::pair<std::string,IEmitter*>(pObjEmitter->getName(), pObjEmitter));
            }
            else if (strType == "debris_emitter")
            {
                INFO_MSG("XML Importer", "Creating debris emitter.")
                
                CDebrisEmitter* pDebrisEmitter = new CDebrisEmitter;
                MEM_ALLOC("CDebrisEmitter")
                
                pDebrisEmitter->setName(checkAttributeString(_Node, "name", pDebrisEmitter->getName()));
                
                std::string strDebrisType = checkAttributeString(_Node, "debris_type", mapDebrisTypeToString.at(pDebrisEmitter->getDebrisType()));
                pDebrisEmitter->setDebrisType(mapStringToDebrisType.at(strDebrisType));
                
                std::string strMode = checkAttributeString(_Node, "mode", mapEmitterModeToString.at(EMITTER_DEFAULT_MODE));
                if (strMode == "emit_once")
                {
                    pDebrisEmitter->setMode(EMITTER_MODE_EMIT_ONCE);
                }
                else if (strMode == "timed")
                {
                    pDebrisEmitter->setMode(EMITTER_MODE_TIMED);
                    pDebrisEmitter->setFrequency(checkAttributeDouble(_Node, "frequency", EMITTER_DEFAULT_FREQUENCY));
                }
                else
                {
                    WARNING_MSG("XML Importer", "Unknown mode " << strMode << ". Using default mode: " << 
                                                mapEmitterModeToString.at(EMITTER_DEFAULT_MODE))
                    WARNING(
                        std::cout << "  Known modes: " << std::endl;
                        for (auto it = mapEmitterModeToString.cbegin(); it != mapEmitterModeToString.cend(); ++it)
                        {
                            std::cout << "    " << (*it).second << std::endl;
                        }
                    )
                }
                
                std::string strDist = checkAttributeString(_Node, "distribution", mapEmitterDistributionToString.at(EMITTER_DEFAULT_DISTRIBUTION));
                if (strDist == "circular_field")
                {
                    NOTICE_MSG("XML Importer", "Circular distribution not yet implemented.")
                    pDebrisEmitter->setDistribution(EMITTER_DISTRIBUTION_CIRCULAR_FIELD);
                }
                else if (strDist == "point_source")
                {
                    pDebrisEmitter->setDistribution(EMITTER_DISTRIBUTION_POINT_SOURCE);
                    pDebrisEmitter->setOrigin(Vector2d(
                                              checkAttributeDouble(_Node, "origin_x", 0.0),
                                              checkAttributeDouble(_Node, "origin_y", 0.0)));
                    pDebrisEmitter->setVelocity(checkAttributeDouble(_Node, "velocity", EMITTER_DEFAULT_VELOCITY));
                    pDebrisEmitter->setVelocityStd(checkAttributeDouble(_Node, "velocity_std", EMITTER_DEFAULT_VELOCITY_STD));
                    pDebrisEmitter->setAngle((checkAttributeDouble(_Node, "angle", EMITTER_DEFAULT_ANGLE)));
                    pDebrisEmitter->setAngleStd((checkAttributeDouble(_Node, "angle_std", EMITTER_DEFAULT_ANGLE_STD)));
                    pDebrisEmitter->setNumber(checkAttributeInt(_Node, "number", DEBRIS_DEFAULT_NUMBER));
                }
                else if (strDist == "rectangular_field")
                {
                    pDebrisEmitter->setDistribution(EMITTER_DISTRIBUTION_RECTANGULAR_FIELD);
                    pDebrisEmitter->setOrigin(Vector2d(
                                              checkAttributeDouble(_Node, "origin_x", 0.0),
                                              checkAttributeDouble(_Node, "origin_y", 0.0)));
                    pDebrisEmitter->setLimits(checkAttributeDouble(_Node, "limit_x_min", EMITTER_DEFAULT_LIMIT_MIN_X),
                                          checkAttributeDouble(_Node, "limit_x_max", EMITTER_DEFAULT_LIMIT_MAX_X),
                                          checkAttributeDouble(_Node, "limit_y_min", EMITTER_DEFAULT_LIMIT_MIN_Y),
                                          checkAttributeDouble(_Node, "limit_y_max", EMITTER_DEFAULT_LIMIT_MAX_Y));
                    pDebrisEmitter->setNumber(checkAttributeInt(_Node, "number", DEBRIS_DEFAULT_NUMBER));
                }
                else
                {
                    WARNING_MSG("XML Importer", "Unknown distribution " << strDist << ". Using default distribution: " <<
                                                mapEmitterDistributionToString.at(EMITTER_DEFAULT_DISTRIBUTION))
                    WARNING(
                        std::cout << "  Known distributions: " << std::endl;
                        for (auto it = mapEmitterDistributionToString.cbegin(); it != mapEmitterDistributionToString.cend(); ++it)
                        {
                            std::cout << "    " << (*it).second << std::endl;
                        }
                    )
                }
                m_Hooks.insert(std::pair<std::string,IKinematicsStateUser*>(
                    checkAttributeString(_Node, "hook", "no_hook", XML_IMPORTER_DO_NOT_NOTICE),
                    pDebrisEmitter
                ));
                m_pCurrentEmitter = pDebrisEmitter;
                m_Emitters.insert(std::pair<std::string,IEmitter*>(pDebrisEmitter->getName(), pDebrisEmitter));
            }
            else
            {
                WARNING_MSG("XML Importer", "Unknown emitter " << strType << ". Aborting creation.")
                WARNING(
                    std::cout << "  Known emitters: " << std::endl;
                    for (auto it = mapEmitterToString.cbegin(); it != mapEmitterToString.cend(); ++it)
                    {
                        std::cout << "    " << (*it).second << std::endl;
                    }
                )
            }
        }
    } // if (!checkFile(_Node))
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Create a component
///
/// \param _Node Current node in xml tree
///
////////////////////////////////////////////////////////////////////////////////
void CXMLImporter::createComponent(const pugi::xml_node& _Node)
{
    METHOD_ENTRY("CXMLImporter::createComponent")

    if (!checkFile(_Node))
    {
        INFO_MSG("XML Importer", "Creating component.")
        
        std::string strThrusterHook = checkAttributeString(_Node, "hook", "no_hook");
        
        // Check, if a thruster hook is given, this is mandatory
        if (strThrusterHook != "no_hook")
        {
            CThruster* pThruster = new CThruster;
            MEM_ALLOC("CThruster")
            
            // Read thruster information
            pThruster->setName(checkAttributeString(_Node, "name", pThruster->getName()));
            pThruster->setAngle(checkAttributeDouble(_Node, "angle", pThruster->getAngle()));
            pThruster->setOrigin(Vector2d(checkAttributeDouble(_Node, "origin_x", pThruster->getOrigin()[0]),
                                          checkAttributeDouble(_Node, "origin_y", pThruster->getOrigin()[1])));
            pThruster->setThrustMax(checkAttributeDouble(_Node, "thrust_max", 1.0));
            
            m_Hooks.insert(std::pair<std::string,IKinematicsStateUser*>(
                strThrusterHook,
                pThruster
            ));
            m_ThrusterHooks.insert(std::pair<CThruster*, std::string>(
                pThruster,
                strThrusterHook
            ));
            
            // Read emitter and body information
            pugi::xml_node N = _Node.first_child();

            while (!N.empty())
            {
                if (std::string(N.name()) == "emitter")
                {
                    this->createEmitter(N);
                    pThruster->IEmitterReferrer::attachTo(m_pCurrentEmitter);
                    
                    // Hook the emitter and ensure it hasn't already been hooked
                    
                    bool bDuplicate = false;
                    auto ciRange = m_Hooks.equal_range(checkAttributeString(_Node, "hook", "no_hook"));
                    auto ci = ciRange.first;
                    while (ci != ciRange.second)
                    {
                      if ((*ci).second == m_pCurrentEmitter) bDuplicate = true;
                      ++ci;
                    }
                    if (bDuplicate)
                    {
                        NOTICE_MSG("XML Importer", "Hook already defined in emitter definition. ")
                    }
                    else
                    {
                        m_Hooks.insert(std::pair<std::string,IKinematicsStateUser*>(
                                              checkAttributeString(_Node, "hook", "no_hook"),
                                              m_pCurrentEmitter
                                          ));
                    }
                }
                
                N = N.next_sibling();
            }
            
            pThruster->setEmitterVelocity(m_pCurrentEmitter->getVelocity());
            pThruster->setEmitterVelocityStd(m_pCurrentEmitter->getVelocityStd());
            
            if (checkAttributeBool(_Node, "active", true) == false) pThruster->deactivate();
            
            // Insert thruster to map
            m_Components.insert(std::pair<std::string,CThruster*>(pThruster->getName(),pThruster));
        }
        else
        {
            WARNING_MSG("XML Importer", "No hook for thruster found, which makes no sense. Thruster won't be created.")
        }
    } // if (!checkFile(_Node))
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
    
    if (!checkFile(_Node))
    {
        INFO_MSG("XML Importer", "Setting constant gravity.")
        
        m_vecGravity = Vector2d(checkAttributeDouble(_Node, "vec_x", 0.0),
                                checkAttributeDouble(_Node, "vec_y", 0.0));
    } // if (!checkFile(_Node))
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
        
    if (!checkFile(_Node))
    {
        INFO_MSG("XML Importer", "Creating rigid body.");
        
        CRigidBody* pRigidBody = new CRigidBody;
        MEM_ALLOC("CRigidBody")
        m_pCurrentBody = pRigidBody;
        
        IObjectVisuals* pObjectVisuals = new IObjectVisuals(pRigidBody);
        MEM_ALLOC("IObjectVisuals");
        m_pCurrentObjectVisuals = pObjectVisuals;
        
        pugi::xml_node N = _Node;
        
        while (!N.empty())
        {
            if (std::string(N.name()) == "core")
            {
                this->readObjectCore(pRigidBody, N);
            }
            else if (std::string(N.name()) == "body_core")
            {
                this->readBodyCore(pRigidBody, N);
            }
            else if (std::string(N.name()) == "shape")
            {
                if (!checkFile(N))
                {
                    std::string strType(N.attribute("type").as_string());
                    if (strType == "shape_planet")
                    {
                        this->createShapePlanet(pRigidBody, pObjectVisuals, N);
                    }
                    else if (strType == "shape_circle")
                    {
                        this->createShapeCircle(pRigidBody, pObjectVisuals, N);
                    }
                    else if (strType == "shape_polygon")
                    {
                        this->createShapePolygon(pRigidBody, pObjectVisuals, N);
                    }
                    else if (strType == "shape_terrain")
                    {
                        this->createShapeTerrain(pRigidBody, pObjectVisuals, N);
                    }
                }
            }        
            
            N = N.next_sibling();
        }
        
        m_pDataStorage->addObject(pRigidBody);
        m_pDataStorage->addObjectVisuals(pObjectVisuals);
    } // if (!checkFile(_Node))
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
    
    if (!checkFile(_Node))
    {
        CCircle* pCircle = new CCircle;
        MEM_ALLOC("IShape")
        
        pCircle->setRadius(_Node.attribute("radius").as_double());
        pCircle->setCenter(_Node.attribute("center_x").as_double(),
                          _Node.attribute("center_y").as_double());
        pCircle->setMass(checkAttributeDouble(_Node, "mass", pCircle->getMass()));
        
        CDoubleBufferedShape* pShape = new CDoubleBufferedShape;
        MEM_ALLOC("CDoubleBufferedShape")
        m_pCurrentDoubleBufferedShape = pShape;
        
        pShape->buffer(pCircle);
            
        // The shape might have visuals
        if (_Node.first_child())
        {
            pugi::xml_node N = _Node.first_child();
            while (!N.empty())
            {
                if (std::string(N.name()) == "visuals")
                {
                    this->createVisualsCircle(pShape, _pObjectVisuals, N);
                }
                N = N.next_sibling();
            }
        }
        
        _pBody->getGeometry()->addShape(pShape);
    } // if (!checkFile(_Node))
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
    
    if (!checkFile(_Node))
    {
        CPlanet* pPlanet = new CPlanet;
        MEM_ALLOC("CPlanet")
        
        pPlanet->setMass(checkAttributeDouble(_Node, "mass", pPlanet->getMass()));
        pPlanet->setRadius(_Node.attribute("radius").as_double());
        pPlanet->setCenter(_Node.attribute("center_x").as_double(),
                          _Node.attribute("center_y").as_double());
        pPlanet->setHeight(_Node.attribute("height_max").as_double());
        pPlanet->setGroundResolution(_Node.attribute("ground_resolution").as_double());
        pPlanet->setSeaLevel(_Node.attribute("sea_level").as_double());
        pPlanet->initTerrain();
        
        CDoubleBufferedShape* pShape = new CDoubleBufferedShape;
        MEM_ALLOC("IShape")
        m_pCurrentDoubleBufferedShape = pShape;
        
        pShape->buffer(pPlanet);
        static_cast<CPlanet*>(pShape->getShapeBuf())->initTerrain();
            
        // The shape might have visuals
        if (_Node.first_child())
        {
            pugi::xml_node N = _Node.first_child();
            while (!N.empty())
            {
                if (std::string(N.name()) == "visuals")
                {
                    this->createVisualsPlanet(pShape, _pObjectVisuals, N);
                }
                N = N.next_sibling();
            }
        }
        
        _pBody->getGeometry()->addShape(pShape);
    } // if (!checkFile(_Node))
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Create a polygon shape
///
/// \param _pBody Body to create the shape for
/// \param _pObjectVisuals Pointer to object visuals to refer shape visuals to
/// \param _Node Current node in xml tree
///
////////////////////////////////////////////////////////////////////////////////
void CXMLImporter::createShapePolygon(CBody* const _pBody,
                                      IObjectVisuals* const _pObjectVisuals,
                                      const pugi::xml_node& _Node)
{
    METHOD_ENTRY("CXMLImporter::createShapePolygon")
    
    if (!checkFile(_Node))
    {
        CPolygon* pPolygon = new CPolygon;
        MEM_ALLOC("CPolygon")
        
        if (std::string(_Node.attribute("polygon_type").as_string()) == "filled")
            pPolygon->setPolygonType(PolygonType::FILLED);
        else if (std::string(_Node.attribute("polygon_type").as_string()) == "line_loop")
            pPolygon->setPolygonType(PolygonType::LINE_LOOP);
        else if (std::string(_Node.attribute("polygon_type").as_string()) == "line_strip")
            pPolygon->setPolygonType(PolygonType::LINE_STRIP);
        else if (std::string(_Node.attribute("polygon_type").as_string()) == "line_single")
            pPolygon->setPolygonType(PolygonType::LINE_SINGLE);
        
        pPolygon->setMass(checkAttributeDouble(_Node, "mass", pPolygon->getMass()));
        
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
            pPolygon->addVertex(fX,fY);
        }
        
        CDoubleBufferedShape* pShape = new CDoubleBufferedShape;
        MEM_ALLOC("CDoubleBufferedShape")
        m_pCurrentDoubleBufferedShape = pShape;
        
        pShape->buffer(pPolygon);
        
        // The shape might have visuals
        if (!_Node.empty())
        {
            pugi::xml_node N = _Node.first_child();
            while (!N.empty())
            {
                if (std::string(N.name()) == "visuals")
                {
                    this->createVisualsPolygon(pShape, _pObjectVisuals, N);
                }
                N = N.next_sibling();
            }
        }
        
        _pBody->getGeometry()->addShape(pShape);
    } // if (!checkFile(_Node))
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
    
    if (!checkFile(_Node))
    {
        CTerrain* pTerrain = new CTerrain;
        MEM_ALLOC("CTerrain")
        
        pTerrain->setMass(checkAttributeDouble(_Node, "mass", pTerrain->getMass()));
        pTerrain->setWidth (_Node.attribute("width").as_double());
        pTerrain->setCenter(_Node.attribute("center_x").as_double(),
                            _Node.attribute("center_y").as_double());
        pTerrain->setHeight(_Node.attribute("height_max").as_double());
        pTerrain->setDiversity(_Node.attribute("diversity").as_double());
        pTerrain->setGroundResolution(_Node.attribute("ground_resolution").as_double());

        CDoubleBufferedShape* pShape = new CDoubleBufferedShape;
        MEM_ALLOC("CDoubleBufferedShape")
        m_pCurrentDoubleBufferedShape = pShape;
        
        pShape->buffer(pTerrain);
        
        // The shape might have visuals
        if (_Node.first_child())
        {
            pugi::xml_node N = _Node.first_child();
            while (!N.empty())
            {
                if (std::string(N.name()) == "visuals")
                {
                    this->createVisualsTerrain(pShape, _pObjectVisuals, N);
                }
                N = N.next_sibling();
            }
        }
        
        _pBody->getGeometry()->addShape(pShape);
    } // if (!checkFile(_Node))
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
void CXMLImporter::createVisualsCircle(CDoubleBufferedShape* const _pCircle,
                                       IObjectVisuals* const _pObjectVisuals,
                                       const pugi::xml_node& _Node)
{
    METHOD_ENTRY("CXMLImporter::createVisualsCircle")
    
    if (!checkFile(_Node))
    {
        if (!_Node.empty())
        {
            if (std::string(_Node.attribute("type").as_string()) == "shape_visuals_circle")
            {
                CCircleVisuals* pCircleVisuals = new CCircleVisuals(_pCircle);
                MEM_ALLOC("CCircleVisuals")
                
                _pObjectVisuals->addVisuals(pCircleVisuals);
            }
        }
    } // if (!checkFile(_Node))
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
void CXMLImporter::createVisualsPlanet(CDoubleBufferedShape* const _pPlanet,
                                       IObjectVisuals* const _pObjectVisuals,
                                       const pugi::xml_node& _Node)
{
    METHOD_ENTRY("CXMLImporter::createVisualsPlanet")
    
    if (!checkFile(_Node))
    {
        if (!_Node.empty())
        {
            if (std::string(_Node.attribute("type").as_string()) == "shape_visuals_planet")
            {
                CPlanetVisuals* pPlanetVisuals = new CPlanetVisuals(_pPlanet);
                MEM_ALLOC("CPlanetVisuals")
                
                _pObjectVisuals->addVisuals(pPlanetVisuals);
            }
        }
    } // if (!checkFile(_Node))
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Create visuals to a polygon shape
///
/// \param _pPolygon Polygon to create visuals for
/// \param _pObjectVisuals Pointer to object visuals to refer shape visuals to
/// \param _Node Current node in xml tree
///
////////////////////////////////////////////////////////////////////////////////
void CXMLImporter::createVisualsPolygon(CDoubleBufferedShape* const _pPolygon,
                                         IObjectVisuals* const _pObjectVisuals,
                                         const pugi::xml_node& _Node)
{
    METHOD_ENTRY("CXMLImporter::createVisualsPolygon")

    if (!checkFile(_Node))
    {
        if (!_Node.empty())
        {
            if (std::string(_Node.attribute("type").as_string()) == "shape_visuals_polygon")
            {
                CPolygonVisuals* pPolygonVisuals = new CPolygonVisuals(_pPolygon);
                MEM_ALLOC("CPolygonVisuals")
                
                _pObjectVisuals->addVisuals(pPolygonVisuals);
            }
        }
    } // if (!checkFile(_Node))
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
void CXMLImporter::createVisualsTerrain(CDoubleBufferedShape* const _pTerrain,
                                        IObjectVisuals* const _pObjectVisuals,
                                        const pugi::xml_node& _Node)
{
    METHOD_ENTRY("CXMLImporter::createVisualsTerrain")
 
    if (!checkFile(_Node))
    {
        if (!_Node.empty())
        {
            if (std::string(_Node.attribute("type").as_string()) == "shape_visuals_terrain")
            {
                CTerrainVisuals* pTerrainVisuals = new CTerrainVisuals(_pTerrain);
                MEM_ALLOC("CTerrainVisuals")
                
                _pObjectVisuals->addVisuals(pTerrainVisuals);
            }
                
        }
    } // if (!checkFile(_Node))
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
    
    if (!checkFile(_Node))
    {
        if (!_Node.empty())
        {
            if (checkAttributeBool(_Node, "procedural_generation", false) == true)
            {
                m_Universe.generate(_Node.attribute("seed").as_int(),
                                    _Node.attribute("number_of_stars").as_int());
                INFO_MSG("XML Importer", "Procedural universe generated.")
            }
        }
    } // if (!checkFile(_Node))
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Read information for object bodies.
///
/// While object core holds information of a point mass, body core adds
/// information depending on volume (here area) like inertia.
///
/// \param _pO Object to read body information for
/// \param _Node Current node in xml tree
///
////////////////////////////////////////////////////////////////////////////////
void CXMLImporter::readBodyCore(CRigidBody* const _pO, const pugi::xml_node& _Node)
{
    METHOD_ENTRY("CXMLImporter::readBodyCore")
    
    if (!_Node.empty())
    {
        _pO->setAngle(checkAttributeDouble(_Node, "angle", _pO->getAngle()));
        _pO->setAngleVelocity(checkAttributeDouble(_Node, "angle_velocity", _pO->getAngleVelocity()));
        
        double fInertia = checkAttributeDouble(_Node, "inertia", _pO->getInertia());
        DOM_VAR(NOTICE_MSG("XML Importer", "Inertia is be calculated automatically now, " <<
                                           "value " << fInertia << " ignored."))
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
void CXMLImporter::readObjectCore(CRigidBody* const _pO, const pugi::xml_node& _Node)
{
    METHOD_ENTRY("CXMLImporter::readObjectCore")
    
    if (!_Node.empty())
    {
        std::string strRef = checkAttributeString(_Node, "kinematics_reference", "no_ref");
        if (strRef != "no_ref")
            m_KinematicsStateReferences.insert(
            std::pair<IKinematicsStateUser*, std::string>(_pO, strRef));
        
        _pO->setName(checkAttributeString(_Node, "name", _pO->getName()));
        
        // Origin might be given with no respect to grid, since user shouldn't have
        // to care unless he really need to place objects with high precision far
        // from the origin of the coordinate system. Therefore, the given values
        // are separated into origin and cell if neccessary.
        Vector2d vecGivenOrigin;
        Vector2d vecOrigin;
        Vector2i vecCell;
        vecGivenOrigin[0] = checkAttributeDouble(_Node, "origin_x", _pO->getOrigin()[0]);
        vecGivenOrigin[1] = checkAttributeDouble(_Node, "origin_y", _pO->getOrigin()[1]);
        IObject::separateCenterCell(vecGivenOrigin, vecOrigin, vecCell);
        
        DOM_VAR(DEBUG_MSG("XML Importer", "Separating origin (" <<
                           vecGivenOrigin[0] << ", " << vecGivenOrigin[1] <<
                           ") into origin (" <<
                           vecOrigin[0] << ", " << vecOrigin[1] <<
                           ") and cell (" <<
                           vecCell[0] << ", " << vecCell[1] <<
                           ")."
                ))
        _pO->setOrigin(vecOrigin);
        // Set cell, but do not notice if attribute is missing. It is optional, since
        // the cell might be implicitly given by position, it shouldn't be neccessarily
        // exposed to the user.
        _pO->setCell(Vector2i(
                     checkAttributeInt(_Node, "cell_x", _pO->getCell()[0], XML_IMPORTER_DO_NOT_NOTICE),
                     checkAttributeInt(_Node, "cell_y", _pO->getCell()[1], XML_IMPORTER_DO_NOT_NOTICE)) +
                     vecCell);
                                
        if (checkAttributeBool(_Node, "gravity", _pO->getGravitationState()) == true)
            _pO->enableGravitation();
        else
            _pO->disableGravitation();
        if (checkAttributeBool(_Node, "dynamics", _pO->getDynamicsState()) == true)
            _pO->enableDynamics();
        else
            _pO->disableDynamics();
        _pO->setVelocity(Vector2d(
                         checkAttributeDouble(_Node, "velocity_x", _pO->getVelocity()[0]),
                         checkAttributeDouble(_Node, "velocity_y", _pO->getVelocity()[1])));
    }
}
