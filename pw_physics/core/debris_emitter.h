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

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class for a source that emits debris.
///
////////////////////////////////////////////////////////////////////////////////
class CDebrisEmitter : public IEmitter
{
    
    public:

        //--- Constructor/Destructor -----------------------------------------//
        CDebrisEmitter();
        ~CDebrisEmitter();
        
        //--- Constant Methods -----------------------------------------------//
        void emit(const double&);
        
        const EmitterType getEmitterType() const;
        
        //--- Methods --------------------------------------------------------//
        void        init();
        void        setNumber(const u_int32_t&);

    private:
        
        //--- Variables [private] --------------------------------------------//
        CDebris*    m_pDebris;      ///< The debris instance
};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Get the emitters type - RTTI
///
/// \return Type: debris emitter
///
////////////////////////////////////////////////////////////////////////////////
inline const EmitterType CDebrisEmitter::getEmitterType() const
{
    METHOD_ENTRY("CDebrisEmitter::getEmitterType")
    return EMITTER_DEBRIS;
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
    m_pDebris->setNumber(_nNrMax);
}

#endif
