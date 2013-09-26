/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */

#ifndef __APPL_BUFFER_H__
#define __APPL_BUFFER_H__

#include <etk/UString.h>
#include <etk/os/FSNode.h>
#include <etk/unicode.h>
#include <ewol/ewol.h>
#include <etk/Buffer.h>
#include <ewol/renderer/EObject.h>
#include <ewol/widget/Widget.h>

namespace appl
{
	class Buffer : public ewol::EObject
	{
		public:
			Buffer(void);
			~Buffer(void) { };
		private:
			etk::UString m_fileName; //!< name of the file (with his path)
		public:
			/**
			 * @brief Get the curent filename of the Buffer
			 */
			const etk::UString& GetFileName(void) { return m_fileName; }
			bool LoadFile(const etk::UString& _name);
			void SetFileName(const etk::UString& _name);
			bool m_isModify; //!< true if the file is modify
			etk::Buffer m_data; //!< copy of the file buffer
		public:
			etk::Buffer& GetData(void) { return m_data; };
			/*
			appl::History m_history;
			Highlight m_highlight;
			ejson::Value* m_property;
			appl::Selection m_selection;
			*/
		public:
			esize_t m_cursorPos; //!< cursor position.
			bool OnEventEntry(const ewol::EventEntry& _event);
			/**
			 * @brief Get the next element in the buffer.
			 * @param[in] _pos Position in the buffer
			 * @param[out] _value Unicode value read in the buffer
			 * @param[in] _charset Charset used to parse the current buffer
			 * @return number ofelement read in the buffer (to increment the position)
			 */
			esize_t Get(esize_t _pos, etk::UniChar& _value, unicode::charset_te _charset) const;
	};
};


#endif
