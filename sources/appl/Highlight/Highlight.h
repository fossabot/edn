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
	
	class ColorInfo {
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
#include <appl/glyphDecoration/GlyphPainting.h>
#include <etk/Buffer.h>
#include <exml/exml.h>

namespace appl {
	class Highlight : public ewol::Resource {
		private:
			appl::GlyphPainting* m_paintingProperties;
		protected:
			// Constructeur
			Highlight(const etk::UString& _xmlFilename, const etk::UString& _colorFile);
			~Highlight(void);
		public:
			bool hasExtention(const etk::UString& _ext);
			bool fileNameCompatible(const etk::UString& _fileName);
			void display(void);
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
		public:
			/**
			 * @brief keep the resource pointer.
			 * @note Never free this pointer by your own...
			 * @param[in] _filename Name of the configuration file.
			 * @return pointer on the resource or NULL if an error occured.
			 */
			static appl::Highlight* keep(const etk::UString& _filename);
			/**
			 * @brief release the keeped resources
			 * @param[in,out] reference on the object pointer
			 */
			static void release(appl::Highlight*& _object);
		public: // herited function :
			virtual void updateContext(void) {
				// no upfate to do ...
			};
	};
};





#endif


