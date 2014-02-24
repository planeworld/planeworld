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
/// \file       emitter.h
/// \brief      Prototype of class "IEmitter"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2014-02-16
///
////////////////////////////////////////////////////////////////////////////////

#ifndef EMITTER_H
#define EMITTER_H

//--- Standard header --------------------------------------------------------//
#include <list>

//--- Program header ---------------------------------------------------------//
#include "log.h"
#include "universe_scaled.h"

// Constants
const double EMITTER_DEFAULT_FREQUENCY = 1.0; ///< Default frequency of emitter

/// Specifies the mode for emitting world entities
typedef enum
{
    EMITTER_EMIT_ONCE,
    EMITTER_MODE_SOURCE
} EmitterModeType;

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Interface for a source that emits world objects like bodies or
///        debris.
///
////////////////////////////////////////////////////////////////////////////////
class IEmitter : public IUniverseScaled
{
    
    public:

        //--- Constructor/Destructor -----------------------------------------//
        IEmitter();
        virtual ~IEmitter();
        
        //--- Constant Methods -----------------------------------------------//
        const EmitterModeType getMode() const;
        
        //--- Methods --------------------------------------------------------//
        virtual void emit(const double& = -1.0) = 0;
        
        void setFrequency(const double&);
        void setMode(const EmitterModeType&);
        

    protected:
        
        EmitterModeType m_EmitterMode;
        
        double          m_fFrequency;
        
};

/// Specifies type a list of emitters
typedef std::list<IEmitter*>   EmittersType;

//--- Implementation is done here for inline optimisation --------------------//

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor
///
///////////////////////////////////////////////////////////////////////////////
inline IEmitter::IEmitter() : m_EmitterMode(EMITTER_EMIT_ONCE),
                              m_fFrequency(EMITTER_DEFAULT_FREQUENCY)
{
    METHOD_ENTRY("IEmitter::IEmitter")
    CTOR_CALL("IEmitter::IEmitter")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Destructor
///
///////////////////////////////////////////////////////////////////////////////
inline IEmitter::~IEmitter()
{
    METHOD_ENTRY("IEmitter::~IEmitter")
    DTOR_CALL("IEmitter::~IEmitter")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Return the emitter mode
///
/// \return Emitter mode
///
///////////////////////////////////////////////////////////////////////////////
inline const EmitterModeType IEmitter::getMode() const
{
    METHOD_ENTRY("IEmitter::getMode")
    return m_EmitterMode;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Set frequency of emitation
///
/// \param _fF Frequency of emitation
///
///////////////////////////////////////////////////////////////////////////////
inline void IEmitter::setFrequency(const double& _fF)
{
    METHOD_ENTRY("IEmitter::setFrequency")
    m_fFrequency = _fF;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets an emitter mode
///
/// \param _EmM Emitter mode to be set
///
///////////////////////////////////////////////////////////////////////////////
inline void IEmitter::setMode(const EmitterModeType& _EmM)
{
    METHOD_ENTRY("IEmitter::setMode")
    m_EmitterMode = _EmM;
}

#endif
