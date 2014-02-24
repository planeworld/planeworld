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
        
        //--- Methods --------------------------------------------------------//
        CDebris* getDebris(){return &m_Debris;}
        
        void setMaxNumber(const int&);

    private:
        
        //--- Variables [private] --------------------------------------------//
        CDebris m_Debris;       ///< The debris instance

        double  m_fResidual;    ///< Residual of debris emitation, since engine frequency differs from emitation frequency
        int     m_nNrMax;       ///< Maximum number of debris
        
};

//--- Implementation is done here for inline optimisation --------------------//

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets the maximum number of debris for this emitter
///
/// \param _nNrMax Maximum number of debris.
///
///////////////////////////////////////////////////////////////////////////////
inline void CDebrisEmitter::setMaxNumber(const int& _nNrMax)
{
    METHOD_ENTRY("IEmitter::setMode")
    m_nNrMax = _nNrMax;
    m_Debris.setNumber(m_nNrMax);
}


#endif
