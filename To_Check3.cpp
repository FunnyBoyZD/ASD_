#define _CRT_SECURE_NO_WARNINGS
#include "stdlib.h"
#include "stdio.h"
#include "locale.h"
#include "windows.h"
#include "time.h"
#include <math.h>
#define Distance 15

/*Структура координат*/
struct Coordinates
{
    int x, //координата x
        y, //координата y
        z; //координата z
};

/*Структура з ключем і цілим числом вузла дерева*/
struct DataAndKeys {
    Coordinates data; /*координати точки вершини*/
    float key; /*ключ вершини*/
};

/*Структура вузла дерева*/
struct BTNode { /*вершина дерева*/
    struct BTNode* left; /*вказівник на лівий нащадок*/
    DataAndKeys treeData; /*ключ і ціле число вершини*/
    struct BTNode* right; /*вказівник на правий нащадок*/
};

/*Формат дескриптора дерева*/
struct BTDescriptor { /*дескриптор дерева*/
    int size; /*число вершин*/
    BTNode* root; /*корінь дерева*/
};

/*Структура стовбура дерева*/
struct Trunk
{
    Trunk* prev; //попередній елемент
    char* str; //для друку з'єднань елементів дерева
};

/*Прототипи функцій*/
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
    FILE* fr = NULL; //для встановлення потоку з файлом
    int workWithBT; //для пункту меню
    int counter = 0; //для кількості елементів у дереві
    int minimum = 0; //для нижньої границі задля генерації випадкових чисел
    int maximum = 0; //для верхньої границі задля генерації випадкових чисел
    int noElementCheck = 1; //для перевірки на наявність елементу в дереві
    int elementNumber = 0; //для отримання коду випадково згенерованого числа
    char filename[50]; //з назвою файлу
    bool flag = true; //для повторного вводу пункту меню
    float forKey; //з ключем, який користувач вводить з клавіатури
    int height = 0; //для висоти дерева
    int curMaxHeight = 0; /*для поточної максимальної висоти дерева при проходженні
його*/
    int maxHeight = 0; //для максимальної висоти дерева при проходженні його
    int count = 0; //для кількості елементів у масиві структурного типу
    int insertCheck = 0; //для перевірки на помилки при вставленні елемента
    int fOpenChecker = 0; /*для перевірки на помилки при встановленні потоку з файлом*/

    SetConsoleCP(1251); //підключення кирилиці
    SetConsoleOutputCP(1251);

    srand((unsigned int)time(NULL));
    BTDescriptor* BTd = createTree(); //створюємо дерево

    if (BTd == NULL) { //перевірка на те, чи створене дерево
        printf("Дерево не створено!");
        return -1;
    }

    printf("Дерево створено!\n");

    do {
        printf("1)Заповнити дерево з існуючого файлу;\n2)Створити новий\
 файл;\n3)Заповнити дерево випадковими елементами.\n");
        numberScan(workWithBT); //зчитування пункту меню
        switch (workWithBT) {
        case 1: { //заповнення дерева з існуючого файлу
            printf("Введіть назву файлу у вигляді name.bin, з якого бажаєте зчитати\
 дані:\n");
            checkFilename(filename); //перевірка правильності введення назви файла
            fOpenChecker = fopenCheck(fr, filename, 'r'); /*встановлення потоку для роботи з файлом у режимі зчитування, перевірка на помилки та на те, чи потік установився*/
            if (fOpenChecker == -1) //не вдалося відкрити файл
            {
                printf("Не вдалося відкрити файл із заданою назвою!\n");
                exit(0);
            }
            else if (fOpenChecker == -2) /*користувач передав некоректний символ для задання типу роботи з файлом*/
            {
                printf("Ви передали некоректний символ для задання типу роботи з файлом!\n");
                exit(0);
            }
            counter = elementCounter(fr); //підрахунок кількості елементів у файлі
            if (counter == -1)
            {
                printf("Помилка при переміщенні показника на початок файлу!\n");
                exit(0);
            }
            DataAndKeys* dAK = (DataAndKeys*)malloc((counter) * sizeof(DataAndKeys));
            //виділення пам'яті для масиву структурного типу
            if (!dAK) /*перевірка на те, чи пам'ять виділена під масив структурного типу*/
            {
                printf("Недостатньо пам'яті для виділення її під масив\n!");
                fclose(fr); //закриття потоку роботи з файлом
                exit(0);
            }
            if (readFromFile(fr, counter, dAK) == -1) /*якщо не вийшло перемістити показник на
початок файлу*/
            {
                printf("Не вийшло перемістити показник на початок файлу!\n");
                free(dAK);
            }
            for (int i = 0; i < counter; i++) { /*перенесення елементів з масиву
структурного типу до дерева*/
                insertCheck = insertTree(BTd, &dAK[i]); /*вставлення елементу до
дерева*/
                if (insertCheck == -1) //не вдалося вставити елемент у дерево
                {
                    printf("Не вдалося вставити елемент у дерево!\n");
                    fclose(fr); //закриття потоку роботи з файлом
                    free(dAK);
                    exit(0);
                }
                else if (insertCheck == -2) /*користувач передав адресу неіснуючого
дескриптору*/
                {
                    printf("Ви передали неіснуючий дескриптор дерева!\n");
                    fclose(fr); //закриття потоку роботи з файлом
                    free(dAK);
                    exit(0);
                }
            }
            fclose(fr); //закриття потоку роботи з файлом
            free(dAK);
            break;
        }
        case 2: { //створення нового файлу
            printf("Введіть назву файлу у вигляді name.bin для подальшої роботи з\
 ним:\n");
            checkFilename(filename); //перевірка правильності введення назви файла
            break;
        }
        case 3: { //заповнення дерева випадковими елементами
            printf("Введіть назву файлу у вигляді name.bin для подальшої роботи з\
 ним:\n");
            checkFilename(filename); //перевірка правильності введення назви файла
            do {
                printf("Введіть кількість елементів дерева:\n");
                numberScan(counter); //зчитування пункту меню
                if (counter < 1) {
                    printf("Введене некоректне значення! Спробуйте ще раз!\n");
                }
            } while (counter < 1);
            printf("Введіть нижню границю для генерації випадкових чисел:\n");
            numberScan(minimum); /*зчитування введеного до консолі значення для
нижньої границі задля генерації випадкових чисел*/
            printf("Введіть верхню границю для генерації випадкових чисел:\n");
            numberScan(maximum); /*зчитування введеного до консолі значення для
верхньої границі задля генерації випадкових чисел*/
            if (maximum < minimum) { /*перевірка на те, щоб верхня границя для
генерації випадкових чисел не була меншою за нижню*/
                printf("Верхня границя не може бути меншою за нижню!\n");
                exit(0);
            }
            DataAndKeys* dAK = (DataAndKeys*)malloc((counter) * sizeof(DataAndKeys));
            //виділення пам'яті для масиву структурного типу
            if (!dAK) /*перевірка на те, чи пам'ять виділена під масив структурного
типу*/
            {
                printf("Недостатньо пам'яті для виділення її під масив\n!");
                exit(0);
            }
            for (int i = 0; i < counter; i++) {
                dAK[i].data.x = minimum + rand() % (maximum - minimum + 1); /*генерація
випадкової координати х*/
                if (dAK[i].data.x < minimum || dAK[i].data.x > maximum) /*перевірка на те,
чи правильно згенерована випадкова координата х*/
                {
                    printf("Випадкова координата х згенерована невірно!\n");
                    free(dAK);
                    exit(0);
                }
                dAK[i].data.y = minimum + rand() % (maximum - minimum + 1); /*генерація
випадкової координати у*/
                if (dAK[i].data.y < minimum || dAK[i].data.y > maximum) /*перевірка на те,
чи правильно згенерована випадкова координата y*/
                {
                    printf("Випадкова координата y згенерована невірно!\n");
                    free(dAK);
                    exit(0);
                }
                dAK[i].data.z = minimum + rand() % (maximum - minimum + 1); /*генерація
випадкової координати z*/
                if (dAK[i].data.z < minimum || dAK[i].data.z > maximum) /*перевірка на те,
чи правильно згенерована випадкова координата z*/
                {
                    printf("Випадкова координата z згенерована невірно!\n");
                    free(dAK);
                    exit(0);
                }
                dAK[i].key = calculateTheKey(dAK[i].data.x, dAK[i].data.y, dAK[i].data.z);
/*задання ключа, зважаючи на координати елемента вершини*/
            }
            for (int i = 0; i < counter; i++) /*перенесення елементів з масиву
структурного типу до дерева*/
            {
                if (insertTree(BTd, &dAK[i]) == -1) /*вставлення елементу до дерева та
перевірка на те, чи це сталося*/
                {
                    printf("Не вдалося вставити елемент у дерево!\n");
                    free(dAK);
                    exit(0);
                }
            }
            free(dAK);
            break;
        }
        default: { //якщо користувач ввів некоректне значення
            printf("Ви ввели некоректне значення, спробуйте ще раз!\n");
            flag = 0;
            continue;
        }
        }
    } while (!flag);

    for (;;) {
        printf("\nВиберіть тип роботи з бінарним деревом цілих чисел:\n1)Вивести на\
 екран елемент;\n2)Вставити елемент у дерево;\n3)Виключити елемент з дерева\n\
4)Отримати розмір дерева та його висоту;\n5)Показ дерева на екрані;\n\
6)Поточне положення елемента в дереві;\n7)Переписати дерево до файлу;\n\
8)Зчитати дерево з файлу;\n9)Проходження дерева;\n0)Завершити програму;\n");
        numberScan(workWithBT); //зчитування пункту меню
        if (workWithBT == 8) //зчитування дерева з файлу
        {
            if (BTd != NULL)
            {
                deleteTree(BTd->root); //звільнення місця під дерево
                free(BTd); //звільнення місця під дескриптор дерева
                BTd = createTree(); //створюємо дерево
                if (BTd == NULL) { //перевірка на те, чи створене дерево
                    printf("Дерево не створено!");
                    return -1;
                }
            }
            fOpenChecker = fopenCheck(fr, filename, 'r'); /*встановлення потоку для роботи з файлом у режимі зчитування, перевірка на помилки та на те, чи потік установився*/
            if (fOpenChecker == -1) //не вдалося відкрити файл
            {
                printf("Не вдалося відкрити файл із заданою назвою!\n");
                exit(0);
            }
            else if (fOpenChecker == -2) /*користувач передав некоректний символ для задання типу роботи з файлом*/
            {
                printf("Ви передали некоректний символ для задання типу роботи з файлом!\n");
                exit(0);
            }
            counter = elementCounter(fr); //підрахунок кількості елементів у файлі
            DataAndKeys* dAK = (DataAndKeys*)malloc((counter) * sizeof(DataAndKeys));
            //виділення пам'яті для масиву структурного типу
            if (!dAK) /*перевірка на те, чи пам'ять виділена під масив структурного
типу*/
            {
                printf("Недостатньо пам'яті для виділення її під масив\n!");
                exit(0);
            }
            if (readFromFile(fr, counter, dAK) == -1) /*якщо не вийшло перемістити показник на
початок файлу*/
            {
                printf("Не вийшло перемістити показник на початок файлу!\n");
                free(dAK);
            }
            for (int i = 0; i < counter; i++) { /*перенесення елементів з масиву
структурного типу до дерева*/
                insertCheck = insertTree(BTd, &dAK[i]); /*вставлення елементу до
дерева*/
                if (insertCheck == -1) //не вдалося вставити елемент у дерево
                {
                    printf("Не вдалося вставити елемент у дерево!\n");
                    free(dAK);
                    fclose(fr); //закриття потоку роботи з файлом
                    exit(0);
                }
                else if (insertCheck == -2) /*користувач передав адресу неіснуючого
дескриптору*/
                {
                    printf("Ви передали неіснуючий дескриптор!\n");
                    free(dAK);
                    fclose(fr); //закриття потоку роботи з файлом
                    exit(0);
                }
            }
            free(dAK);
            fclose(fr); //закриття потоку роботи з файлом
            continue;
        }
        switch (workWithBT) {
        case 1: //виведення на екран елементу
        {
            noElementCheck = 1;
            printf("Введіть ключ елемента дерева, який бажаєте вивести на екран:\n");
            doubleNumberScan(forKey); //зчитування введеного з клавіатури ключа
            if (keyUsingSearch(BTd->root, forKey, &noElementCheck) == 1) /*пошук елементу в дереві за ключем*/
            {
                printf("Елементу з таким ключем немає в дереві!\n");
            }
            break;
        }
        case 2: //вставлення елемента в дерево
        {
            DataAndKeys* dAK = (DataAndKeys*)malloc(sizeof(DataAndKeys)); /*виділення пам'яті для змінної структурного типу*/
            if (!dAK) /*перевірка на те, чи пам'ять виділена під масив структурного
типу*/
            {
                printf("Недостатньо пам'яті для виділення її під масив\n!");
                exit(0);
            }
            printf("Введіть координату х елементу, який бажаєте вставити в дерево:\n");
            numberScan(elementNumber); /*зчитування введеного з клавіатури цілого числа*/
            dAK->data.x = elementNumber;
            printf("Введіть координату y елементу, який бажаєте вставити в дерево:\n");
            numberScan(elementNumber); /*зчитування введеного з клавіатури цілого числа*/
            dAK->data.y = elementNumber;
            printf("Введіть координату z елементу, який бажаєте вставити в дерево:\n");
            numberScan(elementNumber); /*зчитування введеного з клавіатури цілого числа*/
            dAK->data.z = elementNumber;
            dAK->key = calculateTheKey(dAK->data.x, dAK->data.y, dAK->data.z);/*задання ключа, зважаючи на координати елемента вершини*/
            insertCheck = insertTree(BTd, dAK); //вставлення елементу до дерева
            if (insertCheck == -1) //не вдалося вставити елемент у дерево
            {
                printf("Не вдалося вставити елемент у дерево!\n");
                free(dAK); //звільнення пам'яті для змінної структурного типу
                exit(0);
            }
            else if (insertCheck == -2) /*користувач передав адресу неіснуючого
дескриптору*/
            {
                printf("Ви передали неіснуючий дескриптор дерева!\n");
                free(dAK); //звільнення пам'яті для змінної структурного типу
                exit(0);
            }
            counter++;
            free(dAK); //звільнення пам'яті для змінної структурного типу
            break;
        }
        case 3: //виключення елемента з дерева
        {
            printf("Введіть ключ елемента дерева, який бажаєте виключити з дерева:\n");
            doubleNumberScan(forKey); //зчитування введеного з клавіатури ключа
            noElementCheck = 1;
            keyUsingSearch(BTd->root, forKey, &noElementCheck);
            if (noElementCheck == 1)
            {
                printf("Елементу із заданим ключем немає в дереві!\n");
                exit(0);
            }
            if (deleteElement(BTd->root, forKey) == NULL) /*вилучення елементу з дерева*/
            {
                printf("Передане дерево є виродженим!\n");
                exit(0);
            }
            counter--;
            BTd->size--;
            break;
        }
        case 4: //вивід розміру дерева та його висоти
        {
            height = 0;
            curMaxHeight = 0;
            maxHeight = 0;
            getHeight(BTd, BTd->root, &height, &curMaxHeight, &maxHeight); /*отримання висоти дерева*/
            printf("Кількість елементів у дереві-%d\nВисота дерева-%d\n", BTd->size,
                maxHeight);
            break;
        }
        case 5: //показ дерева на екрані
        {
            if (printTree(BTd->root, nullptr, false) == -1) /*вивід до консолі дерева з перевіркою на те, чи це сталося*/
            {
                printf("Помилка при виведенні дерева на екран!\n");
                exit(0);
            }
            break;
        }
        case 6: //показ поточного положення елемента дерева на екрані
        {
            noElementCheck = 1;
            printf("Введіть ключ(ідентифікаційний код з 10 символів) елемента,\
 положення якого бажаєте побачити у дереві:\n");
            doubleNumberScan(forKey); //зчитування введеного з клавіатури ключа
            if (keyUsingSearch(BTd->root, forKey, &noElementCheck) == 1) /*пошук елементу в дереві за ключем*/
            {
                printf("Елементу з таким ключем немає в дереві!\n");
            }
            else
            {
                if (showCurrentElementsPlace(BTd->root, nullptr, false, forKey) == -1) /*вивід до консолі дерева з показом положення шуканого елемента та перевірка на те, чи це сталося*/
                {
                    printf("Помилка при виведенні дерева на екран!\n");
                    exit(0);
                }
            }
            break;
        }
        case 7: //перепис дерева до файлу
        {
            fOpenChecker = fopenCheck(fr, filename, 'w'); /*встановлення потоку для роботи з файлом у режимі запису, перевірка на помилки та на те, чи потік установився*/
            if (fOpenChecker == -1) //не вдалося відкрити файл
            {
                printf("Не вдалося відкрити файл із заданою назвою!\n");
                exit(0);
            }
            else if (fOpenChecker == -2) /*користувач передав некоректний символ для задання типу роботи з файлом*/
            {
                printf("Ви передали некоректний символ для задання типу роботи з файлом!\n");
                exit(0);
            }
            DataAndKeys* dAK = (DataAndKeys*)malloc(BTd->size * sizeof(DataAndKeys));
//виділення пам'яті для масиву структурного типу
            if (!dAK) /*перевірка на те, чи пам'ять виділена під масив структурного типу*/
            {
                printf("Недостатньо пам'яті для виділення її під масив\n!");
                exit(0);
            }
            count = 0;
            if (getDataAndKeys(BTd->root, dAK, &count) == -1) /*запис інформації
вершин дерева до масиву структурного типу та перевірка на те, чи це сталося*/
            {
                printf("Не вдалося переписати інформацію з вершин дерева!\n");
                free(dAK);
                exit(0);
            }
            if (writeToFile(fr, count, dAK) == -1) /*переписування масиву структурного
типу до файлу та перевірка на те, чи це сталося*/
            {
                printf("Не вдалося переписати масив до файлу!\n");
                free(dAK);
                exit(0);
            }
            free(dAK);
            fclose(fr); //закриття потоку роботи з файлом
            break;
        }
        case 9: //проходження дерева
        {
            do
            {
                printf("\nОберіть порядок проходження бінарного\
 дерева:\n1)Прямий;\n2)Зворотній;\n3)Симетричний;\n");
                numberScan(workWithBT); //зчитування пункту меню
                switch (workWithBT)
                {
                case 1:
                {
                    count = 0;
                    preOrder(BTd->root, &count); /*проходження дерева у прямому
порядку*/
                    flag = 0;
                    break;
                }

                case 2:
                {
                    count = 0;
                    postOrder(BTd->root, &count); /*проходження дерева у зворотньому
порядку*/
                    flag = 0;
                    break;
                }
                case 3:
                {
                    count = 0;
                    inOrder(BTd->root, &count); //проходження дерева симетрично
                    flag = 0;
                    break;
                }
                default:
                {
                    printf("Ви ввели некоректне значення, спробуйте ще раз!\n");
                    flag = 1;
                    continue;
                }
                }
            } while (flag != 0);
            break;
        }
        case 0: //завершення роботи
        {
            deleteTree(BTd->root); //вивільнення пам'яті для дерева
            free(BTd); //вивільнення пам'яті для дескриптора дерева
            printf("\nБінарне дерево видалено!\nРобота програми завершена!\n");
            return 0;
        }
        default: { //якщо користувач ввів некоректне значення
            printf("Ви ввели некоректне значення, спробуйте ще раз!\n");
            continue;
        }
        }
    }
}

/*
Функція створення дерева
Вхід: відсутній
Вихід:
    createTree = <адреса створеного дескриптора> - дерево створено;
               = NULL - дерево не створено
*/
BTDescriptor* createTree() {
    BTDescriptor* dsBT;
    dsBT = (BTDescriptor*)malloc(sizeof(BTDescriptor)); /*виділення пам'яті під
дескриптор дерева*/
    if (!dsBT) return NULL; /*немає доступної пам'яті в купі, дескриптор не
створений*/
    dsBT->root = NULL;
    dsBT->size = 0;
    return dsBT;
}

/*
Функція створення вершини дерева та ініціалізації її полів
Вхід:
    entry - адреса змінної типу DataAndKeys з полями для ініціалізації
Вихід:
    initializeNode = <адреса вершини> - створена вершина;
                   = NULL - вершину не створено або не ініціалізовано
*/
BTNode* initializeNode(const DataAndKeys* entry)
{
    BTNode* newNode = (BTNode*)malloc(sizeof(BTNode)); /*виділення пам'яті під вершину
дерева*/
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
Рекурсивна функція знаходження мінімального елемента у дереві
Вхід:
    tree - адреса вершини дерева
Вихід:
    findSmallestElement = <адреса вершини> - вершина знайдена
                        = NULL - вершина не знайдена
*/
BTNode* findSmallestElement(BTNode* tree) {
    if (tree == NULL || tree->right == NULL)
        return tree;
    else
        return findSmallestElement(tree->right);
}

/*
Функція вилучення елемента з дерева
Вхід:
    subTree - адреса вершини дерева
    key - рядок з ключем шуканої вершини дерева
Вихід:
    deleteElement = NULL - якщо дерево вироджене
                  = <адреса вершини, яка займає місце видаленої> - якщо вузол успішно видалений
*/
BTNode* deleteElement(BTNode* subTree, double key) {
    //випадок виродженого дерева
    if (!subTree)
    {
        return subTree;
    }

    //якщо ключ шуканого елемента менший
    //за ключ елемента, з яким ми зараз порівнюємо його
    if (key < subTree->treeData.key)
    {
        subTree->left = deleteElement(subTree->left, key);
    }

    //якщо ключ шуканого елемента більший
    //за ключ елемента, з яким ми зараз порівнюємо його
    else if (key > subTree->treeData.key)
    {
        subTree->right = deleteElement(subTree->right, key);
    }

    //якщо ключ шуканого елемента
    //співпадає з ключем вузла, з яким
    //ми зараз його порівнюємо, то видаляємо
    //цей вузол
    else {
        //якщо вершина з одним нащадком або без них
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

        //якщо вершина має два нащадки
        BTNode* temp = findSmallestElement(subTree->right);
        subTree->treeData.key = temp->treeData.key;
        subTree->treeData.data = temp->treeData.data;

        //видаляємо найменшого нащадка
        subTree->right = deleteElement(subTree->right, temp->treeData.key);
    }
    return subTree;
}

/*
Функція вираховування довжини радіус-вектора до точки з заданими координатами
Вхід:
    x - координата х точки
    y - координата y точки
    z - координата z точки
Вихід:
    calculateTheKey = <значення довжини радіус-вектора до точки з заданими координатами> - при успішному відпрацюванні програми
*/
float calculateTheKey(int x, int y, int z)
{
    return (float)sqrt((double)x * (double)x + (double)y * (double)y + (double)z * (double)z);
}

/*Функція введення значень змінних
Вхід:
    N - адреса змінної для ініціалізації
Вихід:
    numberScan = N - ініціалізована змінна
*/
float doubleNumberScan(float& N) {
    int entered = 0; //змінна для контролю введених користувачем з клавіатури символів
    do {
        do {
            entered = scanf_s("%f", &N);
            while (getchar() != '\n');
            while (entered < 1) {
                printf("Ви ввели некоректне значення, повторіть введення: ");
                entered = scanf_s("%f", &N);
                while (getchar() != '\n');
            }
        } while (entered < 1);
    } while (entered != 1);
    return N;
}

/*Функція введення значень змінних
Вхід:
    N - адреса змінної для ініціалізації
Вихід:
    numberScan = N - ініціалізована змінна
*/
int numberScan(int& N) {
    int entered = 0; //змінна для контролю введених користувачем з клавіатури символів
    do {
        do {
            entered = scanf_s("%d", &N);
            while (getchar() != '\n');
            while (entered < 1) {
                printf("Ви ввели некоректне значення, повторіть введення: ");
                entered = scanf_s("%d", &N);
                while (getchar() != '\n');
            }
        } while (entered < 1);
    } while (entered != 1);
    return N;
}

/*
Функція вiдкриття файлу та перевірки на помилки
Вхід:
    fp - файловий дескриптор
    filename - рядок з введеною користувачем назви файлу
    d - змінна для визначення того, у якому режимі користувач бажає відкрити файл
Вихід:
    fopenCheck = -1 - якщо виникла помилка при відкритті файлу
               = -2 - якщо користувач передав некоректний символ для задання типу роботи з файлом
               = 0 - при успішному встановленні потоку роботи з файлом
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

/*Функція для підрахунку кількості елементів у файлі
Вхід:
    fd - файловий дескриптор
Вихід:
    elementCounter = count - кількість елементів у файлі;
                   = -1 - якщо не переміщено показник на початок файлу
*/
int elementCounter(FILE*& fd) {
    int count = 0; /*змінна для підрахунку кількості елементів, що зберігається у
файлі*/
    while (!feof(fd)) {
        fscanf(fd, "%*d");
        fscanf(fd, "%*d");
        fscanf(fd, "%*d");
        fscanf(fd, "%*lf");
        count++;
    }
    rewind(fd);
    if (ferror(fd)) {
        return -1; //не переміщено показник на початок файлу
    }
    return count;
}

/*
Функція перепису інформації вершин дерева до масиву структурного типу
Вхід:
    tree - адреса вершини дерева
    dAK - адреса масиву структурного типу
    j - адреса змінної з кількістю пройдених елементів дерева
Вихід:
    getDataAndKeys = -1 - якщо не вийшло переписати інформацію з вершин дерева
                   = 0 - якщо вийшло переписати інформацію з вершин дерева
*/
int getDataAndKeys(BTNode* tree, DataAndKeys* dAK, int* j) {
    if (tree != NULL) { /*якщо вершина існує*/
        dAK[*j].data = tree->treeData.data;
        dAK[*j].key = tree->treeData.key;
        *j = *j + 1;
        getDataAndKeys(tree->left, dAK, j); /*"пірнаємо" в ліве піддерево вузла*/
        getDataAndKeys(tree->right, dAK, j); /*"пірнаємо" в праве піддерево вузла*/
    }
    return 0;
}

/*
Функція пошуку елементу за ключем для виведення його на екран
Вхід:
    tree - адреса вершини дерева
    key - ключ вершини дерева
    noElementCheck - адреса змінної для перевірки наявності елемента в дереві
Вихід:
    keyUsingSearch = noElementCheck = 0 - якщо елемент з ключем, за яким відбувався
                             пошук, є в дереві
                                    = 1 - якщо елементу з ключем, за яким відбувався
        пошук, немає в дереві
*/
int keyUsingSearch(BTNode* tree, double key, int* noElementCheck) {
    if (tree != NULL) { /*якщо вершина існує*/
        if (tree->treeData.key == key)
        {
            printf("Елемент дерева з ключем %lf:\nКоординати точки, які зберігає елемент:\
\nx-%d\ny-%d\nz-%d\n", key, tree->treeData.data.x, tree->treeData.data.y, tree->treeData.data.z);
            *noElementCheck = 0;
            return *noElementCheck;
        }
        keyUsingSearch(tree->left, key, noElementCheck); /*"пірнаємо" в ліве піддерево
вузла*/
        keyUsingSearch(tree->right, key, noElementCheck); /*"пірнаємо" в праве
піддерево вузла*/
    }
    return *noElementCheck;
}

/*
Функція вставлення елемента в дерево
Вхід:
    dsTree - адреса дескриптора дерева
    entry - адреса змінної типу DataAndKeys з полями для ініціалізації
Вихід:
    insertTree = 0 - якщо елемент вставлено у дерево
               = -1 - якщо елемент не вставлено у дерево
               = -2 - якщо переданий неіснуючий дескриптор дерева
*/
int insertTree(BTDescriptor* dsTree, DataAndKeys* entry)
{
    if (dsTree == NULL)
    {
        return -2; //якщо переданий неіснуючий дескриптор дерева
    }
    if (dsTree->root == NULL) //перевірка на виродженість дерева
    {
        if (!(dsTree->root = initializeNode(entry)))
        {
            return -1; //не вдалося ініціалізувати вузол
        }
        ++dsTree->size;
        return 0;
    }

    BTNode* subTree = dsTree->root;

    while (subTree != NULL) /*пошук місця задля вставлення нового елементу та подальша
реалізація цього*/
    {
        if (subTree->treeData.key > entry->key)
        {
            if (subTree->left == NULL)
            {
                if (!(subTree->left = initializeNode(entry)))
                {
                    return -1; //не вдалося ініціалізувати вузол
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
                    return -1; //не вдалося ініціалізувати вузол
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
Рекурсивна функція виводу на екран елементів дерева
Вхід:
    root - адреса вершини дерева
    prev - адреса рядку для зображення з'єднань елементів дерева
    isLeft - змінна для визначення існування лівого нащадку
Вихід:
    printTree = 0 - якщо дерево виведене на екран
              = -1 - якщо не вийшло вивести дерево на екран
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
        return -1; //недостатньо пам'яті для виділення її під змінну типу Trunk
    }
    trunk->prev = prev;
    trunk->str = prev_str;

    printTree(root->right, trunk, true); /*"пірнаємо" в праве піддерево вузла*/

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

    showTrunks(trunk); //виводимо на екран з'єднання елементів дерева
    printf("  %lf\n", root->treeData.key);

    if (prev) {
        prev->str = prev_str;
    }
    strcpy(trunk->str, "   |");

    printTree(root->left, trunk, false);  /*"пірнаємо" у ліве піддерево вузла*/
    free(trunk); //звільняємо пам'ять під змінну типу Trunk
    return 0;
};

/*
Рекурсивна функція виводу на екран дерева з показом положення шуканого елемента
Вхід:
    root - адреса вершини дерева
    prev - адреса рядку для зображення з'єднань елементів дерева
    isLeft - змінна для визначення існування лівого нащадку
    key - ключ шуканого елемента
Вихід:
    showCurrentElementsPlace = 0 - якщо дерево виведене на екран та положення шуканого
 елемента показане
                             = -1 - якщо не вийшло вивести дерево на екран
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
        return -1; //недостатньо пам'яті для виділення її під змінну типу Trunk
    }
    trunk->prev = prev;
    trunk->str = prev_str;

    showCurrentElementsPlace(root->right, trunk, true, key); /*"пірнаємо" у праве
піддерево вузла*/

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

    showTrunks(trunk); //виводимо на екран з'єднання елементів дерева
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

    showCurrentElementsPlace(root->left, trunk, false, key); /*"пірнаємо" у ліве
піддерево вузла*/
    free(trunk); //звільняємо пам'ять під змінну типу Trunk
    return 0;
};

/*
Функція для зчитування даних з файлу і запису їх до комірок масиву типу DataAndKeys
Вхід:
    fp - файловий дескриптор
    size - змінна з кількістю елементів у файлі
    dAK - адреса масиву типу DataAndKeys
Вихід:
    readFromFile = -1 - якщо не вийшло перемістити показник на початок файлу
                 = 0 - якщо елементи з файлу успішно переписані
*/
int readFromFile(FILE* fp, int size, DataAndKeys* dAK)
{
    for (int i = 0; i < size; i++) { /*перепис вмісту файлу до масиву структурного
   типу dAK*/
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
Функція запису елементів масиву типу DataAndKeys до файлу
Вхід:
    fp - файловий дескриптор
    size - змінна з кількістю елементів у файлі
    dAK - адреса масиву типу DataAndKeys
Вихід:
    writeToFile = -1 - якщо не вдалося переписати масив до файлу
                = 0 - якщо масив успішно переписаний до файлу
*/
int writeToFile(FILE* fp, int size, DataAndKeys* dAK)
{
    for (int i = 0; i < size; i++) //запис елементів до файлу
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
Функція для перевірки правильності введення назви файлу
Вхід:
    filename - рядок з введеною користувачем назви файлу
Вихід: відсутній
*/
void checkFilename(char filename[]) {
    int fcheck = 0; //змінна для контролю правильності вводу назви файлу користувачем
    int scheck = 0; //змінна для контролю правильності вводу назви файлу користувачем
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
                    printf("Ви ввели розширення не вірно, спробуйте ще раз!\n");
                    fcheck = 0;
                }
            }
        }
        if (filename[0] == '\n')
        {
            printf("Ви нічого не ввели, спробуйте ще раз!\n");
        }

        if (scheck == 0 && filename[0] != '\n')
        {
            printf("Ви не ввели розширення, спробуйте ще раз!\n");
        }
    } while (filename[0] == '\n' || fcheck == 0 || scheck == 0);
    filename[strlen(filename) - 1] = '\0'; //змінює \n на \0
}

/*
Функція визначення висоти дерева
Вхід:
    tree - адреса вершини дерева
    height - адреса змінної з висотою елементу, який наразі проходиться
    curMaxHeight - адреса змінної з поточною максимальною висотою дерева
    maxHeight - адреса змінної з максимальною висотою дерева
Вихід: відсутній
*/
void getHeight(BTDescriptor* dsTree, BTNode* tree, int* height, int* curMaxHeight, int* maxHeight)
{
    if (dsTree == NULL)
    {
        return;
    }

    if (tree != NULL) { /*якщо вершина існує*/
        *height = *height + 1;
        getHeight(dsTree, tree->left, height, curMaxHeight, maxHeight); /*"пірнаємо" в
ліве піддерево вузла*/
        *height = *height + 1;
        getHeight(dsTree, tree->right, height, curMaxHeight, maxHeight); /*"пірнаємо"
в праве піддерево вузла*/
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
Функція проходження дерева у прямому порядку
Вхід:
    tree - адреса вершини дерева
    j - адреса змінної з кількістю пройдених елементів дерева
Вихід: відсутній
*/
void preOrder(BTNode* tree, int* j) {
    if (tree != NULL) { /*якщо вершина існує*/
        *j = *j + 1;
        printf("\nВершина №%d:\nКоординати елемента вершини:\n\
x-%d\ny-%d\nz-%d\nКлюч вершини-%lf\n", *j, tree->treeData.data.x,
tree->treeData.data.y, tree->treeData.data.z, tree->treeData.key);
/*обробка даних на вершині дерева*/
        preOrder(tree->left, j); /*"пірнаємо" в ліве піддерево вузла*/
        preOrder(tree->right, j); /*"пірнаємо" в праве піддерево вузла*/
    }
    return;
}

/*
Функція проходження дерева у зворотньому порядку
Вхід:
    tree - адреса вершини дерева
    j - адреса змінної з кількістю пройдених елементів дерева
Вихід: відсутній
*/
void postOrder(BTNode* tree, int* j) {
    if (tree != NULL) { /*якщо вершина існує*/
        postOrder(tree->left, j); /*"пірнаємо" в ліве піддерево вузла*/
        postOrder(tree->right, j); /*"пірнаємо" в праве піддерево вузла*/
        *j = *j + 1;
        printf("\nВершина №%d:\nКоординати елемента вершини:\n\
x-%d\ny-%d\nz-%d\nКлюч вершини-%lf\n", *j, tree->treeData.data.x,
tree->treeData.data.y, tree->treeData.data.z, tree->treeData.key);
/*обробка даних на вершині дерева*/
    }
    return;
}

/*
Функція симетричного проходження дерева
Вхід:
    tree - адреса вершини дерева
    j - адреса змінної з кількістю пройдених елементів дерева
Вихід: відсутній
*/
void inOrder(BTNode* tree, int* j) {
    if (tree != NULL) { /*якщо вершина існує*/
        inOrder(tree->left, j); /*"пірнаємо" в ліве піддерево вузла*/
        *j = *j + 1;
        printf("\nВершина №%d:\nКоординати елемента вершини:\n\
x-%d\ny-%d\nz-%d\nКлюч вершини-%lf\n", *j, tree->treeData.data.x,
tree->treeData.data.y, tree->treeData.data.z, tree->treeData.key);
/*обробка даних на вершині дерева*/
        inOrder(tree->right, j); /*"пірнаємо" в праве піддерево вузла*/
    }
    return;
}

/*
Рекурсивна функція виводу на екран з'єднань елементів дерева
Вхід:
    trunk - адреса змінної типу Trunk
Вихід: відсутній
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
Функція видалення дерева
Вхід:
    tree - адреса вершини дерева
Вихід: відсутній
*/
void deleteTree(BTNode* tree) {
    if (tree != NULL) { /*якщо вершина існує*/
        deleteTree(tree->left); /*"пірнаємо" в ліве піддерево вузла*/
        deleteTree(tree->right); /*"пірнаємо"в праве піддерево вузла*/
        free(tree); //звільнення пам'яті під вузол
    }
    return;
}
