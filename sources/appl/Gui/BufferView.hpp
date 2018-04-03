/** @file
 * @author Edouard DUPIN
 * @copyright 2010, Edouard DUPIN, all right reserved
 * @license GPL v3 (see license file)
 */
#pragma once

#include <appl/debug.hpp>
#include <appl/BufferManager.hpp>
#include <appl/globalMsg.hpp>
#include <ewol/widget/List.hpp>
#include <ewol/widget/Windows.hpp>

namespace appl {
	class dataBufferStruct {
		public:
			etk::FSNode m_bufferName;
			ememory::SharedPtr<appl::Buffer> m_buffer;
			dataBufferStruct() {
				
			};
			dataBufferStruct(const etk::String& _bufferName, const ememory::SharedPtr<appl::Buffer>& _buffer) :
			  m_bufferName(_bufferName),
			  m_buffer(_buffer) {
				
			};
			virtual ~dataBufferStruct() { };
	};
};

	class BufferView;
	using BufferViewShared = ememory::SharedPtr<BufferView>;
	using BufferViewWeak = ememory::WeakPtr<BufferView>;
class BufferView : public ewol::widget::List {
	private:
		ememory::SharedPtr<appl::BufferManager> m_bufferManager; //!< handle on the buffer manager
	private:
		ememory::SharedPtr<appl::GlyphPainting> m_paintingProperties; //!< element painting property
		int32_t m_colorBackground1;
		int32_t m_colorBackground2;
		int32_t m_colorBackgroundSelect;
		int32_t m_colorTextNormal;
		int32_t m_colorTextModify;
	private:
		int32_t m_selectedIdRequested;
		int32_t m_selectedID;
		etk::Vector<appl::dataBufferStruct> m_list;
		/**
		 * @brief Insert the element in the alphabetic order.
		 * @param[in] _dataStruct element to add.
		 */
		void insertAlphabetic(const appl::dataBufferStruct& _dataStruct, bool _selectNewPosition = false);
	protected:
		// Constructeur
		BufferView();
		void init();
	public:
		DECLARE_FACTORY(BufferView);
		virtual ~BufferView();
	private:
		bool m_openOrderMode; //!< true if the order is the opening order mode, otherwise, Alphabetic order
	protected:
		// function call to display the list :
		virtual etk::Color<> getBasicBG();
		void removeAllElement();
		// Derived function
		virtual uint32_t getNuberOfColomn();
		virtual bool getTitle(int32_t _colomn, etk::String& _myTitle, etk::Color<>& _fg, etk::Color<>& _bg);
		virtual uint32_t getNuberOfRaw();
		virtual bool getElement(int32_t _colomn, int32_t _raw, etk::String& _myTextToWrite, etk::Color<>& _fg, etk::Color<>& _bg);
		virtual bool onItemEvent(int32_t _IdInput, enum gale::key::status _typeEvent, int32_t _colomn, int32_t _raw, float _x, float _y);
	private: //callback function:
		void onCallbackChangeName();
		void onCallbackIsSave();
		void onCallbackIsModify();
		void onCallbackNewBuffer(const etk::String& _value);
		void onCallbackselectNewFile(const etk::String& _value);
		void onCallbackBufferRemoved(const ememory::SharedPtr<appl::Buffer>& _buffer);
		void calculateMinMaxSize();
};

