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
#include "emitter.h"
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
        EmittersType                    getEmitters() const;
        const CUniverse&                getUniverse() const;
                
        //--- Methods --------------------------------------------------------//
        bool import(const std::string&,
                    const ImportModeType& = IMPORT_MODE_UNIVERSE);
        
    private:
        
        bool checkFile(const pugi::xml_node&);
        void createCamera(const pugi::xml_node&);
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
        void readObjectCore(IObject* const, const pugi::xml_node&);
        
        CUniverse                       m_Universe;         ///< The procedurally generated universe
        CCamera*                        m_pCamera;          ///< Main camera
        Vector2d                        m_vecGravity;       ///< Constant gravity vector
        std::string                     m_strCameraHook;    ///< Camera hook
        EmittersType                    m_Emitters;         ///< List of emitters
        std::map<std::string,IObject*>  m_Objects;          ///< List of objects
        std::string                     m_strPath;          ///< Path to read data from
                
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
inline EmittersType CXMLImporter::getEmitters() const
{
    METHOD_ENTRY("CXMLImporter::getEmitters")
    return m_Emitters;
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


#endif
