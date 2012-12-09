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
#include "camera.h"
#include "object.h"
#include "visuals.h"

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class for visualisation of world objects
///
////////////////////////////////////////////////////////////////////////////////
class IObjectVisuals
{
    
    public:

        //--- Constructor/Destructor -----------------------------------------//
        IObjectVisuals(IObject* const);
        virtual ~IObjectVisuals();
        
        //--- Constant Methods -----------------------------------------------//
        virtual void draw(const CCamera* const) const;

        virtual const std::vector<IVisuals*>& getShapeVisuals() const;
                
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
/// \brief Draw all shape visuals of this object
///
/// \param _pCamera Draw visuals with respect to this camera
///
////////////////////////////////////////////////////////////////////////////////
inline void IObjectVisuals::draw(const CCamera* const _pCamera) const
{
    METHOD_ENTRY("draw")
    
    for (int i=0; i<m_Visuals.size(); ++i) m_Visuals[i]->draw(_pCamera);
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

#endif
