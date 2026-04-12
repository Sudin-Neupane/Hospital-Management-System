#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define DFILE "doctors.txt"
#define NFILE "nurses.txt"
#define PFILE "patients.txt"
#define BFILE "beds.txt"
#define MFILE "machines.txt"

struct doctor {
    int id;
    char name[50];
    int age;
    char gender;
    char specialization[50];
    char dept[40];
    char qualification[50];
    char phone[15];
    char schedule[30];
    double salary;
};

struct nurse {
    int id;
    char name[50];
    int age;
    char gender;
    char dept[40];
    char shift[15];
    int assignedward;
    char phone[15];
    double salary;
    char joindate[12];
};

struct patient {
    int id;
    char name[50];
    int age;
    char gender;
    char blood[5];
    char disease[80];
    int bedid;
    int doctorid;
    char phone[15];
    char admitdate[12];
    char status[15];
};

struct bed {
    int id;
    int wardno;
    char type[20];
    char status[12];
    int patientid;
};

struct machine {
    int id;
    char name[60];
    char dept[40];
    char condition[15];
    char lastservice[12];
    int assignedto;
};

int did = 1001;
int nid = 2001;
int pid = 3001;
int bid = 4001;
int mid = 5001;

void pause() {
    printf("\nPress enter to continue...");
    getchar();
}

void stripcr(char *s) {
    int l = strlen(s);
    if (l > 0 && s[l-1] == '\n') s[l-1] = '\0';
}

int goodname(char *s) {
    if (strlen(s) == 0) return 0;
    for (int i = 0; s[i]; i++)
        if (!isalpha(s[i]) && s[i] != ' ' && s[i] != '.') return 0;
    return 1;
}

int goodphone(char *s) {
    if (strlen(s) < 7) return 0;
    for (int i = 0; s[i]; i++)
        if (!isdigit(s[i]) && s[i] != '+' && s[i] != '-') return 0;
    return 1;
}

int strmatch(const char *a, const char *b) {
    char la[200], lb[200];
    int i;
    for (i = 0; a[i]; i++) la[i] = tolower(a[i]);
    la[i] = '\0';
    for (i = 0; b[i]; i++) lb[i] = tolower(b[i]);
    lb[i] = '\0';
    return strstr(la, lb) != NULL;
}

void loadids() {
    FILE *fp;
    char line[300];

    fp = fopen(DFILE, "r");
    if (fp) {
        struct doctor d;
        fgets(line, sizeof(line), fp);
        while (fscanf(fp, "%d|%49[^|]|%d|%c|%49[^|]|%39[^|]|%49[^|]|%14[^|]|%29[^|]|%lf\n",
            &d.id,d.name,&d.age,&d.gender,d.specialization,d.dept,d.qualification,d.phone,d.schedule,&d.salary)==10)
            if (d.id >= did) did = d.id + 1;
        fclose(fp);
    }

    fp = fopen(NFILE, "r");
    if (fp) {
        struct nurse n;
        fgets(line, sizeof(line), fp);
        while (fscanf(fp, "%d|%49[^|]|%d|%c|%39[^|]|%14[^|]|%d|%14[^|]|%lf|%11[^\n]",
            &n.id,n.name,&n.age,&n.gender,n.dept,n.shift,&n.assignedward,n.phone,&n.salary,n.joindate)==10)
            if (n.id >= nid) nid = n.id + 1;
        fclose(fp);
    }

    fp = fopen(PFILE, "r");
    if (fp) {
        struct patient p;
        fgets(line, sizeof(line), fp);
        while (fscanf(fp, "%d|%49[^|]|%d|%c|%4[^|]|%79[^|]|%d|%d|%14[^|]|%11[^|]|%14[^\n]",
            &p.id,p.name,&p.age,&p.gender,p.blood,p.disease,&p.bedid,&p.doctorid,p.phone,p.admitdate,p.status)==11)
            if (p.id >= pid) pid = p.id + 1;
        fclose(fp);
    }

    fp = fopen(BFILE, "r");
    if (fp) {
        struct bed b;
        fgets(line, sizeof(line), fp);
        while (fscanf(fp, "%d|%d|%19[^|]|%11[^|]|%d\n",
            &b.id,&b.wardno,b.type,b.status,&b.patientid)==5)
            if (b.id >= bid) bid = b.id + 1;
        fclose(fp);
    }

    fp = fopen(MFILE, "r");
    if (fp) {
        struct machine m;
        fgets(line, sizeof(line), fp);
        while (fscanf(fp, "%d|%59[^|]|%39[^|]|%14[^|]|%11[^|]|%d\n",
            &m.id,m.name,m.dept,m.condition,m.lastservice,&m.assignedto)==6)
            if (m.id >= mid) mid = m.id + 1;
        fclose(fp);
    }
}
void adddoctor() {
    FILE *fp = fopen(DFILE, "a+");
    if (!fp) { printf("Cannot open file.\n"); return; }
    fseek(fp, 0, SEEK_END);
    if (ftell(fp) == 0)
        fprintf(fp, "ID|Name|Age|Gender|Specialization|Dept|Qualification|Phone|Schedule|Salary\n");

    struct doctor d;
    char more = 'y';
    while (more == 'y' || more == 'Y') {
        printf("\n--- Add Doctor ---\n");

        printf("Name: ");
        fgets(d.name, 50, stdin); stripcr(d.name);
        if (!goodname(d.name)) { printf("Invalid name.\n"); continue; }

        printf("Age: ");
        if (scanf("%d", &d.age) != 1 || d.age < 22 || d.age > 80) {
            printf("Invalid age.\n"); while (getchar()!='\n'); continue;
        }
        while (getchar()!='\n');

        printf("Gender (M/F): ");
        scanf(" %c", &d.gender); d.gender = toupper(d.gender);
        while (getchar()!='\n');
        if (d.gender != 'M' && d.gender != 'F') { printf("Invalid.\n"); continue; }

        printf("Specialization: ");
        fgets(d.specialization, 50, stdin); stripcr(d.specialization);

        printf("Department: ");
        fgets(d.dept, 40, stdin); stripcr(d.dept);

        printf("Qualification (eg MBBS, MD): ");
        fgets(d.qualification, 50, stdin); stripcr(d.qualification);

        printf("Phone: ");
        fgets(d.phone, 15, stdin); stripcr(d.phone);
        if (!goodphone(d.phone)) { printf("Invalid phone.\n"); continue; }

        printf("Schedule (eg Mon-Fri 9am-5pm): ");
        fgets(d.schedule, 30, stdin); stripcr(d.schedule);

        printf("Salary: ");
        if (scanf("%lf", &d.salary) != 1 || d.salary < 0) {
            printf("Invalid.\n"); while (getchar()!='\n'); continue;
        }
        while (getchar()!='\n');

        d.id = did++;
        fprintf(fp, "%d|%s|%d|%c|%s|%s|%s|%s|%s|%.2f\n",
            d.id,d.name,d.age,d.gender,d.specialization,d.dept,d.qualification,d.phone,d.schedule,d.salary);
        printf("Doctor added. ID: D-%d\n", d.id);

        printf("Add another? (y/n): ");
        scanf(" %c", &more); while (getchar()!='\n');
    }
    fclose(fp);
}
