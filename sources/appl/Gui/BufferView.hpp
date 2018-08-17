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
		void init() override;
	public:
		DECLARE_FACTORY(BufferView);
		virtual ~BufferView();
	private:
		bool m_openOrderMode; //!< true if the order is the opening order mode, otherwise, Alphabetic order
	protected:
		// function call to display the list :
		etk::Color<> getBasicBG() override;
		void removeAllElement();
		// Derived function
		bool getTitle(int32_t _colomn, etk::String& _myTitle, etk::Color<>& _fg, etk::Color<>& _bg) override;
		ivec2 getMatrixSize() const override;
		fluorine::Variant getData(int32_t _role, const ivec2& _pos) override;
		bool onItemEvent(int32_t _IdInput, enum gale::key::status _typeEvent, const ivec2& _pos, const vec2& _mousePosition) override;
	private: //callback function:
		void onCallbackChangeName();
		void onCallbackIsSave();
		void onCallbackIsModify();
		void onCallbackNewBuffer(const etk::String& _value);
		void onCallbackselectNewFile(const etk::String& _value);
		void onCallbackBufferRemoved(const ememory::SharedPtr<appl::Buffer>& _buffer);
		void calculateMinMaxSize() override;
};

