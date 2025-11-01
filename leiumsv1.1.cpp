#include <iostream.h>
#include <conio.h>
#include <stdio.h>
#include <string.h>
#include <fstream.h>
#include <dos.h>

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
    textcolor(color); gotoxy(x,y); cprintf(title); gotoxy(1,4); textcolor(WHITE);
}
void returnMenu(){
    gotoxy(1,23); textcolor(YELLOW); cprintf("Press any key to return to main MENU."); textcolor(WHITE);
    getch(); return;
}
void clearLines(int start, int end) {
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
    file.open("equipments.txt", ios::app);

    if (!file) {
        gotoxy(1,23); cout << "Error opening file!\n";
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
    gotoxy(1,23); cout << "\nEquipment saved successfully!\n";
}
// ################################################## DISPLAY EQUIPMENTS ##################################################
void displayEquipments(){
    struct date d; getdate(&d);
    struct time t; gettime(&t);
    clrscr();
    screenTemplate(9, 3, 2, "EQUIPMENTS RECORD");
    textcolor(WHITE); gotoxy(70,2); printf("%d/%d/%d", d.da_day, d.da_mon, d.da_year); gotoxy(1,4);
    cout << "Equipments Display.";

    returnMenu(); return;
}
// ################################################## SEARCH EQUIPMENTS ##################################################
void searchEquipments(){
    clrscr();
    screenTemplate(9, 3, 2, "SEARCH EQUIPMENTS");
    textcolor(WHITE);
    cout << "Search Equipments.";

    returnMenu(); return;
}
// ################################################## ADD EQUIPMENTS ##################################################
void addEquipments(){
    clrscr();
    char ans1[1000], ans2[1000];
    Equipment eq;
get_data:
    screenTemplate(9, 3, 2, "ADD EQUIPMENTS"); gotoxy(35, 2); info("Enter nescessary data or [cancel] to exit.");
    gotoxy(1,4); cout << "Equipment name: "; gets(eq.name);
    gotoxy(1,5); cout << "Quantity: "; cin >> eq.quantity;
    gotoxy(1,6); cout << "Date acquired (MM/DD/YYYY): "; gets(eq.acquisitionDate);
    gotoxy(1,7); cout << "Condition: "; gets(eq.condition);
    gotoxy(1,8); cout << "Hours used: "; cin >> eq.usageHours;
    gotoxy(1,9); cout << "Depreciation (%): "; cin >> eq.depreciation;
    gotoxy(1,10); cout << "Availability Rate (%): "; cin >> eq.availabilityRate;
    gotoxy(1,11); printLine(80,11,'-');
    gotoxy(1,12); cout << "[1] Confirm and Save";
    gotoxy(1,13); cout << "[2] Retry";
    gotoxy(1,14); cout << "[3] Exit (not saved)";
get_ans2:
    gotoxy(1,22); clreol(); cout << "Enter your choice [1-3]: "; gets(ans2);
    if(strcmp(ans2, "1") == 0){
        saveEquipment(eq);
    } else if(strcmp(ans2, "2") == 0){
        goto get_data;
    } else if(strcmp(ans2, "3") == 0){

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
