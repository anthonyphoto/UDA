#include <iostream.h>
#include <vector>
#include <string>
#include <fstream.h>
#include <strstream.h>
#include <iterator>

template <typename Stream>
void csv_read(Stream& S, unsigned int index, vector<string>& vec, char delim = ',') {

    string str;

    strstream sstrm;
    sstrm << S.rdbuf();
    str = sstrm.str();
    delete sstrm.str();
   
    int pos = 0;
    for (int i = 0; i < index; i++) {
        pos = str.find('\n');
        if (pos == string::npos) { break; }
        str.erase(0, pos+1);
    }

    int loc = 0;
    while (true) {
        loc = str.find(delim);
        if (loc == string::npos) { break; }
        vec.push_back(str.substr(0, loc));
        str.erase(0, loc+1);
    }
}


int main() {

    //string s("11ACC01.2001051603.STAT");
    string s("UdaAccount.stat");
    fstream fout(s.c_str(), ios::in|ios::out);
    fout.seekp(0, ios::end);
    cout << fout.tellp() << endl;

    fout << "*****";
    fout.close();
return 0;
    vector<string> vec;

    csv_read(fout, 0, vec, '\t');

    copy(vec.begin(), vec.end(), ostream_iterator<string>(cout, " | "));
    cout << endl;

    fstream f2(s.c_str(), ios::out);
    if (!f2) { cout << "awesome" << endl; }
    f2.close();
    fout.close();
}

