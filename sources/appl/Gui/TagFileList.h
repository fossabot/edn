/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */

#ifndef __APPL_CTAGS_LIST_H__
#define __APPL_CTAGS_LIST_H__

#include <appl/Debug.h>
#include <ewol/widget/List.h>


extern const char * const applEventCtagsListSelect;
extern const char * const applEventCtagsListValidate;
extern const char * const applEventCtagsListUnSelect;

namespace appl {
	class TagListElement {
		public:
			etk::UString filename;
			int32_t      fileLine;
			TagListElement(etk::UString& _file, int32_t _line) :
			  filename(_file),
			  fileLine(_line) {
				
			};
			~TagListElement(void) {
				
			};
	};
	class TagFileList : public widget::List {
		private:
			int32_t m_selectedLine;
			etk::Vector<appl::TagListElement*> m_list;
		public:
			TagFileList(void);
			~TagFileList(void);
			// display API :
			virtual etk::Color<> getBasicBG(void);
			uint32_t getNuberOfColomn(void);
			bool getTitle(int32_t _colomn, etk::UString& _myTitle, etk::Color<>& _fg, etk::Color<>& _bg);
			uint32_t getNuberOfRaw(void);
			bool getElement(int32_t _colomn, int32_t _raw, etk::UString& _myTextToWrite, etk::Color<>& _fg, etk::Color<>& _bg);
			bool onItemEvent(int32_t _IdInput, ewol::keyEvent::status_te _typeEvent, int32_t _colomn, int32_t _raw, float _x, float _y);
			// derived function
			const char * const getObjectType(void) {
				return "appl::TagFileList";
			};
		public:
			/**
			 * @brief add a Ctags item on the curent list
			 * @param[in] file Compleate file name
			 * @param[in] jump line id
			 */
			void add(etk::UString& _file, int32_t _line);
	};
};



#endif


