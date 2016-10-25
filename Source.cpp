/*
*** FILE NAME   : curling_game.c
*** DESCRIPTION : This program is an example program using OpenGL.
*** DATE        : Jan. 2015
*** WRITTEN By  :farheen sultana
*/

/*
This program implements the curling game

The game uses 2-d transformation like translation and rotation to move a stone from player to target.
Data structures used:
	int round_num;  //maintains current round number
	int score ;     //maintains current score
	int cirCen_X = 840, cirCen_Y = 200; //coordinates of circle center
	int squareCen_X = 60, squareCen_Y = 200; //square center coordinates
	int stoneCen_X = 805, stoneCen_Y = 200; //stone center coordinates 
	int rotate_deg = 5; //degree for rotation of square
	
Methods used:

Program execution starts from main()
It calls display() method.
The canvas is displayed with game name, student name, round number and score.
It also displays the power bar, stone, target and source(player-as a circle)

When the user presses the spacebar, the roundnumber is incremented and the power bar starts moving up and down. 
When the user presses the spacebar for the second time, the power bar stops and the ice_control() function is used. 

This ice_control() function determines the pixels by which the stone must move. 
It generates a random number between 95 and 105, and multiplies the random number with power bar y coordinate to obtain percentage strength. 
strength_pixels_move = (Pt1.y * random_num)/100;

Depending upon the strength_pixels_move, the stone is moved. 
*/

#include <stdio.h>              // standard C libraries
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <GL/glut.h>            // GLUT library
#include "cs_graphics_setup.h"  // Header for CS4250/5250/6250 courses

//@@***********************************************************************************@@
// Constants
#define WINDOW_XS 900					// Window size
#define WINDOW_YS 500
#define WINDOW_NAME "Curling Game"  	// Window name

#define ANI_MSEC 10

//@@***********************************************************************************@@
// Function prototypes
void display_func(void);
void keyboard_func(unsigned char c, int x, int y);
void display_text(char *string, int x, int y);
void animation_func(int val);

void ice_control();
void score_calc();
void reset();

//@@***********************************************************************************@@
//Structures
typedef struct pt
{
	GLint x, y;
}MyPoint;

//@@***********************************************************************************@@
// Global Variables
MyPoint Pt1, Pt2; // Pt1 and Pt2 are coordinates of vertices of the top edge of power bar

int round_num = 0;
int score = 0;
char buffer[5];

int moveUp = 1; // =0 progress bar goes down, =1 progress bar goes up

int cirCen_X = 840, cirCen_Y = 200;
int cirRadius = 20;
float DegToRad = 3.141592 / 180.0;

int squareCen_X = 60, squareCen_Y = 200;
int stoneCen_X = 805, stoneCen_Y = 200;
int stoneMove_X = 805, stoneMove_Y = 200;

int startPt_X = 805, startPt_Y = 200;
int stoneRotate_X, stoneRotate_Y;
int rotate_deg = 5;

int spacebar_count = 0;

int strength_pixels_move = 0;

int flag_start_power = 0; //1=> yes, 0 => no
int flag_end_power = 0; // 1 => yes, -1 => no

int flag_call_score = 0;
int flag_for_reset = 0;
//@@***********************************************************************************@@
int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	init_setup(WINDOW_XS, WINDOW_YS, WINDOW_NAME);
	
	Pt1.x = 890;
	Pt1.y = 40;
	Pt2.x = 870;
	Pt2.y = 40;
	glutDisplayFunc(display_func);			// call back for display event
	glutKeyboardFunc(keyboard_func);		// call back for keyboard event
	glutTimerFunc(ANI_MSEC, animation_func, 0);
	glutMainLoop();
	return 1;
}	// end of main()

//@@***********************************************************************************@@
void display_func(void)
{
	float deg;
	float cir_X, cir_Y;

	glClearColor(1.0, 1.0, 1.0, 1.0);   // background color (white)
	glClear(GL_COLOR_BUFFER_BIT);       // clearing the buffer not to keep the color

	glColor3f(0.0, 1.0, 0.0);			// setting pen color (green)

	display_text("Curling Game", 20, 470);

	display_text("Round:", 780, 470);
	_itoa_s(round_num, buffer,10);
	display_text(buffer, 850, 470);  //round number

	display_text("Farheen", 20, 440);

	display_text("Scores:", 780, 440);
	_itoa_s(score, buffer,10);
	display_text(buffer, 850, 440);

	glColor3f(1.0, 0.0, 0.0);			// setting pen color (red)
	glLineWidth(3);					// setting line width

	//draw a line after the names.
	glBegin(GL_LINES);
	glVertex2i(0, 400);
	glVertex2i(900, 400);
	glEnd();

	// draw a rectangle for power bar
	glBegin(GL_LINE_LOOP);
	glVertex2i(870, 30);
	glVertex2i(890, 30);
	glVertex2i(890, 330);
	glVertex2i(870, 330);
	glEnd();

	//fill in the progress bar
	glColor3f(1.0, 0.0, 1.0);			// setting pen color (blue)
	glBegin(GL_POLYGON);
	glVertex2i(870, 30);
	glVertex2i(890, 30);
	glVertex2i(Pt1.x, Pt1.y);
	glVertex2i(Pt2.x, Pt2.y);
	glEnd();

	//draw green square
	glColor3f(0.0, 1.0, 0.0);
	glBegin(GL_POLYGON);
		glVertex2i(squareCen_X - 50, squareCen_Y - 50);
		glVertex2i(squareCen_X + 50, squareCen_Y - 50);
		glVertex2i(squareCen_X + 50, squareCen_Y + 50);
		glVertex2i(squareCen_X - 50, squareCen_Y + 50);
	glEnd();

	//draw yellow square
	glColor3f(1.0, 1.0, 0.0);
	glBegin(GL_POLYGON);
		glVertex2i(squareCen_X - 35, squareCen_Y - 35);
		glVertex2i(squareCen_X + 35, squareCen_Y - 35);
		glVertex2i(squareCen_X + 35, squareCen_Y + 35);
		glVertex2i(squareCen_X - 35, squareCen_Y + 35);
	glEnd();
	                         
	//draw red square
	//center at 60, 200
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_POLYGON);
		glVertex2i(squareCen_X - 20, squareCen_Y - 20);
		glVertex2i(squareCen_X + 20, squareCen_Y - 20);
		glVertex2i(squareCen_X + 20, squareCen_Y + 20);
		glVertex2i(squareCen_X - 20, squareCen_Y + 20);
	glEnd();

	//draw circle
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_LINE_LOOP);
	for (deg = 0.0; deg < 360; deg += 1)
	{
		cir_X = 20 * cos(deg*DegToRad) + cirCen_X;
		cir_Y = 20 * sin(deg*DegToRad) + cirCen_Y;

		//printf("%f, %f\n", cir_X, cir_Y);

		glVertex2f(cir_X, cir_Y);
	}
	glEnd();


	//draw stone
	//center of stone = 805, 200
	glColor3f(0.5, 0.5, 0.5);
	glBegin(GL_POLYGON);
		glVertex2i(stoneCen_X - 10, stoneCen_Y - 10);
		glVertex2i(stoneCen_X + 10, stoneCen_Y - 10);
		glVertex2i(stoneCen_X + 10, stoneCen_Y + 10);
		glVertex2i(stoneCen_X - 10, stoneCen_Y + 10);
	glEnd();

	
	glFlush();

	glutSwapBuffers();
	//reset();
}	// end of display_func()
//@@***********************************************************************************@@
void score_calc()
{

	if ((stoneCen_X > squareCen_X - 20) && (stoneCen_X < squareCen_X + 20) && (stoneCen_Y > squareCen_Y - 20) && (stoneCen_Y < squareCen_Y + 20))
	{
		score += 5;
	}
	else if ((stoneCen_X > squareCen_X - 35) && (stoneCen_X < squareCen_X + 35) && (stoneCen_Y > squareCen_Y - 35) && (stoneCen_Y < squareCen_Y + 35))
	{
		score += 3;
	}
	else if ((stoneCen_X > squareCen_X - 50) && (stoneCen_X < squareCen_X + 50) && (stoneCen_Y > squareCen_Y - 50) && (stoneCen_Y < squareCen_Y + 50))
	{
		score += 1;
	}
	else
	{
		score += 0;
	}
}


//@@***********************************************************************************@@
void ice_control()
{
	//generate a random number between 95 and 105
	//multiply the random number with power bar strength to obtain condition of ice.
	//power bar strength can be determined from Pt1 y/Pt2 y coordinate.
	int random_num;
	random_num = rand() % (105 + 1 - 95) + 95;
	printf("rand = %d", random_num);
	printf("Pt1.y = %d", Pt1.y);
	strength_pixels_move = (Pt1.y * random_num)/100;
	printf("strength = %d", strength_pixels_move);
}

//@@***********************************************************************************@@
void display_text(char *string, int x, int y)
{
	int i;
	glRasterPos2i(x, y);
	for (i = 0; i < strlen(string); i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string[i]);
	}
}


//@@***********************************************************************************@@
void keyboard_func(unsigned char c, int x, int y)
{
	int gap = 1;
	
	switch (c)
	{
	case ' ':
				spacebar_count++;
				if (spacebar_count == 1)
					round_num++;
				if (spacebar_count == 2)
				{
					ice_control();
				}					
		     	break;
	case 'l':
	case 'L':
	case 'q':
	case 'Q':
		exit(1);
		break;

	}  // end of switch
}	// end of keyboard_func()
//@@***********************************************************************************@@
void animation_func(int val)
{
	int gap = 1;
	int i = 0;
	
	//first space bar press triggers the powerbar animation.
	//second space bar press stops the powerbar animation.
	if (spacebar_count == 1)
	{
		flag_start_power = 1;
	}
	if (spacebar_count == 2)
	{
		flag_end_power = -1;
		
	}
	
	if (flag_end_power != -1 && flag_start_power == 1)
	{
		if (moveUp == 1)
		{
			Pt1.y += gap; //Pt1 and Pt2 have the values of the power bar.
			Pt2.y += gap;
			if ((Pt1.y > 330))
			{
				Pt1.y -= gap;
				Pt2.y -= gap;
				moveUp = 0;
			}
		}
		else
		{
			Pt1.y -= gap;
			Pt2.y -= gap;
			if ((Pt1.y < 30))
			{
				Pt1.y += gap;
				Pt2.y += gap;
				moveUp = 1;
			}
		}
	}

	if (flag_end_power == -1)
	{
		while (stoneCen_X - 10 >= strength_pixels_move)
		{

					// translation, bring center of stone to origin T(-c)
					stoneCen_X = stoneCen_X - startPt_X ;
					stoneCen_X = stoneCen_X - startPt_X ;
			
					//rotation, rotate the stone by 5 degrees
					stoneRotate_X = stoneCen_X * (cos(rotate_deg*DegToRad)) - stoneMove_Y * (sin(rotate_deg*DegToRad));
					stoneRotate_Y = stoneCen_X * (sin(rotate_deg*DegToRad)) + stoneMove_Y * (cos(rotate_deg*DegToRad));
			
					// translation the center of stone T(+c)
					stoneCen_X += startPt_X;
					stoneCen_Y += startPt_Y;
			
					//move the stone by 2 pixels
					stoneCen_X -= 2;
		}//end while
		flag_call_score = 1;
		flag_end_power = 0;
		flag_start_power = 0;
		
	}
	
	glutPostRedisplay();
	glutTimerFunc(ANI_MSEC, animation_func, 0);
	//reset();
}
//@@***********************************************************************************@@
//This function resets the stone and spacebar counts. Also, it resets the flags for power bar.
void reset()
{
	stoneCen_X = 805;
	stoneCen_Y = 200;
	spacebar_count = 0;
	flag_end_power = 0;
	flag_start_power = 0;
}

