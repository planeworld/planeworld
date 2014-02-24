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
/// \file       debris_emitter.cpp
/// \brief      Implementation of class "CDebrisEmitter"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2014-02-18
///
////////////////////////////////////////////////////////////////////////////////

#include "debris_emitter.h"

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor
///
///////////////////////////////////////////////////////////////////////////////
CDebrisEmitter::CDebrisEmitter() : m_fResidual(0.0), m_nNrMax(1)
{
    METHOD_ENTRY("CDebrisEmitter::CDebrisEmitter")
    CTOR_CALL("CDebrisEmitter::CDebrisEmitter")
    
    m_Debris.setNumber(1);
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Destructor, deletes objects
///
///////////////////////////////////////////////////////////////////////////////
CDebrisEmitter::~CDebrisEmitter()
{
    METHOD_ENTRY("CDebrisEmitter::~CDebrisEmitter")
    DTOR_CALL("CDebrisEmitter::~CDebrisEmitter")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Starts emitting of world objects
///
/// This calls one emitation. If a timed emitation is aimed at, the method is
/// called with a positive frequency defining the engines frequency of
/// emitation. Since the actual frequency set for this emitter might be higher
/// than the frequency the method is called at, the number of debris per emit
/// is chosen accordingly.
///
/// \param _fF Engine emitation frequency.
///
///////////////////////////////////////////////////////////////////////////////
void CDebrisEmitter::emit(const double& _fF)
{
    METHOD_ENTRY("CDebrisEmitter::emit")
    
    if (_fF < 0.0)
    {
        std::cout << "Emitting all debris once." << std::endl;
    }
    else
    {
        double fNrOfDebris = m_fFrequency * _fF + m_fResidual;
        int    nNrOfDebris = static_cast<int>(fNrOfDebris);
        m_fResidual = fNrOfDebris - nNrOfDebris;
        
        for (int i=0; i<nNrOfDebris; ++i)
        {
            m_Debris.generate(Vector2d(0.0,0.0+i), Vector2d(0.0, 1.0));
        }
        std::cout << "Emitting " << nNrOfDebris << " debris" << std::endl;
    }
}
