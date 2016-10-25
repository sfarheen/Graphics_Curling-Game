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
