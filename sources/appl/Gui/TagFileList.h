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
			TagListElement(etk::UString& file, int32_t line) : filename(file), fileLine(line) {};
			~TagListElement(void) {};
	};
	class TagFileList : public widget::List
	{
		private:
			int32_t m_selectedLine;
			etk::Vector<appl::TagListElement*> m_list;
		public:
			TagFileList(void);
			~TagFileList(void);
			// display API :
			virtual etk::Color<> GetBasicBG(void);
			uint32_t GetNuberOfColomn(void);
			bool GetTitle(int32_t colomn, etk::UString &myTitle, etk::Color<> &fg, etk::Color<> &bg);
			uint32_t GetNuberOfRaw(void);
			bool GetElement(int32_t colomn, int32_t raw, etk::UString &myTextToWrite, etk::Color<> &fg, etk::Color<> &bg);
			bool OnItemEvent(int32_t IdInput, ewol::keyEvent::status_te typeEvent, int32_t colomn, int32_t raw, float x, float y);
			// derived function
			const char * const GetObjectType(void) { return "TagFileList"; };
		public:
			/**
			 * @brief Add a Ctags item on the curent list
			 * @param[in] file Compleate file name
			 * @param[in] jump line id
			 */
			void Add(etk::UString& file, int32_t line);
	};
};



#endif


