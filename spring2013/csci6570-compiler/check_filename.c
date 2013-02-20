#include<string.h>
#include<stdbool.h>

_Bool check_filename(const char* filename){
    int check_len = sizeof(".java");
    char *str = ".java";

    int str_len = strlen(str);
    int filename_len = strlen(filename);

    if (filename_len < check_len){
        return false;
    }

    bool ret = true;
    for (int i=0; i<check_len; i++){
        if(str[str_len-i] != filename[filename_len-i]){
            ret = false;
            break;
        }
    }
    return ret;
}
