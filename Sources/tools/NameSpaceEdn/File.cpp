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

#undef __class__
#define __class__	"Edn::File"


std::ostream& Edn::operator <<(std::ostream &os, const Edn::File &obj)
{
	os << obj.m_folder;
	os << "/";
	os << obj.m_shortFilename;
	return os;
}

Edn::File::File(Edn::String &filename, int32_t LineNumber)
{
	m_lineNumberOpen = LineNumber;
	SetCompleateName(filename);
}


Edn::File::File(const char  *filename, int32_t LineNumber)
{
	Edn::String tmpString = filename;
	m_lineNumberOpen = LineNumber;
	SetCompleateName(tmpString);
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


Edn::String Edn::File::GetFolder(void) const
{
	return m_folder;
}

Edn::String Edn::File::GetShortFilename(void) const
{
	return m_shortFilename;
}

Edn::String Edn::File::GetCompleateName(void) const
{
	Edn::String out;
	out  = m_folder;
	out += '/';
	out += m_shortFilename;
	return out;
}

const Edn::File& Edn::File::operator= (const Edn::File &ednF )
{
	if( this != &ednF ) // avoid copy to itself
	{
		m_folder = ednF.m_folder;
		m_shortFilename = ednF.m_shortFilename;
		m_lineNumberOpen = ednF.m_lineNumberOpen;
	}
	return *this;
}


/**
 * @brief 
 *
 * @param[in,out] 
 *
 * @return 
 *
 */
bool Edn::File::operator== (const Edn::File &ednF) const
{
	if( this != &ednF ) {
		if (ednF.GetCompleateName() == GetCompleateName() ) {
			return true;
		} else {
			return false;
		}
		return true;
	}
	return true;
}

/**
 * @brief 
 *
 * @param[in,out] 
 *
 * @return 
 *
 */
bool Edn::File::operator!= (const Edn::File &ednF) const
{
	return !(*this == ednF);
}


void Edn::File::SetCompleateName(Edn::String &newFilename)
{
	char buf[MAX_FILE_NAME];
	memset(buf, 0, MAX_FILE_NAME);
	char * ok;
	// Reset ALL DATA : 
	m_folder = "";
	m_shortFilename = "";
	m_lineNumberOpen = 0;
	EDN_DEBUG("1 :Set Name : " << newFilename );
	Edn::String destFilename;
	if (newFilename.Size() == 0) {
		destFilename = "no-name";
	} else {
		destFilename = newFilename;
	}
	//EDN_DEBUG("2 : Get file Name : " << destFilename );
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
	//EDN_DEBUG("3 : Get file Name : " << destFilename );
	
	// Get the real Path of the current File
	ok = realpath(destFilename.c_str(), buf);
	if (!ok) {
		int32_t lastPos = destFilename.FindBack('/');
		if (-1 != lastPos) {
			// Get the FileName
			Edn::String tmpFilename = destFilename.Extract(lastPos+1);
			destFilename.Remove(lastPos, destFilename.Size() - lastPos);
			//EDN_DEBUG("try to find :\"" << destFilename << "\" / \"" << tmpFilename << "\" ");
			ok = realpath(destFilename.c_str(), buf);
			if (!ok) {
				EDN_ERROR("Can not find real Path name of \"" << destFilename << "\"");
				m_shortFilename = tmpFilename;
				m_folder        = destFilename;
			} else {
				// ALL is OK ...
				m_shortFilename = tmpFilename;
				m_folder        = destFilename;
			}
		} else {
			EDN_WARNING("file : \"" << destFilename << "\" ==> No data???");
			// Basic ERROR ...
			m_shortFilename = destFilename;
		}
	} else {
		destFilename = buf;
		int32_t lastPos = destFilename.FindBack('/');
		if (-1 != lastPos) {
			m_shortFilename = destFilename.Extract(lastPos+1);
			m_folder        = destFilename.Extract(0, lastPos);
		} else {
			// Basic ERROR ...
			EDN_WARNING("file : \"" << destFilename << "\" ==> No data???");
			m_shortFilename = destFilename;
		}
	}
	EDN_DEBUG("Set FileName :\"" << m_folder << "\" / \"" << m_shortFilename << "\" ");
}

int32_t Edn::File::GetLineNumber(void)
{
	return m_lineNumberOpen;
}


bool Edn::File::HasExtention(void)
{
	int32_t lastPos = m_shortFilename.FindBack('.');
	if(    -1 != lastPos                       // not find the .
	    && 0  != lastPos                       // Find a . at the fist position .jdlskjdfklj ==> hiden file
	    && m_shortFilename.Size() != lastPos ) // Remove file ended with .
	{
		return true;
	} else {
		return false;
	}
}


Edn::String Edn::File::GetExtention(void)
{
	Edn::String tmpExt = "";
	int32_t lastPos = m_shortFilename.FindBack('.');
	if(    -1 != lastPos                       // not find the .
	    && 0  != lastPos                       // Find a . at the fist position .jdlskjdfklj ==> hiden file
	    && m_shortFilename.Size() != lastPos ) // Remove file ended with .
	{
		// Get the FileName
		tmpExt = m_shortFilename.Extract(lastPos+1);
	}
	return tmpExt;
}
