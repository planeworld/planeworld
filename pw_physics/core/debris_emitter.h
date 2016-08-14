////////////////////////////////////////////////////////////////////////////////
//
// This file is part of planeworld, a 2D simulation of physics and much more.
// Copyright (C) 2014-2016 Torsten Büschenfeld
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
/// \file       debris_emitter.h
/// \brief      Prototype of class "CDebrisEmitter"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2014-02-16
///
////////////////////////////////////////////////////////////////////////////////

#ifndef DEBRIS_EMITTER_H
#define DEBRIS_EMITTER_H

//--- Standard header --------------------------------------------------------//

//--- Program header ---------------------------------------------------------//
#include "debris.h"
#include "emitter.h"

/// Specifies the type of debris
typedef enum
{
    DEBRIS_TYPE_DOT,
    DEBRIS_TYPE_THRUST
} DebrisTypeType;

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class for a source that emits debris.
///
////////////////////////////////////////////////////////////////////////////////
class CDebrisEmitter : public IEmitter,
                       public IUniqueIDReferrer<CDebris>
{
    
    public:

        //--- Constructor/Destructor -----------------------------------------//
        CDebrisEmitter();
        ~CDebrisEmitter();
        
        //--- Constant Methods -----------------------------------------------//
        void emit(const double&);
        
        const DebrisTypeType& getDebrisType()  const;
              EmitterType getEmitterType() const;
        
        //--- Methods --------------------------------------------------------//
        void        init();
        void        setDebrisType(const DebrisTypeType&);
        void        setNumber(const u_int32_t&);

    private:
        
        //--- Variables [private] --------------------------------------------//
        DebrisTypeType      m_DebrisType;       ///< Type of debris
        static uint32_t     m_unNrOfEmitters;   ///< Static counter for name initialisation and tracking
};

//--- Enum parser ------------------------------------------------------------//
const std::map<DebrisTypeType, std::string> mapDebrisTypeToString = {
    {DEBRIS_TYPE_DOT, "debris_dot"},
    {DEBRIS_TYPE_THRUST, "debris_thrust"}
}; ///< Map from DebrisTypeType to string

const std::map<std::string, DebrisTypeType> mapStringToDebrisType = {
    {"debris_dot", DEBRIS_TYPE_DOT},
    {"debris_thrust", DEBRIS_TYPE_THRUST}
}; ///< Map from string to DebrisTypeType

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Get the type of debris
///
/// \return Type of debris
///
////////////////////////////////////////////////////////////////////////////////
inline const DebrisTypeType& CDebrisEmitter::getDebrisType() const
{
    METHOD_ENTRY("CDebrisEmitter::getDebrisTypeType")
    return m_DebrisType;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Get the emitters type - RTTI
///
/// \return Type: debris emitter
///
////////////////////////////////////////////////////////////////////////////////
inline EmitterType CDebrisEmitter::getEmitterType() const
{
    METHOD_ENTRY("CDebrisEmitter::getEmitterType")
    return EMITTER_DEBRIS;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets the type of debris
///
/// \param _DebrisType Type of debris
///
///////////////////////////////////////////////////////////////////////////////
inline void CDebrisEmitter::setDebrisType(const DebrisTypeType& _DebrisType)
{
    METHOD_ENTRY("CDebrisEmitter::setDebrisTypeType")
    m_DebrisType = _DebrisType;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets the maximum number of debris for this emitter
///
/// \param _nNrMax Maximum number of debris.
///
///////////////////////////////////////////////////////////////////////////////
inline void CDebrisEmitter::setNumber(const u_int32_t& _nNrMax)
{
    METHOD_ENTRY("CDebrisEmitter::setMode")
    m_nNr = _nNrMax;
    m_pRef->setNumber(_nNrMax);
}

#endif
