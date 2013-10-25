/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */

#ifndef __BUFFER_VIEW_H__
#define __BUFFER_VIEW_H__

#include <appl/debug.h>
#include <BufferManager.h>
#include <appl/globalMsg.h>
#include <ewol/widget/List.h>
#include <ewol/widget/Windows.h>

namespace appl
{
	class dataBufferStruct
	{
		public:
			etk::FSNode m_bufferName;
			uint32_t    m_bufferID;
			bool        m_isModify;
			dataBufferStruct(etk::FSNode& _bufferName, int32_t _bufferID, bool _isModify) :
			  m_bufferName(_bufferName),
			  m_bufferID(_bufferID),
			  m_isModify(_isModify) {
				
			};
			~dataBufferStruct(void) { };
	};
};

class BufferView : public widget::List
{
	private:
		int32_t m_selectedIdRequested;
		int32_t m_selectedID;
		etk::Vector<appl::dataBufferStruct*> m_list;
	public:
		// Constructeur
		BufferView(void);
		~BufferView(void);
		// Derived function
		const char * const getObjectType(void) { return "ApplBufferView"; };
		// Derived function
		virtual void onReceiveMessage(const ewol::EMessage& _msg);
	protected:
		// function call to display the list :
		virtual etk::Color<> getBasicBG(void);
		void removeAllElement(void);
		// Derived function
		virtual uint32_t getNuberOfColomn(void);
		virtual bool getTitle(int32_t _colomn, etk::UString& _myTitle, etk::Color<>& _fg, etk::Color<>& _bg);
		virtual uint32_t getNuberOfRaw(void);
		virtual bool getElement(int32_t _colomn, int32_t _raw, etk::UString& _myTextToWrite, etk::Color<>& _fg, etk::Color<>& _bg);
		virtual bool onItemEvent(int32_t _IdInput, ewol::keyEvent::status_te _typeEvent, int32_t _colomn, int32_t _raw, float _x, float _y);
};


#endif


