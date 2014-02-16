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
/// \file       emitter.h
/// \brief      Prototype of class "IEmitter"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2014-02-16
///
////////////////////////////////////////////////////////////////////////////////

#ifndef EMITTER_H
#define EMITTER_H

//--- Standard header --------------------------------------------------------//

//--- Program header ---------------------------------------------------------//

/// Specifies the mode for emitting world entities
typedef enum
{
    EMITTER_EMIT_ONCE
} EmitterModeType;

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Interface for a source that emits world objects like bodies or
///        debris.
///
////////////////////////////////////////////////////////////////////////////////
class IEmitter
{
    
    public:

        //--- Constructor/Destructor -----------------------------------------//
        IEmitter() : m_EmitterMode(EMITTER_EMIT_ONCE){};
        virtual ~IEmitter();
        
        //--- Constant Methods -----------------------------------------------//
        const EmitterModeType getMode() {return m_EmitterMode;}
        
        void startEmitation() const {};
        
        //--- Methods --------------------------------------------------------//

    private:
        
        EmitterModeType m_EmitterMode;
        
};

//--- Implementation is done here for inline optimisation --------------------//


#endif
