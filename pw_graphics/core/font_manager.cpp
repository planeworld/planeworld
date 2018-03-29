////////////////////////////////////////////////////////////////////////////////
//
// This file is part of planeworld, a 2D simulation of physics and much more.
// Copyright (C) 2017 - 2018 Torsten Büschenfeld
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
/// \file       font_manager.cpp
/// \brief      Implementation of class "CFontManager"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2017-06-05
///
////////////////////////////////////////////////////////////////////////////////

#include "font_manager.h"

//--- Standard header --------------------------------------------------------//
#include <fstream>

//--- Misc-Header ------------------------------------------------------------//

#define STB_RECT_PACK_IMPLEMENTATION
#include "stb_rect_pack.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Destructor, deletes fonts
///
////////////////////////////////////////////////////////////////////////////////
CFontManager::~CFontManager()
{
    METHOD_ENTRY("CFontManager::~CFontManager")
    DTOR_CALL("CFontManager::~CFontManager")
    
    for (auto FontMem : m_FontsMemByName)
    {
        if (FontMem.second != nullptr)
        {
            delete[] FontMem.second;
            MEM_FREED("char")
            FontMem.second = nullptr;
        }    
    }
    for (auto FontMemAtlas : m_FontsMemAtlas)
    {
        if (FontMemAtlas.second != nullptr)
        {
            delete[] FontMemAtlas.second;
            MEM_FREED("std::uint8_t")
            FontMemAtlas.second = nullptr;
        }    
    }
    for (auto CharInfo : m_FontsCharInfo)
    {
        if (CharInfo.second != nullptr)
        {
            delete[] CharInfo.second;
            MEM_FREED("stbtt_packedchar")
            CharInfo.second = nullptr;
        }    
    }
    for (auto FontID : m_FontsByName)
    {
        glDeleteTextures(1, &FontID.second);
    }
    for (auto FontTimer : m_FontsIdleTime)
    {
        if (FontTimer.second != nullptr)
        {
            delete FontTimer.second;
            MEM_FREED("CTimer")
            FontTimer.second = nullptr;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Add font with given name and initialise it
///
/// \param _strFontName Name to register font with
/// \param _strFile Path and name of truetype font
/// \param _nSize Size of font when rasterized
///
/// \return Success?
///
////////////////////////////////////////////////////////////////////////////////
bool CFontManager::addFont(const std::string& _strFontName,
                           const std::string& _strFile,
                           const int _nSize)
{
    METHOD_ENTRY("CFontManager::addFont")
    
    char* pMemFont = nullptr;
   
    //--------------------------------------------------------------------------
    // Load font from given file
    //--------------------------------------------------------------------------
    std::streampos nSize;
    std::ifstream  inStream(_strFile, std::ios::in|std::ios::binary|std::ios::ate);
    if (inStream.is_open())
    {
        nSize = inStream.tellg();
        pMemFont = new char[nSize];
        MEM_ALLOC("char")
        inStream.seekg (0, std::ios::beg);
        inStream.read(pMemFont, nSize);
        inStream.close();

        m_FontsMemByName[_strFontName] = pMemFont;
        
        DOM_FIO(INFO_MSG("Font Manager", "Font " << _strFile << " successfully loaded to memory."))
    }
    else
    {
        DOM_FIO(ERROR_MSG("Font Manager", "Could not load font " << _strFile << "."))
        return false;
    }
    m_strFont = _strFontName;
    
    this->rasterize(_strFontName, _nSize);
    
    return true;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Draws given text, i.e. buffering textured quads for GL to be called
///        by graphics update, based on last position.
///
/// \param _strText     Text to be drawn
/// \param _bCentered   Text centered? (Otherwise align left [default])
/// \param _nWrap       Word wrap position
///
////////////////////////////////////////////////////////////////////////////////
void CFontManager::drawText(const std::string& _strText,
                            const bool _bCentered,
                            const int _nWrap
                           )
{
    METHOD_ENTRY("CFontManager::drawText")
    this->drawText(_strText, m_fLastPosX, m_fLastPosY, _bCentered, _nWrap);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Draws given text, i.e. buffering textured quads for GL to be called
///        by graphics update.
///
/// \param _strText     Text to be drawn
/// \param _fPosX       Position X
/// \param _fPosY       Position Y
/// \param _bCentered   Text centered? (Otherwise align left [default])
/// \param _nWrap       Word wrap position
///
////////////////////////////////////////////////////////////////////////////////
void CFontManager::drawText(const std::string& _strText,
                            const float& _fPosX,
                            const float& _fPosY,
                            const bool _bCentered,
                            const int _nWrap
                           )
{
    METHOD_ENTRY("CFontManager::drawText")
    
    if (m_bChanged) this->changeFont();
    
    stbtt_aligned_quad GlyphQuad;
    float fOffsetX = 0.0f;
    float fOffsetY = 0.0f;
    std::vector<GLfloat> vecUVs; vecUVs.resize(8);
    
    float fSize = 0.0f;
    if (_bCentered)
    {
        const stbtt_packedchar* b;
        for (const auto Ch : _strText)
        {
            b = m_pFontCharInfo + Ch-ASCII_FIRST;
            fSize += b->xadvance;
        }
    }
    for (const auto Ch : _strText)
    {
        if (Ch == 10) // Line feed
        {
            fOffsetX = 0.0f;
            fOffsetY += m_nSize;
        }
        else
        {
            stbtt_GetPackedQuad(m_pFontCharInfo,
                                m_nAtlasSize, m_nAtlasSize,
                                Ch - ASCII_FIRST,
                                &fOffsetX, &fOffsetY, &GlyphQuad, 1);
            if (fOffsetX > _nWrap && _nWrap > 0) // Word wrap
            {
                fOffsetX = 0.0f;
                fOffsetY += m_nSize;
                stbtt_GetPackedQuad(m_pFontCharInfo,
                                    m_nAtlasSize, m_nAtlasSize,
                                    Ch - ASCII_FIRST,
                                    &fOffsetX, &fOffsetY, &GlyphQuad, 1);
            }
            
            vecUVs = {GlyphQuad.s0, GlyphQuad.t0, 
                      GlyphQuad.s1, GlyphQuad.t0,
                      GlyphQuad.s0, GlyphQuad.t1,
                      GlyphQuad.s1, GlyphQuad.t1};
                    
            m_Graphics.texturedRect(Vector2d(GlyphQuad.x0, GlyphQuad.y0)+Vector2d(_fPosX-fSize/2, _fPosY),
                                    Vector2d(GlyphQuad.x1, GlyphQuad.y1)+Vector2d(_fPosX-fSize/2, _fPosY),
                                    &vecUVs);
        }
    }
    
    m_fLastPosX = fOffsetX + _fPosX;
    m_fLastPosY = fOffsetY + _fPosY;
    
    m_FontsIdleTime[m_unTexID]->restart();
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the length (px) of a given text for given font and size
///
/// \param _strText Text to be measured in length
/// \param _strFont Font of text to be measured
/// \param _nSize   Size of text to be measured
///
/// \return Measured length of given Text \ref _strText.
///
////////////////////////////////////////////////////////////////////////////////
float CFontManager::getTextLength(const std::string& _strText, 
                                 const std::string& _strFont,
                                 const int _nSize)
{
    METHOD_ENTRY("CFontManager::getTextLength")
    
    const auto ci = m_FontsMemByName.find(_strFont);
    if (ci != m_FontsMemByName.end())
    {
        std::string strFontDesignator = std::to_string(_nSize)+_strFont;
        
        auto nID = 0u;

        auto it = m_FontsByName.find(strFontDesignator);
        if (it == m_FontsByName.end())
        {
            this->rasterize(_strFont, _nSize);
            it = m_FontsByName.find(strFontDesignator);
        }
        
        nID = it->second;
        
        float fLength = 0.0f;
        float fLengthMax = 0.0f;
        const stbtt_packedchar* b;
        for (const auto Ch : _strText)
        {
            if (Ch == 10) // Line feed
            {
                if (fLength > fLengthMax)
                {
                    fLengthMax = fLength;
                }
                fLength = 0.0;
            }
            b = m_FontsCharInfo[nID] + Ch-ASCII_FIRST;
            fLength += b->xadvance;
        }
        if (fLength > fLengthMax)
        {
            fLengthMax = fLength;
        }
        
        m_FontsIdleTime[nID]->restart();
        
        return fLengthMax;
    }
    else
    {
        WARNING_MSG("Font Manager", "Font <" << m_strFont << "> unknown.")
        return 0.0f;
    }
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Set the given font for usage
///
/// \param _strFontName Name of font to use
///
/// \return Success?
///
////////////////////////////////////////////////////////////////////////////////
bool CFontManager::setFont(const std::string& _strFontName)
{
    METHOD_ENTRY("CFontManager::setFont")
    if (_strFontName != m_strFont)
    {
        m_strFont   = _strFontName;
        m_bChanged  = true;
    }
    return true;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Set the given font size for drawing
///
/// \param _nSize Font size to use for drawing
///
////////////////////////////////////////////////////////////////////////////////
void CFontManager::setSize(const int _nSize)
{
    METHOD_ENTRY("CFontManager::setSize")
    if (_nSize != m_nSize)
    {
        m_nSize     = _nSize;
        m_bChanged  = true;
    }
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Triggers maintenance which removes unused fonts from memory
///
////////////////////////////////////////////////////////////////////////////////
void CFontManager::triggerMaintenance()
{
    METHOD_ENTRY("CFontManager::triggerMaintenance")
    
    if (m_FontsByName.size() > FONT_MGR_MAX_FONTS_BEFORE_REMOVAL)
    {
        std::vector<GLuint> IDs;
        for (auto FontTimer : m_FontsIdleTime)
        {
            if (FontTimer.second->getSplitTime() > FONT_MGR_MAX_IDLE_TIME_DEFAULT)
            {
                DEBUG_MSG("Font Manager", "Removing font with ID " << FontTimer.first)
                this->removeFont(FontTimer.first);
                IDs.push_back(FontTimer.first);
            }
        }
        for (auto ID : IDs)
        {
            m_FontsIdleTime.erase(ID);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Changes currently active font, including font size
///
////////////////////////////////////////////////////////////////////////////////
void CFontManager::changeFont()
{
    METHOD_ENTRY("CFontManager::changeFont")
    
    const auto ci = m_FontsMemByName.find(m_strFont);
    if (ci != m_FontsMemByName.end())
    {
        std::string strFontDesignator = std::to_string(m_nSize)+m_strFont;
        
        const auto ci = m_FontsByName.find(strFontDesignator);
        if (ci != m_FontsByName.end())
        {
            m_unTexID = ci->second;
        }
        else
        {
            this->rasterize(m_strFont, m_nSize);
            m_unTexID = m_FontsByName[strFontDesignator];
        }

        DOM_DEV(
            if (m_pRenderMode == nullptr)
            {
                WARNING_MSG("Font Manager", "Render mode not set.")
                goto DomDevRenderMode;
            }
        )
        
        // End current render batch since it is bound to the font texture
        m_Graphics.restartRenderBatch(m_pRenderMode);
        m_pRenderMode->setTexture0(m_unTexID);
        
        DOM_DEV(DomDevRenderMode:)
        
        m_pFontCharInfo = m_FontsCharInfo[m_unTexID];
        m_nAtlasSize    = m_AtlasSizes[m_unTexID];
    
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_unTexID);
        
        m_FontsIdleTime[m_unTexID]->restart();
    }
    else
    {
        WARNING_MSG("Font Manager", "Font <" << m_strFont << "> unknown.")
    }
    
    
    
    m_bChanged = false;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Rasterizes font
///
/// \param _strFontName Name of font to rasterize
/// \param _nSize Size for rasterized font
///
////////////////////////////////////////////////////////////////////////////////
void CFontManager::rasterize(const std::string& _strFontName, const int _nSize)
{
    METHOD_ENTRY("CFontManager::rasterize")
    
    DEBUG_MSG("Font manager", "Rasterising font " << _strFontName << ", Size: " << _nSize)
    
    std::string strFontDesignator = std::to_string(_nSize)+_strFontName;
    GLuint unIDTex = 0u;
    if (m_FontsByName.find(strFontDesignator) == m_FontsByName.end())
    {
        glGenTextures(1, &unIDTex);
        m_FontsByName[strFontDesignator] = unIDTex;
    }
    else
    {
        WARNING_MSG("Font Manager", "Font with name " << _strFontName << " already existing.")
        unIDTex = m_FontsByName[strFontDesignator];
    }
    
    DOM_VAR(DEBUG_BLK(
        std::cout << "  Font memory: " << std::endl;
        for (const auto Font : m_FontsByName)
        {
            std::cout << "  - " << Font.first << std::endl;
        }
    ))
    
    //--------------------------------------------------------------------------
    // Use stb_truetype to rasterise font
    //--------------------------------------------------------------------------
    m_pFontCharInfo = new stbtt_packedchar[ASCII_NR];
    m_FontsCharInfo[unIDTex] = m_pFontCharInfo;
    MEM_ALLOC("stbtt_packedchar")
    
    bool bPacked = false;
    int  nAtlasScale = 1;
    
    // Pack atlas. Retry with bigger texture if neccessary
    while (!bPacked && nAtlasScale <= 8)
    {
        auto it = m_FontsMemAtlas.find(unIDTex);
        if (it != m_FontsMemAtlas.end())
        {
            if (m_FontsMemAtlas[unIDTex] != nullptr)
            {
                delete m_FontsMemAtlas[unIDTex];
                MEM_FREED("std::uint8_t")
                m_FontsMemAtlas[unIDTex] = nullptr;
            }
        }
        m_FontsMemAtlas[unIDTex] = new std::uint8_t[FONT_MGR_ATLAS_SIZE_DEFAULT*
                                                    FONT_MGR_ATLAS_SIZE_DEFAULT*
                                                    nAtlasScale*nAtlasScale];
        MEM_ALLOC("std::uint8_t")
        
        bPacked = true;
        stbtt_pack_context Context;
        if (!stbtt_PackBegin(&Context, m_FontsMemAtlas[unIDTex],
                             FONT_MGR_ATLAS_SIZE_DEFAULT*nAtlasScale,
                             FONT_MGR_ATLAS_SIZE_DEFAULT*nAtlasScale, 0, 1, nullptr))
        {
            WARNING_MSG("Font Manager", "Could not initialise font.")
            bPacked = true;
        }
        
        stbtt_PackSetOversampling(&Context, 1, 1);
        if (!stbtt_PackFontRange(&Context, reinterpret_cast<unsigned char*>(m_FontsMemByName[_strFontName]), 0,
                                FONT_MGR_SCALE * float(_nSize),
                                ASCII_FIRST, ASCII_NR, m_pFontCharInfo))
        {
            DEBUG_MSG("Font Manager", "Could not pack font, trying larger texture size.")
            nAtlasScale *= 2;
            bPacked = false;
        }
        stbtt_PackEnd(&Context);
    }
    if (nAtlasScale > 8 && !bPacked)
    {
        WARNING_MSG("Font Manager", "Could not pack font, try to reduce font size.")
        
//         if (m_FontsCharInfo[unIDTex] != nullptr)
//         {
//             delete m_FontsCharInfo[unIDTex];
//             MEM_FREED("stbtt_packedchar")
//             m_FontsCharInfo[unIDTex] = nullptr;
//         }
//         m_FontsCharInfo.erase(unIDTex);
//         
//         if (m_FontsMemAtlas[unIDTex] != nullptr)
//         {
//             delete m_FontsMemAtlas[unIDTex];
//             MEM_FREED("std::uint8_t")
//             m_FontsMemAtlas[unIDTex] = nullptr;
//         }
//         m_FontsMemAtlas.erase(unIDTex);
//         m_FontsByName.erase(strFontDesignator);
//         
//         m_pFontCharInfo = pFontCharInfo;
    }
    else
    {
        DEBUG_BLK(
            for (auto i=0u; i < _nSize*3; ++i)
            {
                for (auto j=0u; j < 60u; ++j)
                {
                    if (m_FontsMemAtlas[unIDTex][i*FONT_MGR_ATLAS_SIZE_DEFAULT*nAtlasScale+j] > 200)
                        std::cout << "# ";
                    else if (m_FontsMemAtlas[unIDTex][i*FONT_MGR_ATLAS_SIZE_DEFAULT*nAtlasScale+j] > 100)
                        std::cout << "* ";
                    else if (m_FontsMemAtlas[unIDTex][i*FONT_MGR_ATLAS_SIZE_DEFAULT*nAtlasScale+j] > 50)
                        std::cout << ". ";
                    else
                        std::cout << "  ";
                }
                std::cout << std::endl;
            }
        )
        
        m_AtlasSizes[unIDTex] = FONT_MGR_ATLAS_SIZE_DEFAULT * nAtlasScale;
        
        //--------------------------------------------------------------------------
        // Set GL parameters for font atlas texture
        //--------------------------------------------------------------------------
        glBindTexture(GL_TEXTURE_2D, unIDTex);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
//         glTexStorage2D(GL_TEXTURE_2D, 4, GL_RGBA8,
//                        FONT_MGR_ATLAS_SIZE_DEFAULT*nAtlasScale,
//                        FONT_MGR_ATLAS_SIZE_DEFAULT*nAtlasScale);
//         glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0,
//                         FONT_MGR_ATLAS_SIZE_DEFAULT*nAtlasScale,
//                         FONT_MGR_ATLAS_SIZE_DEFAULT*nAtlasScale,
//                         GL_RED, GL_UNSIGNED_BYTE, m_FontsMemAtlas[unIDTex]);
        
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, FONT_MGR_ATLAS_SIZE_DEFAULT*nAtlasScale, FONT_MGR_ATLAS_SIZE_DEFAULT*nAtlasScale, 0,
        GL_RED, GL_UNSIGNED_BYTE, m_FontsMemAtlas[unIDTex]);
        
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        
        m_FontsIdleTime[unIDTex] = new CTimer;
        MEM_ALLOC("CTimer")
        m_FontsIdleTime[unIDTex]->start();
    }
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Removes font from Font Manager
///
/// All resources will be released. Notice, that by font a font with a
/// particular size is meant. Font metrics themselves will stay in memory
/// until removed manually or at destruction.
///
/// \param _unTexID Texture ID, identifying the font
///
////////////////////////////////////////////////////////////////////////////////
bool CFontManager::removeFont(const GLuint _unTexID)
{
    METHOD_ENTRY("CFontManager::removeFont")
    
    bool bSuccess = true;
    auto it1 = m_FontsMemAtlas.find(_unTexID);
    if (it1 != m_FontsMemAtlas.end())
    {
        m_FontsMemAtlas.erase(it1);
    }
    else bSuccess = false;
    
    auto it2 = m_AtlasSizes.find(_unTexID);
    if (it2 != m_AtlasSizes.end())
    {
        m_AtlasSizes.erase(it2);
    }
    else bSuccess = false;
    
    auto it3 = m_FontsCharInfo.find(_unTexID);
    if (it3 != m_FontsCharInfo.end())
    {
        m_FontsCharInfo.erase(it3);
    }
    else bSuccess = false;
    
    for (auto it = m_FontsByName.begin(); it != m_FontsByName.end(); ++it)
    {
        if (it->second == _unTexID)
        {
            m_FontsByName.erase(it);
            break;
        }
        else bSuccess = false;
    }
    glDeleteTextures(1, &_unTexID);

    if (!bSuccess)
    {
        WARNING_MSG("Font Manager", "Error removing font, this shouldn't happen.")
    }
    return bSuccess;
}

