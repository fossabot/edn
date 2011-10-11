/**
 *******************************************************************************
 * @file charset.cpp
 * @brief Editeur De N'ours : Abstraction Charset layer (Sources)
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

// cf : http://unicode.org/fr/charts/symbols.html#CombiningDiacriticalMarks

#include "tools_debug.h"
#include "tools_globals.h"
#include "charsetTable.h"
#include "charset.h"

//    /usr/include/gtk-3.0/gdk/gdkkeysyms.h


// transform ISO <==> Unicode
void convertIsoToUnicode(charset_te inputCharset, char input_ISO, int32_t & output_Unicode)
{
	switch(inputCharset)
	{
		case EDN_CHARSET_ISO_8859_1:		output_Unicode = TableIso8859_1[(uint32_t)input_ISO&0xFF];			break;
		case EDN_CHARSET_ISO_8859_2:		output_Unicode = TableIso8859_2[(uint32_t)input_ISO&0xFF];			break;
		case EDN_CHARSET_ISO_8859_3:		output_Unicode = TableIso8859_3[(uint32_t)input_ISO&0xFF];			break;
		case EDN_CHARSET_ISO_8859_4:		output_Unicode = TableIso8859_4[(uint32_t)input_ISO&0xFF];			break;
		case EDN_CHARSET_ISO_8859_5:		output_Unicode = TableIso8859_5[(uint32_t)input_ISO&0xFF];			break;
		case EDN_CHARSET_ISO_8859_6:		output_Unicode = TableIso8859_6[(uint32_t)input_ISO&0xFF];			break;
		case EDN_CHARSET_ISO_8859_7:		output_Unicode = TableIso8859_7[(uint32_t)input_ISO&0xFF];			break;
		case EDN_CHARSET_ISO_8859_8:		output_Unicode = TableIso8859_8[(uint32_t)input_ISO&0xFF];			break;
		case EDN_CHARSET_ISO_8859_9:		output_Unicode = TableIso8859_9[(uint32_t)input_ISO&0xFF];			break;
		case EDN_CHARSET_ISO_8859_10:		output_Unicode = TableIso8859_10[(uint32_t)input_ISO&0xFF];			break;
		case EDN_CHARSET_ISO_8859_11:		output_Unicode = TableIso8859_11[(uint32_t)input_ISO&0xFF];			break;
		case EDN_CHARSET_ISO_8859_13:		output_Unicode = TableIso8859_13[(uint32_t)input_ISO&0xFF];			break;
		case EDN_CHARSET_ISO_8859_14:		output_Unicode = TableIso8859_14[(uint32_t)input_ISO&0xFF];			break;
		case EDN_CHARSET_ISO_8859_15:		
			output_Unicode = TableIso8859_15[(uint32_t)input_ISO&0xFF];
			#if 0
			if ((uint32_t)input_ISO&0xFF == 0xb0) {
				printf("Change Element : 0x%02x ==> 0x%08x \n", (uint32_t)input_ISO&0xFF, output_Unicode);
			}
			#endif
			break;
		default :
			EDN_WARNING("Unknow charset ... " <<  inputCharset);
			output_Unicode = '?';
			break;
	}
}


void convertUnicodeToIso(charset_te inputCharset, int32_t input_Unicode, char & output_ISO)
{
	const int32_t *tmpTable = NULL;
	switch(inputCharset)
	{
		case EDN_CHARSET_ISO_8859_1:		tmpTable = TableIso8859_1;		break;
		case EDN_CHARSET_ISO_8859_2:		tmpTable = TableIso8859_2;		break;
		case EDN_CHARSET_ISO_8859_3:		tmpTable = TableIso8859_3;		break;
		case EDN_CHARSET_ISO_8859_4:		tmpTable = TableIso8859_4;		break;
		case EDN_CHARSET_ISO_8859_5:		tmpTable = TableIso8859_5;		break;
		case EDN_CHARSET_ISO_8859_6:		tmpTable = TableIso8859_6;		break;
		case EDN_CHARSET_ISO_8859_7:		tmpTable = TableIso8859_7;		break;
		case EDN_CHARSET_ISO_8859_8:		tmpTable = TableIso8859_8;		break;
		case EDN_CHARSET_ISO_8859_9:		tmpTable = TableIso8859_9;		break;
		case EDN_CHARSET_ISO_8859_10:		tmpTable = TableIso8859_10;		break;
		case EDN_CHARSET_ISO_8859_11:		tmpTable = TableIso8859_11;		break;
		case EDN_CHARSET_ISO_8859_13:		tmpTable = TableIso8859_13;		break;
		case EDN_CHARSET_ISO_8859_14:		tmpTable = TableIso8859_14;		break;
		case EDN_CHARSET_ISO_8859_15:		tmpTable = TableIso8859_15;		break;
		default :
			EDN_WARNING("Unknow charset ... " <<  inputCharset);
			output_ISO = '?';
			return;
	}
	int32_t i;
	for (i=0; i<256; i++) {
		if (tmpTable[i] == input_Unicode) {
			output_ISO = (char)i;
			return;
		}
	}
}


int32_t convertIsoToUnicode(charset_te inputCharset, Edn::VectorType<char>& input_ISO, Edn::VectorType<int32_t>& output_Unicode)
{
	EDN_WARNING("TODO : not coded...");
	return 0;
}


int32_t convertUnicodeToIso(charset_te inputCharset, Edn::VectorType<int32_t>& input_Unicode, Edn::VectorType<char>&    output_ISO)
{
	EDN_WARNING("TODO : not coded...");
	return 0;
}


static uint32_t unicodeToUtf8(uint32_t value)
{
	uint32_t output = 0;
	if (127 >= value) {
		output = value;
	} else if (2047 >= value) {
		// output ==> 00000000 00000000 110xxxxx 10xxxxxx
		// input ==>  -------- -------- -----222 22111111
		output = 0x0000C080;
		output+= (value & 0x000007C0)<<2;
		output+=  value & 0x0000003F;
	} else if (65535 >= value) {
		// output ==> 00000000 1110xxxx 10xxxxxx 10xxxxxx
		// input ==>  -------- -------- 33332222 22111111
		output = 0x00E08080;
		output+= (value & 0x0000F000)<<4;
		output+= (value & 0x00000FC0)<<2;
		output+=  value & 0x0000003F;
	} else if (1114111 >= value) {
		// output ==> 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
		// input ==>  -------- ---44433 33332222 22111111
		output = 0xF0808080;
		output+= (value & 0x001C0000)<<6;
		output+= (value & 0x0003F000)<<4;
		output+= (value & 0x00000FC0)<<2;
		output+=  value & 0x0000003F;
	} else {
		EDN_ERROR("NON UTF8 caracter input...");
		return 0;
	}
	//printf("utf8convertion : %d=%08x ==> %08x\n",value, value, output);
	return output;
}


// Transform UTF-8 <==> Unicode
void convertUnicodeToUtf8(int32_t input_Unicode, char * output_UTF8)
{
	uint32_t value = unicodeToUtf8(input_Unicode);
	if (0xFF >= value) {
		output_UTF8[0] = (char)value;
		output_UTF8[1] = '\0';
	} else if (0xFFFF >= value) {
		output_UTF8[0] = (char)((value>>8)  & 0x000000FF);
		output_UTF8[1] = (char)value;
		output_UTF8[2] = '\0';
	} else if (0xFFFFFF >= value) {
		output_UTF8[0] = (char)((value>>16) & 0x000000FF);
		output_UTF8[1] = (char)((value>>8)  & 0x000000FF);
		output_UTF8[2] = (char)value;
		output_UTF8[3] = '\0';
	} else {
		output_UTF8[0] = (char)((value>>24) & 0x000000FF);
		output_UTF8[1] = (char)((value>>16) & 0x000000FF);
		output_UTF8[2] = (char)((value>>8)  & 0x000000FF);
		output_UTF8[3] = (char)value;
		output_UTF8[4] = '\0';
	}
}



void convertUtf8ToUnicode(char * input_UTF8, int32_t &output_Unicode)
{
	int32_t len = strlen(input_UTF8);
	output_Unicode = 0;
	switch (len) {
		case 1:
			output_Unicode = (uint8_t)(input_UTF8[0]) & 0x7F;
			break;
		case 2:
			output_Unicode  = (((uint8_t)input_UTF8[0]) & 0x1F)<< 6;
			output_Unicode +=  ((uint8_t)input_UTF8[1]) & 0x3F;
			break;
		case 3:
			output_Unicode  = (((uint8_t)input_UTF8[0]) & 0x0F)<< 12;
			output_Unicode += (((uint8_t)input_UTF8[1]) & 0x3F)<< 6;
			output_Unicode +=  ((uint8_t)input_UTF8[2]) & 0x3F;
			break;
		default:
			output_Unicode  = (((uint8_t)input_UTF8[0]) & 0x07)<< 18;
			output_Unicode += (((uint8_t)input_UTF8[1]) & 0x3F)<< 12;
			output_Unicode += (((uint8_t)input_UTF8[2]) & 0x3F)<< 6;
			output_Unicode +=  ((uint8_t)input_UTF8[3]) & 0x3F;
			break;
	}
}


int32_t convertUnicodeToUtf8(Edn::VectorType<int32_t>& input_Unicode, Edn::VectorType<char>& output_UTF8)
{
	EDN_WARNING("TODO : not coded...");
	return 0;
}


int32_t convertUtf8ToUnicode(Edn::VectorType<char>& input_UTF8, Edn::VectorType<int32_t>& output_Unicode)
{
	EDN_WARNING("TODO : not coded...");
	return 0;
}


// Transform ISO <==> UTF-8
void convertIsoToUtf8(charset_te inputCharset,  char input_ISO, char * output_UTF8)
{
	int32_t tmpUnicode;
	// concert Iso in UniCode
	convertIsoToUnicode(inputCharset, input_ISO, tmpUnicode );
	// convert UniCode in Utf-8
	convertUnicodeToUtf8(tmpUnicode, output_UTF8);
}


void convertUtf8ToIso(charset_te inputCharset,  char * input_UTF8, char & output_ISO)
{
	int32_t tmpUnicode;
	// convert Utf-8 in UniCode
	convertUtf8ToUnicode(input_UTF8, tmpUnicode);
	// concert UniCode in Iso
	convertUnicodeToIso(inputCharset, tmpUnicode, output_ISO);
}


int32_t convertIsoToUtf8(charset_te inputCharset, Edn::VectorType<char>& input_ISO, Edn::VectorType<char>& output_UTF8)
{
	EDN_WARNING("TODO : not coded...");
	return 0;
}


int32_t convertUtf8ToIso(charset_te inputCharset, Edn::VectorType<char>& input_UTF8, Edn::VectorType<char>& output_ISO)
{
	EDN_WARNING("TODO : not coded...");
	return 0;
}

/**
 * @brief Get the number of element of the curent UTF8 char (in the curent Buffer)
 *
 * @param[in]			data			pointer on the curent CHAR string (pointer on the allocated buffer)
 * @param[out]			size			Nb of char use in this UTF8 [0..4]
 * @param[out]			baseValid		true : the ase format of the UTF8 is CORRECT
 *
 * @return ---
 *
 */
void Utf8_SizeElement(const char * data, int32_t lenMax , uint8_t &size, bool &baseValid)
{
	EDN_ASSERT(0 <= lenMax, "size can not be < 0 ...");
	if (0 > lenMax) {
		size = 0;
		baseValid = false;
		return;
	}
	//4 case
	if (	1<=lenMax
		&&	0x00 == (data[0] & 0x80) ) {
		// One Char Element
		size = 1;
		baseValid = true;
	} else if(		2<=lenMax
				&&	0xC0 == (data[0] & 0xE0)
				&&	0x80 == (data[1] & 0xC0) ) {
		size = 2;
		baseValid = true;
	} else if(		3<=lenMax
				&&	0xE0 == (data[0] & 0xF0)
				&&	0x80 == (data[1] & 0xC0)
				&&	0x80 == (data[2] & 0xC0)) {
		size = 3;
		baseValid = true;
	} else if(		4<=lenMax
				&&	0xF0 == (data[0] & 0xF8)
				&&	0x80 == (data[1] & 0xC0)
				&&	0x80 == (data[2] & 0xC0)
				&&	0x80 == (data[3] & 0xC0)) {
		size = 4;
		baseValid = true;
	} else {
		// return only one error Caracter ...
		baseValid = false;
		size = 1;
	}
}

#if 0 // Remove for the moment ...
/**
 * @brief Get the number of element of the previous UTF8 char (in the curent Buffer) 
 *
 * @param[in]			data			pointer on the curent CHAR string (pointer on the allocated buffer) (the curent char is not check)
 * @param[out]			size			Nb of char use in this UTF8 [0..4]
 * @param[out]			baseValid		true : the ase format of the UTF8 is CORRECT
 *
 * @return ---
 *
 */
static void Utf8_SizePreviousElement(const char * data, int32_t lenMax, uint8_t &size, bool &baseValid)
{
	EDN_ASSERT(0 <= lenMax, "size can not be < 0 ...");
	if (0 > lenMax) {
		size = 0;
		baseValid = false;
		return;
	}
	//4 case
	if (	1<=lenMax
		&&	0x00 == (data[-1] & 0x80) ) {
		// One Char Element
		size = 1;
		baseValid = true;
	} else if(		2<=lenMax
				&&	0xC0 == (data[-2] & 0xE0)
				&&	0x80 == (data[-1] & 0xC0) ) {
		size = 2;
		baseValid = true;
	} else if(		3<=lenMax
				&&	0xE0 == (data[-3] & 0xF0)
				&&	0x80 == (data[-2] & 0xC0)
				&&	0x80 == (data[-1] & 0xC0)) {
		size = 3;
		baseValid = true;
	} else if(		4<=lenMax
				&&	0xF0 == (data[-4] & 0xF8)
				&&	0x80 == (data[-3] & 0xC0)
				&&	0x80 == (data[-2] & 0xC0)
				&&	0x80 == (data[-1] & 0xC0)) {
		size = 4;
		baseValid = true;
	} else {
		// return only one error Caracter ...
		baseValid = false;
		size = 1;
	}
}
#endif

/**
 * @brief 
 *
 * @param[in,out] 
 *
 * @return 
 *
 */
 /*
static uint32_t Utf8_GetValue(UTF8Element_ts &Element)
{
	uint32_t value = 0;
	const char * data = m_data + Element.CharPosition;
	//4 case
	switch(Element.CharSize)
	{
		case 1:
			value = data[0] & 0x7F;
			break;
		case 2:
			value  = (data[0] & 0x1F)<< 6;
			value +=  data[1] & 0x3F;
			break;
		case 3:
			value  = (data[0] & 0x0F)<< 12;
			value += (data[1] & 0x3F)<< 6;
			value +=  data[2] & 0x3F;
			break;
		case 4:
			value  = (data[0] & 0x07)<< 18;
			value += (data[1] & 0x3F)<< 12;
			value += (data[2] & 0x3F)<< 6;
			value +=  data[3] & 0x3F;
			break;
		default:
			// return only one error Caracter ...
			EDN_ASSERT(false, "impossible case....");
			break;
	}
	// check the validity of the UTF8 ...
	if(		(		0xD800 <= value 
				&&	0xDFFF >= value )
		||	(		0xFDD0 <= value 
				&&	0xFDEF >= value )
		||	(		0xFFFE <= value 
				&&	0xFFFF >= value )
		||	(		0x1FFFE <= value 
				&&	0x1FFFF >= value )
		||	(		0x2FFFE <= value 
				&&	0xDFFFF >= value )
		||	(		0xEFFFE <= value 
				&&	0xEFFFF >= value )
		||	(		0xFFFFE <= value 
				&&	0xFFFFF >= value )
		||	(		0x10FFFE <= value 
				&&	0x10FFFF >= value ) )
	{
		// overwrite the UTF8 validity ==> this is not a diaplayable element
		Element.ValidUTF8 = false;
		return value;
	}
	
	return value;
}
*/


int32_t strUtf8Len(const char *input_UTF8)
{
	int32_t count = 0;
	int32_t size = strlen(input_UTF8);
	uint8_t tmpSize;
	bool baseValid;
	while (size > 0) {
		Utf8_SizeElement(input_UTF8, size , tmpSize, baseValid);
		input_UTF8 += tmpSize;
		size -= tmpSize;
		count++;
	}
	return count;
}



// **************************************************************************************************************
//       simple convertion optention
// **************************************************************************************************************

#if 0

Procedure de recuperation des charset sans ce casser les ...

// generate the basic file
	FILE * mfile = NULL;
	mfile = fopen("fichierIsoBase", "wb");
	if (NULL == mfile) {
		EDN_ERROR("Error to create file");
		return false;
	}
	char newline = '\n';
	for(int32_t i=0x20; i<0x100; i++) {
		char plop = i;
		fwrite(&plop, sizeof(char), 1, mfile);
		fwrite(&newline, sizeof(char), 1, mfile);
	}
	fclose(mfile);
// console script to convert files :
iconv -c --from-code=ISO-8859-1 --to-code=UTF-8 -o fichierUTF8_iso-1 fichierIsoBase
iconv -c --from-code=ISO-8859-2 --to-code=UTF-8 -o fichierUTF8_iso-2 fichierIsoBase
iconv -c --from-code=ISO-8859-3 --to-code=UTF-8 -o fichierUTF8_iso-3 fichierIsoBase
iconv -c --from-code=ISO-8859-4 --to-code=UTF-8 -o fichierUTF8_iso-4 fichierIsoBase
iconv -c --from-code=ISO-8859-5 --to-code=UTF-8 -o fichierUTF8_iso-5 fichierIsoBase
iconv -c --from-code=ISO-8859-6 --to-code=UTF-8 -o fichierUTF8_iso-6 fichierIsoBase
iconv -c --from-code=ISO-8859-7 --to-code=UTF-8 -o fichierUTF8_iso-7 fichierIsoBase
iconv -c --from-code=ISO-8859-8 --to-code=UTF-8 -o fichierUTF8_iso-8 fichierIsoBase
iconv -c --from-code=ISO-8859-9 --to-code=UTF-8 -o fichierUTF8_iso-9 fichierIsoBase
iconv -c --from-code=ISO-8859-10 --to-code=UTF-8 -o fichierUTF8_iso-10 fichierIsoBase
iconv -c --from-code=ISO-8859-11 --to-code=UTF-8 -o fichierUTF8_iso-11 fichierIsoBase
iconv -c --from-code=ISO-8859-12 --to-code=UTF-8 -o fichierUTF8_iso-12 fichierIsoBase
iconv -c --from-code=ISO-8859-13 --to-code=UTF-8 -o fichierUTF8_iso-13 fichierIsoBase
iconv -c --from-code=ISO-8859-14 --to-code=UTF-8 -o fichierUTF8_iso-14 fichierIsoBase
iconv -c --from-code=ISO-8859-15 --to-code=UTF-8 -o fichierUTF8_iso-15 fichierIsoBase

// NOTE : Le format 12 n'existe pas ...
	FILE * mfileout = NULL;
	mfileout = fopen("outputGeneration.c", "wb");
	if (NULL == mfileout) {
		EDN_ERROR("Error to create file");
		return false;
	}
	
	char * inputFileData[] = {
		"fichierUTF8_iso-1",
		"fichierUTF8_iso-2",
//		"fichierUTF8_iso-3",
		"fichierUTF8_iso-4",
		"fichierUTF8_iso-5",
/*		"fichierUTF8_iso-6",
		"fichierUTF8_iso-7",
		"fichierUTF8_iso-8",
		"fichierUTF8_iso-9",
		"fichierUTF8_iso-10",
		"fichierUTF8_iso-11",
		"fichierUTF8_iso-13",
		"fichierUTF8_iso-14",
*/
		"fichierUTF8_iso-15"
	};
	
	for (int32_t k=0; k<5; k++) {
		FILE * mfile = NULL;
		mfile = fopen(inputFileData[k], "rb");
		if (NULL == mfile) {
			EDN_ERROR("Error to open file");
			return false;
		}
		char data[255] ;
		fprintf(mfileout, "\tTYPESTRUCT_TS  %s[] = {\n\t\t", inputFileData[k]);
		for(int32_t i=0x0; i<0x10; i++) {
			fprintf(mfileout, "0x%08X, ", i);
		}
		fprintf(mfileout, "\n\t\t");
		for(int32_t i=0x10; i<0x20; i++) {
			fprintf(mfileout, "0x%08X, ", i);
		}
		for(int32_t i=0x20; i<0x100; i++) {
			if (0==i%16) {
				fprintf(mfileout, "\n\t\t");
			}
			fgets(data, 25, mfile );
			data[strlen(data)-1] = '\0';
			EDN_INFO("sizeofLine=" << strlen(data) << " data=\"" << data << "\"");
			// convert in int : 
			int32_t valUTF8 = 0;
			int32_t valUnicode = 0;
			switch (strlen(data)) {
				case 1:
					valUTF8 = (uint8_t) (data[0]);
					valUnicode = (uint8_t)(data[0]) & 0x7F;
					break;
				case 2:
					valUTF8  = (uint8_t) (data[0]) << 8;
					valUTF8 += (uint8_t) (data[1]);
					valUnicode  = (((uint8_t)data[0]) & 0x1F)<< 6;
					valUnicode +=  ((uint8_t)data[1]) & 0x3F;
					break;
				case 3:
					valUTF8  = (uint8_t) (data[0]) << 16;
					valUTF8 += (uint8_t) (data[1]) << 8;
					valUTF8 += (uint8_t) (data[2]);
					valUnicode  = (((uint8_t)data[0]) & 0x0F)<< 12;
					valUnicode += (((uint8_t)data[1]) & 0x3F)<< 6;
					valUnicode +=  ((uint8_t)data[2]) & 0x3F;
					break;
				default:
					valUTF8  = (uint8_t) (data[0]) <<24;
					valUTF8 += (uint8_t) (data[1]) << 16;
					valUTF8 += (uint8_t) (data[2]) << 8;
					valUTF8 += (uint8_t) (data[3]);
					valUnicode  = (((uint8_t)data[0]) & 0x07)<< 18;
					valUnicode += (((uint8_t)data[1]) & 0x3F)<< 12;
					valUnicode += (((uint8_t)data[2]) & 0x3F)<< 6;
					valUnicode +=  ((uint8_t)data[3]) & 0x3F;
					break;
			}
			fprintf(mfileout, "0x%08X, ", valUnicode);
		}
		fprintf(mfileout, "\n\t};\n\n");
		fclose(mfile);
	}
	fclose(mfileout);

#endif




void ConvertInput(GdkEventKey *event, char* Utf8Out, bool &controlKey, bool &moveKey, int32_t &key)
{
	char *eventMode=(char*)"KEY REALEASED";
	if(event->type == GDK_KEY_PRESS)  {
		eventMode=(char*)"KEY PRESSED";
	}
	moveKey = false;
	controlKey = false;
	key = event->keyval;
	//EDN_INFO("key : " << key);
	switch (key)
	{
#		ifdef USE_GTK_VERSION_3_0
		case GDK_KEY_KP_Space:
#       elif USE_GTK_VERSION_2_0
		case GDK_KP_Space:
#		endif
			key = (int32_t)' ';
			break;
#		ifdef USE_GTK_VERSION_3_0
		case GDK_KEY_KP_Tab:
#       elif USE_GTK_VERSION_2_0
		case GDK_KP_Tab:
#		endif
		// shift + TAB ... same as a tab here ...
		case 0xfe20: //GDK_ISO_Left_Tab
		case 65289:
			key = (int32_t)'\t';
			break;
#		ifdef USE_GTK_VERSION_3_0
		case GDK_KEY_KP_Enter:
#       elif USE_GTK_VERSION_2_0
		case GDK_KP_Enter:
#		endif
			key = '\n';
			break;
#		ifdef USE_GTK_VERSION_3_0
		case GDK_KEY_KP_F1:
		case GDK_KEY_F1:
			key = GDK_KEY_F1;
#       elif USE_GTK_VERSION_2_0
		case GDK_KP_F1:
		case GDK_F1:
			key = GDK_F1;
#		endif
			controlKey = true;
			break;
#		ifdef USE_GTK_VERSION_3_0
		case GDK_KEY_KP_F2:
		case GDK_KEY_F2:
			key = GDK_KEY_F2;
#       elif USE_GTK_VERSION_2_0
		case GDK_KP_F2:
		case GDK_F2:
			key = GDK_F2;
#		endif
			controlKey = true;
			break;
#		ifdef USE_GTK_VERSION_3_0
		case GDK_KEY_KP_F3:
		case GDK_KEY_F3:
			key = GDK_KEY_F3;
#       elif USE_GTK_VERSION_2_0
		case GDK_KP_F3:
		case GDK_F3:
			key = GDK_F3;
#		endif
			controlKey = true;
			break;
#		ifdef USE_GTK_VERSION_3_0
		case GDK_KEY_KP_F4:
		case GDK_KEY_F4:
			key = GDK_KEY_F4;
#       elif USE_GTK_VERSION_2_0
		case GDK_KP_F4:
		case GDK_F4:
			key = GDK_F4;
#		endif
			controlKey = true;
			break;
#		ifdef USE_GTK_VERSION_3_0
		case GDK_KEY_F5:
		case GDK_KEY_F6:
		case GDK_KEY_F7:
		case GDK_KEY_F8:
		case GDK_KEY_F9:
		case GDK_KEY_F10:
		case GDK_KEY_F11:
		case GDK_KEY_F12:
		case GDK_KEY_F13:
		case GDK_KEY_F14:
		case GDK_KEY_F15:
		case GDK_KEY_F16:
#       elif USE_GTK_VERSION_2_0
		case GDK_F5:
		case GDK_F6:
		case GDK_F7:
		case GDK_F8:
		case GDK_F9:
		case GDK_F10:
		case GDK_F11:
		case GDK_F12:
		case GDK_F13:
		case GDK_F14:
		case GDK_F15:
		case GDK_F16:
#		endif
			controlKey = true;
			break;
#		ifdef USE_GTK_VERSION_3_0
		case GDK_KEY_KP_Home:
		case GDK_KEY_Home:
			key = GDK_KEY_Begin;
#       elif USE_GTK_VERSION_2_0
		case GDK_KP_Home:
		case GDK_Home:
			key = GDK_Begin;
#		endif
			controlKey = true;
			moveKey = true;
			break;
#		ifdef USE_GTK_VERSION_3_0
		case GDK_KEY_KP_Left:
		case GDK_KEY_Left:
			key = GDK_KEY_Left;
#       elif USE_GTK_VERSION_2_0
		case GDK_KP_Left:
		case GDK_Left:
			key = GDK_Left;
#		endif
			controlKey = true;
			moveKey = true;
			break;
#		ifdef USE_GTK_VERSION_3_0
		case GDK_KEY_KP_Up:
		case GDK_KEY_Up:
			key = GDK_KEY_Up;
#       elif USE_GTK_VERSION_2_0
		case GDK_KP_Up:
		case GDK_Up:
			key = GDK_Up;
#		endif
			controlKey = true;
			moveKey = true;
			break;
#		ifdef USE_GTK_VERSION_3_0
		case GDK_KEY_KP_Right:
		case GDK_KEY_Right:
			key = GDK_KEY_Right;
#       elif USE_GTK_VERSION_2_0
		case GDK_KP_Right:
		case GDK_Right:
			key = GDK_Right;
#		endif
			controlKey = true;
			moveKey = true;
			break;
#		ifdef USE_GTK_VERSION_3_0
		case GDK_KEY_KP_Down:
		case GDK_KEY_Down:
			key = GDK_KEY_Down;
#       elif USE_GTK_VERSION_2_0
		case GDK_KP_Down:
		case GDK_Down:
			key = GDK_Down;
#		endif
			controlKey = true;
			moveKey = true;
			break;
#		ifdef USE_GTK_VERSION_3_0
		case GDK_KEY_KP_Page_Up:
		case GDK_KEY_Page_Up:
			key = GDK_KEY_Page_Up;
#       elif USE_GTK_VERSION_2_0
		case GDK_KP_Page_Up:
		case GDK_Page_Up:
			key = GDK_Page_Up;
#		endif
			controlKey = true;
			moveKey = true;
			break;
#		ifdef USE_GTK_VERSION_3_0
		case GDK_KEY_KP_Page_Down:
		case GDK_KEY_Page_Down:
			key = GDK_KEY_Page_Down;
#       elif USE_GTK_VERSION_2_0
		case GDK_KP_Page_Down:
		case GDK_Page_Down:
			key = GDK_Page_Down;
#		endif
			controlKey = true;
			moveKey = true;
			break;
#		ifdef USE_GTK_VERSION_3_0
		case GDK_KEY_KP_End:
		case GDK_KEY_End:
			key = GDK_KEY_End;
#       elif USE_GTK_VERSION_2_0
		case GDK_KP_End:
		case GDK_End:
			key = GDK_End;
#		endif
			controlKey = true;
			moveKey = true;
			break;
#		ifdef USE_GTK_VERSION_3_0
		case GDK_KEY_KP_Begin:
		case GDK_KEY_Begin:
			key = GDK_KEY_Begin;
#       elif USE_GTK_VERSION_2_0
		case GDK_KP_Begin:
		case GDK_Begin:
			key = GDK_Begin;
#		endif
			controlKey = true;
			moveKey = true;
			break;
#		ifdef USE_GTK_VERSION_3_0
		case GDK_KEY_KP_Equal:
#       elif USE_GTK_VERSION_2_0
		case GDK_KP_Equal:
#		endif
			key = (int32_t)'=';
			break;
#		ifdef USE_GTK_VERSION_3_0
		case GDK_KEY_KP_Multiply:
#       elif USE_GTK_VERSION_2_0
		case GDK_KP_Multiply:
#		endif
			key = (int32_t)'*';
			break;
#		ifdef USE_GTK_VERSION_3_0
		case GDK_KEY_KP_Add:
#       elif USE_GTK_VERSION_2_0
		case GDK_KP_Add:
#		endif
			key = (int32_t)'+';
			break;
#		ifdef USE_GTK_VERSION_3_0
		case GDK_KEY_KP_Separator:
#       elif USE_GTK_VERSION_2_0
		case GDK_KP_Separator:
#		endif
			// TODO ... key = (int32)'\t';
			break;
#		ifdef USE_GTK_VERSION_3_0
		case GDK_KEY_KP_Subtract:
#       elif USE_GTK_VERSION_2_0
		case GDK_KP_Subtract:
#		endif
			key = (int32_t)'-';
			break;
#		ifdef USE_GTK_VERSION_3_0
		case GDK_KEY_KP_Decimal:
#       elif USE_GTK_VERSION_2_0
		case GDK_KP_Decimal:
#		endif
			// TODO ... key = (int32)'\t';
			break;
#		ifdef USE_GTK_VERSION_3_0
		case GDK_KEY_KP_Divide:
#       elif USE_GTK_VERSION_2_0
		case GDK_KP_Divide:
#		endif
			key = (int32_t)'/';
			break;
#		ifdef USE_GTK_VERSION_3_0
		case GDK_KEY_KP_0:
#       elif USE_GTK_VERSION_2_0
		case GDK_KP_0:
#		endif
			key = (int32_t)'0';
			break;
#		ifdef USE_GTK_VERSION_3_0
		case GDK_KEY_KP_1:
#       elif USE_GTK_VERSION_2_0
		case GDK_KP_1:
#		endif
			key = (int32_t)'1';
			break;
#		ifdef USE_GTK_VERSION_3_0
		case GDK_KEY_KP_2:
#       elif USE_GTK_VERSION_2_0
		case GDK_KP_2:
#		endif
			key = (int32_t)'2';
			break;
#		ifdef USE_GTK_VERSION_3_0
		case GDK_KEY_KP_3:
#       elif USE_GTK_VERSION_2_0
		case GDK_KP_3:
#		endif
			key = (int32_t)'3';
			break;
#		ifdef USE_GTK_VERSION_3_0
		case GDK_KEY_KP_4:
#       elif USE_GTK_VERSION_2_0
		case GDK_KP_4:
#		endif
			key = (int32_t)'4';
			break;
#		ifdef USE_GTK_VERSION_3_0
		case GDK_KEY_KP_5:
#       elif USE_GTK_VERSION_2_0
		case GDK_KP_5:
#		endif
			key = (int32_t)'5';
			break;
#		ifdef USE_GTK_VERSION_3_0
		case GDK_KEY_KP_6:
#       elif USE_GTK_VERSION_2_0
		case GDK_KP_6:
#		endif
			key = (int32_t)'6';
			break;
#		ifdef USE_GTK_VERSION_3_0
		case GDK_KEY_KP_7:
#       elif USE_GTK_VERSION_2_0
		case GDK_KP_7:
#		endif
			key = (int32_t)'7';
			break;
#		ifdef USE_GTK_VERSION_3_0
		case GDK_KEY_KP_8:
#       elif USE_GTK_VERSION_2_0
		case GDK_KP_8:
#		endif
			key = (int32_t)'8';
			break;
#		ifdef USE_GTK_VERSION_3_0
		case GDK_KEY_KP_9:
#       elif USE_GTK_VERSION_2_0
		case GDK_KP_9:
#		endif
			key = (int32_t)'9';
			break;

#		ifdef USE_GTK_VERSION_3_0
		case GDK_KEY_Clear:
#       elif USE_GTK_VERSION_2_0
		case GDK_Clear:
#		endif
			controlKey = true;
			break;
#		ifdef USE_GTK_VERSION_3_0
		case GDK_KEY_Escape:
#       elif USE_GTK_VERSION_2_0
		case GDK_Escape:
#		endif
			controlKey = true;
			break;
#		ifdef USE_GTK_VERSION_3_0
		case GDK_KEY_Meta_L:
		case GDK_KEY_Meta_R:
#       elif USE_GTK_VERSION_2_0
		case GDK_Meta_L:
		case GDK_Meta_R:
#		endif
			EDN_INFO("keyEvent : <META>	  " << eventMode);
			controlKey = true;
			break;
#		ifdef USE_GTK_VERSION_3_0
		case GDK_KEY_Shift_L:
		case GDK_KEY_Shift_R:
#       elif USE_GTK_VERSION_2_0
		case GDK_Shift_L:
		case GDK_Shift_R:
#		endif
			controlKey = true;
			//EDN_INFO("keyEvent : <SHIFT>	  " << eventMode);
			if(event->type == GDK_KEY_PRESS) {
				globals::SetShift();
			} else {
				globals::UnSetShift();
			}
			break;
#		ifdef USE_GTK_VERSION_3_0
		case GDK_KEY_Control_L:
		case GDK_KEY_Control_R:
#       elif USE_GTK_VERSION_2_0
		case GDK_Control_L:
		case GDK_Control_R:
#		endif
			controlKey = true;
			//EDN_INFO("keyEvent : <Ctrl>	  " << eventMode);
			if(event->type == GDK_KEY_PRESS) {
				globals::SetCtrl();
			} else {
				globals::UnSetCtrl();
			}
			break;
#		ifdef USE_GTK_VERSION_3_0
		case GDK_KEY_Alt_L:
		case GDK_KEY_Alt_R:
#       elif USE_GTK_VERSION_2_0
		case GDK_Alt_L:
		case GDK_Alt_R:
#		endif
			controlKey = true;
			//EDN_INFO("keyEvent : <Alt>	  " << eventMode);
			if(event->type == GDK_KEY_PRESS) {
				globals::SetAlt();
			} else {
				globals::UnSetAlt();
			}
			break;
#		ifdef USE_GTK_VERSION_3_0
		case GDK_KEY_Alt_Gr:
#       elif USE_GTK_VERSION_2_0
		case GDK_Alt_Gr:
#		endif
			EDN_INFO("keyEvent : <Alt-G>	  " << eventMode);
			controlKey = true;
			break;
#		ifdef USE_GTK_VERSION_3_0
		case GDK_KEY_KP_Insert:
		case GDK_KEY_Insert:
			key = GDK_KEY_Insert;
#       elif USE_GTK_VERSION_2_0
		case GDK_KP_Insert:
		case GDK_Insert:
			key = GDK_Insert;
#		endif
			//EDN_INFO("keyEvent : <Insert>	  " << eventMode);
			if(event->type == GDK_KEY_PRESS) {
				globals::ToggleInsert();
			}
			controlKey = true;
			break;
#		ifdef USE_GTK_VERSION_3_0
		case GDK_KEY_Pomme_L:
		case GDK_KEY_Pomme_R:
#       elif USE_GTK_VERSION_2_0
		case GDK_Pomme_L:
		case GDK_Pomme_R:
#		endif
			//EDN_INFO("keyEvent : <Pomme>	  " << eventMode);
			if(event->type == GDK_KEY_PRESS) {
				globals::SetPomme();
			} else {
				globals::UnSetPomme();
			}
			controlKey = true;
			break;
#		ifdef USE_GTK_VERSION_3_0
		case GDK_KEY_Return:
#       elif USE_GTK_VERSION_2_0
		case GDK_Return:
#		endif
			key = '\n';
			break;
#		ifdef USE_GTK_VERSION_3_0
		case GDK_KEY_KP_Delete:
		case GDK_KEY_Delete:
			key = GDK_KEY_Delete;
#       elif USE_GTK_VERSION_2_0
		case GDK_KP_Delete:
		case GDK_Delete:
			key = GDK_Delete;
#		endif
			key = 0x7F;
			break;
#		ifdef USE_GTK_VERSION_3_0
		case GDK_KEY_BackSpace:
#       elif USE_GTK_VERSION_2_0
		case GDK_BackSpace:
#		endif
			key = 0x08;
			break;

#		ifdef USE_GTK_VERSION_3_0
		case GDK_KEY_dead_grave:
		case GDK_KEY_dead_acute:
		case GDK_KEY_dead_circumflex:
		case GDK_KEY_dead_tilde:
		case GDK_KEY_dead_macron:
		case GDK_KEY_dead_breve:
		case GDK_KEY_dead_abovedot:
		case GDK_KEY_dead_diaeresis:
		case GDK_KEY_dead_abovering:
		case GDK_KEY_dead_doubleacute:
		case GDK_KEY_dead_caron:
		case GDK_KEY_dead_cedilla:
		case GDK_KEY_dead_ogonek:
		case GDK_KEY_dead_iota:
		case GDK_KEY_dead_voiced_sound:
		case GDK_KEY_dead_semivoiced_sound:
		case GDK_KEY_dead_belowdot:
		case GDK_KEY_dead_hook:
		case GDK_KEY_dead_horn:
		case GDK_KEY_dead_stroke:
		case GDK_KEY_dead_abovecomma:
		case GDK_KEY_dead_abovereversedcomma:
		case GDK_KEY_dead_doublegrave:
		case GDK_KEY_dead_belowring:
		case GDK_KEY_dead_belowmacron:
		case GDK_KEY_dead_belowcircumflex:
		case GDK_KEY_dead_belowtilde:
		case GDK_KEY_dead_belowbreve:
		case GDK_KEY_dead_belowdiaeresis:
		case GDK_KEY_dead_invertedbreve:
		case GDK_KEY_dead_belowcomma:
		case GDK_KEY_dead_currency:
		case GDK_KEY_dead_a:
		case GDK_KEY_dead_A:
		case GDK_KEY_dead_e:
		case GDK_KEY_dead_E:
		case GDK_KEY_dead_i:
		case GDK_KEY_dead_I:
		case GDK_KEY_dead_o:
		case GDK_KEY_dead_O:
		case GDK_KEY_dead_u:
		case GDK_KEY_dead_U:
		case GDK_KEY_dead_small_schwa:
		case GDK_KEY_dead_capital_schwa:
#       elif USE_GTK_VERSION_2_0
		case GDK_dead_grave:
		case GDK_dead_acute:
		case GDK_dead_circumflex:
			key = '^';
			break;
		case GDK_dead_tilde:
		case GDK_dead_macron:
		case GDK_dead_breve:
		case GDK_dead_abovedot:
		case GDK_dead_diaeresis:
		case GDK_dead_abovering:
		case GDK_dead_doubleacute:
		case GDK_dead_caron:
		case GDK_dead_cedilla:
		case GDK_dead_ogonek:
		case GDK_dead_iota:
		case GDK_dead_voiced_sound:
		case GDK_dead_semivoiced_sound:
		case GDK_dead_belowdot:
		case GDK_dead_hook:
		case GDK_dead_horn:
		case GDK_dead_stroke:
		case GDK_dead_abovecomma:
		case GDK_dead_abovereversedcomma:
		case GDK_dead_doublegrave:
		case GDK_dead_belowring:
		case GDK_dead_belowmacron:
		case GDK_dead_belowcircumflex:
		case GDK_dead_belowtilde:
		case GDK_dead_belowbreve:
		case GDK_dead_belowdiaeresis:
		case GDK_dead_invertedbreve:
		case GDK_dead_belowcomma:
		case GDK_dead_currency:
			key = '?';
			break;
		case GDK_dead_a:
			EDN_INFO("dead a");
			//key = 'a';
			break;
		case GDK_dead_A:
		case GDK_dead_e:
		case GDK_dead_E:
		case GDK_dead_i:
		case GDK_dead_I:
		case GDK_dead_o:
		case GDK_dead_O:
		case GDK_dead_u:
		case GDK_dead_U:
		case GDK_dead_small_schwa:
		case GDK_dead_capital_schwa:
#		endif
			key = '?';
			break;
#		ifdef USE_GTK_VERSION_3_0
		case GDK_KEY_Caps_Lock:
#		elif USE_GTK_VERSION_2_0
		case GDK_Caps_Lock:
#		endif
			controlKey = true;
			break;
		default:
			break;
	}
	// Transform in unicode the input : 
	//EDN_INFO("key : " << key);
	int32_t unichar = gdk_keyval_to_unicode(key);
	//EDN_INFO("unichar : " << unichar);
	if (unichar == 0) {
		Utf8Out[0] = (char)key;
		Utf8Out[1] = '\0';
		//EDN_INFO("NON UTF8 : " << Utf8Out);
	} else {
		// Generate UTF8 form UniCode
		convertUnicodeToUtf8(unichar, Utf8Out);
		//EDN_INFO("UTF8 : " << Utf8Out);
		#if 0
		printf(" convertUnicodeToUtf8 : \"0x%08x\" ==> unichar=%d %s\n", key, unichar, Utf8Out);
		for (int32_t uu=0; uu < strlen(Utf8Out); uu++) {
			printf("         0x%02x\n", Utf8Out[uu]);
		}
		#endif
	}

}
