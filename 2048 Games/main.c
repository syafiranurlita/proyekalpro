//INCLUDE
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>    //CLRSCR, SCREEN SIZE
#include <conio.h>      //GOTO
#include <math.h>       //POW
#include <time.h>       //TIME
#include <string.h>     //concat

//KEY
#define ARROW_LEFT 75
#define ARROW_RIGHT 77
#define ARROW_UP 72
#define ARROW_DOWN 80
#define KEY_ENTER 13
#define KEY_BACKSPACE 8
#define KEY_CTRL_Z 26
#define KEY_ESC 27

//COLOR
#define ANSI_COLOR_RED "\033[0;31m"
#define ANSI_COLOR_BOLD_RED "\033[1;31m"

#define ANSI_COLOR_MAGENTA "\033[0;35m"
#define ANSI_COLOR_BOLD_MAGENTA "\033[1;35m"

#define ANSI_COLOR_BLUE "\033[0;34m"
#define ANSI_COLOR_BOLD_BLUE "\033[1;34m"

#define ANSI_COLOR_CYAN "\033[0;36m"
#define ANSI_COLOR_BOLD_CYAN "\033[1;36m"

#define ANSI_COLOR_YELLOW "\033[0;33m"
#define ANSI_COLOR_BOLD_YELLOW "\033[1;33m"

#define ANSI_COLOR_GREEN "\033[0;32m"
#define ANSI_COLOR_BOLD_GREEN "\033[1;32m"

#define ANSI_COLOR_RESET "\033[0m"

//SCREEN SETTINGS
int SCREEN_WIDHT = 120;
int SCREEN_HEIGHT = 30;

//GAMEPLAY
int cells_size = 3;                 //MAX = 20
int cells[20][4][4];
int undo_max;                       //MAX = 20 - 2
int undo_available;
int is_winner = 0;
int is_move_fail[4] = {0, 0, 0, 0}; //UP, LEFT, DOWN, RIGHT
int clock_game_start = 0;
int clock_game_refresh = 0;

//USER DATA
char user_name[16];
int  user_name_count = 0;

int  user_time = 0;
int  user_time_count = 0;

int  user_score[20];
int  user_score_highest = 0;

char user_time_stamp[24];
int  user_time_stamp_count = 0;

char coloumn_arr[1000];
int coloumn_size = 0;

int i;

int main(){
    getScreenSize();

    WelcomeActivity();

    MenuGameActivity(0);

    return 0;
}


void WelcomeActivity(){
	//WELCOME SCREEN

    getScreenSize();

    printf("\n\n");
    printCenter("SELAMAT DATANG DI GAME 2048", 27);
    printCenter("Enter untuk login", 17);
    printf("\n\n");
    printBorderForm("USER NAME", 9, user_name, user_name_count);

    gotoxy(SCREEN_WIDHT/2 + user_name_count/2, 7);

    while (1){
        if (kbhit()){
            char c = getch();
            if (c == KEY_BACKSPACE){
                if (user_name_count <= 0)
                    continue;

                printf("%c", c);
                printf(" ");
                printf("%c", c);

                user_name[user_name_count] = 0;
                user_name_count--;
            } else if (c == KEY_ENTER){
                if (user_name_count > 3)
                    break;
            } else if (c != ' '){
                if (user_name_count >= 15)
                    continue;
                //username
                user_name[user_name_count] = c;
                user_name_count++;
            } else {
                continue;
            }

            gotoxy(0, 6);
            printBorderForm("USER NAME", 9, user_name, user_name_count);
            gotoxy(SCREEN_WIDHT/2 + user_name_count/2, 7);
        }
    }

    loadGame();
}

void MenuGameActivity(int selection){
	//HOME MENU or DASHBOARD

    getScreenSize();

    system("CLS");

    printf(ANSI_COLOR_RESET);

    printf("\n");

    char buffer[50];
    sprintf(buffer, "Hi %s, welcome to", user_name);
    printCenter(buffer, 15 + user_name_count);

    printCenter("2048 GAME", 9);
    printf("\n");

    buffer[50];
    sprintf(buffer, "Your high score : %6d", user_score_highest);
    printCenter(buffer, 15 + 11);

    buffer[50];
    sprintf(buffer, "Last Attempt : %s", user_time_stamp);
    printCenter(buffer, 15 + user_time_stamp_count);
    printf("\n\n");

    printMenu(selection);

    int is_on_menu = 1;
    while (is_on_menu){
        if (kbhit()){
            char c = getch();
            int is_refresh = 1;
            switch (c){
                case ARROW_UP:
                    selection--;
                    break;

                case ARROW_DOWN:
                    selection++;
                    break;

                case KEY_ENTER:
                    is_on_menu = 0;
                    is_refresh = 0;
                    break;

                default:
                    is_refresh = 0;
                    break;
            }
            if (selection > 4)
                selection = 0;

            if (selection < 0)
                selection = 4;

            if (is_refresh)
                printMenu(selection);
        }
    }

	//KEY_ENTER is pressed
    switch (selection){
        case 0:
            MainGameActivity();
            break;

        case 1:
            MenuGameHighScore();
            break;

        case 2:
            MenuGameHelp();
            break;

        case 3:
            MenuGameAbout();
            break;

        case 4:
            return;
            break;

        default:
            break;

    }
}

void MenuGameHighScore(){

    getScreenSize();

    system("CLS");

    printf("\n\n");
    printBorder("HIGH SCORE", 10, 10);
    printf("\n\n");

    /*-------------- read the file -------------------------------------*/
    char words_char[500];
    memset(words_char, 0, sizeof(words_char));
    int words_count = 0;
    int coloumn = 0;

    char fname[8] = "data.csv";

	FILE * fptr = fopen (fname, "r");
    char str1 = fgetc(fptr);

    getMaxString(str1, fptr);


	fptr = fopen (fname, "r");
    str1 = fgetc(fptr);

	while (str1 != EOF){
        if(str1 != ','){
            words_char[words_count] = str1;
            words_count++;
        } else {
            printf("%s", words_char);
            for(i = 0; i < (coloumn_arr[coloumn]/8 - words_count/8) + 1;i++)
                printf("\t");

            words_count = 0;
            coloumn++;
            memset(words_char, 0, sizeof(words_char));
        }
        if(str1 == '\n'){
            printf("%s", words_char);
            words_count = 0;
            coloumn = 0;
            memset(words_char, 0, sizeof(words_char));
        }
        str1 = fgetc(fptr);
    }
    printf("%s", words_char);
    fclose (fptr);

    printf("\n\n");
    printCenter("EXIT : ESC", 10);
    while (1){
        if (kbhit()){
            if (getch() == KEY_ESC)
                break;
        }
    }
    MenuGameActivity(1);
}

void MenuGameHelp(){

    getScreenSize();

    system("CLS");

    printf("\n\n");
    printBorder("HELP", 4, 4);
    printf("\n\n\n");
    printCenter("EXIT : ESC", 10);

    while (1){
        if (kbhit()){
            if (getch() == KEY_ESC)
                break;
        }
    }
    MenuGameActivity(2);
}

void MenuGameAbout(){

    getScreenSize();

    system("CLS");

    printf("\n\n");
    printBorder("ABOUT", 5, 5);
    printf("\n\n\n");
    printCenter("EXIT : ESC", 10);

    while (1){
        if (kbhit()){
            if (getch() == KEY_ESC)
                break;
        }
    }
    MenuGameActivity(3);
}

void MainGameActivity(){
    getScreenSize();

    loadGame();

    system("CLS");

    int t = clock();
    int a = clock() - 1000;
    int timer = 3000;
    while (clock() - t < timer){
        if (clock() - a > 1000){
            a = clock();
            gotoxy(0,0);
            char buffer[13];
            printf("\n\n\n");
            sprintf(buffer, "Ready in %d", (1000 + timer - (clock() - t))/1000);
            printBorder(buffer, 10, 10);
        }
    }


    system("CLS");
    srand(time(0));

    printf(ANSI_COLOR_RESET);
    printf("\n\n");
    printBorderForm("USERNAME", 8, user_name, user_name_count);

    if (user_time == 0){
        //RESET ALL DATAS
        clock_game_start = clock();
        clock_game_refresh = clock();
        for (i = 0; i < 20; i++){
            user_score[i] = 0;
            int x;
            for (x = 0; x < 4; x++){
            	int y;
                for (y = 0; y < 4; y++){
                    cells[i][x][y] = 0;
                }
            }

            if (i <=3 )
                is_move_fail[i] = 0;
        }

        generateCells();
        generateCells();
        printCells();

        undo_max = cells_size - 1;
        undo_available = 0;
    } else {
        //RESET ALL DATAS
        clock_game_start =  clock() - user_time * 1000;
        clock_game_refresh =  clock() - user_time * 1000;
        for (i = 0; i < 20; i++){
            if (i <=3 )
                is_move_fail[i] = 0;
        }
        printCells();

        undo_max = cells_size - 1;
        undo_available = 0;
    }

    if (cells_size > 20)
        cells_size = 20;

	int is_exit = 0;
	int is_save = 0;
    while (is_exit == 0){
        if (clock() - clock_game_refresh > 1000){
            printCells();
            clock_game_refresh = clock();
        }

        if (kbhit()){
            clock_game_refresh = clock();

            char c = getch();
            //printf("%d", c);

            switch (c){
                case KEY_CTRL_Z:
                    //UNDO : CTRL + Z
                    undoCells();
                    printCells();
                    break;

                case ARROW_LEFT:
                    actionLeft();
                    printCells();
                    break;

                case ARROW_RIGHT:
                    actionRight();
                    printCells();
                    break;

                case ARROW_UP:
                    actionUp();
                    printCells();
                    break;

                case ARROW_DOWN:
                    actionDown();
                    printCells();
                    break;

                case KEY_ESC:
                    system("CLS");
                    printf("\n\n");
                    printBorder("GAME PAUSED", 11, 11);
                    printf("\n\n\n\n");
                    printCenter("CONTINUE : ENTER", 16);
                    printCenter("SAVE : S", 8);
                    printCenter("EXIT : ESC", 10);
                    int pause_time = clock();
                    while (1){
                        if (kbhit()){
                            char c = getch();
                            if (c == 83 || c == 115){
                                //key 's' or 'S' is pressed
                                is_save = 1;
								is_exit = 1;
                                break;
                            } else if (c == KEY_ENTER)
                                break;
                            else if (c == KEY_ESC){
                                is_save = 0;
								is_exit = 1;
								break;
                            }
                        }
                    }
                    clock_game_start = clock_game_start + clock() - pause_time;
                    break;

                default:
                    break;
            }
        }
    }

    if (is_save){
        //jangan hitung nilai akhir, simpan saja
        saveGame(user_score[cells_size - 1], (clock() - clock_game_start)/1000, 1);
    } else {
        //hitung nilai akhir
        saveGame(user_score[cells_size - 1], (clock() - clock_game_start)/1000, 0);
    }
    loadGame();
    MenuGameActivity(0);
}


void loadGame(){
    char date_last[100];
    char words_char[100];
    int words_count = 0;

    int isFound = 0;
    int isProdiFilled = 0;
    int isCreated = 0;

    int column = 0;
    memset(words_char, 0, sizeof(words_char));

    int coloumn = 0;

    char fname[8] = "data.csv";

	FILE * fptr = fopen (fname, "r");
    char str1 = fgetc(fptr);

	while (str1 != EOF){
        if(str1 != ','){
            words_char[words_count] = str1;
            words_count++;
        } else if(str1 != '\n'){
            if(column == 0){
                //USER NAME
                if(isFound == 0){
                    isFound = 1;
                    for(i = 0; i < words_count; i++){
                        if(words_char[i] != user_name[i]){
                            isFound = 0;
                            break;
                        }
                    }
                }
            } else if (column == 1){
                //LAST ATTEMPT
                if(isFound == 1){
                    for(int i = 0; i < words_count; i++){
                        user_time_stamp[i] = words_char[i];
                    }
                    user_time_stamp_count = words_count;
                }
            } else if (column == 2){
                //HIGH SCORE
                if(isFound == 1){
					user_score_highest = 0;
                    for(int i = 0; i < words_count; i++){
                        if (words_char[i] == '\n')
                            continue;
                        user_score_highest = user_score_highest + (words_char[i] - '0') * pow(10, words_count - i - 1);
                    }
                }
            } else if (column == 3){
                //TIME

            } else if (column == 4){
                //CURRENT SCORE
                if(isFound == 1){
                    int sc = 0;
                    for(int i = 0; i < words_count; i++){
                        if (words_char[i] == '\n')
                            continue;
                        sc = sc + (words_char[i] - '0') * pow(10, words_count - i - 1);
                    }
                    user_score[cells_size - 1] = sc;
                    user_score[cells_size - 2] = sc;
                    user_score[cells_size - 3] = sc;
                }
            } else if (column == 5){
                //CURRENT TIME
                if(isFound == 1){
                    int sc = 0;
                    for(int i = 0; i < words_count; i++){
                        if (words_char[i] == '\n')
                            continue;
                        sc = sc + (words_char[i] - '0') * pow(10, words_count - i - 1);
                    }
                    user_time = sc;
                }
            }

            words_count = 0;
            column++;
            memset(words_char, 0, sizeof(words_char));
        }
        if(str1 == '\n'){
            if(isFound == 1){
                int sc = 0;
                char sc_char[6];
                int sc_count = 0;
                int x = 0;
                int y = 0;

                for(int i = 0; i < words_count; i++){
                    if (words_char[i] == '\n')
                        continue;

                    if (words_char[i] == ' ' || words_char[i] == ';'){
                        for (int j = 0; j < sc_count; j++){
                            sc = sc + (sc_char[j] - '0') * pow(10, sc_count - j - 1);
                        }
                        if (sc == 127 || sc == 255 || sc == 511)
                            sc++;

                        if (words_char[i] == ' '){
                            cells[cells_size - 1][x][y] = sc;
                            cells[cells_size - 2][x][y] = sc;
                            cells[cells_size - 3][x][y] = sc;
                            y++;
                        } else {
                            cells[cells_size - 1][x][y] = sc;
                            cells[cells_size - 2][x][y] = sc;
                            cells[cells_size - 3][x][y] = sc;
                            x++;
                            y = 0;
                        }

                        sc_count = 0;
                        memset(sc_char, 0, sizeof(sc_char));
                        sc = 0;
                    } else {
                        sc_char[sc_count] = words_char[i];
                        sc_count++;
                    }
                }

                if(isCreated == 0)
                    isCreated = 1;

                break;
            }

            words_count = 0;
            column = 0;
            memset(words_char, 0, sizeof(words_char));
        }
        str1 = fgetc(fptr);
    }
    fclose (fptr);


    printf("\n");
    printf("CURRENT SCORE : %d\n", user_score[cells_size - 1]);
    printf("CURRENT TIME : %d\n", user_time);
    for (int x = 0; x < 4; x++){
        for (int y = 0; y < 4; y++){
            printf("%4d", cells[cells_size - 1][x][y]);
        }
        printf("\n");
    }

    if(isFound){

        return;
        printf("DITEMUKAN!\n");
        printf("User name : %s\n", user_name);
        printf("Last Attempt : %s\n", user_time_stamp);
        printf("Score : %d\n", user_score_highest);

        printf(" =============== UPDATED! ===============");
    } else {
        user_score_highest = 0;
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);
        sprintf(user_time_stamp, "%02d:%02d:%02d %02d-%02d-%02d", tm.tm_hour, tm.tm_min, tm.tm_sec, tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
        user_time_stamp_count = 19;

        return;
        printf("User name : %s\n", user_name);
        printf("Last Attempt : %s\n", user_time_stamp);
        printf("Score : %d\n", user_score_highest);
        printf("Data tidak ditemukan!");
    }
}

void saveGame(int sc, int tm, int is_save_only){
    char words_char[100];
    int words_count = 0;
    memset(words_char, 0, sizeof(words_char));

    char teks[9999];
    int count = 0;
    int isCreated = 0;
    int column = 0;

    //READ AND FIND
    FILE * fptr = fopen ("data.csv", "r");
    char str1 = fgetc(fptr);

    int isFound = 0;
    int isProdiFilled = 0;

	while (str1 != EOF){
        if(str1 != ','){
            words_char[words_count] = str1;
            words_count++;
        } else {
            if(column == 0){
                //USERNAME
                isFound = 1;
                for(int i = 0; i < words_count; i++){
                    teks[count] = words_char[i];
                    count++;
                    if(words_char[i] != user_name[i]){
                        if (isCreated == 0)
                            isFound = 0;
                    }
                }
                teks[count] = ',';
                count++;
            } else if(column == 1){
                //LAST ATTEMPT
                if(isFound == 1 && isCreated == 0){
                    time_t t = time(NULL);
                    struct tm tm = *localtime(&t);
                    sprintf(user_time_stamp, "%02d:%02d:%02d %02d-%02d-%02d", tm.tm_hour, tm.tm_min, tm.tm_sec, tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
                    user_time_stamp_count = 19;
                    for(int i = 0; i < user_time_stamp_count; i++){
                        teks[count] = user_time_stamp[i];
                        count++;
                    }
                } else {
                    for(int i = 0; i < words_count; i++){
                        teks[count] = words_char[i];
                        count++;
                    }
                }
                teks[count] = ',';
                count++;
            } else if(column == 2){
                //HIGH SCORE
                if(isFound == 1 && isCreated == 0){
                    int val = user_score_highest;
                    if (sc > user_score_highest){
                        val = sc;
                    }
                    char buff[10];
                    sprintf(buff, "%d", val);
                    int a = getIntLength(val);

                    for(int i = 0; i < a; i++){
                        teks[count] = buff[i];
                        count++;
                    }
                } else {
                    for(int i = 0; i < words_count; i++){
                        teks[count] = words_char[i];
                        count++;
                    }
                }
                teks[count] = ',';
                count++;
            } else if(column == 3){
                //TIME TAKEN
                if(isFound == 1 && isCreated == 0){
                    int val = user_time;
                    if (sc > user_score_highest){
                        val = tm;
                    }
                    char buff[10];
                    sprintf(buff, "%d", val);
                    int a = getIntLength(val);

                    for(int i = 0; i < a; i++){
                        teks[count] = buff[i];
                        count++;
                    }
                } else {
                    for(int i = 0; i < words_count; i++){
                        teks[count] = words_char[i];
                        count++;
                    }
                }
                teks[count] = ',';
                count++;
            } else if(column == 4){
                //CURRENT SCORE
                if(isFound == 1 && isCreated == 0){
                    int val = 0;
                    if (is_save_only){
                        val = user_score[cells_size - 1];
                    }
                    char buff[10];
                    sprintf(buff, "%d", val);
                    int a = getIntLength(val);

                    for(int i = 0; i < a; i++){
                        teks[count] = buff[i];
                        count++;
                    }
                } else {
                    for(int i = 0; i < words_count; i++){
                        teks[count] = words_char[i];
                        count++;
                    }
                }
                teks[count] = ',';
                count++;

            } else if(column == 5){
                //CURRENT TIME
                if(isFound == 1 && isCreated == 0){
                    int val = 0;
                    if (is_save_only){
                        val = tm;
                    }
                    char buff[10];
                    sprintf(buff, "%d", val);
                    int a = getIntLength(val);

                    for(int i = 0; i < a; i++){
                        teks[count] = buff[i];
                        count++;
                    }
                } else {
                    for(int i = 0; i < words_count; i++){
                        teks[count] = words_char[i];
                        count++;
                    }
                }
                teks[count] = ',';
                count++;
            }
            words_count = 0;
            column++;
            memset(words_char, 0, sizeof(words_char));
        }
        if(str1 == '\n'){
            if(isFound == 1 && isCreated == 0){
                char buff[100];
                int a = 0;

                if (is_save_only){
                    for (int x = 0; x < 4; x++){
                        for (int y = 0; y < 4; y ++){
                            int val = cells[cells_size - 1][x][y];
                            a = a + getIntLength(val) + 1;
                            if (y == 3){
                                sprintf(buff, "%s%d;",  buff, val);

                            } else {
                                if (x == 0 && y == 0)
                                    sprintf(buff, "%d ", val);
                                else
                                    sprintf(buff, "%s%d ", buff, val);
                            }
                        }
                    }
                } else {
                    sprintf(buff, "0 0 0 0;0 0 0 0;0 0 0 0;0 0 0 0;");
                    a = 32;
                }

                for(int i = 0; i < a; i++){
                    teks[count] = buff[i];
                    count++;
                }
                teks[count] = str1;
                count++;

                isCreated = 1;
            } else {
                for(int i = 0; i < words_count; i++){
                    teks[count] = words_char[i];
                    count++;
                }
            }

            words_count = 0;
            column = 0;
            memset(words_char, 0, sizeof(words_char));
        }
        str1 = fgetc(fptr);
    }
    fclose (fptr);

    if(isFound){
		//WRITE
		printf("\n\n =============== UPDATED! ===============");

    } else {
        //ADD NEW USER
		printf("\n\n =============== ADDED! ===============");

		//USERNAME
		for(int i = 0; i < user_name_count; i++){
            teks[count] = user_name[i];
            count++;
        }
        teks[count] = ',';
        count++;

		//LAST ATTEMPT
        for(int i = 0; i < user_time_stamp_count; i++){
            teks[count] = user_time_stamp[i];
            count++;
        }
        teks[count] = ',';
        count++;

        //HIGH SCORE
        int val = user_score_highest;
       if (sc > user_score_highest){
            val = sc;
        }
        char buff[100];
        sprintf(buff, "%d", val);
        int a = getIntLength(val);

        for(int i = 0; i < a; i++){
            teks[count] = buff[i];
            count++;
        }
        teks[count] = ',';
        count++;

        //TIME
        val = user_time;
        if (sc > user_score_highest){
            val = tm;
        }
        sprintf(buff, "%d", val);
        a = getIntLength(val);

        for(int i = 0; i < a; i++){
            teks[count] = buff[i];
            count++;
        }
        teks[count] = ',';
        count++;

        //CURRENT SCORE
        val = 0;
        if (is_save_only){
            val = user_score[cells_size - 1];
        }
        sprintf(buff, "%d", val);
        a = getIntLength(val);

        for(int i = 0; i < a; i++){
            teks[count] = buff[i];
            count++;
        }
        teks[count] = ',';
        count++;

        //CIRRENT TIME
        val = 0;
        if (is_save_only){
            val = tm;
        }
        sprintf(buff, "%d", val);
        a = getIntLength(val);

        for(int i = 0; i < a; i++){
            teks[count] = buff[i];
            count++;
        }
        teks[count] = ',';
        count++;

        //CURRENT STATE
        a = 0;
        if (is_save_only){
            for (int x = 0; x < 4; x++){
                for (int y = 0; y < 4; y ++){
                    int val = cells[cells_size - 1][x][y];
                    a = a + getIntLength(val) + 1;
                    if (y == 3){
                        sprintf(buff, "%s%d;",  buff, val);

                    } else {
                        if (x == 0 && y == 0)
                            sprintf(buff, "%d ", val);
                        else
                            sprintf(buff, "%s%d ", buff, val);
                    }
                }
            }
        } else {
            sprintf(buff, "0 0 0 0;0 0 0 0;0 0 0 0;0 0 0 0;");
            a = 32;
        }

        for(int i = 0; i < a; i++){
            teks[count] = buff[i];
            count++;
        }

        //NEW LINE
        teks[count] = '\n';
        count++;
    }
    //printf("\nOUTPUT : \n'%s' at %d", teks, count);

    FILE * fptr2 = fopen("data.csv", "w");  // akses edit file dengan mode W = WRITE
    fputs(teks, fptr2);                 	// simpan file
    fclose (fptr2);
}



void actionUp(){
    int is_moved = 0;
    for (int x = 0; x < 4; x++){
        for (int y = 0; y < 4; y++){
            //SUM
            for (int z = x + 1; z < 4; z++){
                if (cells[cells_size - 1][z][y] == 0)
                    continue;
                else if (cells[cells_size - 1][x][y] == cells[cells_size - 1][z][y]){
                    user_score[cells_size - 1] = user_score[cells_size - 1] + cells[cells_size - 1][x][y];
                    cells[cells_size - 1][x][y] = 2*cells[cells_size - 1][x][y];
                    cells[cells_size - 1][z][y] = 0;
                    is_moved = 1;
                    break;
                } else if (cells[cells_size - 1][z][y] != 0)
                    break;
            }
        }
    }

    for (int x = 0; x < 4; x++){
        for (int y = 0; y < 4; y++){
            //SORT
            if (cells[cells_size - 1][x][y] == 0){
                for (int z = x + 1; z < 4; z++){
                    if (cells[cells_size - 1][z][y] != 0){
                        cells[cells_size - 1][x][y] = cells[cells_size - 1][z][y];
                        cells[cells_size - 1][z][y] = 0;
                        is_moved = 1;
                        break;
                    }
                }
            }
        }
    }
    is_move_fail[0] = 1;
    if (is_moved)
        generateCells();
}

void actionDown(){
    int is_moved = 0;
    for (int x = 3; x > -1; x--){
        for (int y = 0; y < 4; y++){
            //SUM
            for (int z = x - 1; z > -1; z--){
                if (cells[cells_size - 1][z][y] == 0)
                    continue;
                else if (cells[cells_size - 1][z][y] == cells[cells_size - 1][x][y]){
                    user_score[cells_size - 1] = user_score[cells_size - 1] + cells[cells_size - 1][x][y];
                    cells[cells_size - 1][x][y] = 2*cells[cells_size - 1][x][y];
                    cells[cells_size - 1][z][y] = 0;
                    is_moved = 1;
                    break;
                } else if (cells[cells_size - 1][z][y] != 0)
                    break;
            }
        }
    }

    for (int x = 3; x > -1; x--){
        for (int y = 0; y < 4; y++){
            //SORT
            if (cells[cells_size - 1][x][y] == 0){
                for (int z = x - 1; z > -1; z--){
                    if (cells[cells_size - 1][z][y] != 0){
                        cells[cells_size - 1][x][y] = cells[cells_size - 1][z][y];
                        cells[cells_size - 1][z][y] = 0;
                        is_moved = 1;
                        break;
                    }
                }
            }
        }
    }
    is_move_fail[2] = 1;
    if (is_moved)
        generateCells();
}

void actionLeft(){
    int is_moved = 0;
    for (int x = 0; x < 4; x++){
        for (int y = 0; y < 4; y++){
            //SUM
            for (int z = y + 1; z < 4; z++){
                if (cells[cells_size - 1][x][z] == 0)
                    continue;
                else if (cells[cells_size - 1][x][z] == cells[cells_size - 1][x][y]){
                    user_score[cells_size - 1] = user_score[cells_size - 1] + cells[cells_size - 1][x][y];
                    cells[cells_size - 1][x][y] = 2*cells[cells_size - 1][x][y];
                    cells[cells_size - 1][x][z] = 0;
                    is_moved = 1;
                    break;
                } else if (cells[cells_size - 1][x][z] != 0)
                    break;
            }
        }
    }

    for (int x = 0; x < 4; x++){
        for (int y = 0; y < 4; y++){
            //SORT
            if (cells[cells_size - 1][x][y] == 0){
                for (int z = y + 1; z < 4; z++){
                    if (cells[cells_size - 1][x][z] != 0){
                        cells[cells_size - 1][x][y] = cells[cells_size - 1][x][z];
                        cells[cells_size - 1][x][z] = 0;
                        is_moved = 1;
                        break;
                    }
                }
            }
        }
    }
    is_move_fail[1] = 1;
    if (is_moved)
        generateCells();
}

void actionRight(){
    int is_moved = 0;
    for (int x = 0; x < 4; x++){
        for (int y = 3; y > -1; y--){
            //SUM
            for (int z = y - 1; z > -1; z--){
                if (cells[cells_size - 1][x][z] == 0)
                    continue;
                else if (cells[cells_size - 1][x][z] == cells[cells_size - 1][x][y]){
                    user_score[cells_size - 1] = user_score[cells_size - 1] + cells[cells_size - 1][x][y];
                    cells[cells_size - 1][x][y] = 2*cells[cells_size - 1][x][y];
                    cells[cells_size - 1][x][z] = 0;
                    is_moved = 1;
                    break;
                } else if (cells[cells_size - 1][x][z] != 0)
                    break;
            }
        }
    }
    for (int x = 0; x < 4; x++){
        for (int y = 3; y > -1; y--){
            //SORT
            if (cells[cells_size - 1][x][y] == 0){
                for (int z = y - 1; z > -1; z--){
                    if (cells[cells_size - 1][x][z] != 0){
                        cells[cells_size - 1][x][y] = cells[cells_size - 1][x][z];
                        cells[cells_size - 1][x][z] = 0;
                        is_moved = 1;
                        break;
                    }
                }
            }
        }
    }
    is_move_fail[3] = 1;
    if (is_moved)
        generateCells();
}



void printMenu(int i){
    gotoxy(0, 8);

    if (i == 0)
        printf(ANSI_COLOR_GREEN);
    if (user_time == 0)
        printBorder("START NEW GAME", 14, 0);
    else
        printBorder("RESUME", 6, 0);
    if (i == 0)
        printf(ANSI_COLOR_RESET);

    if (i == 1)
        printf(ANSI_COLOR_GREEN);
    printBorder("HIGH SCORES", 11, 0);
    if (i == 1)
        printf(ANSI_COLOR_RESET);


    if (i == 2)
        printf(ANSI_COLOR_GREEN);
    printBorder("HELP", 4, 0);
    if (i == 2)
        printf(ANSI_COLOR_RESET);


    if (i == 3)
        printf(ANSI_COLOR_GREEN);
    printBorder("ABOUT", 5, 0);
    if (i == 3)
        printf(ANSI_COLOR_RESET);


    if (i == 4)
        printf(ANSI_COLOR_GREEN);
    printBorder("EXIT", 4, 0);
    if (i == 4)
        printf(ANSI_COLOR_RESET);
}

void printCells(){
    //PRINT ALL ARRAY

    gotoxy(0, 7);
    char buffer[50];
    sprintf(buffer, "UNDO : %d | SCORE : %d | TIME : %02d:%02d", undo_available, user_score[cells_size - 1], (clock() - clock_game_start)/60000, (clock() - clock_game_start)/1000 % 60);
    printCenter(buffer, 8 + 11 + 7 + 10 + 5);

    int x, y;
    for (int a = 1; a < 10; a++){
        printf(ANSI_COLOR_RESET);

        gotoxy((SCREEN_WIDHT - 21)/2, 8 + a);
        for (int b = 0; b < 4; b++){
            if (a%2 == 0){
                x = a/2 - 1;
                y = b;
                switch (cells[cells_size - 1][x][y]){
                    case 2:
                        printf(ANSI_COLOR_RESET "|" ANSI_COLOR_BOLD_RED "%4d", cells[cells_size - 1][x][y]);
                        break;
                    case 4:
                        printf(ANSI_COLOR_RESET "|" ANSI_COLOR_MAGENTA "%4d", cells[cells_size - 1][x][y]);
                        break;
                    case 8:
                        printf(ANSI_COLOR_RESET "|" ANSI_COLOR_BOLD_MAGENTA "%4d", cells[cells_size - 1][x][y]);
                        break;
                    case 16:
                        printf(ANSI_COLOR_RESET "|" ANSI_COLOR_CYAN "%4d", cells[cells_size - 1][x][y]);
                        break;
                    case 32:
                        printf(ANSI_COLOR_RESET "|" ANSI_COLOR_BOLD_CYAN "%4d", cells[cells_size - 1][x][y]);
                        break;
                    case 64:
                        printf(ANSI_COLOR_RESET "|" ANSI_COLOR_YELLOW "%4d", cells[cells_size - 1][x][y]);
                        break;
                    case 128:
                        printf(ANSI_COLOR_RESET "|" ANSI_COLOR_BOLD_YELLOW "%4d", cells[cells_size - 1][x][y]);
                        break;
                    case 256:
                        printf(ANSI_COLOR_RESET "|" ANSI_COLOR_GREEN "%4d", cells[cells_size - 1][x][y]);
                        break;
                    case 512:
                        printf(ANSI_COLOR_RESET "|" ANSI_COLOR_BOLD_GREEN "%4d", cells[cells_size - 1][x][y]);
                        break;
                    case 1024:
                        printf(ANSI_COLOR_RESET "|%4d", cells[cells_size - 1][x][y]);
                        break;
                    case 2048:
                        is_winner = 1;
                        printf(ANSI_COLOR_RESET "|%4d", cells[cells_size - 1][x][y]);
                        break;

                    default:
                        printf(ANSI_COLOR_RESET "|    ");
                        break;
                }
            } else
                printf(ANSI_COLOR_RESET "+----");
        }
        if (a%2 == 0)
            printf(ANSI_COLOR_RESET "|\n");
        else
            printf(ANSI_COLOR_RESET "+\n");
    }

    if (is_winner){
        printBorder("YOU ARE A WINNER!", 17, 0);
        printCenter("Keep going on!", 14);
    }

    if (is_move_fail[0] == 1 && is_move_fail[1] == 1 && is_move_fail[2] == 1 && is_move_fail[3] == 1){
        printBorder("GAME OVER!", 10, 0);
        printCenter("PRESS ENTER TO RESTART", 22);

        while (1){
            if (kbhit()){
                char c = getch();
                if (c == KEY_ENTER)
                    break;
            }
        }

        //hitung nilai akhir
        saveGame(user_score[cells_size - 1], (clock() - clock_game_start)/1000, 0);
        loadGame();
        MenuGameActivity(0);
    }
}

void printBorder(char * str, int count, int widht){
	widht = widht == 0 ? 28 : widht;
    for (int i = 0; i < 3; i++){
        if (i == 0 || i == 2){
            for (int i = 0; i < SCREEN_WIDHT; i++){
                if ((SCREEN_WIDHT - widht)/2 + widht < i)
                    break;
                else if ((SCREEN_WIDHT - widht)/ 2 <= i + 1){
                    if ((SCREEN_WIDHT - widht)/ 2 == i + 1 || (SCREEN_WIDHT - widht)/2 + widht == i)
                        printf("+");
                    else
                        printf("-");
                } else
                    printf(" ");
            }
        } else {
            for (int i = 0; i < SCREEN_WIDHT; i++){
                if ((SCREEN_WIDHT - widht)/2 + widht < i){
                    break;
                } else if ((SCREEN_WIDHT - count)/ 2 <= i && i < (SCREEN_WIDHT - count)/2 + count)
                    printf("%c", str[i - (SCREEN_WIDHT - count)/ 2]);
                else if ((SCREEN_WIDHT - widht)/ 2 == i + 1 || (SCREEN_WIDHT - widht)/2 + widht == i)
                        printf("|");
                else
                    printf(" ");
            }
        }
        printf("\n");
    }
    printf("\n");
}

void printBorderForm(char * ttl, int count1, char * str, int count2){
    int widht = 28;
    for (int line = 0; line < 3; line++){
        if (line == 0 ||line == 2){
            for (int i = 0; i < SCREEN_WIDHT; i++){
                if ((SCREEN_WIDHT - widht)/2 + widht < i)
                    break;
                else if (line == 0 && (SCREEN_WIDHT - count1)/ 2 <= i && i < (SCREEN_WIDHT - count1)/2 + count1)
                    printf("%c", ttl[i - (SCREEN_WIDHT - count1)/ 2]);
                else if ((SCREEN_WIDHT - widht)/ 2 <= i + 1){
                    if ((SCREEN_WIDHT - widht)/ 2 == i + 1 || (SCREEN_WIDHT - widht)/2 + widht == i)
                        printf("+");
                    else
                        printf("-");
                } else
                    printf(" ");
            }
        } else {
            for (int i = 0; i < SCREEN_WIDHT; i++){
                if ((SCREEN_WIDHT - widht)/2 + widht < i){
                    break;
                } else if ((SCREEN_WIDHT - count2)/ 2 <= i && i < (SCREEN_WIDHT - count2)/2 + count2)
                    printf("%c", str[i - (SCREEN_WIDHT - count2)/ 2]);
                else if ((SCREEN_WIDHT - widht)/ 2 == i + 1 || (SCREEN_WIDHT - widht)/2 + widht == i)
                        printf("|");
                else
                    printf(" ");
            }
        }
        printf("\n");
    }
    printf("\n");
}

void printCenter(char * str, int count){
    for (int i = 0; i < SCREEN_WIDHT; i++){
        if ((SCREEN_WIDHT - count)/ 2 <= i){
            printf("%s\n", str);
            break;
        } else
            printf(" ");
    }
}


void generateCells(){
    //GENERATING OF NUMBER 2
    while (1){
        int rand_x = rand() % 4;
        int rand_y = rand() % 4;
        if (cells[cells_size - 1][rand_x][rand_y] == 0){
            cells[cells_size - 1][rand_x][rand_y] = 2;
            break;
        }
    }
    for (int i = 0; i <= cells_size - 2; i++){
        copyCells(i, i + 1);
        copyScore(i, i + 1);
    }

    undo_available++;
    if (undo_available > cells_size - 1)
        undo_available = cells_size - 1;

    is_move_fail[0] = 0;
    is_move_fail[1] = 0;
    is_move_fail[2] = 0;
    is_move_fail[3] = 0;
}

void undoCells(){
    if (undo_available <= 0)
        return;
    undo_available--;

    for (int i = cells_size - 2; i >= 1; i--){
        copyCells(i, i - 1);
        copyScore(i, i - 1);
    }
    copyCells(cells_size - 1, cells_size - 2);
    copyScore(cells_size - 1, cells_size - 2);
}


void copyCells(int a, int b){
    //A = B
    for (int x = 0; x < 4; x++){
        for (int y = 0; y < 4; y++){
            cells[a][x][y] = cells[b][x][y];
        }
    }
}

void copyScore(int a, int b){
    user_score[a] = user_score[b];
}


void gotoxy(int x, int y){
    HANDLE hConsoleOutput;
    COORD dwCursorPosition;
    dwCursorPosition.X = x;
    dwCursorPosition.Y = y;
    hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hConsoleOutput,dwCursorPosition);
}


void getMaxString(char str1 , FILE * fptr){
    int words_count = 0;        // jumlah kata pada cell
    int coloumn = 0;            // posisi kolom saat membaca file

    while (str1 != EOF){        // baca hingga End of File
        if(str1 != ','){        // masih di cell yang sama
            words_count++;      // hitung jumlah karakter pada cell tersebut
        }else{
            if(coloumn_arr[coloumn] < words_count)  // menentukan jumlah karakter maksimal pada setiap kolom
                coloumn_arr[coloumn] = words_count; //

            words_count = 0;    // kolom baru, mulai dari 0 lagi
            coloumn++;          // posisi kolom berpindah
        }
        if(str1 == '\n'){                               // kolom terakhir
            if(coloumn_arr[coloumn] < words_count)      // menentukan jumlah karakter maksimal pada setiap kolom
                coloumn_arr[coloumn] = words_count;

            if(coloumn_size < coloumn)                  // menentukan jumlah kolom maksimal pada file tersebut
                coloumn_size = coloumn;

            words_count = 0;    // kolom baru, mulai dari 0 lagi
            coloumn = 0;        // posisi kolom dari 0 lagi
        }
        str1 = fgetc(fptr);     // baca karakter berikutnya
    }
}

int getScreenSize(){
	//menghitung ulang jika terjadi perubahan ukuran layar terminal
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int columns, rows;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    SCREEN_WIDHT = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    SCREEN_HEIGHT = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
}

int getIntLength(int s){
    int a = 1;
    while (s >= 10){
        s = s/10;
        a++;
    }
    return a;
}
