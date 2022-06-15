#define _CRT_SECURE_NO_WARNINGS
#include "stdlib.h"
#include "stdio.h"
#include "locale.h"
#include "windows.h"
#include "time.h"
#include <math.h>
#include "assert.h"
#define Distance 15

/*��������� � ������ � ����� ������ ����� ������*/
struct DataAndKeys {
    int data; /*���� ����� �������*/
    char* key; /*���� �������*/
};

/*��������� ����� ������*/
struct BTNode { /*������� ������*/
    struct BTNode* left; /*�������� �� ���� �������*/
    DataAndKeys treeData; /*���� � ���� ����� �������*/
    struct BTNode* right; /*�������� �� ������ �������*/
};

/*������ ����������� ������*/
struct BTDescriptor { /*���������� ������*/
    int size; /*����� ������*/
    BTNode* root; /*����� ������*/
};

/*��������� �������� ������*/
struct Trunk
{
    Trunk* prev; //��������� �������
    char* str; //��� ����� �'������ �������� ������
};

/*��������� �������*/
BTDescriptor* createTree(void);
BTNode* initializeNode(const DataAndKeys* entry);
BTNode* findSmallestElement(BTNode* tree);
BTNode* deleteElement(BTNode* subTree, char* key);
int numberScan(int& N);
int fopenCheck(FILE*& fp, char filename[], char d);
int elementCounter(FILE*& fd);
int getDataAndKeys(BTNode* tree, DataAndKeys* dAK, int* j);
int compareDataAndKeys(DataAndKeys* left, DataAndKeys* right);
int keyUsingSearch(BTNode* tree, char* key, int* noElementCheck);
int insertTree(BTDescriptor* dsTree, DataAndKeys* entry);
int freeStruct(DataAndKeys* dAK, int n);
int printTree(BTNode* root, Trunk* prev, bool isLeft);
int showCurrentElementsPlace(BTNode* root, Trunk* prev, bool isLeft, char* key);
int readFromFile(FILE* fp, int size, DataAndKeys* dAK);
int writeToFile(FILE* fp, int size, DataAndKeys* dAK);
void setAKey(int number, char* key);
void checkFilename(char filename[]);
void getHeight(BTDescriptor* dsTree, BTNode* tree, int* height, int* curMaxHeight, int* prevMaxHeight);
void preOrder(BTNode* tree, int* j);
void postOrder(BTNode* tree, int* j);
void inOrder(BTNode* tree, int* j);
void keyScan(char* key);
void showTrunks(Trunk* trunk);
void deleteTree(BTNode* tree);

int main()
{
    FILE* fr = NULL; //��� ������������ ������ � ������
    int workWithBT; //��� ������ ����
    int counter = 0; //��� ������� �������� � �����
    int numberForLetter = 0; //��� ��������� ���� ��������� ����������� �����
    int minimum = 0; //��� ������ ������� ����� ��������� ���������� �����
    int maximum = 0; //��� ������� ������� ����� ��������� ���������� �����
    int noElementCheck = 1; //��� �������� �� �������� �������� � �����
    int elementNumber = 0; //��� ��������� ���� ��������� ������������� �����
    char filename[50]; //� ������ �����
    bool flag = true; //��� ���������� ����� ������ ����
    char* forKey = NULL; //� ������, ���� ���������� ������� � ���������
    int height = 0; //��� ������ ������
    int curMaxHeight = 0; /*��� ������� ����������� ������ ������ ��� ����������
����*/
    int maxHeight = 0; //��� ����������� ������ ������ ��� ���������� ����
    int count = 0; //��� ������� �������� � ����� ������������ ����
    int fileReadCheck = 0; /*��� �������� �� ������� ��� ��������� �������� ������ � �����*/
    int insertCheck = 0; //��� �������� �� ������� ��� ��������� ��������
    int fOpenChecker = 0; /*��� �������� �� ������� ��� ����������� ������ � ������*/

    SetConsoleCP(1251); //���������� ��������
    SetConsoleOutputCP(1251);

    srand((unsigned int)time(NULL));
    BTDescriptor* BTd = createTree(); //��������� ������

    if (BTd == NULL) { //�������� �� ��, �� �������� ������
        printf("������ �� ��������!");
        return -1;
    }

    printf("������ ��������!\n");

    do {
        printf("1)��������� ������ � ��������� �����;\n2)�������� �����\
 ����;\n3)��������� ������ ����������� �������.\n");
        numberScan(workWithBT); //���������� ������ ����
        switch (workWithBT) {
        case 1: { //���������� ������ � ��������� �����
            printf("������ ����� ����� � ������ name.bin, � ����� ������ �������\
 ���:\n");
            checkFilename(filename); //�������� ����������� �������� ����� �����
            fOpenChecker = fopenCheck(fr, filename, 'r'); /*������������ ������ ��� ������ � ������ � ����� ����������, �������� �� ������� �� �� ��, �� ���� �����������*/
            if (fOpenChecker == -1) //�� ������� ������� ����
            {
                printf("�� ������� ������� ���� �� ������� ������!\n");
                exit(0);
            }

            else if (fOpenChecker == -2) /*���������� ������� ����������� ������ ��� ������� ���� ������ � ������*/
            {
                printf("�� �������� ����������� ������ ��� ������� ���� ������ � ������!\n");
                exit(0);
            }
            counter = elementCounter(fr); //��������� ������� �������� � ����
            if (counter == -1)
            {
                printf("������� ��� ��������� ��������� �� ������� �����!\n");
                exit(0);
            }
            DataAndKeys* dAK = (DataAndKeys*)malloc((counter) * sizeof(DataAndKeys));
            //�������� ���'�� ��� ������ ������������ ����
            if (!dAK) /*�������� �� ��, �� ���'��� ������� �� ����� ������������ ����*/
            {
                printf("����������� ���'�� ��� �������� �� �� �����\n!");
                fclose(fr); //�������� ������ ������ � ������
                exit(0);
            }
            fileReadCheck = readFromFile(fr, counter, dAK); /*����������� �������� � ����� �� ������ ������������ ����*/
            if (fileReadCheck == -1) //���� �� ������� ���������� �������� � �����
            {
                printf("�� ������� ���������� �������� � �����!\n");
                if (freeStruct(dAK, counter) == -1) /*��������� ���'�� ��� ������ ������������ ���� � �������� �� ��, �� �� �������*/
                {
                    printf("������ ������������ ���� �� ����!\n");
                    exit(0);
                }
            }
            else if (fileReadCheck == -2) /*���� ���� �������� � ���� �� ������� ������ � 8 ��������*/
            {
                printf("���� �������� � ���� �� ������� ������ �\
 8 ��������!\n");
                if (freeStruct(dAK, counter) == -1) /*��������� ���'�� ��� ������ ������������ ���� � �������� �� ��, �� �� �������*/
                {
                    printf("������ ������������ ���� �� ����!\n");
                    exit(0);
                }
            }
            else if (fileReadCheck == -3) /*���� �� ������ ���������� �������� ��
������� �����*/
            {
                printf("�� ������ ���������� �������� �� ������� �����!\n");
                if (freeStruct(dAK, counter) == -1) /*��������� ���'�� ��� ������ ������������ ���� � �������� �� ��, �� �� �������*/
                {
                    printf("������ ������������ ���� �� ����!\n");
                    exit(0);
                }
            }

            for (int i = 0; i < counter; i++) { /*����������� �������� � ������
������������ ���� �� ������*/
                insertCheck = insertTree(BTd, &dAK[i]); /*���������� �������� ��
������*/
                if (insertCheck == -1) //�� ������� �������� ������� � ������
                {
                    printf("�� ������� �������� ������� � ������!\n");
                    fclose(fr); //�������� ������ ������ � ������
                    if (freeStruct(dAK, counter) == -1) /*��������� ���'�� ���
������ ������������ ���� � �������� �� ��, �� �� �������*/
                    {
                        printf("������ ������������ ���� �� ����!\n");
                        exit(0);
                    }
                    exit(0);
                }
                else if (insertCheck == -2) /*���������� ������� ������ ����������
�����������*/
                {
                    printf("�� �������� ��������� ���������� ������!\n");
                    fclose(fr); //�������� ������ ������ � ������
                    if (freeStruct(dAK, counter) == -1) /*��������� ���'�� ���
������ ������������ ���� � �������� �� ��, �� �� �������*/
                    {
                        printf("������ ������������ ���� �� ����!\n");
                        exit(0);
                    }
                    exit(0);
                }
            }
            fclose(fr); //�������� ������ ������ � ������
            if (freeStruct(dAK, counter) == -1) /*��������� ���'�� ��� ������
������������ ���� � �������� �� ��, �� �� �������*/
            {
                printf("������ ������������ ���� �� ����!\n");
                exit(0);
            }
            break;
        }
        case 2: { //��������� ������ �����
            printf("������ ����� ����� � ������ name.bin ��� �������� ������ �\
 ���:\n");
            checkFilename(filename); //�������� ����������� �������� ����� �����
            break;
        }
        case 3: { //���������� ������ ����������� �������
            printf("������ ����� ����� � ������ name.bin ��� �������� ������ �\
 ���:\n");
            checkFilename(filename); //�������� ����������� �������� ����� �����

            do {
                printf("������ ������� �������� ������:\n");
                numberScan(counter); //���������� ������ ����

                if (counter < 1) {
                    printf("������� ���������� ��������! ��������� �� ���!\n");
                }
            } while (counter < 1);

            printf("������ ����� ������� ��� ��������� ���������� �����:\n");
            numberScan(minimum); /*���������� ��������� �� ������ �������� ���
������ ������� ����� ��������� ���������� �����*/
            printf("������ ������ ������� ��� ��������� ���������� �����:\n");
            numberScan(maximum); /*���������� ��������� �� ������ �������� ���
������� ������� ����� ��������� ���������� �����*/
            if (maximum < minimum) { /*�������� �� ��, ��� ������ ������� ���
��������� ���������� ����� �� ���� ������ �� �����*/
                printf("������ ������� �� ���� ���� ������ �� �����!\n");
                exit(0);
            }

            DataAndKeys* dAK = (DataAndKeys*)malloc((counter) * sizeof(DataAndKeys));
            //�������� ���'�� ��� ������ ������������ ����
            if (!dAK) /*�������� �� ��, �� ���'��� ������� �� ����� ������������
����*/
            {
                printf("����������� ���'�� ��� �������� �� �� �����\n!");
                exit(0);
            }
            for (int i = 0; i < counter; i++) {
                dAK[i].key = (char*)malloc(10 * sizeof(char)); /*�������� ���'�� ���
����� �������� ������ ������������ ����*/
                if (!dAK[i].key) /*�������� �� ��, �� ��������� ���'��� �� ����
�������� ������ ������������ ����*/
                {
                    printf("����������� ���'�� ��� �������� �� �� �����\n!");
                    if (freeStruct(dAK, counter) == -1) /*��������� ���'�� ���
������ ������������ ���� � �������� �� ��, �� �� �������*/
                    {
                        printf("������ ������������ ���� �� ����!\n");
                        exit(0);
                    }
                    exit(0);
                }
                dAK[i].data = minimum + rand() % (maximum - minimum + 1); /*���������
����������� �����*/

                if (dAK[i].data < minimum || dAK[i].data > maximum) /*�������� �� ��,
�� ��������� ����������� ��������� ���� �����*/
                {
                    printf("��������� ���� ����� ����������� ������!\n");
                    if (freeStruct(dAK, counter) == -1) /*��������� ���'�� ���
������ ������������ ���� � �������� �� ��, �� �� �������*/
                    {
                        printf("������ ������������ ���� �� ����!\n");
                        exit(0);
                    }
                    exit(0);
                }
                setAKey(dAK[i].data, dAK[i].key);
            }

            for (int i = 0; i < counter; i++) /*����������� �������� � ������
������������ ���� �� ������*/
            {
                if (insertTree(BTd, &dAK[i]) == -1) /*���������� �������� �� ������ ��
�������� �� ��, �� �� �������*/
                {
                    printf("�� ������� �������� ������� � ������!\n");
                    if (freeStruct(dAK, counter) == -1) /*��������� ���'�� ���
������ ������������ ���� � �������� �� ��, �� �� �������*/
                    {
                        printf("������ ������������ ���� �� ����!\n");
                        exit(0);
                    }
                    exit(0);
                }
            }
            if (freeStruct(dAK, counter) == -1) /*��������� ���'�� ��� ������
������������ ���� � �������� �� ��, �� �� �������*/
            {
                printf("������ ������������ ���� �� ����!\n");
                exit(0);
            }
            break;
        }
        default: { //���� ���������� ��� ���������� ��������
            printf("�� ����� ���������� ��������, ��������� �� ���!\n");
            flag = 0;
            continue;
        }
        }
    } while (!flag);

    for (;;) {
        printf("\n������� ��� ������ � ������� ������� ����� �����:\n1)������� ��\
 ����� �������;\n2)�������� ������� � ������;\n3)��������� ������� � ������\n\
4)�������� ����� ������ �� ���� ������;\n5)����� ������ �� �����;\n\
6)������� ��������� �������� � �����;\n7)���������� ������ �� �����;\n\
8)������� ������ � �����;\n9)����������� ������;\n0)��������� ��������;\n");
        numberScan(workWithBT); //���������� ������ ����
        if (workWithBT == 8) //���������� ������ � �����
        {
            if (BTd != NULL)
            {
                deleteTree(BTd->root); //��������� ���� �� ������
                free(BTd); //��������� ���� �� ���������� ������
                BTd = createTree(); //��������� ������
                if (BTd == NULL) { //�������� �� ��, �� �������� ������
                    printf("������ �� ��������!");
                    return -1;
                }
            }
            fOpenChecker = fopenCheck(fr, filename, 'r'); /*������������ ������ ��� ������ � ������ � ����� ����������, �������� �� ������� �� �� ��, �� ���� �����������*/
            if (fOpenChecker == -1) //�� ������� ������� ����
            {
                printf("�� ������� ������� ���� �� ������� ������!\n");
                exit(0);
            }

            else if (fOpenChecker == -2) /*���������� ������� ����������� ������ ��� ������� ���� ������ � ������*/
            {
                printf("�� �������� ����������� ������ ��� ������� ���� ������ � ������!\n");
                exit(0);
            }
            counter = elementCounter(fr); //��������� ������� �������� � ����
            DataAndKeys* dAK = (DataAndKeys*)malloc((counter) * sizeof(DataAndKeys));
            //�������� ���'�� ��� ������ ������������ ����
            if (!dAK) /*�������� �� ��, �� ���'��� ������� �� ����� ������������
����*/
            {
                printf("����������� ���'�� ��� �������� �� �� �����\n!");
                exit(0);
            }
            fileReadCheck = readFromFile(fr, counter, dAK); /*����������� �������� �
����� �� ������ ������������ ����*/
            if (fileReadCheck == -1) //���� �� ������� ���������� �������� � �����
            {
                printf("�� ������� ���������� �������� � �����!\n");
                if (freeStruct(dAK, counter) == -1) /*��������� ���'�� ��� ������
������������ ���� � �������� �� ��, �� �� �������*/
                {
                    printf("������ ������������ ���� �� ����!\n");
                    exit(0);
                }
            }

            else if (fileReadCheck == -2) /*���� ���� �������� � ����
�� ������� ������ � 8 ��������*/
            {
                printf("���� �������� � ���� �� ������� ������ �\
 8 ��������!\n");
                if (freeStruct(dAK, counter) == -1) /*��������� ���'�� ��� ������
������������ ���� � �������� �� ��, �� �� �������*/
                {
                    printf("������ ������������ ���� �� ����!\n");
                    exit(0);
                }
            }
            else if (fileReadCheck == -3) /*���� �� ������ ���������� �������� ��
������� �����*/
            {
                printf("�� ������ ���������� �������� �� ������� �����!\n");
                if (freeStruct(dAK, counter) == -1) /*��������� ���'�� ��� ������
������������ ���� � �������� �� ��, �� �� �������*/
                {
                    printf("������ ������������ ���� �� ����!\n");
                    exit(0);
                }
            }
            for (int i = 0; i < counter; i++) { /*����������� �������� � ������
������������ ���� �� ������*/
                insertCheck = insertTree(BTd, &dAK[i]); /*���������� �������� ��
������*/
                if (insertCheck == -1) //�� ������� �������� ������� � ������
                {
                    printf("�� ������� �������� ������� � ������!\n");

                    if (freeStruct(dAK, counter) == -1) /*��������� ���'�� ���
������ ������������ ���� � �������� �� ��, �� �� �������*/
                    {
                        printf("������ ������������ ���� �� ����!\n");
                        exit(0);
                    }
                    fclose(fr); //�������� ������ ������ � ������
                    exit(0);
                }
                else if (insertCheck == -2) /*���������� ������� ������ ����������
�����������*/
                {
                    printf("�� �������� ��������� ����������!\n");
                    if (freeStruct(dAK, counter) == -1) /*��������� ���'�� ���
������ ������������ ���� � �������� �� ��, �� �� �������*/
                    {
                        printf("������ ������������ ���� �� ����!\n");
                        exit(0);
                    }
                    fclose(fr); //�������� ������ ������ � ������
                    exit(0);
                }
            }

            if (freeStruct(dAK, counter) == -1) /*��������� ���'�� ��� ������
������������ ���� � �������� �� ��, �� �� �������*/
            {
                printf("������ ������������ ���� �� ����!\n");
                exit(0);
            }
            fclose(fr); //�������� ������ ������ � ������
            continue;
        }
        switch (workWithBT) {
        case 1: //��������� �� ����� ��������
        {
            if (!(forKey = (char*)malloc(sizeof(char) * 256))) /*�������� ���'�� ��� �����, ���� ������� � ���������, � �������� �� ��, �� �� �������*/
            {
                printf("����������� ���'�� ��� �������� �� �� �����!\n");
                exit(0);
            }
            noElementCheck = 1;

            printf("������ ���� �������� ������,\
 ���� ������ ������� �� �����:\n");
            keyScan(forKey); //���������� ��������� � ��������� �����
            if (keyUsingSearch(BTd->root, forKey, &noElementCheck) == 1) /*����� �������� � ����� �� ������*/
            {
                printf("�������� � ����� ������ ���� � �����!\n");
            }
            free(forKey); //��������� ���'�� ��� ��������� � ��������� �����
            break;
        }
        case 2: //���������� �������� � ������
        {
            DataAndKeys* dAK = (DataAndKeys*)malloc(sizeof(DataAndKeys)); /*�������� ���'�� ��� ����� ������������ ����*/

            if (!dAK) /*�������� �� ��, �� ���'��� ������� �� ����� ������������
����*/
            {
                printf("����������� ���'�� ��� �������� �� �� �����\n!");
                exit(0);
            }
            if (!(dAK->key = (char*)malloc(10 * sizeof(char)))) /*�������� ���'��
��� ����� ����� ������������ ���� � �������� �� ��, �� �� �������*/
            {
                printf("����������� ���'�� ��� �������� �� �� �����!\n");
                free(dAK); //��������� ���'�� ��� ����� ������������ ����
                exit(0);
            }
            printf("������ ���� �����, ������� � ���� ������ �������� � ������:\n");
            numberScan(elementNumber); /*���������� ��������� � ��������� ������ �����*/
            dAK->data = elementNumber;
            setAKey(dAK->data, dAK->key);
            insertCheck = insertTree(BTd, dAK); //���������� �������� �� ������
            if (insertCheck == -1) //�� ������� �������� ������� � ������
            {
                printf("�� ������� �������� ������� � ������!\n");
                free(dAK->key); /*��������� ���'�� ��� ����� ����� ������������
����*/
                free(dAK); //��������� ���'�� ��� ����� ������������ ����
                exit(0);
            }
            else if (insertCheck == -2) /*���������� ������� ������ ����������
�����������*/
            {
                printf("�� �������� ��������� ���������� ������!\n");
                free(dAK->key); /*��������� ���'�� ��� ����� ����� ������������
����*/
                free(dAK); //��������� ���'�� ��� ����� ������������ ����
                exit(0);
            }
            counter++;
            free(dAK->key); //��������� ���'�� ��� ����� ����� ������������ ����
            free(dAK); //��������� ���'�� ��� ����� ������������ ����
            break;
        }
        case 3: //���������� �������� � ������
        {
            if (!(forKey = (char*)malloc(sizeof(char) * 256))) /*�������� ���'�� ��� �����, ���� ������� � ���������, � �������� �� ��, �� �� �������*/
            {
                printf("����������� ���'�� ��� �������� �� �� �����!\n");
                exit(0);
            }

            printf("������ ����, ������� � ����\ ������ ��������� � ������:\n");
            keyScan(forKey); //���������� ��������� � ��������� �����

            noElementCheck = 1;
            keyUsingSearch(BTd->root, forKey, &noElementCheck);
            if (noElementCheck == 1)
            {
                printf("�������� �� ������� ������ ���� � �����!\n");
                exit(0);
            }
            if (deleteElement(BTd->root, forKey) == NULL) /*��������� �������� � ������*/
            {
                printf("�������� ������ � ����������!\n");
                exit(0);
            }
            counter--;
            free(forKey); //��������� ���'�� ��� ��������� � ��������� �����
            break;
        }
        case 4: //���� ������ ������ �� ���� ������
        {
            height = 0;
            curMaxHeight = 0;
            maxHeight = 0;
            getHeight(BTd, BTd->root, &height, &curMaxHeight, &maxHeight); /*��������� ������ ������*/
            printf("ʳ������ �������� � �����-%d\n������ ������-%d\n", BTd->size,
                maxHeight);
            break;
        }
        case 5: //����� ������ �� �����
        {
            if (printTree(BTd->root, nullptr, false) == -1) /*���� �� ������ ������ � ��������� �� ��, �� �� �������*/
            {
                printf("������� ��� �������� ������ �� �����!\n");
                exit(0);
            }
            break;
        }
        case 6: //����� ��������� ��������� �������� ������ �� �����
        {
            if (!(forKey = (char*)malloc(sizeof(char) * 256))) /*�������� ���'�� ��� �����, ���� ������� � ���������, � �������� �� ��, �� �� �������*/
            {
                printf("����������� ���'�� ��� �������� �� �� �����!\n");
                exit(0);
            }
            noElementCheck = 1;
            printf("������ ���� ��������,\
 ��������� ����� ������ �������� � �����:\n");
            keyScan(forKey); //���������� ��������� � ��������� �����
            if (keyUsingSearch(BTd->root, forKey, &noElementCheck) == 1) /*����� �������� � ����� �� ������*/
            {
                printf("�������� � ����� ������ ���� � �����!\n");
            }
            else
            {
                if (showCurrentElementsPlace(BTd->root, nullptr, false, forKey) == -1) /*���� �� ������ ������ � ������� ��������� �������� �������� �� �������� �� ��, �� �� �������*/
                {
                    printf("������� ��� �������� ������ �� �����!\n");
                    exit(0);
                }
            }
            free(forKey); //��������� ���'�� ��� ��������� � ��������� �����
            break;
        }
        case 7: //������� ������ �� �����
        {
            fOpenChecker = fopenCheck(fr, filename, 'w'); /*������������ ������ ��� ������ � ������ � ����� ������, �������� �� ������� �� �� ��, �� ���� �����������*/
            if (fOpenChecker == -1) //�� ������� ������� ����
            {
                printf("�� ������� ������� ���� �� ������� ������!\n");
                exit(0);
            }
            else if (fOpenChecker == -2) /*���������� ������� ����������� ������ ��� ������� ���� ������ � ������*/
            {
                printf("�� �������� ����������� ������ ��� ������� ���� ������ � ������!\n");
                exit(0);
            }
            DataAndKeys* dAK = (DataAndKeys*)malloc(BTd->size * sizeof(DataAndKeys));
            //�������� ���'�� ��� ������ ������������ ����

            if (!dAK) /*�������� �� ��, �� ���'��� ������� �� ����� ������������ ����*/
            {
                printf("����������� ���'�� ��� �������� �� �� �����\n!");
                exit(0);
            }
            count = 0;

            if (getDataAndKeys(BTd->root, dAK, &count) == -1) /*����� ����������
������ ������ �� ������ ������������ ���� �� �������� �� ��, �� �� �������*/
            {
                printf("�� ������� ���������� ���������� � ������ ������!\n");
                if (freeStruct(dAK, count) == -1) /*��������� ���'�� ��� ������
������������ ���� � �������� �� ��, �� �� �������*/
                {
                    printf("������ ������������ ���� �� ����!\n");
                    exit(0);
                }
                exit(0);
            }
            if (writeToFile(fr, count, dAK) == -1) /*������������� ������ ������������
���� �� ����� �� �������� �� ��, �� �� �������*/
            {
                printf("�� ������� ���������� ����� �� �����!\n");
                if (freeStruct(dAK, count) == -1) /*��������� ���'�� ��� ������
������������ ���� � �������� �� ��, �� �� �������*/
                {
                    printf("������ ������������ ���� �� ����!\n");
                    exit(0);
                }
                exit(0);
            }
            if (freeStruct(dAK, count) == -1) /*��������� ���'�� ��� ������
������������ ���� � �������� �� ��, �� �� �������*/
            {
                printf("������ ������������ ���� �� ����!\n");
                exit(0);
            }
            fclose(fr); //�������� ������ ������ � ������
            break;
        }
        case 9: //����������� ������
        {
            do
            {
                printf("\n������ ������� ����������� ��������\
 ������:\n1)������;\n2)��������;\n3)�����������;\n");
                numberScan(workWithBT); //���������� ������ ����
                switch (workWithBT)
                {
                case 1:
                {
                    count = 0;
                    preOrder(BTd->root, &count); /*����������� ������ � �������
�������*/
                    flag = 0;
                    break;
                }

                case 2:
                {
                    count = 0;
                    postOrder(BTd->root, &count); /*����������� ������ � �����������
�������*/
                    flag = 0;
                    break;
                }
                case 3:
                {
                    count = 0;
                    inOrder(BTd->root, &count); //����������� ������ ����������
                    flag = 0;
                    break;
                }
                default:
                {
                    printf("�� ����� ���������� ��������, ��������� �� ���!\n");
                    flag = 1;
                    continue;
                }
                }
            } while (flag != 0);
            break;
        }
        case 0: //���������� ������
        {
            deleteTree(BTd->root); //���������� ���'�� ��� ������
            free(BTd); //���������� ���'�� ��� ����������� ������
            printf("\n������� ������ ��������!\n������ �������� ���������!\n");
            return 0;
        }
        default: { //���� ���������� ��� ���������� ��������
            printf("�� ����� ���������� ��������, ��������� �� ���!\n");
            continue;
        }
        }
    }
}

/*
������� ��������� ������
����: �������
�����:
    createTree = <������ ���������� �����������> - ������ ��������;
               = NULL - ������ �� ��������
*/
BTDescriptor* createTree() {
    BTDescriptor* dsBT;
    dsBT = (BTDescriptor*)malloc(sizeof(BTDescriptor)); /*�������� ���'�� ��
���������� ������*/
    if (!dsBT) return NULL; /*���� �������� ���'�� � ���, ���������� ��
���������*/
    dsBT->root = NULL;
    dsBT->size = 0;
    return dsBT;
}

/*
������� ��������� ������� ������ �� ����������� �� ����
����:
    entry - ������ ����� ���� DataAndKeys � ������ ��� �����������
�����:
    initializeNode = <������ �������> - �������� �������;
                   = NULL - ������� �� �������� ��� �� ������������
*/
BTNode* initializeNode(const DataAndKeys* entry)
{
    BTNode* newNode = (BTNode*)malloc(sizeof(BTNode)); /*�������� ���'�� �� �������
������*/
    if (!newNode)
    {
        return NULL;
    }
    if (!(newNode->treeData.key = (char*)malloc(10 * sizeof(char)))) /*��������
���'�� �� ���� key �������*/
    {
        return NULL;
    }
    newNode->treeData.data = entry->data;
    strcpy(newNode->treeData.key, entry->key);
    newNode->left = newNode->right = NULL;
    return newNode;
}

/*
���������� ������� ����������� ���������� �������� � �����
����:
    tree - ������ ������� ������
�����:
    findSmallestElement = <������ �������> - ������� ��������
                        = NULL - ������� �� ��������
*/
BTNode* findSmallestElement(BTNode* tree) {
    if (tree == NULL || tree->right == NULL)
        return tree;
    else
        return findSmallestElement(tree->right);
}

/*
������� ��������� �������� � ������
����:
    subTree - ������ ������� ������
    key - ����� � ������ ������ ������� ������
�����:
    deleteElement = NULL - ���� ������ ���������
                  = <������ �������, ��� ����� ���� ��������> - ���� ����� ������ ���������
*/
BTNode* deleteElement(BTNode* subTree, char* key) {
    //������� ����������� ������
    if (!subTree)
    {
        return subTree;
    }

    //���� ���� �������� �������� ������
    //�� ���� ��������, � ���� �� ����� ��������� ����
    if (strcmp(key, subTree->treeData.key) > 0)
    {
        subTree->left = deleteElement(subTree->left, key);
    }

    //���� ���� �������� �������� ������
    //�� ���� ��������, � ���� �� ����� ��������� ����
    else if (strcmp(key, subTree->treeData.key) < 0)
    {
        subTree->right = deleteElement(subTree->right, key);
    }

    //���� ���� �������� ��������
    //������� � ������ �����, � ����
    //�� ����� ���� ���������, �� ���������
    //��� �����
    else {
        //���� ������� � ����� �������� ��� ��� ���
        if (subTree->left == NULL) {
            BTNode* temp = subTree->right;
            free(subTree);
            return temp;
        }
        else if (subTree->right == NULL) {
            BTNode* temp = subTree->left;
            free(subTree);
            return temp;
        }

        //���� ������� �� ��� �������
        BTNode* temp = findSmallestElement(subTree->right);

        strcpy(subTree->treeData.key, temp->treeData.key);
        subTree->treeData.data = temp->treeData.data;

        //��������� ���������� �������
        subTree->right = deleteElement(subTree->right, temp->treeData.key);
    }
    return subTree;
}

/*������� �������� ������� ������
����:
    N - ������ ����� ��� �����������
�����:
    numberScan = N - ������������ �����
*/
int numberScan(int& N) {
    int entered = 0; //����� ��� �������� �������� ������������ � ��������� �������
    do {
        do {
            entered = scanf_s("%d", &N);
            while (getchar() != '\n');
            while (entered < 1) {
                printf("�� ����� ���������� ��������, �������� ��������: ");
                entered = scanf_s("%d", &N);
                while (getchar() != '\n');
            }
        } while (entered < 1);
    } while (entered != 1);
    return N;
}

/*
������� �i������� ����� �� �������� �� �������
����:
    fp - �������� ����������
    filename - ����� � �������� ������������ ����� �����
    d - ����� ��� ���������� ����, � ����� ����� ���������� ���� ������� ����
�����:
    fopenCheck = -1 - ���� ������� ������� ��� ������� �����
               = -2 - ���� ���������� ������� ����������� ������ ��� ������� ���� ������ � ������
               = 0 - ��� �������� ����������� ������ ������ � ������
*/
int fopenCheck(FILE*& fp, char filename[], char d) {
    if (d == 'r') {
        if ((fp = fopen(filename, "r")) == NULL)
        {
            return -1;
        }
    }
    else if (d == 'w') {
        if ((fp = fopen(filename, "w")) == NULL)
        {
            return -1;
        }
    }
    else if (d == 'a') {
        if ((fp = fopen(filename, "a")) == NULL)
        {
            return -1;
        }
    }
    else
    {
        return -2;
    }
    return 0;
}

/*������� ��� ��������� ������� �������� � ����
����:
    fd - �������� ����������
�����:
    elementCounter = count - ������� �������� � ����;
                   = -1 - ���� �� ��������� �������� �� ������� �����
*/
int elementCounter(FILE*& fd) {
    int count = 0; /*����� ��� ��������� ������� ��������, �� ���������� �
����*/

    while (!feof(fd)) {
        fscanf(fd, "%*s");
        fscanf(fd, "%*d");
        count++;
    }
    rewind(fd);
    if (ferror(fd)) {
        return -1; //�� ��������� �������� �� ������� �����
    }
    return count;
}

/*
������� �������� ���������� ������ ������ �� ������ ������������ ����
����:
    tree - ������ ������� ������
    dAK - ������ ������ ������������ ����
    j - ������ ����� � ������� ��������� �������� ������
�����:
    getDataAndKeys = -1 - ���� �� ������ ���������� ���������� � ������ ������
                   = 0 - ���� ������ ���������� ���������� � ������ ������
*/
int getDataAndKeys(BTNode* tree, DataAndKeys* dAK, int* j) {
    if (tree != NULL) { /*���� ������� ����*/
        dAK[*j].key = (char*)malloc(10 * sizeof(char));
        if (!dAK[*j].key)
        {
            return -1;
        }
        dAK[*j].data = tree->treeData.data;
        strcpy(dAK[*j].key, tree->treeData.key);
        *j = *j + 1;
        getDataAndKeys(tree->left, dAK, j); /*"������" � ��� �������� �����*/
        getDataAndKeys(tree->right, dAK, j); /*"������" � ����� �������� �����*/
    }
    return 0;
}

/*
������� ��������� ������ ���� ������ ������������ ����
����:
    left - ������ ����� ���� DataAndKeys
    right - ������ ����� ���� DataAndKeys
�����:
    compareDataAndKeys = 0 - ���� ����� ���
                       > 0 - ���� ���� ����� ����� ������ �� ���� ����� �����
                       < 0 - ���� ���� ����� ����� ������ �� ���� ����� �����
*/
int compareDataAndKeys(DataAndKeys* left, DataAndKeys* right)
{
    return strcmp(left->key, right->key);
}

/*
������� ������ �������� �� ������ ��� ��������� ���� �� �����
����:
    tree - ������ ������� ������
    key - ����� � ������
    noElementCheck - ������ ����� ��� �������� �������� �������� � �����
�����:
    keyUsingSearch = noElementCheck = 0 - ���� ������� � ������, �� ���� ���������
                             �����, � � �����
                                    = 1 - ���� �������� � ������, �� ���� ���������
        �����, ���� � �����
*/
int keyUsingSearch(BTNode* tree, char* key, int* noElementCheck) {
    if (tree != NULL) { /*���� ������� ����*/
        if (strcmp(key, tree->treeData.key) == 0)
        {
            printf("������� ������ � ������ %s:\nֳ�� �����, ��� ������ �������-\
%d\n", key, tree->treeData.data);
            *noElementCheck = 0;
            return *noElementCheck;
        }
        keyUsingSearch(tree->left, key, noElementCheck); /*"������" � ��� ��������
�����*/
        keyUsingSearch(tree->right, key, noElementCheck); /*"������" � �����
�������� �����*/
    }
    return *noElementCheck;
}

/*
������� ������ �������� �� ������ ��� ��������� ���� �� �����
����:
    tree - ������ ������� ������
    key - ����� � ������
    data - ������ ����� ��� ������ ������ ����� �������
�����:
    searchForData = data = 0 - ���� �������� � ������, �� ���� ��������� �����, ����
    � �����
                        != 0 - ���� ����� �������, ���� ��� ������� � ����� � �������
    ������ �
*/
int searchForData(BTNode* tree, char* key, int* data) {
    if (tree != NULL) { /*���� ������� ����*/
        if (strcmp(key, tree->treeData.key) == 0)
        {
            printf("������� ������ � ������ %s:\nֳ�� �����, ��� ������ �������-\
%d\n", key, tree->treeData.data);
            *data = tree->treeData.data;
            return *data;
        }
        searchForData(tree->left, key, data); /*"������" � ��� �������� �����*/
        searchForData(tree->right, key, data); /*"������" � ����� �������� �����*/
    }
    return *data;
}

/*
������� ���������� �������� � ������
����:
    dsTree - ������ ����������� ������
    entry - ������ ����� ���� DataAndKeys � ������ ��� �����������
�����:
    insertTree = 0 - ���� ������� ��������� � ������
               = -1 - ���� ������� �� ��������� � ������
               = -2 - ���� ��������� ��������� ���������� ������
*/
int insertTree(BTDescriptor* dsTree, DataAndKeys* entry)
{
    if (dsTree == NULL)
    {
        return -2; //���� ��������� ��������� ���������� ������
    }
    if (dsTree->root == NULL) //�������� �� ����������� ������
    {
        if (!(dsTree->root = initializeNode(entry)))
        {
            return -1; //�� ������� ������������ �����
        }
        ++dsTree->size;
        return 0;
    }

    BTNode* subTree = dsTree->root;

    while (subTree != NULL) /*����� ���� ����� ���������� ������ �������� �� ��������
��������� �����*/
    {
        if (compareDataAndKeys(&subTree->treeData, entry) < 0)
        {
            if (subTree->left == NULL)
            {
                if (!(subTree->left = initializeNode(entry)))
                {
                    return -1; //�� ������� ������������ �����
                }
                ++dsTree->size;
                break;
            }
            subTree = subTree->left;
        }
        else if (compareDataAndKeys(&subTree->treeData, entry) > 0)
        {
            if (subTree->right == NULL)
            {
                if (!(subTree->right = initializeNode(entry)))
                {
                    return -1; //�� ������� ������������ �����
                }
                ++dsTree->size;
                break;
            }
            subTree = subTree->right;
        }
        else
        {
            subTree->treeData.data = entry->data;
            break;
        }
    }
    return 0;
}

/*
������� ��������� ���'�� ��� ������ ������������ ����
����:
    dAK - ������ ������ ���� DataAndKeys
    n - ����� � ��������� ������� �������� � �����
�����:
    freeStruct = 0 - ���� ���'��� �� ����� ��������
               = -1 - ���� ������ �� ����
*/
int freeStruct(DataAndKeys* dAK, int n)
{
    if (!dAK)
    {
        return -1; //������ ������������ ���� �� ����
    }
    for (int i = 0; i < n; i++)
    {
        free(dAK[i].key); //��������� ���'�� �� ���� key ������ dAK
    }
    free(dAK); //��������� ���'�� �� ����� dAK
    return 0;
}

/*
���������� ������� ������ �� ����� �������� ������
����:
    root - ������ ������� ������
    prev - ������ ����� ��� ���������� �'������ �������� ������
    isLeft - ����� ��� ���������� ��������� ����� �������
�����:
    printTree = 0 - ���� ������ �������� �� �����
              = -1 - ���� �� ������ ������� ������ �� �����
*/
int printTree(BTNode* root, Trunk* prev, bool isLeft)
{
    if (root == nullptr) {
        return 0;
    }

    char prev_str[] = "    ";
    Trunk* trunk = NULL;
    if (!(trunk = (Trunk*)malloc(sizeof(Trunk))))
    {
        return -1; //����������� ���'�� ��� �������� �� �� ����� ���� Trunk
    }
    trunk->prev = prev;
    trunk->str = prev_str;

    printTree(root->right, trunk, true); /*"������" � ����� �������� �����*/

    if (!prev) {
        strcpy(trunk->str, "----");
    }
    else if (isLeft)
    {
        strcpy(trunk->str, ".---");
        strcpy(prev_str, "   |");
    }
    else {
        strcpy(trunk->str, "`---");
        prev->str = prev_str;
    }

    showTrunks(trunk); //�������� �� ����� �'������� �������� ������
    printf("  %s\n", root->treeData.key);

    if (prev) {
        prev->str = prev_str;
    }
    strcpy(trunk->str, "   |");

    printTree(root->left, trunk, false);  /*"������" � ��� �������� �����*/
    free(trunk); //��������� ���'��� �� ����� ���� Trunk
    return 0;
};

/*
���������� ������� ������ �� ����� ������ � ������� ��������� �������� ��������
����:
    root - ������ ������� ������
    prev - ������ ����� ��� ���������� �'������ �������� ������
    isLeft - ����� ��� ���������� ��������� ����� �������
    key - ������ ����� � ������ �������� ��������
�����:
    showCurrentElementsPlace = 0 - ���� ������ �������� �� ����� �� ��������� ��������
 �������� ��������
                             = -1 - ���� �� ������ ������� ������ �� �����
*/
int showCurrentElementsPlace(BTNode* root, Trunk* prev, bool isLeft, char* key)
{
    if (root == nullptr) {
        return 0;
    }
    char prev_str[] = "    ";
    Trunk* trunk = NULL;
    if (!(trunk = (Trunk*)malloc(sizeof(Trunk))))
    {
        return -1; //����������� ���'�� ��� �������� �� �� ����� ���� Trunk
    }
    trunk->prev = prev;
    trunk->str = prev_str;

    showCurrentElementsPlace(root->right, trunk, true, key); /*"������" � �����
�������� �����*/

    if (!prev) {
        strcpy(trunk->str, "----");
    }
    else if (isLeft)
    {
        strcpy(trunk->str, ".---");
        strcpy(prev_str, "   |");
    }
    else {
        strcpy(trunk->str, "`---");
        prev->str = prev_str;
    }

    showTrunks(trunk); //�������� �� ����� �'������� �������� ������
    if (strcmp(key, root->treeData.key) == 0)
    {
        printf("  %s<---\n", root->treeData.key);
    }
    else
    {
        printf("  %s\n", root->treeData.key);
    }

    if (prev) {
        prev->str = prev_str;
    }
    strcpy(trunk->str, "   ");

    showCurrentElementsPlace(root->left, trunk, false, key); /*"������" � ���
�������� �����*/
    free(trunk); //��������� ���'��� �� ����� ���� Trunk
    return 0;
};

/*
������� ��� ���������� ����� � ����� � ������ �� �� ������ ������ ���� DataAndKeys
����:
    fp - �������� ����������
    size - ����� � ������� �������� � ����
    dAK - ������ ������ ���� DataAndKeys
�����:
    readFromFile = -1 - ���� �� ������� ���������� �������� � �����
                 = -2 - ���� ���� �������� � ���� �� ������� ������ 8
    ��������
                 = -3 - ���� �� ������ ���������� �������� �� ������� �����
                 = 0 - ���� �������� � ����� ������ ���������
*/
int readFromFile(FILE* fp, int size, DataAndKeys* dAK)
{
    char* data = (char*)malloc(102 * sizeof(char)); /*�������� ���'�� �� ����� ���
�������� ������ ����� � ����� �� �����*/
    if (!data)
    {
        return -1;
    }
    for (int i = 0; i < size; i++) { /*������� ����� ����� �� ������ ������������
   ���� dAK*/
        dAK[i].key = (char*)malloc(256 * sizeof(char)); /*�������� ���'�� �� �����
��� �������� ����� � ����� �� �����*/
        if (!dAK[i].key)
        {
            free(data);
            return -1;
        }
        fscanf(fp, "%s", dAK[i].key);
        if (strlen(dAK[i].key) > 8)
        {
            free(data);
            for (int j = 0; j < i; j++)
            {
                free(dAK[i].key);
            }
            return -2;
        }
        if (strlen(dAK[i].key) < 8)
        {
            free(data);
            for (int j = 0; j < i; j++)
            {
                free(dAK[i].key);
            }
            return -2;
        }
        fscanf(fp, "%s", data);
        dAK[i].data = 0;

        for (int j = strlen(data) - 1, g = 0; j >= 0; j--, g++)
        {
            if (data[j] == '-')
            {
                dAK[i].data = dAK[i].data * (-1);
            }
            else
            {
                dAK[i].data = (int)(dAK[i].data + (data[j] - 48) * (int)pow(10, g));
            }
        }
    }
    rewind(fp);
    if (ferror(fp)) {
        free(data); //��������� ���'�� �� ����� data
        for (int i = 0; i < size; i++)
        {
            free(dAK[i].key);
        }
        return -3;
    }
    free(data); //��������� ���'�� �� ����� data
    return 0;
}

/*
������� ������ �������� ������ ���� DataAndKeys �� �����
����:
    fp - �������� ����������
    size - ����� � ������� �������� � ����
    dAK - ������ ������ ���� DataAndKeys
�����:
    writeToFile = -1 - ���� �� ������� ���������� ����� �� �����
                = 0 - ���� ����� ������ ����������� �� �����
*/
int writeToFile(FILE* fp, int size, DataAndKeys* dAK)
{
    for (int i = 0; i < size; i++) //����� �������� �� �����
    {
        if (!fprintf(fp, "%s ", dAK[i].key))
        {
            return -1;
        }

        if (i == size - 1)
        {
            if (!fprintf(fp, "%d", dAK[i].data))
            {
                return -1;
            }
        }
        else
        {
            if (!fprintf(fp, "%d\n", dAK[i].data))
            {
                return -1;
            }
        }
    }
    return 0;
}

/*
������� ������ �������� ������ ���� DataAndKeys �� �����
����:
    number - ����� ��� �����������
    key - ���� ��� ������
�����: �������
*/
void setAKey(int number, char* key)
{
    int k = 0, counter = 0;
    if (number < 0)
    {
        key[counter] = '-';
        counter++;
    }
    for (int c = 7;c >= 0;c--, counter++)
    {
        k = number >> c;
        if (k & 1)
        {
            key[counter] = '1';
        }
        else
        {
            key[counter] = '0';
        }
    }
    if (number < 0)
    {
        key[9] = '\0'; //������������ ���� ����� ��� �����
    }
    else
    {
        key[8] = '\0'; //������������ ���� ����� ��� �����
    }
}

/*
������� ��� �������� ����������� �������� ����� �����
����:
    filename - ����� � �������� ������������ ����� �����
�����: �������
*/
void checkFilename(char filename[]) {
    int fcheck = 0; //����� ��� �������� ����������� ����� ����� ����� ������������
    int scheck = 0; //����� ��� �������� ����������� ����� ����� ����� ������������
    do {
        fgets(filename, 1024, stdin);
        fcheck = 1;
        scheck = 0;
        for (int i = 0; i < strlen(filename); i++)
        {
            if (filename[i] == '.' && i != 0)
            {
                scheck = 1;
                if (filename[i + 1] != 'b' || filename[i + 2] != 'i' ||
                    filename[i + 3] != 'n' || filename[i + 4] != '\n')
                {
                    printf("�� ����� ���������� �� ����, ��������� �� ���!\n");
                    fcheck = 0;
                }
            }
        }
        if (filename[0] == '\n')
        {
            printf("�� ����� �� �����, ��������� �� ���!\n");
        }

        if (scheck == 0 && filename[0] != '\n')
        {
            printf("�� �� ����� ����������, ��������� �� ���!\n");
        }
    } while (filename[0] == '\n' || fcheck == 0 || scheck == 0);
    filename[strlen(filename) - 1] = '\0'; //����� \n �� \0
}

/*
������� ���������� ������ ������
����:
    tree - ������ ������� ������
    height - ������ ����� � ������� ��������, ���� ����� �����������
    curMaxHeight - ������ ����� � �������� ������������ ������� ������
    maxHeight - ������ ����� � ������������ ������� ������
�����: �������
*/
void getHeight(BTDescriptor* dsTree, BTNode* tree, int* height, int* curMaxHeight, int* maxHeight)
{
    if (dsTree == NULL)
    {
        return;
    }

    if (tree != NULL) { /*���� ������� ����*/
        *height = *height + 1;
        getHeight(dsTree, tree->left, height, curMaxHeight, maxHeight); /*"������" �
��� �������� �����*/
        *height = *height + 1;
        getHeight(dsTree, tree->right, height, curMaxHeight, maxHeight); /*"������"
� ����� �������� �����*/
    }
    else
    {
        *curMaxHeight = *height;
        if (*curMaxHeight > *maxHeight)
        {
            *maxHeight = *curMaxHeight;
        }
    }
    *height = *height - 1;
    return;
}

/*
������� ����������� ������ � ������� �������
����:
    tree - ������ ������� ������
    j - ������ ����� � ������� ��������� �������� ������
�����: �������
*/
void preOrder(BTNode* tree, int* j) {
    if (tree != NULL) { /*���� ������� ����*/
        *j = *j + 1;
        printf("\n������� �%d:\nֳ�� �����, ��� ������ �������-%d\n���� �������-\
%s\n", *j, tree->treeData.data, tree->treeData.key); /*������� ����� ��
������ ������*/
        preOrder(tree->left, j); /*"������" � ��� �������� �����*/
        preOrder(tree->right, j); /*"������" � ����� �������� �����*/
    }
    return;
}

/*
������� ����������� ������ � ����������� �������
����:
    tree - ������ ������� ������
    j - ������ ����� � ������� ��������� �������� ������
�����: �������
*/
void postOrder(BTNode* tree, int* j) {
    if (tree != NULL) { /*���� ������� ����*/
        postOrder(tree->left, j); /*"������" � ��� �������� �����*/
        postOrder(tree->right, j); /*"������" � ����� �������� �����*/
        *j = *j + 1;
        printf("\n������� �%d:\nֳ�� �����, ��� ������ �������-%d\n���� �������-\
%s\n", *j, tree->treeData.data, tree->treeData.key); /*������� ����� ��
������ ������*/
    }
    return;
}

/*
������� ������������ ����������� ������
����:
    tree - ������ ������� ������
    j - ������ ����� � ������� ��������� �������� ������
�����: �������
*/
void inOrder(BTNode* tree, int* j) {
    if (tree != NULL) { /*���� ������� ����*/
        inOrder(tree->left, j); /*"������" � ��� �������� �����*/
        *j = *j + 1;
        printf("\n������� �%d:\nֳ�� �����, ��� ������ �������-%d\n���� �������\
%s\n", *j, tree->treeData.data, tree->treeData.key); /*������� ����� ��
������ ������*/
        inOrder(tree->right, j); /*"������" � ����� �������� �����*/
    }
    return;
}

/*
������� �������� ���������� �����
����:
    key - ����� � ������, ���� ���������� ��� � ���������
�����: �������
*/
void keyScan(char* key)
{
    int entered = 0; //����� ��� �������� �������� ������������ � ��������� �������
    int lCheck = 1; /*����� ��� �������� ������� �������� ������������ � ���������
�������*/
    do {
        do {
            entered = scanf("%s", key);
            while (getchar() != '\n');
            while (entered < 1) {
                printf("�� ����� ����������� ����, �������� ��������: ");
                entered = scanf("%s", key);
                while (getchar() != '\n');
            }
        } while (entered < 1);

        if (strlen(key) < 8)
        {
            printf("�� ����� ���� � ������ ������� �������, �� �������,\
 �������� ��������:\n");
            lCheck = 0;
        }
        else if (strlen(key) > 8)
        {
            printf("�� ����� ���� � ������ ������� �������, �� �������,\
 �������� ��������:\n");
            lCheck = 0;
        }
        else
        {
            lCheck = 1;
        }
    } while (entered != 1 || lCheck == 0);
}

/*
���������� ������� ������ �� ����� �'������ �������� ������
����:
    trunk - ������ ����� ���� Trunk
�����: �������
*/
void showTrunks(Trunk* trunk)
{
    if (trunk == nullptr) {
        return;
    }

    showTrunks(trunk->prev);
    printf("%s", trunk->str);
}

/*
������� ��������� ������
����:
    tree - ������ ������� ������
�����: �������
*/
void deleteTree(BTNode* tree) {
    if (tree != NULL) { /*���� ������� ����*/
        deleteTree(tree->left); /*"������" � ��� �������� �����*/
        deleteTree(tree->right); /*"������"� ����� �������� �����*/
        free(tree->treeData.key); //��������� ���'�� �� ���� key �����
        free(tree); //��������� ���'�� �� �����
    }
    return;
}