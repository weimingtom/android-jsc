/*
 *  This file is part of the KDE libraries
 *  Copyright (C) 2007 Harri Porten <porten@kde.org>
 *  Copyright (C) 2006 George Staikos <staikos@kde.org>
 *  Copyright (C) 2006 Alexey Proskuryakov <ap@nypop.com>
 *  Copyright (C) 2015 Eddie Hillenbrand <eddie.hillenbrand@adcolony.com>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public License
 *  along with this library; see the file COPYING.LIB.  If not, write to
 *  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA 02110-1301, USA.
 *
 */

#ifndef KJS_UNICODE_LIBC_H
#define KJS_UNICODE_LIBC_H

#include "UnicodeMacrosFromICU.h"
#include "wtf/ASCIICType.h"
#include <assert.h>

typedef uint16_t UChar;
typedef int32_t UChar32;

namespace WTF
{
namespace Unicode
{

enum Direction {
     LeftToRight = 0,
     RightToLeft,
     EuropeanNumber,
     EuropeanNumberSeparator,
     EuropeanNumberTerminator,
     ArabicNumber,
     CommonNumberSeparator,
     BlockSeparator,
     SegmentSeparator,
     WhiteSpaceNeutral,
     OtherNeutral,
     LeftToRightEmbedding,
     LeftToRightOverride,
     RightToLeftArabic,
     RightToLeftEmbedding,
     RightToLeftOverride,
     PopDirectionalFormat,
     NonSpacingMark,
     BoundaryNeutral,
};

enum DecompositionType {
     DecompositionNone = 0,
     DecompositionCanonical,
     DecompositionCompat,
     DecompositionCircle,
     DecompositionFinal,
     DecompositionFont,
     DecompositionFraction,
     DecompositionInitial,
     DecompositionIsolated,
     DecompositionMedial,
     DecompositionNarrow,
     DecompositionNoBreak,
     DecompositionSmall,
     DecompositionSquare,
     DecompositionSub,
     DecompositionSuper,
     DecompositionVertical,
     DecompositionWide,
};

enum CharCategory {
    NoCategory = 0,
    Mark_NonSpacing = 0x00000001,          // Unicode class name Mn
    Mark_SpacingCombining = 0x00000002,    // Unicode class name Mc
    Mark_Enclosing = 0x00000004,           // Unicode class name Me
    Number_DecimalDigit = 0x00000008,      // Unicode class name Nd
    Number_Letter = 0x00000010,            // Unicode class name Nl
    Number_Other = 0x00000020,             // Unicode class name No
    Separator_Space = 0x00000040,          // Unicode class name Zs
    Separator_Line = 0x00000080,           // Unicode class name Zl
    Separator_Paragraph = 0x00000100,      // Unicode class name Zp
    Other_Control = 0x00000200,            // Unicode class name Cc
    Other_Format = 0x00000400,             // Unicode class name Cf
    Other_Surrogate = 0x00000800,          // Unicode class name Cs
    Other_PrivateUse = 0x00001000,         // Unicode class name Co
    Other_NotAssigned = 0x00002000,        // Unicode class name Cn
    Letter_Uppercase = 0x00004000,         // Unicode class name Lu
    Letter_Lowercase = 0x00008000,         // Unicode class name Ll
    Letter_Titlecase = 0x00010000,         // Unicode class name Lt
    Letter_Modifier = 0x00020000,          // Unicode class name Lm
    Letter_Other = 0x00040000,             // Unicode class name Lo
    Punctuation_Connector = 0x00080000,    // Unicode class name Pc
    Punctuation_Dash = 0x00100000,         // Unicode class name Pd
    Punctuation_Open = 0x00200000,         // Unicode class name Ps
    Punctuation_Close = 0x00400000,        // Unicode class name Pe
    Punctuation_InitialQuote = 0x00800000, // Unicode class name Pi
    Punctuation_FinalQuote = 0x01000000,   // Unicode class name Pf
    Punctuation_Other = 0x02000000,        // Unicode class name Po
    Symbol_Math = 0x04000000,              // Unicode class name Sm
    Symbol_Currency = 0x08000000,          // Unicode class name Sc
    Symbol_Modifier = 0x10000000,          // Unicode class name Sk
    Symbol_Other = 0x20000000              // Unicode class name So
};

inline int toLower(uint16_t *str, int strLength, uint16_t *&destIfNeeded)
{
    destIfNeeded = 0;

    for (int i = 0; i < strLength; ++i) {
        str[i] = toASCIILower(str[i]);
    }

    return strLength;
}

inline int toUpper(uint16_t *str, int strLength, uint16_t *&destIfNeeded)
{
    destIfNeeded = 0;

    for (int i = 0; i < strLength; ++i) {
        str[i] = toASCIIUpper(str[i]);
    }

    return strLength;
}

inline bool isSeparatorSpace(int32_t c)
{
    return (c & 0xffff0000) == 0 && isASCIISpace(static_cast<unsigned short>(c));
}

inline CharCategory category(int32_t c)
{
    if (c < 0) {
        return NoCategory;
    }
    if (c < 0x000000ff) {
        static const CharCategory cats[] = {
            Other_Control, Other_Control, Other_Control, Other_Control,
            Other_Control, Other_Control, Other_Control, Other_Control,
            Other_Control, Other_Control, Other_Control, Other_Control,
            Other_Control, Other_Control, Other_Control, Other_Control,
            Other_Control, Other_Control, Other_Control, Other_Control,
            Other_Control, Other_Control, Other_Control, Other_Control,
            Other_Control, Other_Control, Other_Control, Other_Control,
            Other_Control, Other_Control, Other_Control, Other_Control,
            Separator_Space, Punctuation_Other, Punctuation_Other,
            Punctuation_Other, Symbol_Currency, Punctuation_Other,
            Punctuation_Other, Punctuation_Other, Punctuation_Open,
            Punctuation_Close, Punctuation_Other, Symbol_Math,
            Punctuation_Other, Punctuation_Dash, Punctuation_Other,
            Punctuation_Other, Number_DecimalDigit, Number_DecimalDigit,
            Number_DecimalDigit, Number_DecimalDigit, Number_DecimalDigit,
            Number_DecimalDigit, Number_DecimalDigit, Number_DecimalDigit,
            Number_DecimalDigit, Number_DecimalDigit, Punctuation_Other,
            Punctuation_Other, Symbol_Math, Symbol_Math, Symbol_Math,
            Punctuation_Other, Punctuation_Other,
            Letter_Uppercase, Letter_Uppercase, Letter_Uppercase,
            Letter_Uppercase, Letter_Uppercase, Letter_Uppercase,
            Letter_Uppercase, Letter_Uppercase, Letter_Uppercase,
            Letter_Uppercase, Letter_Uppercase, Letter_Uppercase,
            Letter_Uppercase, Letter_Uppercase, Letter_Uppercase,
            Letter_Uppercase, Letter_Uppercase, Letter_Uppercase,
            Letter_Uppercase, Letter_Uppercase, Letter_Uppercase,
            Letter_Uppercase, Letter_Uppercase, Letter_Uppercase,
            Letter_Uppercase, Letter_Uppercase, Punctuation_Open,
            Punctuation_Other, Punctuation_Close, Symbol_Modifier,
            Punctuation_Connector, Symbol_Modifier, Letter_Lowercase,
            Letter_Lowercase, Letter_Lowercase, Letter_Lowercase,
            Letter_Lowercase, Letter_Lowercase, Letter_Lowercase,
            Letter_Lowercase, Letter_Lowercase, Letter_Lowercase,
            Letter_Lowercase, Letter_Lowercase, Letter_Lowercase,
            Letter_Lowercase, Letter_Lowercase, Letter_Lowercase,
            Letter_Lowercase, Letter_Lowercase, Letter_Lowercase,
            Letter_Lowercase, Letter_Lowercase, Letter_Lowercase,
            Letter_Lowercase, Letter_Lowercase, Letter_Lowercase,
            Letter_Lowercase, Punctuation_Open, Symbol_Math, Punctuation_Close,
            Symbol_Math, Other_Control, Other_Control, Other_Control,
            Other_Control, Other_Control, Other_Control, Other_Control,
            Other_Control, Other_Control, Other_Control, Other_Control,
            Other_Control, Other_Control, Other_Control, Other_Control,
            Other_Control, Other_Control, Other_Control, Other_Control,
            Other_Control, Other_Control, Other_Control, Other_Control,
            Other_Control, Other_Control, Other_Control, Other_Control,
            Other_Control, Other_Control, Other_Control, Other_Control,
            Other_Control, Other_Control, Separator_Space, Punctuation_Other,
            Symbol_Currency, Symbol_Currency, Symbol_Currency, Symbol_Currency,
            Symbol_Other, Symbol_Other, Symbol_Modifier, Symbol_Other,
            Letter_Lowercase, Punctuation_InitialQuote, Symbol_Math,
            Other_Format, Symbol_Other, Symbol_Modifier, Symbol_Other,
            Symbol_Math, Number_Other, Number_Other, Symbol_Modifier,
            Letter_Lowercase, Symbol_Other, Punctuation_Other,
            Symbol_Modifier, Number_Other, Letter_Lowercase,
            Punctuation_FinalQuote, Number_Other, Number_Other,
            Number_Other, Punctuation_Other, Letter_Uppercase,
            Letter_Uppercase, Letter_Uppercase, Letter_Uppercase,
            Letter_Uppercase, Letter_Uppercase, Letter_Uppercase,
            Letter_Uppercase, Letter_Uppercase, Letter_Uppercase,
            Letter_Uppercase, Letter_Uppercase, Letter_Uppercase,
            Letter_Uppercase, Letter_Uppercase, Letter_Uppercase,
            Letter_Uppercase, Letter_Uppercase, Letter_Uppercase,
            Letter_Uppercase, Letter_Uppercase, Letter_Uppercase,
            Letter_Uppercase, Symbol_Math, Letter_Uppercase,
            Letter_Uppercase, Letter_Uppercase, Letter_Uppercase,
            Letter_Uppercase, Letter_Uppercase, Letter_Uppercase,
            Letter_Lowercase, Letter_Lowercase, Letter_Lowercase,
            Letter_Lowercase, Letter_Lowercase, Letter_Lowercase,
            Letter_Lowercase, Letter_Lowercase, Letter_Lowercase,
            Letter_Lowercase, Letter_Lowercase, Letter_Lowercase,
            Letter_Lowercase, Letter_Lowercase, Letter_Lowercase,
            Letter_Lowercase, Letter_Lowercase, Letter_Lowercase,
            Letter_Lowercase, Letter_Lowercase, Letter_Lowercase,
            Letter_Lowercase, Letter_Lowercase, Letter_Lowercase,
            Symbol_Math, Letter_Lowercase, Letter_Lowercase,
            Letter_Lowercase, Letter_Lowercase, Letter_Lowercase,
            Letter_Lowercase, Letter_Lowercase, Letter_Lowercase
        };
        assert(sizeof(cats) / sizeof(CharCategory) == 0x0100);
        return cats[c];
    }

    // FIXME: implement for the rest ...
    return NoCategory;
}

inline UChar32 foldCase(UChar32 c)
{
     return c;
}

inline int foldCase(UChar* result, int resultLength, const UChar* src, int srcLength, bool* error)
{
     return 0;
}

inline int toLower(UChar* result, int resultLength, const UChar* src, int srcLength, bool* error)
{
     return 0;
}

inline UChar32 toLower(UChar32 c)
{
     return c;
}

inline UChar32 toUpper(UChar32 c)
{
     return c;
}

inline int toUpper(UChar* result, int resultLength, const UChar* src, int srcLength, bool* error)
{
     return 0;
}

inline UChar32 toTitleCase(UChar32 c)
{
     return c;
}

inline bool isArabicChar(UChar32 c)
{
     return false;
}

inline bool isAlphanumeric(UChar32 c)
{
     return false;
}

/* inline bool isSeparatorSpace(UChar32 c) */
/* { */
/*      return false; */
/* } */

inline bool isPrintableChar(UChar32 c)
{
     return false;
}

inline bool isPunct(UChar32 c)
{
     return false;
}

inline bool hasLineBreakingPropertyComplexContext(UChar32 c)
{
     return false;
}

inline bool hasLineBreakingPropertyComplexContextOrIdeographic(UChar32 c)
{
     return false;
}

inline UChar32 mirroredChar(UChar32 c)
{
     return c;
}

/* inline CharCategory category(UChar32 c) */
/* { */
/*     return static_cast<CharCategory>(U_GET_GC_MASK(c)); */
/* } */

inline Direction direction(UChar32 c)
{
     return LeftToRight;
}

inline bool isLower(UChar32 c)
{
     return false;
}

inline uint8_t combiningClass(UChar32 c)
{
     return 0;
}

inline DecompositionType decompositionType(UChar32 c)
{
     return DecompositionNone;
}

inline int umemcasecmp(const UChar* a, const UChar* b, int len)
{
     return 0;
}

}
}

#endif