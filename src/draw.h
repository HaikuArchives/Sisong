
#ifndef _DRAW_H
#define _DRAW_H

#include "FontDrawer.h"
#include "draw_cursor.h"
#include "LNPanel.h"
#include "spacer.h"
#include "ViewTimer.h"
#include "EditPane.h"
#include "OffscreenBuffer.h"

#define M_CURSOR_TIMER			'Tcrs'

#define GET_CURSOR_PX(cx)		((cx * editor.font_width) - (editor.curev ? editor.curev->xscroll : 0))
#define GET_CURSOR_PY(cy)		(cy * editor.font_height)

#endif
