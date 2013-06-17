#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define rnd() (double)rand()/(double)RAND_MAX

//void generate(int*array);
static int addToBoard(int x, int y);
static int validTest(int x, int y);
//static void fullBoard(void);

int init_array[9][9];
static int count = 0;
static int takenRow = 0;





/*int main()
{
	int a,b;
	generate();
	fullBoard();
	for (a=0;a<9;a++){
		for (b=0;b<9;b++){
			if ((!validTest(a,b))||(init_array[a][b] == 0)){
				printf("Bad Number at:  ");
				printf("%i", a);
				printf(" : ");
				printf("%i \n", b);
			}
		}
    }
}*/





void generate(void)
{
    int x,y,z,a,b;
	for (a=9;a<9;a++){
		for (b=0;b<9;b++){
			init_array[a][b] = 0;
		}
	}
	srand((unsigned int)time(NULL));
	x = (rand() % 8)+1;
	for (a=0;a<9;a++){
		do{
			z = (rand() % 9) +1;
			init_array[x][a] = z;
		}while(!validTest(x,a));
	}
	takenRow = x;
	printf("Set row:  %i\n", takenRow);
	x = 0;
    y = 0;
	count = 0;
	addToBoard(x,y);
}





int addToBoard(int x, int y)
{
	int a;
	srand((unsigned int)time(NULL));
	if (count == 72){
		return 1;
	}
	for (a=1;a<10;a++){
		init_array[x][y] = a;
		if (validTest(x,y)){
			if (y == 8){
				x++;
				if(x == takenRow) x++;
				y = -1;
			}
			count++;
			if (addToBoard(x,(y+1))) return 1;
		}
	}
	count--;
	init_array[x][y] = 0;
	return 0;
}






/*
void fullBoard(void)
{
	int x, y;
	printf("\n\n");
	for (x = 0; x < 9; x++){
		for (y = 0; y < 9; y++){
			printf("%i ", init_array[x][y]);
			if ((y == 2)|| (y == 5)){
				printf("| ");
			}
		}
		if ((x == 2)|| (x == 5)){
		    printf("\n");
		    printf("---------------------");
		}
		printf("\n");
	}
	printf("\n\n");
}
*/







int validTest(int x, int y)
{
	int a, b;
	
	//Row Check
	for (a=0;a<9;a++){
		if ((init_array[x][y] == init_array[x][a])&&(y != a) && (init_array[x][y] != 0)){
			return 0;
		}
	}
	
	//Column Check
	for (a=0;a<9;a++){
		if ((init_array[x][y] == init_array[a][y])&&(x != a) && (init_array[x][y] != 0)){
			return 0;
		}
	}
	
	//Check 1x1
	if (((x>=0)&&(x<=2)) && ((y>=0)&&(y<=2))){
		for (a=0;a<3;a++){
			for (b=0;b<3;b++){
				if ((init_array[x][y] == init_array[a][b])&&((x!=a)&&(y!=b)) && (init_array[x][y] != 0)){
					return 0;
				}
			}
		}
	}
	
	//Check 1x2
	if (((x>=0)&&(x<=2)) && ((y>=3)&&(y<=5))){
		for (a=0;a<3;a++){
			for (b=3;b<6;b++){
				if ((init_array[x][y] == init_array[a][b])&&((x!=a)&&(y!=b)) && (init_array[x][y] != 0)){
					return 0;
				}
			}
		}
	}
	
	//Check 1x3
	if (((x>=0)&&(x<=2)) && ((y>=6)&&(y<=8))){
		for (a=0;a<3;a++){
			for (b=6;b<9;b++){
				if ((init_array[x][y] == init_array[a][b])&&((x!=a)&&(y!=b)) && (init_array[x][y] != 0)){
					return 0;
				}
			}
		}
	}
	
	//Check 2x1
	if (((x>=3)&&(x<=5)) && ((y>=0)&&(y<=2))){
		for (a=3;a<6;a++){
			for (b=0;b<3;b++){
				if ((init_array[x][y] == init_array[a][b])&&((x!=a)&&(y!=b)) && (init_array[x][y] != 0)){
					return 0;
				}
			}
		}
	}
	
	//Check 2x2
	if (((x>=3)&&(x<=5)) && ((y>=3)&&(y<=5))){
		for (a=3;a<6;a++){
			for (b=3;b<6;b++){
				if ((init_array[x][y] == init_array[a][b])&&((x!=a)&&(y!=b)) && (init_array[x][y] != 0)){
					return 0;
				}
			}
		}
	}
	
	//Check 2x3
	if (((x>=3)&&(x<=5)) && ((y>=6)&&(y<=8))){
		for (a=3;a<6;a++){
			for (b=6;b<9;b++){
				if ((init_array[x][y] == init_array[a][b])&&((x!=a)&&(y!=b)) && (init_array[x][y] != 0)){
					return 0;
				}
			}
		}
	}
	
	//Check 3x1
	if (((x>=6)&&(x<=8)) && ((y>=0)&&(y<=2))){
		for (a=6;a<9;a++){
			for (b=0;b<3;b++){
				if ((init_array[x][y] == init_array[a][b])&&((x!=a)&&(y!=b)) && (init_array[x][y] != 0)){
					return 0;
				}
			}
		}
	}
	
	//Check 3x2
	if (((x>=6)&&(x<=8)) && ((y>=3)&&(y<=5))){
		for (a=6;a<9;a++){
			for (b=3;b<6;b++){
				if ((init_array[x][y] == init_array[a][b])&&((x!=a)&&(y!=b)) && (init_array[x][y] != 0)){
					return 0;
				}
			}
		}
	}
	
	//Check 3x3
	if (((x>=6)&&(x<=8)) && ((y>=6)&&(y<=8))){
		for (a=6;a<9;a++){
			for (b=6;b<9;b++){
				if ((init_array[x][y] == init_array[a][b])&&((x!=a)&&(y!=b)) && (init_array[x][y] != 0)){
					return 0;
				}
			}
		}
	}
	return 1;
}
