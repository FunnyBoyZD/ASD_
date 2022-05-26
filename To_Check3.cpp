#define _CRT_SECURE_NO_WARNINGS
#include "stdlib.h"
#include "stdio.h"
#include "locale.h"
#include "windows.h"
#include "time.h"
#include <math.h>
#include "assert.h"
#define Distance 15

/*��������� ���������*/
struct Coordinates
{
    int x, //���������� x
        y, //���������� y
        z; //���������� z
};

/*��������� � ������ � ����� ������ ����� ������*/
struct DataAndKeys {
    Coordinates data; /*���������� ����� �������*/
    float key; /*���� �������*/
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
BTNode* deleteElement(BTNode* subTree, double key);
float calculateTheKey(int x, int y, int z);
float doubleNumberScan(float& N);
int numberScan(int& N);
int fopenCheck(FILE*& fp, char filename[], char d);
int elementCounter(FILE*& fd);
int getDataAndKeys(BTNode* tree, DataAndKeys* dAK, int* j);
int keyUsingSearch(BTNode* tree, double key, int* noElementCheck);
int insertTree(BTDescriptor* dsTree, DataAndKeys* entry);
int printTree(BTNode* root, Trunk* prev, bool isLeft);
int showCurrentElementsPlace(BTNode* root, Trunk* prev, bool isLeft, double key);
int readFromFile(FILE* fp, int size, DataAndKeys* dAK);
int writeToFile(FILE* fp, int size, DataAndKeys* dAK);
void checkFilename(char filename[]);
void getHeight(BTDescriptor* dsTree, BTNode* tree, int* height, int* curMaxHeight, int* prevMaxHeight);
void preOrder(BTNode* tree, int* j);
void postOrder(BTNode* tree, int* j);
void inOrder(BTNode* tree, int* j);
void showTrunks(Trunk* trunk);
void deleteTree(BTNode* tree);

int main()
{
    FILE* fr = NULL; //��� ������������ ������ � ������
    int workWithBT; //��� ������ ����
    int counter = 0; //��� ������� �������� � �����
    int minimum = 0; //��� ������ ������� ����� ��������� ���������� �����
    int maximum = 0; //��� ������� ������� ����� ��������� ���������� �����
    int noElementCheck = 1; //��� �������� �� �������� �������� � �����
    int elementNumber = 0; //��� ��������� ���� ��������� ������������� �����
    char filename[50]; //� ������ �����
    bool flag = true; //��� ���������� ����� ������ ����
    float forKey; //� ������, ���� ���������� ������� � ���������
    int height = 0; //��� ������ ������
    int curMaxHeight = 0; /*��� ������� ����������� ������ ������ ��� ����������
����*/
    int maxHeight = 0; //��� ����������� ������ ������ ��� ���������� ����
    int count = 0; //��� ������� �������� � ����� ������������ ����
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
 ����;\n3)��������� ������ ����������� ����������.\n");
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
            if (readFromFile(fr, counter, dAK) == -1) /*���� �� ������ ���������� �������� ��
������� �����*/
            {
                printf("�� ������ ���������� �������� �� ������� �����!\n");
                free(dAK);
            }
            for (int i = 0; i < counter; i++) { /*����������� �������� � ������
������������ ���� �� ������*/
                insertCheck = insertTree(BTd, &dAK[i]); /*���������� �������� ��
������*/
                if (insertCheck == -1) //�� ������� �������� ������� � ������
                {
                    printf("�� ������� �������� ������� � ������!\n");
                    fclose(fr); //�������� ������ ������ � ������
                    free(dAK);
                    exit(0);
                }
                else if (insertCheck == -2) /*���������� ������� ������ ����������
�����������*/
                {
                    printf("�� �������� ��������� ���������� ������!\n");
                    fclose(fr); //�������� ������ ������ � ������
                    free(dAK);
                    exit(0);
                }
            }
            fclose(fr); //�������� ������ ������ � ������
            free(dAK);
            break;
        }
        case 2: { //��������� ������ �����
            printf("������ ����� ����� � ������ name.bin ��� �������� ������ �\
 ���:\n");
            checkFilename(filename); //�������� ����������� �������� ����� �����
            break;
        }
        case 3: { //���������� ������ ����������� ����������
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
                dAK[i].data.x = minimum + rand() % (maximum - minimum + 1); /*���������
��������� ���������� �*/
                if (dAK[i].data.x < minimum || dAK[i].data.x > maximum) /*�������� �� ��,
�� ��������� ����������� ��������� ���������� �*/
                {
                    printf("��������� ���������� � ����������� ������!\n");
                    free(dAK);
                    exit(0);
                }
                dAK[i].data.y = minimum + rand() % (maximum - minimum + 1); /*���������
��������� ���������� �*/
                if (dAK[i].data.y < minimum || dAK[i].data.y > maximum) /*�������� �� ��,
�� ��������� ����������� ��������� ���������� y*/
                {
                    printf("��������� ���������� y ����������� ������!\n");
                    free(dAK);
                    exit(0);
                }
                dAK[i].data.z = minimum + rand() % (maximum - minimum + 1); /*���������
��������� ���������� z*/
                if (dAK[i].data.z < minimum || dAK[i].data.z > maximum) /*�������� �� ��,
�� ��������� ����������� ��������� ���������� z*/
                {
                    printf("��������� ���������� z ����������� ������!\n");
                    free(dAK);
                    exit(0);
                }
                dAK[i].key = calculateTheKey(dAK[i].data.x, dAK[i].data.y, dAK[i].data.z);
/*������� �����, �������� �� ���������� �������� �������*/
            }
            for (int i = 0; i < counter; i++) /*����������� �������� � ������
������������ ���� �� ������*/
            {
                if (insertTree(BTd, &dAK[i]) == -1) /*���������� �������� �� ������ ��
�������� �� ��, �� �� �������*/
                {
                    printf("�� ������� �������� ������� � ������!\n");
                    free(dAK);
                    exit(0);
                }
            }
            free(dAK);
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
            if (readFromFile(fr, counter, dAK) == -1) /*���� �� ������ ���������� �������� ��
������� �����*/
            {
                printf("�� ������ ���������� �������� �� ������� �����!\n");
                free(dAK);
            }
            for (int i = 0; i < counter; i++) { /*����������� �������� � ������
������������ ���� �� ������*/
                insertCheck = insertTree(BTd, &dAK[i]); /*���������� �������� ��
������*/
                if (insertCheck == -1) //�� ������� �������� ������� � ������
                {
                    printf("�� ������� �������� ������� � ������!\n");
                    free(dAK);
                    fclose(fr); //�������� ������ ������ � ������
                    exit(0);
                }
                else if (insertCheck == -2) /*���������� ������� ������ ����������
�����������*/
                {
                    printf("�� �������� ��������� ����������!\n");
                    free(dAK);
                    fclose(fr); //�������� ������ ������ � ������
                    exit(0);
                }
            }
            free(dAK);
            fclose(fr); //�������� ������ ������ � ������
            continue;
        }
        switch (workWithBT) {
        case 1: //��������� �� ����� ��������
        {
            noElementCheck = 1;
            printf("������ ���� �������� ������, ���� ������ ������� �� �����:\n");
            doubleNumberScan(forKey); //���������� ��������� � ��������� �����
            if (keyUsingSearch(BTd->root, forKey, &noElementCheck) == 1) /*����� �������� � ����� �� ������*/
            {
                printf("�������� � ����� ������ ���� � �����!\n");
            }
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
            printf("������ ���������� � ��������, ���� ������ �������� � ������:\n");
            numberScan(elementNumber); /*���������� ��������� � ��������� ������ �����*/
            dAK->data.x = elementNumber;
            printf("������ ���������� y ��������, ���� ������ �������� � ������:\n");
            numberScan(elementNumber); /*���������� ��������� � ��������� ������ �����*/
            dAK->data.y = elementNumber;
            printf("������ ���������� z ��������, ���� ������ �������� � ������:\n");
            numberScan(elementNumber); /*���������� ��������� � ��������� ������ �����*/
            dAK->data.z = elementNumber;
            dAK->key = calculateTheKey(dAK->data.x, dAK->data.y, dAK->data.z);/*������� �����, �������� �� ���������� �������� �������*/
            insertCheck = insertTree(BTd, dAK); //���������� �������� �� ������
            if (insertCheck == -1) //�� ������� �������� ������� � ������
            {
                printf("�� ������� �������� ������� � ������!\n");
                free(dAK); //��������� ���'�� ��� ����� ������������ ����
                exit(0);
            }
            else if (insertCheck == -2) /*���������� ������� ������ ����������
�����������*/
            {
                printf("�� �������� ��������� ���������� ������!\n");
                free(dAK); //��������� ���'�� ��� ����� ������������ ����
                exit(0);
            }
            counter++;
            free(dAK); //��������� ���'�� ��� ����� ������������ ����
            break;
        }
        case 3: //���������� �������� � ������
        {
            printf("������ ���� �������� ������, ���� ������ ��������� � ������:\n");
            doubleNumberScan(forKey); //���������� ��������� � ��������� �����
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
            BTd->size--;
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
            noElementCheck = 1;
            printf("������ ����(���������������� ��� � 10 �������) ��������,\
 ��������� ����� ������ �������� � �����:\n");
            doubleNumberScan(forKey); //���������� ��������� � ��������� �����
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
                free(dAK);
                exit(0);
            }
            if (writeToFile(fr, count, dAK) == -1) /*������������� ������ ������������
���� �� ����� �� �������� �� ��, �� �� �������*/
            {
                printf("�� ������� ���������� ����� �� �����!\n");
                free(dAK);
                exit(0);
            }
            free(dAK);
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
    newNode->treeData.data = entry->data;
    newNode->treeData.key = entry->key;
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
BTNode* deleteElement(BTNode* subTree, double key) {
    //������� ����������� ������
    if (!subTree)
    {
        return subTree;
    }

    //���� ���� �������� �������� ������
    //�� ���� ��������, � ���� �� ����� ��������� ����
    if (key < subTree->treeData.key)
    {
        subTree->left = deleteElement(subTree->left, key);
    }

    //���� ���� �������� �������� ������
    //�� ���� ��������, � ���� �� ����� ��������� ����
    else if (key > subTree->treeData.key)
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
        subTree->treeData.key = temp->treeData.key;
        subTree->treeData.data = temp->treeData.data;

        //��������� ���������� �������
        subTree->right = deleteElement(subTree->right, temp->treeData.key);
    }
    return subTree;
}

/*
������� ������������� ������� �����-������� �� ����� � �������� ������������
����:
    x - ���������� � �����
    y - ���������� y �����
    z - ���������� z �����
�����:
    calculateTheKey = <�������� ������� �����-������� �� ����� � �������� ������������> - ��� �������� ����������� ��������
*/
float calculateTheKey(int x, int y, int z)
{
    return (float)sqrt((double)x * (double)x + (double)y * (double)y + (double)z * (double)z);
}

/*������� �������� ������� ������
����:
    N - ������ ����� ��� �����������
�����:
    numberScan = N - ������������ �����
*/
float doubleNumberScan(float& N) {
    int entered = 0; //����� ��� �������� �������� ������������ � ��������� �������
    do {
        do {
            entered = scanf_s("%f", &N);
            while (getchar() != '\n');
            while (entered < 1) {
                printf("�� ����� ���������� ��������, �������� ��������: ");
                entered = scanf_s("%f", &N);
                while (getchar() != '\n');
            }
        } while (entered < 1);
    } while (entered != 1);
    return N;
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
        fscanf(fd, "%*d");
        fscanf(fd, "%*d");
        fscanf(fd, "%*d");
        fscanf(fd, "%*lf");
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
        dAK[*j].data = tree->treeData.data;
        dAK[*j].key = tree->treeData.key;
        *j = *j + 1;
        getDataAndKeys(tree->left, dAK, j); /*"������" � ��� �������� �����*/
        getDataAndKeys(tree->right, dAK, j); /*"������" � ����� �������� �����*/
    }
    return 0;
}

/*
������� ������ �������� �� ������ ��� ��������� ���� �� �����
����:
    tree - ������ ������� ������
    key - ���� ������� ������
    noElementCheck - ������ ����� ��� �������� �������� �������� � �����
�����:
    keyUsingSearch = noElementCheck = 0 - ���� ������� � ������, �� ���� ���������
                             �����, � � �����
                                    = 1 - ���� �������� � ������, �� ���� ���������
        �����, ���� � �����
*/
int keyUsingSearch(BTNode* tree, double key, int* noElementCheck) {
    if (tree != NULL) { /*���� ������� ����*/
        if (tree->treeData.key == key)
        {
            printf("������� ������ � ������ %lf:\n���������� �����, �� ������ �������:\
\nx-%d\ny-%d\nz-%d\n", key, tree->treeData.data.x, tree->treeData.data.y, tree->treeData.data.z);
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
        if (subTree->treeData.key > entry->key)
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
        else if (subTree->treeData.key < entry->key)
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
    printf("  %lf\n", root->treeData.key);

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
    key - ���� �������� ��������
�����:
    showCurrentElementsPlace = 0 - ���� ������ �������� �� ����� �� ��������� ��������
 �������� ��������
                             = -1 - ���� �� ������ ������� ������ �� �����
*/
int showCurrentElementsPlace(BTNode* root, Trunk* prev, bool isLeft, double key)
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
    if (key == root->treeData.key)
    {
        printf("  %lf<---\n", root->treeData.key);
    }
    else
    {
        printf("  %lf\n", root->treeData.key);
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
    readFromFile = -1 - ���� �� ������ ���������� �������� �� ������� �����
                 = 0 - ���� �������� � ����� ������ ���������
*/
int readFromFile(FILE* fp, int size, DataAndKeys* dAK)
{
    for (int i = 0; i < size; i++) { /*������� ����� ����� �� ������ ������������
   ���� dAK*/
        fscanf(fp, "%d", &dAK[i].data.x);
        fscanf(fp, "%d", &dAK[i].data.y);
        fscanf(fp, "%d", &dAK[i].data.z);
        fscanf(fp, "%f", &dAK[i].key);
    }
    rewind(fp);
    if (ferror(fp)) {
        return -1;
    }
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
        if (!fprintf(fp, "%d ", dAK[i].data.x))
        {
            return -1;
        }
        if (!fprintf(fp, "%d ", dAK[i].data.y))
        {
            return -1;
        }
        if (!fprintf(fp, "%d ", dAK[i].data.z))
        {
            return -1;
        }

        if (i == size - 1)
        {
            if (!fprintf(fp, "%lf", dAK[i].key))
            {
                return -1;
            }
        }
        else
        {
            if (!fprintf(fp, "%lf\n", dAK[i].key))
            {
                return -1;
            }
        }
    }
    return 0;
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
        printf("\n������� �%d:\n���������� �������� �������:\n\
x-%d\ny-%d\nz-%d\n���� �������-%lf\n", *j, tree->treeData.data.x,
tree->treeData.data.y, tree->treeData.data.z, tree->treeData.key);
/*������� ����� �� ������ ������*/
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
        printf("\n������� �%d:\n���������� �������� �������:\n\
x-%d\ny-%d\nz-%d\n���� �������-%lf\n", *j, tree->treeData.data.x,
tree->treeData.data.y, tree->treeData.data.z, tree->treeData.key);
/*������� ����� �� ������ ������*/
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
        printf("\n������� �%d:\n���������� �������� �������:\n\
x-%d\ny-%d\nz-%d\n���� �������-%lf\n", *j, tree->treeData.data.x,
tree->treeData.data.y, tree->treeData.data.z, tree->treeData.key);
/*������� ����� �� ������ ������*/
        inOrder(tree->right, j); /*"������" � ����� �������� �����*/
    }
    return;
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
        free(tree); //��������� ���'�� �� �����
    }
    return;
}