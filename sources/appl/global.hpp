/** @file
 * @author Edouard DUPIN
 * @copyright 2010, Edouard DUPIN, all right reserved
 * @license GPL v3 (see license file)
 */
#pragma once
#include <appl/debug.hpp>
#include <ewol/widget/Sizer.hpp>

namespace globals {
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
		protected:
			ParameterGlobalsGui();
			void init();
		public:
			DECLARE_FACTORY(ParameterGlobalsGui);
			virtual ~ParameterGlobalsGui();
			void onCallbackEndOfLine(const bool& _value);
			void onCallbackIndentation(const bool& _value);
			void onCallbackSpace(const bool& _value);
			void onCallbackTabulation(const bool& _value);
			void onCallbackRounded(const bool& _value);
	};
}


