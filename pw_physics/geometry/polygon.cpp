////////////////////////////////////////////////////////////////////////////////
//
// This file is part of planeworld, a 2D simulation of physics and much more.
// Copyright (C) 2009-2016 Torsten Büschenfeld
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
/// \file       polygon.cpp
/// \brief      Implementation of class "CPolygon"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2009-10-18
///
////////////////////////////////////////////////////////////////////////////////

#include "polygon.h"

#include <type_traits>

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Clones polygon
///
/// \return Pointer to cloned polygon
///
////////////////////////////////////////////////////////////////////////////////
CPolygon* CPolygon::clone() const
{
    METHOD_ENTRY("CPolygon::clone")
    
    CPolygon* pClone = new CPolygon();
    MEM_ALLOC("IShape")
    
    pClone->copy(this);
    
    return pClone;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Adds a vertex to the current list of vertices
///
/// \param _vecV Vertex to be added to list
///
///////////////////////////////////////////////////////////////////////////////
void CPolygon::addVertex(const Vector2d& _vecV)
{
    METHOD_ENTRY("CPolygon::addVertex");

    m_VertList0.push_back(_vecV);
    m_VertList.push_back(_vecV);
    
    this->updateGeometry();
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Adds a vertex to the current list of vertices
///
/// \param _fX Vertex x-coordinate to be added to list
/// \param _fY Vertex y-coordinate to be added to list
///
///////////////////////////////////////////////////////////////////////////////
void CPolygon::addVertex(const double& _fX, const double& _fY)
{
    METHOD_ENTRY("CPolygon::addVertex");

    m_VertList0.push_back(Vector2d(_fX, _fY));
    m_VertList.push_back(Vector2d(_fX, _fY));
    
    this->updateGeometry();
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Transforms the shape
///
/// \param _fAngle Rotation angle
/// \param _vecCOM Center of mass in local (object) coordinates
/// \param _vecV Translation vector for global translation
///
///////////////////////////////////////////////////////////////////////////////
void CPolygon::transform(const double& _fAngle,
                         const Vector2d& _vecCOM,
                         const Vector2d& _vecV)
{
    METHOD_ENTRY("CPolygon::transform");

    Rotation2Dd Rotation(_fAngle);

    VertexListType::const_iterator ci = m_VertList0.begin();
    VertexListType::iterator it = m_VertList.begin();
    
    (*it) = Rotation * ((*ci)-_vecCOM) + _vecCOM + _vecV;
    
    m_AABB.setLowerLeft( (*it));
    m_AABB.setUpperRight((*it));

    ++it; ++ci;
    while (ci != m_VertList0.end())
    {
        (*it) = Rotation * ((*ci)-_vecCOM) + _vecCOM + _vecV;

        // Update bounding box
        m_AABB.update((*it));
        ++it;
        ++ci;
    }
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Input stream for game state information
///
/// \param _is  Source stream
///
/// \return Remaining stream with game state information
///
////////////////////////////////////////////////////////////////////////////////
std::istream& CPolygon::myStreamIn(std::istream& _is)
{
    METHOD_ENTRY("CPolygon::myStreamIn")
    
    std::string strTmp;
    _is >> strTmp;
    
    m_VertList.clear();
    m_VertList0.clear();
    
    // Cast streamable basetype to strongly typed enum PolygonType
    std::underlying_type<PolygonType>::type nPolygonType;
    _is >> nPolygonType; m_PolygonType = static_cast<PolygonType>(nPolygonType);
    
    VertexListType::size_type nSize;
    _is >> nSize;
    
    for (auto i=0u; i<nSize; ++i)
    {
        Vector2d vecIn;
        _is >> vecIn[0] >> vecIn[1];
        m_VertList.push_back(vecIn);
    }
    for (auto i=0u; i<nSize; ++i)
    {
        Vector2d vecIn;
        _is >> vecIn[0] >> vecIn[1];
        m_VertList0.push_back(vecIn);
    }
        
    return _is;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Output stream for game state information
///
/// \param _os  Source stream
///
/// \return Stream with game state information of CPolygon instance
///
////////////////////////////////////////////////////////////////////////////////
std::ostream& CPolygon::myStreamOut(std::ostream& _os)
{
    METHOD_ENTRY("CPolygon::myStreamOut")

    _os << "Polygon:" << std::endl;
    
    // Cast strongly typed enum PolygonType to streamable base type
    auto nPolygonType = static_cast<std::underlying_type<PolygonType>::type>(m_PolygonType);
    _os << nPolygonType << std::endl;
    
    _os << m_VertList.size() << std::endl;
    for (const auto ci : m_VertList)
    {
        _os << ci[0] << " " <<
               ci[1] << std::endl;
    }
    for (const auto ci : m_VertList0)
    {
        _os << ci[0] << " " <<
               ci[1] << std::endl;
    }
    
    return _os;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Copies information of a given polygon
///
/// This method does not create a new polygon, use clone in that case!
///
/// \param _pShape Shape to be copied
///
////////////////////////////////////////////////////////////////////////////////
void CPolygon::myCopy(const IShape* const _pShape)
{
    METHOD_ENTRY("CPolygon::myCopy");
    
    const CPolygon* const pPolygon = static_cast<const CPolygon* const>(_pShape);
        
    m_PolygonType  = pPolygon->m_PolygonType;
    m_VertList     = pPolygon->m_VertList;
    m_VertList0    = pPolygon->m_VertList0;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Update geometry relevant data, e.g. inertia, area, center of mass
///
////////////////////////////////////////////////////////////////////////////////
void CPolygon::myUpdateGeometry()
{
    METHOD_ENTRY("CPolygon::myUpdateGeometry")
    
    // Shape is no longer valid
    // Recalculate accordant parameters
    m_vecCentroid.setZero();
    m_fArea = 0.0;
    m_fInertia = 0.0;
    
    auto nSize = m_VertList0.size();
    std::vector<double> vecLength; vecLength.reserve(nSize);
    double fLength = 0.0;

    switch (m_PolygonType)
    {
        case PolygonType::LINE_LOOP:
        {
            // Calculate centroid
            std::vector<Vector2d> vecCenter; vecCenter.reserve(nSize);
            for (auto i=0u; i<nSize-1; ++i)
            {
                vecLength.push_back((m_VertList0[i+1]-m_VertList0[i]).norm());
                vecCenter.push_back((m_VertList0[i+1]+m_VertList0[i])*0.5);
                m_vecCentroid += vecCenter[i]*vecLength[i];
                fLength += vecLength[i];
            }
            vecLength.push_back((m_VertList0[0]-m_VertList0[nSize-1]).norm());
            vecCenter.push_back((m_VertList0[nSize-1]+m_VertList0[0])*0.5);
            m_vecCentroid += vecCenter[nSize-1]*vecLength[nSize-1];
            fLength       += vecLength[nSize-1];
            if (fLength > 0.0) m_vecCentroid /= fLength;
            
            // Calculate inertia
            for (auto i=0u; i<nSize; ++i)
            {
                // Calculate for each segment: (m*L^2)/12
                // Additionally, add Steiner's part (Steiner's theorem)
                m_fInertia += m_fMass * vecLength[i] * vecLength[i] * vecLength[i] / 12.0 + 
                              m_fMass * vecLength[i] * (vecCenter[i]-m_vecCentroid).squaredNorm();
            }
            if (fLength > 0.0) m_fInertia /= fLength;
            break;
        }
        case PolygonType::LINE_SINGLE:
        {
            // Calculate centroid
            std::vector<Vector2d> vecCenter; vecCenter.reserve(nSize);
            for (auto i=0u; i<(nSize>>1); ++i)
            {
                vecLength.push_back((m_VertList0[2*i+1]-m_VertList0[2*i]).norm());
                vecCenter.push_back((m_VertList0[2*i+1]+m_VertList0[2*i])*0.5);
                m_vecCentroid += vecCenter[i]*vecLength[i];
                fLength += vecLength[i];
            }
            if (fLength > 0.0) m_vecCentroid /= fLength;
            
            // Calculate inertia
            for (auto i=0u; i<vecLength.size(); ++i)
            {
                // Calculate for each segment: (m*L^2)/12
                // Additionally, add Steiner's part (Steiner's theorem)
                m_fInertia += m_fMass * vecLength[i] * vecLength[i] * vecLength[i] / 12.0 + 
                              m_fMass * vecLength[i] * (vecCenter[i]-m_vecCentroid).squaredNorm();
            }
            if (fLength > 0.0) m_fInertia /= fLength;
            break;
        }
        case PolygonType::LINE_STRIP:
        {
            // Calculate centroid
            std::vector<Vector2d> vecCenter; vecCenter.reserve(nSize);
            for (auto i=0u; i<nSize-1; ++i)
            {
                vecLength.push_back((m_VertList0[i+1]-m_VertList0[i]).norm());
                vecCenter.push_back((m_VertList0[i+1]+m_VertList0[i])*0.5);
                m_vecCentroid += vecCenter[i]*vecLength[i];
                fLength += vecLength[i];
            }
            if (fLength > 0.0) m_vecCentroid /= fLength;
            
            // Calculate inertia
            for (auto i=0u; i<nSize-1; ++i)
            {
                // Calculate for each segment: (m*L^2)/12
                // Additionally, add Steiner's part (Steiner's theorem)
                m_fInertia += m_fMass * vecLength[i] * vecLength[i] * vecLength[i] / 12.0 + 
                              m_fMass * vecLength[i] * (vecCenter[i]-m_vecCentroid).squaredNorm();
            }
            if (fLength > 0.0) m_fInertia /= fLength;
            break;
        }
        case PolygonType::FILLED:
        {
            std::vector<double> vecArea; vecArea.reserve(nSize);
            for (auto i=0u; i<nSize-1; ++i)
            {
                vecArea.push_back((m_VertList0[i][0]*m_VertList0[i+1][1]) -
                                  (m_VertList0[i][1]*m_VertList0[i+1][0]));
                m_fArea += vecArea[i];
            }
            vecArea.push_back((m_VertList0[nSize-1][0]*m_VertList0[0][1]) -
                              (m_VertList0[nSize-1][1]*m_VertList0[0][0]));
            m_fArea += vecArea[nSize-1];
            m_fArea *= 0.5; // Note: factor 0.5 not stored in vecArea
            
            if (m_fArea != 0)
            {
                for (auto i=0u; i<nSize-1; ++i)
                {
                    m_vecCentroid[0] += (m_VertList0[i][0] + m_VertList0[i+1][0]) *
                                        ((m_VertList0[i][0]*m_VertList0[i+1][1]) -
                                        (m_VertList0[i][1]*m_VertList0[i+1][0]));
                    m_vecCentroid[1] += (m_VertList0[i][1] + m_VertList0[i+1][1]) *
                                        ((m_VertList0[i][0]*m_VertList0[i+1][1]) -
                                        (m_VertList0[i][1]*m_VertList0[i+1][0]));
                }
                m_vecCentroid[0] += (m_VertList0[nSize-1][0] + m_VertList0[0][0]) *
                                    ((m_VertList0[nSize-1][0]*m_VertList0[0][1]) -
                                    (m_VertList0[nSize-1][1]*m_VertList0[0][0]));
                m_vecCentroid[1] += (m_VertList0[nSize-1][1] + m_VertList0[0][1]) *
                                    ((m_VertList0[nSize-1][0]*m_VertList0[0][1]) -
                                    (m_VertList0[nSize-1][1]*m_VertList0[0][0]));
                m_vecCentroid = 1.0/(6.0*m_fArea) * m_vecCentroid;
                
                // Calculate inertia
                for (auto i=0u; i<nSize-1; ++i)
                {
                    m_fInertia +=   vecArea[i] * (
                                    (m_VertList0[i]-m_vecCentroid).squaredNorm()+
                                    (m_VertList0[i]-m_vecCentroid).dot(
                                        (m_VertList0[i+1]-m_vecCentroid)) +
                                    (m_VertList0[i+1]-m_vecCentroid).squaredNorm());
                }
                m_fInertia +=   vecArea[nSize-1] * (
                                (m_VertList0[nSize-1]-m_vecCentroid).squaredNorm()+
                                (m_VertList0[nSize-1]-m_vecCentroid).dot(
                                    (m_VertList0[0]-m_vecCentroid)) +
                                (m_VertList0[0]-m_vecCentroid).squaredNorm());
                m_fInertia *= m_fMass;
                m_fInertia /= 12.0*m_fArea; // Note: factor 2.0 because above,
                                            //       factor 0.5 is not stored in vecArea
            }
            else
            {
                DEBUG(
                    NOTICE_MSG("Polygon", "Not a valid polygon, area=0. => inertia=0.")
                )
            }
            break;
        }
    }
    DOM_VAR(DEBUG_MSG("Polygon", "Inertia calculated: " << m_fInertia))
}
