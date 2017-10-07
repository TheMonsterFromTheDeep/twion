#ifndef CURSOR_H
#define CURSOR_H

enum Cursor {
    CURSOR_DEFAULT,
    CURSOR_GRAB,
    CURSOR_ROTATE,
    CURSOR_SCALE,
    CURSOR_THICKEN
};

void init_cursors();
void set_cursor(Cursor);

/* TODO: Don't use void pointer */
void set_cursor_window(void*);

#endif