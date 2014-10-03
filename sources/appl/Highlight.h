/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */

#ifndef __HIGHLIGHT_H__
#define __HIGHLIGHT_H__


namespace appl {
	class Highlight;
	class HighlightPattern;
	
	class HighlightInfo {
		public:
			int32_t start;
			int32_t stop;
			bool notEnded;
			appl::HighlightPattern* patern; // pointer on class :
	};
};

#include <memory>
#include <etk/os/FSNode.h>
#include <appl/HighlightPattern.h>
#include <appl/GlyphPainting.h>
#include <etk/Buffer.h>
#include <exml/exml.h>

namespace appl {
	class Highlight : public ewol::Resource {
		private:
			std::shared_ptr<appl::GlyphPainting> m_paintingProperties;
		public:
			// Constructeur
			Highlight();
			void init(const std::string& _xmlFilename, const std::string& _colorFile = "THEME:COLOR:textViewer.json");
		public:
			DECLARE_RESOURCE_NAMED_FACTORY(Highlight);
			virtual ~Highlight();
		private:
			std::string m_typeName; //!< descriptive string type like "C/C++"
		public:
			const std::string& getTypeName() {
				return m_typeName;
			}
		public:
			bool hasExtention(const std::string& _ext);
			bool fileNameCompatible(const std::string& _fileName);
			void display();
			void parse(int64_t _start,
			           int64_t _stop,
			           std::vector<appl::HighlightInfo> &_metaData,
			           int64_t _addingPos,
			           std::u32string &_buffer);
			void parse2(int64_t _start,
			            int64_t _stop,
			            std::vector<appl::HighlightInfo> &_metaData,
			            std::u32string &_buffer);
		private:
			void parseRules(exml::Element* _child,
			                std::vector<std::unique_ptr<HighlightPattern>> &_mListPatern,
			                int32_t _level,
			                bool forceMaximize=false);
			std::string m_styleName; //!< curent style name (like "c++" or "c" or "script Bash")
			std::vector<std::string> m_listExtentions; //!< List of possible extention for this high-light, like : ".c", ".cpp", ".h"
			std::vector<std::unique_ptr<HighlightPattern>> m_listHighlightPass1; //!< List of ALL hightlight modules (pass 1  == > when we load and wride data on the buffer)
			std::vector<std::unique_ptr<HighlightPattern>> m_listHighlightPass2; //!< List of ALL hightlight modules (pass 2  == > When we display the buffer( only the display area (100 lines)) )
		public: // herited function :
			virtual void updateContext() {
				// no upfate to do ...
			};
	};
};





#endif


