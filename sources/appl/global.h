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
	void init(void);
	void UnInit(void);
	int32_t getNbColoneBorder(void);
	int32_t getNbLineBorder(void);

	bool isSetDisplayEndOfLine(void);
	void setDisplayEndOfLine(bool _newVal);
	
	bool isSetDisplaySpaceChar(void);
	void setDisplaySpaceChar(bool _newVal);
	
	bool isSetDisplayTabChar(void);
	void setDisplayTabChar(bool _newVal);
	
	bool isSetAutoIndent(void);
	void setAutoIndent(bool _newVal);

	void init2(void);
	
	bool OrderTheBufferList(void);
	
	class ParameterGlobalsGui : public widget::Sizer {
		public :
			ParameterGlobalsGui(void);
			~ParameterGlobalsGui(void);
			// herited function
			virtual void onReceiveMessage(const ewol::EMessage& _msg);
	};

}

#endif


