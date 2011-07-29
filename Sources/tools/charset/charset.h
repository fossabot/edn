/**
 *******************************************************************************
 * @file charset.h
 * @brief Editeur De N'ours : Abstraction Charset layer (header)
 * @author Edouard DUPIN
 * @date 06/06/2011
 * @par Project
 * Edn
 *
 * @par Copyright
 * Copyright 2010 Edouard DUPIN, all right reserved
 *
 * This software is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY.
 *
 * Licence summary : 
 *    You can modify and redistribute the sources code and binaries.
 *    You can send me the bug-fix
 *    You can not earn money with this Software (if the source extract from Edn
 *        represent less than 50% of original Sources)
 * Term of the licence in in the file licence.txt.
 *
 *******************************************************************************
 */
 
#ifndef __CHARSET_H__
#define __CHARSET_H__

#include "VectorType.h"

typedef enum {
	EDN_CHARSET_UTF8,
	EDN_CHARSET_ISO_8859_1,
	EDN_CHARSET_ISO_8859_2,
	EDN_CHARSET_ISO_8859_3,
	EDN_CHARSET_ISO_8859_4,
	EDN_CHARSET_ISO_8859_5,
	EDN_CHARSET_ISO_8859_6,
	EDN_CHARSET_ISO_8859_7,
	EDN_CHARSET_ISO_8859_8,
	EDN_CHARSET_ISO_8859_9,
	EDN_CHARSET_ISO_8859_10,
	EDN_CHARSET_ISO_8859_11,
	EDN_CHARSET_ISO_8859_13,
	EDN_CHARSET_ISO_8859_14,
	EDN_CHARSET_ISO_8859_15,
} charset_te;

// transform ISO <==> Unicode
void    convertIsoToUnicode(charset_te inputCharset, char                   input_ISO,     int32_t &              output_Unicode);
void    convertUnicodeToIso(charset_te inputCharset, int32_t                input_Unicode, char &                 output_ISO);
int32_t convertIsoToUnicode(charset_te inputCharset, Edn::VectorType<char>&    input_ISO,     Edn::VectorType<int32_t>& output_Unicode);
int32_t convertUnicodeToIso(charset_te inputCharset, Edn::VectorType<int32_t>& input_Unicode, Edn::VectorType<char>&    output_ISO);
// Transform UTF-8 <==> Unicode
void    convertUnicodeToUtf8(                        int32_t                input_Unicode, char *                 output_UTF8);
void    convertUtf8ToUnicode(                        char *                 input_UTF8,    int32_t&               output_Unicode);
int32_t convertUnicodeToUtf8(                        Edn::VectorType<int32_t>& input_Unicode, Edn::VectorType<char>&    output_UTF8);
int32_t convertUtf8ToUnicode(                        Edn::VectorType<char>&    input_UTF8,    Edn::VectorType<int32_t>& output_Unicode);
// Transform ISO <==> UTF-8
void    convertIsoToUtf8(  charset_te inputCharset,  char                   input_ISO,     char *                 output_UTF8);
void    convertUtf8ToIso(  charset_te inputCharset,  char *                 input_UTF8,    char &                 output_ISO);
int32_t convertIsoToUtf8(  charset_te inputCharset,  Edn::VectorType<char>&    input_ISO,     Edn::VectorType<char>&    output_UTF8);
int32_t convertUtf8ToIso(  charset_te inputCharset,  Edn::VectorType<char>&    input_UTF8,    Edn::VectorType<char>&    output_ISO);

void    Utf8_SizeElement(const char * data, int32_t lenMax , uint8_t &size, bool &baseValid);
int32_t strUtf8Len(const char *input_UTF8);


#ifdef USE_GTK_VERSION_3_0
#	define GDK_KEY_Esc			(65305)
#	define GDK_KEY_Alt_Gr		(65027)
#	define GDK_KEY_Pomme_L		(65515)
#	define GDK_KEY_Pomme_R		(65516)
#elif USE_GTK_VERSION_2_0
#	define GDK_Esc			(65305)
#	define GDK_Alt_Gr		(65027)
#	define GDK_Pomme_L		(65515)
#	define GDK_Pomme_R		(65516)
#endif
//#	define GDK_Retur		(65516)

void ConvertInput(GdkEventKey *event, char* Utf8Out, bool &controlKey, bool &moveKey, int32_t &key);

void testDisplayConvChar(void);

#endif

