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
        
        //--- Methods --------------------------------------------------------//

    private:
        
};

//--- Implementation is done here for inline optimisation --------------------//


#endif
