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
/// \file       contact.h
/// \brief      Prototype of class "CContact"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2010-03-25
///
////////////////////////////////////////////////////////////////////////////////

#ifndef CONTACT_H
#define CONTACT_H

//--- Program header ---------------------------------------------------------//
#include "log.h"

//--- Standard header --------------------------------------------------------//
#include <list>

//--- Misc header ------------------------------------------------------------//
#include <eigen3/Eigen/Core>

using namespace Eigen;

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class to return contact information
///
////////////////////////////////////////////////////////////////////////////////
class CContact
{
    
    public:
    
        //--- Constructor/Destructor -----------------------------------------//
        CContact();

        //--- Constant methods -----------------------------------------------//
        Vector2d getPointsOfContact() const;
        bool occurred() const;

        //--- Methods --------------------------------------------------------//
        void setOccurred(const bool&);
        void setPOC(const Vector2d&);
        
    private:
        
        bool     m_bContact; ///< Flags if contact occurred
        Vector2d m_vecPOC;   ///< Point of contact

};

//--- Implementation is done here for inlining optimisation ------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor
///
////////////////////////////////////////////////////////////////////////////////
inline CContact::CContact():m_bContact(false)
{
    METHOD_ENTRY("CContact::CContact")
    CTOR_CALL("CContact::CContact")

    METHOD_EXIT("CContact::CContact")
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Return the list of contacts
///
/// \return Contact list
///
////////////////////////////////////////////////////////////////////////////////
inline Vector2d CContact::getPointsOfContact() const
{
    METHOD_ENTRY("CContact::getPointsOfContact")

    METHOD_EXIT("CContact::getPointsOfContact")
    return m_vecPOC;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Defines, where the contact occurred
///
/// \return Flag if contact occurred
///
////////////////////////////////////////////////////////////////////////////////
inline bool CContact::occurred() const
{
    METHOD_ENTRY("CContact::occurred")

    METHOD_EXIT("CContact::occurred")
    return m_bContact;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Defines, where the contact occurred
///
/// \param _bContact Flags if contact occurred
///
////////////////////////////////////////////////////////////////////////////////
inline void CContact::setOccurred(const bool& _bContact)
{
    METHOD_ENTRY("CContact::setOccurred")

    m_bContact = _bContact;
    
    METHOD_EXIT("CContact::setOccurred")
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Defines, where the contact occurred
///
/// \param _vecPOC Point of contact
///
////////////////////////////////////////////////////////////////////////////////
inline void CContact::setPointOfContact(const Vector2d>& _POC)
{
    METHOD_ENTRY("CContact::setPointsOfContact")

    m_vecPOC = _POCList;
    
    METHOD_EXIT("CContact::setPointsOfContact")
}

#endif
