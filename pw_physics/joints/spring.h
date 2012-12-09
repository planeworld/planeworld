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
/// \file       spring.h
/// \brief      Prototype of class "CSpring"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2009-10-18
///
////////////////////////////////////////////////////////////////////////////////

#ifndef SPRING_H
#define SPRING_H

//--- Program header ---------------------------------------------------------//
#include "joint.h"

//--- Standard header --------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class for spring connection between objects
///
////////////////////////////////////////////////////////////////////////////////
class CSpring : public IJoint
{
    
    public:
    
        //--- Constructor/Destructor -----------------------------------------//
        CSpring();
        virtual ~CSpring();

        //--- Constant methods -----------------------------------------------//
        const double getC() const;
        const double getLength() const;

        void react() const;

        //--- Methods --------------------------------------------------------//
        void setC(const double&);
        void setLength(const double&);

    protected:

        double m_fC;        ///< Spring constant
        double m_fLength;   ///< Length of spring

};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the spring constant
///
/// \return Spring constant
///
////////////////////////////////////////////////////////////////////////////////
inline const double CSpring::getC() const
{
    METHOD_ENTRY("CSpring::getC")
    return m_fC;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the length of the spring in neutral position
///
/// \return Spring length in neutral position
///
////////////////////////////////////////////////////////////////////////////////
inline const double CSpring::getLength() const
{
    METHOD_ENTRY("CSpring::getLength")
    return m_fLength;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets the spring constant
///
/// \param _fC Spring constant
///
////////////////////////////////////////////////////////////////////////////////
inline void CSpring::setC(const double& _fC)
{
    METHOD_ENTRY("CSpring::setC")
    m_fC = _fC;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets the spring length for neutral position
///
/// \param _fLength Spring constant
///
////////////////////////////////////////////////////////////////////////////////
inline void CSpring::setLength(const double& _fLength)
{
    METHOD_ENTRY("CSpring::setLength")
    m_fLength = _fLength;
}

#endif
