/** @file
 * @author Edouard DUPIN
 * @copyright 2010, Edouard DUPIN, all right reserved
 * @license GPL v3 (see license file)
 */
#ifndef __EWOL_WIDGET_H__
#define __EWOL_WIDGET_H__

#include <ewol/object/Object.h>
#include <ewol/Dimension.h>
'dfgd\'fg'
'e' sdfsdf '\e'

"dqf\"gsdfg" // \\ \n "
// TODO : sqdkfjsdldkqfj


example_param

=_param

_
_s
_3RTDRsdfqsd

m_

m_d6


namespace ewol {
	class Widget;
	namespace widget {
		class Manager;
		class Windows;
	};
};
#include <etk/types.h>
#include <vector>
#include <etk/math/Vector2D.h>
#include <ewol/debug.h>
#include <ewol/context/clipBoard.h>
#include <ewol/key/key.h>
#include <ewol/context/cursor.h>
#include <ewol/event/Input.h>
#include <ewol/event/Entry.h>
#include <ewol/event/Time.h>

#define ULTIMATE_MAX_SIZE  (99999999)

namespace ewol {
#if 0
	/**
	 * @not_in_doc
	 */
#endif
	class DrawProperty{
		/*
		                                                          /--> m_windowsSize
		      *--------------------------------------------------*
		      |                                           g       |
		      |                                                  |
		      |                                    m_size        |
		      |                                   /              |
		      |              o-------------------o               |
		      |              |                   |               |
		      |              |                   |               |
		      |              |                   |               |
		      |              |                   |               |
		      |              |                   |               |
		      |              |                   |               |
		      |              |                   |               |
		      |              |                   |               |
		      |              o-------------------o               |
		      |             /                                    |
		      |     m_origin                                     |
		      |                                                  |
		      *--------------------------------------------------*
		     /
		   (0,0)
		 */
		public :
			ivec2 m_windowsSize; //!< Windows compleate size
			ivec2 m_origin; //!< Windows clipping upper widget (can not be <0)
			ivec2 m_size; //!< Windows clipping upper widget (can not be <0 and >m_windowsSize)
			void limit(const vec2& _origin, const vec2& _size);
	};
	std::ostream& operator <<(std::ostream& _os, const ewol::DrawProperty& _obj);
	/**
	 * @brief Gravity of the widget property
	 * @not_in_doc
	 */
	enum gravity {
		gravityCenter=0x00, //!< gravity is in certer
		gravityTopLeft=0x05,
		gravityTop=0x01,
		gravityTopRight=0x03,
		gravityRight=0x02,
		gravityButtomRight=0x06,
		gravityButtom=0x04,
		gravityButtomLeft=0x0C,
		gravityLeft=0x08,
	};
	std::ostream& operator <<(std::ostream& _os, const enum ewol::gravity _obj);
	std::string gravityToString(const enum ewol::gravity _obj);
	enum ewol::gravity stringToGravity(const std::string& _obj);
	/**
	 * @not_in_doc
	 */
	class EventShortCut {
		public:
			bool broadcastEvent; //!< if it is true, then the message is sent to all the system
			const char* generateEventId; //!< Local generated event
			std::string eventData; //!< data link with the event
			ewol::key::Special specialKey; //!< special board key
			char32_t unicodeValue; //!< 0 if not used
			enum ewol::key::keyboard keyboardMoveValue; //!< ewol::EVENT_KB_MOVE_TYPE_NONE if not used
			EventShortCut(void) {
				broadcastEvent = false;
				generateEventId = NULL;
				eventData = "";
				unicodeValue = 0;
				keyboardMoveValue = ewol::key::keyboardUnknow;
			};
			~EventShortCut(void) { };
	};
	/**
	 * @brief Widget class is the main widget interface, it hase some generic properties: 
	 * :** known his parent
	 * :** Can be display at a special position with a special scale
	 * :** Can get focus
	 * :** Receive Event (keyboard / mouse / ...)
	 * 
	 */
	class Widget : public ewol::Object {
		public:
			// Config list of properties
			static const char* const configFill;
			static const char* const configExpand;
			static const char* const configHide;
			static const char* const configFocus;
			static const char* const configMinSize;
			static const char* const configMaxSize;
			static const char* const configGravity;
		public:
			/**
			 * @brief Constructor of the widget classes
			 * @return (no execption generated (not managed in embended platform))
			 */
			Widget(void);
			/**
			 * @brief Destructor of the widget classes
			 */
			virtual ~Widget(void) {
				vec2 plop = vec2(15.2, 56.6f);
			}
	};
};


#endif
