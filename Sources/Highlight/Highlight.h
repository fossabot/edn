/**
 *******************************************************************************
 * @file Highlight.h
 * @brief Editeur De N'ours : Hightlightning Specific (header)
 * @author Edouard DUPIN
 * @date 14/12/2010
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

#ifndef __HIGHLIGHT_H__
#define __HIGHLIGHT_H__


class Highlight;
class HighlightPattern;

extern "C" {
	typedef struct
	{
		int32_t             beginStart;
		int32_t             beginStop;
		int32_t             endStart;
		int32_t             endStop;
		bool                notEnded;
		HighlightPattern *  patern;		// pointer on class : 
	} colorInformation_ts;
}

#include <vector>
#include "HighlightPattern.h"
#include "Colorize.h"
#include "Edn.h"
#include "EdnVectorBuf.h"
#include "tinyxml.h"

class Highlight {
	public:
		// Constructeur
		Highlight(Edn::String &xmlFilename);
		~Highlight(void);
		bool HasExtention(Edn::String &ext);
		bool FileNameCompatible(Edn::File &fileName);
		void Display(void);
		void Parse( int32_t start, int32_t stop, std::vector<colorInformation_ts> &metaData, int32_t &addingPos, EdnVectorBuf &buffer, int32_t elementID=0);
		void Parse2(int32_t start, int32_t stop, std::vector<colorInformation_ts> &metaData, EdnVectorBuf &buffer, int32_t elementID=0);

	private:
		void                            ParseRules(TiXmlNode *child, std::vector<HighlightPattern*> &mListPatern, int32_t level);
		Edn::String                     m_styleName;               //!< curent style name (like "c++" or "c" or "script Bash")
		std::vector<Edn::String>        m_listExtentions;          //!< List of possible extention for this high-light, like : ".c", ".cpp", ".h"
		std::vector<HighlightPattern*>  m_listHighlightPass1;      //!< List of ALL hightlight modules (pass 1 ==> when we load and wride data on the buffer)
		std::vector<HighlightPattern*>  m_listHighlightPass2;      //!< List of ALL hightlight modules (pass 2 ==> When we display the buffer( only the display area (100 lines)) )
};





#endif


