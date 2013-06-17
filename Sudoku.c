//cs335 Spring 2013
//program: Sudoku.c
//author:  Scott Dunning
//
//OpenGL
//
//
//
//
//

//Libraries
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <GL/glfw.h>
#include <string.h>
//macros
#define rnd() (double)rand()/(double)RAND_MAX


extern void generator(void);
extern int init_array[9][9];

//prototypes
void validTest(int x, int y);
int validTestReturn(int x, int y);
void place_random(void);
void clear_board(void);
void updateBoardSize(void);
void fullBoard(void);
int winCheck();
void new_game(void);
void init(void);
void generate(void);
int addToBoard(int x, int y);
void total(void);
void hint(void);

void GLFWCALL checkkey(int k1, int k2);
void GLFWCALL mouse_click(int button, int action);
void check_mouse(void);

int init_glfw(void);
void init_opengl(void);

void render(void);
void get_grid_center(const int i, const int j, int cent[2]);


//Global Variables
int playerMove = 0;
int gameOver  = 0;
int gameState = 0;
int endGame = 0;
int count = 0;
int hints = 0;
int takenRow = 0;
int mouseOver01 = 0;
int mouseOver02 = 0;
int mouseOver03 = 0;
int mouseOver04 = 0;

int xres=640;
int yres=480;

//Structures
typedef struct t_board {
	int status;
	int value;
} Board;
typedef struct t_grid {
	int value;
	int status;
	int highlight;
	int total;
	int over;
	float color[4];
	struct t_grid *prev;
	struct t_grid *next;
} Grid;

//Main Arrays
Grid grid[9][9];
Board board[9][9];
					 
int grid_dim=9;
int board_dim;
int qsize;


//Variables for images
GLuint backGround;
GLuint ball;
GLuint playGame;
GLuint newGame;
GLuint quit;
GLuint rules_button;
GLuint rules;
GLuint win;
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


//Main
int main(int argc, char *argv[])
{
	if (init_glfw()) {
		exit(EXIT_FAILURE);
	}
	init_opengl();
	new_game();
	
	//Main Loop
	while(1) {
	    glfwGetWindowSize(&xres, &yres);
		updateBoardSize();
		check_mouse();
		render();
		glfwSwapBuffers();
		if (endGame == 1) break;
		if (glfwGetKey(GLFW_KEY_ESC) == GLFW_PRESS) break;
		if (!glfwGetWindowParam(GLFW_OPENED)) break;
	}
	glfwTerminate();
	exit(EXIT_SUCCESS);
}
//End Main

//Checks for number key presses
//and sets playerMove as that number
void GLFWCALL checkkey(int k1, int k2)
{
	if ((k1 == 'P') && (k2 == GLFW_RELEASE)) {
		if(gameState != 0){ 
			gameState = 0;
			return;
		}
		else{
			gameState = 1;
			return;
		}
	}
	if ((k1 == 'N') && (k2 == GLFW_RELEASE)) {
		new_game();
	}
	if ((k1 == 'H') && (k2 == GLFW_RELEASE)) {
		if(hints < 5){
			hint();
			hints++;
		}
		else printf("Out of Hints!!\n");
	}
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

//Detects whether the player board
//is completed correctly or not
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

//Main Algorithm -- Accepts grid
//coordinates and looks for
//contradicting cells
void validTest(int x, int y)
{
	int a, b, pass;
	pass = 1;
	
	//Row Check
	for (a=0;a<9;a++){
		grid[x][a].highlight = 1;
		if ((grid[x][y].value == grid[x][a].value)&&(y != a) && (grid[x][y].value != 0)){
			grid[x][a].status = 1;
			pass = 0;
		}
	}
	
	//Column Check
	for (a=0;a<9;a++){
		grid[a][y].highlight = 1;
		if ((grid[x][y].value == grid[a][y].value)&&(x != a) && (grid[x][y].value != 0)){
			grid[a][y].status = 1;
			pass = 0;
		}
	}
	
	//Check 1x1
	if (((x>=0)&&(x<=2)) && ((y>=0)&&(y<=2))){
		for (a=0;a<3;a++){
			for (b=0;b<3;b++){
				grid[a][b].highlight = 1;
				if ((grid[x][y].value == grid[a][b].value)&&((x!=a)&&(y!=b)) && (grid[x][y].value != 0)){
					grid[a][b].status = 1;
					pass = 0;
				}
			}
		}
	}
	
	//Check 1x2
	if (((x>=0)&&(x<=2)) && ((y>=3)&&(y<=5))){
		for (a=0;a<3;a++){
			for (b=3;b<6;b++){
				grid[a][b].highlight = 1;
				if ((grid[x][y].value == grid[a][b].value)&&((x!=a)&&(y!=b)) && (grid[x][y].value != 0)){
					grid[a][b].status = 1;
					pass = 0;
				}
			}
		}
	}
	
	//Check 1x3
	if (((x>=0)&&(x<=2)) && ((y>=6)&&(y<=8))){
		for (a=0;a<3;a++){
			for (b=6;b<9;b++){
				grid[a][b].highlight = 1;
				if ((grid[x][y].value == grid[a][b].value)&&((x!=a)&&(y!=b)) && (grid[x][y].value != 0)){
					grid[a][b].status = 1;
					pass = 0;
				}
			}
		}
	}
	
	//Check 2x1
	if (((x>=3)&&(x<=5)) && ((y>=0)&&(y<=2))){
		for (a=3;a<6;a++){
			for (b=0;b<3;b++){
				grid[a][b].highlight = 1;
				if ((grid[x][y].value == grid[a][b].value)&&((x!=a)&&(y!=b)) && (grid[x][y].value != 0)){
					grid[a][b].status = 1;
					pass = 0;
				}
			}
		}
	}
	
	//Check 2x2
	if (((x>=3)&&(x<=5)) && ((y>=3)&&(y<=5))){
		for (a=3;a<6;a++){
			for (b=3;b<6;b++){
				grid[a][b].highlight = 1;
				if ((grid[x][y].value == grid[a][b].value)&&((x!=a)&&(y!=b)) && (grid[x][y].value != 0)){
					grid[a][b].status = 1;
					pass = 0;
				}
			}
		}
	}
	
	//Check 2x3
	if (((x>=3)&&(x<=5)) && ((y>=6)&&(y<=8))){
		for (a=3;a<6;a++){
			for (b=6;b<9;b++){
				grid[a][b].highlight = 1;
				if ((grid[x][y].value == grid[a][b].value)&&((x!=a)&&(y!=b)) && (grid[x][y].value != 0)){
					grid[a][b].status = 1;
					pass = 0;
				}
			}
		}
	}
	
	//Check 3x1
	if (((x>=6)&&(x<=8)) && ((y>=0)&&(y<=2))){
		for (a=6;a<9;a++){
			for (b=0;b<3;b++){
				grid[a][b].highlight = 1;
				if ((grid[x][y].value == grid[a][b].value)&&((x!=a)&&(y!=b)) && (grid[x][y].value != 0)){
					grid[a][b].status = 1;
					pass = 0;
				}
			}
		}
	}
	
	//Check 3x2
	if (((x>=6)&&(x<=8)) && ((y>=3)&&(y<=5))){
		for (a=6;a<9;a++){
			for (b=3;b<6;b++){
				grid[a][b].highlight = 1;
				if ((grid[x][y].value == grid[a][b].value)&&((x!=a)&&(y!=b)) && (grid[x][y].value != 0)){
					grid[a][b].status = 1;
					pass = 0;
				}
			}
		}
	}
	
	//Check 3x3
	if (((x>=6)&&(x<=8)) && ((y>=6)&&(y<=8))){
		for (a=6;a<9;a++){
			for (b=6;b<9;b++){
				grid[a][b].highlight = 1;
				if ((grid[x][y].value == grid[a][b].value)&&((x!=a)&&(y!=b)) && (grid[x][y].value != 0)){
					grid[a][b].status = 1;
					pass = 0;
				}
			}
		}
	}
	
	if(!pass) grid[x][y].status = 1;
	if(pass){
		for (a=0;a<9;a++){
			for (b=0;b<9;b++){
				grid[a][b].status = 0;
			}
		}
	}
}
//End Main Algorithm


//Initializes a new game
void new_game()
{
	clear_board();
	init();
	fullBoard();
	place_random();
	gameOver = 0;
}

//Clears Board and structure values
void clear_board(void)
{
	int x, y;
	hints = 0;
	for (x = 0; x < 9; x++){
		for (y = 0; y < 9; y++){
			init_array[x][y] = 0;
			board[x][y].value = 0;
			board[x][y].status = 0;
			grid[x][y].value = 0;
			grid[x][y].status = 0;
			grid[x][y].highlight = 0;
		}
	}
}

//Picks a random hard coded board and
//sets it equal to the board structure
void init(void)
{
	int x,y,a,b,pass;
	pass = 0;
	do{
		pass = 0;
		generate();
		for (a=0;a<9;a++){
			for (b=0;b<9;b++){
				if (init_array[a][b] == 0) pass = 1;
			}
		}
		
	}while(pass == 1);
	srand((unsigned int)time(NULL));
	for (x = 0; x < 9; x++){
		for (y = 0; y < 9; y++){
			grid[x][y].status = 0;
			board[x][y].value = init_array[x][y];
		}
	}
}


//Prints the chosen hard coded board
//to the console for debuging
void fullBoard(void)
{
	int x, y;
	printf("\n\n");
	for (x = 8; x >= 0; x--){
		for (y = 0; y < 9; y++){
			printf("%i ", board[x][y].value);
			if ((y == 2)|| (y == 5)){
				printf("| ");
			}
		}
		if ((x == 3)|| (x == 6)){
		    printf("\n");
		    printf("------+-------+------");
		}
		printf("\n");
	}
	printf("\n\n");
}

//Sets up the playable board by
//chosing a maximum of 50 cells
//and filling them with the
//correct value
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

void hint(void)
{
	int x,y,success;
	success = 0;
	srand((unsigned int)time(NULL));
	while(success == 0){
		x = rand() % 9;
		y = rand() % 9;
		if (grid[x][y].value == 0){
			grid[x][y].value = board[x][y].value;
			success = 1;
		}
	}
	printf("Hints used: %i/5 \n",hints+1);
}

void total(void)
{
	int a,b;
	count = 0;
	for (a=0;a<9;a++){
		for (b=0;b<9;b++){
			grid[a][b].total = 0;
		}
	}
	for (a=0;a<9;a++){
		for (b=0;b<9;b++){
			if ((grid[a][b].value == playerMove) &&(playerMove != 0)){
				grid[a][b].total = 1;
				count++;
			}
		}
	}
}

//initialize GLFW
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
	xres = (glist[nmodes-1].Width)/2;
	yres = (glist[nmodes-1].Height)/2;
	//create a window
	if (!glfwOpenWindow(xres, yres, 0, 0, 0, 0, 0, 0, GLFW_WINDOW)) {
	//if (!glfwOpenWindow(xres,yres,8,8,8,0,32,0,GLFW_FULLSCREEN)) {
		glfwTerminate();
		return 1;
	}
	glfwSetWindowTitle("Freaking Awesome Sause");
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

//initialize OpenGL
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
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(GL_FALSE);
	glDisable(GL_CULL_FACE);
	glBindTexture(GL_TEXTURE_2D, 0);
	//
	glEnable(GL_TEXTURE_2D);
	backGround = loadBMP("images/backGround.bmp");
	ball = loadBMP("images/ball.bmp");
	playGame = loadBMP("images/play_game.bmp");
	newGame = loadBMP("images/new_game.bmp");
	quit = loadBMP("images/quit.bmp");
	rules_button = loadBMP("images/rules_button.bmp");
	rules = loadBMP("images/rules.bmp");
	win = loadBMP("images/win.bmp");
	num_one = loadBMP("images/one.bmp");
	num_two = loadBMP("images/two.bmp");
	num_three = loadBMP("images/three.bmp");
	num_four = loadBMP("images/four.bmp");
	num_five = loadBMP("images/five.bmp");
	num_six = loadBMP("images/six.bmp");
	num_seven = loadBMP("images/seven.bmp");
	num_eight = loadBMP("images/eight.bmp");
	num_nine = loadBMP("images/nine.bmp");
	glBindTexture(GL_TEXTURE_2D, 0);
}


//Self explanitory
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

//Checks mouse position
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
	if (gameState == 1){
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
	if (gameState == 0){
		mouseOver01 = 0;
		mouseOver02 = 0;
		mouseOver03 = 0;
		mouseOver04 = 0;
		int b2 = board_dim/2;
		int screen_center[2] = {xres/2, yres/2};
		int s0 = screen_center[0];
		int s1 = screen_center[1];
		if (x >= s0-(b2/3) &&
			x <= s0+(b2/3) &&
			y >= (s1-(b2/8))+((s1-(b2/4))/3) &&
			y <= (s1+(b2/8))+((s1-(b2/4))/3)){
			mouseOver01 = 1;
		}
		if (x >= s0-(b2/3) &&
			x <= s0+(b2/3) &&
			y >= s1-(b2/8) &&
			y <= s1+(b2/8)){
			mouseOver02 = 1;
		}
		if (x >= s0-(b2/3) &&
			x <= s0+(b2/3) &&
			y >= (s1-(b2/8))-((s1-(b2/4))/3) &&
			y <= (s1+(b2/8))-((s1-(b2/4))/3)){
			mouseOver03 = 1;
		}
		if (x >= s0-(b2/6) &&
			x <= s0+(b2/6) &&
			y >= (s1-(b2/11))-(s1-(b2/3)) &&
			y <= (s1+(b2/11))-(s1-(b2/3))){
			mouseOver04 = 1;
		}
	}
}

//Detects left mouse click
//and does actions that follow
void GLFWCALL mouse_click(int button, int action)
{
	int x,y,a,b;
	if (action == GLFW_PRESS) {
		int i,j,k=0;
		//center of a grid
		int cent[2];
		glfwGetMousePos(&x, &y);
		//reverse the y position
		y = yres - y;
		if (gameState == 1){
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
									for (a=0;a<9;a++){
										for (b=0;b<9;b++){
											grid[a][b].status = 0;
											grid[a][b].highlight = 0;
										}
									}
									validTest(i,j);
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
		if (gameState == 0){
			int b2 = board_dim/2;
			int screen_center[2] = {xres/2, yres/2};
			int s0 = screen_center[0];
			int s1 = screen_center[1];
			if (x >= s0-(b2/3) &&
				x <= s0+(b2/3) &&
				y >= (s1-(b2/8))+((s1-(b2/4))/3) &&
				y <= (s1+(b2/8))+((s1-(b2/4))/3)){
				if (button == GLFW_MOUSE_BUTTON_LEFT){
					gameState = 1;
				}
			}
			if (x >= s0-(b2/3) &&
				x <= s0+(b2/3) &&
				y >= s1-(b2/8) &&
				y <= s1+(b2/8)){
				if (button == GLFW_MOUSE_BUTTON_LEFT){
					new_game();
					gameState = 1;
				}
			}
			if (x >= s0-(b2/3) &&
				x <= s0+(b2/3) &&
				y >= (s1-(b2/8))-((s1-(b2/4))/3) &&
				y <= (s1+(b2/8))-((s1-(b2/4))/3)){
				if (button == GLFW_MOUSE_BUTTON_LEFT){
					endGame = 1;
				}
			}
			if (x >= s0-(b2/6) &&
				x <= s0+(b2/6) &&
				y >= (s1-(b2/11))-(s1-(b2/3)) &&
				y <= (s1+(b2/11))-(s1-(b2/3))){
				if (button == GLFW_MOUSE_BUTTON_LEFT){
					gameState = 2;
				}
			}
		}
	}
}

//Self explanitory
void get_grid_center(const int i, const int j, int cent[2])
{
	int b2 = board_dim/2;
	int screen_center[2] = {xres/2, yres/2};
	int s0 = screen_center[0];
	int s1 = screen_center[1];
	int bq;
	//quad upper-left corner
	int quad[2];
	
	//Remove excess board
    board_dim = board_dim - (board_dim % grid_dim);
	
	//bq is the width of one grid section
	bq = (board_dim / grid_dim);
	
	quad[0] = s0-b2;
	quad[1] = s1-b2;
	cent[0] = quad[0] + bq/2;
	cent[1] = quad[1] + bq/2;
	cent[0] += (bq * j);
	cent[1] += (bq * i);
}


//Render OpenGL function
//includes statements for
//color changes
void render(void)
{
	int i,j;
	int b2 = board_dim/2;
	int screen_center[2] = {xres/2, yres/2};
	int s0 = screen_center[0];
	int s1 = screen_center[1];
	int bq, bp;

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
	if (gameState == 1){
		if (gameOver == 1){
			glClearColor(0.2f, 0.2f, 1.0f, 1.0f);
		}
		else glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	}
	if (gameState == 0){
		glClearColor(0.3f, 0.8f, 0.3f, 0.3f);
	}
	glClear(GL_COLOR_BUFFER_BIT);
	//init matrices
	glMatrixMode (GL_PROJECTION); glLoadIdentity();
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();
	//this sets to 2D mode (no perspective)
	glOrtho(0, xres, 0, yres, -1, 1);
	
	//draw stuff
	if (gameState == 1){
		if (gameOver == 1){
			glColor3f(0.2f, 0.2f, 1.0f);
		}
		else glColor3f(0.7f, 0.7f, 0.7f);
	}
	if (gameState == 0){
		glColor3f(0.3f, 0.8f, 0.3f);
	}
	if (gameState == 2){
		glColor3f(0.3f, 0.8f, 0.3f);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindTexture(GL_TEXTURE_2D, backGround);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f); glVertex2i(0, 0);
			glTexCoord2f(0.0f, 1.0f); glVertex2i(0, yres);
			glTexCoord2f(1.0f, 1.0f); glVertex2i(xres, yres);
			glTexCoord2f(1.0f, 0.0f); glVertex2i(xres, 0);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, 0);
	
	
	
	
	//=====================================================================================================
	if (gameState == 0){
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindTexture(GL_TEXTURE_2D, ball);
		glColor4f(0.2f, 0.2f, 1.0f, 0.8f);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f); glVertex2i(s0-b2, s1-b2);
			glTexCoord2f(0.0f, 1.0f); glVertex2i(s0-b2, s1+b2);
			glTexCoord2f(1.0f, 1.0f); glVertex2i(s0+b2, s1+b2);
			glTexCoord2f(1.0f, 0.0f); glVertex2i(s0+b2, s1-b2);
		glEnd();
		
		if (mouseOver01 == 1){
			glColor3f(1.0f, 1.0f, 0.0f);
		}
		else glColor4f(0.2f, 1.0f, 0.2f, 0.9f);
		glBindTexture(GL_TEXTURE_2D, playGame);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f); glVertex2i(s0-(b2/3), (s1-(b2/8))+((s1-(b2/4))/3));
			glTexCoord2f(0.0f, 1.0f); glVertex2i(s0-(b2/3), (s1+(b2/8))+((s1-(b2/4))/3));
			glTexCoord2f(1.0f, 1.0f); glVertex2i(s0+(b2/3), (s1+(b2/8))+((s1-(b2/4))/3));
			glTexCoord2f(1.0f, 0.0f); glVertex2i(s0+(b2/3), (s1-(b2/8))+((s1-(b2/4))/3));
		glEnd();
		
		if (mouseOver02 == 1){
			glColor3f(1.0f, 1.0f, 0.0f);
		}
		else glColor4f(0.2f, 1.0f, 0.2f, 0.9f);
		glBindTexture(GL_TEXTURE_2D, newGame);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f); glVertex2i(s0-(b2/3), s1-(b2/8));
			glTexCoord2f(0.0f, 1.0f); glVertex2i(s0-(b2/3), s1+(b2/8));
			glTexCoord2f(1.0f, 1.0f); glVertex2i(s0+(b2/3), s1+(b2/8));
			glTexCoord2f(1.0f, 0.0f); glVertex2i(s0+(b2/3), s1-(b2/8));
		glEnd();
		
		if (mouseOver03 == 1){
			glColor3f(1.0f, 1.0f, 0.0f);
		}
		else glColor4f(0.2f, 1.0f, 0.2f, 0.9f);
		glBindTexture(GL_TEXTURE_2D, quit);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f); glVertex2i(s0-(b2/3), (s1-(b2/8))-((s1-(b2/4))/3));
			glTexCoord2f(0.0f, 1.0f); glVertex2i(s0-(b2/3), (s1+(b2/8))-((s1-(b2/4))/3));
			glTexCoord2f(1.0f, 1.0f); glVertex2i(s0+(b2/3), (s1+(b2/8))-((s1-(b2/4))/3));
			glTexCoord2f(1.0f, 0.0f); glVertex2i(s0+(b2/3), (s1-(b2/8))-((s1-(b2/4))/3));
		glEnd();
		
		if (mouseOver04 == 1){
			glColor3f(1.0f, 1.0f, 0.0f);
		}
		else glColor4f(0.2f, 1.0f, 0.2f, 0.9f);
		glBindTexture(GL_TEXTURE_2D, rules_button);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f); glVertex2i(s0-(b2/6), (s1-(b2/11))-(s1-(b2/3)));
			glTexCoord2f(0.0f, 1.0f); glVertex2i(s0-(b2/6), (s1+(b2/11))-(s1-(b2/3)));
			glTexCoord2f(1.0f, 1.0f); glVertex2i(s0+(b2/6), (s1+(b2/11))-(s1-(b2/3)));
			glTexCoord2f(1.0f, 0.0f); glVertex2i(s0+(b2/6), (s1-(b2/11))-(s1-(b2/3)));
		glEnd();
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	
	//===============================================================================================
	if (gameState == 2){
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindTexture(GL_TEXTURE_2D, rules);
		glColor4f(0.5f, 0.5f, 1.0f, 0.8f);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f); glVertex2i(s0-b2, s1-b2);
			glTexCoord2f(0.0f, 1.0f); glVertex2i(s0-b2, s1+b2);
			glTexCoord2f(1.0f, 1.0f); glVertex2i(s0+b2, s1+b2);
			glTexCoord2f(1.0f, 0.0f); glVertex2i(s0+b2, s1-b2);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	
	//=================================================================================================
	//draw the main game board in middle of screen
	
	if (gameState == 1){
		glColor3f(0.4f, 0.4f, 0.4f);
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
		for(i = 0; i < grid_dim-1; i++)
		{
			if ((i==2)||(i==5)){
				glLineWidth(6);
				glColor3f(0.0f, 0.0f, 0.0f);
			}
			else{
				glLineWidth(2);
				glColor3f(0.4f, 0.4f, 0.4f);
			}
			glBegin(GL_LINES);
			bp += bq;
			glVertex2i((s0-b2) + bp, s1-b2);
			glVertex2i((s0-b2) + bp, s1+b2);
			glVertex2i(s0-b2, (s1-b2) + bp);
			glVertex2i(s0+b2, (s1-b2) + bp);
			glEnd();
		}
	
		//draw a new square in center of each grid
		//squares are slightly smaller than grid
	
		for (i=0; i<grid_dim; i++) {
			for (j=0; j<grid_dim; j++) {
				get_grid_center(i,j,cent);
				glColor3f(1.0f, 1.0f, 1.0f);
				if (grid[i][j].highlight == 1){
					glColor3f(1.0f, 1.0f, 0.3f);
					}
				if (grid[i][j].over) {
					glColor3f(1.0f, 1.0f, 0.0f);
				}
				if (board[i][j].status == 1){
					glColor3f(0.8f, 0.8f, 0.8f);
				}
				if ((grid[i][j].highlight == 1)&&(board[i][j].status == 1)){
					glColor3f(0.8f, 0.8f, 0.5f);
				}
				if ((grid[i][j].value == playerMove) &&(playerMove != 0)){
					glColor3f(0.4f, 1.0f, 0.4f);
					if (grid[i][j].status == 1){
						glColor3f(1.0f, 0.4f, 0.4f);
					}
				}
				total();
				if ((grid[i][j].total == 1)&&(count == 9)){
					glColor3f(0.4f, 0.4f, 1.0f);
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
		if (gameOver){
			glBindTexture(GL_TEXTURE_2D, 0);
			glBindTexture(GL_TEXTURE_2D, win);
			glColor4f(0.2f, 0.2f, 1.0f, 0.9f);
			glBegin(GL_QUADS);
				glTexCoord2f(0.0f, 0.0f); glVertex2i(s0-b2, s1-b2);
				glTexCoord2f(0.0f, 1.0f); glVertex2i(s0-b2, s1+b2);
				glTexCoord2f(1.0f, 1.0f); glVertex2i(s0+b2, s1+b2);
				glTexCoord2f(1.0f, 0.0f); glVertex2i(s0+b2, s1-b2);
			glEnd();
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}
}

//BMP image loader function
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

