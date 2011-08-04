/**
 *******************************************************************************
 * @file File.h
 * @brief Editeur De N'ours : File folder and name abstraction (header)
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

#ifndef __EDN__FILE_H__
#define __EDN__FILE_H__

namespace Edn
{
	class File
	{
		public:
			            File(void) { m_lineNumberOpen=0; }
			            File(Edn::String &filename, int32_t LineNumber = 0);
			            File(Edn::String &filename, Edn::String &folder, int32_t lineNumber = 0);
			            ~File(void);
			Edn::String GetFolder(void);
			Edn::String GetShortFilename(void);
			Edn::String GetCompleateName(void);
			int32_t     GetLineNumber(void);
			
			const Edn::File& operator= (const Edn::File &ednF );
			void        SetCompleateName(Edn::String &newFilename);
			
		private :
			Edn::String m_folder;
			Edn::String m_shortFilename;
			int32_t     m_lineNumberOpen;
	};
}

#endif

