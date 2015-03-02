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
#include "log.h"

//--- Standard header --------------------------------------------------------//
#include <list>
#include <string>

/// Specifies the type of hooker
typedef enum 
{
    HOOKER_META,
    HOOKER_POSITIONAL
} HookerType;

class IHookable;

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Interface for meta objects to be hooked to their physical representation.
///
/// This interface allows for hooking of meta objects, i.e. objects that have
/// no physical representation but need to be bound to physical object. Reason
/// might be the deletion of physical objects. Hence, their hooks e.g. cameras,
/// emitters, lights, control units, players, components, visuals, etc. need to 
/// be destroyed, too. The examples show that their are different classes of 
/// hooks, like positional hooks, where an update has to be triggered by the
/// hookable. For instance, a camera hooked to a physical object has to be
/// continously updated in its position.
/// 
////////////////////////////////////////////////////////////////////////////////
class IHooker
{
    
    public:
    
        //--- Constructor/Destructor -----------------------------------------//
        IHooker();
        virtual ~IHooker();
        
        //--- Constant methods -----------------------------------------------//
        virtual const HookerType    getHookerType() const;
                const std::string   getName() const;
        
        //--- Methods --------------------------------------------------------//
        virtual bool hook(IHookable* const);
        virtual void updateFromHookable();
        
    protected:
        
        //--- Methods --------------------------------------------------------//

        //--- Variables ------------------------------------------------------//
        bool            m_bIsHooked;        ///< Defines, if hooker is hooked
        std::string     m_strName;          ///< Name of hooker
        
        IHookable*      m_pHookable;        ///< Hookable to refer to
        
};

typedef std::list<IHooker*> HookersType;    ///< Specifies a list of hookers

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor
///
////////////////////////////////////////////////////////////////////////////////
inline IHooker::IHooker() : m_bIsHooked(false),
                            m_pHookable(nullptr)
{
    METHOD_ENTRY("IHooker::IHooker")
    CTOR_CALL("IHooker::IHooker")
    
    // Default name for any hooker
    m_strName = "Meta_Hook";
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Get the hooker type
///
/// \return Type of hooker (meta). HOOKER_META is the default hook.
///
////////////////////////////////////////////////////////////////////////////////
inline const HookerType IHooker::getHookerType() const
{
    METHOD_ENTRY("IHooker::getHookerType")
    return HOOKER_META;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the name of the hooker
///
/// \return Name of the hooker
///
////////////////////////////////////////////////////////////////////////////////
inline const std::string IHooker::getName() const
{
    METHOD_ENTRY("IHooker::getName")
    return (m_strName);
}

#endif // HOOKER_H