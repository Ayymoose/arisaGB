#include "rom.hpp"
#include "gb.hpp"

#include <iostream>
#include <GL/glut.h>

// Display size
#define SCREEN_WIDTH 160
#define SCREEN_HEIGHT 144

#define RED 0
#define GREEN 1
#define BLUE 2
#define RGB 3

// Window size
int display_width = SCREEN_WIDTH * 1;
int display_height = SCREEN_HEIGHT * 1;

unsigned char screen[SCREEN_HEIGHT][SCREEN_WIDTH][RGB];
unsigned char *data;//[SCREEN_WIDTH*SCREEN_HEIGHT] = {0x00};

// Setup Texture
void setup_texture()
{
    // Clear screen
    for(int y = 0; y < SCREEN_HEIGHT; ++y) {
        for(int x = 0; x < SCREEN_WIDTH; ++x) {
            screen[y][x][RED] = 0;
            screen[y][x][GREEN] = 0;
            screen[y][x][BLUE] = 0;
        }
    }
    // Create a texture
    glTexImage2D(GL_TEXTURE_2D, 0, 3, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, (GLvoid*)screen);

    // ??
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    // Enable textures
    glEnable(GL_TEXTURE_2D);
}

void update_texture() {
    // Update pixels
    for(int y = 0; y < SCREEN_HEIGHT; ++y) {
        for(int x = 0; x < SCREEN_WIDTH; ++x) {
        	int colour_data = data[SCREEN_WIDTH * y + x];
        	int colour;
        	switch (colour_data) {
				case 0x03: colour = 0; break;
				case 0x02: colour = 96; break;
				case 0x01: colour = 192; break;
				case 0x00: colour = 255; break;
        	default: colour = 0;
        	}
			screen[y][x][RED] = colour;
			screen[y][x][GREEN] = colour;
			screen[y][x][BLUE] = colour;
        }
    }
    // Update Texture
    glTexSubImage2D(GL_TEXTURE_2D, 0 ,0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, (GLvoid*)screen);

    glBegin(GL_QUADS );
        glTexCoord2d(0.0, 0.0);		glVertex2d(0.0,0.0);
        glTexCoord2d(1.0, 0.0); 	glVertex2d(display_width, 0.0);
        glTexCoord2d(1.0, 1.0); 	glVertex2d(display_width, display_height);
        glTexCoord2d(0.0, 1.0); 	glVertex2d(0.0, display_height);
    glEnd();
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	update_texture();
	glutSwapBuffers();
}

void reshape_window(GLsizei w, GLsizei h)
{
    glClearColor(0.0f, 0.0f, 0.5f, 0.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, w, h, 0);
    glMatrixMode(GL_MODELVIEW);
    glViewport(0, 0, w, h);

    // Resize quad
    display_width = w;
    display_height = h;
}

int main(int argc, char **argv)
{

    // Setup OpenGL
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

    glutInitWindowSize(display_width, display_height);
    glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH)-SCREEN_WIDTH)/2,
                           (glutGet(GLUT_SCREEN_HEIGHT)-SCREEN_HEIGHT)/2);
    glutCreateWindow("ArisaGB");


	rom r;
	gb gameboy;
	try {

		r.load_rom("/home/ayman/Desktop/arisaGB/rom/opus5.gb");
		gameboy.load_rom(r);

	} catch (const char *msg) {
		std::cout << msg << std::endl;
	}

	gameboy.set_context(&data);
	gameboy.start();



    glutDisplayFunc(display);
    glutIdleFunc(display);
    glutReshapeFunc(reshape_window);

    setup_texture();
    glutMainLoop();

    return 0;
}
