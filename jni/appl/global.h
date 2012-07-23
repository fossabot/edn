/**
 *******************************************************************************
 * @file appl/global.h
 * @brief Editeur De N'ours : Globals Values (header)
 * @author Edouard DUPIN
 * @date 05/12/2010
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
 
#ifndef __TOOLS_GLOBALS_H__
#define __TOOLS_GLOBALS_H__

#include <appl/Debug.h>
#include <etk/Types.h>
#include <ewol/widget/SizerVert.h>


namespace globals
{
	erreurCode_te   init(void);
	int32_t         getNbColoneBorder(void);
	int32_t         getNbLineBorder(void);

	bool    IsSetDisplayEndOfLine(void);
	void    SetDisplayEndOfLine(bool newVal);
	
	bool    IsSetDisplaySpaceChar(void);
	void    SetDisplaySpaceChar(bool newVal);
	
	bool    IsSetDisplayTabChar(void);
	void    SetDisplayTabChar(bool newVal);
	
	bool    IsSetAutoIndent(void);
	void    SetAutoIndent(bool newVal);

	void    init2(void);
	
	
	class ParameterGlobalsGui : public ewol::SizerVert
	{
		public :
			ParameterGlobalsGui(void);
			~ParameterGlobalsGui(void);
			/**
			 * @brief Receive a message from an other EObject with a specific eventId and data
			 * @param[in] CallerObject Pointer on the EObject that information came from
			 * @param[in] eventId Message registered by this class
			 * @param[in] data Data registered by this class
			 * @return ---
			 */
			virtual void OnReceiveMessage(ewol::EObject * CallerObject, const char * eventId, etk::UString data);
	};

}

#endif


