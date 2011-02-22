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
        
        AnchorIDType    getAnchorIDA() const;
        AnchorIDType    getAnchorIDB() const;
        IObject*        getObjectA() const;
        IObject*        getObjectB() const;

        //--- Methods --------------------------------------------------------//
        void attachObjectA(IObject*, const AnchorIDType&);
        void attachObjectB(IObject*, const AnchorIDType&);
        void setVisualsID(const VisualsIDType&);

    protected:
        
        IObject* m_pObjectA;        ///< Pointer to attached object
        IObject* m_pObjectB;        ///< Pointer to attached object
        
        VisualsIDListType  m_VisualsIDs; ///< Visuals of joint
        
        AnchorIDType    m_AnchorIDA;  ///< Anchor ID of first attached object
        AnchorIDType    m_AnchorIDB;  ///< Anchor ID of second attached object
};

//--- Implementation is done here for inline optimisation --------------------//

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns anchor id of the first object attached to joint
///
/// \return Anchor ID
///
///////////////////////////////////////////////////////////////////////////////
inline AnchorIDType IJoint::getAnchorIDA() const
{
    METHOD_ENTRY("IJoint::getAnchorIDA()");

    METHOD_EXIT("IJoint::getAnchorIDA()");
    return (m_AnchorIDA);
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns anchor id of the second object attached to joint
///
/// \return Anchor ID
///
///////////////////////////////////////////////////////////////////////////////
inline AnchorIDType IJoint::getAnchorIDB() const
{
    METHOD_ENTRY("IJoint::getAnchorIDB()");

    METHOD_EXIT("IJoint::getAnchorIDB()");
    return (m_AnchorIDB);
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
/// \param _AnchorID Anchor ID
///
///////////////////////////////////////////////////////////////////////////////
inline void IJoint::attachObjectA(IObject* _pA, const AnchorIDType& _AnchorID)
{
    METHOD_ENTRY("IJoint::attachObjectA")

    m_pObjectA = _pA;
    m_AnchorIDA = _AnchorID;
    
    METHOD_EXIT("IJoint::attachObjectA")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Attach the second object to joint
///
/// \param _pB Second object 
/// \param _AnchorID Anchor ID
///
///////////////////////////////////////////////////////////////////////////////
inline void IJoint::attachObjectB(IObject* _pB, const AnchorIDType& _AnchorID)
{
    METHOD_ENTRY("IJoint::attachObjectA")

    m_pObjectB = _pB;
    m_AnchorIDB = _AnchorID;
    
    METHOD_EXIT("IJoint::attachObjectA")
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Adds an ID of joints visuals to list
///
/// The joint just stores ID's to visuals. The visuals itself are handled by 
/// the visualsmanager.
///
/// \param _VisualsID ID of visuals that should be added to list
///
////////////////////////////////////////////////////////////////////////////////
inline void IJoint::setVisualsID(const VisualsIDType& _VisualsID)
{
    METHOD_ENTRY("IJoint::setVisualsID")
    
    m_VisualsIDs.push_back(_VisualsID);

    METHOD_EXIT("IJoint::setVisualsID")
}

#endif
