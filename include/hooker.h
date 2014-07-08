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
/// \file       hooker.h
/// \brief      Prototype of interface "IHooker"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2014-06-27
///
////////////////////////////////////////////////////////////////////////////////

#ifndef HOOKER_H
#define HOOKER_H

//--- Program header ---------------------------------------------------------//

//--- Standard header --------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Interface for meta objects to be hooked to their physical representation.
///
/// This interface allows for hooking of meta objects, i.e. objects that have
/// no physical representation but need to be positioned. This might be
/// cameras, emitters, lights, control units, players, etc. This interface
/// represents the hooker, i.e. the class that hooks to an object.
/// Physical objects will be connected by joints.
/// 
////////////////////////////////////////////////////////////////////////////////
class IHooker
{
    
    public:
    
        //--- Constructor/Destructor -----------------------------------------//
        IHooker();
        
        //--- Constant methods -----------------------------------------------//
        
        //--- Methods --------------------------------------------------------//
        virtual void updateFromHooked(const Vector2i&,
                                      const Vector2d&,
                                      const double&
                                     );
                                       
        void enableAngleHook();
        void disableAngleHook();

    protected:
        
        //--- Methods ------------.-------------------------------------------//
        virtual void myUpdateFromHooked();

        //--- Variables ----------.-------------------------------------------//
        Vector2d        m_vecHookOrigin;    ///< Origin of hooked entity
        Vector2i        m_vecHookCell;      ///< Cell of hooked entity
        double          m_fHookAngle;       ///< Angle of hooked entity
        bool            m_bAngleIsHooked;   ///< Defines, if angle is hooked
        bool            m_bIsHooked;        ///< Defines, if hooker is hooked
        
};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor
///
////////////////////////////////////////////////////////////////////////////////
inline IHooker::IHooker() : m_fHookAngle(0.0),
                            m_bAngleIsHooked(true),
                            m_bIsHooked(false)
{
    METHOD_ENTRY("CCamera::IHooker")
    CTOR_CALL("IHooker::IHooker")
    
    m_vecHookCell.setZero();
    m_vecHookOrigin.setZero();
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Updates hooker triggered by hooked entity
///
/// \param _vecCell Cell of hooked entitiy
/// \param _vecOrigin Origin of hooked entity
/// \param _fAngle Angle of hooked Entity
///
////////////////////////////////////////////////////////////////////////////////
inline void IHooker::updateFromHooked(const Vector2i& _vecCell,
                                      const Vector2d& _vecOrigin,
                                      const double& _fAngle)
{
    METHOD_ENTRY("IHooker::updateFromHooked")
    m_vecHookCell = _vecCell;
    m_vecHookOrigin = _vecOrigin;
    m_fHookAngle = _fAngle;
    
    // If this method is called, a hook is present, hence:
    m_bIsHooked = true;
    
    this->myUpdateFromHooked();
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Activates angle hook
///
////////////////////////////////////////////////////////////////////////////////
inline void IHooker::enableAngleHook()
{
    METHOD_ENTRY("CCamera::enableAngleHook")
    m_bAngleIsHooked = true;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Deactivates angle hook
///
////////////////////////////////////////////////////////////////////////////////
inline void IHooker::disableAngleHook()
{
    METHOD_ENTRY("CCamera::enableAngleHook")
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
inline void IHooker::myUpdateFromHooked()
{
    METHOD_ENTRY("IHooker::myUpdateFromHooked")
}

#endif