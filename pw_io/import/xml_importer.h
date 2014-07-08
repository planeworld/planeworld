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
#include <map>

//--- Program header ---------------------------------------------------------//
#include "body.h"
#include "debris_emitter.h"
#include "objects_emitter.h"
#include "universe.h"
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
class CXMLImporter : public IWorldDataStorageUser
{

    public:
   
        //--- Constructor/Destructor -----------------------------------------//
        CXMLImporter();
        ~CXMLImporter();

        //--- Constant Methods -----------------------------------------------//
        CCamera*                        getCamera() const;
        Vector2d                        getGravity() const;
        const EmittersType&             getEmitters() const;
        const double&                   getFrequencyDebris() const;
        const double&                   getFrequencyLua() const;
        const double&                   getPhysicsFrequency() const;
        const std::string&              getPhysicsInterface() const;
        const CUniverse&                getUniverse() const;
        const double&                   getVisualsFrequency() const;
                
        //--- Methods --------------------------------------------------------//
        bool import(const std::string&,
                    const ImportModeType& = IMPORT_MODE_UNIVERSE);
        
    private:
        
        const bool          checkAttributeBool(const pugi::xml_node&,
                                    const std::string&,
                                    const bool&,
                                    const bool = true
                                   ) const;
        const double        checkAttributeDouble(const pugi::xml_node&,
                                    const std::string&,
                                    const double&,
                                    const bool = true
                                   ) const;
        const int           checkAttributeInt(const pugi::xml_node&,
                                    const std::string&,
                                    const int&,
                                    const bool = true
                                   ) const;
        const std::string   checkAttributeString(const pugi::xml_node&,
                                    const std::string&,
                                    const std::string&,
                                    const bool = true
                                   ) const;

        bool checkFile(const pugi::xml_node&);
        void createCamera(const pugi::xml_node&);
        void createEmitter(const pugi::xml_node&);
        void createGravity(const pugi::xml_node&);
        void createShapeCircle(CBody* const, IObjectVisuals* const, const pugi::xml_node&);
        void createShapePlanet(CBody* const, IObjectVisuals* const, const pugi::xml_node&);
        void createShapeTerrain(CBody* const, IObjectVisuals* const, const pugi::xml_node&);
        void createShapePolyline(CBody* const, IObjectVisuals* const, const pugi::xml_node&);
        void createVisualsCircle(CDoubleBufferedShape* const, IObjectVisuals* const, const pugi::xml_node&);
        void createVisualsPlanet(CDoubleBufferedShape* const, IObjectVisuals* const, const pugi::xml_node&);
        void createVisualsTerrain(CDoubleBufferedShape* const, IObjectVisuals* const, const pugi::xml_node&);
        void createVisualsPolyline(CDoubleBufferedShape* const, IObjectVisuals* const, const pugi::xml_node&);
        void createRigidBody(const pugi::xml_node&);
        void createUniverse(const pugi::xml_node&);
        void readBodyCore(CRigidBody* const, const pugi::xml_node&);
        void readObjectCore(CRigidBody* const, const pugi::xml_node&);
        
        CUniverse                       m_Universe;         ///< The procedurally generated universe
        CCamera*                        m_pCamera;          ///< Main camera
        Vector2d                        m_vecGravity;       ///< Constant gravity vector
        std::string                     m_strCameraHook;    ///< Camera hook
        EmittersType                    m_Emitters;         ///< List of emitters
        std::string                     m_strPath;          ///< Path to read data from
        std::string                     m_strLuaPhysicsInterface; ///< Lua file for physics access
        double                          m_fDebrisFrequency; ///< Frequency for debris processing
        double                          m_fLuaFrequency;    ///< Frequency for lua interface
        double                          m_fPhysicsFrequency;///< Frequency for physics processing
        double                          m_fVisualsFrequency;///< Frequency for visual update
        
        std::map<IHooker*, std::string> m_Hooks;            ///< List of hookers and related object names
//         std::map<IHookable*, IHooker*>  m_Hooks;            ///< List of hookers and related hookables
                
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
/// \brief Return frequency for debris physics processing
///
/// \return Frequency for debris physics processing
///
////////////////////////////////////////////////////////////////////////////////
inline const double& CXMLImporter::getFrequencyDebris() const
{
    METHOD_ENTRY("CXMLImporter::getFrequencyDebris")
    return m_fDebrisFrequency;
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
