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
/// \file       thruster.h
/// \brief      Prototype of class "CThruster"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2014-07-09
///
////////////////////////////////////////////////////////////////////////////////

#ifndef THRUSTER_H
#define THRUSTER_H

//--- Program header ---------------------------------------------------------//
#include "debris_emitter.h"
#include "hooker.h"

//--- Standard header --------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class representing a thruster component
/// 
////////////////////////////////////////////////////////////////////////////////
class CThruster : public IHooker
{
    
    public:
    
        //--- Constructor/Destructor -----------------------------------------//
        CThruster();
        
        //--- Constant methods -----------------------------------------------//
        CDebrisEmitter* const getEmitter() const;
        
        //--- Methods --------------------------------------------------------//
        bool init(IHookable* const _pBody);
        void fire();
         
    protected:
        
        //--- Methods --------------------------------------------------------//

        //--- Variables ------------------------------------------------------//
        CDebrisEmitter* m_pEmitter;  ///< Emitter for thrust particle output
       
};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the hookers hooked to this hookable
///
/// \return List of hookers
///
////////////////////////////////////////////////////////////////////////////////
inline CThruster::CThruster() : m_pEmitter(nullptr)
{
    METHOD_ENTRY("CThruster::CThruster")
    CTOR_CALL("CThruster::CThruster")
    
    IHooker::m_strName += ": Thruster";
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the internal debris emitter
///
/// \return Debris emitter
///
////////////////////////////////////////////////////////////////////////////////
inline CDebrisEmitter* const CThruster::getEmitter() const
{
    METHOD_ENTRY("CThruster::getEmitter")
    return m_pEmitter;    
}

#endif // THRUSTER_H