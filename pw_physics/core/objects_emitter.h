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
/// \file       object_emitter.h
/// \brief      Prototype of class "CObjectEmitter"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2014-02-16
///
////////////////////////////////////////////////////////////////////////////////

#ifndef OBJECT_EMITTER_H
#define OBJECT_EMITTER_H

//--- Standard header --------------------------------------------------------//

//--- Program header ---------------------------------------------------------//
#include "emitter.h"

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class for a source that emits objects.
///
////////////////////////////////////////////////////////////////////////////////
class CObjectEmitter : public IEmitter
{
    
    public:

        //--- Constructor/Destructor -----------------------------------------//
        CObjectEmitter();
        ~CObjectEmitter();
   
        //--- Constant Methods -----------------------------------------------//
        const EmitterType getEmitterType() const;
        
        //--- Methods --------------------------------------------------------//
        void emit(const double&);
        void init();
        void setTemplate(IObject* const, IObjectVisuals* const);

    private:
        
        IObject*        m_pTemplate;        ///< Template for objects to emit
        IObjectVisuals* m_pTemplateVisuals; ///< Template visuals for objects to emit
        
};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Get the emitters type - RTTI
///
/// \return Type: object emitter
///
////////////////////////////////////////////////////////////////////////////////
inline const EmitterType CObjectEmitter::getEmitterType() const
{
    METHOD_ENTRY("CObjectEmitter::getEmitterType")
    return EMITTER_OBJECT;
}


#endif
