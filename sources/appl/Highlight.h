/** @file
 * @author Edouard DUPIN
 * @copyright 2010, Edouard DUPIN, all right reserved
 * @license GPL v3 (see license file)
 */
#pragma once

namespace appl {
	class Highlight;
	class HighlightPattern;
	
	class HighlightInfo {
		public:
			int32_t start;
			int32_t stop;
			bool notEnded;
			appl::HighlightPattern* patern;
	};
}

#include <ememory/memory.h>
#include <etk/os/FSNode.h>
#include <appl/HighlightPattern.h>
#include <appl/GlyphPainting.h>
#include <exml/exml.h>

namespace appl {
	class Highlight : public gale::Resource {
		private:
			ememory::SharedPtr<appl::GlyphPainting> m_paintingProperties;
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
			/**
			 * @brief Get the Type of the Hightlight like c++/Bash/...
			 * @return descriptive string
			 */
			const std::string& getTypeName() {
				return m_typeName;
			}
		public:
			bool isCompatible(const std::string& _name);
			bool fileNameCompatible(const std::string& _fileName);
			void display();
			void parse(int64_t _start,
			           int64_t _stop,
			           std::vector<appl::HighlightInfo>& _metaData,
			           int64_t _addingPos,
			           std::string& _buffer);
			void parse2(int64_t _start,
			            int64_t _stop,
			            std::vector<appl::HighlightInfo>& _metaData,
			            std::string& _buffer);
			void parseSubElement(const appl::HighlightInfo& _upper,
			                     std::vector<appl::HighlightInfo>& _metaData,
			                     std::string &_buffer);
		private:
			std::string m_styleName; //!< curent style name (like "c++" or "c" or "script Bash")
			std::vector<std::string> m_listExtentions; //!< List of possible extention for this high-light, like : ".c", ".cpp", ".h"
			std::vector<HighlightPattern> m_listHighlightPass1; //!< List of ALL hightlight modules (pass 1  == > when we load and wride data on the buffer)
			std::vector<HighlightPattern> m_listHighlightPass2; //!< List of ALL hightlight modules (pass 2  == > When we display the buffer( only the display area (100 lines)) )
			std::map<std::string, std::vector<HighlightPattern>> m_listHighlightNamed; //!< list of all sub partern to parse...
		public: // herited function :
			virtual bool updateContext() {
				// no upfate to do ...
				return true;
			};
	};
}


