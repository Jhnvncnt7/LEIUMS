#include <iostream.h>
#include <conio.h>
#include <stdio.h>
#include <string.h>
#include <fstream.h> // for file handling
#include <dos.h>
#include <stdlib.h> // for atoi()
#include <ctype.h> // for tolower()
#define equipments_file_path "C:\\USERS\\K3PISO~1\\JV\\LEIUMS\\eq-data1.txt"

struct Equipment {
    char name[50];
    int quantity;
    char acquisitionDate[15];
    char condition[20];
    float usageHours;
    float depreciation;
    float availabilityRate;
};
// ############################## DIVIDERS & SPACES ETC. ##############################
void printLine(int length, int color, char symbol){ // LINE DIVIDER
    textcolor(color);
    for(int i = 0; i < length; i++){
        cprintf("%c",symbol);
    }
    textcolor(WHITE);
}
void error(char* msg){ textcolor(RED); cprintf("%s", msg); textcolor(WHITE); } // ERROR MESSAGE
void info(char* msg){ textcolor(YELLOW); cprintf("%s", msg); textcolor(WHITE); } // INFO MESSAGE
void screenTemplate(int color, int x, int y, char* title){ //SCREEN TEMPLATE
    gotoxy(1,1); printLine(80, 11, '=');gotoxy(1,3); printLine(80, 11, '-'); gotoxy(1,24); printLine(80, 11, '=');
    textcolor(color); gotoxy(x,y); highvideo(); cprintf(title); normvideo(); gotoxy(1,4); textcolor(WHITE);
}
void returnMenu(){ // RETURN MENU
    gotoxy(43,23); textcolor(YELLOW); cprintf("Press any key to return to main MENU."); textcolor(WHITE);
    getch(); return;
}
void clearLines(int start, int end) { // CLEAR MULTIPLE LINES
    for (int y = start; y <= end; y++) {
        gotoxy(1, y);
        clreol();
    }
}
// ############################## DISPLAY MENU ##############################
void displayMenu(){
    // SCREEN DISPLAY
    screenTemplate(9, 35, 2, "MAIN MENU");
    cout << "[1] Display Equipments\n";
    cout << "[2] Search Equipments\n";
    cout << "[3] Add Equipments\n";
    cout << "[1]\n";
    cout << "[1]\n";
    cout << "[1]\n";
    cout << "[0] Exit\n";
}
// ############################## SAVE EQUIPMENTS ##############################
void saveEquipment(Equipment e) {
    ofstream file;
    file.open(equipments_file_path, ios::app);

    if (!file) {
        gotoxy(1,23); textcolor(RED); cprintf("Error opening file!\n"); textcolor(WHITE);
        return;
    }

    file << e.name << "|"
         << e.quantity << "|"
         << e.acquisitionDate << "|"
         << e.condition << "|"
         << e.usageHours << "|"
         << e.depreciation << "|"
         << e.availabilityRate << "\n";

    file.close();
    textcolor(2); gotoxy(1,23); cprintf("Equipment saved successfully!"); textcolor(15);
}
// ############################## COUNT FILE LINES (DATA COUNTER) ##############################
int countLines(const char *filename) {
    ifstream file(filename);
    if (!file) return 0;

    int count = 0;
    char line[256];
    while(file.getline(line, sizeof(line))) count++;
    file.close();
    return count;
}
// ################################################## DISPLAY EQUIPMENTS ##################################################
void displayEquipments(){
    struct date d; getdate(&d);
    struct time t; gettime(&t);
    clrscr();
    screenTemplate(9, 3, 2, "> EQUIPMENTS RECORD");
    textcolor(WHITE); gotoxy(65,2); printf("DATE: %d-%d-%d", d.da_day, d.da_mon, d.da_year);
    gotoxy(1,4); textbackground(LIGHTGRAY); textcolor(BLACK);
    cprintf(" %-24s %-5s %-11s %-12s %-7s %-7s %-7s", "NAME", "QTY", "DATE", "CONDITION", "HOURS", "DEP%", "AVAIL%");
    textbackground(BLACK);

    ifstream file(equipments_file_path);
    if(!file){ // check for internal errors
        gotoxy(1,23); textcolor(RED); cprintf("Cannot open file or not found.");
        returnMenu(); return;
    }
    if(file.peek() == EOF){ // check if empty
        gotoxy(1,5); textcolor(YELLOW); cprintf("No equipment records.");
        gotoxy(1,23); textcolor(RED); cprintf("File is empty.");
        file.close(); returnMenu(); return;
    }
    Equipment e;
    char line[256]; int y = 5; int lineCount = 0; int totalLines = countLines(equipments_file_path);
    //Read line by line
    while(file.getline(line, sizeof(line))){
        // Name
        char *token = strtok(line, "|"); if(token) strcpy(e.name, token);
        // Quantity
        token = strtok(NULL, "|"); if(token) e.quantity = atoi(token);
        // Date acquired
        token = strtok(NULL, "|"); if(token) strcpy(e.acquisitionDate, token);
        // Condition
        token = strtok(NULL, "|"); if(token) strcpy(e.condition, token);
        // Usage hours
        token = strtok(NULL, "|"); if(token) e.usageHours = atof(token);
        // Depriciation
        token = strtok(NULL, "|"); if(token) e.depreciation = atof(token);
        // Availability Rate
        token = strtok(NULL, "|"); if(token) e.availabilityRate = atof(token);
        // Print line
        lineCount++; gotoxy(1,y++); textcolor(WHITE); lowvideo();
        cprintf(" %-24s %-5d %-11s %-12s %-7.2f %-7.2f %-7.2f", e.name, e.quantity, e.acquisitionDate, e.condition, e.usageHours, e.depreciation, e.availabilityRate);
        if (y >= 20){ // stop at 15th data
            gotoxy(1,20); printLine(80, 11, '-');
            gotoxy(1, 21); textcolor(YELLOW); cprintf("%d out of %d data. (press any key to continue)", lineCount, totalLines); getch(); clrscr();
            screenTemplate(9, 3, 2, "> EQUIPMENTS RECORD");
            gotoxy(1,4); textbackground(LIGHTGRAY); textcolor(BLACK);
            cprintf(" %-24s %-5s %-11s %-12s %-7s %-7s %-7s", "NAME", "QTY", "DATE", "CONDITION", "HOURS", "DEP%", "AVAIL%");
            textbackground(BLACK); textcolor(WHITE); normvideo();
            y = 5;
        }
    }
    normvideo();
    gotoxy(1,20); printLine(80, 11, '-');
    gotoxy(1,21); textcolor(YELLOW); cprintf("%d out of %d data.", lineCount, totalLines);
    file.close();
    returnMenu(); return;
}
// ################################################## SEARCH EQUIPMENTS ##################################################
void searchEquipments(){
    clrscr();
    screenTemplate(9, 3, 2, "> SEARCH EQUIPMENTS");
    textcolor(WHITE);
    cout << "Search Equipments.\n";
    highvideo(); cprintf("THIS IS BOLD\n");
    lowvideo(); cprintf("THIS IS NOT BOLD");

    returnMenu(); return;
}
// ################################################## ADD EQUIPMENTS ##################################################
void addEquipments(){
    clrscr();
    char ans1[1000], ans2[1000];
    int i;
    Equipment eq;
get_data:
    screenTemplate(9, 3, 2, "> ADD EQUIPMENTS"); gotoxy(35, 2); info("Enter nescessary data or [cancel] to exit.");
get_name:
    // Input name
    gotoxy(1,4); clreol(); cout << "Equipment name [20 characters max]: "; gets(ans1); for(i = 0; ans1[i]; i++){ ans1[i] = tolower(ans1[i]); }
    if(strlen(ans1) == 0){ // check if empty
        gotoxy(36,4); textcolor(RED); cprintf("Cannot be empty. "); textcolor(YELLOW); cprintf("Press any key to continue."); textcolor(WHITE); getch(); goto get_name;
    } else if(strcmp(ans1, "cancel") == 0){ // check for cancel
        gotoxy(1,23); cout << "Operation cancelled.";
        returnMenu(); return;
    } strcpy(eq.name, ans1);
get_qty:
    // Input quantity
    gotoxy(1,5); clreol(); cout << "Quantity [ex. 1]: "; gets(ans1); for(i = 0; ans1[i]; i++){ ans1[i] = tolower(ans1[i]); }
    if(strlen(ans1) == 0){ // check if empty
        gotoxy(18,5); textcolor(RED); cprintf("Cannot be empty. "); textcolor(YELLOW); cprintf("Press any key to continue."); textcolor(WHITE); getch(); goto get_qty;
    } else if(strcmp(ans1, "cancel") == 0){ // check for cancel
        gotoxy(1,23); cout << "Operation cancelled.";
        returnMenu(); return;
    } eq.quantity = atoi(ans1);
get_date:
    // Input acquisition date
    gotoxy(1,6); clreol(); cout << "Date acquired (DD/MM/YYYY): "; gets(ans1); for(i = 0; ans1[i]; i++){ ans1[i] = tolower(ans1[i]); }
    if(strlen(ans1) == 0){ // check if empty
        gotoxy(29,6); textcolor(RED); cprintf("Cannot be empty. "); textcolor(YELLOW); cprintf("Press any key to continue."); textcolor(WHITE); getch(); goto get_date;
    } else if(strcmp(ans1, "cancel") == 0){ // check for cancel
        gotoxy(1,23); cout << "Operation cancelled.";
        returnMenu(); return;
    } strcpy(eq.acquisitionDate, ans1);


    gotoxy(1,7); cout << "Condition: "; gets(eq.condition);
    gotoxy(1,8); cout << "Hours used: "; cin >> eq.usageHours; cin.ignore();
    gotoxy(1,9); cout << "Depreciation (%): "; cin >> eq.depreciation; cin.ignore();
    gotoxy(1,10); cout << "Availability Rate (%): "; cin >> eq.availabilityRate; cin.ignore();
    gotoxy(1,11); printLine(80,11,'-');
    gotoxy(1,12); cout << "[1] Confirm and Save";
    gotoxy(1,13); cout << "[2] Retry";
    gotoxy(1,14); cout << "[3] Exit (not saved)";
get_ans2:
    gotoxy(1,22); clreol(); cout << "Enter your choice [1-3]: "; gets(ans2);
    if(strcmp(ans2, "1") == 0){
        saveEquipment(eq);
    } else if(strcmp(ans2, "2") == 0){
        clearLines(4,23);
        goto get_data;
    } else if(strcmp(ans2, "3") == 0){
        return;
    } else {
        gotoxy(1,23); error("Invalid Input."); goto get_ans2;
    }

    returnMenu(); return;
}
// // ################################################## DISPLAY EQUIPMENTS ################################################## TEMPLATE/DONT DELETE
// void displayEquipments(){
//     clrscr();
//     textcolor(WHITE);
//     cout << "Equipments Display.";
// }

int main(){
    char ans[1000];
    int option;
    do{
        clrscr();
        displayMenu();
    get_ans:
        gotoxy(1,22); clreol(); cout <<"Enter your choice [0-7]: "; gets(ans);
        if(strcmp(ans, "1") == 0) option = 1;
        else if(strcmp(ans, "2") == 0) option = 2;
        else if(strcmp(ans, "3") == 0) option = 3;
        else if(strcmp(ans, "4") == 0) option = 4;
        else if(strcmp(ans, "5") == 0) option = 5;
        else if(strcmp(ans, "6") == 0) option = 6;
        else if(strcmp(ans, "7") == 0) option = 7;
        else if(strcmp(ans, "0") == 0) option = 0;
        else {
            gotoxy(1,23);
            error("Invalid input.");
            goto get_ans;
        }
        switch (option){
        case 0:
            return 0;
        case 1:
            displayEquipments();
            break;
        case 2:
            searchEquipments();
            break;
        case 3:
            addEquipments();
            break;
        default:
            break;
        }
    } while (option != 0);

    getch();
    return 0;
}
