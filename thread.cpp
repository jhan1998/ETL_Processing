#include <bits/stdc++.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
using namespace std;
// check commit @jhan1998
class thread_out
{
private:
    /* data */
    int flag = 0;
    int thread_num;
    vector <string> res;
public:
    thread_out(int n) : res(vector<string> (n)){thread_num = n;}
    void out_file(int, int, int);
    vector <vector<int>> num;
    double time = 0;
    string get_res(int index) {return res[index];}
    ~thread_out();
};

void thread_out::out_file(int turns, int begin, int end){
    stringstream tmp;
    for(int i = begin; i < end; i++){
        tmp << "\t{\n";
        for(int j = 0; j < 20; j++){
            j != 19 ? tmp << "\t" << "\t" << "\"col_" << j+1 << "\":" << num[i][j] << ",\n"
                        :tmp << "\t" << "\t" << "\"col_" << j+1 << "\":" << num[i][j] << "\n";
        }
        if(i != num.size() - 1)
            tmp << "\t},\n";
        else tmp << "\t}\n";
        
    }
    res[turns] = tmp.str();
    // while(turns != flag)
    //     ;
    //output_buffer << tmp.str();
    //flag++;
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
    int num_thread = atoi(argv[1]);
    thread_out thread_out(num_thread);
    cout << "Thread's number : " << num_thread << endl;
    struct timeval start, end;
    gettimeofday(&start, 0);

    FILE *in = fopen("input.csv", "r");
    FILE *out = fopen("output.json", "w");
    char buf[1000];
    fputs("[\n",out);
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
    struct timeval thread_s, thread_e;
    gettimeofday(&thread_s, 0);
    vector <thread> threads;
    for(int i = 0; i < num_thread; i++){
        i == (num_thread - 1) ? threads.push_back(thread(func, ref(thread_out), i, i*set_num, thread_out.num.size()))
                            : threads.push_back(thread(func, ref(thread_out), i, i*set_num, (i*set_num) + set_num));
    }
    for(int i = 0; i < num_thread; i++){
        threads[i].join();
        //output_buffer << thread_out.get_res(i);
    }
    gettimeofday(&thread_e, 0);
    long seconds = thread_e.tv_sec - thread_s.tv_sec;
    long microseconds = thread_e.tv_usec - thread_s.tv_usec;
    double elapsed = seconds + microseconds*1e-6;
    cout << "Threads processing time : " << elapsed << endl;

    for(int i = 0; i < num_thread; i++){
        fputs(thread_out.get_res(i).c_str(), out);
    }
    fputs("]", out);
    gettimeofday(&end, 0);
    fclose(in);
    fclose(out);
    seconds = end.tv_sec - start.tv_sec;
    microseconds = end.tv_usec - start.tv_usec;
    elapsed = seconds + microseconds*1e-6;
    cout << num_thread << " threads total transfer time ----> " << elapsed << endl;

    return 0;
}
