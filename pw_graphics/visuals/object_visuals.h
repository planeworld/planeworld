////////////////////////////////////////////////////////////////////////////////
//
// This file is part of planeworld, a 2D simulation of physics and much more.
// Copyright (C) 2012-2016 Torsten Büschenfeld
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
/// \file       object_visuals.h
/// \brief      Prototype of class "IObjectVisuals"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2012-12-03
///
////////////////////////////////////////////////////////////////////////////////

#ifndef OBJECT_VISUALS_H
#define OBJECT_VISUALS_H

//--- Standard header --------------------------------------------------------//

//--- Program header ---------------------------------------------------------//
#include "unique_id_referrer.h"
#include "visuals.h"

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class for visualisation of world objects
///
////////////////////////////////////////////////////////////////////////////////
class IObjectVisuals : virtual public CGraphicsBase,
                               public IUniqueIDReferrer<IObject>
{
    
    public:

        //--- Constructor/Destructor -----------------------------------------//
        IObjectVisuals();
        IObjectVisuals(IObject* const);
        virtual ~IObjectVisuals();
        
        //--- Constant Methods -----------------------------------------------//
        IObjectVisuals* clone(IObject* const) const;
        
        virtual void draw(CCamera* const) const;

        virtual const std::vector<IVisuals*>& getShapeVisuals() const;
        
        //--- Methods --------------------------------------------------------//
        void addVisuals(IVisuals* const);
        
        //--- friends --------------------------------------------------------//
        friend std::istream&    operator>>(std::istream&, IObjectVisuals* const);
        friend std::ostream&    operator<<(std::ostream&, IObjectVisuals* const);
        
    protected:
      
        std::vector<IVisuals*> m_Visuals;   ///< List of visuals

};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor
///
////////////////////////////////////////////////////////////////////////////////
inline IObjectVisuals::IObjectVisuals()
{
    METHOD_ENTRY("IObjectVisuals")
    CTOR_CALL("IObjectVisuals")
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor, initialises object pointer
///
/// \param _pObject Pointer to object
///
////////////////////////////////////////////////////////////////////////////////
inline IObjectVisuals::IObjectVisuals(IObject* const _pObject)
{
    METHOD_ENTRY("IObjectVisuals")
    CTOR_CALL("IObjectVisuals")
    
    IUniqueIDReferrer::attachTo(_pObject);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Return a list of all shape visuals of this object
///
/// \return List of shape visuals for this object
///
////////////////////////////////////////////////////////////////////////////////
inline const std::vector<IVisuals*>& IObjectVisuals::getShapeVisuals() const
{
    METHOD_ENTRY("IObjectVisuals::getShapeVisuals")
    return m_Visuals;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Add shape visuals to this object
///
/// \param _pVisuals Shape visuals to add to this object
///
////////////////////////////////////////////////////////////////////////////////
inline void IObjectVisuals::addVisuals(IVisuals* const _pVisuals)
{
    METHOD_ENTRY("addVisuals")
    m_Visuals.push_back(_pVisuals);
}

#endif // OBJECT_VISUALS_H
