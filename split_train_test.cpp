#include <bits/stdc++.h>
using namespace std;

int SIZE;
unordered_set<int> st;
vector<int> all;
ifstream ifs;
ofstream ofs_clear;
ofstream ofs_train;
ofstream ofs_test;

bool check(string &s) {
    for (int i = 0; i < (int)s.size(); ++i) {
        if (s[i] <= 32 || s[i] > 126) return false;
    }
    return true;
}

void clearDataSet() {
    ifs.open("myspace.txt", ios::in);
    ofs_clear.open("myspace_clear.txt", ios::out);

    string buf;
    SIZE = 0;
    while (getline(ifs, buf)) {
        if (!check(buf)) continue;
        ofs_clear << buf << endl;
        SIZE++;
    }
    ifs.close();
    ofs_clear.close();
}

void getTrainAndTest() {
    srand(time(NULL));
    st.clear();
    int n = SIZE / 2;
    while (n--) {
        int x;
        do {
            x = rand() % SIZE;
        }while (st.count(x));
        st.insert(x);
    }

    ifs.open("myspace_clear.txt", ios::in);
    ofs_train.open("myspace_train.txt", ios::out);
    ofs_test.open("myspace_test.txt", ios::out);

    string buf;
    int i = 0;
    while (getline(ifs, buf)) {
        transform(buf.begin(),buf.end(),buf.begin(),::tolower);
        if (st.count(i)) ofs_train << buf << endl;
        else ofs_test << buf << endl;
        ++i;
    }
    cout << "split train and test has done!" << endl;
    cout << SIZE/2 << "train data, " << SIZE-SIZE/2 << "test data" << endl;
    ifs.close();
    ofs_train.close();
    ofs_test.close();
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0); cout.tie(0);

    // 清除不可见字符
    clearDataSet();

    // 生成train和test
    getTrainAndTest();

    return 0;
}
