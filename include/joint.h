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
/// \file       joint.h
/// \brief      Prototype of interface "IJoint"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2009-10-18
///
////////////////////////////////////////////////////////////////////////////////

#ifndef JOINT_H
#define JOINT_H

//--- Program header ---------------------------------------------------------//
#include "object.h"

//--- Standard header --------------------------------------------------------//

// Forward declarations
class IObject;

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Interface class for general object-object connections (joints)
///
////////////////////////////////////////////////////////////////////////////////
class IJoint
{
    
    public:
    
        //--- Constructor/Destructor -----------------------------------------//
        IJoint(){};
        virtual ~IJoint(){};

        //--- Constant methods -----------------------------------------------//
        virtual void react() const = 0;
        
        uint        getAnchorIDA() const;
        uint        getAnchorIDB() const;
        IObject*    getObjectA() const;
        IObject*    getObjectB() const;

        //--- Methods --------------------------------------------------------//
        void attachObjectA(IObject*, const uint&);
        void attachObjectB(IObject*, const uint&);

    protected:
        
        IObject* m_pObjectA;        ///< Pointer to attached object
        IObject* m_pObjectB;        ///< Pointer to attached object
        
        uint        m_unAnchorIDA;  ///< Anchor ID of first attached object
        uint        m_unAnchorIDB;  ///< Anchor ID of second attached object
};

//--- Implementation is done here for inline optimisation --------------------//

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns anchor id of the first object attached to joint
///
/// \return Anchor ID
///
///////////////////////////////////////////////////////////////////////////////
inline uint IJoint::getAnchorIDA() const
{
    METHOD_ENTRY("IJoint::getAnchorIDA()");

    METHOD_EXIT("IJoint::getAnchorIDA()");
    return (m_unAnchorIDA);
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns anchor id of the second object attached to joint
///
/// \return Anchor ID
///
///////////////////////////////////////////////////////////////////////////////
inline uint IJoint::getAnchorIDB() const
{
    METHOD_ENTRY("IJoint::getAnchorIDB()");

    METHOD_EXIT("IJoint::getAnchorIDB()");
    return (m_unAnchorIDB);
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the first object attached to joint
///
/// \return Pointer to object
///
///////////////////////////////////////////////////////////////////////////////
inline IObject* IJoint::getObjectA() const
{
    METHOD_ENTRY("IJoint::getObjectA()");

    METHOD_EXIT("IJoint::getObjectA()");
    return (m_pObjectA);
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the second object attached to joint
///
/// \return Pointer to object
///
///////////////////////////////////////////////////////////////////////////////
inline IObject* IJoint::getObjectB() const
{
    METHOD_ENTRY("IJoint::getObjectB()");

    METHOD_EXIT("IJoint::getObjectB()");
    return (m_pObjectB);
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Attach the first object to joint
///
/// \param _pA First object 
/// \param _unAnchorID Anchor ID
///
///////////////////////////////////////////////////////////////////////////////
inline void IJoint::attachObjectA(IObject* _pA, const uint& _unAnchorID)
{
    METHOD_ENTRY("IJoint::attachObjectA(IObject*, const Vector2d&)");

    m_pObjectA = _pA;
    m_unAnchorIDA = _unAnchorID;
    
    METHOD_EXIT("IJoint::attachObjectA(IObject*, const Vector2d&)");
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Attach the second object to joint
///
/// \param _pB Second object 
/// \param _unAnchorID Anchor ID
///
///////////////////////////////////////////////////////////////////////////////
inline void IJoint::attachObjectB(IObject* _pB, const uint& _unAnchorID)
{
    METHOD_ENTRY("IJoint::attachObjectA(IObject*, const Vector2d&)");

    m_pObjectB = _pB;
    m_unAnchorIDB = _unAnchorID;
    
    METHOD_EXIT("IJoint::attachObjectA(IObject*, const Vector2d&)");
}

#endif
