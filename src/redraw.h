#ifndef REDRAW_H
#define REDRAW_H

void rd_clear_dirty_bits(EditView *ev);
void rd_invalidate_all(EditView *ev);
void rd_invalidate_line(EditView *ev, int y);
void rd_invalidate_range(EditView *ev, int y1, int y2);
void rd_invalidate_range_exclusive(EditView *ev, int y1, int y2);
void rd_invalidate_current_line(EditView *ev);
void rd_invalidate_below(EditView *ev, int y);
void rd_invalidate_above(EditView *ev, int y);
void rd_invalidate_selection(EditView *ev);
bool rd_is_screenline_dirty(int y);
bool rd_is_absline_dirty(EditView *ev, int y);
char rd_fullredraw();

#endif // REDRAW_H
