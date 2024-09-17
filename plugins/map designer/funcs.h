#ifndef LE_MAPDESIGNER_FUNCS_H
#define LE_MAPDESIGNER_FUNCS_H

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <math.h>

#ifdef __unix__
#define FILE_SEP '/'
#define FILE_SEP_STR "/"
#elif defined(_WIN32)
#define FILE_SEP '\\'
#define FILE_SEP_STR "\\"
#else
#warning File Separator Not Deduced, It Will Therefore Be Taken As A Default To '/'
#define FILE_SEP '/'
#define FILE_SEP_STR "/"
#endif

#define FIRST(X, ...) X

#define REST(X, ...) __VA_ARGS__

#define CALL_RECURSIVE(FUN, ...) FUN(FIRST(__VA_ARGS__)) FUN(REST(__VA_ARGS__))

#define PATH(DIR, X) DIR FILE_SEP_STR X

size_t size_of_strsE(const char* ignore, ...){
    va_list args;
    va_start(args, ignore);
    const char* current = NULL;
    size_t size = 0;
    do{
        current = va_arg(args, const char*);
        if(current == NULL) break;
        int i = 0;
        for(; current[i]; i+=1){
        }
        size += i;
    }while (current);
    va_end(args);
    return size;
}

int get_mother_dir_size(const char* file_path){
    int size_file_path = 0;
    int last_slash = 0;
    while (file_path[size_file_path]){
        if(file_path[size_file_path] == FILE_SEP){
            last_slash = size_file_path;
        }
        size_file_path += 1;
    }
    return last_slash + 1;
}

void cap_str(const char* str, char* output, int size){
    for(int i = 0; str[i] && i < size; i+=1){
        output[i] = str[i];
    }
    output[size] = '\0';
}

void concatonate(char* output, const char* str1, const char* str2){
    int i = 0;
    while(str1[i]){
        output[i] = str1[i];
        i += 1;
    }
    int j = 0;
    while(str2[j]){
        output[i + j] = str2[j];
        j += 1;
    }
    output[i + j] = '\0';
}

int n_digits(int input){
    int counter = input < 0;
    input = abs(input);
    while (input)
    {
        input /= 10;
        counter += 1;
    }
    return counter;    
}

int to_string(int input, char* output, size_t size){
    if(input == 0){
        output[0] = '0';
        output[1] = '\0';
        return 0;
    }
    bool isnegative = false;
    if(input < 0){
        isnegative = true;
        input = -input;
        output[0] = '-';
    }
    int counter = isnegative;
    for(; input && counter < size; input /= 10){
        int digit = input - (int)(input / 10) * 10;
        output[size -1 - counter] = '0' + digit;
        counter += 1;
    }
    output[size] = '\0';
    return size - counter;
}

void write_to_file(FILE* file, const char* __restrict__ string, ...){
    bool reading = false;
    va_list args;
    va_start(args, string);
    for(int i = 0; string[i]; i += 1){
        if(reading){
            switch (string[i])
            {
            case 'i':
                int n = va_arg(args, int);
                int ddigits = n_digits(n);
                char* str = (char*)alloca((ddigits + 1) * sizeof(char));
                to_string(n, str, ddigits);
                fputs(str, file);
                break;
            case 'c':
                fputc((char)va_arg(args, int), file);
                break;
            case 's':
                fputs(va_arg(args, const char*), file);
                break;
            default:
                printf("[ERROR] Unimplemented Format For 'write_file', '%c%c'.\n", '%', string[i]);
                exit(EXIT_FAILURE);
                break;
            }
            reading = false;
            continue;
        }
        if(string[i] == '%'){
            reading = true;
            continue;
        } else {
            fputc(string[i], file);
            reading = false;
        }
    }
    va_end(args);
}

static inline void save_to_file(FILE* file, int* map, size_t sizex, size_t sizey){
    if(!sizex) return;
    write_to_file(file, "#define SIZEX %i\n"
    "#define SIZEY %i\n\n"
    "const int map[] = {\n\t",
    sizex, sizey);

    for(size_t i = 0; i < sizey; i+=1){
        for(size_t j = 0; j < sizex; j += 1){ 
            if(j == sizex - 1){
                int ddigits = n_digits(map[j + i * sizex]);
                char* str = (char*)alloca(sizeof(char) * (ddigits + 1));
                to_string(map[j + i * sizex], str, ddigits);
                fputs(str, file);
            } else {
            int ddigits = n_digits(map[j + i * sizex]);
            char* str = (char*)alloca(sizeof(char) * (ddigits + 1));
            to_string(map[j + i * sizex], str, ddigits);
            fputs(str, file);
            fputs(", ", file);
            }
        }
        if(i && i < sizey - 1){
            fputs(", ", file);
        }
        fputs("\n\t", file);
    }
    fputs("\n};", file);
}

#define SIZE_OF_STRS_E(size, ...) size = size_of_strsE("IGNORE", __VA_ARGS__, NULL)

#define COMPILE(FILE) do{\
    int size_of_md = get_mother_dir_size(FILE);\
    char* mother_dir = (char*)alloca(size_of_md * sizeof(char));\
    cap_str(FILE, mother_dir, size_of_md - 1);\
    char* output_file = (char*)alloca((size_of_md) * sizeof(char) + sizeof("/map_designer.so'") );\
    concatonate(output_file, mother_dir, "/map_designer.so'");\
    SIZE_OF_STRS_E(int size_of_command, "gcc -fPIC -shared '" FILE "' -o '", output_file);\
    char* command = (char*)alloca(size_of_command * sizeof(char));\
    concatonate(command, "gcc -fPIC -shared '" FILE "' -o '", output_file);\
    printf("[INFO] Requested Compilation: %s\n", command);\
    if(system(command)){\
        printf("[WARNING] Compilation Failed\n");\
        SDL_SetWindowTitle(map_designer.window, "Le: Map Designer *compilation fail");\
    } else{\
        printf("[INFO] Compilation Success, No Errors, Possible Warnings\n");\
        SDL_SetWindowTitle(map_designer.window, "Le: Map Designer *compilation no errors");\
    }}while (0)

int to_int(const char* str, int size){
    int output = 0;
    for(int i = 0; i < size; i+=1){
        if(str[i] == ' ' || str[i] == '\n') continue;
        if(str[i] < '0' || str[i] > '9'){
            printf("[WARNING] Can't Convert '%s' to int, Invalid Character '%c'.\n", str, str[i]);
            return 0;
        }
        output += (str[i] - '0') * (int) pow(10, size - 1 - i);
    }
    return output;
}

#define REPORT printf(\
        "[INFO] MapDesigner:\nheld tile: %u\n"\
        "mouse position (%.2f, %.2f)\nscroll (%.2f, %.2f)\n"\
        "map_size (%lu, %lu)\nmap canvas (%u, %u, %u, %u)\n"\
        "selection (%u, %u, %u, %u)",\
        (int)map_designer.held_tile,\
        map_designer.mousex, map_designer.mousey, map_designer.scrollx, map_designer.scrolly,\
        map_designer.map_sizex, map_designer.map_sizey,\
        map_designer.map_canvas.x, map_designer.map_canvas.y,\
        map_designer.map_canvas.w, map_designer.map_canvas.h,\
        map_designer.selection.x, map_designer.selection.y,\
        map_designer.selection.w, map_designer.selection.h\
    )



extern void compile();

#endif // END OF FILE ======================================================