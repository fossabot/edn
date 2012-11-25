/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */

#ifndef __TOOLS_GLOBALS_H__
#define __TOOLS_GLOBALS_H__

#include <appl/Debug.h>
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
	
	bool    OrderTheBufferList(void);
	
	class ParameterGlobalsGui : public widget::SizerVert
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


