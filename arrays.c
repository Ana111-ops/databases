#define _CRT_SECURE_NO_WARNINGS
#include "array.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>

const char* get_design_type_name(DesignType type) {
    switch (type) {
    case DESIGN_2D: return "2D дизайн";
    case DESIGN_3D: return "3D дизайн";
    case DESIGN_UI: return "UI дизайн";
    case DESIGN_UX: return "UX дизайн";
    default: return "Неизвестно";
    }
}

const char* get_format_name(Format format) {
    switch (format) {
    case FORMAT_PNG: return "PNG";
    case FORMAT_JPEG: return "JPEG";
    case FORMAT_SVG: return "SVG";
    case FORMAT_PDF: return "PDF";
    case FORMAT_PSD: return "PSD";
    default: return "Неизвестно";
    }
}

const char* get_requirement_name(Platform req) {
    switch (req) {
    case REQ_WIN10: return "Windows 10";
    case REQ_WIN11: return "Windows 11";
    case REQ_MACOS12: return "macOS 12";
    case REQ_LINUX_UBUNTU20: return "Linux Ubuntu 20";
    default: return "Неизвестно";
    }
}

int output_software(const Software* data, int count) {
    if (count <= 0 || data == NULL) {
        printf("База пуста!\n");
        return 0;
    }

    for (int i = 0; i < count; i++) {
        printf("\nЗапись %d:\n", i + 1);
        printf("Название: %s\n", data[i].name);
        printf("Тип дизайна: %s\n", get_design_type_name(data[i].design_type));
        printf("Разработчик: %s\n", data[i].developer);
        printf("Форматы: ");
        for (int j = 0; j < 5; j++) {
            printf("%s", get_format_name(data[i].formats[j]));
            if (j < 4) printf(", ");
        }
        printf("\nЦена: %.2f\n", data[i].price);
        printf("Сайт: %s\n", data[i].website);
        printf("API: %d\n", data[i].api_version);
        printf("Требования: ");
        for (int j = 0; j < 3; j++) {
            printf("%s", get_requirement_name(data[i].requirements[j]));
            if (j < 2) printf(", ");
        }
        printf("\n");
    }
    printf("---------------------------------------------\n");
    return count;
}

int input_software(Software* data) {
    printf("Введите название ПО: ");
    fgets(data->name, sizeof(data->name), stdin);
    data->name[strcspn(data->name, "\n")] = 0;

    printf("Введите тип дизайна (0-3): ");
    scanf("%d", &data->design_type);
    getchar();

    printf("Введите разработчика: ");
    fgets(data->developer, sizeof(data->developer), stdin);
    data->developer[strcspn(data->developer, "\n")] = 0;

    printf("Введите цену: ");
    scanf("%f", &data->price);
    getchar();

    printf("Введите веб-сайт: ");
    fgets(data->website, sizeof(data->website), stdin);
    data->website[strcspn(data->website, "\n")] = 0;

    printf("Введите версию API: ");
    scanf("%d", &data->api_version);
    getchar();

    printf("Введите форматы (5 чисел 0-4): ");
    for (int j = 0; j < 5; j++) {
        int format;
        scanf("%d", &format);
        data->formats[j] = (Format)format;
    }
    getchar();

    printf("Введите требования (3 числа 0-3): ");
    for (int j = 0; j < 3; j++) {
        int req;
        scanf("%d", &req);
        data->requirements[j] = (Platform)req;
    }
    getchar();

    return 1;
}

int generate_test(Software* data, int count) {
    const char* names[] = { "Photoshop", "GIMP", "Sketch", "Figma", "Canva" };
    const char* devs[] = { "Adobe", "Figma", "Sketch", "GIMP", "Canva" };
    const char* sites[] = { "adobe.com", "figma.com", "sketch.com", "gimp.org", "canva.com" };

    for (int i = 0; i < count; i++) {
        strncpy(data[i].name, names[rand() % 5], sizeof(data[i].name) - 1);
        data[i].name[sizeof(data[i].name) - 1] = '\0';

        strncpy(data[i].developer, devs[rand() % 5], sizeof(data[i].developer) - 1);
        data[i].developer[sizeof(data[i].developer) - 1] = '\0';

        strncpy(data[i].website, sites[rand() % 5], sizeof(data[i].website) - 1);
        data[i].website[sizeof(data[i].website) - 1] = '\0';

        data[i].design_type = (DesignType)(rand() % 4);
        data[i].price = 10.0f + rand() % 90;
        data[i].api_version = 1 + rand() % 5;

        for (int j = 0; j < 5; j++) data[i].formats[j] = (Format)(rand() % 5);
        for (int j = 0; j < 3; j++) data[i].requirements[j] = (Platform)(rand() % 4);
    }
    return 1;
}

int save_to_file(const Software* data, int count, const char* filename) {
    FILE* f = fopen(filename, "w");
    if (f == NULL) return 0;

    fprintf(f, "%d\n", count);
    for (int i = 0; i < count; i++) {
        fprintf(f, "%s\n%d\n%s\n%.2f\n%s\n%d\n",
            data[i].name, data[i].design_type, data[i].developer,
            data[i].price, data[i].website, data[i].api_version);
        for (int j = 0; j < 5; j++) {
            fprintf(f, "%d ", data[i].formats[j]);
        }
        fprintf(f, "\n");

        for (int j = 0; j < 3; j++) {
            fprintf(f, "%d ", data[i].requirements[j]);
        }
        fprintf(f, "\n");
    }
    fclose(f);
    return 1;
}


int load_from_file(Software** data, int* count, const char* filename) {
    FILE* f = fopen(filename, "r");
    if (f == NULL) {
        printf("Файл не найден\n");
        return 0;
    }

    fscanf(f, "%d", count);

    *data = realloc(*data, *count * sizeof(Software));
    if (*data == NULL) {
        fclose(f);
        return 0;
    }
    int c;
    char temp[50];
    fgets(temp, sizeof(temp), f);

    for (int i = 0; i < *count; i++) {
        fgets((*data)[i].name, sizeof((*data)[i].name), f);
        (*data)[i].name[strcspn((*data)[i].name, "\n")] = 0;

        fgets(temp, sizeof(temp), f);
        sscanf(temp, "%d", &(*data)[i].design_type);

        fgets((*data)[i].developer, sizeof((*data)[i].developer), f);
        (*data)[i].developer[strcspn((*data)[i].developer, "\n")] = 0;

        fgets(temp, sizeof(temp), f);
        sscanf(temp, "%f", &(*data)[i].price);

        fgets((*data)[i].website, sizeof((*data)[i].website), f);
        (*data)[i].website[strcspn((*data)[i].website, "\n")] = 0;

        fgets(temp, sizeof(temp), f);
        sscanf(temp, "%d", &(*data)[i].api_version);

        for (int j = 0; j < 5; j++) {
            fscanf(f, "%d",&(*data)[i].formats[j]);
        }
        fgetc(f);  

        for (int j = 0; j < 3; j++) {
            fscanf(f, "%d", &(*data)[i].requirements[j]);
        }
        fgetc(f);
    }
    fclose(f);
    return 1;
}
Software* add_software(Software* data, int* count) {
    (*count)++;  

    data = realloc(data, *count * sizeof(Software)); 
    if (data == NULL) {  
        (*count)--;     
        return data;    
    }
    input_software(&data[*count - 1]);
    return data; 
}


int search_by_design(const Software* data, int count, int type) {
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (data[i].design_type == (DesignType)type) {
            output_software(&data[i], 1);
            found = found + 1;
        }
    }
    printf("Найдено: %d\n", found);
    return found;
}

int search_by_price_and_format(const Software* data, int count, float max_price, int format) {
    int found = 0;
    int i, j;

    for (i = 0; i < count; i++) {
        if (data[i].price <= max_price) {
            for (j = 0; j < 5; j++) {
                if (data[i].formats[j] == format) {
                    output_software(&data[i], 1);
                    found = found + 1;
                    break;
                }
            }
        }
    }

    printf("Найдено: %d\n", found);
    return found;
}

int compare_price(const void* a, const void* b) {
    float pa = ((Software*)a)->price;
    float pb = ((Software*)b)->price;
    return (pa > pb) - (pa < pb);
}