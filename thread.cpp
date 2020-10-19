#include <bits/stdc++.h>
#include <stdio.h>
#include <unistd.h>
using namespace std;
stringstream output_buffer;

string into_Tempelate(int index, int data){
    stringstream ss;
    ss << "\"col_" << index << "\":";
    if(index < 20)
        ss << data << ",\n";
    else ss << data << "\n";
    return ss.str();
}

class thread_out
{
private:
    /* data */
    int flag = 0;
public:
    thread_out(/* args */);
    void out_file(int, int, int);
    vector <vector<int>> num;
    ~thread_out();
};

thread_out::thread_out(/* args */)
{
}

void thread_out::out_file(int turns, int begin, int end){
    stringstream tmp;
    for(int i = begin; i < end; i++){
        tmp << "\t{\n";
        for(int j = 0; j < 20; j++){
            tmp << "\t" << "\t" << into_Tempelate(j+1, num[i][j]);
        }
        if(i != num.size() - 1)
            tmp << "\t},\n";
        else tmp << "\t}\n";
        
    }

    while(turns != flag)
        ;

    output_buffer << tmp.str();

    flag++;
}

thread_out::~thread_out()
{
}

void func(thread_out &out, int turn, int begin, int end){
    out.out_file(turn, begin, end);
}

int main(int argc, char **argv){
    if(argc < 2){
        cout << "please enter the nums of threads\n";
        exit(1);
    }
    thread_out thread_out;
    int num_thread = atoi(argv[1]);
    cout << "Thread's number : " << num_thread << endl;
    double s, e;
    
    s = clock();
    FILE *in = fopen("input.csv", "r");
    FILE *out = fopen("output.json", "w");
    char buf[1000];
    output_buffer << "[\n";
    while(fgets(buf, 1000, in)){
        const char *d = "|";
        char *p = strtok(buf,d);
        vector<int> tmp;
        for(int i = 0; i < 20; i++){
            tmp.push_back(atoi(p));
            p=strtok(NULL,d);
        }
        thread_out.num.push_back(tmp);
    }
    if(num_thread > thread_out.num.size()){
        cout << "Thread's number should less than set's number\n";
        exit(1);
    }
    int set_num = thread_out.num.size() / num_thread;
    cout << "A thread need to deal with " << set_num << " sets of nums\n";
    vector <thread> threads;
    for(int i = 0; i < num_thread; i++){
        threads.push_back(thread(func, ref(thread_out), i, i*set_num, (i*set_num) + set_num));
    }
    for(int i = 0; i < num_thread; i++){
        threads[i].join();
    }

    output_buffer << "]";
    fputs(output_buffer.str().c_str(),out);
    e = clock();
    output_buffer.str("");
    fclose(in);
    fclose(out);
    cout << num_thread << " threads total transfer time ----> " << (e - s) / CLOCKS_PER_SEC << endl;

    return 0;
}