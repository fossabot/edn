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
#include <ewol/widget/Sizer.h>


namespace globals
{
	void Init(void);
	void UnInit(void);
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
	
	class ParameterGlobalsGui : public widget::Sizer
	{
		public :
			ParameterGlobalsGui(void);
			~ParameterGlobalsGui(void);
			// herited function
			virtual void OnReceiveMessage(const ewol::EMessage& _msg);
	};

}

#endif


