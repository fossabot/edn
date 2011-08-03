/**
 *******************************************************************************
 * @file File.cpp
 * @brief Editeur De N'ours : File folder and name abstraction (Sources)
 * @author Edouard DUPIN
 * @date 16/07/2011
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


#include "tools_debug.h"
#include "tools_globals.h"
#include "Edn.h"

#define MAX_FILE_NAME      (10240)

Edn::File::File(Edn::String &filename, int32_t LineNumber)
{
	m_lineNumberOpen = 0;
	SetCompleateName(filename);
}


Edn::File::File(Edn::String &filename, Edn::String &folder, int32_t lineNumber)
{
	Edn::String tmpString = folder;
	tmpString += '/';
	tmpString += filename;
	SetCompleateName(tmpString);
	m_lineNumberOpen = lineNumber;
}


Edn::File::~File(void)
{
	// nothing to do ...
}

void Edn::File::ExtranctAndName(Edn::String &inputString)
{
	m_folder = "";
	m_shortFilename = "";
	
	for (int32_t iii=inputString.Size()-1; iii >= 0 ; iii--) {
		/*
		if (inputString[iii] != '/') {
			m_shortFilename.Insert(0, inputString[iii]);
		} else {
			break;
		}
		*/
	}
	


/*
	char tmpVal[4096];
	strncpy(tmpVal, inputString.c_str(), 4096);
	tmpVal[4096-1] = '\0';
	char *ptr = strrchr(tmpVal, '/');
	if (NULL == ptr) {
		ptr = strrchr(tmpVal, '\\');
	}
	Edn::String out = "./";
	if (NULL != ptr) {
		*ptr = '\0';
		out = tmpVal;
		out+= '/';
	}
	return out;
*/
}


Edn::String Edn::File::GetFolder(void)
{
	return m_folder;
}

Edn::String Edn::File::GetShortFilename(void)
{
	return m_shortFilename;
}

Edn::String Edn::File::GetCompleateName(void)
{
	Edn::String out;
	out  = m_folder;
	out += '/';
	out += m_shortFilename;
}

void Edn::File::SetCompleateName(Edn::String &newFilename)
{
	char buf[MAX_FILE_NAME];
	memset(buf, 0, MAX_FILE_NAME);
	char * ok;
	Edn::String destFilename;
	if (newFilename.Size() == 0) {
		destFilename = "no-name";
	} else {
		destFilename = newFilename;
	}
	if ('/' != *destFilename.c_str()) {
		// Get the command came from the running of the program : 
		char cCurrentPath[FILENAME_MAX];
		if (!getcwd(cCurrentPath, FILENAME_MAX)) {
			return;
		}
		cCurrentPath[FILENAME_MAX - 1] = '\0';
		Edn::String tmpFilename = destFilename;
		destFilename = cCurrentPath;
		destFilename += '/';
		destFilename += tmpFilename;
	} 
	
	// Get the real Path of the current File
	ok = realpath(destFilename.c_str(), buf);
	if (!ok) {
		EDN_ERROR("Can not find real name of \"" << destFilename.c_str() << "\"");
	} else {
		EDN_DEBUG("file : \"" << destFilename.c_str() << "\" done:\"" << buf << "\" ");
		// TODO : try again with no name
	}
}

int32_t Edn::File::GetLineNumber(void)
{
	return m_lineNumberOpen;
}