////////////////////////////////////////////////////////////////////////////////
//
// This file is part of planeworld, a 2D simulation of physics and much more.
// Copyright (C) 2016-2016 Torsten Büschenfeld
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
/// \file       visuals_data_storage.h
/// \brief      Prototype of class "CVisualsDataStorage"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2016-06-28
///
////////////////////////////////////////////////////////////////////////////////

#ifndef VISUALS_DATA_STORAGE_H
#define VISUALS_DATA_STORAGE_H

//--- Standard header --------------------------------------------------------//
#include <unordered_map>

//--- Program header ---------------------------------------------------------//
#include "camera.h"
#include "debris_visuals.h"
#include "debris_visuals_thruster.h"
#include "object_visuals.h"

/// Map of cameras, accessed by name
typedef std::unordered_map<std::string, CCamera*> CamerasByNameType;

typedef std::vector<CDebrisVisuals*>            DebrisVisualsType;          ///< Specifies a list of debris visuals
typedef std::list<CDebrisVisualsThruster*>      DebrisVisualsThrusterType;  ///< Specifies a list of debris visuals
typedef std::vector<IObjectVisuals*>            ObjectVisualsType;          ///< Specifies a list of object visuals

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class that stores all visuals data
///
////////////////////////////////////////////////////////////////////////////////
class CVisualsDataStorage
{
    
    public:
        
        //--- Constructor/Destructor -----------------------------------------//
        CVisualsDataStorage();
        ~CVisualsDataStorage();
        
        //--- Constant Methods -----------------------------------------------//
//         CCamera*                            getCamera() const;
        const CamerasByNameType&            getCamerasByName() const;
        const DebrisVisualsType&            getDebrisVisuals() const;
        const DebrisVisualsThrusterType&    getDebrisVisualsThruster() const;
        const ObjectVisualsType&            getObjectVisuals() const;
        
        //--- Methods --------------------------------------------------------//
        void addCamera(CCamera*);
        void addDebrisVisuals(CDebrisVisuals*);
        void addDebrisVisualsThruster(CDebrisVisualsThruster*);
        
        void            addObjectVisuals(IObjectVisuals*);
        
        //--- friends --------------------------------------------------------//
        friend std::istream& operator>>(std::istream&, CVisualsDataStorage&);
        friend std::ostream& operator<<(std::ostream&, CVisualsDataStorage&);
        
    private:
      
//         CCamera*                    m_pCamera;                  ///< Active camera for visuals
        CamerasByNameType           m_CamerasByName;            ///< Cameras, accessed by name
        DebrisVisualsType           m_DebrisVisuals;            ///< List of debris visuals
        DebrisVisualsThrusterType   m_DebrisVisualsThruster;    ///< List of debris visuals
        ObjectVisualsType           m_ObjectVisuals;            ///< List of object visuals
};

//--- Implementation is done here for inline optimisation --------------------//

// ////////////////////////////////////////////////////////////////////////////////
// ///
// /// \brief Returns active camera
// ///
// /// \return Active camera
// ///
// ////////////////////////////////////////////////////////////////////////////////
// inline CCamera* CVisualsDataStorage::getCamera() const
// {
//     METHOD_ENTRY("CVisualsDataStorage::getCamera")
//     return m_pCamera;
// }

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns cameras.
///
/// \return Cameras
///
////////////////////////////////////////////////////////////////////////////////
inline const CamerasByNameType& CVisualsDataStorage::getCamerasByName() const
{
    METHOD_ENTRY("CVisualsDataStorage::getCamerasByName")
    return m_CamerasByName;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns a list of debris visuals.
///
/// \return List of debris visuals
///
////////////////////////////////////////////////////////////////////////////////
inline const DebrisVisualsType& CVisualsDataStorage::getDebrisVisuals() const
{
    METHOD_ENTRY("CVisualsDataStorage::getDebrisVisuals")
    return m_DebrisVisuals;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns a list of thruster debris visuals.
///
/// \return List of thruster debris visuals
///
////////////////////////////////////////////////////////////////////////////////
inline const DebrisVisualsThrusterType& CVisualsDataStorage::getDebrisVisualsThruster() const
{
    METHOD_ENTRY("CVisualsDataStorage::getDebrisVisualsThruster")
    return m_DebrisVisualsThruster;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Add a object visuals to internal list
///
/// \param _pObjectVisuals Objects visuals that should be added to list
///
///////////////////////////////////////////////////////////////////////////////
inline void CVisualsDataStorage::addObjectVisuals(IObjectVisuals* _pObjectVisuals)
{
    METHOD_ENTRY("CVisualsDataStorage::addObjectVisuals")
    m_ObjectVisuals.push_back(_pObjectVisuals);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns a list of object visuals.
///
/// \return List of object visuals
///
////////////////////////////////////////////////////////////////////////////////
inline const ObjectVisualsType& CVisualsDataStorage::getObjectVisuals() const
{
    METHOD_ENTRY("CVisualsDataStorage::getObjectVisuals")
    return m_ObjectVisuals;
}

// ////////////////////////////////////////////////////////////////////////////////
// ///
// /// \brief Sets the active camera
// ///
// /// \param _pCam Active camera
// ///
// ////////////////////////////////////////////////////////////////////////////////
// inline void CVisualsDataStorage::setCamera(CCamera* const _pCam)
// {
//     METHOD_ENTRY("CVisualsDataStorage::setCamera")
//     m_pCamera = _pCam;
// }

#endif // VISUALS_DATA_STORAGE_H
