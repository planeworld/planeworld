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
/// \file       debris_emitter.h
/// \brief      Prototype of class "CDebrisEmitter"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2014-02-16
///
////////////////////////////////////////////////////////////////////////////////

#ifndef DEBRIS_EMITTER_H
#define DEBRIS_EMITTER_H

//--- Standard header --------------------------------------------------------//

//--- Program header ---------------------------------------------------------//
#include "emitter.h"

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class for a source that emits debris.
///
////////////////////////////////////////////////////////////////////////////////
class CDebrisEmitter : public IEmitter
{
    
    public:

        //--- Constructor/Destructor -----------------------------------------//
        CDebrisEmitter();
        ~CDebrisEmitter();
        
        //--- Constant Methods -----------------------------------------------//
        
        //--- Methods --------------------------------------------------------//

    private:
        
};

//--- Implementation is done here for inline optimisation --------------------//


#endif
