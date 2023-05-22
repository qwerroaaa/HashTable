#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstring>
using namespace std;
class HashTable {
public:
    int q;              // Размер хеш-таблицы           
    int r;              // Параметр r для второй хеш-функции
    int s;              // Параметр s для второй хеш-функции
    int size;           // Текущий размер таблицы
    char** table;       // Хеш-таблица
    HashTable() {
        q = 99991;
        s = 100;
        r = 31;
        size = 0;
        table = new char* [q];
        for (int i = 0; i < q; i++) {
            table[i] = nullptr;
        }
    }

    ~HashTable() {
        clear();
        delete[] table;
    }

    // Хеш-функция по схеме Горнера
    int hash(const char* str) const {
        int len = strlen(str);
        int hashVal = 0;

        for (int i = 0; i < len; i++) {
            hashVal = ((hashVal % q) * 31 + str[i]) % q;
        }

        return hashVal;
    }

    // Вставка строки в таблицу
    bool insert(const char* str) {
        if (size == q) {
            return false;  // Таблица полна
        }

        int index = hash(str);
        int step = secondHash(hash(str));
        //int step = secondHash(index);

        while (table[index] != nullptr) {
            if (strcmp(table[index], str) == 0) {
                return false;  // Строка уже присутствует в таблице
            }

            index = (index + step) % q;
        }

        table[index] = new char[strlen(str) + 1]; //выделяем память под строку
        strcpy(table[index], str);
        size++;

        return true;
    }

    // Поиск строки в таблице
    bool search(const char* str) const {
        int index = hash(str);
        int step = secondHash(hash(str));
        //int step = secondHash(index);

        int iter = 0;
        while (table[index] != nullptr && iter < q) {
            if (strcmp(table[index], str) == 0) {
                return true;  // Строка найдена
            }

            index = (index + step) % q;
            iter++;
        }

        return false;  // Строка не найдена
    }

    // Очистка таблицы
    void clear() {
        for (int i = 0; i < q; i++) {
            if (table[i] != nullptr) {
                delete[] table[i];
                table[i] = nullptr;
                size--;
            }
        }
    }

    // Вторая хеш-функция
    int secondHash(int hashVal) const {
        return hashVal % r + s;
    }

    // Возвращает число строк в таблице
    int getSize() const {
        return size;
    }

    // Возвращает среднее число сравнений строк
    double getAverageComparisons() const {
        if (size == 0) {
            return 0.0;
        }

        int totalComparisons = 0;

        for (int i = 0; i < q; i++) {
                if (table[i] != nullptr) {
                    int index = hash(table[i]);
                    int step = secondHash(hash(table[i]));
                    int comparisons = 1;

                    while (table[index] != nullptr && table[index] != table[i]) {
                        index = (index + step) % q;
                        comparisons++;
                    }

                    totalComparisons += comparisons;
                }
        }

        return totalComparisons / size;
    }
};

int main() {
    setlocale(LC_CTYPE, "rus");
    HashTable hashtable;

    int n = 80000;  // Число строк

    // Генерация исходного массива строк
    char** strings = new char* [n];
    for (int i = 0; i < n; i++) {
        int len = rand() % 10 + 1;  // Длина строки от 1 до 10 символов
        strings[i] = new char[len + 1];

        for (int j = 0; j < len; j++) {
            strings[i][j] = 'a' + (rand() % 26);
        }
        strings[i][len] = '\0';
    }
    // Вставка уникальных строк в хеш-таблицу
    const char* newString = "example";
    hashtable.insert(newString);
    for (int i = 0; i < n; i++) {
        //hashtable.insert(newString);
        hashtable.insert(strings[i]);
    }

    // Получение числа строк и среднего числа сравнений
    int stringCount = hashtable.getSize();
    double avgComparisons = hashtable.getAverageComparisons();

    cout << "Число строк в таблице: " << stringCount << endl;
    cout << "Среднее число сравнений: " << avgComparisons << endl;

    // Поиск заданной строки
    const char* searchString = "example";
    bool found = hashtable.search(searchString);

    if (found) {
        cout << "Строка '" << searchString << "' найдена в таблице." << endl;
    }
    else {
        cout << "Строка '" << searchString << "' не найдена в таблице." << endl;
    }
}
