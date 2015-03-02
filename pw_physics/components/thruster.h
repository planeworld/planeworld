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
#include "rigidbody.h"

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
        bool init(IHookable* const _pBody)
        {
            if (_pBody->getHookableType() != HOOKABLE_OBJECT)
            {
                ERROR_MSG("Thruster", "Unable to init to this entity. Hookable should be an object.")
                return false;
            }
            else
            {
              m_pEmitter = new CDebrisEmitter;
              m_pEmitter->setOrigin(Vector2d(0.0, 15.0));
              m_pEmitter->setNumber(1000);
              m_pEmitter->setDistribution(EMITTER_DISTRIBUTION_POINT_SOURCE);
              m_pEmitter->setMode(EMITTER_MODE_TIMED);
              m_pEmitter->setFrequency(10);
              m_pEmitter->setVelocity(10.0);
              m_pEmitter->setAngleStd(0.2);
              m_pHookable->addHooker(m_pEmitter);
            }
         }
         
         void fire()
         {
           static_cast<IObject*>(m_pHookable)->addForce(Vector2d(0.0, 200.0),
                                                        Vector2d(0.0, 0.0));
         }
         
    protected:
        
        //--- Methods --------------------------------------------------------//

        //--- Variables ------------------------------------------------------//
        CDebrisEmitter* m_pEmitter;  ///< Emitter for thrust particle output
//         IHookable*      m_pBody;     ///< Physical thruster representation
       
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