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
#include "emitter.h"
#include "hooker_positional.h"

//--- Standard header --------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class representing a thruster component
/// 
////////////////////////////////////////////////////////////////////////////////
class CThruster : public IHookerPositional
{
    
    public:
    
        //--- Constructor/Destructor -----------------------------------------//
        CThruster();
        
        //--- Constant methods -----------------------------------------------//
        const double&   getAngle() const;
        const Vector2d& getOrigin() const;
        
        IEmitter* const getEmitter() const;
        
        //--- Methods --------------------------------------------------------//
        void activate(const double&);
        void deactivate();
        
        void execute();
        
        void setAngle(const double&);
        void setOrigin(const Vector2d&);
        
        void setEmitter(IEmitter* const);
        void setThrustMax(const double&);
        void setEmitterVelocity(const double&);
        void setEmitterVelocityStd(const double&);
         
    protected:
        
        //--- Methods --------------------------------------------------------//

        //--- Variables ------------------------------------------------------//
        bool        m_bActive;      ///< Flags if thruster is activated
      
        double      m_fAngle;       ///< Angle of thrust vector
        Vector2d    m_vecOrigin;    ///< Origin of thruster in local coordinates of hooked entity
        
        double      m_fThrust;      ///< Thrust applied to hooked object
        double      m_fThrustMax;   ///< Maximum thrust of this thruster
        
        /// \todo Thruster might have more than one emitter. (De-)activation needs to be adressed then, too.
        IEmitter*   m_pEmitter;     ///< Emitter for thrust particle output
        double      m_fEmitterVelocity;     ///< Default emitter velocity
        double      m_fEmitterVelocityStd;  ///< Default emitter velocity standard deviation
       
};

/// Specifies a list of thruster components
typedef std::unordered_multimap<std::string, CThruster*> ComponentsType;

//--- Implementation is done here for inline optimisation --------------------//

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Deactivate thruster
///
/// This method stops the thruster and thus, no force is applied within the
/// execute() method.
///
///////////////////////////////////////////////////////////////////////////////
inline void CThruster::deactivate()
{
    METHOD_ENTRY("CThruster::deactivate")
    m_fThrust = 0.0;
    
    m_bActive = false;
    m_pEmitter->deactivate();
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets the angle of thrust vector.
///
/// \param _fAngle Angle of thrust vector
///
////////////////////////////////////////////////////////////////////////////////
inline void CThruster::setAngle(const double& _fAngle)
{
    METHOD_ENTRY("CThruster::setAngle")
    m_fAngle = _fAngle;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets the origin of thruster
///
/// \param _vecOrigin Origin of thruster
///
////////////////////////////////////////////////////////////////////////////////
inline void CThruster::setOrigin(const Vector2d& _vecOrigin)
{
    METHOD_ENTRY("CThruster::setOrigin")
    m_vecOrigin = _vecOrigin;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets the emitter for this thruster.
///
/// \param _pEmitter Emitter to use for this thruster
///
////////////////////////////////////////////////////////////////////////////////
inline void CThruster::setEmitter(IEmitter* const _pEmitter)
{
    METHOD_ENTRY("CThruster::setEmitter")
    m_pEmitter = _pEmitter;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets the maximum thrust for this thruster.
///
/// \param _fThrustMax Maximum thrust for this thruster
///
////////////////////////////////////////////////////////////////////////////////
inline void CThruster::setThrustMax(const double& _fThrustMax)
{
    METHOD_ENTRY("CThruster::setThrustMax")
    m_fThrust = _fThrustMax;
    m_fThrustMax = _fThrustMax;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Stores the default velocity of emitter
///
/// The default emitter velocity is stored to enable changing it according to
/// thrust.
///
/// \param _fEmitterVelocity Default velocity of emitter
///
////////////////////////////////////////////////////////////////////////////////
inline void CThruster::setEmitterVelocity(const double& _fEmitterVelocity)
{
    METHOD_ENTRY("CThruster::setEmitterVelocity")
    m_fEmitterVelocity = _fEmitterVelocity;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Stores the default velocity standard deviation of emitter
///
/// The default emitter velocity standard deviation is stored to enable changing
/// it according tothrust.
///
/// \param _fEmitterVelocityStd Default velocity standard deviation of emitter
///
////////////////////////////////////////////////////////////////////////////////
inline void CThruster::setEmitterVelocityStd(const double& _fEmitterVelocityStd)
{
    METHOD_ENTRY("CThruster::setEmitterVelocityStd")
    m_fEmitterVelocityStd = _fEmitterVelocityStd;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns angle of thrust vector
///
/// \return Angle of thrust vector
///
////////////////////////////////////////////////////////////////////////////////
inline const double& CThruster::getAngle() const
{
    METHOD_ENTRY("CThruster::getAngle")
    return m_fAngle;    
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns origin of thruster
///
/// \return Origin of thruster
///
////////////////////////////////////////////////////////////////////////////////
inline const Vector2d& CThruster::getOrigin() const
{
    METHOD_ENTRY("CThruster::getOrigin")
    return m_vecOrigin;    
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the internal debris emitter
///
/// \return Debris emitter
///
////////////////////////////////////////////////////////////////////////////////
inline IEmitter* const CThruster::getEmitter() const
{
    METHOD_ENTRY("CThruster::getEmitter")
    return m_pEmitter;    
}

#endif // THRUSTER_H