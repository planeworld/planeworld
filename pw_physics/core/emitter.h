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
#include <chrono>
#include <list>
#include <random>

//--- Program header ---------------------------------------------------------//
#include "kinematics_state_user.h"
#include "unique_id_user.h"
#include "unique_id_referrer.h"
#include "world_data_storage_user.h"

//--- Enumerations -----------------------------------------------------------//
/// Specifies the type of the emitter
enum class EmitterType
{
    NONE,
    PARTICLE,
    OBJECT
};

/// Specifies the type of possible distributions
enum class EmitterDistributionType
{
    NONE,
    CIRCULAR_FIELD,
    POINT_SOURCE,
    RECTANGULAR_FIELD
};

/// Specifies the mode for emitting world entities
enum class EmitterModeType
{
    NONE,
    ONCE,
    TIMED
};

//--- Constants --------------------------------------------------------------//
const double EMITTER_DEFAULT_ANGLE = 0.0; ///< Default angle of emitted objects
const double EMITTER_DEFAULT_ANGLE_STD = 1.0; ///< Default angle of velocity of emitted objects
const double EMITTER_DEFAULT_LIMIT_MAX_X =  100.0; ///< Default distribution limits
const double EMITTER_DEFAULT_LIMIT_MAX_Y =  100.0; ///< Default distribution limits
const double EMITTER_DEFAULT_LIMIT_MIN_X = -100.0; ///< Default distribution limits
const double EMITTER_DEFAULT_LIMIT_MIN_Y = -100.0; ///< Default distribution limits
const double EMITTER_DEFAULT_FREQUENCY = 1.0; ///< Default frequency of emitter
const double EMITTER_DEFAULT_VELOCITY = 10.0; ///< Default velocity of emitted objects
const double EMITTER_DEFAULT_VELOCITY_STD = 1.0; ///< Default standard deviation of velocity of emitted objects

const EmitterDistributionType EMITTER_DEFAULT_DISTRIBUTION = EmitterDistributionType::RECTANGULAR_FIELD; ///< Default emitter distribution
const EmitterModeType EMITTER_DEFAULT_MODE = EmitterModeType::ONCE;                                      ///<  Default emitter mode
const EmitterType EMITTER_DEFAULT_TYPE = EmitterType::PARTICLE;                                          ///< Default emitter type

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Interface for a source that emits world objects like bodies or
///        particle.
///
////////////////////////////////////////////////////////////////////////////////
class IEmitter : public IKinematicsStateUser,
                 public IUniqueIDUser,
                 public IWorldDataStorageUser
{
    
    public:

        //--- Constructor/Destructor -----------------------------------------//
        IEmitter();
        virtual ~IEmitter();
        
        //--- Constant Methods -----------------------------------------------//
        virtual EmitterType         getEmitterType() const;
        
        EmitterModeType             getMode() const;
        const double&               getVelocity() const;
        const double&               getVelocityStd() const;
        const bool&                 isActive() const;
        
        //--- Methods --------------------------------------------------------//
        virtual void emit(const double& = -1.0) = 0;
        virtual void init() = 0;
        
        virtual void setNumber(const std::uint32_t&);
        
        void activate();
        void deactivate();
        
        void setAngle(const double&);
        void setAngleStd(const double&);
        void setDistribution(const EmitterDistributionType&);
        void setFrequency(const double&);
        void setLimits(const double&, const double&, const double&, const double&);
        void setMode(const EmitterModeType&);
        void setOrigin(const Vector2d&);
        void setVelocity(const double&);
        void setVelocityStd(const double&);
        
    protected:

        EmitterModeType         m_EmitterMode;              ///< Emit mode
        EmitterDistributionType m_EmitterDistribution;      ///< Distribution of emitter
        
        std::mt19937                            m_Generator;    ///< Generator for random distributions
        std::normal_distribution<double>        m_NormalDist;   ///< Normal distribution;
        std::uniform_real_distribution<double>  m_UniformDist;  ///< Uniform distribution;
        
        std::uint32_t           m_nNr;                      ///< Maximum number of emitted entities
        
        bool                    m_bActive;                  ///< Flags if emitter is activated
        
        double                  m_fAngleStd;                ///< Angle standard deviation for point source distribution
        double                  m_fFrequency;               ///< Frequency of timed emitter
        double                  m_fMinX;                    ///< Minimum limit x direction, rectangular field
        double                  m_fMaxX;                    ///< Maximum limit x direction, rectangular field
        double                  m_fMinY;                    ///< Minimum limit y direction, rectangular field
        double                  m_fMaxY;                    ///< Maximum limit y direction, rectangular field
        double                  m_fVelocity;                ///< Velocity of emitted entities
        double                  m_fVelocityStd;             ///< Velocity standard deviation of emitted entities
        
        double                  m_fResidual;                ///< Residual of emitation, since engine frequency differs from emitation frequency
};

/// Specifies a list of emitters
typedef std::unordered_multimap<std::string, IEmitter*> EmittersType;

//--- Enum parser ------------------------------------------------------------//
const std::map<EmitterType, std::string> mapEmitterToString = {
    {EmitterType::PARTICLE, "particle"},
    {EmitterType::OBJECT, "object"}
}; ///< Map from EmitterType to string

const std::map<std::string, EmitterType> STRING_TO_EMITTER_TYPE_MAP = {
    {"particle", EmitterType::PARTICLE},
    {"object", EmitterType::OBJECT}
}; ///< Map from string to Emitter

const std::map<EmitterDistributionType, std::string> mapEmitterDistributionToString = {
    {EmitterDistributionType::CIRCULAR_FIELD, "circular_field"},
    {EmitterDistributionType::POINT_SOURCE, "point_source"},
    {EmitterDistributionType::RECTANGULAR_FIELD, "rectangular_field"}
}; ///< Map from EmitterDistributionType to string

const std::map<std::string, EmitterDistributionType> STRING_TO_EMITTER_DISTRIBUTION_TYPE_MAP = {
    {"circular_field", EmitterDistributionType::CIRCULAR_FIELD},
    {"point_source", EmitterDistributionType::POINT_SOURCE},
    {"rectangular_field", EmitterDistributionType::RECTANGULAR_FIELD}
}; ///< Map from string to EmitterDistributionType

const std::map<EmitterModeType, std::string> mapEmitterModeToString = {
    {EmitterModeType::ONCE, "once"},
    {EmitterModeType::TIMED, "timed"}
}; ///< Map from EmitterModeType to string

const std::map<std::string, EmitterModeType> STRING_TO_EMITTER_MODE_TYPE_MAP = {
    {"once", EmitterModeType::ONCE},
    {"timed", EmitterModeType::TIMED}
}; ///< Map from string to EmitterMode

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Maps given string to emitter type
///
/// \return Emitter type
///
////////////////////////////////////////////////////////////////////////////////
static EmitterType mapStringToEmitterType(const std::string& _strS)
{
    METHOD_ENTRY("mapStringToEmitterType")
    
    const auto ci = STRING_TO_EMITTER_TYPE_MAP.find(_strS);
    if (ci != STRING_TO_EMITTER_TYPE_MAP.end())
        return ci->second;
    else
        return EmitterType::NONE;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Maps given string to emitter distribution type
///
/// \return Emitter distribution type
///
////////////////////////////////////////////////////////////////////////////////
static EmitterDistributionType mapStringToEmitterDistributionType(const std::string& _strS)
{
    METHOD_ENTRY("mapStringToEmitterDistributionType")
    
    const auto ci = STRING_TO_EMITTER_DISTRIBUTION_TYPE_MAP.find(_strS);
    if (ci != STRING_TO_EMITTER_DISTRIBUTION_TYPE_MAP.end())
        return ci->second;
    else
        return EmitterDistributionType::NONE;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Maps given string to emitter mode type
///
/// \return Emitter mode type
///
////////////////////////////////////////////////////////////////////////////////
static EmitterModeType mapStringToEmitterModeType(const std::string& _strS)
{
    METHOD_ENTRY("mapStringToEmitterModeType")
    
    const auto ci = STRING_TO_EMITTER_MODE_TYPE_MAP.find(_strS);
    if (ci != STRING_TO_EMITTER_MODE_TYPE_MAP.end())
        return ci->second;
    else
        return EmitterModeType::NONE;
}

//--- Implementation is done here for inline optimisation --------------------//

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor
///
///////////////////////////////////////////////////////////////////////////////
inline IEmitter::IEmitter() : m_EmitterMode(EMITTER_DEFAULT_MODE),
                              m_EmitterDistribution(EMITTER_DEFAULT_DISTRIBUTION),
                              m_NormalDist(0.0,1.0),
                              m_UniformDist(0.0,1.0),
                              m_nNr(10),
                              m_bActive(true),
                              m_fAngleStd(EMITTER_DEFAULT_ANGLE_STD),
                              m_fFrequency(EMITTER_DEFAULT_FREQUENCY),
                              m_fMinX(EMITTER_DEFAULT_LIMIT_MIN_X),
                              m_fMaxX(EMITTER_DEFAULT_LIMIT_MAX_X),
                              m_fMinY(EMITTER_DEFAULT_LIMIT_MIN_Y),
                              m_fMaxY(EMITTER_DEFAULT_LIMIT_MAX_Y),
                              m_fVelocity(EMITTER_DEFAULT_VELOCITY),
                              m_fVelocityStd(EMITTER_DEFAULT_VELOCITY_STD),
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
inline EmitterType IEmitter::getEmitterType() const
{
    METHOD_ENTRY("IEmitter::getEmitterType")
    return EmitterType::NONE;
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
inline EmitterModeType IEmitter::getMode() const
{
    METHOD_ENTRY("IEmitter::getMode")
    return m_EmitterMode;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Return the emitters velocity
///
/// \return Emitter velocity
///
///////////////////////////////////////////////////////////////////////////////
inline const double& IEmitter::getVelocity() const
{
    METHOD_ENTRY("IEmitter::getVelocity")
    return m_fVelocity;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Return the emitters velocity standard deviation
///
/// \return Emitter velocity standard deviation
///
///////////////////////////////////////////////////////////////////////////////
inline const double& IEmitter::getVelocityStd() const
{
    METHOD_ENTRY("IEmitter::getVelocityStd")
    return m_fVelocityStd;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Return the activation state of emitter
///
/// \return Emitter active?
///
///////////////////////////////////////////////////////////////////////////////
inline const bool& IEmitter::isActive() const
{
    METHOD_ENTRY("IEmitter::isActive")
    return m_bActive;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets the number of emitted entities
///
/// \param _nNr Number of entities.
///
///////////////////////////////////////////////////////////////////////////////
inline void IEmitter::setNumber(const std::uint32_t& _nNr)
{
    METHOD_ENTRY("IEmitter::setNumber")
    m_nNr = _nNr;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Activates the emitter
///
///////////////////////////////////////////////////////////////////////////////
inline void IEmitter::activate()
{
    METHOD_ENTRY("IEmitter::activate")
    
    m_bActive = true;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Dectivates the emitter
///
///////////////////////////////////////////////////////////////////////////////
inline void IEmitter::deactivate()
{
    METHOD_ENTRY("IEmitter::deactivate")
    
    m_bActive = false;
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
    
    if (m_EmitterDistribution != EmitterDistributionType::POINT_SOURCE)
    {
        NOTICE_MSG("Emitter Interface", "Setting angle although distribution " 
                   "mode is not point_source. This does not have any effect.")
    }
    else
    {
        m_KinematicsState.setAngle(_fA);
    }
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Set angle standard deviation of point source distribution
///
/// \param _fAV Angle standard deviation of point source distribution
///
///////////////////////////////////////////////////////////////////////////////
inline void IEmitter::setAngleStd(const double& _fAV)
{
    METHOD_ENTRY("IEmitter::setAngleStd")
    
    if (m_EmitterDistribution != EmitterDistributionType::POINT_SOURCE)
    {
        NOTICE_MSG("Emitter Interface", "Setting angle standard deviation although distribution " 
                   "mode is not point_source. This does not have any effect.")
    }
    else
    {
        m_fAngleStd = _fAV;
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
       
    if (m_EmitterMode == EmitterModeType::ONCE)
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
       
    if (m_EmitterDistribution != EmitterDistributionType::RECTANGULAR_FIELD)
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
/// \brief Sets the origin for this emitter
///
/// \param _vecOrigin Origin of emitter
///
///////////////////////////////////////////////////////////////////////////////
inline void IEmitter::setOrigin(const Vector2d& _vecOrigin)
{
    METHOD_ENTRY("IEmitter::setOrigin")
    m_KinematicsState.setOrigin(_vecOrigin);
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
    
    if (m_EmitterDistribution != EmitterDistributionType::POINT_SOURCE)
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
/// \brief Set velocity standard deviation for point source distribution
///
/// \param _fVStd Velocity standard deviation for point source distribution
///
///////////////////////////////////////////////////////////////////////////////
inline void IEmitter::setVelocityStd(const double& _fVStd)
{
    METHOD_ENTRY("IEmitter::setVelocityStd")
    
    if (m_EmitterDistribution != EmitterDistributionType::POINT_SOURCE)
    {
        NOTICE_MSG("Emitter Interface", "Setting velocity standard deviation although distribution " 
                   "mode is not point_source. This does not have any effect.")
    }
    else
    {
        m_fVelocityStd = _fVStd;
    }
}

#endif
