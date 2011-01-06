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
/// \file       random_line.cpp
/// \brief      Implementation of class "CRandomLine"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2009-10-18
///
////////////////////////////////////////////////////////////////////////////////

#include "random_line.h"

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor, initialising members
///
///////////////////////////////////////////////////////////////////////////////
CRandomLine::CRandomLine():m_fHeightMax(0.0), m_fHeightMin(0.0)
{
    METHOD_ENTRY("CRandomLine::CRandomLine")
    CTOR_CALL("CRandomLine::CRandomLine")

    m_LineType = GRAPHICS_LINETYPE_STRIP;

    METHOD_EXIT("CRandomLine::CRandomLine")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Destructor
///
///////////////////////////////////////////////////////////////////////////////
CRandomLine::~CRandomLine()
{
    METHOD_ENTRY("CRandomLine::~CRandomLine")
    DTOR_CALL("CRandomLine::~CRandomLine")

    METHOD_EXIT("CRandomLine::~CRandomLine")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Transforms the randomline
///
/// Randomline needs a special transform method because left and right values
/// have to be adjusted.
///
/// \param _fAngle Rotation angle
/// \param _vecV Translation vector
///
///////////////////////////////////////////////////////////////////////////////
void CRandomLine::transform(const double& _fAngle, const Vector2d& _vecV)
{
    METHOD_ENTRY("CRandomLine::transform")

    CPolyLine::transform(_fAngle, _vecV);

    // Transform additional values concerning clipping
    m_fLeft = m_fLeft0*std::cos(_fAngle) - m_fLeft0*std::sin(_fAngle) + _vecV[0];
    m_fRight = m_fRight0*std::cos(_fAngle) - m_fRight0*std::sin(_fAngle) + _vecV[0];

    m_fHeightMax =  m_fHeightMax0*std::sin(_fAngle) +
                    m_fHeightMax0*std::cos(_fAngle) + _vecV[1];
    m_fHeightMin =  m_fHeightMin0*std::sin(_fAngle) +
                    m_fHeightMin0*std::cos(_fAngle) + _vecV[1];

    METHOD_EXIT("CRandomLine::transform")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Draws a random line.
///
/// The main part of this method is the clipping of the line. To test if
/// clipping is working correctly, you might initialise a line with left and
/// right height set to the same value, which results in a straight line. Then,
/// visualise the second (last) point with a dot or anything else.
///
/// \note Clipping should be implemented by calculating the intersections with
///       the frame.
///
///////////////////////////////////////////////////////////////////////////////
void CRandomLine::draw() const
{
//     METHOD_ENTRY("CRandomLine::draw")
// 
//     double  fInc;
//     int     nInc;
//     int     iXMin0, iYMin0, iXMax0, iYMax0;
//     int     iXMinN, iYMinN, iXMaxN, iYMaxN;
//     int     i0, iN;
// 
//     // This part calculates the increment. First is the length for one element,
//     // set by the programmer. Second line is the real length. The last part
//     // is the resolution in meter per pixels.
//     fInc = m_Graphics.getDynPelSize() /
//             (((m_fRight-m_fLeft) / (m_nNrOV-1))
//             /
//             (m_Graphics.getResMPX()))
//             +1.0;
// 
//     // Adapt to recursive detail level
//     nInc = static_cast<int>(pow(2,floor(log(fInc)/log(2.0))));
//     if (nInc < 1) nInc=1;
// 
//     //--- First, test for clipping at left and right border (x coordinate) ---//
//     //========================================================================//
//     if (std::abs(std::sin(m_Graphics.getCamAng())) == 1.0)
//     {
//         // Vertically positioned. Sine of camera angle catches both angles
//         // For x direction, no clipping is done
//         iXMin0=0;
//         iXMax0=0;
//         iXMinN=m_nNrOV-1;
//         iXMaxN=iXMinN;
//     }
//     else
//     {
//         // Left index for min value of random line
//         iXMin0 = static_cast<int>(
//                 (m_Graphics.getCamPos()[0]+(GRAPHICS_LEFT_DEFAULT/m_Graphics.getCamZoom())
//                 / std::cos(m_Graphics.getCamAng()) - std::tan(m_Graphics.getCamAng()) *
//                 (m_fHeightMin + m_Graphics.getCamPos()[1])
//                 -m_fLeft)  /
//                 (m_fRight - m_fLeft)*(m_nNrOV-1)
//                 )/nInc*nInc;
//         // Left index for max value of random line
//         iXMax0 = static_cast<int>(
//                 (m_Graphics.getCamPos()[0]+(GRAPHICS_LEFT_DEFAULT/m_Graphics.getCamZoom())
//                 / std::cos(m_Graphics.getCamAng()) - std::tan(m_Graphics.getCamAng()) *
//                 (m_fHeightMax + m_Graphics.getCamPos()[1])
//                 -m_fLeft)  /
//                 (m_fRight - m_fLeft)*(m_nNrOV-1)
//                 )/nInc*nInc;
//         // Right index for min value of random line
//         iXMinN = static_cast<int>(
//                 (m_Graphics.getCamPos()[0]+(GRAPHICS_RIGHT_DEFAULT/m_Graphics.getCamZoom())
//                 / std::cos(m_Graphics.getCamAng()) - std::tan(m_Graphics.getCamAng()) *
//                 (m_fHeightMin + m_Graphics.getCamPos()[1])
//                 -m_fLeft) /
//                 (m_fRight - m_fLeft)*(m_nNrOV-1)
//                 )/nInc*nInc+nInc;
//         // Right index for max value of random line
//         iXMaxN = static_cast<int>(
//                 (m_Graphics.getCamPos()[0]+(GRAPHICS_RIGHT_DEFAULT/m_Graphics.getCamZoom())
//                 / std::cos(m_Graphics.getCamAng()) - std::tan(m_Graphics.getCamAng()) *
//                 (m_fHeightMax + m_Graphics.getCamPos()[1])
//                 -m_fLeft) /
//                 (m_fRight - m_fLeft)*(m_nNrOV-1)
//                 )/nInc*nInc+nInc;
//     }
// 
//     //--- Test for clipping at top and bottom of the screen (y coordinate) ---//
//     //========================================================================//
//     if (std::abs(std::cos(m_Graphics.getCamAng())) == 1.0)
//     {
//         // Horizontally positioned. Cos of camera angle catches both angles
//         iYMin0=0;
//         iYMax0=0;
//         iYMinN=m_nNrOV-1;
//         iYMaxN=iYMinN;
//     }
//     else
//     {
//         // Left index for min value of random line
//         iYMin0 = static_cast<int>(
//                 (m_Graphics.getCamPos()[0]+(GRAPHICS_BOTTOM_DEFAULT/m_Graphics.getCamZoom())
//                 / std::sin(m_Graphics.getCamAng()) +
//                 std::cos(m_Graphics.getCamAng())/std::sin(m_Graphics.getCamAng()) *
//                 (m_fHeightMin + m_Graphics.getCamPos()[1])
//                 -m_fLeft)  /
//                 (m_fRight - m_fLeft)*(m_nNrOV-1)
//                 )/nInc*nInc;
//         // Left index for max value of random line
//         iYMax0 = static_cast<int>(
//                 (m_Graphics.getCamPos()[0]+(GRAPHICS_BOTTOM_DEFAULT/m_Graphics.getCamZoom())
//                 / std::sin(m_Graphics.getCamAng()) +
//                 std::cos(m_Graphics.getCamAng())/std::sin(m_Graphics.getCamAng()) *
//                 (m_fHeightMax + m_Graphics.getCamPos()[1])
//                 -m_fLeft)  /
//                 (m_fRight - m_fLeft)*(m_nNrOV-1)
//                 )/nInc*nInc;
//         // Right index for min value of random line
//         iYMinN = static_cast<int>(
//                 (m_Graphics.getCamPos()[0]+(GRAPHICS_TOP_DEFAULT/m_Graphics.getCamZoom())
//                 / std::sin(m_Graphics.getCamAng()) +
//                 std::cos(m_Graphics.getCamAng())/std::sin(m_Graphics.getCamAng())*
//                 (m_fHeightMin + m_Graphics.getCamPos()[1])
//                 -m_fLeft) /
//                 (m_fRight - m_fLeft)*(m_nNrOV-1)
//                 )/nInc*nInc+nInc;
//         // Right index for max value of random line
//         iYMaxN = static_cast<int>(
//                 (m_Graphics.getCamPos()[0]+(GRAPHICS_TOP_DEFAULT/m_Graphics.getCamZoom())
//                 / std::sin(m_Graphics.getCamAng()) +
//                 std::cos(m_Graphics.getCamAng())/std::sin(m_Graphics.getCamAng())*
//                 (m_fHeightMax + m_Graphics.getCamPos()[1])
//                 -m_fLeft) /
//                 (m_fRight - m_fLeft)*(m_nNrOV-1)
//                 )/nInc*nInc+nInc;
//     }
// 
//     // Exchange indices for negative angles
//     // -nInc, see above (reference for fraction)
//     // +nInc, see above (reference for fraction)
//     {
//     int nTmp;
//     if (iXMax0>=iXMaxN)
//     {
//         nTmp = iXMax0;
//         iXMax0 = iXMaxN-nInc;
//         iXMaxN = nTmp+nInc;
//     }
//     if (iXMin0>=iXMinN)
//     {
//         nTmp = iXMin0;
//         iXMin0 = iXMinN-nInc;
//         iXMinN = nTmp+nInc;
//     }
//     if (iYMax0>=iYMaxN)
//     {
//         nTmp = iYMax0;
//         iYMax0 = iYMaxN-nInc;   
//         iYMaxN = nTmp+nInc;     
//     }
//     if (iYMin0>=iYMinN)
//     {
//         nTmp = iYMin0;
//         iYMin0 = iYMinN-nInc;
//         iYMinN = nTmp+nInc;
//     }
//     }
// 
//     
//     // Test the "left" index for clipping
//     if (iXMin0 > iYMin0)
//     {
//         if (iXMax0 > iYMax0)
//         {
//             // Left border, use lower value
//             if (iXMin0 < iXMax0)
//                 i0 = iXMin0;
//             else
//                 i0 = iXMax0;
//         }
//         else // iXMax0 <= iYMax0
//         {
//             // Top left edge, use greater of values outside
//             if (iXMax0 > iYMin0)
//                 i0 = iXMax0;
//             else
//                 i0 = iYMin0;
//         }
//     }
//     else
//     {
//         if (iYMax0 > iXMax0)
//         {
//             // Top border, use lower value
//             if (iYMin0 < iYMax0)
//                 i0 = iYMin0;
//             else
//                 i0 = iYMax0;    
//         }
//         else // iYMax0 <= iXMax0
//         {
//             // Bottom left edge, use greater of values outside
//             if (iXMin0 > iYMax0)
//                 i0 = iXMin0;
//             else
//                 i0 = iYMax0;
//         }
//     }
// 
//     // Test the "right" index for clipping
//     if (iXMinN < iYMinN)
//     {
//         if (iXMaxN < iYMaxN)
//         {
//             // right border, use greater value
//             if (iXMinN > iXMaxN)
//                 iN = iXMinN;
//             else
//                 iN = iXMaxN;
//         }
//         else // iXMaxN >= iYMaxN
//         {
//             // Top right edge, use lower of values outside
//             if (iYMinN < iXMaxN)
//                 iN = iYMinN;
//             else
//                 iN = iXMaxN;
//         }
//     }
//     else
//     {
//         if (iYMaxN < iXMaxN)
//         {
//             // Bottom border, use greater value
//             if (iYMinN > iYMaxN)
//                 iN = iYMinN;
//             else
//                 iN = iYMaxN;
//         }
//         else // iYMaxN >= iXMaxN
//         {
//             // Bottom right edge, use lower of values outside
//             if (iXMinN > iYMaxN)
//                 iN = iXMinN;
//             else
//                 iN = iYMaxN;
//         }
//     }
// 
//     // Avoid overflow
//     if (i0 < 0) i0=0;
//     if (iN > m_nNrOV-1) iN=m_nNrOV-1;
//     if ((i0+nInc) < m_nNrOV) m_Graphics.dot(m_paVertices[i0+nInc]);
//     if (iN >= nInc) m_Graphics.dot(m_paVertices[iN-nInc]);
// 
//     m_Graphics.beginLine(m_LineType, SHAPE_DEFAULT_DEPTH);
//     for (int i=i0; i<=iN; i+=nInc)
//     {
//         m_Graphics.addVertex(m_paVertices[i]);
//     }
//     m_Graphics.endLine();
// 
//     METHOD_EXIT("CRandomLine::draw")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Initializes the points of the line by given parameters
///
/// \param _fLeft Left x-coordinate
/// \param _fRight Right x-coordinate
/// \param _fHMin Minimum height
/// \param _fHMax Maximum height
/// \param _fSmooth Smoothing factor
/// \param _nLevel Recursive level
/// \param _nNrOCtrP Number of control points
///
///////////////////////////////////////////////////////////////////////////////
void CRandomLine::init(const double& _fLeft, const double& _fRight,
                        const double& _fHMin, const double& _fHMax,
                        const double& _fSmooth, const int& _nLevel,
                        const int& _nNrOCtrP)
{
    METHOD_ENTRY("CRandomLine::init")

//  double* paHeight = new double[static_cast<int>((_nNrOCtrP +
//              (_nNrOCtrP-1)*pow(2.0,static_cast<double>(_nLevel))))];
    
    INFO_MSG("Randomline", "Array size: " << _nNrOCtrP +
            (_nNrOCtrP-1)*pow(2.0,static_cast<double>(_nLevel)))

    std::list<Vector2d>::iterator it2;
    
    double fHLeft = 0.0;
    double fHRight = 0.0;

    m_fHeightMax0 = _fHMax;
    m_fHeightMin0 = _fHMin;
    m_fHeightMax = _fHMax;
    m_fHeightMin = _fHMin;
    
    // Make sure to clean up, if init is called multiple times
    m_VertList0.clear();
    m_VertList.clear();

    // Store left and right border
    m_fLeft0 = _fLeft;
    m_fRight0 = _fRight;
    m_fLeft = _fLeft;
    m_fRight = _fRight;

    // Test for straight line, mainly for testing purposes
    if (_fHMax == _fHMin)
    {
        fHLeft = _fHMin;
        fHRight = _fHMin;
    }
    else
    {
        // Ensure a certain rockyness
        while (std::abs(fHRight - fHLeft) < std::abs(_fHMax - _fHMin) * _fSmooth)
        {
            fHLeft = randomDouble(_fHMin, _fHMax);
            fHRight = randomDouble(_fHMin, _fHMax);
        }
    }

    // Set control point
    m_VertList0.push_back(Vector2d(_fLeft, fHLeft));
    it2 = m_VertList0.begin();
    m_VertList0.push_back(Vector2d(_fRight, fHRight));
    ++it2;

    // Subdivide recursivly
    if (_nLevel > 0)
    {
        subdivide(_fLeft, _fRight, fHLeft, fHRight, _fSmooth, _nLevel-1, it2);
    }
    
    m_VertList = m_VertList0;

    METHOD_EXIT("CRandomLine::init")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief This method is called recursivly for subdivision
///
/// \param _fLeft Left x-coordinate
/// \param _fRight Right x-coordinate
/// \param _fHLeft Height of left control point
/// \param _fHRight Height of right control point
/// \param _fSmooth Smoothing factor
/// \param _nLevel Current recursive level
///
///////////////////////////////////////////////////////////////////////////////
void CRandomLine::subdivide(const double& _fLeft, const double& _fRight,
                        const double& _fHLeft, const double& _fHRight,
                        const double& _fSmooth, const int& _nLevel,
                        std::list<Vector2d>::iterator _it)
{
    METHOD_ENTRY("CRandomLine::subdivide")
    double fH;
    std::list<Vector2d>::iterator it;
    
    fH = (_fHRight+_fHLeft)/2.0 + _fSmooth * 
        randomDouble(-std::abs(_fHRight-_fHLeft)/2.0, std::abs(_fHRight-_fHLeft)/2.0);

    m_VertList0.insert(_it, Vector2d((_fRight+_fLeft) / 2.0, fH));
    
    it = _it;
    --it;
    
    // Subdivide    
    if (_nLevel > 0)
    {
        subdivide(_fLeft, (_fRight + _fLeft) / 2.0, _fHLeft, fH,
            _fSmooth, _nLevel-1, it);
        subdivide((_fRight + _fLeft) / 2.0, _fRight, fH, _fHRight,
            _fSmooth, _nLevel-1, _it);
    }

    METHOD_EXIT("CRandomLine::subdivide")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief This method gives a random double value within given interval
///
/// \param _fMin Minimum value
/// \param _fMax Maxymum value
/// \return Random value
///
///////////////////////////////////////////////////////////////////////////////
double CRandomLine::randomDouble(const double& _fMin, const double& _fMax)
{
    METHOD_ENTRY("CRandomLine::randomDouble")

    {
        METHOD_EXIT("CRandomLine::randomDouble")

        int nResult;
        double fResult;

        nResult = (rand() % 1001);

        fResult = ((_fMax-_fMin) / 1000.0) * static_cast<double>(nResult) + _fMin;

        return (fResult);
    }
}
