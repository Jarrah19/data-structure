#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME_LEN 17
#define GENDER_LEN 5

FILE *readfile, *writefile;

int i = 0, n = 0, c = 1;

typedef struct familyMemberInfo {

    int generation, birth_year, birth_month, birth_day;
    char name[NAME_LEN], gender[GENDER_LEN];

} FamilyMemberInfo;

typedef struct familyMember *FamilyMemberAddr;

typedef struct familyMember {

    FamilyMemberInfo info;
    FamilyMemberAddr older_sibling;
    FamilyMemberAddr parent;
    FamilyMemberAddr sibling;
    FamilyMemberAddr child;

} FamilyMember;

FamilyMemberAddr membersArray[25];

FamilyMemberInfo getInfoFromKeyboard() {

    FamilyMemberInfo temp;

    temp.generation = 1;
    printf("请输入成员姓名：");
    scanf("%s", temp.name);

    do {
        printf("请输入成员性别：");
        scanf("%s", temp.gender);
        if (!(strcmp(temp.gender, "男") == 0 || strcmp(temp.gender, "女") == 0))
            printf("输入的值有误，请输入【男】或者【女】\n");

    } while (!(strcmp(temp.gender, "男") == 0 || strcmp(temp.gender, "女") == 0));

    do {
        printf("您输入成员出生年份：");
        scanf("%d", &temp.birth_year);
        while (getchar() != '\n');
        if (temp.birth_year < 1873 || temp.birth_year > 2020)
            printf("输入的年份有误，请输入 1873 至 2020 的整数\n");

    } while (temp.birth_year < 1873 || temp.birth_year > 2020);

    do {
        printf("您输入成员出生月份：");
        scanf("%d", &temp.birth_month);
        while (getchar() != '\n');
        if (temp.birth_month < 1 || temp.birth_month > 12) printf("输入的月份有误，请输入 1 至 12 的整数\n");

    } while (temp.birth_month < 1 || temp.birth_month > 12);

    do {
        printf("您输入成员出生日期：");
        scanf("%d", &temp.birth_day);
        while (getchar() != '\n');
        if (temp.generation < 0 || temp.generation > 9)
            printf("输入的值有误，请输入 1 至 9 的整数\n");
    } while (temp.generation < 0 || temp.generation > 9);

    return temp;
}

FamilyMemberAddr memberCreate() {

    FamilyMemberAddr member = (FamilyMemberAddr) malloc(sizeof(FamilyMember));

    fscanf(readfile, "%d%s%s%d%d%d", &member->info.generation, member->info.name, member->info.gender,
           &member->info.birth_year, &member->info.birth_month, &member->info.birth_day);
    member->parent = NULL;
    member->sibling = NULL;
    member->child = NULL;
    member->older_sibling = NULL;

    return member;
}

void fileAddChild(FamilyMemberAddr memberAddr) {

    memberAddr->child = memberCreate();
    memberAddr->child->parent = memberAddr;
}

void fileAddSibling(FamilyMemberAddr memberAddr) {

    memberAddr->sibling = memberCreate();
    memberAddr->sibling->parent = (memberAddr->parent);
    memberAddr->sibling->older_sibling = memberAddr;
}

void printSingleMember(FamilyMemberAddr memberAddr) {
    FamilyMemberAddr temp = memberAddr;
    printf("姓名：%-8s　性别：%s　出生日期：%d年%2d月%2d日　", memberAddr->info.name, memberAddr->info.gender,
           memberAddr->info.birth_year, memberAddr->info.birth_month, memberAddr->info.birth_day);
    if (memberAddr->child != NULL) {
        printf("子女：%s", temp->child->info.name);
        temp = temp->child;
        while (temp->sibling != NULL) {
            printf("，%s", temp->sibling->info.name);
            temp = temp->sibling;
        }
    }
    putchar('\n');
}

void getArray(FamilyMemberAddr memberAddr) {

    if (memberAddr == NULL)
        return;

    membersArray[i++] = memberAddr;
    getArray(memberAddr->child);
    getArray(memberAddr->sibling);

    n = i;
}

void swap(FamilyMemberAddr *a, FamilyMemberAddr *b) {

    FamilyMemberAddr temp = *a;
    *a = *b;
    *b = temp;
}

void sortByGeneration() {

    for (i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - 1 - i; j++) {

            if ((membersArray[j]->info.generation) > (membersArray[j + 1]->info.generation))
                swap(&membersArray[j], &membersArray[j + 1]);
        }
    }

    int temp = 0;
    for (i = 0; i < n; i++) {
        if (temp != membersArray[i]->info.generation) {
            printf("第%d代：\n", membersArray[i]->info.generation);
            temp = membersArray[i]->info.generation;
        }
        printSingleMember(membersArray[i]);
    }

    i = 0;
    putchar('\n');
}

void sortByBirthday() {

    for (i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - 1 - i; j++) {

            if ((10000 * (membersArray[j]->info.birth_year) + 100 * (membersArray[j]->info.birth_month) + (membersArray[j]->info.birth_year)) >
                (10000 * (membersArray[j + 1]->info.birth_year) + 100 * (membersArray[j + 1]->info.birth_month) + (membersArray[j + 1]->info.birth_year)))
                swap(&membersArray[j], &membersArray[j + 1]);
        }
    }

    int temp = 0;
    for (i = 0; i < n; i++) {
        if (temp != membersArray[i]->info.generation) {
            printf("第%d代：\n", membersArray[i]->info.generation);
            temp = membersArray[i]->info.generation;
        }
        printSingleMember(membersArray[i]);
    }

    i = 0;
    putchar('\n');
}

void searchByName (FamilyMemberAddr memberAddr, char name[]) {

    if (memberAddr == NULL)
        return;

    if (strcmp(memberAddr->info.name, name) == 0) {
        printSingleMember(memberAddr);
        c = 0;
    }
    searchByName(memberAddr->child, name);
    searchByName(memberAddr->sibling, name);
}

void manuAddChild (FamilyMemberAddr memberAddr, char name[]) {

    if (memberAddr == NULL) {
        return;
    }
    if (!strcmp(memberAddr->info.name, name)) {
        printf("查找成功，开始录入新成员信息\n");
        c = 0;
        if (memberAddr->child == NULL) {
            FamilyMemberAddr member = (FamilyMemberAddr) malloc(sizeof(FamilyMember));
            member->info = getInfoFromKeyboard();
            member->info.generation = memberAddr->info.generation + 1;
            memberAddr->child = member;
            member->parent = memberAddr;
            member->child = NULL;
            member->sibling = NULL;
            member->older_sibling = NULL;
        }
        else {
            FamilyMemberAddr member = (FamilyMemberAddr) malloc(sizeof(FamilyMember));
            member->info = getInfoFromKeyboard();
            member->info.generation = memberAddr->info.generation + 1;
            member->sibling = memberAddr->child->sibling;
            memberAddr->child->sibling = member;
            member->parent = memberAddr;
            member->child = NULL;
            member->older_sibling = member->parent->child;
            if(member->sibling != NULL) {
                member->sibling->older_sibling = member;
            }
        }
    }
    manuAddChild(memberAddr->child, name);
    manuAddChild(memberAddr->sibling, name);
}

void modifyMember(FamilyMemberAddr memberAddr, char name[]) {
    if (memberAddr == NULL) {
        return;
    }
    if (!strcmp(memberAddr->info.name, name)) {
        printf("查找成功，开始修改成员信息\n");
        c = 0;
        memberAddr->info = getInfoFromKeyboard();
        memberAddr->info.generation = memberAddr->parent->info.generation + 1;
    }
    modifyMember(memberAddr->child, name);
    modifyMember(memberAddr->sibling, name);
}

void deleteMember(FamilyMemberAddr memberAddr, char name[]) {
    if (memberAddr == NULL) {
        return;
    }
    if (!strcmp(memberAddr->info.name, name)) {

        c = 0;
        if(memberAddr->older_sibling == NULL) {
            if (memberAddr->sibling == NULL) {
            memberAddr->parent->child = NULL;
            }
            else {
                memberAddr->sibling->older_sibling = NULL;
                memberAddr->parent->child = memberAddr->sibling;
            }
        }
        else {
            memberAddr->sibling->older_sibling = memberAddr->older_sibling;
            memberAddr->older_sibling->sibling = memberAddr->sibling;
        }

    }
    deleteMember(memberAddr->child, name);
    deleteMember(memberAddr->sibling, name);

}

void saveLoad() {
    writefile = fopen("family","w");
    for ( i = 0; i < n; i++) {
        fprintf(writefile, "%d %s %s %d %d %d\n", membersArray[i]->info.generation, membersArray[i]->info.name, membersArray[i]->info.gender,
                membersArray[i]->info.birth_year, membersArray[i]->info.birth_month, membersArray[i]->info.birth_day);
    }
    fclose(writefile);
    i = 0;
}

int main(void) {

    readfile = fopen(".family", "r");
    writefile = fopen("family", "w");
    int function_selected;

    FamilyMemberAddr ancestor = memberCreate();
    fileAddChild(ancestor);
    fileAddSibling(ancestor->child);
    fileAddSibling(ancestor->child->sibling);
    fileAddSibling(ancestor->child->sibling->sibling);
    fileAddSibling(ancestor->child->sibling->sibling->sibling);
    fileAddSibling(ancestor->child->sibling->sibling->sibling->sibling);
    fileAddSibling(ancestor->child->sibling->sibling->sibling->sibling->sibling);
    fileAddSibling(ancestor->child->sibling->sibling->sibling->sibling->sibling->sibling);
    fileAddSibling(ancestor->child->sibling->sibling->sibling->sibling->sibling->sibling->sibling);
    fileAddChild(ancestor->child->sibling);
    fileAddSibling(ancestor->child->sibling->child);
    fileAddChild(ancestor->child->sibling->child->sibling);
    fileAddChild(ancestor->child->sibling->child);
    fileAddSibling(ancestor->child->sibling->child->child);
    fileAddChild(ancestor->child->sibling->child->sibling->child);

    fclose(readfile);

        printf("欢迎来到家谱管理理系统!\n"
               "1. 显示家谱图\n"
               "2. 根据姓名查询，输出成员信息 \n"
               "3. 给某个成员添加孩⼦\n"
               "4. 修改某个成员信息\n"
               "5. 删除某成员(连同其后代) \n"
               "6. 按出生⽇期增顺显示家族成员 \n"
               "7. 退出系统\n");

    do {
        printf("请输入1至7的整数实现相应功能：");
        scanf("%d", &function_selected);
        while (getchar() != '\n');

        switch (function_selected) {

            char input_name[NAME_LEN];

            case 1: getArray(ancestor); sortByGeneration(); break;
            case 2: printf("请输⼊姓名：");
                scanf("%s", input_name);
                searchByName(ancestor, input_name);
                if (c) printf("查无此人\n");
                c = 1; break;
            case 3: printf("请输⼊成员姓名：");
                scanf("%s", input_name);
                manuAddChild(ancestor, input_name);
                if (c) printf("查无此人\n");
                else {
                    getArray(ancestor);
                    saveLoad();
                    printf("已添加该成员，并保存至./family\n");
                }
                c = 1; break;
            case 4: printf("请输⼊成员姓名：");
                scanf("%s", input_name);
                modifyMember(ancestor, input_name);
                if (c) printf("查无此人\n");
                else {
                    getArray(ancestor);
                    saveLoad();
                    printf("已修改该成员信息，并保存至./family\n");
                }
                c = 1; break;
            case 5: printf("请输⼊成员姓名：");
                scanf("%s", input_name);
                deleteMember(ancestor, input_name);
                if (c) printf("查无此人\n");
                else {
                    getArray(ancestor);
                    saveLoad();
                    printf("已删除该成员及其子女信息，并保存至./family\n");
                }
                c = 1; break;
            case 6: getArray(ancestor); sortByBirthday(); break;
            case 7: break;
            default: printf("输入有误，请重新选择！\n"); break;
        }

    } while (function_selected != 7);

    return 0;
}
