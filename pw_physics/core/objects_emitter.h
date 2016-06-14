////////////////////////////////////////////////////////////////////////////////
//
// This file is part of planeworld, a 2D simulation of physics and much more.
// Copyright (C) 2014-2016 Torsten Büschenfeld
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
/// \file       objects_emitter.h
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
        EmitterType getEmitterType() const;
        
        //--- Methods --------------------------------------------------------//
        void emit(const double&);
        void init();
        void setTemplate(CObject* const, IObjectVisuals* const);

    private:
        
        CObject*        m_pTemplate;        ///< Template for objects to emit
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
inline EmitterType CObjectEmitter::getEmitterType() const
{
    METHOD_ENTRY("CObjectEmitter::getEmitterType")
    return EMITTER_OBJECT;
}


#endif
