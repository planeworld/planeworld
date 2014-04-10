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
#include "visuals.h"

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class for visualisation of world objects
///
////////////////////////////////////////////////////////////////////////////////
class IObjectVisuals : virtual public CGraphicsBase
{
    
    public:

        //--- Constructor/Destructor -----------------------------------------//
        IObjectVisuals(IObject* const);
        virtual ~IObjectVisuals();
        
        //--- Constant Methods -----------------------------------------------//
        IObjectVisuals* clone(IObject* const) const;
        
        virtual void draw(const CCamera* const) const;

        virtual const std::vector<IVisuals*>& getShapeVisuals() const;
        
        IObject* const getObject() const;
                
        //--- Methods --------------------------------------------------------//
        void addVisuals(IVisuals* const);
        
    protected:
        
        IObjectVisuals();                   ///< Constructor is private        
        IObject* m_pObject;                 ///< Pointer to corresponding object
        std::vector<IVisuals*> m_Visuals;   ///< List of visuals

};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor, initialises object pointer
///
/// \param _pObject Pointer to object
///
////////////////////////////////////////////////////////////////////////////////
inline IObjectVisuals::IObjectVisuals(IObject* const _pObject) : m_pObject(_pObject)
{
    METHOD_ENTRY("IObjectVisuals")
    CTOR_CALL("IObjectVisuals")
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
/// \brief Returns object these visuals refer to.
///
/// \return Object these visuals refer to
///
////////////////////////////////////////////////////////////////////////////////
inline IObject* const IObjectVisuals::getObject() const
{
    METHOD_ENTRY("IObjectVisuals::getObject")
    return m_pObject;
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

#endif
