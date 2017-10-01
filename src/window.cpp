#include "window.h"
#include "graphics.h"
#include "event.h"
#include <iostream>

int Window::wincount = 0;

static Window* retrieve_window(GLFWwindow *glfwWin) {
	return static_cast<Window*>(glfwGetWindowUserPointer(glfwWin));
}

void Window::callback_framebuffer_size(GLFWwindow *glfwWin, int width, int height) {
	Window* window = retrieve_window(glfwWin);
	window->calculate_viewport(width, height);
}

void Window::callback_cursor_position(GLFWwindow* glfwWin, double xpos, double ypos) {
	Window* window = retrieve_window(glfwWin);
	
	Vec mouse(xpos, window->viewport_top - ypos);
    Vec delta = mouse - window->previous_mouse;
    window->previous_mouse = mouse;
	
	for(Control* c : window->children) {
		c->mouse_move(mouse - Vec(c->x, c->y), delta);
	}
}

void Window::callback_mouse_button(GLFWwindow *glfwWin, int button, int action, int mods) {
	Window* window = retrieve_window(glfwWin);
    
    MouseEvent event;
    if(button == GLFW_MOUSE_BUTTON_LEFT) event.button = LEFT;
    if(button == GLFW_MOUSE_BUTTON_MIDDLE) event.button = MIDDLE;
    if(button == GLFW_MOUSE_BUTTON_RIGHT) event.button = RIGHT;
    
    if(action == GLFW_PRESS) event.action = PRESS;
    if(action == GLFW_RELEASE) event.action = RELEASE;
    
    event.shift_down = mods & GLFW_MOD_SHIFT;
    event.alt_down = mods & GLFW_MOD_ALT;
    event.control_down = mods & GLFW_MOD_CONTROL;
    
	for(Control* c : window->children) {
		c->mouse_button(event);
	}
}

void Window::callback_key(GLFWwindow *glfwWin, int key, int scancode, int action, int mods) {
	Window* window = retrieve_window(glfwWin);
    
    KeyEvent event;
    event.key = (char)key;
    
    if(action == GLFW_PRESS) event.action = PRESS;
    if(action == GLFW_RELEASE) event.action = RELEASE;
    
    event.shift_down = mods & GLFW_MOD_SHIFT;
    event.alt_down = mods & GLFW_MOD_ALT;
    event.control_down = mods & GLFW_MOD_CONTROL;
    
    for(Control* c : window->children) {
        c->key(event);
    }
}

void Window::callback_scroll(GLFWwindow *glfwWin, double scrollx, double scrolly) {
	Window *window = retrieve_window(glfwWin);
	
	Vec scroll(scrollx, scrolly);
	
	for(Control* c : window->children) {
		c->scroll(scroll);
	}
}

Window::Window(int width, int height, const char *title) {	
	if (glfwInit()) {
		++wincount;
		
		glfwWin = glfwCreateWindow(width, height, title, NULL, NULL);
		if (!glfwWin) { glfwTerminate(); }
		
		glfwSetWindowUserPointer(glfwWin, this);
		
		glfwSetFramebufferSizeCallback(glfwWin, &Window::callback_framebuffer_size);
		glfwSetCursorPosCallback(glfwWin, &Window::callback_cursor_position);
		glfwSetMouseButtonCallback(glfwWin, &Window::callback_mouse_button);
		glfwSetScrollCallback(glfwWin, &Window::callback_scroll);
        glfwSetKeyCallback(glfwWin, &Window::callback_key);
		
		calculate_viewport(width, height);
        
        if(wincount == 1) {
            /* Setup of global OpenGL stuff */
            
            glEnable(GL_TEXTURE_2D);
            glEnable(GL_ALPHA_TEST);
            glAlphaFunc(GL_GREATER, 0);
            
           /* TODO: Get these to work */
           // glEnable(GL_POLYGON_SMOOTH);
           // glEnable(GL_BLEND);
           // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }
	}
}

void Window::calculate_viewport(int width, int height) {
	glfwMakeContextCurrent(glfwWin);
	
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, 0, height, -20, 20);
    
    glViewport(0, 0, width, height);
	
	viewport_right = width;
	viewport_top = height;
	
	for(Control* c : children) {
		if(c->sizer != NULL) {
			c->sizer->size(*c, width, height);
		}
	}
}

Vec Window::getMouse() {
	double xpos, ypos;
    glfwGetCursorPos(glfwWin, &xpos, &ypos);
	ypos = viewport_top - ypos;
	
	return Vec(xpos, ypos);
}

void Window::draw(Control& c) {
	glEnable(GL_SCISSOR_TEST);
	
	glScissor(c.x, c.y, c.width, c.height);
	
	Graphics gfx(Vec(c.x, c.y));
	c.render(gfx);
	
	glDisable(GL_SCISSOR_TEST);
}

void Window::attach(Control *c) {
    if(!c) return;
    
	if(c->sizer != NULL) {
		c->sizer->size(*c, viewport_right, viewport_top);
	}
	
    c->parent = this;
	children.push_back(c);
}

void Window::open() {
	if(!glfwWin) return;
	
	while (!glfwWindowShouldClose(glfwWin))
    {
		glfwMakeContextCurrent(glfwWin);
		
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		for(Control* child : children) {
			draw(*child);
		}
		
        /* Swap front and back buffers */
        glfwSwapBuffers(glfwWin);

        /* Poll for and process events */
        glfwPollEvents();
    }

	--wincount;
	if(!wincount) {
		glfwTerminate();
	}
}