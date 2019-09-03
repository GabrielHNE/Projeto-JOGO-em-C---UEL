#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h> 		//contem time(), que retorna o tempo atual em ms
#include <windows.h>	//contem funcoes necessarias para a funcao gotoxy;
#include <math.h>


#define WIDTH 60

//define da Cores
#define RESET "\x1B[0m"
#define YEL   "\x1B[33m"	//pacman - 0
#define RED   "\x1B[31m"	//ghost - 1
#define MAG   "\x1B[35m"	//ghost 2 - 2
#define CYN   "\x1B[36m"	//ghost 3 - 3 
#define GRN   "\x1B[32m"	//ghost 4 - 4

typedef struct Ghost{
	int posX;
	int	posY;
	int oldX;
	int oldY;
	int kill;	//boolean
	char charact;
	char mov;
}Ghost;

typedef struct Pacman{
	int posX;
	int	posY;
	int oldX;
	int oldY;
	int Life;
	char charact;
}Pacman;

//funcoes gerais
void delay(unsigned int milliseconds);
void gotoxy(int x, int y);
void timer(int tInicio, int tFim);
int difTimer(int tInicio, int tFim);
//funcoes iniciais 
void inicio();	
void showLetreiro();
void animacao();
void inf();		
void screenPoint();
char screenFinal(int* score);

void start(Pacman* pm, Ghost* ghost, Ghost* ghost2, Ghost* ghost3, Ghost* ghost4, int mapa[][30], int mapaO[][30]);
void copiaMapa(int mapaO[][30], int mapa[][30]);
//funcoes de saida
void mapaDraw(int mapa[][30]);
void printMapa(int map[][30],int i,int j);	

void printPac(int Nx, int Ny, int Ox, int Oy, char charc, int mapa[][30], int a);
void auxPrintPac(int Ox, int Oy, int mapa[][30], char perso);

//funções de verificacao corrente no jogo
void pause(char tecla);
void setVelocity(char tecla, int* velocidade);
void checaEstado(Pacman* pm, Ghost* ghost, int* qtd_comeu, int* score);
void checaWin(int mapa[][30], Pacman *pm);
void pontuacao(int *score,int mapa[][30], Pacman* pm);

//funções de movimento
int verMovX(char** tecla,char** keepMove,int x, int y,int mapa[][30]);
int verMovY(char** tecla,char** keepMove, int y, int x,int mapa[][30]);
void movPac( Pacman* pm, char* tecla, char* keepMove, int mapa[][30]);
int calculadist(int PMx, int PMy, int GHx, int GHy);
void movGhost(Ghost* ghost, Pacman* pacman, int mapa[][30]);	
void movGhost2(Ghost* ghost, Pacman* pacman, int mapa[][30]);
void randomMov(Ghost** ghost, int mapa[][30]);
void perseguePac(Ghost** ghost, Pacman** pacman, int mapa[][30]);
void fogePac(Ghost** ghost, Pacman** pacman, int mapa[][30]);
void teleport(Pacman **pm);
void teleportGhost(Ghost*** ghost);

int main(int argc, char** argv){
	int mapa[30][30];
	int mapaO[30][30] = {
						{4,9,9,9,9,9,9,9,9,9,9,9,9,9,5,4,9,9,9,9,9,9,9,9,9,9,9,9,9,5},//0
						{8,1,1,1,1,1,1,1,1,1,1,1,1,1,8,8,1,1,1,1,1,1,1,1,1,1,1,1,1,8},//1
						{8,1,4,9,5,1,4,9,9,9,9,9,5,1,8,8,1,4,9,9,9,9,9,5,1,4,9,5,1,8},//2
						{8,2,8,0,8,1,8,0,0,0,0,0,8,1,8,8,1,8,0,0,0,0,0,8,1,8,0,8,2,8},//3
						{8,1,6,9,7,1,6,9,9,9,9,9,7,1,6,7,1,6,9,9,9,9,9,7,1,6,9,7,1,8},//4
						{8,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,8},//5
						{8,1,4,9,5,1,4,5,1,9,9,9,9,9,5,4,9,9,9,9,9,1,4,5,1,4,9,5,1,8},//6
						{8,1,6,9,7,1,8,8,1,1,1,1,1,1,8,8,1,1,1,1,1,1,8,8,1,6,9,7,1,8},//7
						{8,1,1,1,1,1,8,8,9,9,9,9,9,1,6,7,1,9,9,9,9,9,8,8,1,1,1,1,1,8},//8
						{6,9,9,9,5,1,8,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8,8,1,4,9,9,9,7},//9
						{0,0,0,0,8,1,8,8,0,4,9,9,5,0,0,0,0,4,9,9,5,0,8,8,1,8,0,0,0,0},//10
						{0,0,0,0,8,1,8,8,0,8,0,0,8,0,0,0,0,8,0,0,8,0,8,8,1,8,0,0,0,0},//11
						{0,0,0,0,8,1,8,8,0,8,0,0,8,0,0,0,0,8,0,0,8,0,8,8,1,8,0,0,0,0},//12
						{0,0,0,0,8,1,8,8,0,8,0,0,8,0,0,0,0,8,0,0,8,0,8,8,1,8,0,0,0,0},//13
						{9,9,9,9,7,1,6,7,0,8,0,0,8,0,0,0,0,8,0,0,8,0,6,7,1,6,9,9,9,9},//14
						{0,1,1,1,1,1,0,0,0,8,0,0,8,0,0,0,0,8,0,0,8,0,0,0,1,1,1,1,1,0},//15
						{9,9,9,9,5,1,4,5,0,8,0,0,8,0,0,0,0,8,0,0,8,0,4,5,1,4,9,9,9,9},//16
						{0,0,0,0,8,1,8,8,0,6,9,9,7,0,0,0,0,6,9,9,7,0,8,8,1,8,0,0,0,0},//17
						{0,0,0,0,8,1,8,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8,8,1,8,0,0,0,0},//18
						{0,0,0,0,8,1,8,8,0,4,9,9,9,9,9,9,9,9,9,9,5,0,8,8,1,8,0,0,0,0},//19
						{4,9,9,9,7,1,6,7,1,6,9,9,9,9,5,4,9,9,9,9,7,1,6,7,1,6,9,9,9,5},//20
						{8,1,1,1,1,1,1,1,1,1,1,1,1,1,8,8,1,1,1,1,1,1,1,1,1,1,1,1,1,8},//21
						{8,1,9,9,5,1,9,9,9,9,9,9,9,1,6,7,1,9,9,9,9,9,9,9,1,4,9,9,1,8},//22
						{8,2,1,1,8,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,8,1,1,2,8},//23
						{8,9,9,1,8,1,4,5,1,9,9,9,9,9,5,4,9,9,9,9,9,1,4,5,1,8,1,9,9,8},//24
						{8,1,1,1,1,1,8,8,1,1,1,1,1,1,8,8,1,1,1,1,1,1,8,8,1,1,1,1,1,8},//25
						{8,1,4,9,9,9,7,6,9,9,9,9,5,1,8,8,1,4,9,9,9,9,7,6,9,9,9,5,1,8},//26
						{8,1,6,9,9,9,9,9,9,9,9,9,7,1,6,7,1,6,9,9,9,9,9,9,9,9,9,7,1,8},//27
						{8,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,8},//28					
						{6,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,7}};//29
						
	int gameOver=0;
	int score = 0; 
	int velocidade = 150;
	char tecla = 's', keepMove, f = 'n';
	int tInicio, tFim, tempoComer = 10, qtd_comeu = 1, especial = 0;

	Pacman pm;
	Ghost ghost, ghost2, ghost3, ghost4;

	srand(time(0));

	system("MODE con cols=150 lines=30"); //define o tamanho da tela !!! para centralizar faz (150 - (largura do mapa))/2;
	system("echo"); //usado para poder utilizar as cores
	system("cls");

	while(!gameOver){

		if(f == 'n'){
			inicio();
			inf();
			delay(5000);
		}
		
		start(&pm, &ghost, &ghost2, &ghost3, &ghost4, mapa, mapaO);

		while(pm.Life > 0){
			//verificacao de teclas pressionadas
			if(kbhit()){
				keepMove = tecla;
				tecla = getch();
				pause(tecla);
				setVelocity(tecla, &velocidade);
			}
			
			//verificar quem tem precedencias
			if(!especial){
				movPac(&pm, &tecla, &keepMove, mapa);

				movGhost(&ghost, &pm, mapa);
				movGhost2(&ghost2, &pm, mapa);
				movGhost2(&ghost3, &pm, mapa);
				movGhost2(&ghost4, &pm, mapa);
			} else{
				movGhost(&ghost, &pm, mapa);
				movGhost2(&ghost2, &pm, mapa);
				movGhost2(&ghost3, &pm, mapa);
				movGhost2(&ghost4, &pm, mapa);

				movPac(&pm, &tecla, &keepMove, mapa);
			}

			//verifica a posicao do mapa referente a posicao do pacman
			if(mapa[pm.posY][pm.posX-WIDTH] == 2){
				especial = 1;
				tInicio = clock();
				//ghost
				ghost.charact = 'm';
				ghost.kill = 1;
				//ghost2
				ghost2.charact = 'm';
				ghost2.kill = 1;
				//ghost3
				ghost3.charact = 'm';
				ghost3.kill = 1;
				//ghost4
				ghost4.charact = 'm';
				ghost4.kill = 1;

			}

			checaEstado(&pm,&ghost, &qtd_comeu, &score);
			checaEstado(&pm,&ghost2, &qtd_comeu, &score);
			checaEstado(&pm,&ghost3, &qtd_comeu, &score);
			checaEstado(&pm,&ghost4, &qtd_comeu, &score);
			tFim = clock();

			if(especial){
				timer(tInicio, tFim);
			}

			if(difTimer(tInicio, tFim) >= tempoComer){
				especial = 0;
				//ghost
				ghost.kill = 0;
				ghost.charact = 'w';
				//ghost2
				ghost2.kill = 0;
				ghost2.charact = 'w';
				//ghost3
				ghost3.kill = 0;
				ghost3.charact = 'w';
				//ghost4
				ghost4.kill = 0;
				ghost4.charact = 'w';
				
				qtd_comeu = 1;
			} 

			pontuacao(&score, mapa, &pm);
			checaWin(mapa,&pm);

			if(!especial){
				printPac(pm.posX, pm.posY, pm.oldX, pm.oldY, pm.charact, mapa, 0);
				printPac(ghost.posX, ghost.posY, ghost.oldX, ghost.oldY, ghost.charact, mapa, 1);
				printPac(ghost2.posX, ghost2.posY, ghost2.oldX, ghost2.oldY, ghost2.charact, mapa, 2);
				printPac(ghost3.posX, ghost3.posY, ghost3.oldX, ghost3.oldY, ghost3.charact, mapa, 3);
				printPac(ghost4.posX, ghost4.posY, ghost4.oldX, ghost4.oldY, ghost4.charact, mapa, 4);
			} else{
				printPac(ghost.posX, ghost.posY, ghost.oldX, ghost.oldY, ghost.charact, mapa,1);
				printPac(ghost2.posX, ghost2.posY, ghost2.oldX, ghost2.oldY, ghost2.charact, mapa,2);
				printPac(ghost3.posX, ghost3.posY, ghost3.oldX, ghost3.oldY, ghost3.charact, mapa,3);
				printPac(ghost4.posX, ghost4.posY, ghost4.oldX, ghost4.oldY, ghost4.charact, mapa,4);
				printPac(pm.posX, pm.posY, pm.oldX, pm.oldY, pm.charact, mapa,0);
			}
			delay(velocidade);
		}

		system("cls"); 
		velocidade = 150;
		f = screenFinal(&score);

		if(f =='s'){
			score = 0;
			system("cls");
		} else if (f == 'n'){
			system("cls");
		}
	}

	return 0;
}
void delay(unsigned int milliseconds){	//funcao que permite criar delays para debugging
    clock_t start = clock();
    while((clock() - start) * 1000 / CLOCKS_PER_SEC < milliseconds){}
}
void gotoxy(int x, int y){
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
void timer(int tInicio, int tFim){

	int i = 10 - difTimer(tInicio, tFim);
	//posicao onde esta a caixa do tempo
	gotoxy(38,11);
	i != 0? printf(" %d ", 10 - difTimer(tInicio, tFim)):printf("   ");
	gotoxy(0,0);
}
int difTimer(int tInicio, int tFim){
	return ((tFim - tInicio)/(CLOCKS_PER_SEC));
}
void pause(char tecla){
	if(tecla == 'p'){
		gotoxy(0,0);
		system("pause");
		gotoxy(0,0);
		//apaga o texto de pause gerado automaticamente
		printf("                                       ");
	}
}
void setVelocity(char tecla, int* velocidade){
	if(tecla == '+'){
		(*velocidade) -= 10;
	} else if(tecla == '-'){
		(*velocidade) += 10;
	} else if (tecla == '.'){
		(*velocidade) += 150;
	}
}
void inicio(){
	showLetreiro();
	animacao();
	system("cls");
}
void showLetreiro(){
	gotoxy(WIDTH,0);
	printf(" _____   _____               _   _   _____   _     _     \n");
    gotoxy(WIDTH,1);
	printf("|  _  | |  _  |   _____     | | | | |  _  | | \\   | |    \n");
   	gotoxy(WIDTH,2);	
	printf("| | | | | | | | /       \\   | | | | | | | | |  \\  | | \n");
    gotoxy(WIDTH,3);	
	printf("| | | | | | |  /      X  \\  | \\ / | | | | | |   \\ | | \n");
    gotoxy(WIDTH,4);	
	printf("| | | | | | | /           \\ |     | | | | | |    \\| | \n");
    gotoxy(WIDTH,5);	
	printf("| |_| | | |_||           /  | |v| | | |_| | | |\\    |\n");
    gotoxy(WIDTH,6);	
	printf("|  ___/ |    |         <    | | | | |     | | | \\   |\n");
    gotoxy(WIDTH,7);	
	printf("|  |    |  _ |           \\  | | | | |  _  | | |  \\  | \n");
    gotoxy(WIDTH,8);
    printf("|  |    | | | \\           / | | | | | | | | | |   | | \n");
    gotoxy(WIDTH,9);
    printf("|  |    | | | |\\         /  | | | | | | | | | |   | |\n");
    gotoxy(WIDTH,10);
    printf("|__|    |_| |_| \\ _____ /   |_| |_| |_| |_| |_|   |_|        \n");
}
void animacao(){
	char mStart;
	int i;
	gotoxy(74,11);
	printf("Pressione SPACE para JOGAR");
	gotoxy(76,20);
	printf("Pressione ESC para SAIR");

	//cria a caixa de animacao
	for(i=0;i<31;i++){
		if(i==0){
			gotoxy(71,13);
			printf("|");
			gotoxy(101,13);
			printf("|");
		}
		gotoxy(71+i,12);
		printf("-");
		gotoxy(71+i,14);
		printf("-");
	}

	while(mStart != 32){
		for(i=0;i<26;i++){
			if(kbhit()){					//sempre verifica se alguma tecla foi pressionada, caso sim guarda a tecla
				mStart = getch();
			break;
		 	}								
			//faz a animacao
			gotoxy(75+i,13);
			printf("C");
			gotoxy(0,0);
			delay(60);
			gotoxy(75+i,13);
			printf(" ");
			gotoxy(0,0);
			delay(100);
		}
		if(mStart == 27){
			//gambiarra pra fechar o game!
			exit(1);
		}
		gotoxy(0,0);
	}
}
void inf(){
	gotoxy(83,5);
	printf("Controles do jogo!");
	gotoxy(WIDTH,6);
	printf("W -> Cima");
	gotoxy(WIDTH,7);
	printf("S -> Baixo");
	gotoxy(WIDTH,8);
	printf("A -> Esquerda");
	gotoxy(WIDTH,9);
	printf("D -> Direita");
	gotoxy(WIDTH,10);
	printf("Regras!");
	gotoxy(WIDTH,11);
	printf("%c Para ganhar coma todos os pontos;", 175);
	gotoxy(WIDTH,12);
	printf("%c Voce so tem 3 vidas",175);
	gotoxy(WIDTH,13);
	printf("%c Cerejas dao pontos extras (50pts)",175);
	gotoxy(WIDTH,14);
	printf("%c NAO SEJA PEGO!!",175);
	gotoxy(83,15);
	printf("BOMMMM JOOOGO!");
	gotoxy(0,0);
}
void start(Pacman* pm, Ghost* ghost,Ghost* ghost2, Ghost* ghost3, Ghost* ghost4, int mapa[][30], int mapaO[][30]){

	copiaMapa(mapaO, mapa);
	system("cls");  
	delay(1000);
	mapaDraw(mapa);
	screenPoint();

	
	//configurando o pacman
	(*pm).Life = 3;
	(*pm).charact = 'c';
	(*pm).posX = 74; 						//distancia da parede (largura)
	(*pm).posY	= 23; 						//altura
	gotoxy((*pm).posX,(*pm).posY);		// coloca o pacman na posicao inicial


	//ghost
	(*ghost).charact = 'w';
	(*ghost).kill = 0;
	(*ghost).posX = 74;
    (*ghost).posY = 13;
	(*ghost).mov  = 'w';
    gotoxy((*ghost).posX, (*ghost).posY);
    gotoxy(0,0);

	//ghost2
	(*ghost2).charact = 'w';
	(*ghost2).kill = 0;
	(*ghost2).posX = 74;
    (*ghost2).posY = 12;
	(*ghost2).mov  = 'd';
    gotoxy((*ghost2).posX, (*ghost2).posY);
    gotoxy(0,0);

	//ghost3
	(*ghost3).charact = 'w';
	(*ghost3).kill = 0;
	(*ghost3).posX = 74;
    (*ghost3).posY = 11;
	(*ghost3).mov  = 'w';
    gotoxy((*ghost3).posX, (*ghost3).posY);
    gotoxy(0,0);

	//ghost4
	(*ghost4).charact = 'w';
	(*ghost4).kill = 0;
	(*ghost4).posX = 74;
    (*ghost4).posY = 10;
	(*ghost4).mov  = 'a';
    gotoxy((*ghost4).posX, (*ghost4).posY);
    gotoxy(0,0);
}
char screenFinal(int* score){
	char f;
	gotoxy(70,10);
	//printa parte superior
	printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",201,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,187);
	gotoxy(70,11);
	printf("%c!!!!!!!SCORE!!!!!!!!%c",186,186);
	gotoxy(70,12);
	printf("%c                    %c",186,186);
	gotoxy(70,13);
	printf("%c                    %c",186,186);
	gotoxy(70,14);
	printf("%c  JOGAR NOVAMENTE?  %c",186,186);
	gotoxy(70,15);
	printf("%c       (s/n)        %c",186,186);
	gotoxy(70,16);
	printf("%c                    %c",186,186);
	gotoxy(70,17);
	printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",200,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,188);

	gotoxy(79,12);
	printf("%d", *score);
	gotoxy(0,0); 

	while(f != 's'|| f != 'n'){
		if(kbhit()){
			f = getch();
			if(f=='s')
				return f;
		} else if(f == 'n'){
				return f;
		}
	} 
}
void mapaDraw(int mapa[][30]){
	int i,j;
	for(i=0;i<30;i++){
		for(j=0;j<30;j++){
			printMapa(mapa,i,j);
		}
	}
}
void printMapa(int mapa[][30],int i, int j){
		
	switch(mapa[i][j]){
		case 0:{
			//vazio
			gotoxy(WIDTH+j,i);
			printf(" ");
			break;
		}
		case 1:{
			//printa pontos
			gotoxy(WIDTH+j,i);
			printf("*");
			break;
		}
		case 2:{
			gotoxy(WIDTH+j,i);
			printf("O");
			break;
		}
		case 4:{
			//canto superio esquerdo
			gotoxy(WIDTH+j,i);
			printf("%c", 201);
			break;
		}
		case 5:{
			//canto superior direito
			gotoxy(WIDTH+j,i);
			printf("%c", 187);
			break;
		}
		case 6:{
			//inferior direito
			gotoxy(WIDTH+j,i);
			printf("%c", 200);
			break;
		}
		case 7:{
			//inferio esquerdo
			gotoxy(WIDTH+j,i);
			printf("%c", 188);
			break;
		}
		case 8:{
			//vertical			
			gotoxy(WIDTH+j,i);
			printf("%c", 186);
			break;
		}
		case 9:{
			//horizontal
			gotoxy(WIDTH+j,i);
			printf("%c", 205);
			break;
		}
	}
}
void printPac(int Nx, int Ny, int Ox, int Oy, char charc, int mapa[][30], int a){
	if(Nx!=Ox || Ny!=Oy)
	{
		switch(a){
			case 0: {
				gotoxy(Nx,Ny);
				printf(YEL "%c" RESET, charc);
				gotoxy(0,0);
				auxPrintPac(Ox, Oy, mapa, charc);
				break;
			}
			case 1:{
				gotoxy(Nx,Ny);
				printf(RED "%c" RESET, charc);
				gotoxy(0,0);
				auxPrintPac(Ox, Oy, mapa, charc);	
				break;
			}
			case 2:{
				gotoxy(Nx,Ny);
				printf(MAG "%c" RESET, charc);
				gotoxy(0,0);
				auxPrintPac(Ox, Oy, mapa, charc);
				break;
			}
			case 3:{
				gotoxy(Nx,Ny);
				printf(CYN "%c" RESET, charc);
				gotoxy(0,0);
				auxPrintPac(Ox, Oy, mapa, charc);
				break;
			}
			case 4:{
				gotoxy(Nx,Ny);
				printf(GRN "%c" RESET, charc);
				gotoxy(0,0);
				auxPrintPac(Ox, Oy, mapa, charc);
				break;
			}	
		}
	}
}
void auxPrintPac(int Ox, int Oy, int mapa[][30], char perso){
	if(perso == 'w'|| perso == 'm'){	
		//printa o que antes estava na posicao do ghost
		printMapa(mapa, Oy, (Ox-WIDTH));	
		gotoxy(0,0);		
	} else{
		//apaga os pontos do mapa, independente se contabilizou ou não;
		gotoxy(Ox,Oy);
		printf(" ");
		gotoxy(0,0);
	}
}
void movPac(Pacman* pm, char* tecla, char* keepMove, int mapa[][30]){

	if((*pm).oldX != (*pm).posX ){
		(*pm).oldX = (*pm).posX;
	}
	if((*pm).oldY != (*pm).posY)    {
		(*pm).oldY = (*pm).posY;
	}

	if(*keepMove == 'w' || *keepMove == 's'){
		(*pm).posX = verMovX(&tecla, &keepMove, (*pm).oldX, (*pm).oldY, mapa);
		(*pm).posY = verMovY(&tecla, &keepMove, (*pm).oldY, (*pm).oldX, mapa);
	} else if(*keepMove == 'a' || *keepMove == 'd'){
		(*pm).posY = verMovY(&tecla, &keepMove, (*pm).oldY, (*pm).oldX, mapa);
		(*pm).posX = verMovX(&tecla, &keepMove, (*pm).oldX, (*pm).oldY, mapa);
	}
	teleport(&pm);
}
int verMovX(char** tecla, char** keepMove, int x,int y,int mapa[][30]){
	switch(**tecla){
		case 'a':{
			if(mapa[y][x -(1+WIDTH)] <=3){
				x--;
				**keepMove = **tecla;
				return x;
			} else{
				**tecla = **keepMove;
				return x;}
			break;
		}
		case 'd':{
			if(mapa[y][x +(1-WIDTH)] <=3){
				x++;
				**keepMove = **tecla;				
				return x;
			} else {
				**tecla = **keepMove;
				return x;}
			break;
		}
		default:{
			return x;
			break;
		}
	}
}
int verMovY(char** tecla, char** keepMove, int y, int x,int mapa[][30]){
	//pointeiro do pointeiro hehe
	switch(**tecla){
		case 'w':{
			if(mapa[y-1][x-WIDTH] <=3){
				y--;
				**keepMove = **tecla;
				return y;
			}else{
				**tecla = **keepMove;
				return y;}
			break;
		}
		case 's':{
			if(mapa[y+1][x-WIDTH]<=3){
				y++;
				**keepMove = **tecla;				
				return y;
			}else{
				**tecla = **keepMove;
				return y;}
			break;
		}
		default:{
			return y;
			break;
		}
	}
}
void screenPoint(){
 int i,j=0,x,y;
  
  for(i=0;i<3-j;i++){
	   if(i==0){
         x=35;
         y=3;
	    }
	   else if(i==1){
	   	 x=104;
	   	 y=3;
	    }
	   else if(i==2){
	   	 if(j==0){
	   	 x=35;
	   	 y=9;
		   }
	    }
		// desenha as tabelas de timer, vida e pontos;
	 gotoxy(x,y);
	 printf("%c%c%c%c%c%c%c%c%c%c%c",201,205,205,205,205,205,205,205,205,205,187);
	 gotoxy(x,y+1);
	 printf("%c",186);
	 if(i==0){
	 	gotoxy(x+3,y+1);
	    printf("SCORE");
	 }
	 else if(i==1){
	 	gotoxy(x+3,y+1);
	    printf("LIFES");
	 }
	 else if(i==2){
	 	gotoxy(x+3,y+1);
	    printf("TIMER");
	 }
	 gotoxy(x+10,y+1);
	 printf("%c",186);
	 gotoxy(x,y+2);
	 printf("%c",186);
	 gotoxy(x+10,y+2);
	 printf("%c",186);
	 gotoxy(x,y+3);
	 printf("%c%c%c%c%c%c%c%c%c%c%c",200,205,205,205,205,205,205,205,205,205,188);
	 gotoxy(0,0);
	}
}
void pontuacao(int *score, int mapa[][30], Pacman* pm){
	switch(mapa[(*pm).posY][(*pm).posX-WIDTH]){
		case 1:{
			(*score)+=10;
			mapa[(*pm).posY][(*pm).posX-WIDTH] = 0;
			break;
		}
		case 2:{
			(*score)+=50;
			mapa[(*pm).posY][(*pm).posX-WIDTH] = 0;
			break;
		}
	}
	//vai na posição onde printa o score dentro da tabela desenhada pela screenPoint
	gotoxy(37,5);
	printf("  %d",(*score));
	//tirar daqui
	gotoxy(106,5);
	printf("  %d",(*pm).Life);
	gotoxy(0,0);
}
int calculadist(int PMx, int PMy, int GHx, int GHy){
	return sqrt(pow(GHx-PMx,2) + pow(GHy-PMy,2));
}
void movGhost(Ghost* ghost, Pacman* pacman, int mapa[][30]){

	if((*ghost).oldX != (*ghost).posX){
		(*ghost).oldX = (*ghost).posX;
	}
	if((*ghost).oldY != (*ghost).posY){
		(*ghost).oldY = (*ghost).posY;
	}
 
  	if((*ghost).kill == 0){
		if(rand()%100 < 70){
			perseguePac(&ghost, &pacman, mapa);
		}
	} else{
		if(rand()%100 < 50){
			fogePac(&ghost, &pacman, mapa);
		} 
	}
}
void movGhost2(Ghost* ghost, Pacman* pacman, int mapa[][30]){

	int dist_menor = calculadist((*ghost).posX,(*ghost).posY,(*pacman).posX,(*pacman).posY);

	//ifs para nao deixar os personagens se apagarem caso parados
	if((*ghost).oldX != (*ghost).posX){
		(*ghost).oldX = (*ghost).posX;
	}
	if((*ghost).oldY != (*ghost).posY){
		(*ghost).oldY = (*ghost).posY;
	}

	if(dist_menor < 5){

		if((*ghost).kill == 0){
		if(rand()%100 < 80){
			perseguePac(&ghost, &pacman, mapa);
		}
	} else {
		if(rand()%100 < 80){
			fogePac(&ghost, &pacman, mapa);
			} 
		}
	} else {
		randomMov(&ghost, mapa);
	}
}
void teleport(Pacman **pm){
	if((**pm).posX == WIDTH){
		(**pm).posX = WIDTH + 29; 
	} else if((**pm).posX == WIDTH + 29){
		(**pm).posX = WIDTH;
	}	
}
void teleportGhost(Ghost*** ghost){
	if((***ghost).posX == WIDTH){
			(***ghost).posX = WIDTH +29;
	} else if((***ghost).posX == WIDTH +29){
		(***ghost).posX = WIDTH;
	}
}
void checaEstado(Pacman* pm, Ghost* ghost, int* qtd_comeu, int* score){

	if((*pm).posX == (*ghost).posX && (*pm).posY == (*ghost).posY){
		switch((*ghost).kill){
			case 0:{
				(*pm).Life --;
				(*pm).posX=74;
				(*pm).posY=23;
				delay(1000);
				break;
			}
			case 1:{
				//respawn no ghost
				(*ghost).posX = 74;
				(*ghost).posY = 13;
				(*ghost).kill = 0;
				(*ghost).charact = 'w';

				//showScreen
				gotoxy(75,33);
				printf("+%d", 100*(*qtd_comeu));
				delay(250);
				gotoxy(75,33);
				printf("              ");
				
				//logica
				(*score) +=100*(*qtd_comeu);
				(*qtd_comeu)++;
				break;
			}
		}
	}	
}
void perseguePac(Ghost** ghost, Pacman** pacman, int mapa[][30]){
	int GHx_menor = (**ghost).posX;
	int GHy_menor = (**ghost).posY;
	int dist_menor;

	dist_menor = calculadist((**ghost).posX,(**ghost).posY,(**pacman).posX,(**pacman).posY);

	if((mapa[(**ghost).posY][(**ghost).posX - (1+WIDTH)] < 3) && (calculadist(((**ghost).posX-1),(**ghost).posY,(**pacman).posX,(**pacman).posY) < dist_menor)){
		GHx_menor = ((**ghost).posX - 1);
		GHy_menor = (**ghost).posY;
		}	
		else if((mapa[(**ghost).posY][((**ghost).posX + (1-WIDTH))] < 3 ) && (calculadist(((**ghost).posX +1),(**ghost).posY,(**pacman).posX,(**pacman).posY) < dist_menor))
		{
		GHx_menor = ((**ghost).posX) + 1;
		GHy_menor = (**ghost).posY;
		}
		else if((mapa[(**ghost).posY-1][((**ghost).posX - WIDTH)] < 3 ) && (calculadist((**ghost).posX ,((**ghost).posY-1),(**pacman).posX,(**pacman).posY) < dist_menor))
		{
		GHx_menor = (**ghost).posX ;
		GHy_menor = ((**ghost).posY - 1);
		}
		else if((mapa[(**ghost).posY + 1][((**ghost).posX - WIDTH)] < 3) && (calculadist((**ghost).posX ,((**ghost).posY+1),(**pacman).posX,(**pacman).posY) < dist_menor))
		{
		GHx_menor = (**ghost).posX ;
		GHy_menor = ((**ghost).posY + 1);
		}
	

	if((**ghost).posX  == GHx_menor && ((**ghost).posY == GHy_menor)){
		if(( pow( ((**ghost).posX -1) - (**pacman).posX ,2) < pow( ((**ghost).posX - (**pacman).posX) ,2))  && (mapa[(**ghost).posY][(**ghost).posX - (1+WIDTH)] < 3)){
			GHx_menor = (**ghost).posX -1;
		} else if(( pow( ((**ghost).posX +1) - (**pacman).posX, 2) < pow( ((**ghost).posX - (**pacman).posX), 2)) && (mapa[(**ghost).posY][(**ghost).posX + (1-WIDTH)] < 3)){
			GHx_menor = (**ghost).posX +1;
		}

		if(( pow( ((**ghost).posY-1) - (**pacman).posY ,2) < pow( ((**ghost).posY - (**pacman).posY) , 2)) && (mapa[(**ghost).posY-1][(**ghost).posX -WIDTH] < 3)){
			GHy_menor = (**ghost).posY-1;
		} else if(( pow( ((**ghost).posY+1) - (**pacman).posY, 2) < pow( ((**ghost).posY - (**pacman).posY), 2)) && (mapa[(**ghost).posY+1][(**ghost).posX -WIDTH] < 3)){
			GHy_menor = (**ghost).posY+1;
		}
	}

	(**ghost).posX  = GHx_menor;
	(**ghost).posY  = GHy_menor;
	teleportGhost(&ghost);
}
void fogePac(Ghost** ghost, Pacman** pacman, int mapa[][30]){
	int GHx_menor = (**ghost).posX;
	int GHy_menor = (**ghost).posY;
	int dist_menor;

	dist_menor = calculadist((**ghost).posX,(**ghost).posY,(**pacman).posX,(**pacman).posY);

	if((mapa[(**ghost).posY][(**ghost).posX - (1+WIDTH)] < 3) && (calculadist(((**ghost).posX-1),(**ghost).posY,(**pacman).posX,(**pacman).posY) > dist_menor)){
		GHx_menor = ((**ghost).posX - 1);
		GHy_menor = (**ghost).posY;
		}	
		else if((mapa[(**ghost).posY][((**ghost).posX + (1-WIDTH))] < 3 ) && (calculadist(((**ghost).posX +1),(**ghost).posY,(**pacman).posX,(**pacman).posY) > dist_menor))
		{
		GHx_menor = ((**ghost).posX) + 1;
		GHy_menor = (**ghost).posY;
		}	
		else if((mapa[(**ghost).posY-1][((**ghost).posX - WIDTH)] < 3 ) && (calculadist((**ghost).posX ,((**ghost).posY-1),(**pacman).posX,(**pacman).posY) > dist_menor))
		{
		GHx_menor = (**ghost).posX ;
		GHy_menor = ((**ghost).posY - 1);
		}
		else if((mapa[(**ghost).posY + 1][((**ghost).posX - WIDTH)] < 3) && (calculadist((**ghost).posX ,((**ghost).posY+1),(**pacman).posX,(**pacman).posY) > dist_menor))
		{
		GHx_menor = (**ghost).posX ;
		GHy_menor = ((**ghost).posY + 1);
		}
	

	if((**ghost).posX  == GHx_menor && ((**ghost).posY == GHy_menor)){

		if(( pow( (**ghost).posX -1 - (**pacman).posX ,2) > pow( ((**ghost).posX - (**pacman).posX) ,2))  && (mapa[(**ghost).posY][(**ghost).posX - (1+WIDTH)] < 3)){
			GHx_menor = (**ghost).posX -1;
		} else if(( pow( ((**ghost).posX +1) - (**pacman).posX, 2) > pow( ((**ghost).posX - (**pacman).posX), 2)) && (mapa[(**ghost).posY][(**ghost).posX + (1-WIDTH)] < 3)){
			GHx_menor = (**ghost).posX +1;
		}

		if(( pow( ((**ghost).posY-1) - (**pacman).posY ,2) > pow( ((**ghost).posY - (**pacman).posY) , 2)) && (mapa[(**ghost).posY-1][(**ghost).posX - WIDTH] < 3)){
			GHy_menor = (**ghost).posY-1;
		} else if(( pow( ((**ghost).posY+1) - (**pacman).posY, 2) > pow( ((**ghost).posY - (**pacman).posY), 2)) && (mapa[(**ghost).posY+1][(**ghost).posX - WIDTH] < 3)){
			GHy_menor = (**ghost).posY+1;
		}
	}

	(**ghost).posX  = GHx_menor;
	(**ghost).posY  = GHy_menor;
	teleportGhost(&ghost);
}
void randomMov(Ghost** ghost, int mapa[][30]){
	
	switch((**ghost).mov){
		case 'w':{
			if(mapa[(**ghost).posY][(**ghost).posX  - (1+WIDTH)] < 3){
				(**ghost).posX --;
			} else{
				if(rand()%100 < 40){
					(**ghost).mov = 'a';
					randomMov(ghost, mapa);
				}else if(rand()%100 < 80){
					(**ghost).mov = 'd';
					randomMov(ghost, mapa);
				}else{
					(**ghost).mov = 's';
					randomMov(ghost, mapa);
				}
			}
			break;
		}
		case 'a':{
			if(mapa[(**ghost).posY-1][(**ghost).posX  - WIDTH] < 3){
				(**ghost).posY --;
			} else{
				if(rand()%100 < 40){
					(**ghost).mov = 'w';
					randomMov(ghost, mapa);
				}else if(rand()%100 < 80){
					(**ghost).mov = 's';
					randomMov(ghost, mapa);
				}else{
					(**ghost).mov = 'd';
					randomMov(ghost, mapa);
				}
			}
			break;
		}		
		case 's':{
			if(mapa[(**ghost).posY][(**ghost).posX  + (1-WIDTH)] < 3){
				(**ghost).posX ++;
			} else{
				if(rand()%100 < 40){
					(**ghost).mov = 'a';
					randomMov(ghost, mapa);
				}else if(rand()%100 < 80){
					(**ghost).mov = 'd';
					randomMov(ghost, mapa);
				}else{
					(**ghost).mov = 'w';
					randomMov(ghost, mapa);
				}
			}
			break;
		}
		case 'd':{
			if(mapa[(**ghost).posY+1][(**ghost).posX  - WIDTH] < 3){
				(**ghost).posY ++;
			} else{
				if(rand()%100 < 40){
					(**ghost).mov = 'w';
					randomMov(ghost, mapa);
				}else if(rand()%100 < 80){
					(**ghost).mov = 's';
					randomMov(ghost, mapa);
				}else{
					(**ghost).mov = 'a';
					randomMov(ghost, mapa);
				}
			}
			break;
		}
	}
	teleportGhost(&ghost);
}  
void copiaMapa(int mapaO[30][30], int mapa[30][30]){
	int i,j;

	for(i=0;i<30;i++){
		for(j=0;j<30;j++){
			mapa[i][j]=mapaO[i][j];
		}
	}
}
void checaWin(int mapa[][30], Pacman *pm){
	int i,j, soma=0;

	for(i=1;i<29;i++){
		for(j=1;j<29;j++){
			if(mapa[i][j] == 1 ||mapa[i][j] == 2){
				soma ++;
			}
		}
	}

	if(soma == 0){
		(*pm).Life = -1;		
	}
}
