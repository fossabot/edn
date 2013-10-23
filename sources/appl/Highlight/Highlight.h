/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */

#ifndef __HIGHLIGHT_H__
#define __HIGHLIGHT_H__


class appl {
	class Highlight;
	class HighlightPattern;
	
	class ColorInfo{
		public:
			int32_t beginStart;
			int32_t beginStop;
			int32_t endStart;
			int32_t endStop;
			bool notEnded;
			appl::HighlightPattern* patern; // pointer on class :
	};
};

#include <etk/os/FSNode.h>
#include <HighlightPattern.h>
#include <Colorize.h>
#include <etk/Buffer.h>
#include <exml/exml.h>

class appl {
	class Highlight : public ewol::EObject {
		public:
			// Constructeur
			Highlight(const etk::UString& _xmlFilename);
			~Highlight(void);
			bool hasExtention(const etk::UString& _ext);
			bool fileNameCompatible(etk::FSNode &_fileName);
			void display(void);
			void reloadColor(void);
			void parse(int32_t _start,
			           int32_t _stop,
			           etk::Vector<appl::ColorInfo> &_metaData,
			           int32_t _addingPos,
			           etk::Buffer &_buffer);
			void parse2(int32_t _start,
			            int32_t _stop,
			            etk::Vector<appl::ColorInfo> &_metaData,
			            etk::Buffer &_buffer);
		private:
			void parseRules(exml::Element* _child,
			                etk::Vector<HighlightPattern*> &_mListPatern,
			                int32_t _level);
			etk::UString m_styleName; //!< curent style name (like "c++" or "c" or "script Bash")
			etk::Vector<etk::UString> m_listExtentions; //!< List of possible extention for this high-light, like : ".c", ".cpp", ".h"
			etk::Vector<HighlightPattern*> m_listHighlightPass1; //!< List of ALL hightlight modules (pass 1  == > when we load and wride data on the buffer)
			etk::Vector<HighlightPattern*> m_listHighlightPass2; //!< List of ALL hightlight modules (pass 2  == > When we display the buffer( only the display area (100 lines)) )
	};
};





#endif


