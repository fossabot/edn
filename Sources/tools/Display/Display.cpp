/**
 *******************************************************************************
 * @file Display.cpp
 * @brief Editeur De N'ours : Basic Pixbuf display function
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

#include "tools_debug.h"
#include "tools_globals.h"
#include "Display.h"
#include "ColorizeManager.h"
#include "charset.h"
#include <string>

#undef __class__
#define __class__	"Display"


#define FONT_BOLD_NO	(0)
#define FONT_BOLD_YES	(1)
#define FONT_ITALIC_NO	(0)
#define FONT_ITALIC_YES	(1)


// Variables privé du namespace
#define		POLICE_NAME		"Monospace"

#ifdef USE_GTK_VERSION_3_0
	#define		POLICE_SIZE		15
	static int32_t				m_pangoFontWidth  = 9;
	static int32_t				m_pangoFontHeight = 19;
#elif USE_GTK_VERSION_2_0
	#define		POLICE_SIZE		12
	static int32_t				m_pangoFontWidth  = 7;
	static int32_t				m_pangoFontHeight = 15;
#endif
/*
	#define		POLICE_SIZE		11
	static int32_t				  m_pangoFontWidth  = 7;
	static int32_t				  m_pangoFontHeight = 11;
static cairo_font_face_t	* m_cairoFont[2][2] = {{NULL, NULL},{ NULL, NULL}};
*/
static cairo_font_face_t	* m_cairoFont[2][2] = {{NULL, NULL},{ NULL, NULL}};
void Display::Init(void)
{
	/*
	charWidth = gdk_char_width(myFont[FONT_ITALIC_YES][FONT_BOLD_NO], 'Z');
	EDN_INFO("Font Width = %d", charWidth);
	charHeignt = gdk_char_height(myFont[FONT_ITALIC_YES][FONT_BOLD_NO], 'Z');
	EDN_INFO("Font Height = %d", charHeignt);
	*/
	
	m_cairoFont[FONT_ITALIC_NO][FONT_BOLD_NO]		= cairo_toy_font_face_create(POLICE_NAME, CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
	m_cairoFont[FONT_ITALIC_YES][FONT_BOLD_NO]		= cairo_toy_font_face_create(POLICE_NAME, CAIRO_FONT_SLANT_ITALIC, CAIRO_FONT_WEIGHT_NORMAL);
	m_cairoFont[FONT_ITALIC_NO][FONT_BOLD_YES]		= cairo_toy_font_face_create(POLICE_NAME, CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
	m_cairoFont[FONT_ITALIC_YES][FONT_BOLD_YES]		= cairo_toy_font_face_create(POLICE_NAME, CAIRO_FONT_SLANT_ITALIC, CAIRO_FONT_WEIGHT_BOLD);
	if ( NULL == m_cairoFont[FONT_ITALIC_NO][FONT_BOLD_NO]) {
		EDN_ASSERT(FALSE, "basic font ERROR");
	}
	if ( NULL == m_cairoFont[FONT_ITALIC_YES][FONT_BOLD_NO]) {
		EDN_ERROR("Italic font error ... link with basic font");
		m_cairoFont[FONT_ITALIC_YES][FONT_BOLD_NO] = m_cairoFont[FONT_ITALIC_NO][FONT_BOLD_NO];
	}
	if ( NULL == m_cairoFont[FONT_ITALIC_NO][FONT_BOLD_YES]) {
		EDN_ERROR("Bold font error ... link with basic font");
		m_cairoFont[FONT_ITALIC_NO][FONT_BOLD_YES] = m_cairoFont[FONT_ITALIC_NO][FONT_BOLD_NO];
	}
	if ( NULL == m_cairoFont[FONT_ITALIC_YES][FONT_BOLD_YES]) {
		EDN_ERROR("Italic & Bold font error ... link with basic font");
		m_cairoFont[FONT_ITALIC_YES][FONT_BOLD_YES] = m_cairoFont[FONT_ITALIC_NO][FONT_BOLD_NO];
	}
}





void Display::UnInit(void)
{
	// clean the builder...
	free(m_cairoFont[FONT_ITALIC_NO][FONT_BOLD_NO]);
	free(m_cairoFont[FONT_ITALIC_YES][FONT_BOLD_NO]);
	free(m_cairoFont[FONT_ITALIC_NO][FONT_BOLD_YES]);
	free(m_cairoFont[FONT_ITALIC_YES][FONT_BOLD_YES]);
}


int32_t Display::GetFontHeight(void)
{
	return m_pangoFontHeight;
}

int32_t Display::GetFontWidth(void)
{
	return m_pangoFontWidth;
}



cairo_font_face_t * Display::GetFont(bool bold, bool italic)
{
    if(    false == bold
        && false == italic) {
        return m_cairoFont[FONT_ITALIC_NO][FONT_BOLD_NO];
    } else if(    true  == bold
               && false == italic) {
        return m_cairoFont[FONT_ITALIC_NO][FONT_BOLD_YES];
    } else if (   false == bold
               && true  == italic) {
        return m_cairoFont[FONT_ITALIC_YES][FONT_BOLD_NO];
    }
    return m_cairoFont[FONT_ITALIC_YES][FONT_BOLD_YES];
}


#undef __class__
#define __class__	"DrawerManager"

/**
 * @brief DrawerManager constructor : generate a memoryDC where we can draw everything...
 *
 * @param [in]		window			Current windows where the memoryDC must be paint
 * @param [in]		x				Current width of the Display
 * @param [in]		y				Current Height of the Display
 *
 * @note : No exeption
 *
 * @return ---
 *
 */
DrawerManager::DrawerManager(GtkWidget * widget, int32_t x, int32_t y)
{

	m_size.x = x;
	m_size.y = y;
	m_haveWork = false;
	// Create the Cairo Element
#   if USE_GTK_VERSION_3_0
	m_cairo = gdk_cairo_create(gtk_widget_get_window(widget));
#   elif USE_GTK_VERSION_2_0
	m_cairo = gdk_cairo_create(widget->window);
#   endif
	//cairo_translate(m_cairo, 0, 7);
	cairo_set_source_rgb(m_cairo, 0, 0, 0);
	cairo_paint(m_cairo);
	cairo_set_font_size(m_cairo, POLICE_SIZE);
	m_dataToDisplay[0] = '\0';
	
	cairo_scale(m_cairo, 1.0, 1.0);
	
	// http://cairographics.org/FAQ/#clear_a_surface
	// http://gtk.developpez.com/faq/?page=gtkwidget#GTK_WIDGET_transparent
	cairo_set_source_rgba(m_cairo, 0, 1, 1, 0);
	cairo_set_operator(m_cairo, CAIRO_OPERATOR_SOURCE);
	cairo_paint(m_cairo);
	//cairo_fill(m_cairo);
	//cairo_stroke (m_cairo);

	
	
	
}


/**
 * @brief main DrawerManager destructor : Copy data on the curent screen
 *
 * @param ---
 *
 * @note : No exeption
 *
 * @return ---
 *
 */
DrawerManager::~DrawerManager()
{
	cairo_destroy(m_cairo);
}


/**
 * @brief Draw Text with the specify color
 *
 *		This function does not display the text directly, it save it while a flush appare or the y position differ, or the color differ.
 *		This is for the char by char writing ==> more efficient when we write multiple char.
 *
 * @param [in]		SelectColor		Color that is used to display data
 * @param [in]		x				Horizontal position to display data
 * @param [in]		y				Vertical position to display data
 * @param [in]		myText			Text to write in UTF8 ...
 * @param [in]		displayBG		unused
 *
 * @return ---
 *
 */
void DrawerManager::Text(Colorize *SelectColor, int32_t x, int32_t y,const char *myUTF8Text, int32_t len)
{
	EDN_CHECK_INOUT(NULL!=SelectColor);
	// check if flush is needed : 
	if (true == m_haveWork) {
		if(		m_pos.y != y
			||	m_selectColor != SelectColor)
		{
			Flush();
		}
	}
	
	// check change
	if (false == m_haveWork) {
		m_pos.x = x;
		m_pos.y = y;
		m_selectColor = SelectColor;
	}
	//EDN_WARNING("add data : \"" << myText << "\" x=" << x << " y=" << y );
	// process : 
	m_haveWork = true;
	strcat(m_dataToDisplay, myUTF8Text);
	if (len != -1 ) {
		m_nbElement+=len;
	} else {
		m_nbElement+=strUtf8Len(myUTF8Text);
	}
}

void DrawerManager::Text(color_ts & SelectColorFg, color_ts & SelectColorBg, int32_t x, int32_t y,const char *myText)
{
	Flush();
	cairo_set_font_face(m_cairo, Display::GetFont(false, false));
	int32_t letterHeight = Display::GetFontHeight();
	int32_t letterWidth = Display::GetFontWidth();
	int32_t stringLen = strUtf8Len(myText);
	DirectRectangle(SelectColorBg, x, y, letterWidth*strlen(myText), letterHeight);
	cairo_fill(m_cairo);
	cairo_move_to(m_cairo, x, y+letterHeight-4);
	cairo_set_source_rgb(m_cairo, SelectColorFg.red, SelectColorFg.green, SelectColorFg.blue);
	cairo_show_text(m_cairo, myText);
	cairo_fill(m_cairo);
}

void DrawerManager::Text(color_ts & SelectColorFg, int32_t x, int32_t y,const char *myText)
{
	Flush();
	cairo_set_font_face(m_cairo, Display::GetFont(false, false));
	int32_t letterHeight = Display::GetFontHeight();
	cairo_move_to(m_cairo, x, y+letterHeight-4);
	cairo_set_source_rgb(m_cairo, SelectColorFg.red, SelectColorFg.green, SelectColorFg.blue);
	cairo_show_text(m_cairo, myText);
	cairo_fill(m_cairo);
	
}

void DrawerManager::SpaceText(color_ts & SelectColor, int32_t x, int32_t y,int32_t nbChar)
{
	Flush();
	int32_t letterHeight = Display::GetFontHeight();
	int32_t letterWidth = Display::GetFontWidth();
	DirectRectangle(SelectColor, x, y, letterWidth*nbChar, letterHeight);
	cairo_fill(m_cairo);
}


/**
 * @brief Force de display of the curent Text
 *
 *		The flush is to be more performent with the Pango display methode...
 *
 * @param ---
 *
 * @return ---
 *
 */
void DrawerManager::Flush(void)
{
	if (true == m_haveWork) {
		//EDN_WARNING("flush : \"" << m_dataToDisplay << "\"");
		m_haveWork = false;

		cairo_set_font_face(m_cairo, Display::GetFont(m_selectColor->GetBold(), m_selectColor->GetItalic()));
		int32_t letterHeight = Display::GetFontHeight();
		if (true == m_selectColor->HaveBg() ) {
			int32_t letterWidth = Display::GetFontWidth();
			int32_t stringLen = m_nbElement;
			// generate Clean BG:
			DirectRectangle(m_selectColor, m_pos.x, m_pos.y, letterWidth*stringLen, letterHeight);
		}
		cairo_move_to(m_cairo, m_pos.x, m_pos.y+letterHeight-4);
		m_selectColor->ApplyFG(m_cairo);
		cairo_show_text(m_cairo, m_dataToDisplay);
		cairo_fill(m_cairo);
		
		m_dataToDisplay[0] = '\0';
		m_nbElement = 0;
	}
}


/**
 * @brief display a rectangle with the curent bgColor and no border
 *
 *		This function in the open API to drow the rectangle, it flush the curent Text in the buffer
 *
 * @param [in]		SelectColor		Color that is used to display rectangle
 * @param [in]		x				Horizontal position to display rectangle
 * @param [in]		y				Vertical position to display rectangle
 * @param [in]		width			Width of the rectangle
 * @param [in]		height			Height of the rectangle
 *
 * @return ---
 *
 */
void DrawerManager::Rectangle(Colorize *SelectColor, int32_t x, int32_t y, int32_t width, int32_t height)
{
	Flush();
	DirectRectangle(SelectColor, x, y, width, height);
}


/**
 * @brief Real function to display the rectangle (no flush done (expectially for the Background display)
 *
 * @param [in]		SelectColor		Color that is used to display rectangle
 * @param [in]		x				Horizontal position to display rectangle
 * @param [in]		y				Vertical position to display rectangle
 * @param [in]		width			Width of the rectangle
 * @param [in]		height			Height of the rectangle
 *
 * @return ---
 *
 */
void DrawerManager::DirectRectangle(Colorize *SelectColor, int32_t x, int32_t y, int32_t width, int32_t height)
{
	EDN_CHECK_INOUT(NULL!=SelectColor);
	//EDN_INFO("x="<< x <<" y="<< y <<" width="<< width <<" height="<< height);
	//gdk_draw_rectangle(	p_pixmap, SelectColor->GetColorBG(), TRUE, x, y, width, height);
	// set color
	SelectColor->ApplyBG(m_cairo);
	// set postion
	cairo_rectangle(m_cairo, x, y, width, height);
	//cairo_stroke(m_cairo);
	// flush
	cairo_fill(m_cairo);
}
void DrawerManager::DirectRectangle(color_ts &SelectColor, int32_t x, int32_t y, int32_t width, int32_t height)
{

	cairo_set_source_rgb(m_cairo, SelectColor.red, SelectColor.green, SelectColor.blue);
	// set postion
	cairo_rectangle(m_cairo, x, y, width, height);
	//cairo_stroke(m_cairo);
	// flush
	cairo_fill(m_cairo);
}


/**
 * @brief Clean the curent Windows with the curent color
 *
 * @param [in]		SelectColor		Color that is used to display background
 *
 * @return ---
 *
 */
void DrawerManager::Clean(Colorize *SelectColor)
{
	m_haveWork = false;
	DirectRectangle(SelectColor, 0, 0, m_size.x, m_size.y);
}

/**
 * @brief Clean the curent Windows with the curent color
 *
 * @param [in]		SelectColor		Color that is used to display background
 *
 * @return ---
 *
 */
void DrawerManager::Clean(color_ts & SelectColor)
{
	m_haveWork = false;
	DirectRectangle(SelectColor, 0, 0, m_size.x, m_size.y);
}

/**
 * @brief Display a cursor with the cortect form.
 *
 *		Automatic selection of the XML color "cursorColor"
 *
 * @param [in]		x				Horizontal position to display cursor
 * @param [in]		y				Vertical position to display cursor
 *
 * @return ---
 *
 */
#define CURSOR_WIDTH	(4)
void DrawerManager::Cursor(int32_t x, int32_t y)
{
	Flush();
	// get the cursor Color : 
	color_ts myColor = ColorizeManager::getInstance()->Get(COLOR_CODE_CURSOR);
	cairo_set_source_rgb(m_cairo, myColor.red, myColor.green, myColor.blue);

	// draw cursor
	int32_t letterHeight = Display::GetFontHeight();
	int32_t letterWidth = Display::GetFontWidth();
	// depending on the inserting mode
	if (false == globals::IsSetInsert()) {
		cairo_set_line_width(m_cairo, 2);
		cairo_move_to(m_cairo, x-CURSOR_WIDTH, y - letterHeight+1);
		cairo_rel_line_to(m_cairo, CURSOR_WIDTH*2, 0);
		cairo_rel_move_to(m_cairo, -CURSOR_WIDTH, 0);
		cairo_rel_line_to(m_cairo, 0, letterHeight-2);
		cairo_rel_move_to(m_cairo, -CURSOR_WIDTH, 0);
		cairo_rel_line_to(m_cairo, CURSOR_WIDTH*2, 0);
	} else {
		cairo_set_line_width(m_cairo, 1);
		cairo_move_to(m_cairo, x, y - letterHeight  + 1);
		cairo_rel_line_to(m_cairo, letterWidth, 0);
		cairo_rel_line_to(m_cairo, 0, letterHeight);
		cairo_rel_line_to(m_cairo, -letterWidth, 0);
		cairo_rel_line_to(m_cairo, 0, -letterHeight);
	}
	cairo_stroke(m_cairo);
	cairo_fill(m_cairo);
}

/**
 * @brief Display a end of the curent line ...
 *
 *		Automatic draw the end of line with the curent XML color "cursorColor"
 *
 * @param [in]		x				Horizontal position to display cursor
 * @param [in]		y				Vertical position to display cursor
 *
 * @return ---
 *
 */
void DrawerManager::EndOfLine(int32_t x, int32_t y)
{
	if (true == globals::IsSetDisplayEndOfLine() ) {
		Flush();
		// get the cursor Color : 
		color_ts myColor = ColorizeManager::getInstance()->Get(COLOR_CODE_CURSOR);
		cairo_set_source_rgb(m_cairo, myColor.red, myColor.green, myColor.blue);
		// draw cursor
		int32_t letterHeight = Display::GetFontHeight();
		// depending on the inserting mode
		/*
				x1		x2
			y1	| |-----
				| |-----
				| |
			y2	| |
		*/
		cairo_set_line_width(m_cairo, 2);
		cairo_move_to(m_cairo, x, y - letterHeight+1);
		cairo_rel_line_to(m_cairo, 0, letterHeight);
		cairo_move_to(m_cairo, x+2, y - letterHeight+1);
		cairo_rel_line_to(m_cairo, 0, letterHeight);
		cairo_arc(m_cairo, x+3, y - letterHeight/4*3, 3.0, -3.149/2, 3.149/2);
		cairo_stroke(m_cairo);
		cairo_fill(m_cairo);
	}
}



/**
 * @brief Display a Tabulation with the user form selection.
 *
 * @param [in]		SelectColor		Color that is used to display Tabulation
 * @param [in]		x				Horizontal position to display cursor
 * @param [in]		y				Vertical position to display cursor
 * @param [in]		mbColomn		Width of the current Tabulation caracter (in number of Char)
 *
 * @return ---
 *
 */
void DrawerManager::Tabulation(Colorize *SelectColor, int32_t x, int32_t y, int32_t mbColomn)
{
	Flush();
	int32_t letterWidth = Display::GetFontWidth();
	int32_t letterHeight = Display::GetFontHeight();
	
	// generate Clean BG:
	DirectRectangle(SelectColor, x, y-letterHeight, letterWidth*mbColomn, letterHeight);
	
	// set the pen for the display
	SelectColor->ApplyFG(m_cairo);
	
	uint32_t yCalc = y - (letterHeight+1)/2;
	
	cairo_move_to(m_cairo, x + 3, yCalc);
	cairo_line_to(m_cairo, x + letterWidth*mbColomn - 2 , yCalc);
	cairo_rel_line_to(m_cairo, -6 , -2);
	cairo_move_to(m_cairo, x + letterWidth*mbColomn - 2 , yCalc);
	cairo_rel_line_to(m_cairo, -6 , +2);
	
	cairo_stroke(m_cairo);
	cairo_fill(m_cairo);
}


/**
 * @brief Display an unknow UTF8 character (a special rectangle)
 *
 * @param [in]		SelectColor			Color that is used to display Tabulation
 * @param [in]		x					Horizontal position to display cursor
 * @param [in]		y					Vertical position to display cursor
 * @param [in]		utf8Size			number of char used by the UTF8 character
 * @param [in]		ValidUtf8			true if the character UTF8 is well encoded
 *
 * @return ---
 *
 */
void DrawerManager::UTF8UnknownElement(Colorize *SelectColor, int32_t x, int32_t y, int8_t utf8Size, bool ValidUtf8)
{
	Flush();
	int32_t letterWidth = Display::GetFontWidth();
	int32_t letterHeight = Display::GetFontHeight();
	
	// generate Clean BG:
	DirectRectangle(SelectColor, x, y-letterHeight, letterWidth*2, letterHeight);
	
	SelectColor->ApplyFG(m_cairo);
	
	/*
			y1		y2
		x1	|-------|
			|		|
			|		|
		x2	|-------|
	*/
	uint32_t x1 = x                 + 2;
	uint32_t x2 = x + letterWidth*2 - 2;
	uint32_t y1 = y - letterHeight  + 2;
	uint32_t y2 = y                 - 2;
	// Box
	cairo_move_to(m_cairo, x1 , y1);
	cairo_line_to(m_cairo, x2 , y1);
	cairo_line_to(m_cairo, x2 , y2);
	cairo_line_to(m_cairo, x1 , y2);
	cairo_line_to(m_cairo, x1 , y1);
	// croix in the middle
	switch(utf8Size)
	{
		case 1:
			break;
		case 2:
			cairo_line_to(m_cairo, x2 , y2);
			break;
		case 3:
			cairo_move_to(m_cairo, x1 , y2);
			cairo_line_to(m_cairo, x2 , y1);
			break;
		default:
			cairo_line_to(m_cairo, x2 , y2);
			cairo_move_to(m_cairo, x1 , y2);
			cairo_line_to(m_cairo, x2 , y1);
			break;
	}
	if (false == ValidUtf8) {
		cairo_move_to(m_cairo, x1 , y2-2);
		cairo_line_to(m_cairo, x2 , y2-2);
	}
	cairo_stroke(m_cairo);
	cairo_fill(m_cairo);
}




/* Basic axample with cairo and pango...
	cairo_t *cr;

	cr = gdk_cairo_create(widget->window);

	cairo_translate(cr, 0, 7);

	cairo_set_source_rgb(cr, 0, 0, 0);
	cairo_paint(cr);

	gint pos = 18;//GTK_CPU(widget)->sel;
	gint rect = pos / 5;

	cairo_set_source_rgb(cr, 0.2, 0.4, 0);

	gint i;
	for ( i = 1; i <= 20; i++) {
		if (i > 20 - rect) {
			cairo_set_source_rgb(cr, 0.6, 1.0, 0);
		} else {
			cairo_set_source_rgb(cr, 0.2, 0.4, 0);
		}
		cairo_rectangle(cr, 8, i*4, 30, 3);
		cairo_rectangle(cr, 42, i*4, 30, 3);
		cairo_fill(cr);
	}
//----
	PangoLayout *layout;
	PangoFontDescription *desc;

	cairo_translate(cr, 10, 20);
	layout = pango_cairo_create_layout(cr);
	pango_layout_set_text(layout, "Hello World!", -1);
	desc = pango_font_description_from_string("Sans Bold 12");
	pango_layout_set_font_description(layout, desc);
	pango_font_description_free(desc);

	cairo_set_source_rgb(cr, 0.0, 0.0, 1.0);
	pango_cairo_update_layout(cr, layout);
	pango_cairo_show_layout(cr, layout);

	g_object_unref(layout);
//-----
	cairo_destroy(cr);
*/