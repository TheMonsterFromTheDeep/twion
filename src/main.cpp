#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <GLFW/glfw3.h>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "curve.h"
#include "cubic.h"

int mouse_top;

void resize(int width, int height) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, 0, height, -20, 20);
    
    glViewport(0, 0, width, height);
    
    mouse_top = height;
}

void on_resize(GLFWwindow *window, int width, int height) {
    resize(width, height);
}

void glVec(Vec v) {
    glVertex2f(v.x, v.y);
}

void line(Vec start, Vec end, float width) {
    glBegin(GL_QUADS);
     
    Vec delta = end - start;
    Vec orth = delta.ortho().scaled(width * 0.5f);
    
    glVec(start - orth);
    glVec(start + orth);
    glVec(end + orth);
    glVec(end - orth);
    
    glEnd();
}

void line1(Vec start, Vec end) {
    glBegin(GL_LINES);
    
    glVec(start);
    glVec(end);
    
    glEnd();
}

void stroke(Curve c) {
    if(c.size() < 2) return;
    
    glBegin(GL_QUAD_STRIP);
    
    {
        Vec delta = c[1].location - c[0].location;
        Vec orth = delta.ortho().scaled(c[0].width * 0.5f);
        
        glVec(c[0].location + orth);
        glVec(c[0].location - orth);
    }
    
    /* "Final index" - last point indexed; also happens to be number at far range of for loop */
    size_t fi = c.size() - 1;
    
    for(size_t i = 1; i < fi; ++i) {
        CurvePoint next = c[i + 1];
        CurvePoint previous = c[i - 1];
        
        size_t j = i + 1;
        
        Vec delta, orth, bi, proj;
        
        while((next.location - c[i].location).lensqr() < 0.1f) {
            if(j == fi) goto next_loop;
            next = c[j];
            ++j;
        }
        
        j = i - 1;
        
        while((previous.location - c[i].location).lensqr() < 0.1f) {
            if(j == 0) goto next_loop;
            next = c[j];
            --j;
        }
        
        delta = c[i].location - previous.location;
        
        orth = delta.ortho().scaled(c[i].width * 0.5f); 
        bi = delta.bisector(c[i].location - next.location);
        
        proj = bi * orth.lensqr() / orth.dot(bi);
        
        glVec(c[i].location + proj);
        glVec(c[i].location - proj);
        
        next_loop: { }
    }
    
    {
        Vec delta = c[fi].location - c[fi - 1].location;
        Vec orth = delta.ortho().scaled(c[fi].width * 0.5f);
        
        glVec(c[fi].location + orth);
        glVec(c[fi].location - orth);
    }
    
    glEnd();
}

bool mdown;

float scale_value = 0;
float scale_amount = 1;

double lx = 0, ly = 0;

float panx = 0, pany = 0;
bool panning = false;

int active_handle = 0;

InterpolatedCubic cubic(CurvePoint(100, 100, 5), CurvePoint(400, 400, 0), Vec(-100, 0), Vec(100, 0));

Curve curve;

float vel_w(float v) {
    return 0.5f + 1 * exp(-v / 20);
}

Vec getMouse(GLFWwindow *w) {
	double xpos, ypos;
    glfwGetCursorPos(w, &xpos, &ypos);
	ypos = mouse_top - ypos;
	
	return Vec(xpos, ypos);
}

Vec getCursor(GLFWwindow *w) {
	return (getMouse(w) - Vec(panx, pany)) / scale_amount;//(getMouse(w)) * scale_amount;
}

GLFWcursor *cursor_pan;
GLFWcursor *cursor_zoom;
GLFWcursor *cursor_default;
GLFWcursor *cursor_edit;

double scroll_previous = 0;

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    if(yoffset != 0) {
        scale_value += yoffset;
        
        float old_amount = scale_amount;
        
        scale_amount = pow(1.1, scale_value);
        
        old_amount = scale_amount / old_amount;
        
        Vec m = getMouse(window);
        
        panx -= m.x;
        pany -= m.y;
        
        panx *= old_amount;
        pany *= old_amount;
        
        panx += m.x;
        pany += m.y;
    }
}


static void cursor_moved(GLFWwindow* window, double xpos, double ypos) {
	Vec m = getMouse(window);
	
    lx -= m.x; ly -= m.y;
    //if(lx * lx + ly * ly > 0.05) { 
    //if(mdown) curve.push_back(CurvePoint((float)xpos, mouse_top - (float)ypos, vel_w(sqrt(lx * lx + ly * ly))));
    //}
    if(panning) {
        panx -= lx;
        pany -= ly;
    }
    
    lx = m.x; ly = m.y;
	
	Vec cursor = getCursor(window);
    
    switch(active_handle) {
		case 1:
			cubic.start.location = cursor;
			break;
		case 2:
			cubic.end.location = cursor;
			break;
		case 3:
			cubic.ease_in = cursor - cubic.start.location;
			break;
		case 4:
			cubic.ease_out = cursor - cubic.end.location;
			break;
		default: { }
	}
	
	if(active_handle) {
		cubic.calculate();
	
		curve = cubic.generate(0.01f);
	}
}

bool check(Vec location, Vec mouse) {
	return (location - mouse).lensqr() < 5 * 5;
}

void mouse_button(GLFWwindow* window, int button, int action, int mods) {
   /* if (button == GLFW_MOUSE_BUTTON_LEFT) {
        double xpos, ypos;
        
        glfwGetCursorPos(window, &xpos, &ypos);
        lx -= xpos; ly -= ypos;
        if(action == GLFW_PRESS) {
            mdown = true;
            //curve.push_back(CurvePoint((float)xpos, mouse_top - (float)ypos, vel_w(sqrt(lx * lx + ly * ly))));
        }
        if(action == GLFW_RELEASE) mdown = false;
        lx = xpos; ly = ypos;
    }  */
    
	Vec m = getMouse(window);
	
	
    if(button == GLFW_MOUSE_BUTTON_MIDDLE) {
        if(action == GLFW_PRESS) {
            glfwSetCursor(window, cursor_pan);
            panning = true;
        }
        if(action == GLFW_RELEASE) {
            glfwSetCursor(window, cursor_default);
            panning = false;
        }
    }
    
	if(button == GLFW_MOUSE_BUTTON_LEFT) {
		if(action == GLFW_PRESS) {
			Vec cursor = getCursor(window);
			
			     if(check(cubic.start.location,cursor)) active_handle = 1;
			else if(check(cubic.end.location, cursor)) active_handle = 2;
			else if(check(cubic.start.location + cubic.ease_in, cursor)) active_handle = 3;
			else if(check(cubic.end.location + cubic.ease_out, cursor)) active_handle = 4;
            
            if(active_handle) {
                glfwSetCursor(window, cursor_edit);
            }
		}
		
		if(action == GLFW_RELEASE) { active_handle = 0; glfwSetCursor(window, cursor_default); }
	}
	
    
    lx = m.x; ly = m.y;
}

void handle(Vec v, float r) {
	glColor3f(1.0f, 0.1f, 0.1f);
	
	glBegin(GL_LINE_LOOP);
	
	for(float t = 0; t < 6.28; t += 0.27) {
		glVec(v + Vec(r * cos(t), r * sin(t)));
	}
	
	glEnd();
	
}

void joint(Vec v, float r) {
	glColor3f(1.0f, 0.1f, 0.1f);
	
	glBegin(GL_TRIANGLE_FAN);
	
	glVec(v);
	
	for(float t = 0; t < 6.28; t += 0.27) {
		glVec(v + Vec(r * cos(t), r * sin(t)));
	}
	
	glVec(v + Vec(r, 0));
	
	glEnd();
}

void control_point(Vec v, float r) {
	glColor3f(0.0f, 0.0f, 1.0f);
	glBegin(GL_QUADS);
	glVec(v - Vec(r, 0));
	glVec(v - Vec(0, r));
	glVec(v + Vec(r, 0));
	glVec(v + Vec(0, r));
	glEnd();
}

void visualize(InterpolatedCubic ic) {
    glColor3f(1.0f, 0.1f, 0.1f);
    line1(ic.start.location, ic.start.location + ic.ease_in);
    line1(ic.end.location, ic.end.location + ic.ease_out);
	
	float sw = ic.start.width / sqrt(2);
	if(sw < 2) sw = 2;
	
	float ew = ic.end.width / sqrt(2);
	if(ew < 2) ew = 2;
	
	control_point(ic.start.location, sw);
	control_point(ic.end.location, ew);
	joint(ic.start.location, ic.start.width  * 0.5f);
	joint(ic.end.location, ic.end.width  * 0.5f);
	handle(ic.start.location + ic.ease_in, 2);
	handle(ic.end.location + ic.ease_out, 2);
}

void load_icon(GLFWwindow *window, const char *path) {
    GLFWimage image;
    int channels;
    image.pixels = stbi_load(path, &(image.width), &(image.height), &channels, 0);
    
    glfwSetWindowIcon(window, 1, &image);
}



GLFWcursor *load_cursor(const char *path, int hx, int hy) {
    GLFWimage image;
    int channels;
    image.pixels = stbi_load(path, &(image.width), &(image.height), &channels, 0);
    
    return glfwCreateCursor(&image, hx, hy);
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;
    
    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(mode->width / 2, mode->height / 2, "Twion | Untitled.twi", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    
    glfwSetFramebufferSizeCallback(window, &on_resize);
    
    glfwSetCursorPosCallback(window, &cursor_moved);
    glfwSetMouseButtonCallback(window, &mouse_button);
    glfwSetScrollCallback(window, &scroll_callback);
    
    //load_icon(window, "icon.png");
    
    resize(mode->width / 2, mode->height / 2);
	
    cubic.calculate();
	
	curve = cubic.generate(0.01f);
	
	glEnable(GL_POLYGON_SMOOTH);
	 
	cursor_pan = load_cursor("pan.png", 8, 8);
    cursor_zoom = load_cursor("zoom.png", 8, 8);
    cursor_default = load_cursor("default.png", 0, 0);
    cursor_edit = load_cursor("edit.png", 0, 0);
    
    glfwSetCursor(window, cursor_default);
		
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();
        
        glColor3f(0.93f, 0.93f, 0.93f);
        glBegin(GL_QUADS);
            glVertex2f(-1.0f, -1.0f);
            glVertex2f(-1.0f,  1.0f);
            glVertex2f( 1.0f,  1.0f);
            glVertex2f( 1.0f, -1.0f);
        glEnd();
        
        glPopMatrix();
        
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        
        glClear(GL_DEPTH_BUFFER_BIT);
        
        glTranslatef(panx, pany, 0);
        
        glScalef(scale_amount, scale_amount, 1);
		
		glColor3f(0.95f, 0.95f, 0.95f);
		glBegin(GL_QUADS);
			glVertex2f(0.0f, 0.0f);
			glVertex2f(0.0f, 360.0f);
			glVertex2f(480.0f, 360.0f);
			glVertex2f(480.0f, 0.0f);
		glEnd();
		
		glColor3f(0.9f, 0.9f, 0.9f);
		glBegin(GL_LINE_LOOP);
			glVertex2f(0.0f, 0.0f);
			glVertex2f(0.0f, 360.0f);
			glVertex2f(480.0f, 360.0f);
			glVertex2f(480.0f, 0.0f);
		glEnd();
		
		glColor3f(0.f, 0.f, 0.f);
        
        stroke(curve);
        
        
        
        glClear(GL_DEPTH_BUFFER_BIT);
        
        visualize(cubic);
		
		glClear(GL_DEPTH_BUFFER_BIT);
		
		glColor3f(0.f, 1.f, 0.f);
		//line(getCursor(window) - Vec(5, 0), getCursor(window) + Vec(5, 0),5);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}