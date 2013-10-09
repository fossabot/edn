/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */

#ifndef __HIGHLIGHT_H__
#define __HIGHLIGHT_H__


class Highlight;
class HighlightPattern;

extern "C" {
	typedef struct {
		int32_t beginStart;
		int32_t beginStop;
		int32_t endStart;
		int32_t endStop;
		bool notEnded;
		HighlightPattern* patern; // pointer on class :
	} colorInformation_ts;
}

#include <etk/os/FSNode.h>
#include <HighlightPattern.h>
#include <Colorize.h>
#include <etk/Buffer.h>
#include <exml/exml.h>

class Highlight {
	public:
		// Constructeur
		Highlight(const etk::UString& _xmlFilename);
		~Highlight(void);
		bool hasExtention(const etk::UString& _ext);
		bool fileNameCompatible(etk::FSNode &_fileName);
		void display(void);
		void reloadColor(void);
		void parse(int32_t start,
		           int32_t stop,
		           etk::Vector<colorInformation_ts> &metaData,
		           int32_t addingPos,
		           etk::Buffer &buffer);
		void parse2(int32_t start,
		            int32_t stop,
		            etk::Vector<colorInformation_ts> &metaData,
		            etk::Buffer &buffer);
	private:
		void parseRules(exml::Element* child, etk::Vector<HighlightPattern*> &mListPatern, int32_t level);
		etk::UString m_styleName; //!< curent style name (like "c++" or "c" or "script Bash")
		etk::Vector<etk::UString> m_listExtentions; //!< List of possible extention for this high-light, like : ".c", ".cpp", ".h"
		etk::Vector<HighlightPattern*> m_listHighlightPass1; //!< List of ALL hightlight modules (pass 1  == > when we load and wride data on the buffer)
		etk::Vector<HighlightPattern*> m_listHighlightPass2; //!< List of ALL hightlight modules (pass 2  == > When we display the buffer( only the display area (100 lines)) )
};





#endif


