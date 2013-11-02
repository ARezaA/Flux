/****************************************************************************
Copyright (c) 2010 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#ifndef __CC_COMMON_H__
#define __CC_COMMON_H__

#include "CCPlatformMacros.h"
#include <string>

NS_CC_BEGIN;

/// The max length of CCLog message.
static const int kMaxLogLen = 16*1024;

/**
@brief Output Debug message.
*/
void CC_DLL CCLog(const char * pszFormat, ...);

/**
 * lua can not deal with ...
 */
inline void CC_DLL CCLuaLog(const char * pszFormat)
{
	CCLog(pszFormat);
}

std::wstring CC_DLL CCUtf8ToUnicode(const char * pszUtf8Str, unsigned len = -1);
std::string CC_DLL CCUnicodeToUtf8(const wchar_t* pwszStr);

/**
@brief Pop out a message box
*/
void CC_DLL CCMessageBox(const char * pszMsg, const char * pszTitle);

/**
@brief Enum the language type supportted now
*/
typedef enum LanguageType
{
    kLanguageEnglish = 0,
    kLanguageChinese,
    kLanguageChinese_Simplified = kLanguageChinese,
    kLanguageChinese_Traditional,
    kLanguageJapanese,
    kLanguageFrench,
    kLanguageItalian,
    kLanguageGerman,
    kLanguageSpanish,
    kLanguageRussian
} ccLanguageType;

NS_CC_END;

#endif	// __CC_COMMON_H__
