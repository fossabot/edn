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
#include <appl/BufferManager.h>
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
			dataBufferStruct(const std::string& _bufferName, appl::Buffer* _buffer) :
			  m_bufferName(_bufferName),
			  m_buffer(_buffer) {
				
			};
			~dataBufferStruct() { };
	};
};

class BufferView : public ewol::widget::List
{
	private:
		appl::BufferManager* m_bufferManager; //!< handle on the buffer manager
	private:
		appl::GlyphPainting* m_paintingProperties; //!< element painting property
		int32_t m_colorBackground1;
		int32_t m_colorBackground2;
		int32_t m_colorBackgroundSelect;
		int32_t m_colorTextNormal;
		int32_t m_colorTextModify;
	private:
		int32_t m_selectedIdRequested;
		int32_t m_selectedID;
		std::vector<appl::dataBufferStruct*> m_list;
		/**
		 * @brief Insert the element in the alphabetic order.
		 * @param[in] _dataStruct element to add.
		 */
		void insertAlphabetic(appl::dataBufferStruct* _dataStruct, bool _selectNewPosition = false);
	public:
		// Constructeur
		BufferView();
		~BufferView();
		// Derived function
		virtual void onReceiveMessage(const ewol::object::Message& _msg);
		virtual void onObjectRemove(ewol::Object* _removeObject);
	private:
		bool m_openOrderMode; //!< true if the order is the opening order mode, otherwise, Alphabetic order
	protected:
		// function call to display the list :
		virtual etk::Color<> getBasicBG();
		void removeAllElement();
		// Derived function
		virtual uint32_t getNuberOfColomn();
		virtual bool getTitle(int32_t _colomn, std::string& _myTitle, etk::Color<>& _fg, etk::Color<>& _bg);
		virtual uint32_t getNuberOfRaw();
		virtual bool getElement(int32_t _colomn, int32_t _raw, std::string& _myTextToWrite, etk::Color<>& _fg, etk::Color<>& _bg);
		virtual bool onItemEvent(int32_t _IdInput, enum ewol::key::status _typeEvent, int32_t _colomn, int32_t _raw, float _x, float _y);
};


#endif


