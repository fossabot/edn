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
			appl::Buffer* m_buffer;
			dataBufferStruct(const etk::UString& _bufferName, appl::Buffer* _buffer) :
			  m_bufferName(_bufferName),
			  m_buffer(_buffer) {
				
			};
			~dataBufferStruct(void) { };
	};
};

class BufferView : public widget::List
{
	private:
		appl::BufferManager* m_bufferManager; //!< handle on the buffer manager
	private:
		appl::GlyphPainting* m_paintingProperties; //!< element painting property
		esize_t m_colorBackground1;
		esize_t m_colorBackground2;
		esize_t m_colorBackgroundSelect;
		esize_t m_colorTextNormal;
		esize_t m_colorTextModify;
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
		virtual void onObjectRemove(ewol::EObject* _removeObject);
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


