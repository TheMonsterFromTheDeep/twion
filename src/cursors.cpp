#include "cursors.h"

#include "stb_image.h"

#include <GLFW/glfw3.h>
#include <iostream>

GLFWcursor** cursors;

GLFWwindow *window = NULL;

static GLFWcursor *load_cursor_image(const char *path, int hx, int hy) {
    GLFWimage image;
    int channels;
    image.pixels = stbi_load(path, &(image.width), &(image.height), &channels, 0);
    
    return glfwCreateCursor(&image, hx, hy);
}

void init_cursors() {
    if(glfwInit()) {
        /* TODO: Figure out some way to make sure that this is always the right size */
        cursors = new GLFWcursor*[5];
        
        /* TODO: Handle missing images */
        cursors[CURSOR_DEFAULT] = load_cursor_image("default.png", 8, 8);
        cursors[CURSOR_GRAB] = load_cursor_image("grab.png", 8, 8);
        cursors[CURSOR_ROTATE] = load_cursor_image("rotate.png", 8, 8);
        cursors[CURSOR_SCALE] = load_cursor_image("scale.png", 8, 8);
        cursors[CURSOR_THICKEN] = load_cursor_image("thicken.png", 8, 8);
    }
}

void set_cursor(Cursor c) {
    if(window) glfwSetCursor(window, cursors[c]);
}

void set_cursor_window(void* win) {
    window = static_cast<GLFWwindow*>(win);
}