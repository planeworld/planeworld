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
#include "hooker.h"

//--- Standard header --------------------------------------------------------//
#include <list>

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
        
        
        //--- Methods --------------------------------------------------------//
        void addHooker(IHooker* const);
        void updateHookers();

    protected:

        //--- Constant methods -----------------------------------------------//
        virtual const double   getHookAngle() const = 0;
        virtual const Vector2i getHookCell() const = 0;
        virtual const Vector2d getHookOrigin() const = 0;

        //--- Variables ------------------------------------------------------//
        std::list<IHooker*> m_Hookers; ///< Entities hooked to this hookable
};

//--- Implementation is done here for inline optimisation --------------------//

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
        (*ci)->updateFromHooked(this->getHookCell(),
                                this->getHookOrigin(),
                                this->getHookAngle()
        );
    }
}

#endif