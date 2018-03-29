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
/// \file       font_manager.h
/// \brief      Prototype of class "CFontManager"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2017-06-05
///
////////////////////////////////////////////////////////////////////////////////

#ifndef FONT_MANAGER_H
#define FONT_MANAGER_H

#define GL_GLEXT_PROTOTYPES

//--- Standard header --------------------------------------------------------//

//--- Program header ---------------------------------------------------------//
#include "graphics.h"

//--- Misc header ------------------------------------------------------------//
#include "GL/gl.h"
#include "GL/glext.h"

#include "stb_rect_pack.h"
#include "stb_truetype.h"

//-- Constants ---------------------------------------------------------------//
constexpr int ASCII_FIRST = 32;
constexpr int ASCII_NR = 95;
constexpr int FONT_MGR_ATLAS_SIZE_DEFAULT = 512;
constexpr int FONT_MGR_SIZE_DEFAULT = 16;
constexpr int FONT_MGR_MAX_FONTS_BEFORE_REMOVAL = 10;
constexpr float FONT_MGR_SCALE = 1.33f;
constexpr double FONT_MGR_MAX_IDLE_TIME_DEFAULT = 180.0;

constexpr int FONT_MGR_NO_WORD_WRAP = -1;
const std::string FONT_MGR_FONT_DEFAULT = "anka_c87_r";

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class for loading, rasterizing and rendering fonts
///
////////////////////////////////////////////////////////////////////////////////
class CFontManager : public CGraphicsBase
{

    public:

        //--- Constructor/Destructor -----------------------------------------//
        CFontManager() :    m_pFontCharInfo(nullptr),
                            m_strFont(""),
                            m_strRenderModeName("font"),
                            m_nAtlasSize(FONT_MGR_ATLAS_SIZE_DEFAULT),
                            m_nSize(FONT_MGR_SIZE_DEFAULT),
                            m_unTexID(0u),
                            m_bChanged(false),
                            m_fLastPosX(0.0),
                            m_fLastPosY(0.0){}
        ~CFontManager();

        //--- Constant Methods -----------------------------------------------//
        const std::unordered_map<std::string, GLuint>* getFontsAvailable() const;
        const std::unordered_map<GLuint, CTimer*>* getFontsIdleTime() const
        {
            return &m_FontsIdleTime;
        }
        
        //--- Methods --------------------------------------------------------//
        bool    addFont(const std::string&, const std::string&, const int = FONT_MGR_SIZE_DEFAULT);
        void    drawText(const std::string&, const bool = false, const int = FONT_MGR_NO_WORD_WRAP);
        void    drawText(const std::string&, const float&, const float&, const bool = false, const int = FONT_MGR_NO_WORD_WRAP);
        GLuint  getIDTex(const std::string&);
        float   getTextLength(const std::string&, const std::string&, const int);
        bool    setFont(const std::string&);
        void    setRenderModeName(const std::string& _strName) {m_strRenderModeName = _strName;}
        void    setSize(const int);
        void    triggerMaintenance();
                
        //--- friends --------------------------------------------------------//

    private:
        
        //--- Methods [private] ----------------------------------------------//
        void    changeFont();
        void    rasterize(const std::string&, const int);
        bool    removeFont(const GLuint);
        
        //--- Variables [private] --------------------------------------------//
        std::unordered_map<std::string, GLuint>         m_FontsByName;      ///< Fonts GL IDs accessed by name
        std::unordered_map<GLuint, CTimer*>             m_FontsIdleTime;    ///< Time that a font hasn't been used
        std::unordered_map<std::string, char*>          m_FontsMemByName;   ///< Fonts stored in memory after loading
        std::unordered_map<GLuint, std::uint8_t*>       m_FontsMemAtlas;    ///< Memory of font atlas texture
        std::unordered_map<GLuint, int>                 m_AtlasSizes;       ///< Size of font atlases
        std::unordered_map<GLuint, stbtt_packedchar*>   m_FontsCharInfo;    ///< Font information like kerning
        stbtt_packedchar*                               m_pFontCharInfo;    ///< Char info of current font
        std::string                                     m_strFont;          ///< Current font
        std::string                                     m_strRenderModeName;///< Name of registered render mode for fonts
        int                                             m_nAtlasSize;       ///< Current Atlas size
        int                                             m_nSize;            ///< Current font size
        GLuint                                          m_unTexID;          ///< Current texture
        bool                                            m_bChanged;         ///< Indicates, if current font was changed
        float                                           m_fLastPosX;        ///< Last position, x coordinate
        float                                           m_fLastPosY;        ///< Last position, x coordinate
};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Return map of all currently available fonts, accessed by name
///
/// \return Map of all currently available fonts, accessed by name
///
////////////////////////////////////////////////////////////////////////////////
inline const std::unordered_map<std::string, GLuint>* CFontManager::getFontsAvailable() const
{
    METHOD_ENTRY("CFontManager::getFontsAvailable")
    return &m_FontsByName;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Return id of texture
///
/// \param _strFont Font to get texture id from
///
/// \return ID of texture of given font
///
////////////////////////////////////////////////////////////////////////////////
inline GLuint CFontManager::getIDTex(const std::string& _strFont)
{
    METHOD_ENTRY("CFontManager::getIDTex")
    
    DOM_DEV
    (
        const auto ci = m_FontsByName.find(_strFont);
        if (ci == m_FontsByName.end())
        {
            ERROR_MSG("Font Manager", "Unknown font with name " << _strFont << ".")
            return 0;
        }
    )
    return m_FontsByName[_strFont];
}

#endif // FONT_MANAGER_H
