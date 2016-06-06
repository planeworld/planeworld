////////////////////////////////////////////////////////////////////////////////
//
// This file is part of planeworld, a 2D simulation of physics and much more.
// Copyright (C) 2009-2016 Torsten Büschenfeld
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
////////////////////////////////////////////////////////////////////////////////

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
class CObject;

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
        CObject*        getObjectA() const;
        CObject*        getObjectB() const;

        //--- Methods --------------------------------------------------------//
        void attachObjectA(CObject*, const AnchorIDType&);
        void attachObjectB(CObject*, const AnchorIDType&);

    protected:
        
        CObject* m_pObjectA;        ///< Pointer to attached object
        CObject* m_pObjectB;        ///< Pointer to attached object
        
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
    return (m_AnchorIDB);
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the first object attached to joint
///
/// \return Pointer to object
///
///////////////////////////////////////////////////////////////////////////////
inline CObject* IJoint::getObjectA() const
{
    METHOD_ENTRY("IJoint::getObjectA()");
    return (m_pObjectA);
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the second object attached to joint
///
/// \return Pointer to object
///
///////////////////////////////////////////////////////////////////////////////
inline CObject* IJoint::getObjectB() const
{
    METHOD_ENTRY("IJoint::getObjectB()");
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
inline void IJoint::attachObjectA(CObject* _pA, const AnchorIDType& _AnchorID)
{
    METHOD_ENTRY("IJoint::attachObjectA")

    m_pObjectA = _pA;
    m_AnchorIDA = _AnchorID;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Attach the second object to joint
///
/// \param _pB Second object 
/// \param _AnchorID Anchor ID
///
///////////////////////////////////////////////////////////////////////////////
inline void IJoint::attachObjectB(CObject* _pB, const AnchorIDType& _AnchorID)
{
    METHOD_ENTRY("IJoint::attachObjectA")

    m_pObjectB = _pB;
    m_AnchorIDB = _AnchorID;
}

#endif
