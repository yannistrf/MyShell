#include <string.h>

int pressed_enter(char* line) {
    if (strlen(line) == 1)
        return 1;

    return 0;
}

char* remove_spaces(char* str) {
    while(*str == ' ')
        str++;

    int end = strlen(str)-1;
    while (str[end] == ' ')
        end--;
    
    str[end+1] = '\0';
    return str;
}