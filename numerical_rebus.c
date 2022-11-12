#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int is_letter(char c)
{
    return c >= 'A' && c <= 'Z';
}

// ввод строки
char* read_string()
{
    int size = 0; // размер строки
    int capacity = 1; // емкость строки
    int plus_count = 0; // количество плюсов
    char* s = (char*)malloc(capacity * sizeof(char)); // выделяем память под 1 символ

    // пока не дойдём до переноса строки или конца ввода
    for (char c = getchar(); c != '\n'; c = getchar()) {
        if (c == ' ' || c == '\t') // пропускаем пробельные символы
            continue;

        if (c >= 'a' && c <= 'z') // если буква в нижнем регистре
            c += 'A' - 'a'; // переводим её в верхний

        if (c == '+') // если плюс
            plus_count++; // увеличиваем счётчик плюсов

        if (!is_letter(c) && c != '+' && c != '=') { // если не буква в верхнем регистре, не плюс и не равно
            printf("Invalid rebus (error character '%c')\n", c);
            free(s); // освобождаем память
            return NULL; // возвращаем пустой указатель
        }

        s[size++] = c; // запоминаем символ

        if (size >= capacity) { // если ёмкости недостаточно
            capacity++; //увеличиваем ее на 1
            s = (char*)realloc(s, capacity * sizeof(char)); // и перераспределяем память
        }
    }

    // если некорректное количество слагаемых
    if (plus_count == 0 || plus_count > 6)
    {
        printf("Invalid rebus\n");
        free(s); // освобождаем память
        return NULL; // возвращаем пустой указатель
    }

    s[size] = '\0'; // проставляем призна конца строки

    return s; // возвращаем строку
}

// получение уникальных символов
char* get_unique(char* s)
{
    int chars[26];
    //----------------------------------------------------//
    // обнуляем счётчики
    //for (int i = 0; i < 26; i++)
        //chars[i] = 0;
    //----------------------------------------------------//
   //делаем оптимизированное заполнение массива
    //-------------------------------------------------//
    memset(chars, 0, 26*sizeof(int));
    //-------------------------------------------------//
   for (int i = 0; s[i]; i++)
        if (is_letter(s[i])) // если буква
            chars[s[i] - 'A'] = 1; // то добавляем её

    int count = 0; // количество уникальных символов
    char* unique = (char*)calloc(27, sizeof(char));

    for (int i = 0; i < 26; i++)
        if (chars[i]) // если буква есть
            unique[count++] = 'A' + i; // то добавляем букву

    if (count > 10) { // если больше 10 символов
        printf("Too more unique characters (%d - %s)\n", count, unique);
        free(unique); // освобождаем память
        return NULL; // возвращаем нелувой указатель
    }

    return unique; // возвращаем уникальные символы
}

// разбиение выражения на лексемы
char** split_to_lexemes(char* s, int* count)
{
    char** lexemes = (char**)malloc(8 * sizeof(char*));
    *count = 0;

    // разбиваем на лексемы
    for (char* pch = strtok(s, "+="); pch; pch = strtok(NULL, "+="))
        lexemes[(*count)++] = pch; // запоминаем начало лексемы

    return lexemes; // возвращаем лексемы
}

// получение числа из лексемы
int lexeme_to_number(int* values, char* lexeme)
{
    int number = 0;

    for (int i = 0; lexeme[i]; i++)
        number = number * 10 + values[lexeme[i] - 'A']; // формируем число

    return number; // возвраащем число
}

// проверка корректности ребуса
int check_value(int* values, char** lexemes, int lexemes_count)
{
    for (int i = 0; i < lexemes_count; i++)
        if (values[lexemes[i][0] - 'A'] == 0) // если ведущий ноль
            return 0; // то некорректно

    int sum = 0; // считаем сумму

    for (int i = 0; i < lexemes_count - 1; i++)
        sum += lexeme_to_number(values, lexemes[i]); // наращиваем сумму

    return sum == lexeme_to_number(values, lexemes[lexemes_count - 1]); // проверяем с ответом
}

int find_solve(int start, int* values, int* used, char* unique, char** lexemes, int lexemes_count)
{
    if (unique[start] == '\0')
        return check_value(values, lexemes, lexemes_count);

    // идём по цифрам
    for (int i = 0; i < 10; i++) {
        if (used[i]) // если цифра уже использовалась
            continue; // то пропускаем её

        used[i] = 1; // используем цифру
        values[unique[start] - 'A'] = i; // проставляем букве цифру

        if (find_solve(start + 1, values, used, unique, lexemes, lexemes_count)) // ищем решение дальше
            return 1;

        used[i] = 0; // не используем цифру
    }

    return 0; // не нашли решение
}

// вывод результата
void print_result(int* values, char* unique, char** lexemes, int lexemes_count)
{
    printf("Find solve: ");
    for (int i = 0; unique[i]; i++)
        printf("%c->%d ", unique[i], values[unique[i] - 'A']);
    printf("\n");

    for (int i = 0; i < lexemes_count; i++) {
        if (i > 0)
            printf("%c", i < lexemes_count - 1 ? '+' : '=');

        printf("%d", lexeme_to_number(values, lexemes[i]));
    }

    printf("\n");
}


int main() {
    srand(time(NULL));
    //float start = clock();
    printf("Enter rebus: ");
    char* rebus = read_string(); // считыааем строку
    //исправление ошибки подсчета времени работы программы
    float start = clock();
    if (!rebus) // если некорректный ввод
        return -1; // то выходим

    char* unique = get_unique(rebus); // получаем уникальные символы

    if (!unique) // если слишком много уникальных символов
        return -1; // то выходим

    printf("Rebus: %s\n", rebus);
    printf("Unique: %s\n", unique);

    int lexemes_count; // количество лексем
    char** lexemes = split_to_lexemes(rebus, &lexemes_count); // разбиваем на лексемы

    //вывод лексем
    /*printf("Lexemes count: %d\n", lexemes_count);
    for (int i = 0; i < lexemes_count; i++)
    {
        printf("Lexemes: %s\n", lexemes[i]);
    }*/

    int values[26];
    //убираем зануление, так как оно нам не требуется
    //-------------------------------------------------//
    //for (int i = 0; i < 26; i++)
        //values[i] = 0;
    //-------------------------------------------------//

    int used[10];
    //-------------------------------------------------//
    //for (int i = 0; i <10;i++)
        //values[i] = 0;
    //-------------------------------------------------//
    //оптимизируем зануление массива, чтобы не пробегать по нему в данном случае 10 раз, используем memset
    //-------------------------------------------------//
    memset(used, 0, 10 * sizeof(int));
    //-------------------------------------------------//

    if (find_solve(0, values, used, unique, lexemes, lexemes_count))
    { // если нашли ответ
        print_result(values, unique, lexemes, lexemes_count); // то выводим его
        printf("Time - %f sec\n", (clock() - start) / CLOCKS_PER_SEC);
    }
    else
    {
        printf("No solve...\n");
    }

    // освобождаем память
    free(rebus);
    free(unique);
    free(lexemes);

    return 0;
}
