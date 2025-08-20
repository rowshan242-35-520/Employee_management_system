#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <time.h>
#include <conio.h>

struct employee {
    int id;
    char name[100];
    int age;
    char gender[10];
    char department[20];
    float salary;
    char phn[15];
    char email[50];
    char joinDate[15];
    char address[30];
    char bloodGroup[10];
    char Nid[25];
    char employeeType[20];
    char shift[15];
    char branch[30];
    char bankAccount[30];
    int leaveCount;
    char mastersInst[100], mastersSub[50];
    double mastersResult;
    int mastersYear;
    char bachelorsInst[100], bachelorsSub[50];
    double bachelorsResult;
    int bachelorsYear;
    char hscInst[100], hscSub[50];
    double hscResult;
    int hscYear;
    char sscInst[100], sscSub[50];
    double sscResult;
    int sscYear;
    char emrName[50], emrPhone[20], emrRelation[30];
};

char globalNotice[500] = "";
//ln 75
int visibleWidth(const char *s) {
    int width = 0;
    for (int i = 0; s[i];) {
        if ((s[i] & 0x80) == 0) {
            width += 1;
            i += 1;
        } else if ((s[i] & 0xE0) == 0xC0) {
            width += 1;
            i += 2;
        } else if ((s[i] & 0xF0) == 0xE0) {
            width += 1;
            i += 3;
        } else if ((s[i] & 0xF8) == 0xF0) {
            width += 1;
            i += 4;
        } else {
            i += 1;
        }
    }
    return width;
}
//ln 74, 94, 622, 887, 938
int getConsoleWidth() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int width = 80;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    }
    return width;
}
void printCenteredLine(const char *line) {
    int screenWidth = getConsoleWidth();//ln 64
    int visLen = visibleWidth(line);//ln 43
    int padding = (screenWidth - visLen) / 2;
    if (padding < 0) padding = 0;
    for (int j = 0; j < padding; j++) printf(" ");
    printf("%s\n", line);
}

void printCenteredLogo() {
    const char *logo[] = {
        " ██████████ ██████   ██████  █████████ ",
        "░░███░░░░░█░░██████ ██████  ███░░░░░███",
        " ░███  █ ░  ░███░█████░███ ░███    ░░░ ",
        " ░██████    ░███░░███ ░███ ░░█████████ ",
        " ░███░░█    ░███ ░░░  ░███  ░░░░░░░░███",
        " ░███ ░   █ ░███      ░███  ███    ░███",
        " ██████████ █████     █████░░█████████ ",
        "░░░░░░░░░░ ░░░░░     ░░░░░  ░░░░░░░░░  "
    };
    int logoLines = sizeof(logo) / sizeof(logo[0]);
    int screenWidth = getConsoleWidth();//65
    for (int i = 0; i < logoLines; i++) {
        int visLen = visibleWidth(logo[i]);
        int padding = (screenWidth - visLen) / 2;
        if (padding < 0) padding = 0;
        for (int j = 0; j < padding; j++) printf(" ");
        printf("%s\n", logo[i]);
    }
    printf("\n");
}

void setTextColorLightBlue() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 11);
}
//873
void getToday(char *buffer, int size) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    snprintf(buffer, size, "%02d-%02d-%04d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
}
// ln 984
int employeeExists(int id) {
    FILE *file = fopen("ems.txt", "r");
    if (!file) return 0;
    char line[500];
    while (fgets(line, sizeof(line), file)) {
        int fileId;
        sscanf(line, "%d|", &fileId);
        if (fileId == id) {
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}
//ln 311, 351, 391, 582
int getEmployeeById(int id, struct employee *out) {
    FILE *file = fopen("ems.txt", "r");
    if (!file) return 0;
    char line[2000];
    while (fgets(line, sizeof(line), file)) {
        struct employee e;
        sscanf(line,
            "%d|%[^|]|%d|%[^|]|%[^|]|%f|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%d|"
            "%[^|]|%lf|%[^|]|%d|"
            "%[^|]|%lf|%[^|]|%d|"
            "%[^|]|%lf|%[^|]|%d|"
            "%[^|]|%lf|%[^|]|%d|"
            "%[^|]|%[^|]|%[^|]",
            &e.id, e.name, &e.age, e.gender, e.department, &e.salary, e.phn, e.email, e.joinDate, e.address,
            e.bloodGroup, e.Nid, e.employeeType, e.shift, e.branch, e.bankAccount, &e.leaveCount,
            e.mastersInst, &e.mastersResult, e.mastersSub, &e.mastersYear,
            e.bachelorsInst, &e.bachelorsResult, e.bachelorsSub, &e.bachelorsYear,
            e.hscInst, &e.hscResult, e.hscSub, &e.hscYear,
            e.sscInst, &e.sscResult, e.sscSub, &e.sscYear,
            e.emrName, e.emrPhone, e.emrRelation
        );
        if (e.id == id) {
            if (out) *out = e;
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}
// ln 583
void updateEmployeeLeaveCount(int id, int newLeaveCount) {
    FILE *file = fopen("ems.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    char line[500];
    while (fgets(line, sizeof(line), file)) {
        struct employee e;
        sscanf(line, "%d|%[^|]|%d|%[^|]|%[^|]|%f|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%d",
            &e.id, e.name, &e.age, e.gender, e.department, &e.salary, e.phn, e.email, e.joinDate, e.address,
            e.bloodGroup, e.Nid, e.employeeType, e.shift, e.branch, e.bankAccount, &e.leaveCount);
        if (e.id == id) e.leaveCount = newLeaveCount;
        fprintf(temp, "%d|%s|%d|%s|%s|%.2f|%s|%s|%s|%s|%s|%s|%s|%s|%s|%s|%d\n",
            e.id, e.name, e.age, e.gender, e.department, e.salary, e.phn, e.email, e.joinDate, e.address,
            e.bloodGroup, e.Nid, e.employeeType, e.shift, e.branch, e.bankAccount, e.leaveCount);
    }
    fclose(file);
    fclose(temp);
    remove("ems.txt");
    rename("temp.txt", "ems.txt");
}

struct application {
    int appId;
    int empId;
    char empName[100];
    char type[30];
    char leaveType[30];
    char message[1001];
    char status[20];
};
//ln 398
int getNextApplicationId() {
    FILE *file = fopen("applications.txt", "r");
    int maxId = 0;
    char line[1200];
    while (file && fgets(line, sizeof(line), file)) {
        int id;
        sscanf(line, "%d|", &id);
        if (id > maxId) maxId = id;
    }
    if (file) fclose(file);
    return maxId + 1;
}
//ln 422
void saveApplication(struct application *app) {
    FILE *file = fopen("applications.txt", "a");
    if (!file) return;
    fprintf(file, "%d|%d|%s|%s|%s|%s|%s\n", app->appId, app->empId, app->empName, app->type, app->leaveType, app->message, app->status);
    fclose(file);
}
//ln 993
void employeeRegistration() {
    struct employee emp;
    printf("\nEnter Employee ID: ");
    scanf("%d", &emp.id); getchar();
    printf("Enter Employee Name: ");
    scanf(" %[^\n]", emp.name);
    printf("Enter Age: ");
    scanf("%d", &emp.age); getchar();
    printf("Enter Gender: ");
    scanf(" %[^\n]", emp.gender);
    printf("Enter Department: ");
    scanf(" %[^\n]", emp.department);
    printf("Enter Salary: ");
    scanf("%f", &emp.salary); getchar();
    printf("Enter Phone Number: ");
    scanf(" %[^\n]", emp.phn);
    printf("Enter Email: ");
    scanf(" %[^\n]", emp.email);
    printf("Enter Join Date (dd-mm-yyyy): ");
    scanf(" %[^\n]", emp.joinDate);
    printf("Enter Address: ");
    scanf(" %[^\n]", emp.address);
    printf("Enter Blood Group: ");
    scanf(" %[^\n]", emp.bloodGroup);
    printf("Enter NID Number: ");
    scanf(" %[^\n]", emp.Nid);
    printf("Enter Employee Type (Full time/Intern): ");
    scanf(" %[^\n]", emp.employeeType);
    printf("Enter Shift (Morning/Evening/Night): ");
    scanf(" %[^\n]", emp.shift);
    printf("Enter Branch (Uttara/Mirpur/Dhanmondi): ");
    scanf(" %[^\n]", emp.branch);
    printf("Enter Bank Account: ");
    scanf(" %[^\n]", emp.bankAccount);
    emp.leaveCount = 15;

    printf("\n--- Educational Details ---\n");
    printf("\n--- Masters ---\n");
    printf("Institution: "); scanf(" %[^\n]", emp.mastersInst);
    printf("Result: "); scanf("%lf", &emp.mastersResult);
    printf("Subject: "); scanf(" %[^\n]", emp.mastersSub);
    printf("Passing Year: "); scanf("%d", &emp.mastersYear);

    printf("\n--- Bachelors ---\n");
    printf("Institution: "); scanf(" %[^\n]", emp.bachelorsInst);
    printf("Result: "); scanf("%lf", &emp.bachelorsResult);
    printf("Subject: "); scanf(" %[^\n]", emp.bachelorsSub);
    printf("Passing Year: "); scanf("%d", &emp.bachelorsYear);

    printf("\n--- HSC ---\n");
    printf("Institution: "); scanf(" %[^\n]", emp.hscInst);
    printf("Result: "); scanf("%lf", &emp.hscResult);
    printf("Subject: "); scanf(" %[^\n]", emp.hscSub);
    printf("Passing Year: "); scanf("%d", &emp.hscYear);

    printf("\n--- SSC ---\n");
    printf("Institution: "); scanf(" %[^\n]", emp.sscInst);
    printf("Result: "); scanf("%lf", &emp.sscResult);
    printf("Subject: "); scanf(" %[^\n]", emp.sscSub);
    printf("Passing Year: "); scanf("%d", &emp.sscYear);

    printf("\n--- Emergency Contact ---\n");
    printf("Name: "); scanf(" %[^\n]", emp.emrName);
    printf("Phone: "); scanf(" %[^\n]", emp.emrPhone);
    printf("Relation: "); scanf(" %[^\n]", emp.emrRelation);

    FILE *file = fopen("ems.txt", "a");
    if (!file) {
        printf("File could not be opened!\n");
        return;
    }
    fprintf(file,
        "%d|%s|%d|%s|%s|%.2f|%s|%s|%s|%s|%s|%s|%s|%s|%s|%s|%d|"
        "%s|%.2lf|%s|%d|"
        "%s|%.2lf|%s|%d|"
        "%s|%.2lf|%s|%d|"
        "%s|%.2lf|%s|%d|"
        "%s|%s|%s\n",
        emp.id, emp.name, emp.age, emp.gender, emp.department, emp.salary, emp.phn, emp.email, emp.joinDate, emp.address,
        emp.bloodGroup, emp.Nid, emp.employeeType, emp.shift, emp.branch, emp.bankAccount, emp.leaveCount,
        emp.mastersInst, emp.mastersResult, emp.mastersSub, emp.mastersYear,
        emp.bachelorsInst, emp.bachelorsResult, emp.bachelorsSub, emp.bachelorsYear,
        emp.hscInst, emp.hscResult, emp.hscSub, emp.hscYear,
        emp.sscInst, emp.sscResult, emp.sscSub, emp.sscYear,
        emp.emrName, emp.emrPhone, emp.emrRelation
    );
    fclose(file);
    printf("\nEmployee Registered Successfully.\n\n");
    printf("\nPress Enter to continue...");
    getchar();
    getchar();
}
//ln 693
void searchEmployeeById() {
    int id;
    printf("Enter Employee ID to search: ");
    scanf("%d", &id); getchar();
    struct employee e;
    if (getEmployeeById(id, &e)) {// ln 132
        printf("\n------------------ Employee Profile ------------------\n");
        printf("%-20s  %-30s\n", "ID", e.id == 0 ? "" : e.name);
        printf("%-20s  %-30d\n", "Employee ID", e.id);
        printf("%-20s  %-30s\n", "Name", e.name);
        printf("%-20s  %-30d\n", "Age", e.age);
        printf("%-20s  %-30s\n", "Gender", e.gender);
        printf("%-20s  %-30s\n", "Department", e.department);
        printf("%-20s  %-30.2f\n", "Salary", e.salary);
        printf("%-20s  %-30s\n", "Phone", e.phn);
        printf("%-20s  %-30s\n", "Email", e.email);
        printf("%-20s  %-30s\n", "Join Date", e.joinDate);
        printf("%-20s  %-30s\n", "Address", e.address);
        printf("%-20s  %-30s\n", "Blood Group", e.bloodGroup);
        printf("%-20s  %-30s\n", "NID", e.Nid);
        printf("%-20s  %-30s\n", "Employee Type", e.employeeType);
        printf("%-20s  %-30s\n", "Shift", e.shift);
        printf("%-20s  %-30s\n", "Branch", e.branch);
        printf("%-20s  %-30s\n", "Bank Account", e.bankAccount);
        printf("%-20s  %-30d\n", "Leaves Left", e.leaveCount);
        printf("\n------------------ Emergency Contact ------------------\n");
        printf("\n%-20s  %-30s\n", "Name", e.emrName);
        printf("%-20s  %-30s\n", "Phone", e.emrPhone);
        printf("%-20s  %-30s\n", "Relation", e.emrRelation);

        printf("\n%-20s | %-30s | %-10s | %-10s | %-10s\n", "Degree", "Institution", "Result", "Subject", "Year");
        printf("%-20s | %-30s | %-10.2lf | %-10s | %-10d\n", "Masters", e.mastersInst, e.mastersResult, e.mastersSub, e.mastersYear);
        printf("%-20s | %-30s | %-10.2lf | %-10s | %-10d\n", "Bachelors", e.bachelorsInst, e.bachelorsResult, e.bachelorsSub, e.bachelorsYear);
        printf("%-20s | %-30s | %-10.2lf | %-10s | %-10d\n", "HSC", e.hscInst, e.hscResult, e.hscSub, e.hscYear);
        printf("%-20s | %-30s | %-10.2lf | %-10s | %-10d\n", "SSC", e.sscInst, e.sscResult, e.sscSub, e.sscYear);
        printf("------------------------------------------------------\n");
    } else {
        printf("Employee not found.\n");
    }
    printf("\nPress Enter to continue...");
    getchar();
}
//ln 916, 686
void viewProfile(int empId) {
    struct employee e;
    if (getEmployeeById(empId, &e)) {// ln 132
        printf("\n------------------ Employee Profile ------------------\n");
        printf("%-20s  %-30s\n", "ID", e.id == 0 ? "" : e.name);
        printf("%-20s  %-30d\n", "Employee ID", e.id);
        printf("%-20s  %-30s\n", "Name", e.name);
        printf("%-20s  %-30d\n", "Age", e.age);
        printf("%-20s  %-30s\n", "Gender", e.gender);
        printf("%-20s  %-30s\n", "Department", e.department);
        printf("%-20s  %-30.2f\n", "Salary", e.salary);
        printf("%-20s  %-30s\n", "Phone", e.phn);
        printf("%-20s  %-30s\n", "Email", e.email);
        printf("%-20s  %-30s\n", "Join Date", e.joinDate);
        printf("%-20s  %-30s\n", "Address", e.address);
        printf("%-20s  %-30s\n", "Blood Group", e.bloodGroup);
        printf("%-20s  %-30s\n", "NID", e.Nid);
        printf("%-20s  %-30s\n", "Employee Type", e.employeeType);
        printf("%-20s  %-30s\n", "Shift", e.shift);
        printf("%-20s  %-30s\n", "Branch", e.branch);
        printf("%-20s  %-30s\n", "Bank Account", e.bankAccount);
        printf("%-20s  %-30d\n", "Leaves Left", e.leaveCount);
        printf("\n------------------ Emergency Contact ------------------\n");
        printf("\n%-20s  %-30s\n", "Name", e.emrName);
        printf("%-20s  %-30s\n", "Phone", e.emrPhone);
        printf("%-20s  %-30s\n", "Relation", e.emrRelation);

        printf("\n%-20s | %-30s | %-10s | %-10s | %-10s\n", "Degree", "Institution", "Result", "Subject", "Year");
        printf("%-20s | %-30s | %-10.2lf | %-10s | %-10d\n", "Masters", e.mastersInst, e.mastersResult, e.mastersSub, e.mastersYear);
        printf("%-20s | %-30s | %-10.2lf | %-10s | %-10d\n", "Bachelors", e.bachelorsInst, e.bachelorsResult, e.bachelorsSub, e.bachelorsYear);
        printf("%-20s | %-30s | %-10.2lf | %-10s | %-10d\n", "HSC", e.hscInst, e.hscResult, e.hscSub, e.hscYear);
        printf("%-20s | %-30s | %-10.2lf | %-10s | %-10d\n", "SSC", e.sscInst, e.sscResult, e.sscSub, e.sscYear);
        printf("------------------------------------------------------\n");
    } else {
        printf("Profile not found.\n");
    }
    printf("\nPress Enter to continue...");
    getchar();
}
//ln918
void makeApplication(int empId) {
    struct employee e;
    if (!getEmployeeById(empId, &e)) {// ln 132
        printf("Employee not found.\n");
        printf("\nPress Enter to continue...");
        getchar();
        return;
    }
    struct application app;
    app.appId = getNextApplicationId();//ln 193
    app.empId = empId;
    strcpy(app.empName, e.name);
    strcpy(app.status, "pending");
    printf("Application Type:\n1. Leave\n2. General\nEnter choice: ");
    int t;
    scanf("%d", &t); getchar();
    if (t == 1) {
        strcpy(app.type, "leave");
        printf("Leave Type:\n1. Casual\n2. Sick\n3. Maternity\n4. Study\nEnter choice: ");
        int lt;
        scanf("%d", &lt); getchar();
        if (lt == 1) strcpy(app.leaveType, "casual");
        else if (lt == 2) strcpy(app.leaveType, "sick");
        else if (lt == 3) strcpy(app.leaveType, "maternity");
        else if (lt == 4) strcpy(app.leaveType, "study");
        else strcpy(app.leaveType, "other");
    } else {
        strcpy(app.type, "general");
        strcpy(app.leaveType, "-");
    }
    printf("Write your application (max 1000 chars):\n");
    fgets(app.message, 1000, stdin);
    app.message[strcspn(app.message, "\n")] = 0;
    saveApplication(&app);//ln 206
    printf("Application submitted.\nPress Enter to continue...");
    getchar();
}
//920
void viewApplicationStatus(int empId) {
    FILE *file = fopen("applications.txt", "r");
    char line[1200];
    int found = 0;
    while (file && fgets(line, sizeof(line), file)) {
        struct application app;
        sscanf(line, "%d|%d|%[^|]|%[^|]|%[^|]|%[^|]|%[^\n]",
            &app.appId, &app.empId, app.empName, app.type, app.leaveType, app.message, app.status);
        if (app.empId == empId) {
            found = 1;
            printf("\nApplication ID: %d\nType: %s\nLeave Type: %s\nStatus: %s\nMessage: %s\n",
                app.appId, app.type, app.leaveType, app.status, app.message);
        }
    }
    if (!found) printf("No applications were made.\n");
    if (file) fclose(file);
    printf("\nPress Enter to continue...");
    getchar();
}
//816, 818
void displayApplicationsByStatus(const char *status) {
    FILE *file = fopen("applications.txt", "r");
    char line[1200];
    struct application apps[200];
    int count = 0;
    while (file && fgets(line, sizeof(line), file)) {
        struct application app;
        sscanf(line, "%d|%d|%[^|]|%[^|]|%[^|]|%[^|]|%[^\n]",
            &app.appId, &app.empId, app.empName, app.type, app.leaveType, app.message, app.status);
        if (strcmp(app.status, status) == 0) {
            apps[count++] = app;
        }
    }
    if (file) fclose(file);
    if (count == 0) {
        printf("No %s applications.\n", status);
        printf("\nPress Enter to continue...");
        getchar();
        return;
    }
    printf("\n%-12s %-12s %-12s %-14s %-10s\n", "App ID", "Emp ID", "Type", "Leave Type", "Status");
    for (int i = 0; i < count; i++) {
        printf("%-12d %-12d %-12s %-14s %-10s\n", apps[i].appId, apps[i].empId, apps[i].type, apps[i].leaveType, apps[i].status);
    }
    printf("\nOpen an application? (y/n): ");
    char ch = getchar();
    getchar();
    if (ch == 'y' || ch == 'Y') {
        int openId;
        printf("Enter Application ID: ");
        scanf("%d", &openId); getchar();
        int found = 0;
        for (int i = 0; i < count; i++) {
            if (apps[i].appId == openId) {
                printf("\nApplication ID: %d\nEmployee ID: %d\nName: %s\nType: %s\nLeave Type: %s\nStatus: %s\nMessage: %s\n",
                    apps[i].appId, apps[i].empId, apps[i].empName, apps[i].type, apps[i].leaveType, apps[i].status, apps[i].message);
                found = 1;
                break;
            }
        }
        if (!found) printf("Application not found.\n");
        printf("\nPress Enter to return...");
        getchar();
    }
}
//820
void displayApplicationHistory() {
    FILE *file = fopen("applications.txt", "r");
    char line[1200];
    struct application apps[200];
    int count = 0;
    while (file && fgets(line, sizeof(line), file)) {
        struct application app;
        sscanf(line, "%d|%d|%[^|]|%[^|]|%[^|]|%[^|]|%[^\n]",
            &app.appId, &app.empId, app.empName, app.type, app.leaveType, app.message, app.status);
        if (strcmp(app.status, "approved") == 0 || strcmp(app.status, "denied") == 0) {
            apps[count++] = app;
        }
    }
    if (file) fclose(file);
    if (count == 0) {
        printf("No application history.\n");
        printf("\nPress Enter to continue...");
        getchar();
        return;
    }
    printf("\n%-12s %-12s %-12s %-14s %-10s\n", "App ID", "Emp ID", "Type", "Leave Type", "Status");
    for (int i = 0; i < count; i++) {
        printf("%-12d %-12d %-12s %-14s %-10s\n", apps[i].appId, apps[i].empId, apps[i].type, apps[i].leaveType, apps[i].status);
    }
    printf("\nOpen an application? (y/n): ");
    char ch = getchar();
    getchar();
    if (ch == 'y' || ch == 'Y') {
        int openId;
        printf("Enter Application ID: ");
        scanf("%d", &openId); getchar();
        int found = 0;
        for (int i = 0; i < count; i++) {
            if (apps[i].appId == openId) {
                printf("\nApplication ID: %d\nEmployee ID: %d\nName: %s\nType: %s\nLeave Type: %s\nStatus: %s\nMessage: %s\n",
                    apps[i].appId, apps[i].empId, apps[i].empName, apps[i].type, apps[i].leaveType, apps[i].status, apps[i].message);
                found = 1;
                break;
            }
        }
        if (!found) printf("Application not found.\n");
        printf("\nPress Enter to return...");
        getchar();
    }
}

void handleNewApplications() {
    FILE *file = fopen("applications.txt", "r");
    char line[1200];
    struct application apps[100];
    int count = 0;
    while (file && fgets(line, sizeof(line), file)) {
        struct application app;
        sscanf(line, "%d|%d|%[^|]|%[^|]|%[^|]|%[^|]|%[^\n]",
            &app.appId, &app.empId, app.empName, app.type, app.leaveType, app.message, app.status);
        if (strcmp(app.status, "pending") == 0) {
            apps[count++] = app;
        }
    }
    if (file) fclose(file);
    if (count == 0) {
        printf("No new applications.\n");
        printf("\nPress Enter to continue...");
        getchar();
        return;
    }
    printf("\n%-12s %-12s %-12s %-14s %-10s\n", "App ID", "Emp ID", "Type", "Leave Type", "Status");
    for (int i = 0; i < count; i++) {
        printf("%-12d %-12d %-12s %-14s %-10s\n", apps[i].appId, apps[i].empId, apps[i].type, apps[i].leaveType, apps[i].status);
    }
    printf("\nOpen an application? (y/n): ");
    char ch = getchar();
    getchar();
    if (ch == 'y' || ch == 'Y') {
        int openId;
        printf("Enter Application ID: ");
        scanf("%d", &openId); getchar();
        int found = 0;
        for (int i = 0; i < count; i++) {
            if (apps[i].appId == openId) {
                printf("\nApplication ID: %d\nEmployee ID: %d\nName: %s\nType: %s\nLeave Type: %s\nStatus: %s\nMessage: %s\n",
                    apps[i].appId, apps[i].empId, apps[i].empName, apps[i].type, apps[i].leaveType, apps[i].status, apps[i].message);
                printf("Approve (1) / Deny (2) / Skip (other): ");
                int ch2;
                scanf("%d", &ch2); getchar();
                if (ch2 == 1) {
                    strcpy(apps[i].status, "approved");
                    if (strcmp(apps[i].type, "leave") == 0) {
                        struct employee e;
                        if (getEmployeeById(apps[i].empId, &e) && e.leaveCount > 0) {// ln 132
                            updateEmployeeLeaveCount(apps[i].empId, e.leaveCount - 1);// ln 163
                        }
                    }
                } else if (ch2 == 2) {
                    strcpy(apps[i].status, "denied");
                }
                FILE *rf = fopen("applications.txt", "r");
                FILE *wf = fopen("apptemp.txt", "w");
                char l[1200];
                while (rf && fgets(l, sizeof(l), rf)) {
                    struct application a;
                    sscanf(l, "%d|%d|%[^|]|%[^|]|%[^|]|%[^|]|%[^\n]",
                        &a.appId, &a.empId, a.empName, a.type, a.leaveType, a.message, a.status);
                    if (a.appId == apps[i].appId) {
                        fprintf(wf, "%d|%d|%s|%s|%s|%s|%s\n", apps[i].appId, apps[i].empId, apps[i].empName, apps[i].type, apps[i].leaveType, apps[i].message, apps[i].status);
                    } else {
                        fputs(l, wf);
                    }
                }
                if (rf) fclose(rf);
                if (wf) fclose(wf);
                remove("applications.txt");
                rename("apptemp.txt", "applications.txt");
                found = 1;
                break;
            }
        }
        if (!found) printf("Application not found.\n");
        printf("\nPress Enter to return...");
        getchar();
    }
}
//974
void adminMenu() {
    int n;
    while (1) {
        system("cls");
        setTextColorLightBlue();
        printCenteredLogo();
        int screenWidth = getConsoleWidth();//65
        char menu[7][40] = {
            "1   Display All Employees",
            "2   Search Employee by ID",
            "3   Delete Employee",
            "4   Application Requests",
            "5   Publish Notice",
            "6   Exit to Login"
        };
        int menuLen = 6;
        int maxLen = 0;
        for (int i = 0; i < menuLen; i++) {
            int len = strlen(menu[i]);
            if (len > maxLen) maxLen = len;
        }
        int padding = (screenWidth - maxLen) / 2;
        printf("\n");
        for (int i = 0; i < menuLen; i++) {
            for (int j = 0; j < padding; j++) printf(" ");
            printf("%s\n", menu[i]);
        }
        printf("\n");
        for (int j = 0; j < padding; j++) printf(" ");
        printf("Enter your choice: ");
        scanf("%d", &n); getchar();
        system("cls");
        setTextColorLightBlue();
        switch (n) {
            case 1: {
                FILE *file = fopen("ems.txt", "r");
                if (file == NULL) {
                    printf("No employee data found.\n\n");
                    break;
                }
                char line[2000];
                printf("\n%-15s %-30s\n", "Employee ID", "Employee Name");
                printf("---------------------------------------------\n");
                while (fgets(line, sizeof(line), file)) {
                    struct employee e;
                    sscanf(line,
                        "%d|%[^|]|%d|%[^|]|%[^|]|%f|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%d|"
                        "%[^|]|%lf|%[^|]|%d|"
                        "%[^|]|%lf|%[^|]|%d|"
                        "%[^|]|%lf|%[^|]|%d|"
                        "%[^|]|%lf|%[^|]|%d|"
                        "%[^|]|%[^|]|%[^|]",
                        &e.id, e.name, &e.age, e.gender, e.department, &e.salary, e.phn, e.email, e.joinDate, e.address,
                        e.bloodGroup, e.Nid, e.employeeType, e.shift, e.branch, e.bankAccount, &e.leaveCount,
                        e.mastersInst, &e.mastersResult, e.mastersSub, &e.mastersYear,
                        e.bachelorsInst, &e.bachelorsResult, e.bachelorsSub, &e.bachelorsYear,
                        e.hscInst, &e.hscResult, e.hscSub, &e.hscYear,
                        e.sscInst, &e.sscResult, e.sscSub, &e.sscYear,
                        e.emrName, e.emrPhone, e.emrRelation
                    );
                    printf("%-15d %-30s\n", e.id, e.name);
                }
                fclose(file);
                printf("\nDo you want to view an employee profile? (y/n): ");
                char ch = getchar();
                getchar();
                if (ch == 'y' || ch == 'Y') {
                    int empId;
                    printf("Enter Employee ID: ");
                    scanf("%d", &empId); getchar();
                    viewProfile(empId);//ln 349
                }
                printf("\nPress Enter to continue...");
                getchar();
                break;
            }
            case 2:
                searchEmployeeById();//ln 306
                break;
            case 3: {
                int del;
                printf("\n");
                for (int j = 0; j < padding; j++) printf(" ");
                printf("Delete Employee:\n");
                for (int j = 0; j < padding; j++) printf(" ");
                printf("1   By ID\n");
                for (int j = 0; j < padding; j++) printf(" ");
                printf("2   By Name\n");
                for (int j = 0; j < padding; j++) printf(" ");
                printf("Enter your choice: ");
                scanf("%d", &del); getchar();

                FILE* file = fopen("ems.txt", "r");
                if (file == NULL) {
                    printf("No employee data found.\n");
                    break;
                }

                FILE* temp = fopen("temp.txt", "w");
                if (temp == NULL) {
                    printf("Temporary file could not be created.\n");
                    fclose(file);
                    break;
                }

                int found = 0;
                char line[2000];

                if (del == 1) {
                    int delId;
                    for (int j = 0; j < padding; j++) printf(" ");
                    printf("Enter ID to delete: ");
                    scanf("%d", &delId); getchar();

                    while (fgets(line, sizeof(line), file)) {
                        struct employee e;
                        sscanf(line,
                            "%d|%[^|]|%d|%[^|]|%[^|]|%f|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%d|"
                            "%[^|]|%lf|%[^|]|%d|"
                            "%[^|]|%lf|%[^|]|%d|"
                            "%[^|]|%lf|%[^|]|%d|"
                            "%[^|]|%lf|%[^|]|%d|"
                            "%[^|]|%[^|]|%[^|]",
                            &e.id, e.name, &e.age, e.gender, e.department, &e.salary, e.phn, e.email, e.joinDate, e.address,
                            e.bloodGroup, e.Nid, e.employeeType, e.shift, e.branch, e.bankAccount, &e.leaveCount,
                            e.mastersInst, &e.mastersResult, e.mastersSub, &e.mastersYear,
                            e.bachelorsInst, &e.bachelorsResult, e.bachelorsSub, &e.bachelorsYear,
                            e.hscInst, &e.hscResult, e.hscSub, &e.hscYear,
                            e.sscInst, &e.sscResult, e.sscSub, &e.sscYear,
                            e.emrName, e.emrPhone, e.emrRelation
                        );
                        if (e.id == delId) {
                            found = 1;
                            printf("Employee with ID %d deleted.\n", delId);
                        } else {
                            fputs(line, temp);
                        }
                    }
                } else if (del == 2) {
                    char delName[100];
                    for (int j = 0; j < padding; j++) printf(" ");
                    printf("Enter Name to delete: ");
                    scanf(" %[^\n]", delName);

                    while (fgets(line, sizeof(line), file)) {
                        struct employee e;
                        sscanf(line,
                            "%d|%[^|]|%d|%[^|]|%[^|]|%f|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%d|"
                            "%[^|]|%lf|%[^|]|%d|"
                            "%[^|]|%lf|%[^|]|%d|"
                            "%[^|]|%lf|%[^|]|%d|"
                            "%[^|]|%lf|%[^|]|%d|"
                            "%[^|]|%[^|]|%[^|]",
                            &e.id, e.name, &e.age, e.gender, e.department, &e.salary, e.phn, e.email, e.joinDate, e.address,
                            e.bloodGroup, e.Nid, e.employeeType, e.shift, e.branch, e.bankAccount, &e.leaveCount,
                            e.mastersInst, &e.mastersResult, e.mastersSub, &e.mastersYear,
                            e.bachelorsInst, &e.bachelorsResult, e.bachelorsSub, &e.bachelorsYear,
                            e.hscInst, &e.hscResult, e.hscSub, &e.hscYear,
                            e.sscInst, &e.sscResult, e.sscSub, &e.sscYear,
                            e.emrName, e.emrPhone, e.emrRelation
                        );
                        if (strcasecmp(e.name, delName) == 0) {
                            found = 1;
                            printf("Employee named '%s' deleted.\n", delName);
                        } else {
                            fputs(line, temp);
                        }
                    }
                }

                fclose(file);
                fclose(temp);

                if (!found) {
                    printf("Employee not found.\n");
                    remove("temp.txt");
                    break;
                }

                remove("ems.txt");
                rename("temp.txt", "ems.txt");

                printf("\nPress Enter to continue...");
                getchar();
                break;
            }
            case 4: {
                int reqChoice;
                while (1) {
                    printf("\n--- Application Requests ---\n");
                    printf("1. New Applications\n");
                    printf("2. Accepted Applications\n");
                    printf("3. Rejected Applications\n");
                    printf("4. Application History\n");
                    printf("5. Return\n");
                    printf("Enter your choice: ");
                    scanf("%d", &reqChoice); getchar();
                    if (reqChoice == 1) {
                        handleNewApplications();//539
                    } else if (reqChoice == 2) {
                        displayApplicationsByStatus("approved");//446
                    } else if (reqChoice == 3) {
                        displayApplicationsByStatus("denied");//446
                    } else if (reqChoice == 4) {
                        displayApplicationHistory();//493
                    } else if (reqChoice == 5) {
                        break;
                    } else {
                        printf("Invalid input. Press Enter to continue...");
                        getchar();
                    }
                }
                break;
            }
            case 5: {
                printf("\nEnter notice to publish (max 500 chars):\n");
                fgets(globalNotice, sizeof(globalNotice), stdin);
                globalNotice[strcspn(globalNotice, "\n")] = 0;
                printf("Notice published.\nPress Enter to continue...");
                getchar();
                break;
            }
            case 6:
                return;
            default:
                printf("Invalid input. Please try again.\n\n");
                printf("Press Enter to continue...");
                getchar();
        }
    }
}

void getPassword(char *password, int maxLen) {//971
    int i = 0;
    char ch;
    while (1) {
        ch = _getch(); // get char without showing it

        if (ch == 13) { // Enter key
            break;
        }
        else if (ch == 8) { // Backspace
            if (i > 0) {
                i--;
                printf("\b \b"); // erase last *
            }
        }
        else if (i < maxLen - 1) {
            password[i++] = ch;
            printf("*");
        }
    }
    password[i] = '\0';
}
//985
void logEmployeeLogin(int empId) {
    char today[30];
    getToday(today, sizeof(today));//110
    FILE *file = fopen("login_log.txt", "a");
    if (file) {
        fprintf(file, "%d|%s\n", empId, today);
        fclose(file);
    }
}
//986
void employeeMenu(int empId) {
    int choice;
    while (1) {
        system("cls");
        setTextColorLightBlue();
        printCenteredLogo();
        int screenWidth = getConsoleWidth();//65
        char menu[6][40] = {
            "1   View Profile",
            "2   Make an Application",
            "3   View Application Status",
            "4   Exit"
        };
        int menuLen = 4;
        int maxLen = 0;
        for (int i = 0; i < menuLen; i++) {
            int len = strlen(menu[i]);
            if (len > maxLen) maxLen = len;
        }
        int padding = (screenWidth - maxLen) / 2;
        printf("\n");
        for (int i = 0; i < menuLen; i++) {
            for (int j = 0; j < padding; j++) printf(" ");
            printf("%s\n", menu[i]);
        }
        printf("                                       -----------------------------------------------\n");
        if (strlen(globalNotice) > 0) {
            for (int j = 0; j < padding; j++) printf(" ");
            printf("NOTICE: %s\n", globalNotice);
            printf("                                   -----------------------------------------------\n");
        }
        for (int j = 0; j < padding; j++) printf(" ");
        printf("Enter your choice: ");
        scanf("%d", &choice); getchar();
        if (choice == 1) {
            viewProfile(empId);//ln 349
        } else if (choice == 2) {
            makeApplication(empId);//ln 389
        } else if (choice == 3) {
            viewApplicationStatus(empId);//426
        } else if (choice == 4) {
            break;
        } else {
            for (int j = 0; j < padding; j++) printf(" ");
            printf("Invalid choice. Press Enter to continue...");
            getchar();
        }
    }
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    int choice;
    while (1) {
        system("cls");
        setTextColorLightBlue();
        printCenteredLogo();
        int screenWidth = getConsoleWidth();//65
        char menu[5][40] = {
            "1   Admin Login",
            "2   Employee Login",
            "3   Employee Registration",
            "4   Exit"
        };
        int menuLen = 4;
        int maxLen = 0;
        for (int i = 0; i < menuLen; i++) {
            int len = strlen(menu[i]);
            if (len > maxLen) maxLen = len;
        }
        int padding = (screenWidth - maxLen) / 2;
        printf("\n");
        printf("                                     === Welcome to Employee Management System ===\n");
        printf("\n");
        for (int i = 0; i < menuLen; i++) {
            for (int j = 0; j < padding; j++) printf(" ");
            printf("%s\n", menu[i]);
        }
        printf("\n");
        for (int j = 0; j < padding; j++) printf(" ");
        printf("Enter your choice: ");
        scanf("%d", &choice); getchar();

        if (choice == 1) {
            char password[50];
            system("cls");
            setTextColorLightBlue();
            printCenteredLogo();
            printCenteredLine("Admin Login");
            printf("\nEnter password: ");
            getPassword(password, sizeof(password));//848
            getchar();
            if (strcmp(password, "admin1234") == 0) {
                adminMenu();//615
            } else {
                printCenteredLine("Wrong password");
                printf("\nPress Enter to continue...");
                getchar();
            }
        } else if (choice == 2) {
            int empId;
            printf("Enter your Employee ID: ");
            scanf("%d", &empId); getchar();
            if (employeeExists(empId)) {//ln 115
                logEmployeeLogin(empId);//871
                employeeMenu(empId);//881
            } else {
                printCenteredLine("Employee not found, contact admin for support.");
                printf("\nPress Enter to continue...");
                getchar();
            }
        } else if (choice == 3) {
            employeeRegistration();//ln 213
        } else if (choice == 4) {
            printCenteredLine("Goodbye!");
            break;
        } else {
            printCenteredLine("Invalid choice. Try again.");
            printf("\nPress Enter to continue...");
            getchar();
        }
    }
    return 0;
}
