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
/// \file       pointmass.h
/// \brief      Prototype of class "CPointMass"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2009-10-18
///
////////////////////////////////////////////////////////////////////////////////

#ifndef POINTMASS_H
#define POINTMASS_H

//--- Standard header --------------------------------------------------------//

//--- Program header ---------------------------------------------------------//
#include "body.h"
#include "object.h"
#include "polyline.h"

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class representing a single particle
///
/// This class represents a particle, which is mainly a single mass with all its
/// features.
///
////////////////////////////////////////////////////////////////////////////////
class CPointMass : public IObject
{

    public:

        //--- Constructor/Destructor -----------------------------------------//
        CPointMass();
        virtual ~CPointMass();                          ///< Destructor

        //--- Constant Methods -----------------------------------------------//
        const Vector2d getAnchor(const int&) const {return Vector2d();};
//         void draw() const;

        //--- Methods --------------------------------------------------------//
        void addForce(const Vector2d&, const Vector2d&);
        void calcMovement(const double&);
        void clearForces();

    protected:

        const CContact myCollidesWith(IObject*);
        const CContact myCollidesWith(CBody*);
        const CContact myCollidesWith(CPointMass*);
//      void myDynamics(const double&){}
//      void mySetNewIntegrator(const IntegratorType&){}
//      void myInit(){}
        void myTransform();

        //--- Protected Variables --------------------------------------------//

};


#endif
