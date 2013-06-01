//cs335 Spring 2013
//program: Sudoku.c
//author:  Scott Dunning
//
//
//
//
//
//
//
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <GL/glfw.h>
#include <string.h>
//macros
#define rnd() (double)rand()/(double)RAND_MAX
//prototypes


void generate_board(void);
void place_random(void);
void clear_board(void);
void updateBoardSize(void);
void fullBoard(void);
int winCheck();

int playerMove = 0;
int gameOver  = 0;
/////
void init(void);
int init_glfw(void);
void init_opengl(void);

void render(void);
void GLFWCALL mouse_click(int button, int action);
void check_mouse(void);
void GLFWCALL checkkey(int k1, int k2);
void new_game(void);
void get_grid_center(const int i, const int j, int cent[2]);
int xres=640;
int yres=480;
//
typedef struct t_board {
	int status;
	int value;
} Board;
typedef struct t_grid {
	int value;
	int over;
	float color[4];
	struct t_grid *prev;
	struct t_grid *next;
} Grid;
Grid grid[9][9];
Board board[9][9];
int test_array[9][9] = {{4,9,1,2,5,3,6,8,7},
						{5,2,6,8,7,1,9,3,4},
						{7,3,8,9,4,6,5,1,2},
						{8,7,2,3,9,4,1,6,5},
						{3,4,9,6,1,5,2,7,8},
						{1,6,5,7,2,8,3,4,9},
						{6,1,4,5,8,9,7,2,3},
						{2,5,3,4,6,7,8,9,1},
						{9,8,7,1,3,2,4,5,6}};
int grid_dim=9;
int board_dim;
int qsize;

GLuint num_one;
GLuint num_two;
GLuint num_three;
GLuint num_four;
GLuint num_five;
GLuint num_six;
GLuint num_seven;
GLuint num_eight;
GLuint num_nine;
GLuint loadBMP(const char *imagepath);


int main(int argc, char *argv[])
{
	/*int i, j;

	//Read parameters and skip first value
	for (i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], "-gridSize") == 0)
		{
			if(argc < i+2)
			{
				printf("Usage: -gridSize [VALUE]\n");
				exit(EXIT_FAILURE);
			}
			j = atoi(argv[i+1]);
			if(j == 0) {
			    printf("Enter a number for the -gridSize switch\n");
			    exit(EXIT_FAILURE);
			}
			//Limit dimensions to anything between MINSIZE and MAXSIZE
			grid_dim = (j <= MINSIZE) ? MINSIZE : ((j <= MAXSIZE) ? j : MAXSIZE);
			i++;
		}
	}*/

	if (init_glfw()) {
		exit(EXIT_FAILURE);
	}
	init_opengl();
	new_game();
	
	
	while(1) {
	    glfwGetWindowSize(&xres, &yres);
		updateBoardSize();	
		if (glfwGetKey('N') == GLFW_PRESS) {
			new_game();
        }
		check_mouse();
		render();
		glfwSwapBuffers();
		if (glfwGetKey(GLFW_KEY_ESC) == GLFW_PRESS) break;
		if (!glfwGetWindowParam(GLFW_OPENED)) break;
	}
	glfwTerminate();
	exit(EXIT_SUCCESS);
}


void GLFWCALL checkkey(int k1, int k2)
{
	if (k1 == '0') {
		playerMove = 0;
		return;
	}
	if (k1 == '1') {
		playerMove = 1;
		return;
	}
	if (k1 == '2') {
		playerMove = 2;
		return;
	}
	if (k1 == '3') {
		playerMove = 3;
		return;
	}
	if (k1 == '4') {
		playerMove = 4;
		return;
	}
	if (k1 == '5') {
		playerMove = 5;
		return;
	}
	if (k1 == '6') {
		playerMove = 6;
		return;
	}
	if (k1 == '7') {
		playerMove = 7;
		return;
	}
	if (k1 == '8') {
		playerMove = 8;
		return;
	}
	if (k1 == '9') {
		playerMove = 9;
		return;
	}
}

void generate_board(void)
{
	srand((unsigned int)time(NULL));
	
}

int winCheck()
{
	int x, y;
	for(x=0;x<9;x++){
		for(y=0;y<9;y++){
			if (board[x][y].value != grid[x][y].value){
				return 0;
			}
		}
	}
	return 1;
}

void new_game()
{
	clear_board();
	init();
	fullBoard();
	place_random();
}

void clear_board(void)
{
	int x, y;
	for (x = 0; x < 9; x++){
		for (y = 0; y < 9; y++){
			board[x][y].value = 0;
			board[x][y].status = 0;
			grid[x][y].value = 0;
		}
	}
}

void init(void)
{
	int x, y;
	for (x = 0; x < 9; x++){
		for (y = 0; y < 9; y++){
			board[x][y].value = test_array[x][y];
		}
	}
}

void fullBoard(void)
{
	int x, y;
	printf("\n\n");
	for (x = 8; x >= 0; x--){
		for (y = 0; y < 9; y++){
			printf("%i ", board[x][y].value);
		}
		printf("\n");
	}
	printf("\n\n");
}

void place_random(void)
{
	int i, x, y;
	srand((unsigned int)time(NULL));
	for (i=0; i<50;i++){
		x = rand() % 9;
		y = rand() % 9;
		if(board[x][y].status != 1){
			grid[x][y].value = board[x][y].value;
			board[x][y].status = 1;
		}
	}
		
}


int init_glfw(void)
{
	int nmodes;
	GLFWvidmode glist[256];
	if (!glfwInit()){
		printf("Failed to initialize GLFW\n");
		return 1;
	}
	//get the monitor native full-screen resolution
	nmodes = glfwGetVideoModes(glist, 250);
	xres = glist[nmodes-1].Width;
	yres = glist[nmodes-1].Height;
	//create a window
	if (!glfwOpenWindow(xres, yres, 0, 0, 0, 0, 0, 0, GLFW_WINDOW)) {
	//if (!glfwOpenWindow(xres,yres,8,8,8,0,32,0,GLFW_FULLSCREEN)) {
		glfwTerminate();
		return 1;
	}
	glfwSetWindowTitle("Game Board and Grid");
	glfwSetWindowPos(0, 0);
	//make sure we see the escape key pressed
	glfwEnable(GLFW_STICKY_KEYS);
	glfwSetMouseButtonCallback(mouse_click);
	glfwSetKeyCallback(checkkey);
	glfwEnable( GLFW_MOUSE_CURSOR );
	//enable vertical sync (on cards that support it)
	glfwSwapInterval(1);
	return 0;
}

void init_opengl(void)
{
	//OpenGL initialization
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_COLOR_MATERIAL);
	//
	//choose one of these
	//glShadeModel(GL_FLAT);
	glShadeModel(GL_SMOOTH);
	glDisable(GL_LIGHTING);
	glBindTexture(GL_TEXTURE_2D, 0);
	//
	glEnable(GL_TEXTURE_2D);
	num_one = loadBMP("one.bmp");
	num_two = loadBMP("two.bmp");
	num_three = loadBMP("three.bmp");
	num_four = loadBMP("four.bmp");
	num_five = loadBMP("five.bmp");
	num_six = loadBMP("six.bmp");
	num_seven = loadBMP("seven.bmp");
	num_eight = loadBMP("eight.bmp");
	num_nine = loadBMP("nine.bmp");
	glBindTexture(GL_TEXTURE_2D, 0);
	printf("tex: %i %i\n",num_one);
}

void updateBoardSize(void)
{
	//Add border of 10% of height or width of the window
	board_dim = (yres < xres) ? yres - (yres * .1f) : xres - (xres * .1f);

	//Remove excess board
	board_dim = board_dim - (board_dim % grid_dim);

	int bq;
	//quad upper-left corner
	//bq is the width of one grid section
	bq = (board_dim / grid_dim);

    //Size is based on percentage so that border can scale with board.
    qsize = (bq - (board_dim * .005f))/2;
}

void check_mouse(void)
{
	static int sx=0,sy=0;	
	int x,y;
	int i,j;
	int cent[2];
	glfwGetMousePos(&x, &y);
	//reverse the y position
	y = yres - y;
	if (x == sx && y == sy) return;
	sx=x;
	sy=y;	
	//
	//is the mouse over any grid squares?
	//
	for (i=0; i<grid_dim; i++) {
		for (j=0; j<grid_dim; j++) {
			grid[i][j].over=0;
		}
	}
	for (i=0; i<grid_dim; i++) {
		for (j=0; j<grid_dim; j++) {
			get_grid_center(i,j,cent);
			if (x >= cent[0]-qsize &&
				x <= cent[0]+qsize &&
				y >= cent[1]-qsize &&
				y <= cent[1]+qsize) {
				grid[i][j].over=1;
				break;
				//You could do a return here.
				//If more code is added below, a return
				//would cause you to exit too early.
			}
		}
		if (grid[i][j].over) break;
	}
}

void GLFWCALL mouse_click(int button, int action)
{
	int x,y;
	if (action == GLFW_PRESS) {
		int i,j,k=0;
		//center of a grid
		int cent[2];
		glfwGetMousePos(&x, &y);
		//reverse the y position
		y = yres - y;
		for (i=0; i<grid_dim; i++) {
			for (j=0; j<grid_dim; j++) {
				get_grid_center(i,j,cent);
				if (x >= cent[0]-qsize &&
					x <= cent[0]+qsize &&
					y >= cent[1]-qsize &&
					y <= cent[1]+qsize) {
					if (button == GLFW_MOUSE_BUTTON_LEFT){
						if(!gameOver){
							if(board[i][j].status != 1){
								grid[i][j].value = playerMove;
								if (winCheck()){
									gameOver = 1;
								}
							}
						}
						k=1;
						break;
					}
				}
				if (k) break;
			}
		}
	}
}
void get_grid_center(const int i, const int j, int cent[2])
{
	//This function can be optimized, and made more generic.
	int b2 = board_dim/2;
	int screen_center[2] = {xres/2, yres/2};
	int s0 = screen_center[0];
	int s1 = screen_center[1];
	int bq, bp;
	//quad upper-left corner
	int quad[2];
	
	//Remove excess board
    board_dim = board_dim - (board_dim % grid_dim);
	
	//bq is the width of one grid section
	bq = (board_dim / grid_dim);
	//-------------------------------------
	quad[0] = s0-b2;
	quad[1] = s1-b2;
	cent[0] = quad[0] + bq/2;
	cent[1] = quad[1] + bq/2;
	cent[0] += (bq * j);
	cent[1] += (bq * i);
}

void render(void)
{
	int i,j;
	//--------------------------------------------------------
	//This code is repeated several times in this program, so
	//it can be made more generic and cleaner with some work.
	int b2 = board_dim/2;
	int screen_center[2] = {xres/2, yres/2};
	int s0 = screen_center[0];
	int s1 = screen_center[1];
	int bq, bp;
	//quad upper-left corner
	int quad[2], saveq0;
	//center of a grid
	int cent[2];
	//bq is the width of one grid section
	bq = (board_dim / grid_dim);
	//--------------------------------------------------------
	//start the opengl stuff
	//set the viewing area on screen
	glViewport(0, 0, xres, yres);
	//clear color buffer
	glClearColor(0.3f, 0.3f, 0.3f, 0.3f);
	if (gameOver == 1){
		glClearColor(0.2f, 0.2f, 9.0f, 0.0f);
	}
	glClear(GL_COLOR_BUFFER_BIT);
	//init matrices
	glMatrixMode (GL_PROJECTION); glLoadIdentity();
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();
	//this sets to 2D mode (no perspective)
	glOrtho(0, xres, 0, yres, -1, 1);
	glColor3f(0.2f, 0.2f, 0.2f);
	//=======================================================
	
	//draw stuff
	//draw the main game board in middle of screen
	glBegin(GL_QUADS);
		glVertex2i(s0-b2, s1-b2);
		glVertex2i(s0-b2, s1+b2);
		glVertex2i(s0+b2, s1+b2);
		glVertex2i(s0+b2, s1-b2);
	glEnd();
	//draw grid lines
	//vertical
	glColor3f(0.0f, 0.0f, 0.0f);
	bp = 0;
	glLineWidth(2);
	//glBegin(GL_LINES);
	for(i = 0; i < grid_dim-1; i++)
	{
		if ((i==2)||(i==5)){
			glLineWidth(6);
			glColor3f(1.0f, 0.0f, 0.0f);
		}
		else{
			glLineWidth(2);
			glColor3f(0.2f, 0.2f, 0.2f);
		}
		glBegin(GL_LINES);
		bp += bq;
		glVertex2i((s0-b2) + bp, s1-b2);
		glVertex2i((s0-b2) + bp, s1+b2);
		glVertex2i(s0-b2, (s1-b2) + bp);
		glVertex2i(s0+b2, (s1-b2) + bp);
		glEnd();
	}
	//glEnd();
	//
	//draw a new square in center of each grid
	//squares are slightly smaller than grid
	//
	for (i=0; i<grid_dim; i++) {
		for (j=0; j<grid_dim; j++) {
			get_grid_center(i,j,cent);
			glColor3f(1.0f, 1.0f, 1.0f);
			if (grid[i][j].over) {
				glColor3f(1.0f, 1.0f, 0.0f);
			}
			if (board[i][j].status == 1){
				glColor3f(0.8f, 0.8f, 0.8f);
			}
			if ((grid[i][j].value == playerMove) &&(playerMove != 0)){
				glColor3f(0.4f, 1.0f, 0.4f);
			}
			if ((grid[i][j].value != board[i][j].value) && (grid[i][j].value != 0)){
				glColor3f(1.0f, 0.4f, 0.4f);
			}
			glBindTexture(GL_TEXTURE_2D, 0);
			if (grid[i][j].value==1) glBindTexture(GL_TEXTURE_2D, num_one);
			if (grid[i][j].value==2) glBindTexture(GL_TEXTURE_2D, num_two);
			if (grid[i][j].value==3) glBindTexture(GL_TEXTURE_2D, num_three);
			if (grid[i][j].value==4) glBindTexture(GL_TEXTURE_2D, num_four);
			if (grid[i][j].value==5) glBindTexture(GL_TEXTURE_2D, num_five);
			if (grid[i][j].value==6) glBindTexture(GL_TEXTURE_2D, num_six);
			if (grid[i][j].value==7) glBindTexture(GL_TEXTURE_2D, num_seven);
			if (grid[i][j].value==8) glBindTexture(GL_TEXTURE_2D, num_eight);
			if (grid[i][j].value==9) glBindTexture(GL_TEXTURE_2D, num_nine);
			glBegin(GL_QUADS);
				glTexCoord2f(0.0f, 0.0f); glVertex2i(cent[0]-qsize,cent[1]-qsize);
				glTexCoord2f(0.0f, 1.0f); glVertex2i(cent[0]-qsize,cent[1]+qsize);
				glTexCoord2f(1.0f, 1.0f); glVertex2i(cent[0]+qsize,cent[1]+qsize);
				glTexCoord2f(1.0f, 0.0f); glVertex2i(cent[0]+qsize,cent[1]-qsize);
			glEnd();
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}
}

GLuint loadBMP(const char *imagepath)
{
	//When you create your texture files, please specify
	//type: BMP
	//color depth: 24-bit
	unsigned int retval;
	unsigned char header[54];
	//Each BMP file begins by a 54-bytes header
	//Position in the file where the actual data begins
	unsigned int dataPos;
	unsigned int width, height;
	unsigned int imageSize;
	// = width*height*3
	//RGB data will go in this
	unsigned char *data; 
	//
	printf("loadBMP(%s)...\n",imagepath);
	//Log("opening file **%s**\n",imagepath);
	FILE * file = fopen(imagepath,"r");
	if (!file) {
		printf("Image could not be opened\n");
		return 0;
	} 
	if (fread(header, 1, 54, file)!=54) {
		// If not 54 bytes read : problem
		printf("Not a correct BMP file\n");
		return 0;
	}
	if (header[0]!='B' || header[1]!='M') {
		printf("Not a correct BMP file\n");
		return 0;
	}
	dataPos   = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	width     = *(int*)&(header[0x12]);
	height    = *(int*)&(header[0x16]); 
	//Some BMP files are misformatted, guess missing information
	if (imageSize==0) imageSize=width*height*3;
	if (dataPos==0) dataPos=54; 
	data = (unsigned char *)malloc(imageSize+1);
	//Read the actual data from the file into the buffer
	retval = fread(data,1,imageSize,file);
	fclose(file); 
	//In glTexImage2D, the GL_RGB indicates that we are talking
	//about a 3-component color, and GL_BGR says how exactly
	//it is represented in RAM. As a matter of fact, BMP does
	//not store Red->Green->Blue but Blue->Green->Red, so we
	//have to tell it to OpenGL.
	#define GL_BGR 0x80E0
	//Create one OpenGL texture
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
	free(data);
	return textureID;
}

