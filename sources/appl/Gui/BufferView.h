/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */

#ifndef __BUFFER_VIEW_H__
#define __BUFFER_VIEW_H__

#include <appl/Debug.h>
#include <CodeView.h>
#include <BufferManager.h>
#include <appl/globalMsg.h>
#include <ewol/widget/List.h>

namespace appl
{
	class dataBufferStruct
	{
		public:
			etk::FSNode m_bufferName;
			uint32_t    m_bufferID;
			bool        m_isModify;
			dataBufferStruct(etk::FSNode& bufferName, int32_t bufferID, bool isModify) :
				m_bufferName(bufferName),
				m_bufferID(bufferID),
				m_isModify(isModify)
			{
				
			};
			~dataBufferStruct(void) { };
	};
};

class BufferView : public widget::List
{
	private:
		int32_t                               m_selectedIdRequested;
		int32_t                               m_selectedID;
		etk::Vector<appl::dataBufferStruct*>  m_list;
	public:
		// Constructeur
		BufferView(void);
		~BufferView(void);
		// Derived function
		const char * const GetObjectType(void) { return "ApplBufferView"; };
		// Derived function
		virtual void OnReceiveMessage(ewol::EObject * CallerObject, const char * eventId, etk::UString data);
	protected:
		// function call to display the list :
		virtual draw::Color GetBasicBG(void);
		// Derived function
		virtual uint32_t GetNuberOfColomn(void);
		// Derived function
		virtual bool GetTitle(int32_t colomn, etk::UString &myTitle, draw::Color &fg, draw::Color &bg);
		// Derived function
		virtual uint32_t GetNuberOfRaw(void);
		// Derived function
		virtual bool GetElement(int32_t colomn, int32_t raw, etk::UString &myTextToWrite, draw::Color &fg, draw::Color &bg);
		// Derived function
		virtual bool OnItemEvent(int32_t IdInput, ewol::keyEvent::status_te typeEvent, int32_t colomn, int32_t raw, float x, float y);
		void RemoveAllElement(void);
};


#endif


