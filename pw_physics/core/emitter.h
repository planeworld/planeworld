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
#include <list>
#include <random>

//--- Program header ---------------------------------------------------------//
#include "world_data_storage_user.h"


// Constants
const double EMITTER_DEFAULT_FREQUENCY = 1.0; ///< Default frequency of emitter

/// Specifies the type of the emitter
typedef enum 
{
    EMITTER_NONE,
    EMITTER_DEBRIS,
    EMITTER_OBJECT
} EmitterType;

/// Specifies the type of possible distributions
typedef enum 
{
    EMITTER_DISTRIBUTION_POINT_SOURCE,
    EMITTER_DISTRIBUTION_RECTANGULAR_FIELD,
    EMITTER_DISTRIBUTION_CIRCULAR_FIELD
} EmitterDistributionType;

/// Specifies the mode for emitting world entities
typedef enum
{
    EMITTER_MODE_EMIT_ONCE,
    EMITTER_MODE_TIMED
} EmitterModeType;

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Interface for a source that emits world objects like bodies or
///        debris.
///
////////////////////////////////////////////////////////////////////////////////
class IEmitter : public IUniverseScaled, public IWorldDataStorageUser
{
    
    public:

        //--- Constructor/Destructor -----------------------------------------//
        IEmitter();
        virtual ~IEmitter();
        
        //--- Constant Methods -----------------------------------------------//
        virtual const EmitterType   getEmitterType() const;
        
        const EmitterModeType       getMode() const;
        
        //--- Methods --------------------------------------------------------//
        virtual void emit(const double& = -1.0) = 0;
        virtual void init() = 0;
        
        void setAngle(const double&);
        void setAngleVariance(const double&);
        void setDistribution(const EmitterDistributionType&);
        void setFrequency(const double&);
        void setLimits(const double&, const double&, const double&, const double&);
        void setMode(const EmitterModeType&);
        void setNumber(const u_int32_t&);
        void setVelocity(const double&);
        void setVelocityVariance(const double&);

    protected:
        
        EmitterModeType         m_EmitterMode;              ///< Emit mode
        EmitterDistributionType m_EmitterDistribution;      ///< Distribution of emitter
        
        std::mt19937                            m_Generator;    ///< Generator for random distributions
        std::normal_distribution<double>        m_NormalDist;   ///< Normal distribution;
        std::uniform_real_distribution<double>  m_UniformDist;  ///< Uniform distribution;
        
        u_int32_t               m_nNr;                      ///< Maximum number of emitted entities
        
        double                  m_fAngle;                   ///< Angle for point source distribution
        double                  m_fAngleVariance;           ///< Angle variance for point source distribution
        double                  m_fFrequency;               ///< Frequency of timed emitter
        double                  m_fMinX;                    ///< Minimum limit x direction, rectangular field
        double                  m_fMaxX;                    ///< Maximum limit x direction, rectangular field
        double                  m_fMinY;                    ///< Minimum limit y direction, rectangular field
        double                  m_fMaxY;                    ///< Maximum limit y direction, rectangular field
        double                  m_fVelocity;                ///< Velocity of emitted entities
        double                  m_fVelocityVariance;        ///< Velocity variance of emitted entities
        
        double                  m_fResidual;                ///< Residual of emitation, since engine frequency differs from emitation frequency
                
};

/// Specifies a list of emitters
typedef std::list<IEmitter*> EmittersType;

//--- Implementation is done here for inline optimisation --------------------//

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor
///
///////////////////////////////////////////////////////////////////////////////
inline IEmitter::IEmitter() : m_EmitterMode(EMITTER_MODE_EMIT_ONCE),
                              m_EmitterDistribution(EMITTER_DISTRIBUTION_RECTANGULAR_FIELD),
                              m_NormalDist(0.0,1.0),
                              m_UniformDist(0.0,1.0),
                              m_nNr(10),
                              m_fAngle(0.0),
                              m_fAngleVariance(1.0),
                              m_fFrequency(EMITTER_DEFAULT_FREQUENCY),
                              m_fMinX(-1.0),
                              m_fMaxX(1.0),
                              m_fMinY(-1.0),
                              m_fMaxY(1.0),
                              m_fVelocity(1.0),
                              m_fVelocityVariance(1.0),
                              m_fResidual(0.0)
{
    METHOD_ENTRY("IEmitter::IEmitter")
    CTOR_CALL("IEmitter::IEmitter")
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Get the emitters type - RTTI
///
/// \return Type of emitter
///
////////////////////////////////////////////////////////////////////////////////
inline const EmitterType IEmitter::getEmitterType() const
{
    METHOD_ENTRY("IEmitter::getEmitterType")
    return EMITTER_NONE;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Destructor
///
///////////////////////////////////////////////////////////////////////////////
inline IEmitter::~IEmitter()
{
    METHOD_ENTRY("IEmitter::~IEmitter")
    DTOR_CALL("IEmitter::~IEmitter")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Return the emitter mode
///
/// \return Emitter mode
///
///////////////////////////////////////////////////////////////////////////////
inline const EmitterModeType IEmitter::getMode() const
{
    METHOD_ENTRY("IEmitter::getMode")
    return m_EmitterMode;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Set angle of point source distribution
///
/// \param _fA Angle of point source distribution
///
///////////////////////////////////////////////////////////////////////////////
inline void IEmitter::setAngle(const double& _fA)
{
    METHOD_ENTRY("IEmitter::setAngle")
    
    if (m_EmitterDistribution != EMITTER_DISTRIBUTION_POINT_SOURCE)
    {
        NOTICE_MSG("Emitter Interface", "Setting angle although distribution " 
                   "mode is not point_source. This does not have any effect.")
    }
    else
    {
        m_fAngle = _fA;
    }
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Set angle variance of point source distribution
///
/// \param _fAV Angle variance of point source distribution
///
///////////////////////////////////////////////////////////////////////////////
inline void IEmitter::setAngleVariance(const double& _fAV)
{
    METHOD_ENTRY("IEmitter::setAngleVariance")
    
    if (m_EmitterDistribution != EMITTER_DISTRIBUTION_POINT_SOURCE)
    {
        NOTICE_MSG("Emitter Interface", "Setting angle variance although distribution " 
                   "mode is not point_source. This does not have any effect.")
    }
    else
    {
        m_fAngleVariance = _fAV;
    }
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets an emitter distribution
///
/// \param _EmD Emitter distribution to be set
///
///////////////////////////////////////////////////////////////////////////////
inline void IEmitter::setDistribution(const EmitterDistributionType& _EmD)
{
    METHOD_ENTRY("IEmitter::setDistribution")
    m_EmitterDistribution = _EmD;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Set frequency of emitation
///
/// \param _fF Frequency of emitation
///
///////////////////////////////////////////////////////////////////////////////
inline void IEmitter::setFrequency(const double& _fF)
{
    METHOD_ENTRY("IEmitter::setFrequency")
       
    if (m_EmitterMode == EMITTER_MODE_EMIT_ONCE)
    {
        NOTICE_MSG("Emitter Interface", "Setting frequency for mode that emits " 
                   "only once. This does not have any effect.")
    }
    else
    {
        m_fFrequency = _fF;
    }
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Set limits for rectangular field
///
/// \param _fMinX Minimum x coordinate
/// \param _fMaxX Maximum x coordinate
/// \param _fMinY Minimum y coordinate
/// \param _fMaxY Maximum y coordinate
///
///////////////////////////////////////////////////////////////////////////////
inline void IEmitter::setLimits(const double& _fMinX, const double& _fMaxX,
                                const double& _fMinY, const double& _fMaxY)
{
    METHOD_ENTRY("IEmitter::setLimits")
       
    if (m_EmitterDistribution != EMITTER_DISTRIBUTION_RECTANGULAR_FIELD)
    {
        NOTICE_MSG("Emitter Interface", "Setting limits for rectangular field doesn't " 
                   "take any effect here.")
    }
    else
    {
        m_fMinX = _fMinX;
        m_fMaxX = _fMaxX;
        m_fMinY = _fMinY;
        m_fMaxY = _fMaxY;
    }
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets an emitter mode
///
/// \param _EmM Emitter mode to be set
///
///////////////////////////////////////////////////////////////////////////////
inline void IEmitter::setMode(const EmitterModeType& _EmM)
{
    METHOD_ENTRY("IEmitter::setMode")
    m_EmitterMode = _EmM;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets the number of emitted entities
///
/// \param _nNr Number of entities.
///
///////////////////////////////////////////////////////////////////////////////
inline void IEmitter::setNumber(const u_int32_t& _nNr)
{
    METHOD_ENTRY("IEmitter::setNumber")
    m_nNr = _nNr;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Set velocity for point source distribution
///
/// \param _fV Velocity for point source distribution
///
///////////////////////////////////////////////////////////////////////////////
inline void IEmitter::setVelocity(const double& _fV)
{
    METHOD_ENTRY("IEmitter::setVelocity")
    
    if (m_EmitterDistribution != EMITTER_DISTRIBUTION_POINT_SOURCE)
    {
        NOTICE_MSG("Emitter Interface", "Setting velocity although distribution " 
                   "mode is not point_source. This does not have any effect.")
    }
    else
    {
        m_fVelocity = _fV;
    }
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Set velocity variance for point source distribution
///
/// \param _fVV Velocity variance for point source distribution
///
///////////////////////////////////////////////////////////////////////////////
inline void IEmitter::setVelocityVariance(const double& _fVV)
{
    METHOD_ENTRY("IEmitter::setVelocityVariance")
    
    if (m_EmitterDistribution != EMITTER_DISTRIBUTION_POINT_SOURCE)
    {
        NOTICE_MSG("Emitter Interface", "Setting velocity variance although distribution " 
                   "mode is not point_source. This does not have any effect.")
    }
    else
    {
        m_fVelocityVariance = _fVV;
    }
}

#endif
