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
#include <map>

//--- Program header ---------------------------------------------------------//
#include "particle_emitter.h"
#include "objects_emitter.h"
#include "thruster.h"
#include "universe.h"
#include "visuals_data_storage_user.h"
#include "world_data_storage_user.h"

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
class CXMLImporter : public IVisualsDataStorageUser,
                     public IWorldDataStorageUser
{

    public:
   
        //--- Constructor/Destructor -----------------------------------------//
        CXMLImporter();
        ~CXMLImporter();

        //--- Constant Methods -----------------------------------------------//
        Vector2d                        getGravity() const;
        const ComponentsType&           getComponents() const;
        const EmittersType&             getEmitters() const;
        const double&                   getFrequencyParticle() const;
        const double&                   getFrequencyLua() const;
        const std::string&              getGraphicsDataPath() const;
        const double&                   getPhysicsFrequency() const;
        const std::string&              getPhysicsInterface() const;
        const CUniverse&                getUniverse() const;
        const double&                   getVisualsFrequency() const;
                
        //--- Methods --------------------------------------------------------//
        bool import(const std::string&,
                    const ImportModeType& = IMPORT_MODE_UNIVERSE);
        
    private:
        
        bool          checkAttributeBool(const pugi::xml_node&,
                              const std::string&,
                              const bool&,
                              const bool = true
                             ) const;
        double        checkAttributeDouble(const pugi::xml_node&,
                              const std::string&,
                              const double&,
                              const bool = true
                             ) const;
        int           checkAttributeInt(const pugi::xml_node&,
                              const std::string&,
                              const int&,
                              const bool = true
                             ) const;
        std::string   checkAttributeString(const pugi::xml_node&,
                              const std::string&,
                              const std::string&,
                              const bool = true
                             ) const;

        bool checkFile(const pugi::xml_node&);
        void createCamera(const pugi::xml_node&);
        void createComponent(const pugi::xml_node&);
        void createEmitter(const pugi::xml_node&);
        void createGravity(const pugi::xml_node&);
        void createShapeCircle(CObject* const, const pugi::xml_node&);
        void createShapePlanet(CObject* const, const pugi::xml_node&);
        void createShapeTerrain(CObject* const, const pugi::xml_node&);
        void createShapePolygon(CObject* const, const pugi::xml_node&);
        void createRigidBody(const pugi::xml_node&);
        void createUniverse(const pugi::xml_node&);
        void readBodyCore(CObject* const, const pugi::xml_node&);
        void readObjectCore(CObject* const, const pugi::xml_node&);
        
        IEmitter*               m_pCurrentEmitter;              ///< Temporary variable for state machine
        CObject*                m_pCurrentObject;               ///< Temporary variable for state machine
        IShape*                 m_pCurrentShape;                ///< Temporary variable for state machine
        
        CUniverse                       m_Universe;             ///< The procedurally generated universe
        CCamera*                        m_pCamera;              ///< Main camera
        Vector2d                        m_vecGravity;           ///< Constant gravity vector
        std::string                     m_strCameraHook;        ///< Camera hook
        ComponentsType                  m_Components;           ///< List of components
        EmittersType                    m_Emitters;             ///< List of emitters
        std::string                     m_strPath;              ///< Path to read data from
        std::string                     m_strGraphicsDataPath;              ///< Font to load
        std::string                     m_strLuaPhysicsInterface; ///< Lua file for physics access
        double                          m_fParticleFrequency;     ///< Frequency for particle processing
        double                          m_fLuaFrequency;        ///< Frequency for lua interface
        double                          m_fPhysicsFrequency;    ///< Frequency for physics processing
        double                          m_fVisualsFrequency;    ///< Frequency for visual update
        
        std::unordered_multimap<std::string, IKinematicsStateUser*> m_Hooks;                     ///< List of hooks and related object names
        std::unordered_multimap<IKinematicsStateUser*, std::string> m_KinematicsStateReferences; ///< List of states to be referred to others
        std::unordered_map<IObjectReferrer*, std::string> m_ObjectReferrers;                     ///< List of object referrers
        ObjectsByNameType   m_ObjectsByName; ///< List of objects, accessed by name
};

//--- Implementation is done here for inline optimisation --------------------//

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
/// \brief Return imported components
///
/// \return List of components
///
////////////////////////////////////////////////////////////////////////////////
inline const ComponentsType& CXMLImporter::getComponents() const
{
    METHOD_ENTRY("CXMLImporter::getComponents")
    return m_Components;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Return imported emitters
///
/// \return List of emitters
///
////////////////////////////////////////////////////////////////////////////////
inline const EmittersType& CXMLImporter::getEmitters() const
{
    METHOD_ENTRY("CXMLImporter::getEmitters")
    return m_Emitters;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Return frequency for particle physics processing
///
/// \return Frequency for particle physics processing
///
////////////////////////////////////////////////////////////////////////////////
inline const double& CXMLImporter::getFrequencyParticle() const
{
    METHOD_ENTRY("CXMLImporter::getFrequencyParticle")
    return m_fParticleFrequency;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Return frequency for Lua interface
///
/// \return Frequency for Lua interface
///
////////////////////////////////////////////////////////////////////////////////
inline const double& CXMLImporter::getFrequencyLua() const
{
    METHOD_ENTRY("CXMLImporter::getFrequencyLua")
    return m_fLuaFrequency;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Return path for graphics data such as fonts and shaders
///
/// \return Name and location of font
///
////////////////////////////////////////////////////////////////////////////////
inline const std::string& CXMLImporter::getGraphicsDataPath() const
{
    METHOD_ENTRY("CXMLImporter::getGraphicsDataPath")
    return m_strGraphicsDataPath;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Return frequency for physics calculations
///
/// \return Frequency for physics calculations
///
////////////////////////////////////////////////////////////////////////////////
inline const double& CXMLImporter::getPhysicsFrequency() const
{
    METHOD_ENTRY("CXMLImporter::getPhysicsFrequency")
    return m_fPhysicsFrequency;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Return Lua physics interface
///
/// \return Lua physics interface
///
////////////////////////////////////////////////////////////////////////////////
inline const std::string& CXMLImporter::getPhysicsInterface() const
{
    METHOD_ENTRY("CXMLImporter::getPhysicsInterface")
    return m_strLuaPhysicsInterface;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Return procedurally generated universe
///
/// \return Procedurally generated universe
///
////////////////////////////////////////////////////////////////////////////////
inline const CUniverse& CXMLImporter::getUniverse() const
{
    METHOD_ENTRY("CXMLImporter::getUniverse")
    return m_Universe;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Return frequency for visual update
///
/// \return Frequency for visual update
///
////////////////////////////////////////////////////////////////////////////////
inline const double& CXMLImporter::getVisualsFrequency() const
{
    METHOD_ENTRY("CXMLImporter::getVisualsFrequency")
    return m_fVisualsFrequency;
}


#endif
