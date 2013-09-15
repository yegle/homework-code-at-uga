#include<string>
using namespace std;

bool check_filename(const char* filename){
    string str = ".java";
    string filename_string = string(filename);
    int check_len = str.size();

    int str_len = str.size();
    int filename_len = filename_string.size();

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
