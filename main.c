#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <time.h>
#include "array.h"

int main() {
    setlocale(LC_ALL, "RUS");

    Software* data = NULL;
    int count = 0;
    char file_name[64];

    while (1) {
        int operation;
        printf("|----- Базы данных для графических редакторов ----|\n");
        printf("|Выполнила: Карнаух Анастасия, БТИИ-251           |\n");
        printf("|--------------------- Меню: ---------------------|\n");
        printf("|1. Загрузка данных из файла                      |\n");
        printf("|2. Просмотр всех записей                         |\n");
        printf("|3. Поиск по типу дизайна + поддерживаемые форматы|\n");
        printf("|4. Сортировка данных по цене                     |\n");
        printf("|5. Добавление новой записи                       |\n");
        printf("|6. Сохранение данных в файл                      |\n");
        printf("|7. Генерация тестовых значений                   |\n");
        printf("|0. Выход                                         |\n");
        printf("Введите номер операции: ");

        scanf("%d", &operation);
        getchar();

        switch (operation) {
        case 1:
            printf("Введите имя файла: ");
            fgets(file_name, sizeof(file_name), stdin);
            file_name[strcspn(file_name, "\n")] = 0;

            load_from_file(&data, &count, file_name);
            printf("Загружено %d записей\n", count);
            break;

        case 2:
            output_software(data, count);
            break;

        case 3:
            printf("1 - поиск по типу дизайна\n");
            printf("2 - комбинированный поиск (цена + формат)\n");
            printf("Выбор: ");
            int choice;
            scanf("%d", &choice);
            getchar();

            if (choice == 1) {
                printf("Тип дизайна (0-3): ");
                int type;
                scanf("%d", &type);
                getchar();
                search_by_design(data, count, type);
            }
            else if (choice == 2) {
                printf("Макс цена: ");
                float max_price;
                scanf("%f", &max_price);
                printf("Формат (0-4): ");
                int fmt;
                scanf("%d", &fmt);
                getchar();
                search_by_price_and_format(data, count, max_price, fmt);
            }
            break;

        case 4:
            if (count == 0) {
                printf("База пуста!\n");
            }
            else {
                qsort(data, count, sizeof(Software), compare_price);
                output_software(data, count);
            }
            break;

        case 5:
            data = add_software(data, &count);
            printf("Запись добавлена. Всего записей: %d\n", count);
            break;

        case 6:
            printf("Имя файла: ");
            fgets(file_name, sizeof(file_name), stdin);
            file_name[strcspn(file_name, "\n")] = 0;
            save_to_file(data, count, file_name);
            printf("Запись успешно сохранена!");
            break;

        case 7:
            printf("Введите количество записей: ");
            scanf("%d", &count);
            getchar();
            data = realloc(data, count * sizeof(Software));
            generate_test(data, count);
            printf("Сгенерировано %d тестовых записей\n", count);
            break;

        case 0:
            if (data) free(data);
            printf("До свидания!\n");
            return 0;

        default:
            printf("Неверная операция!\n");
            break;
        }
        printf("\n");
    }
}