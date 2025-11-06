#include <iostream.h>
#include <conio.h>
#include <stdio.h>
#include <string.h>
#include <fstream.h>
#include <dos.h>
#include <stdlib.h>
#include <ctype.h>
#define equipments_file_path "EQ-DATA1.txt"
#define max_equipments 350
#define year_min 1900
#define year_max 2100

struct Equipment {
    char name[50];
    char acquisitionDate[15];
    char condition[20]; // [very bad, bad, good, very good]
    float usageHours;
    float efficiency;
    float availabilityRate;
    char status[20]; // [available, in use, maintenance, not working]
    char category[20]; // [functional, for repair, for replacement]
} equipments[max_equipments];
int eqCounter = 0;

// ===== Function Prototypes for New Features =====
void editEquipment();
void showSummary();
void ensureFileExists();
void refreshDataAfterDelete();
int confirmExit();

// ############################## HELPERS DIVIDERS & SPACES ETC. ##############################
void printLine(int length, int color, char symbol){ // LINE DIVIDER
    textcolor(color);
    for(int i = 0; i < length; i++){
        cprintf("%c",symbol);
    }
    textcolor(WHITE);
}
void error(char* msg){ textcolor(RED); cprintf("%s", msg); textcolor(WHITE); } // ERROR MESSAGE
void info(char* msg){ textcolor(YELLOW); cprintf("%s", msg); textcolor(WHITE); } // INFO MESSAGE
void success(char* msg){ textcolor(LIGHTGREEN); cprintf("%s", msg); textcolor(WHITE); } // SUCCESS MESSAGE
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
int valid (char* type, char* value) { // Check input type (int n float)
    int i, dotCount = 0, start = 0;
    if (value[0] == '-') start = 1;
    if (strcmp(type, "int") == 0) { // check for int
        for (i = start; i < strlen(value); i++) {
            if (!isdigit((unsigned char)value[i])) return 0;
        }
        return 1;
    }
    else if (strcmp(type, "float") == 0) { // check for float
        for (i = start; i < strlen(value); i++) {
            if (value[i] == '.') {
                dotCount++;
                if (dotCount > 1) return 0; // more than one dot is invalid
            } 
            else if (!isdigit((unsigned char)value[i])) return 0; // anything not digit or '.' is invalid
        }
        if (strlen(value) == start + 1 && value[start] == '.') return 0; // Make sure not just '.' or '-.'
        return 1;
    }
    return 0; // if no matched type
}
int isCancel (char* input) { // CANCEL CHECK
    char lower[1000];
    for (int i = 0; input[i]; i++) lower[i] = tolower(input[i]); lower[strlen(input)] = '\0';
    return strcmp(lower, "icancel") == 0;
}

// ############################## DISPLAY MENU ##############################
void displayMenu(){
    screenTemplate(9, 35, 2, "MAIN MENU");
    cout << "[1] Display All Equipments\n";
    cout << "[2] Search / View / Delete Equipments\n";
    cout << "[3] Add Equipments\n";
    cout << "[4] Edit Equipment\n";      
    cout << "[5] View Summary / Statistics\n";
    cout << "[0] Exit Program\n";
}
// ############################## SAVE EQUIPMENTS ##############################
void saveEquipment(Equipment e) {
    ofstream file(equipments_file_path, ios::app); // append mode

    if (!file) {
        gotoxy(1,23); textcolor(RED); cprintf("Error opening file!\n"); textcolor(WHITE);
        return;
    }

    // Save current fields
    file << e.name << "|"
         << e.acquisitionDate << "|"
         << e.condition << "|"
         << e.usageHours << "|"
         << e.efficiency << "|"
         << e.availabilityRate << "|"
         << e.status << "|"
         << e.category << "\n";

    file.close();
    textcolor(2); gotoxy(1,23); clreol(); cprintf("Equipment saved successfully!"); textcolor(15);
}

void saveAllEquipment() {
    ofstream file(equipments_file_path); // overwrite file

    if (!file) {
        gotoxy(1,23); textcolor(RED); cprintf("Error opening file!\n"); textcolor(WHITE);
        return;
    }

    for (int i = 0; i < eqCounter; i++) {
        Equipment e = equipments[i];
        file << e.name << "|"
             << e.acquisitionDate << "|"
             << e.condition << "|"
             << e.usageHours << "|"
             << e.efficiency << "|"
             << e.availabilityRate << "|"
             << e.status << "|"
             << e.category << "\n";
    }

    file.close();
}
// ############################## LOAD EQUIPMENTS ##############################
void loadEquipment() {
    ifstream file(equipments_file_path);
    if (!file) return;

    char line[512]; // larger buffer for longer lines
    eqCounter = 0;

    while (!file.eof()) {
        file.getline(line, sizeof(line), '\n'); // read one line
        if (strlen(line) == 0) continue;        // skip empty lines
        if (eqCounter >= max_equipments) break; // avoid overflow

        char *token = strtok(line, "|"); 
        if (!token) continue;
        strcpy(equipments[eqCounter].name, token); // name

        token = strtok(NULL, "|"); 
        if (!token) continue;
        strcpy(equipments[eqCounter].acquisitionDate, token); // acquisitionDate

        token = strtok(NULL, "|"); 
        if (!token) continue;
        strcpy(equipments[eqCounter].condition, token); // condition

        token = strtok(NULL, "|"); 
        if (!token) continue;
        equipments[eqCounter].usageHours = atof(token); // usageHours

        token = strtok(NULL, "|"); 
        if (!token) continue;
        equipments[eqCounter].efficiency = atof(token); // efficiency

        token = strtok(NULL, "|"); 
        if (!token) continue;
        equipments[eqCounter].availabilityRate = atof(token); // availabilityRate

        token = strtok(NULL, "|"); 
        if (!token) continue;
        strcpy(equipments[eqCounter].status, token); // status

        token = strtok(NULL, "|"); 
        if (!token) continue;
        strcpy(equipments[eqCounter].category, token); // category

        eqCounter++; // increment counter
    }
    file.close();
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
// ############################## VIEW EQUIPMENT DETAILS ##############################
void viewEquipmentDetails(Equipment eq) {
    clrscr();
    int boxLeft = 15, boxTop = 3, boxWidth = 55, boxHeight = 14, i;

    // Draw top border
    gotoxy(boxLeft, boxTop); printf("%c", 201);
    for (i = 0; i < boxWidth - 2; i++) printf("%c", 205);
    printf("%c", 187);

    // Draw sides
    for (i = 1; i < boxHeight - 1; i++) {
        gotoxy(boxLeft, boxTop + i); printf("%c", 186);
        gotoxy(boxLeft + boxWidth - 1, boxTop + i); printf("%c", 186);
    }

    // Draw bottom border
    gotoxy(boxLeft, boxTop + boxHeight - 1); printf("%c", 200);
    for (i = 0; i < boxWidth - 2; i++) printf("%c", 205);
    printf("%c", 188);

    // Title
    gotoxy(boxLeft + (boxWidth / 2) - 9, boxTop + 1);
    textcolor(YELLOW);
    cprintf("EQUIPMENT DETAILS");
    textcolor(WHITE);

    // Display fields
    int y = boxTop + 3, x = boxLeft + 3;
    gotoxy(x, y++); printf("Name            : %s", eq.name);
    gotoxy(x, y++); printf("Date Acquired   : %s", eq.acquisitionDate);
    gotoxy(x, y++); printf("Condition       : %s", eq.condition);
    gotoxy(x, y++); printf("Usage Hours     : %.2f", eq.usageHours);
    gotoxy(x, y++); printf("Efficiency      : %.2f %%", eq.efficiency);
    gotoxy(x, y++); printf("Availability    : %.2f %%", eq.availabilityRate);
    gotoxy(x, y++); printf("Status          : %s", eq.status);
    gotoxy(x, y++); printf("Category        : %s", eq.category);

    // Bottom info
    textcolor(LIGHTGRAY);
    gotoxy(boxLeft + 14, boxTop + boxHeight);
    cprintf("Press ESC to go back");
    textcolor(WHITE);

    // Wait for ESC
    while (1) {
        int ch = getch();
        if (ch == 27) break; // ESC → return
    }
}
// ############################## VALID DATE FORMAT ##############################
int validDateFormat(char* date) {
    // Must be in format DD/MM/YYYY (10 chars)
    if (strlen(date) != 10) return 0;

    // Check '/' positions
    if (date[2] != '/' || date[5] != '/') return 0;

    // Check digits' positions
    for (int i = 0; i < 10; i++) {
        if (i == 2 || i == 5) continue; // skip slash
        if (!isdigit(date[i])) return 0;
    }

    // Extract numbers
    int day = (date[0] - '0') * 10 + (date[1] - '0');
    int month = (date[3] - '0') * 10 + (date[4] - '0');
    int year = (date[6] - '0') * 1000 + (date[7] - '0') * 100 + (date[8] - '0') * 10 + (date[9] - '0');

    // Basic range check
    if (month < 1 || month > 12) return 0;
    if (day < 1 || day > 31) return 0;
    if (year < year_min || year > year_max) return 0;

    // Handle days per month
    int daysInMonth;
    switch (month) {
        case 2: // February
            daysInMonth = (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)) ? 29 : 28;
            break;
        case 4: case 6: case 9: case 11:
            daysInMonth = 30;
            break;
        default:
            daysInMonth = 31;
    }
    if (day > daysInMonth) return 0;

    return 1; // Valid date
}
// ============================== DISPLAY EQUIPMENTS =============================
void displayEquipments() {
    struct date d; getdate(&d);
    struct time t; gettime(&t);
    clrscr();
    screenTemplate(9, 3, 2, "> EQUIPMENTS RECORD");

    textcolor(WHITE);
    gotoxy(65, 2);
    printf("DATE: %02d-%02d-%04d", d.da_day, d.da_mon, d.da_year);

    // Load file data
    loadEquipment();

    if (eqCounter <= 0) {
        gotoxy(32, 10); info("(No records found)");
        gotoxy(1, 23); error("File is empty or not found.");
        returnMenu(); return;
    }

    int perPage = 15;
    int page = 0;
    int totalPages = (eqCounter + perPage - 1) / perPage;

    while (1) {
        clrscr();
        _setcursortype(_NOCURSOR);
        screenTemplate(9, 3, 2, "> EQUIPMENTS RECORD");
        textcolor(WHITE);
        gotoxy(65, 2);
        printf("DATE: %02d-%02d-%04d", d.da_day, d.da_mon, d.da_year);

        // === HEADER ===
        gotoxy(1, 4);
        textbackground(LIGHTGRAY); textcolor(BLACK);
        cprintf(" %-25s %-15s %-15s %10s %9s ", "NAME", "STATUS", "CONDITION", "EFFICIENCY%", "AVAIL%");
        textbackground(BLACK); textcolor(WHITE); normvideo();

        // === BODY ===
        int start = page * perPage;
        int end = start + perPage;
        if (end > eqCounter) end = eqCounter;
        int y = 5;

        for (int i = start; i < end; i++) {
            gotoxy(1, y++); lowvideo();
            cprintf(" %-25s %-15s %-15s %10.2f %9.2f ",
                    equipments[i].name,
                    equipments[i].status,
                    equipments[i].condition,
                    equipments[i].efficiency,
                    equipments[i].availabilityRate);
            normvideo();
        }

        // === FOOTER ===
        gotoxy(1, 20); printLine(80, 11, '-');
        gotoxy(1, 21); textcolor(YELLOW);
        cprintf("%02d-%02d out of %d data.  Page %d/%d",
                start + 1, end, eqCounter, page + 1, totalPages);
        textcolor(CYAN);
        cprintf("  [%c/%c arrows for pages] [ESC exit]", (char)17, (char)16);
        textcolor(WHITE);

        // === Controls ===
        int ch = getch();
        if (ch == 27) break; // ESC exit
        if (ch == 0 || ch == 224) {
            ch = getch();
            if (ch == 77 && page < totalPages - 1) page++;
            else if (ch == 75 && page > 0) page--;
        }
    }
    _setcursortype(_NORMALCURSOR);
}
// ====================== SEARCH EQUIPMENT BY NAME ==========================
void searchEquipmentByName() {
    struct date d; getdate(&d);
    struct time t; gettime(&t);
    clrscr();
    screenTemplate(9, 3, 2, "> SEARCH EQUIPMENT BY NAME");
    gotoxy(30, 2); info("Enter equipment name or \"icancel\" to exit.");

    // Load file data
    loadEquipment();

    if (eqCounter <= 0) {
        gotoxy(32, 10); info("(No records found)");
        gotoxy(1, 23); error("File is empty or not found.");
        returnMenu(); return;
    }

    char ans1[50], lowerAns1[50];
    int i, j;

get_ans1:
    gotoxy(1,4); clreol(); cout << "Enter name to search: "; gets(ans1);
    for (i = 0; ans1[i]; i++) lowerAns1[i] = tolower(ans1[i]);
    lowerAns1[strlen(ans1)] = '\0';

    if (strlen(ans1) == 0 || ans1[0] == ' ' || ans1[0] == '\t') {
        gotoxy(36,4); error(" Cannot be empty."); info(" Press any key to continue."); getch(); goto get_ans1;
    }
    else if (isCancel(ans1)) {
        gotoxy(1,23); cout << "Operation cancelled.";
        returnMenu(); return;
    }

    Equipment matches[200];
    int matchCount = 0;
    for (i = 0; i < eqCounter; i++) {
        char lowerName[50];
        for (j = 0; equipments[i].name[j]; j++) lowerName[j] = tolower(equipments[i].name[j]);
        lowerName[strlen(equipments[i].name)] = '\0';
        if (strstr(lowerName, lowerAns1)) matches[matchCount++] = equipments[i];
    }

    if (matchCount == 0) {
        gotoxy(1,23); error("No matches found.");
        returnMenu(); return;
    }

    int perPage = 15, page = 0, totalPages = (matchCount + perPage - 1) / perPage;
    int selected = 0;

    while (1) {
        clrscr();
        _setcursortype(_NOCURSOR);
        screenTemplate(9,3,2, "> SEARCH RESULTS");
        textcolor(WHITE); gotoxy(65,2);
        printf("DATE: %02d-%02d-%04d", d.da_day, d.da_mon, d.da_year);
        gotoxy(1,4); textbackground(LIGHTGRAY); textcolor(BLACK);
        cprintf(" %-25s %-15s %-15s %10s %9s ", "NAME", "STATUS", "CATEGORY", "EFFICIENCY%", "AVAIL%");
        textbackground(BLACK); textcolor(WHITE); normvideo();

        int start = page * perPage;
        int end = start + perPage;
        if (end > matchCount) end = matchCount;
        int y = 5;

        for (i = start; i < end; i++) {
            if (i == start + selected) {
                textbackground(BLUE); textcolor(WHITE);
            } else {
                textbackground(BLACK); textcolor(WHITE);
            }
            gotoxy(1, y++);
            cprintf(" %-25s %-15s %-15s %10.2f %9.2f ",
                    matches[i].name,
                    matches[i].status,
                    matches[i].category,
                    matches[i].efficiency,
                    matches[i].availabilityRate);
        }

        textbackground(BLACK); textcolor(WHITE);
        gotoxy(1,20); printLine(80,11,'-');
        gotoxy(1,21); textcolor(YELLOW);
        cprintf("%02d-%02d out of %d results. Page %d/%d",
                start + 1, end, matchCount, page + 1, totalPages);
        gotoxy(1,22); textcolor(WHITE);
        cprintf("[%c %c select] [%c %c page] [DEL delete] [ENTER view details] [ESC exit]",
                (char)30, (char)31, (char)17, (char)16);
        textcolor(WHITE);

        // SCREEN CONTROLS
        int ch = getch();

        if (ch == 27) { // ESC
            _setcursortype(_NORMALCURSOR);
            break;
        }
        
        if (ch == 0 || ch == 224) {   // extended key
            ch = getch();             // detect actual key
            if (ch == 80 && selected < (end - start - 1)) selected++;               // DOWN
            else if (ch == 72 && selected > 0) selected--;                          // UP
            else if (ch == 77 && page < totalPages - 1) { page++; selected = 0; }   // RIGHT
            else if (ch == 75 && page > 0) { page--; selected = 0; }                // LEFT
            else if (ch == 83) {                                                    // DEL
                int delIndex = start + selected;
                gotoxy(1,23); clreol();
                printf("Delete '%s'? (Y/N): ", matches[delIndex].name);
                char confirm = getch();
                if (tolower(confirm) != 'y') continue;

                for (i = 0; i < eqCounter; i++) {
                    
                    if (strcmpi(equipments[i].name, matches[delIndex].name) == 0) {
                        for (j = i; j < eqCounter - 1; j++)
                            equipments[j] = equipments[j + 1];
                        eqCounter--;
                        break;
                    }
                }
                saveAllEquipment();
                loadEquipment();
                gotoxy(1,23); success("Record deleted successfully."); delay(600);


                matchCount = 0;
                for (i = 0; i < eqCounter; i++) {
                    char lowerName[50];
                    for (j = 0; equipments[i].name[j]; j++) lowerName[j] = tolower(equipments[i].name[j]);
                    lowerName[strlen(equipments[i].name)] = '\0';
                    if (strstr(lowerName, lowerAns1)) matches[matchCount++] = equipments[i];
                }
                if (matchCount == 0) {
                    gotoxy(1,23); error("No remaining matches found. Exiting search.");
                    getch(); break;
                }
                totalPages = (matchCount + perPage - 1) / perPage;
                if (page >= totalPages) page = totalPages - 1;
                if (selected >= (end - start)) selected = 0;
            }
        }
        else if (ch == 13) { // ENTER → view details
            viewEquipmentDetails(matches[start + selected]);
        }
    }
}
// ======================= ADD EQUIPMENTS =======================
void addEquipments() {
    clrscr();
    char ans1[1000], lowerA[1000], ans2[1000];
    int i;
    Equipment eq;

get_data:
    screenTemplate(9, 3, 2, "> ADD EQUIPMENTS");
    gotoxy(35, 2); info("Enter necessary data or \"icancel\" to exit.");

get_name: // ========== NAME ==========
    gotoxy(1,5); clreol(); info("[!] 20 characters max"); // user guide
    gotoxy(1,4); clreol(); cout << "Equipment name  : "; gets(ans1); // store input
    // empty check
    if (strlen(ans1) == 0 || ans1[0] == ' ' || ans1[0] == '\t') {
        gotoxy(19,4); error(" Cannot be empty."); info(" Press any key to continue."); getch(); goto get_name;
    }
    // cancel check
    else if (isCancel(ans1)) { gotoxy(1,23); cout << "Operation cancelled."; returnMenu(); return; }
    // length check
    else if (strlen(ans1) > 20) { gotoxy(19,4); error(" Too long."); info(" Press any key to continue."); getch(); goto get_name; }
    // store data
    else strcpy(eq.name, ans1);

get_date: // ========== DATE ACQUIRED ==========
    gotoxy(1,6); clreol(); info("[!] Format (DD/MM/YYYY)"); // user guide
    gotoxy(1,5); clreol(); cout << "Date acquired   : "; gets(ans1); // store input
    // empty check
    if (strlen(ans1) == 0 || ans1[0] == ' ' || ans1[0] == '\t') {
        gotoxy(19,5); error(" Cannot be empty."); info(" Press any key to continue."); getch(); goto get_date;
    }
    // cancel check
    else if (isCancel(ans1)) { gotoxy(1,23); cout << "Operation cancelled."; returnMenu(); return; }
    // format check
    else if (!validDateFormat(ans1)) { gotoxy(19,5); error(" Invalid format."); info(" Press any key to continue."); getch(); goto get_date; }
    // store data
    else strcpy(eq.acquisitionDate, ans1);

get_condition: // ========== CONDITION ==========
    gotoxy(1,7); clreol(); info("[!] Enter (very bad, bad, good, very good)"); // user guide
    gotoxy(1,6); clreol(); cout << "Condition       : "; gets(ans1); // store input
    for (i = 0; ans1[i]; i++) lowerA[i] = tolower(ans1[i]); lowerA[i] = '\0'; // loweer input for format validation
    int validCondition =
        strcmp(lowerA, "very bad") == 0 ||
        strcmp(lowerA, "bad") == 0 ||
        strcmp(lowerA, "good") == 0 ||
        strcmp(lowerA, "very good") == 0;
    // empty check
    if (strlen(ans1) == 0 || ans1[0] == ' ' || ans1[0] == '\t') {
        gotoxy(19,6); error(" Cannot be empty."); info(" Press any key to continue."); getch(); goto get_condition;
    }
    // cancel check
    else if (isCancel(ans1)) { gotoxy(1,23); cout << "Operation cancelled."; returnMenu(); return; }
    // format check
    else if (!validCondition) { gotoxy(19,6); error(" Invalid input."); info(" Press any key to continue."); getch(); goto get_condition; }
    // store data
    strcpy(eq.condition, lowerA);

get_hoursUsed: // ========== HOURS USED ==========
    gotoxy(1,8); clreol(); info("[!] Range (0 - 9999)"); // user guide
    gotoxy(1,7); clreol(); cout << "Hours used      : "; gets(ans1); // store input
    // empty check
    if (strlen(ans1) == 0 || ans1[0] == ' ' || ans1[0] == '\t') {
        gotoxy(19,7); error(" Cannot be empty."); info(" Press any key to continue."); getch(); goto get_hoursUsed;
    }
    // cancel check
    else if (isCancel(ans1)) { gotoxy(1,23); cout << "Operation cancelled."; returnMenu(); return; }
    // input validation
    else if (valid("int", ans1) == 0 && valid("float", ans1) == 0) {
        gotoxy(19,7); error(" Invalid input."); info(" Press any key to continue."); getch(); goto get_hoursUsed;
    }
    // store data
    else eq.usageHours = atof(ans1);
    // range check
    if (eq.usageHours < 0 || eq.usageHours > 9999) {
        gotoxy(19,7); error(" Out of range."); info(" Press any key to continue."); getch(); goto get_hoursUsed;
    }

get_status: // ===== STATUS =====
    gotoxy(1,9); clreol(); info("[!] Enter (available, in use, maintenance, not working)");
    gotoxy(1,8); clreol(); cout << "Status          : "; gets(ans1); // store input
    for (i = 0; ans1[i]; i++) lowerA[i] = tolower(ans1[i]); lowerA[i] = '\0';
    int validStatus =
        strcmp(lowerA, "available") == 0 ||
        strcmp(lowerA, "in use") == 0 ||
        strcmp(lowerA, "maintenance") == 0 ||
        strcmp(lowerA, "not working") == 0;
    // empty check
    if (strlen(ans1) == 0 || ans1[0] == ' ' || ans1[0] == '\t') {
        gotoxy(19,8); error(" Cannot be empty."); info(" Press any key to continue."); getch(); goto get_status;
    }
    // cancel check
    else if (isCancel(ans1)) { gotoxy(1,23); cout << "Operation cancelled."; returnMenu(); return; }
    // format check
    else if (!validStatus) { gotoxy(19,8); error(" Invalid input."); info(" Press any key to continue."); getch(); goto get_status; }
    // store data
    else strcpy(eq.status, lowerA);

    // ===== COMPUTE EFFICIENCY =====
    eq.efficiency = 100 - (eq.usageHours * 0.05);
    if (eq.efficiency < 0) eq.efficiency = 0;
    if (eq.efficiency > 100) eq.efficiency = 100;

    // ===== COMPUTE AVAILABILITY =====
    if (strcmp(eq.status, "available") == 0) eq.availabilityRate = 100;
    else if (strcmp(eq.status, "in use") == 0) eq.availabilityRate = 80;
    else if (strcmp(eq.status, "maintenance") == 0) eq.availabilityRate = 50;
    else eq.availabilityRate = 0;

    // ===== AUTO CATEGORIZATION =====
    if (eq.efficiency >= 70 && eq.availabilityRate >= 70)
        strcpy(eq.category, "functional");
    else if (eq.efficiency >= 40 && eq.availabilityRate >= 40)
        strcpy(eq.category, "for repair");
    else
        strcpy(eq.category, "for replacement");

    // ===== DISPLAY COMPUTED RESULTS =====
    gotoxy(1,9); clreol(); printLine(80,11,'-');
    gotoxy(1,10); clreol(); printf("Computed Efficiency    : %.2f%", eq.efficiency);
    gotoxy(1,11); clreol(); printf("Computed Availability  : %.2f%", eq.availabilityRate);
    gotoxy(1,12); clreol(); cout << "Category               : " << eq.category;

    // ===== CONFIRMATION MENU =====
    gotoxy(1,15); printLine(80,11,'-');
    gotoxy(1,16); cout << "[1] Confirm and Save";
    gotoxy(1,17); cout << "[2] Retry";
    gotoxy(1,18); cout << "[3] Exit (not saved)";

get_ans2:
    gotoxy(1,22); clreol(); cout << "Enter your choice [1-3]: "; gets(ans2);
    if (strcmp(ans2, "1") == 0) {
        saveEquipment(eq);
    } 
    else if (strcmp(ans2, "2") == 0) {
        clearLines(4,23);
        goto get_data;
    } 
    else if (strcmp(ans2, "3") == 0) {
        return;
    } 
    else {
        gotoxy(1,23); error("Invalid Input."); goto get_ans2;
    }
    returnMenu(); return;
}  
// ===================== EDIT EQUIPMENT ====================
void editEquipment() {
    clrscr();
    screenTemplate(9, 3, 2, "> EDIT EQUIPMENT");
    gotoxy(30, 2); info("Enter equipment name to edit or [icancel] to exit.");

    char name[50], lowerName[50], input[50], lowerInput[50];
    int i, found = -1;

get_name:
    gotoxy(1,4); clreol(); cout << "Enter name: "; gets(name);
    if (isCancel(name)) { gotoxy(1,23); info("Operation cancelled."); returnMenu(); return; }
    if (strlen(name) == 0 || name[0] == ' ') { error("Cannot be empty."); getch(); goto get_name; }

    loadEquipment();
    for (i = 0; i < eqCounter; i++) {
        char temp[50]; for (int j = 0; equipments[i].name[j]; j++) temp[j] = tolower(equipments[i].name[j]);
        temp[strlen(equipments[i].name)] = '\0';
        if (strcmpi(equipments[i].name, name) == 0 || strstr(temp, name)) { found = i; break; }
    }

    if (found == -1) {
        gotoxy(1,23); error("No matching equipment found."); returnMenu(); return;
    }

    Equipment &eq = equipments[found];
    clrscr();
    screenTemplate(9, 3, 2, "> EDIT EQUIPMENT DETAILS");
    gotoxy(1,5); cout << "Editing: " << eq.name;

get_condition:    // ==== CONDITION ====
    gotoxy(1,8); clreol(); info("[!] Enter (very bad, bad, good, very good)");
    gotoxy(1,7); clreol(); cout << "Condition       : "; gets(input);
    for (i = 0; input[i]; i++) lowerInput[i] = tolower(input[i]); lowerInput[i] = '\0';
    int validCondition =
        strcmp(lowerInput, "very bad") == 0 ||
        strcmp(lowerInput, "bad") == 0 ||
        strcmp(lowerInput, "good") == 0 ||
        strcmp(lowerInput, "very good") == 0;

    // empty check
    if (strlen(input) == 0 || input[0] == ' ' || input[0] == '\t') {
        gotoxy(19,7); error(" Cannot be empty."); info(" Press any key to continue."); getch(); goto get_condition;
    }
    // cancel check
    else if (isCancel(input)) { gotoxy(1,23); cout << "Operation cancelled."; returnMenu(); return; }
    // format check
    else if (!validCondition) { gotoxy(19,7); error(" Invalid input."); info(" Press any key to continue."); getch(); goto get_condition; }
    // store data
    strcpy(eq.condition, lowerInput);

get_hoursUsed:    // ==== HOURS ====
    gotoxy(1,9); clreol(); info("[!] Range (0 - 9999)"); // user guide
    gotoxy(1,8); clreol(); cout << "Hours used      : "; gets(input); // store input
    // empty check
    if (strlen(input) == 0 || input[0] == ' ' || input[0] == '\t') {
        gotoxy(19,8); error(" Cannot be empty."); info(" Press any key to continue."); getch(); goto get_hoursUsed;
    }
    // cancel check
    else if (isCancel(input)) { gotoxy(1,23); cout << "Operation cancelled."; returnMenu(); return; }
    // input validation
    else if (valid("int", input) == 0 && valid("float", input) == 0) {
        gotoxy(19,8); error(" Invalid input."); info(" Press any key to continue."); getch(); goto get_hoursUsed;
    }
    // store data
    else eq.usageHours = atof(input);
    // range check
    if (eq.usageHours < 0 || eq.usageHours > 9999) {
        gotoxy(19,8); error(" Out of range."); info(" Press any key to continue."); getch(); goto get_hoursUsed;
    }

get_status:    // ==== STATUS ====
    gotoxy(1,10); clreol(); info("[!] Enter (available, in use, maintenance, not working)");
    gotoxy(1,9); clreol(); cout << "Status          : "; gets(input); // store input
    for (i = 0; input[i]; i++) lowerInput[i] = tolower(input[i]); lowerInput[i] = '\0';
    int validStatus =
        strcmp(lowerInput, "available") == 0 ||
        strcmp(lowerInput, "in use") == 0 ||
        strcmp(lowerInput, "maintenance") == 0 ||
        strcmp(lowerInput, "not working") == 0;
    // empty check
    if (strlen(input) == 0 || input[0] == ' ' || input[0] == '\t') {
        gotoxy(19,9); error(" Cannot be empty."); info(" Press any key to continue."); getch(); goto get_status;
    }
    // cancel check
    else if (isCancel(input)) { gotoxy(1,23); cout << "Operation cancelled."; returnMenu(); return; }
    // format check
    else if (!validStatus) { gotoxy(19,9); error(" Invalid input."); info(" Press any key to continue."); getch(); goto get_status; }
    // store data
    else strcpy(eq.status, lowerInput);

    // ==== RECALCULATE ====
    eq.efficiency = 100 - (eq.usageHours * 0.05);
    if (eq.efficiency < 0) eq.efficiency = 0;
    if (eq.efficiency > 100) eq.efficiency = 100;
    if (strcmp(eq.status, "available") == 0) eq.availabilityRate = 100;
    else if (strcmp(eq.status, "in use") == 0) eq.availabilityRate = 80;
    else if (strcmp(eq.status, "maintenance") == 0) eq.availabilityRate = 50;
    else eq.availabilityRate = 0;
    if (eq.efficiency >= 70 && eq.availabilityRate >= 70)
        strcpy(eq.category, "functional");
    else if (eq.efficiency >= 40 && eq.availabilityRate >= 40)
        strcpy(eq.category, "for repair");
    else strcpy(eq.category, "for replacement");

    saveAllEquipment();
    gotoxy(1,23); success("Equipment updated successfully.");
    returnMenu();
}

// ######################## SUMMARY VIEW ###########################
void showSummary() {
    clrscr();
    screenTemplate(9, 3, 2, "> LABORATORY SUMMARY");

    loadEquipment();
    if (eqCounter == 0) {
        error("No data available.");
        returnMenu();
        return;
    }

    int functional = 0, repair = 0, replace = 0;
    float totalEff = 0, totalAvail = 0;

    int i1;
    for (i1 = 0; i1 < eqCounter; i1++) {
        if (strcmp(equipments[i1].category, "functional") == 0)
            functional++;
        else if (strcmp(equipments[i1].category, "for repair") == 0)
            repair++;
        else
            replace++;

        totalEff += equipments[i1].efficiency;
        totalAvail += equipments[i1].availabilityRate;
    }

    float avgEff = totalEff / eqCounter;
    float avgAvail = totalAvail / eqCounter;
    float operationalPercent = ((float)functional / eqCounter) * 100;

    // ==== DISPLAY NUMERIC SUMMARY ====
    gotoxy(1,6);  cout << "Total Equipments      : " << eqCounter;
    gotoxy(1,7);  cout << "Functional            : " << functional;
    gotoxy(1,8);  cout << "For Repair            : " << repair;
    gotoxy(1,9);  cout << "For Replacement       : " << replace;
    gotoxy(1,10); printLine(80, 11, '-');

    gotoxy(1,11); printf("Average Efficiency    : %.2f%", avgEff);
    gotoxy(1,12); printf("Average Availability  : %.2f%", avgAvail);
    gotoxy(1,13); printf("Operational Rate      : %.2f%", operationalPercent);

    // ==== RATING SUMMARY ====
    char rating[25];
    if (operationalPercent >= 80)       strcpy(rating, "Excellent");
    else if (operationalPercent >= 60)  strcpy(rating, "Good");
    else if (operationalPercent >= 40)  strcpy(rating, "Needs Improvement");
    else                                strcpy(rating, "Critical");

    gotoxy(1,15); textcolor(YELLOW);
    cprintf("Overall Lab Condition : %s", rating);
    textcolor(WHITE);

    // ==== SIMPLE VISUAL BAR GRAPH (1 # per item) ====
    gotoxy(1,17); cout << "Condition Overview:";

    int i2;
    gotoxy(1,18); textcolor(LIGHTGREEN);
    cprintf("Functional    : ");
    for (i2 = 0; i2 < functional; i2++) cprintf("#");
    textcolor(WHITE); cout << " (" << functional << ")";

    int i3;
    gotoxy(1,19); textcolor(YELLOW);
    cprintf("For Repair    : ");
    for (i3 = 0; i3 < repair; i3++) cprintf("#");
    textcolor(WHITE); cout << " (" << repair << ")";

    int i4;
    gotoxy(1,20); textcolor(RED);
    cprintf("Replacement   : ");
    for (i4 = 0; i4 < replace; i4++) cprintf("#");
    textcolor(WHITE); cout << " (" << replace << ")";

    gotoxy(1,22); info("Press any key to return...");
    getch();
}

// ######################## AUTO-CREATE FILE ############################
void ensureFileExists() {
    ofstream ensure(equipments_file_path, ios::app);
    ensure.close();
}

// ################### SAFE DATA RELOAD AFTER DELETE #######################
void refreshDataAfterDelete() {
    saveAllEquipment();
    loadEquipment();
}

// ################### EXIT CONFIRMATION #########################
int confirmExit() {
    gotoxy(1,23); clreol();
    cout << "Are you sure you want to exit? (Y/N): ";
    char confirm = getch();
    if (tolower(confirm) == 'y')
        return 1;   // true
    return 0;       // false
}

int main(){
    char ans[1000];
    int option;

    ensureFileExists(); 

    do {
        clrscr();
        displayMenu();
    get_ans:
        gotoxy(1,22); clreol(); cout <<"Enter your choice [0-5]: "; gets(ans);
        if(strcmp(ans, "1") == 0) option = 1;
        else if(strcmp(ans, "2") == 0) option = 2;
        else if(strcmp(ans, "3") == 0) option = 3;
        else if(strcmp(ans, "4") == 0) option = 4;
        else if(strcmp(ans, "5") == 0) option = 5;
        else if(strcmp(ans, "0") == 0) option = 0;
        else {
            gotoxy(1,23);
            error("Invalid input.");
            goto get_ans;
        }

        switch (option){
            case 0:
                if (confirmExit()) 
                   return 0;
                else {
                    option = -1;
                    break;
                }
            case 1:
                displayEquipments();
                break;
            case 2:
                searchEquipmentByName();
                break;
            case 3:
                addEquipments();
                break;
            case 4:
                editEquipment();   //inadd
                break;
            case 5:
                showSummary();     
                break;
            default:
                break;
        }
    } while (option != 0);

    getch();
    return 0;
}
