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

void showdoctors() {
    FILE *fp = fopen(DFILE, "r");
    if (!fp) { printf("No doctor records.\n"); pause(); return; }

    struct doctor d;
    char line[300];
    int n = 0;
    fgets(line, sizeof(line), fp);

    printf("\n%-6s %-22s %-4s %-2s %-20s %-18s %-14s %-14s\n",
        "ID","Name","Age","G","Specialization","Department","Qualification","Phone");
    printf("--------------------------------------------------------------------------------------------------\n");

    while (fscanf(fp, "%d|%49[^|]|%d|%c|%49[^|]|%39[^|]|%49[^|]|%14[^|]|%29[^|]|%lf\n",
        &d.id,d.name,&d.age,&d.gender,d.specialization,d.dept,d.qualification,d.phone,d.schedule,&d.salary)==10) {
        printf("%-6d %-22s %-4d %-2c %-20s %-18s %-14s %-14s\n",
            d.id,d.name,d.age,d.gender,d.specialization,d.dept,d.qualification,d.phone);
        n++;
    }
    if (n == 0) printf("No records found.\n");
    else printf("\nTotal doctors: %d\n", n);
    fclose(fp);
    pause();
}

void searchdoctor() {
    FILE *fp = fopen(DFILE, "r");
    if (!fp) { printf("No data.\n"); pause(); return; }

    char kw[50];
    printf("Search (name/specialization/dept): ");
    fgets(kw, 50, stdin); stripcr(kw);

    struct doctor d;
    char line[300];
    int n = 0;
    fgets(line, sizeof(line), fp);

    printf("\n%-6s %-22s %-20s %-18s %-14s %-28s\n",
        "ID","Name","Specialization","Department","Phone","Schedule");
    printf("--------------------------------------------------------------------------------------------------\n");

    while (fscanf(fp, "%d|%49[^|]|%d|%c|%49[^|]|%39[^|]|%49[^|]|%14[^|]|%29[^|]|%lf\n",
        &d.id,d.name,&d.age,&d.gender,d.specialization,d.dept,d.qualification,d.phone,d.schedule,&d.salary)==10) {
        if (strmatch(d.name,kw)||strmatch(d.specialization,kw)||strmatch(d.dept,kw)) {
            printf("%-6d %-22s %-20s %-18s %-14s %-28s\n",
                d.id,d.name,d.specialization,d.dept,d.phone,d.schedule);
            n++;
        }
    }
    if (n == 0) printf("No match.\n");
    else printf("\n%d found.\n", n);
    fclose(fp);
    pause();
}

void deletedoctor() {
    int target;
    printf("Enter doctor ID to delete: ");
    if (scanf("%d",&target)!=1){while(getchar()!='\n');return;}
    while(getchar()!='\n');

    FILE *fp = fopen(DFILE,"r");
    FILE *tmp = fopen("dtmp.txt","w");
    if (!fp||!tmp){printf("File error.\n");return;}

    struct doctor d;
    char line[300];
    int found = 0;
    fgets(line,sizeof(line),fp);
    fprintf(tmp,"%s",line);

    while (fscanf(fp, "%d|%49[^|]|%d|%c|%49[^|]|%39[^|]|%49[^|]|%14[^|]|%29[^|]|%lf\n",
        &d.id,d.name,&d.age,&d.gender,d.specialization,d.dept,d.qualification,d.phone,d.schedule,&d.salary)==10) {
        if (d.id==target){found=1;continue;}
        fprintf(tmp,"%d|%s|%d|%c|%s|%s|%s|%s|%s|%.2f\n",
            d.id,d.name,d.age,d.gender,d.specialization,d.dept,d.qualification,d.phone,d.schedule,d.salary);
    }
    fclose(fp); fclose(tmp);
    remove(DFILE); rename("dtmp.txt",DFILE);
    if (found) printf("Doctor deleted.\n"); else printf("ID not found.\n");
}
void addnurse() {
    FILE *fp = fopen(NFILE, "a+");
    if (!fp) { printf("Cannot open file.\n"); return; }
    fseek(fp, 0, SEEK_END);
    if (ftell(fp) == 0)
        fprintf(fp, "ID|Name|Age|Gender|Dept|Shift|Ward|Phone|Salary|JoinDate\n");

    struct nurse n;
    char more = 'y';
    while (more == 'y' || more == 'Y') {
        printf("\n--- Add Nurse ---\n");

        printf("Name: ");
        fgets(n.name, 50, stdin); stripcr(n.name);
        if (!goodname(n.name)) { printf("Invalid name.\n"); continue; }

        printf("Age: ");
        if (scanf("%d",&n.age)!=1||n.age<18||n.age>70){
            printf("Invalid.\n"); while(getchar()!='\n'); continue;
        }
        while(getchar()!='\n');

        printf("Gender (M/F): ");
        scanf(" %c",&n.gender); n.gender=toupper(n.gender);
        while(getchar()!='\n');
        if (n.gender!='M'&&n.gender!='F'){printf("Invalid.\n");continue;}

        printf("Department: ");
        fgets(n.dept,40,stdin); stripcr(n.dept);

        printf("Shift (Morning/Evening/Night): ");
        fgets(n.shift,15,stdin); stripcr(n.shift);

        printf("Assigned ward no: ");
        if (scanf("%d",&n.assignedward)!=1){while(getchar()!='\n');continue;}
        while(getchar()!='\n');

        printf("Phone: ");
        fgets(n.phone,15,stdin); stripcr(n.phone);
        if (!goodphone(n.phone)){printf("Invalid phone.\n");continue;}

        printf("Salary: ");
        if (scanf("%lf",&n.salary)!=1||n.salary<0){
            printf("Invalid.\n"); while(getchar()!='\n'); continue;
        }
        while(getchar()!='\n');

        printf("Join date (DD/MM/YYYY): ");
        fgets(n.joindate,12,stdin); stripcr(n.joindate);

        n.id = nid++;
        fprintf(fp,"%d|%s|%d|%c|%s|%s|%d|%s|%.2f|%s\n",
            n.id,n.name,n.age,n.gender,n.dept,n.shift,n.assignedward,n.phone,n.salary,n.joindate);
        printf("Nurse added. ID: N-%d\n",n.id);

        printf("Add another? (y/n): ");
        scanf(" %c",&more); while(getchar()!='\n');
    }
    fclose(fp);
}

void shownurses() {
    FILE *fp = fopen(NFILE,"r");
    if (!fp){printf("No nurse records.\n");pause();return;}

    struct nurse n;
    char line[300];
    int cnt = 0;
    fgets(line,sizeof(line),fp);

    printf("\n%-6s %-22s %-4s %-2s %-18s %-10s %-6s %-14s %-10s\n",
        "ID","Name","Age","G","Department","Shift","Ward","Phone","Salary");
    printf("--------------------------------------------------------------------------------------------\n");

    while (fscanf(fp,"%d|%49[^|]|%d|%c|%39[^|]|%14[^|]|%d|%14[^|]|%lf|%11[^\n]",
        &n.id,n.name,&n.age,&n.gender,n.dept,n.shift,&n.assignedward,n.phone,&n.salary,n.joindate)==10){
        printf("%-6d %-22s %-4d %-2c %-18s %-10s %-6d %-14s %-10.2f\n",
            n.id,n.name,n.age,n.gender,n.dept,n.shift,n.assignedward,n.phone,n.salary);
        cnt++;
    }
    if (cnt==0) printf("No records.\n");
    else printf("\nTotal nurses: %d\n",cnt);
    fclose(fp);
    pause();
}

void searchnurse() {
    FILE *fp = fopen(NFILE,"r");
    if (!fp){printf("No data.\n");pause();return;}

    char kw[50];
    printf("Search (name/dept/shift): ");
    fgets(kw,50,stdin); stripcr(kw);

    struct nurse n;
    char line[300];
    int cnt = 0;
    fgets(line,sizeof(line),fp);

    printf("\n%-6s %-22s %-18s %-10s %-6s %-14s\n","ID","Name","Department","Shift","Ward","Phone");
    printf("----------------------------------------------------------------------\n");

    while (fscanf(fp,"%d|%49[^|]|%d|%c|%39[^|]|%14[^|]|%d|%14[^|]|%lf|%11[^\n]",
        &n.id,n.name,&n.age,&n.gender,n.dept,n.shift,&n.assignedward,n.phone,&n.salary,n.joindate)==10){
        if (strmatch(n.name,kw)||strmatch(n.dept,kw)||strmatch(n.shift,kw)){
            printf("%-6d %-22s %-18s %-10s %-6d %-14s\n",
                n.id,n.name,n.dept,n.shift,n.assignedward,n.phone);
            cnt++;
        }
    }
    if (cnt==0) printf("No match.\n");
    else printf("\n%d found.\n",cnt);
    fclose(fp);
    pause();
}

void deletenurse() {
    int target;
    printf("Enter nurse ID to delete: ");
    if (scanf("%d",&target)!=1){while(getchar()!='\n');return;}
    while(getchar()!='\n');

    FILE *fp=fopen(NFILE,"r");
    FILE *tmp=fopen("ntmp.txt","w");
    if (!fp||!tmp){printf("File error.\n");return;}

    struct nurse n;
    char line[300];
    int found=0;
    fgets(line,sizeof(line),fp);
    fprintf(tmp,"%s",line);

    while (fscanf(fp,"%d|%49[^|]|%d|%c|%39[^|]|%14[^|]|%d|%14[^|]|%lf|%11[^\n]",
        &n.id,n.name,&n.age,&n.gender,n.dept,n.shift,&n.assignedward,n.phone,&n.salary,n.joindate)==10){
        if (n.id==target){found=1;continue;}
        fprintf(tmp,"%d|%s|%d|%c|%s|%s|%d|%s|%.2f|%s\n",
            n.id,n.name,n.age,n.gender,n.dept,n.shift,n.assignedward,n.phone,n.salary,n.joindate);
    }
    fclose(fp); fclose(tmp);
    remove(NFILE); rename("ntmp.txt",NFILE);
    if (found) printf("Nurse deleted.\n"); else printf("ID not found.\n");
}

void addpatient() {
    FILE *fp = fopen(PFILE,"a+");
    if (!fp){printf("Cannot open file.\n");return;}
    fseek(fp,0,SEEK_END);
    if (ftell(fp)==0)
        fprintf(fp,"ID|Name|Age|Gender|Blood|Disease|BedID|DoctorID|Phone|AdmitDate|Status\n");

    struct patient p;
    char more='y';
    while (more=='y'||more=='Y') {
        printf("\n--- Admit Patient ---\n");

        printf("Name: ");
        fgets(p.name,50,stdin); stripcr(p.name);
        if (!goodname(p.name)){printf("Invalid name.\n");continue;}

        printf("Age: ");
        if (scanf("%d",&p.age)!=1||p.age<0||p.age>120){
            printf("Invalid.\n"); while(getchar()!='\n'); continue;
        }
        while(getchar()!='\n');

        printf("Gender (M/F): ");
        scanf(" %c",&p.gender); p.gender=toupper(p.gender);
        while(getchar()!='\n');
        if (p.gender!='M'&&p.gender!='F'){printf("Invalid.\n");continue;}

        printf("Blood group: ");
        fgets(p.blood,5,stdin); stripcr(p.blood);

        printf("Disease/Diagnosis: ");
        fgets(p.disease,80,stdin); stripcr(p.disease);

        printf("Assigned bed ID (0 if none): ");
        if (scanf("%d",&p.bedid)!=1){while(getchar()!='\n');p.bedid=0;}
        while(getchar()!='\n');

        printf("Assigned doctor ID (0 if none): ");
        if (scanf("%d",&p.doctorid)!=1){while(getchar()!='\n');p.doctorid=0;}
        while(getchar()!='\n');

        printf("Phone: ");
        fgets(p.phone,15,stdin); stripcr(p.phone);
        if (!goodphone(p.phone)){printf("Invalid phone.\n");continue;}

        printf("Admit date (DD/MM/YYYY): ");
        fgets(p.admitdate,12,stdin); stripcr(p.admitdate);

        printf("Status (Admitted/Discharged/Critical): ");
        fgets(p.status,15,stdin); stripcr(p.status);

        p.id = pid++;
        fprintf(fp,"%d|%s|%d|%c|%s|%s|%d|%d|%s|%s|%s\n",
            p.id,p.name,p.age,p.gender,p.blood,p.disease,p.bedid,p.doctorid,p.phone,p.admitdate,p.status);
        printf("Patient admitted. ID: P-%d\n",p.id);

        printf("Add another? (y/n): ");
        scanf(" %c",&more); while(getchar()!='\n');
    }
    fclose(fp);
}

void showpatients() {
    FILE *fp=fopen(PFILE,"r");
    if (!fp){printf("No patient records.\n");pause();return;}

    struct patient p;
    char line[300];
    int n=0;
    fgets(line,sizeof(line),fp);

    printf("\n%-6s %-22s %-4s %-2s %-5s %-22s %-6s %-6s %-12s %-12s\n",
        "ID","Name","Age","G","Blood","Disease","BedID","DocID","AdmitDate","Status");
    printf("------------------------------------------------------------------------------------------------------\n");

    while (fscanf(fp,"%d|%49[^|]|%d|%c|%4[^|]|%79[^|]|%d|%d|%14[^|]|%11[^|]|%14[^\n]",
        &p.id,p.name,&p.age,&p.gender,p.blood,p.disease,&p.bedid,&p.doctorid,p.phone,p.admitdate,p.status)==11){
        printf("%-6d %-22s %-4d %-2c %-5s %-22s %-6d %-6d %-12s %-12s\n",
            p.id,p.name,p.age,p.gender,p.blood,p.disease,p.bedid,p.doctorid,p.admitdate,p.status);
        n++;
    }
    if (n==0) printf("No records.\n");
    else printf("\nTotal patients: %d\n",n);
    fclose(fp);
    pause();
}

void searchpatient() {
    FILE *fp=fopen(PFILE,"r");
    if (!fp){printf("No data.\n");pause();return;}

    char kw[50];
    printf("Search (name/disease/status): ");
    fgets(kw,50,stdin); stripcr(kw);

    struct patient p;
    char line[300];
    int n=0;
    fgets(line,sizeof(line),fp);

    printf("\n%-6s %-22s %-4s %-5s %-22s %-6s %-12s %-12s\n",
        "ID","Name","Age","Blood","Disease","BedID","AdmitDate","Status");
    printf("------------------------------------------------------------------------------------\n");

    while (fscanf(fp,"%d|%49[^|]|%d|%c|%4[^|]|%79[^|]|%d|%d|%14[^|]|%11[^|]|%14[^\n]",
        &p.id,p.name,&p.age,&p.gender,p.blood,p.disease,&p.bedid,&p.doctorid,p.phone,p.admitdate,p.status)==11){
        if (strmatch(p.name,kw)||strmatch(p.disease,kw)||strmatch(p.status,kw)){
            printf("%-6d %-22s %-4d %-5s %-22s %-6d %-12s %-12s\n",
                p.id,p.name,p.age,p.blood,p.disease,p.bedid,p.admitdate,p.status);
            n++;
        }
    }
    if (n==0) printf("No match.\n");
    else printf("\n%d found.\n",n);
    fclose(fp);
    pause();
}

void updatepatient() {
    int target;
    printf("Enter patient ID to update: ");
    if (scanf("%d",&target)!=1){while(getchar()!='\n');return;}
    while(getchar()!='\n');

    FILE *fp=fopen(PFILE,"r");
    FILE *tmp=fopen("ptmp.txt","w");
    if (!fp||!tmp){printf("File error.\n");return;}

    struct patient p;
    char line[300];
    int found=0;
    fgets(line,sizeof(line),fp);
    fprintf(tmp,"%s",line);

    while (fscanf(fp,"%d|%49[^|]|%d|%c|%4[^|]|%79[^|]|%d|%d|%14[^|]|%11[^|]|%14[^\n]",
        &p.id,p.name,&p.age,&p.gender,p.blood,p.disease,&p.bedid,&p.doctorid,p.phone,p.admitdate,p.status)==11){
        if (p.id==target){
            found=1;
            printf("Patient: %s | Status: %s | Bed: %d | Doctor: %d\n",
                p.name,p.status,p.bedid,p.doctorid);
            char buf[80];
            printf("New status [%s]: ",p.status);
            fgets(buf,15,stdin); stripcr(buf);
            if (strlen(buf)>0) strcpy(p.status,buf);
            printf("New bed ID [%d]: ",p.bedid);
            fgets(buf,10,stdin); stripcr(buf);
            if (strlen(buf)>0) p.bedid=atoi(buf);
            printf("New doctor ID [%d]: ",p.doctorid);
            fgets(buf,10,stdin); stripcr(buf);
            if (strlen(buf)>0) p.doctorid=atoi(buf);
            printf("New disease/diagnosis [%s]: ",p.disease);
            fgets(buf,80,stdin); stripcr(buf);
            if (strlen(buf)>0) strcpy(p.disease,buf);
        }
        fprintf(tmp,"%d|%s|%d|%c|%s|%s|%d|%d|%s|%s|%s\n",
            p.id,p.name,p.age,p.gender,p.blood,p.disease,p.bedid,p.doctorid,p.phone,p.admitdate,p.status);
    }
    fclose(fp); fclose(tmp);
    remove(PFILE); rename("ptmp.txt",PFILE);
    if (found) printf("Patient updated.\n"); else printf("ID not found.\n");
}

void deletepatient() {
    int target;
    printf("Enter patient ID to delete: ");
    if (scanf("%d",&target)!=1){while(getchar()!='\n');return;}
    while(getchar()!='\n');

    FILE *fp=fopen(PFILE,"r");
    FILE *tmp=fopen("ptmp.txt","w");
    if (!fp||!tmp){printf("File error.\n");return;}

    struct patient p;
    char line[300];
    int found=0;
    fgets(line,sizeof(line),fp);
    fprintf(tmp,"%s",line);

    while (fscanf(fp,"%d|%49[^|]|%d|%c|%4[^|]|%79[^|]|%d|%d|%14[^|]|%11[^|]|%14[^\n]",
        &p.id,p.name,&p.age,&p.gender,p.blood,p.disease,&p.bedid,&p.doctorid,p.phone,p.admitdate,p.status)==11){
        if (p.id==target){found=1;continue;}
        fprintf(tmp,"%d|%s|%d|%c|%s|%s|%d|%d|%s|%s|%s\n",
            p.id,p.name,p.age,p.gender,p.blood,p.disease,p.bedid,p.doctorid,p.phone,p.admitdate,p.status);
    }
    fclose(fp); fclose(tmp);
    remove(PFILE); rename("ptmp.txt",PFILE);
    if (found) printf("Patient deleted.\n"); else printf("ID not found.\n");
}

void addbed() {
    FILE *fp=fopen(BFILE,"a+");
    if (!fp){printf("Cannot open file.\n");return;}
    fseek(fp,0,SEEK_END);
    if (ftell(fp)==0)
        fprintf(fp,"ID|Ward|Type|Status|PatientID\n");

    struct bed b;
    char more='y';
    while (more=='y'||more=='Y') {
        printf("\n--- Add Bed ---\n");

        printf("Ward number: ");
        if (scanf("%d",&b.wardno)!=1){while(getchar()!='\n');continue;}
        while(getchar()!='\n');

        printf("Bed type (General/ICU/Private/Semi-Private): ");
        fgets(b.type,20,stdin); stripcr(b.type);

        printf("Status (Available/Occupied): ");
        fgets(b.status,12,stdin); stripcr(b.status);

        printf("Patient ID if occupied (0 if none): ");
        if (scanf("%d",&b.patientid)!=1) b.patientid=0;
        while(getchar()!='\n');

        b.id=bid++;
        fprintf(fp,"%d|%d|%s|%s|%d\n",b.id,b.wardno,b.type,b.status,b.patientid);
        printf("Bed added. ID: B-%d\n",b.id);

        printf("Add another? (y/n): ");
        scanf(" %c",&more); while(getchar()!='\n');
    }
    fclose(fp);
}

void showbeds() {
    FILE *fp=fopen(BFILE,"r");
    if (!fp){printf("No bed records.\n");pause();return;}

    struct bed b;
    char line[300];
    int total=0,avail=0,occ=0;
    fgets(line,sizeof(line),fp);

    printf("\n%-6s %-6s %-18s %-12s %-10s\n","ID","Ward","Type","Status","PatientID");
    printf("--------------------------------------------------\n");

    while (fscanf(fp,"%d|%d|%19[^|]|%11[^|]|%d\n",
        &b.id,&b.wardno,b.type,b.status,&b.patientid)==5){
        printf("%-6d %-6d %-18s %-12s %-10d\n",
            b.id,b.wardno,b.type,b.status,b.patientid);
        total++;
        if (strmatch(b.status,"available")) avail++;
        else occ++;
    }
    if (total==0) printf("No bed records.\n");
    else {
        printf("----------------------------------------------------\n");
        printf("Total: %d  |  Available: %d  |  Occupied: %d\n",total,avail,occ);
    }
    fclose(fp);
    pause();
}

void updatebed() {
    int target;
    printf("Enter bed ID to update: ");
    if (scanf("%d",&target)!=1){while(getchar()!='\n');return;}
    while(getchar()!='\n');

    FILE *fp=fopen(BFILE,"r");
    FILE *tmp=fopen("btmp.txt","w");
    if (!fp||!tmp){printf("File error.\n");return;}

    struct bed b;
    char line[300];
    int found=0;
    fgets(line,sizeof(line),fp);
    fprintf(tmp,"%s",line);

    while (fscanf(fp,"%d|%d|%19[^|]|%11[^|]|%d\n",
        &b.id,&b.wardno,b.type,b.status,&b.patientid)==5){
        if (b.id==target){
            found=1;
            printf("Bed %d | Ward %d | %s | Status: %s | Patient: %d\n",
                b.id,b.wardno,b.type,b.status,b.patientid);
            char buf[20];
            printf("New status [%s]: ",b.status);
            fgets(buf,12,stdin); stripcr(buf);
            if (strlen(buf)>0) strcpy(b.status,buf);
            printf("Patient ID (0 if none) [%d]: ",b.patientid);
            fgets(buf,10,stdin); stripcr(buf);
            if (strlen(buf)>0) b.patientid=atoi(buf);
        }
        fprintf(tmp,"%d|%d|%s|%s|%d\n",b.id,b.wardno,b.type,b.status,b.patientid);
    }
    fclose(fp); fclose(tmp);
    remove(BFILE); rename("btmp.txt",BFILE);
    if (found) printf("Bed updated.\n"); else printf("ID not found.\n");
}
