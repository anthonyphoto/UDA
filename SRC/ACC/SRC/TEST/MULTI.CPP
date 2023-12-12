#include <string>
#include <iostream.h>
#include <map>

class PdsnStat {
public:
    typedef map<string, unsigned long, less<string> > counter_type;
    typedef map<string, counter_type, less<string> > stat_type;
public:
    explicit  PdsnStat();
    virtual  ~PdsnStat();
    void      insert(const string& tp, const string& ip);
    void      print();
private:    
    string       start_;
    string       interim_;
    string       stop_;
    string       drop_;
    string       total_;
    stat_type    pdsnTotal_;
};


PdsnStat::PdsnStat() : start_  (string("Start")  )
                     , interim_(string("Interim"))
                     , stop_   (string("Stop")   )
                     , drop_   (string("Drop")   )
                     , total_  (string("Total")  ) {
}

PdsnStat::~PdsnStat() {;}

void PdsnStat::insert(const string& tp, const string& ip) {

    stat_type::iterator keyfound = pdsnTotal_.find(ip);   
    
    counter_type counter;

    if (keyfound != pdsnTotal_.end()) {  // update
        counter = (*keyfound).second;
        pdsnTotal_.erase(keyfound);
    } else {  // register new ip
        counter.insert(counter_type::value_type(start_  , 0));
        counter.insert(counter_type::value_type(interim_, 0));
        counter.insert(counter_type::value_type(stop_   , 0));
        counter.insert(counter_type::value_type(drop_   , 0));
        counter.insert(counter_type::value_type(total_  , 0));
    }

    // increase
    counter_type::iterator found = counter.find(tp);
    if (found != counter.end()) {
        (*found++).second++;
        cout << (*found).second << endl;
    } else {
        cout << "not found" << endl;
    }

    counter_type::iterator beg1 = counter.begin();
    counter_type::iterator end1 = counter.end();
    advance(end1, -1);
    // sum
    unsigned long subTotal = 0;
    while (beg1 != end1) {
        subTotal += (*beg1++).second;
    }

    counter_type::iterator result = counter.end();
    advance(result, -1);
    (*result).second = subTotal;

    pdsnTotal_.insert(stat_type::value_type(ip, counter));
}

void PdsnStat::print() {

    stat_type::iterator beg = pdsnTotal_.begin();
    stat_type::iterator end = pdsnTotal_.end();
    while (beg != end) {
        cout << beg->first << " / ";
        cout << beg->second.find("Start")->second  << " / ";
        cout << beg->second.find("Iterim")->second << " / ";
        cout << beg->second.find("Stop")->second   << " / ";
        cout << beg->second.find("Drop")->second   << " / ";
        cout << beg->second.find("Total")->second  << " / ";
        beg++;
        cout << endl;
    }        
}

int main() {

    PdsnStat ps;
    
    string type1("Drop");
    string ip1("150.2.26.200");
    ps.insert(type1, ip1);

    string type2("Drop");
    string ip2("150.2.26.200");
    ps.insert(type2, ip2);
    
    string type3("Drop");
    string ip3("150.2.26.201");
    ps.insert(type3, ip3);

    string type4("Drop");
    string ip4("150.2.26.202");
    ps.insert(type4, ip4);

    string type5("Drop");
    string ip5("150.2.26.202");
    ps.insert(type5, ip5);

    string type6("Start");
    string ip6("150.2.26.202");
    ps.insert(type6, ip6);

    ps.print();

    return 0;

}
