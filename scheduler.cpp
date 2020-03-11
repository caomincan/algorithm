#include "utils.h"
#include "Heap.h"

struct Job_opt {
    Job_opt(int w, int l):
        weight(w), length(l)
    {}

    Job_opt() : weight(0), length(0) {}

    ~Job_opt() = default;

    double  getKey() const {
        return ((double)weight / (double)length);
    }
    friend bool operator>(const Job_opt& j1, const Job_opt& j2)
    {
        return  j1.getKey() > j2.getKey();
    }
 
    friend bool operator<(const Job_opt& j1, const Job_opt& j2)
    {
        return  j1.getKey() < j2.getKey();
    }

    bool operator==(const Job_opt& other) {
        return (weight == other.weight) && (length == other.length);
    }
    int weight;
    int length;

};

struct Job_diff {
    Job_diff(int w, int l):
        weight(w), length(l)
    {}

    Job_diff() : weight(0), length(0) {}

    ~Job_diff() = default;

    int  getKey() const {
        return weight -length;
    }
    friend bool operator>(const Job_diff& j1, const Job_diff& j2)
    {
        return  j1.getKey() > j2.getKey();
    }

    friend bool operator<(const Job_diff& j1, const Job_diff& j2)
    {
        return j1.getKey() < j2.getKey();
    }
    bool operator==(const Job_diff& other) {
        return (weight == other.weight) && (length == other.length);
    }
    int weight;
    int length;
};


int ReadData(Test::Heap<Job_opt>& heap1, Test::Heap<Job_diff>& heap2, const char* filename)
{
    ifstream data(filename);
    int num = -1;
    if (data.is_open())
    {
        string line;
        getline(data, line);
        num = stoi(line);
        while (getline(data, line))
        {
            int idx = 0;
            while (idx < line.size() && line[idx] != ' ') idx++;
            int w = stoi(line.substr(0, idx));
            int l = stoi(line.substr(idx + 1));         
            heap1.push(Job_opt(w, l));
            heap2.push(Job_diff(w, l));
        }
        assert(heap1.size() == num && heap2.size() == num);
    } else {
        cout << "Fail to open file!" << endl;
    }
    return num;
}

template<class T>
bool findHigherWeight(const T& j1, const T& j2)
{
    return j1.weight > j2.weight;
}

template<class T>
void findSameJobs(Test::Heap<T>& heap, vector<T>& jobs, const T& j)
{
    if (j.getKey() == heap.max().getKey()) {
        while (j.getKey() == heap.max().getKey()) {
            jobs.push_back(heap.max());
            heap.popMax();
        }
        sort(jobs.begin(), jobs.end(), findHigherWeight<T>);
    }
}
void CalculateCompletionTimes()
{
    Test::Heap<Job_opt> heap_opt(true);
    Test::Heap<Job_diff> heap_diff(true);

    const int num = ReadData(heap_opt, heap_diff, "jobs.txt");
    long long compTime_opt = 0;
    long long compTime_diff = 0;
    long long total_length_opt = 0;
    long long total_length_diff = 0;

    while (!heap_diff.isEmpty()) {
        Job_diff j = heap_diff.max(); heap_diff.popMax();
        vector<Job_diff> jobs(1, j);
        findSameJobs<Job_diff>(heap_diff, jobs, j);
        for (auto j : jobs) {
            total_length_diff += j.length;
            compTime_diff += (total_length_diff * j.weight);
        }
    }

    // correct answer: 69119377652
    cout << "weight-length : res = " << compTime_diff << endl;
    while (!heap_opt.isEmpty()) {
        Job_opt j = heap_opt.max(); heap_opt.popMax();        
        // break ties
        vector<Job_opt> jobs(1, j);
        findSameJobs<Job_opt>(heap_opt, jobs, j);     
        // calculate results
        for (auto j : jobs) {
            total_length_opt += j.length;
            compTime_opt += (total_length_opt * j.weight);
        }       
    }

    // correct answer: 67311454237
    cout << "weight/length : res = " << compTime_opt << endl;
}