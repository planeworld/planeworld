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
/// \file       hooker_positional.h
/// \brief      Prototype of interface "IHookerPositional"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2014-07-13
///
////////////////////////////////////////////////////////////////////////////////

#ifndef HOOKER_POSITIONAL_H
#define HOOKER_POSITIONAL_H

//--- Program header ---------------------------------------------------------//
#include "hooker.h"

//--- Standard header --------------------------------------------------------//

//--- Misc. header -----------------------------------------------------------//
#include <eigen3/Eigen/Geometry>

using namespace Eigen;

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Interface for meta objects to be hooked to a physics object that need
///        positional information
///
/// This interface allows for hooking of meta objects, i.e. objects that have
/// no physical representation but need to be positioned. This might be
/// cameras, emitters, lights, control units, players, etc. This interface
/// represents the hooker, i.e. the class that hooks to an object.
/// Physical objects will be connected by joints.
/// 
////////////////////////////////////////////////////////////////////////////////
class IHookerPositional : public IHooker
{
    
    public:
    
        //--- Constructor/Destructor -----------------------------------------//
        IHookerPositional();
        ~IHookerPositional(){};
        
        //--- Constant methods -----------------------------------------------//
        const HookerType getHookerType() const;
        
        //--- Methods --------------------------------------------------------//
        virtual void updateFromHookable(const Vector2i&,
                                        const Vector2d&,
                                        const double&
                                       );
                                       
        void enableAngleHook();
        void disableAngleHook();

    protected:
        
        //--- Methods ------------.-------------------------------------------//
        virtual void myUpdateFromHookable();

        //--- Variables ----------.-------------------------------------------//
        Vector2d        m_vecHookOrigin;    ///< Origin of hooked entity
        Vector2i        m_vecHookCell;      ///< Cell of hooked entity
        double          m_fHookAngle;       ///< Angle of hooked entity
        bool            m_bAngleIsHooked;   ///< Defines, if angle is hooked
        
};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor
///
////////////////////////////////////////////////////////////////////////////////
inline IHookerPositional::IHookerPositional() : m_fHookAngle(0.0),
                                                m_bAngleIsHooked(true)
{
    METHOD_ENTRY("IHookerPositional::IHookerPositional")
    CTOR_CALL("IHookerPositional::IHookerPositional")
    
    m_vecHookCell.setZero();
    m_vecHookOrigin.setZero();
    m_strName = "Positional_Hook";
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Get the hooker type
///
/// \return Type of hooker (positional)
///
////////////////////////////////////////////////////////////////////////////////
inline const HookerType IHookerPositional::getHookerType() const
{
    METHOD_ENTRY("IHookerPositional::getHookerType")
    return HOOKER_POSITIONAL;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Updates hooker triggered by hooked entity
///
/// \param _vecCell Cell of hooked entitiy
/// \param _vecOrigin Origin of hooked entity
/// \param _fAngle Angle of hooked entity
///
////////////////////////////////////////////////////////////////////////////////
inline void IHookerPositional::updateFromHookable(const Vector2i& _vecCell,
                                                  const Vector2d& _vecOrigin,
                                                  const double& _fAngle)
{
    METHOD_ENTRY("IHookerPositional::updateFromHookable")
    m_vecHookCell = _vecCell;
    m_vecHookOrigin = _vecOrigin;
    m_fHookAngle = _fAngle;
    
    // If this method is called, a hook is present, hence:
    m_bIsHooked = true;
    
    this->myUpdateFromHookable();
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Activates angle hook
///
////////////////////////////////////////////////////////////////////////////////
inline void IHookerPositional::enableAngleHook()
{
    METHOD_ENTRY("IHookerPositional::enableAngleHook")
    m_bAngleIsHooked = true;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Deactivates angle hook
///
////////////////////////////////////////////////////////////////////////////////
inline void IHookerPositional::disableAngleHook()
{
    METHOD_ENTRY("IHookerPositional::enableAngleHook")
    m_bAngleIsHooked = false;
    m_fHookAngle = 0.0;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Updates hooker triggered by hooked entity. 
///
/// Allows for additional processing within derived class. This might be useful
/// if update has to be synced to physics frequency like the camera and its
/// Bounding Box.
///
////////////////////////////////////////////////////////////////////////////////
inline void IHookerPositional::myUpdateFromHookable()
{
    METHOD_ENTRY("IHookerPositional::myUpdateFromHookable")
}

#endif