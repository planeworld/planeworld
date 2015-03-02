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
/// \file       hookable.h
/// \brief      Prototype of interface "IHookable"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2012-12-02
///
////////////////////////////////////////////////////////////////////////////////

#ifndef HOOKABLE_H
#define HOOKABLE_H

//--- Program header ---------------------------------------------------------//
#include "hooker_positional.h"

//--- Standard header --------------------------------------------------------//

/// specifies the type of shape
typedef enum 
{
    HOOKABLE_NONE,
    HOOKABLE_OBJECT,
} HookableType;

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Interface for hookable objects.
///
/// This interface allows for hooking of meta objects, i.e. objects that have
/// no physical representation but need to be positioned. This might be
/// cameras, emitters, lights, control units, players, etc. This class represents
/// the physical representation that meta objects can be hooked to.
/// Physical objects will be connected by joints.
/// 
////////////////////////////////////////////////////////////////////////////////
class IHookable
{
    
    public:
    
        //--- Constructor/Destructor -----------------------------------------//

        //--- Constant methods -----------------------------------------------//
        virtual const HookableType getHookableType() const;
                const HookersType& getHookers() const;
        
        //--- Methods --------------------------------------------------------//
        void addHooker(IHooker* const);
        void updateHookers();

    protected:

        //--- Constant methods -----------------------------------------------//
        virtual const double   getHookAngle() const = 0;
        virtual const Vector2i getHookCell() const = 0;
        virtual const Vector2d getHookOrigin() const = 0;

        //--- Variables ------------------------------------------------------//
        HookersType m_Hookers;              ///< Entities hooked to this hookable
};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Get the type of hookable
///
/// \return Type of hookable
///
////////////////////////////////////////////////////////////////////////////////
inline const HookableType IHookable::getHookableType() const
{
    METHOD_ENTRY("IHookable::getHookableType")
    return HOOKABLE_NONE;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the hookers hooked to this hookable
///
/// \return List of hookers
///
////////////////////////////////////////////////////////////////////////////////
inline const HookersType& IHookable::getHookers() const
{
    METHOD_ENTRY("IHookable::getHookers")
    return m_Hookers;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Hooks entity to this hookable
///
/// \param _pHooker Entity to be hooked
///
////////////////////////////////////////////////////////////////////////////////
inline void IHookable::addHooker(IHooker* const _pHooker)
{
    METHOD_ENTRY("IHookable::addHooker")
    m_Hookers.push_back(_pHooker); 
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Updates trajectory with a new value
///
/// \param _pHooker Entity to be hooked
///
////////////////////////////////////////////////////////////////////////////////
inline void IHookable::updateHookers()
{
    METHOD_ENTRY("IHookable::updateHookers")
    for (auto ci=m_Hookers.cbegin(); ci != m_Hookers.cend(); ++ci)
    {
        switch((*ci)->getHookerType())
        {
            case HOOKER_META:
                // Meta information. No update information from hookable needed.
                break;
            case HOOKER_POSITIONAL:
                static_cast<IHookerPositional*>((*ci))->updateFromHookable(this->getHookCell(),
                                          this->getHookOrigin(),
                                          this->getHookAngle()
                );
        }
    }
}

#endif // HOOKABLE_H