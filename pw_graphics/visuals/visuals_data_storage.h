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

/// Map of cameras, accessed by name
typedef std::unordered_map<std::string, CCamera*> CamerasByNameType;
/// Vector of cameras, accessed by index to cycle through cameras
typedef std::vector<CCamera*> CamerasByIndexType;

typedef std::vector<CDebrisVisuals*>            DebrisVisualsType;          ///< Specifies a list of debris visuals
typedef std::list<CDebrisVisualsThruster*>      DebrisVisualsThrusterType;  ///< Specifies a list of debris visuals

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
        const CamerasByIndexType&           getCamerasByIndex() const;
        const CamerasByNameType&            getCamerasByName() const;
        const DebrisVisualsType&            getDebrisVisuals() const;
        const DebrisVisualsThrusterType&    getDebrisVisualsThruster() const;
        
        //--- Methods --------------------------------------------------------//
        void addCamera(CCamera*);
        void addDebrisVisuals(CDebrisVisuals*);
        void addDebrisVisualsThruster(CDebrisVisualsThruster*);
        
        //--- friends --------------------------------------------------------//
        friend std::istream& operator>>(std::istream&, CVisualsDataStorage&);
        friend std::ostream& operator<<(std::ostream&, CVisualsDataStorage&);
        
    private:
      
        CamerasByIndexType          m_CamerasByIndex;           ///< Cameras, accessed by index
        CamerasByNameType           m_CamerasByName;            ///< Cameras, accessed by name
        DebrisVisualsType           m_DebrisVisuals;            ///< List of debris visuals
        DebrisVisualsThrusterType   m_DebrisVisualsThruster;    ///< List of debris visuals
};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns cameras.
///
/// \return Cameras
///
////////////////////////////////////////////////////////////////////////////////
inline const CamerasByIndexType& CVisualsDataStorage::getCamerasByIndex() const
{
    METHOD_ENTRY("CVisualsDataStorage::getCamerasByIndex")
    return m_CamerasByIndex;
}

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

#endif // VISUALS_DATA_STORAGE_H
