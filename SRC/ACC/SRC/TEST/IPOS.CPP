#include <iostream.h>
#include <fstream.h>
#include <iterator>
#include <algorithm>
#include <string>
#include <vector>

template <typename Iterator, typename Stream>
void csv_print(Iterator I, Stream& S) {
    copy(I.begin(), I.end(), ostream_iterator<string>(S, ","));
    S << endl;
}

template <typename Stream>
void csv_read(Stream& S, unsigned int index, vector<string>& vec, char delim = ',') {

    string str;

    for (int i = 0; i <= index; i++) {
        S >> str;
        cout << str << endl;
    }
   
    int loc = 0;

    while (true) {
        loc = str.find(delim);
        if (loc == string::npos) { break; }
        vec.push_back(str.substr(0, loc));
        str.erase(0, loc+1);
    }
}

/*****
void csv_read(fstream& fout, unsigned int index, vector<string>& vec, char delim = ',') {

    string str;

    for (int i = 0; i <= index; i++) {
        fout >> str;
        cout << str << endl;
    }
   
    int loc = 0;

    while (true) {
        loc = str.find(delim);
        if (loc == string::npos) { break; }
        vec.push_back(str.substr(0, loc));
        str.erase(0, loc+1);
    }
}
***/


class AtoI {
public:
    AtoI(const string& s) : withWhat_(s) { ; }
    int operator()(const string& s) { return atoi(withWhat_.c_str()); }
private:
    string withWhat_;
};


int ATOI(const string& s) { return atoi(s.c_str()); }

string LTOA(int i) { return ltoa(i); }

int main() {

    string s("11ACC01.2001051603.STAT");
    fstream fout(s.c_str(), ios::in);

    vector<string> vec;        

    csv_read(fout, 1, vec, ',');

    fout.close();

    vector<string> add;
    add.push_back("100");
    add.push_back("100");
    add.push_back("100");
    add.push_back("100");

    vector<int> result(vec.size());
    transform(vec.begin(), vec.end(), result.begin(), ATOI);

    vector<int> result2(add.size());
    transform(add.begin(), add.end(), result2.begin(), ATOI);

    transform(result.begin(), result.end(), result2.begin(), result.begin(), plus<int>());

    transform(result.begin(), result.end(), vec.begin(), LTOA);


    vector<string>::iterator beg = vec.begin();
    vector<string>::iterator end = vec.end();

    while (beg != end) {
        cout << *beg++ << endl;
    }

    vector<int>::iterator rbeg = result.begin();
    vector<int>::iterator rend = result.end();

    while (rbeg != rend) {
        cout << *rbeg++ << endl;
    }

    return 0;
}

