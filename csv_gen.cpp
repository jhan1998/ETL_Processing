#include <iostream>
#include <cstdio>
#include <sstream>
#include <ctime>

using namespace std;
stringstream output;


int main(int argc, char **argv){
    srand(time(NULL));
    int count = atoi(argv[1]);
    FILE *out = fopen("input.csv","w");
    for (int i = 0; i < count; i++)
    {
        for(int j = 0; j < 20; j++){
            int sign = rand() % 2;
            int num = rand();
            int res = (sign == 0) ? num : -num;
            if(j != 19)
                output << res << "|";
            else output << res << "\n";
        }
        fputs(output.str().c_str(),out);
        output.str("");
    }
    fclose(out);
    return 0;
}