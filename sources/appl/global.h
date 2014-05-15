/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */

#ifndef __TOOLS_GLOBALS_H__
#define __TOOLS_GLOBALS_H__

#include <appl/debug.h>
#include <ewol/widget/Sizer.h>


namespace globals
{
	void init();
	void UnInit();
	int32_t getNbColoneBorder();
	int32_t getNbLineBorder();

	bool isSetDisplayEndOfLine();
	void setDisplayEndOfLine(bool _newVal);
	
	bool isSetDisplaySpaceChar();
	void setDisplaySpaceChar(bool _newVal);
	
	bool isSetDisplayTabChar();
	void setDisplayTabChar(bool _newVal);
	
	bool isSetAutoIndent();
	void setAutoIndent(bool _newVal);

	void init2();
	
	bool OrderTheBufferList();
	
	class ParameterGlobalsGui : public ewol::widget::Sizer {
		public :
			ParameterGlobalsGui();
			~ParameterGlobalsGui();
			// herited function
			virtual void onReceiveMessage(const ewol::object::Message& _msg);
	};

}

#endif


