/**
 *******************************************************************************
 * @file Display.h
 * @brief Editeur De N'ours : Basic Pixbuf display function (header)
 * @author Edouard DUPIN
 * @date 21/01/2010
 * @par Project
 * Edn
 *
 * @par Copyright
 * Copyright 2010 Edouard DUPIN, all right reserved
 *
 * This software is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY.
 *
 * Licence summary : 
 *    You can modify and redistribute the sources code and binaries.
 *    You can send me the bug-fix
 *    You can not earn money with this Software (if the source extract from Edn
 *        represent less than 50% of original Sources)
 * Term of the licence in in the file licence.txt.
 *
 *******************************************************************************
 */
 
#ifndef __TOOLS_DISPLAY_H__
#define __TOOLS_DISPLAY_H__

#include "tools_debug.h"
#include "Colorize.h"
#include "ColorizeManager.h"


/**
 *  Basic namespace for the font display system
 */
namespace Display
{
	void                    Init(void);
	void                    UnInit(void);
	cairo_font_face_t *     GetFont(bool bold, bool italic);
	int32_t                 GetFontHeight(void);
	int32_t                 GetFontWidth(void);
};

#define MAX_CARACTER_CYCLE		(512)
/**
 * class to abstrate the writing on the curent GUI (INTEFACE to be no dependent of the one chosen)
 */
class DrawerManager;

class DrawerManager {
	public:
		// Constructeur
		DrawerManager(GtkWidget * widget, int32_t x, int32_t y);
		~DrawerManager();

		void    Rectangle(Colorize *SelectColor, int32_t x, int32_t y, int32_t width, int32_t height);
		void    Rectangle(color_ts & SelectColor, int32_t x, int32_t y, int32_t width, int32_t height) {
			DirectRectangle(SelectColor, x, y, width, height);
		}
		void    Clean(Colorize *SelectColor);
		void    Clean(color_ts & SelectColor);
		void    Text(Colorize *SelectColor, int32_t x, int32_t y,const char *myUTF8Text, int32_t len = -1);
		void    Text(color_ts & SelectColorFg, color_ts & SelectColorBg, int32_t x, int32_t y,const char *myText);
		void    Text(color_ts & SelectColorFg, int32_t x, int32_t y,const char *myText);
		void    SpaceText(color_ts & SelectColor, int32_t x, int32_t y,int32_t nbChar);
		void    Cursor(int32_t x, int32_t y);
		void    EndOfLine(int32_t x, int32_t y);
		void    Tabulation(Colorize *SelectColor, int32_t x, int32_t y, int32_t mbColomn);
		void    UTF8UnknownElement(Colorize *SelectColor, int32_t x, int32_t y, int8_t utf8Size, bool ValidUtf8);
		void    Flush(void);
		int32_t GetWidth(void) { return m_size.x; };
		int32_t GetHeight(void) { return m_size.y; };

	private:
		void    DirectRectangle(Colorize *SelectColor, int32_t x, int32_t y, int32_t width, int32_t height);
		void    DirectRectangle(color_ts & SelectColor, int32_t x, int32_t y, int32_t width, int32_t height);

		bool            m_haveWork;                             //!< if data might be print (true)
		char            m_dataToDisplay[MAX_CARACTER_CYCLE];    //!< curent string to display
		uint32_t        m_nbElement;                            //!< nb element in the string
		position_ts     m_pos;                                  //!< position where start the string display (X,Y)
		Colorize *      m_selectColor;                          //!< curent color to display

		position_ts     m_size;                                 //!< Total size
		cairo_t *       m_cairo;                                //!< Cairo Layout pointer
		GdkWindow *     m_windows;                              //!< remember the current widget ==> for some internal problems
		cairo_t *       m_cairoWindows;                         //!< Cairo context
		cairo_surface_t *m_imageSurface;
};


#endif

