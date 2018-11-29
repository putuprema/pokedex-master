#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <windows.h>
#include <time.h>
#include <conio.h>
#include <ctype.h>
#define pauseScreen(); do {b=getch();} while (b != 13);

char map[102][102]={}, playerOne[16], b, fileName[20];
char pokemonName[31][50], pokemonType[31][50], pokemonCategory[31][50], pokemonTrade[31][50];
int mapRow, mapLength, nameLength, coorX, coorY, money, pokemonCount[31]={}, tradeCount[31]={}, pokedexCount, random, pokeball, greatball, ultraball, masterball, tradeNo;
bool isCaught, isFlee, err;

void init();
void helpMenu();
void readMap();
void printMap();
void readPokedex();
void printPokedex();
void cetakHeader();
void play(int p);
void playerName();
void pokeShop();
void randomEventGrass();
void foundUltraball();
void pokemonEncounter();
void foundGreatball();
void foundPokeball();
void foundMoney();
void attack(int z, int y);
void buyBall();
void sellBall();
void trade();
void tradeList();
void saveGame();
void load();
void sortCount();
void sortName();

int main() {
	int menuInput;
	readMap(); readPokedex();
	// INITIALIZE RANDOM NUMBER GENERATOR
	time_t t;
	srand((unsigned) time(&t));
	// END INITIALIZE RANDOM NUMBER GENERATOR
	mainMenu:
	cetakHeader();
	printf("1. New Game\n");
	printf("2. Load Game\n");
	printf("3. Exit Game\n\n");
	mainChoose:
	printf("Your Choice [1..3]: "); scanf("%d", &menuInput); getchar();

	switch (menuInput) {
		case 1: play(1); goto mainMenu; break;
		case 2: play(2); goto mainMenu; break;
		case 3: goto exit; break;
		default: printf("Invalid input.\n"); goto mainChoose; break;
	}
	
	exit:
	cetakHeader();
	printf("Thanks for playing :)\n");	
	system("pause");
}

void cetakHeader() {
	system("cls");
	printf("###################################################################################\n");
	printf("#                        Pokedex Master by Putu Prema (v1)                        #\n");
	printf("###################################################################################\n\n");	
}

void helpMenu() {
	cetakHeader();
	printf("Key Information\n===============\n");
	printf("To move use key [w|a|s|d]\n");
	printf("To open your pokedex use key [p]\n");
	printf("To exit and save the game use key [x]\n\n\n");
	printf("Map Information\n===============\n");
	printf("grass [v] is where you can find pokemon, item, or money\n");
	printf("shop [s] is where you can open shop menu to sell or buy item\n\n\n");
	printf("Press enter to continue..."); pauseScreen();
}

void init() {
	coorX=coorY=1;
	pokeball=greatball=ultraball=masterball=money=pokedexCount=tradeNo=err=0;
	for (int i=1; i<=30; i++) {
		pokemonCount[i] = 0; tradeCount[i] = 0;
	}
	// INIT FOR TRADE TESTING
//	pokemonCount[15] = 7;
//	tradeNo++; strcpy(pokemonTrade[tradeNo],pokemonName[15]); tradeCount[tradeNo] = pokemonCount[15];
	// END INIT FOR TRADE TESTING
}

void pokeShop() {
	int menuShop;
	cetakHeader();
	printf("PokeShop\n===========\nMoney: %d\n===========\n", money);
	printf("1. Buy Pokeballs\n");
	printf("2. Sell Pokeballs\n");
	printf("3. Trade Pokemon\n");
	printf("4. Exit\n");
	choose:
	printf("Choose [1..4]: "); scanf("%d", &menuShop); getchar();
	switch (menuShop) {
		case 1 : buyBall(); break;
		case 2 : sellBall(); break;
		case 3 : trade(); break;
		case 4 : break;
		default : goto choose;
	}
}

void trade() {
	int menuTrade, qty, no;
	cetakHeader();
	tradeList();
	printf("1. Sort By Count\n");
	printf("2. Sort By Name\n");
	printf("3. Trade Pokemon | 1000 each\n");
	printf("4. Exit\n");
	tradeChoose:
	printf("Choose [1..4]: "); scanf("%d", &menuTrade); getchar();
	switch (menuTrade) {
		case 1 : sortCount(); trade(); break;
		case 2 : sortName(); trade(); break;
		case 3 : 
			do {printf("Which pokemon do you want to trade ? [1..%d]: ", tradeNo); scanf("%d", &no); getchar();} while (no < 1 || no > tradeNo);
			do {printf("How many do you want to trade? [1..9]: "); scanf("%d", &qty); getchar();} while (qty < 1 || qty > 9);
			if (tradeCount[no] < qty) {printf("You only have %d %s(s)\n", tradeCount[no], pokemonTrade[no]); printf("Press enter to continue..."); pauseScreen(); trade(); break;}
			else {
				money += (1000*qty); printf("You got %d from trading your pokemon!\n", 1000*qty); printf("Press enter to continue..."); pauseScreen(); 
				tradeCount[no] -= qty; pokedexCount--;
				for (int i=1; i<=30; i++) {
					if (pokemonCount[i] != 0) {
						if (strcmp(pokemonTrade[no],pokemonName[i]) == 0) {
							pokemonCount[i]=tradeCount[no];
							break;
						}
					}
				}
				trade(); break;
			}
		case 4 : pokeShop(); break;
		default : goto tradeChoose; break;
	}
}

void sortCount() {
	char *temp; int tempC;
	for (int i=1; i<=tradeNo; i++) {
		for (int j=i+1; j<=tradeNo; j++) {
			if (tradeCount[i] < tradeCount[j]) {
				strcpy(temp,pokemonTrade[i]); tempC = tradeCount[i];
				strcpy(pokemonTrade[i],pokemonTrade[j]); tradeCount[i] = tradeCount[j];
				strcpy(pokemonTrade[j],temp); tradeCount[j] = tempC;						
			}
		}
	}	
}

void sortName() {
	for (int i=1; i<=tradeNo; i++) {
		for (int j=i+1; j<=tradeNo; j++) {
			char *temp; int tempC;
			if (pokemonTrade[i][0] > pokemonTrade[j][0]) {
				strcpy(temp,pokemonTrade[i]); tempC = tradeCount[i];
				strcpy(pokemonTrade[i],pokemonTrade[j]); tradeCount[i] = tradeCount[j];
				strcpy(pokemonTrade[j],temp); tradeCount[j] = tempC;
			} else if (pokemonTrade[i][0] == pokemonTrade[j][0]) {
				if (pokemonTrade[i][1] > pokemonTrade[j][1]) {
					strcpy(temp,pokemonTrade[i]); tempC = tradeCount[i];
					strcpy(pokemonTrade[i],pokemonTrade[j]); tradeCount[i] = tradeCount[j];
					strcpy(pokemonTrade[j],temp); tradeCount[j] = tempC;
				} else if (pokemonTrade[i][1] == pokemonTrade[j][1] && pokemonTrade[i][2] > pokemonTrade[j][2]) {
					strcpy(temp,pokemonTrade[i]); tempC = tradeCount[i];
					strcpy(pokemonTrade[i],pokemonTrade[j]); tradeCount[i] = tradeCount[j];
					strcpy(pokemonTrade[j],temp); tradeCount[j] = tempC;							
				} else if (pokemonTrade[i][2] == pokemonTrade[j][2] && pokemonTrade[i][3] > pokemonTrade[j][3]) {
					strcpy(temp,pokemonTrade[i]); tempC = tradeCount[i];
					strcpy(pokemonTrade[i],pokemonTrade[j]); tradeCount[i] = tradeCount[j];
					strcpy(pokemonTrade[j],temp); tradeCount[j] = tempC;							
				} else if (pokemonTrade[i][3] == pokemonTrade[j][3] && pokemonTrade[i][4] > pokemonTrade[j][4]) {
					strcpy(temp,pokemonTrade[i]); tempC = tradeCount[i];
					strcpy(pokemonTrade[i],pokemonTrade[j]); tradeCount[i] = tradeCount[j];
					strcpy(pokemonTrade[j],temp); tradeCount[j] = tempC;							
				} else if (pokemonTrade[i][4] == pokemonTrade[j][4] && pokemonTrade[i][5] > pokemonTrade[j][5]) {
					strcpy(temp,pokemonTrade[i]); tempC = tradeCount[i];
					strcpy(pokemonTrade[i],pokemonTrade[j]); tradeCount[i] = tradeCount[j];
					strcpy(pokemonTrade[j],temp); tradeCount[j] = tempC;							
				}
			}
		}
	}
}

void buyBall() {
	int menuShop, qty;
	cetakHeader();
	printf("Buy Pokeballs\n=============\nMoney: %d\n=============\n", money);
	printf("1. Pokeball   | 500  | You have: %d\n", pokeball);
	printf("2. Greatball  | 1000 | You have: %d\n", greatball);
	printf("3. Ultraball  | 2000 | You have: %d\n", ultraball);
	printf("4. Masterball | 9999 | You have: %d\n", masterball);
	printf("5. Exit\n");
	buyChoose:
	printf("Choose [1..5]: "); scanf("%d", &menuShop);
	switch (menuShop) {
		case 1 : 
			printf("How many do you want to buy ? "); scanf("%d", &qty); getchar(); if (qty < 1) {printf("Invalid input!\n"); printf("Press enter to continue..."); pauseScreen(); buyBall(); break;}
			if (money < (500*qty)) {
				printf("You don\'t have enough money!\n"); printf("Press enter to continue..."); pauseScreen(); buyBall(); break;
			} else {
				pokeball += qty; money -= (500*qty); printf("Thank you for buying."); pauseScreen(); buyBall(); break;
			}
		case 2 : 
			printf("How many do you want to buy ? "); scanf("%d", &qty); getchar(); if (qty < 1) {printf("Invalid input!\n"); printf("Press enter to continue..."); pauseScreen(); buyBall(); break;}
			if (money < (1000*qty)) {
				printf("You don\'t have enough money!\n"); printf("Press enter to continue..."); pauseScreen(); buyBall(); break;
			} else {
				greatball += qty; money -= (1000*qty); printf("Thank you for buying."); pauseScreen(); buyBall(); break;
			}
		case 3 :
			printf("How many do you want to buy ? "); scanf("%d", &qty); getchar(); if (qty < 1) {printf("Invalid input!\n"); printf("Press enter to continue..."); pauseScreen(); buyBall(); break;}
			if (money < (2000*qty)) {
				printf("You don\'t have enough money!\n"); printf("Press enter to continue..."); pauseScreen(); buyBall(); break;
			} else {
				ultraball += qty; money -= (2000*qty); printf("Thank you for buying."); pauseScreen(); buyBall(); break;
			}
		case 4 :
			printf("How many do you want to buy ? "); scanf("%d", &qty); getchar(); if (qty < 1) {printf("Invalid input!\n"); printf("Press enter to continue..."); pauseScreen(); buyBall(); break;}
			if (money < (9999*qty)) {
				printf("You don\'t have enough money!\n"); printf("Press enter to continue..."); pauseScreen(); buyBall(); break;
			} else {
				masterball += qty; money -= (9999*qty); printf("Thank you for buying."); pauseScreen(); buyBall(); break;
			}
		case 5 : pokeShop(); break;
		default : goto buyChoose;
	}	
}

void sellBall() {
	int menuShop, qty;
	cetakHeader();
	printf("Sell Pokeballs\n==============\nMoney: %d\n==============\n", money);
	printf("1. Pokeball   | 250  | You have: %d\n", pokeball);
	printf("2. Greatball  | 500  | You have: %d\n", greatball);
	printf("3. Ultraball  | 1000 | You have: %d\n", ultraball);
	printf("4. Masterball | 4999 | You have: %d\n", masterball);
	printf("5. Exit\n");
	sellChoose:
	printf("Choose [1..5]: "); scanf("%d", &menuShop);
	switch (menuShop) {
		case 1 : 
			printf("How many do you want to sell ? "); scanf("%d", &qty); getchar(); if (qty < 1) {printf("Invalid input!\n"); printf("Press enter to continue..."); pauseScreen(); sellBall(); break;}
			if (pokeball < qty || pokeball == 0) {
				printf("You don\'t have that much Pokeballs!\n"); printf("Press enter to continue..."); pauseScreen(); sellBall(); break;
			} else {
				pokeball -= qty; money += (250*qty); printf("Thank you for selling."); pauseScreen(); sellBall(); break;
			}
		case 2 : 
			printf("How many do you want to sell ? "); scanf("%d", &qty); getchar(); if (qty < 1) {printf("Invalid input!\n"); printf("Press enter to continue..."); pauseScreen(); sellBall(); break;}
			if (greatball < qty || greatball == 0) {
				printf("You don\'t have that much Greatballs!\n"); printf("Press enter to continue..."); pauseScreen(); sellBall(); break;
			} else {
				greatball -= qty; money += (500*qty); printf("Thank you for selling."); pauseScreen(); sellBall(); break;
			}
		case 3 :
			printf("How many do you want to sell ? "); scanf("%d", &qty); getchar(); if (qty < 1) {printf("Invalid input!\n"); printf("Press enter to continue..."); pauseScreen(); sellBall(); break;}
			if (ultraball < qty || ultraball == 0) {
				printf("You don\'t have that much Ultraballs!\n"); printf("Press enter to continue..."); pauseScreen(); sellBall(); break;
			} else {
				ultraball -= qty; money += (1000*qty); printf("Thank you for selling."); pauseScreen(); sellBall(); break;
			}
		case 4 :
			printf("How many do you want to sell ? "); scanf("%d", &qty); getchar(); if (qty < 1) {printf("Invalid input!\n"); printf("Press enter to continue..."); pauseScreen(); sellBall(); break;}
			if (masterball < qty || masterball == 0) {
				printf("You don\'t have that much Masterballs!\n"); printf("Press enter to continue..."); pauseScreen(); sellBall(); break;
			} else {
				masterball -= qty; money += (4999*qty); printf("Thank you for selling."); pauseScreen(); sellBall(); break;
			}
		case 5 : pokeShop(); break;
		default : goto sellChoose;
	}	
}

void playerName() {
	int adaAngka, adaHuruf;
	cetakHeader();
	printf("Welcome new adventurer, may I have your name ?\n");
	do {
		adaAngka=adaHuruf=0;
		printf("My name is [5..15][alpha numeric]: ");
		scanf("%s", playerOne); //getchar();
		nameLength = strlen(playerOne);
		for (int i=0; i<nameLength; i++) {
			if (isalpha(playerOne[i]) != 0 && adaHuruf==0) adaHuruf=1;
			if (isdigit(playerOne[i]) != 0 && adaAngka==0) adaAngka=1;
			if (adaHuruf == 1 && adaAngka == 1) break;
		}		
	} while ((strlen(playerOne) < 5 || strlen(playerOne) > 15) || (adaHuruf != adaAngka));
	strcpy(fileName,playerOne);
	strcat(fileName,".txt");
}

void load() {
	int adaAngka, adaHuruf;
	char dump[50];
	cetakHeader();
	printf("Aahh. Welcome back. I have forgotten your name. May I ask your name again ?\n");
	loadSaved:
	do {
		adaAngka=adaHuruf=0;
		printf("My name is [5..15][alpha numeric]: ");
		scanf("%s", playerOne); //getchar();
		nameLength = strlen(playerOne);
		for (int i=0; i<nameLength; i++) {
			if (isalpha(playerOne[i]) != 0 && adaHuruf==0) adaHuruf=1;
			if (isdigit(playerOne[i]) != 0 && adaAngka==0) adaAngka=1;
			if (adaHuruf == 1 && adaAngka == 1) break;
		}		
	} while ((strlen(playerOne) < 5 || strlen(playerOne) > 15) || (adaHuruf != adaAngka));
	strcpy(fileName,playerOne);
	strcat(fileName,".txt");
	FILE *fgame;
	fgame = fopen(fileName, "r");
	if (fgame == NULL) {
		err=1; printf("Username not available.\n"); printf("Press enter to go back to main menu..."); fclose(fgame); pauseScreen();
	} else {
		fscanf(fgame, "%d#%d#%d#%d#%d\n", &money, &pokeball, &greatball, &ultraball, &masterball);
		for (int i=1; i<=30; i++) {
			fscanf(fgame, "%[^#]#%[^#]#%d#%[^\n]\n", dump, dump, &pokemonCount[i], dump);
			pokedexCount += pokemonCount[i];
			if (pokemonCount[i] != 0) {
				tradeNo++;
				strcpy(pokemonTrade[tradeNo],pokemonName[i]); tradeCount[tradeNo] = pokemonCount[i];
			}
		}
		fclose(fgame);	
	}
}

void play(int p) {
	init();
	if (p==1) playerName(); 
	else if (p==2) load();
	if (err==1);
	else {
		char a;
		do {
			if (map[coorY][coorX] == 'S') pokeShop();
			else if (map[coorY][coorX] == 'v') randomEventGrass();
			cetakHeader();
			puts(playerOne);
			for (int i=1; i<=nameLength; i++) printf("-");
			printf("\n\n"); printMap();
			a = getch();
			switch (a) {
				case 'w' : if (coorY > 1 && map[coorY-1][coorX] != '#') coorY--; break;
				case 's' : if (coorY < mapRow-2 && map[coorY+1][coorX] != '#') coorY++; break;
				case 'a' : if (coorX > 1 && map[coorY][coorX-1] != '#') coorX--; break;
				case 'd' : if (coorX < mapLength-2 && map[coorY][coorX+1] != '#') coorX++; break;
				case 'h' : helpMenu(); break;
				case 'p' : printPokedex(); break;
			}
		} while (a != 'x');
		printf("Saving game...\n"); sleep(1); saveGame(); printf("Press enter to go back to main menu...") pauseScreen();	
	}
}

void randomEventGrass() {
	double val = (double)rand() / RAND_MAX;
//	bool isNothing = (rand() % 100) < 70;
//	if (isNothing == 1) random = 0;
//	else {
//		if (val < 0.01) { // 1% (ultraball)
//			random = 1;
//		} else if (val < 0.10) { // 1% + 5% (random pokemon)
//			random = 2;
//		} else if (val < 0.11) { // 1% + 5% + 5% (greatball)
//			random = 3;
//		} else if (val < 0.20) { // 1% + 5% + 5% + 9% (pokeball)
//			random = 4;
//		} else if (val < 0.30) { // 1% + 5% + 5% + 9% + 10% (money)
//			random = 5;
//		} else random = 0;
//	}
	bool isNothing = (rand() % 100) < 70;
	if (isNothing == 1) random = 0;
	else {
		bool isMoney = (rand() % 100) < 10;
		if (isMoney == 1) random = 5;
		else {
			bool isPokeball = (rand() % 100) < 9;
			if (isPokeball == 1) random = 4;
			else {
				bool isGreatball = (rand() % 100) < 5;
				if (isGreatball == 1) random = 3;
				else {
					bool isPokemon = (rand() % 100) < 5;
					if (isPokemon == 1) random = 2;
					else {
						bool isUltraball = (rand() % 100) < 1;
						if (isUltraball == 1) random = 1;
					}
				}
			}
		}
	}
	switch (random) {
		case 0 : break;
		case 1 : foundUltraball(); break;
		case 2 : pokemonEncounter(); break;
		case 3 : foundGreatball(); break;
		case 4 : foundPokeball(); break;
		case 5 : foundMoney(); break;
	}
}

void attack(int z, int y) {
	bool pokemonExist;
	switch (z) {
		case 1 : isCaught = (rand() % 100) < 10; break;
		case 2 : isCaught = (rand() % 100) < 20; break;
		case 3 : isCaught = (rand() % 100) < 50; break;
		case 4 : isCaught = (rand() % 100) < 100; break;
	}
	isFlee = (rand() % 100) < 20;
	printf("..!"); fflush(stdout); sleep(1);
	for (int i=1; i<=2; i++) {
		printf(" ..!");
		fflush(stdout);
		sleep(1);
	}
	if (isCaught == 1) {
		printf(" You have cought %s\n", pokemonName[y]); pokemonCount[y]++; pokedexCount++;
		for (int i=1; i<=tradeNo; i++) {
			if (strcmp(pokemonTrade[i], pokemonName[y]) == 0) {
				pokemonExist=1;
				tradeCount[i]++;
				break;
			} else {
				pokemonExist=0;
			}
		}
		if (pokemonExist==0) {tradeNo++; strcpy(pokemonTrade[tradeNo],pokemonName[y]); tradeCount[tradeNo] = pokemonCount[y];}
		printf("Press enter to continue..."); pauseScreen();
	} else {
		if (isFlee == 1) {
			printf(" The pokemon broke away\n"); printf("%s has flee!", pokemonName[y]); pauseScreen();
		} else {
			printf(" The pokemon broke away\n"); printf("Press enter to continue..."); pauseScreen();
		}
	}
}

void pokemonEncounter() {
	int menuAttack, pokemonNo = 1 + rand() / (RAND_MAX / (30 - 1 + 1) + 1), length=strlen(pokemonName[pokemonNo]);
	printf("You have encountered a wild legendary pokemon\nPress enter to continue...");
	pauseScreen(); cetakHeader(); printf("Wild %s has appeared!", pokemonName[pokemonNo]); pauseScreen();
	attackMenu:
	cetakHeader(); printf("%s\n", pokemonName[pokemonNo]);
	for (int i=1; i<=length; i++) printf("=");
	printf("\nWhat will you do ?\n");
	printf("1. Use Pokeball (%d left)\n", pokeball);
	printf("2. Use Greatball (%d left)\n", greatball);
	printf("3. Use Ultraball (%d left)\n", ultraball);
	printf("4. Use Masterball (%d left)\n", masterball);
	printf("5. Flee\n");
	attackChoose:
	printf("Choose [1..5]: "); scanf("%d", &menuAttack); getchar();
	switch (menuAttack) {
		case 1 : 
			if (pokeball == 0) {
				printf("You don\'t have any Pokeballs!"); pauseScreen(); goto attackMenu;
			} else pokeball--; printf("Throwing a Pokeball !\n"); attack(1,pokemonNo); break;
		case 2 : 
			if (greatball == 0) {
				printf("You don\'t have any Greatballs!"); pauseScreen(); goto attackMenu;
			} else greatball--; printf("Throwing a Greatball !\n"); attack(2,pokemonNo); break;
		case 3 : 
			if (ultraball == 0) {
				printf("You don\'t have any Ultraballs!"); pauseScreen(); goto attackMenu;
			} else ultraball--; printf("Throwing a Ultraball !\n"); attack(3,pokemonNo); break;
		case 4 : 
			if (masterball == 0) {
				printf("You don\'t have any Masterballs!"); pauseScreen(); goto attackMenu;
			} else masterball--; printf("Throwing a Masterball !\n"); attack(4,pokemonNo); break;
		case 5 : printf("You have escaped from %s", pokemonName[pokemonNo]); pauseScreen(); break;
		default: printf("Invalid input.\n"); goto attackChoose; break;
	}
	if (isFlee == 0 && menuAttack != 5 && isCaught == 0) goto attackMenu;
}

void foundPokeball() {
	pokeball++;
	printf("You have found a Pokeball!\n");
	printf("You have %d Pokeball(s) now!\n", pokeball);
	printf("Press enter to continue..."); pauseScreen();
}

void foundGreatball() {
	greatball++;
	printf("You have found a Greatball!\n");
	printf("You have %d Greatball(s) now!\n", greatball);
	printf("Press enter to continue..."); pauseScreen();
}

void foundUltraball() {
	ultraball++;
	printf("You have found an Ultraball!\n");
	printf("You have %d Ultraball(s) now!\n", ultraball);
	printf("Press enter to continue..."); pauseScreen();
}

void foundMoney() {
	int gotMoney = 50 + rand() / (RAND_MAX / (100 - 50 + 1) + 1);
	money += gotMoney;
	printf("You got %d money(s)\n", gotMoney);
	printf("You have %d money(s) now\n", money);
	printf("Press enter to continue..."); pauseScreen();
}

void saveGame() {
	FILE *fgame;
	fgame = fopen(fileName, "w+");
	fprintf(fgame, "%d#%d#%d#%d#%d\n", money, pokeball, greatball, ultraball, masterball);
	for (int i=1; i<=30; i++) {
		if (pokemonCount[i] == 0) fprintf(fgame, "?????#???#0#?????");
		else fprintf(fgame, "%s#%s#%d#%s", pokemonName[i], pokemonType[i], pokemonCount[i], pokemonCategory[i]);
		fprintf(fgame, "\n");
	}
	fclose(fgame);
}

void readMap() {
	mapRow=1;
	FILE *fmap;
	fmap = fopen("map.txt", "r");
	
	char temp = fgetc(fmap);
	while (temp != EOF) {
		if (temp == 10) mapRow++;
		temp = fgetc(fmap);
	}
	fclose(fmap);
	
	fmap = fopen("map.txt", "r");
	char buffMap[100]; fgets(buffMap, 255, (FILE*)fmap); mapLength = strlen(buffMap)-1;
	fclose(fmap);
	
	fmap = fopen("map.txt", "r");
	for (int i=0; i<mapRow; i++) {
		fgets(map[i], 255, (FILE*)fmap); map[i][mapLength]='\0';
	}
	fclose(fmap);
}

void printMap() {
	for (int i=0; i<mapRow; i++) {
		if (i != coorY) printf("%s", map[i]);
		else {
			for (int j=0; j<mapLength; j++) {
				if (i == coorY && j == coorX) printf("@");
				else printf("%c", map[i][j]);
			}	
		}
		if (i==0) printf("\t\t==========================");
		else if (i==1) printf("\t\t|       Your Stats       |");
		else if (i==2) printf("\t\t==========================");
		else if (i==3) printf("\t\t| Money      : %-9d |", money);
		else if (i==4) printf("\t\t| Pokedex    : %-9d |", pokedexCount);
		else if (i==5) printf("\t\t| Pokeball   : %-9d |", pokeball);
		else if (i==6) printf("\t\t| Greatball  : %-9d |", greatball);
		else if (i==7) printf("\t\t| Ultraball  : %-9d |", ultraball);
		else if (i==8) printf("\t\t| Masterball : %-9d |", masterball);
		else if (i==9) printf("\t\t==========================");
		else if (i==10) printf("\t\t| W,A,S,D -- Move        |");
		else if (i==11) printf("\t\t| H       -- Help        |");
		else if (i==12) printf("\t\t| P       -- Pokedex     |");
		else if (i==13) printf("\t\t| X       -- Save & Exit |");
		else if (i==14) printf("\t\t==========================");
		printf("\n");
	}
	printf("\n");
}

void readPokedex() {
	FILE *fpoke;
	fpoke = fopen("pokedex.txt", "r");
	for (int i=1; i<=30; i++) {
		fscanf(fpoke, "%[^#]#%[^#]#%d#%[^\n]\n", pokemonName[i], pokemonType[i], &pokemonCount[i], pokemonCategory[i]);
	}
	for (int i=1; i<=30; i++) {
		int length = strlen(pokemonType[i]);
		for (int j=0; j<length; j++) {
			if (pokemonType[i][j] == '$') {
				pokemonType[i][j] = '-'; 
				break;
			}
		}
	}
	fclose(fpoke);
}

void printPokedex() {
	char unknown[]="?????";
	cetakHeader();
	printf("Pokedex\n=======\n");
	printf("==========================================================================\n");
	printf("| No  | Name           | Type                | Count | Category          |\n");
	printf("==========================================================================\n");
	for (int no=1; no<=30; no++) {
		printf("| %-4d", no);
		if (pokemonCount[no] == 0) {
			printf("| %-15s", unknown);
			printf("| %-20s", unknown);
			printf("| %-6d", pokemonCount[no]);
			printf("| %-18s|\n", unknown);
		} else {
			printf("| %-15s", pokemonName[no]);
			printf("| %-20s", pokemonType[no]);
			printf("| %-6d", pokemonCount[no]);
			printf("| %-18s|\n", pokemonCategory[no]);
		}
	}
	printf("==========================================================================\n\n");
	printf("Press enter to continue..."); pauseScreen();
}

void tradeList() {
	printf("Pokemon(s) in your Pokedex\n==========================\n");
	printf("================================\n");
	printf("| No  | Name           | Count |\n");
	printf("================================\n");
	for (int i=1; i<=tradeNo; i++) {
		printf("| %-4d", i);
		printf("| %-15s", pokemonTrade[i]);
		printf("| %-6d|\n", tradeCount[i]);
	}
	printf("================================\n");
}
