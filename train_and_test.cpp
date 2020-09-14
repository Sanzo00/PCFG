#include <bits/stdc++.h>
using namespace std;
/*
    s = hel$$666
    P(s) = P(L3S2D3) * P(hel) * P($$) * P(666);
*/
const int N = 10000;
const int max_guesses = 1e9;
unordered_map<string, int> LDS;
unordered_map<string, int> L;
unordered_map<string, int> D;
unordered_map<string, int> S;
unordered_map<int, int> cntLDS;
unordered_map<int, int> cntL;
unordered_map<int, int> cntD;
unordered_map<int, int> cntS;
unordered_set<string> Test;
unordered_set<string> Train;
unordered_set<string> cracked_all;

int produced;
int cracked;
ofstream ofs_XY;

struct pNode{
    string s;
    double p;
    bool operator< (const pNode &t) const{
        return p - t.p > 1e-10;
    }
};

struct Node{
    string s;
    string lds;
    int pos;
    int pivot;
    double p;
    vector<int> idx;
    bool operator< (const Node &t) const{
        return p - t.p < 1e-10;
    }
};

// 存储每个字符串的概率
vector<pNode> vecL[N];
vector<pNode> vecD[N];
vector<pNode> vecS[N];
char getC(char c);
string getLDS(string s, int flag);

void initVec();
int getNum(string &s, int i, int len);
Node getFirstNode(string lds);
void getNextNode(priority_queue<Node> &que, Node& f);
void train(); // 获得训练集
void getTestData(); // 读取测试集
void test(); // 测试

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0); cout.tie(0);

    train();
    getTestData();
    test();

    return 0;
}

char getC(char c) {
    if (isdigit(c)) return 'D';
    else if (isalpha(c)) return 'L';
    else return 'S';
}

string getLDS(string s, int flag) {
    char c = getC(s[0]);
    string t;
    t += s[0];
    int cnt = 1;
    string ret;

    for (int i = 1; i < (int)s.size(); ++i) {
        if (c == getC(s[i])) {
            cnt++;
            t += s[i];
        } else {
            ret += c;
            ret += to_string(cnt);
            // 统计每个单词、数字、特殊字符串出现的次数 和 对应每个出现的次数
            if (flag && getC(t[0]) == 'L') {
                L[t]++;
                cntL[t.size()]++;
            }else if (flag && getC(t[0]) == 'D') {
                D[t]++;
                cntD[t.size()]++;
            }else if (flag && getC(t[0]) == 'S'){
                S[t]++;
                cntS[t.size()]++;
            }
            c = getC(s[i]);
            cnt = 1;
            t.clear();
            t += s[i];
        }
    }
    ret += c;
    ret += to_string(cnt);
    if (flag && getC(t[0]) == 'L') {
        L[t]++;
        cntL[t.size()]++;
    }else if (flag && getC(t[0]) == 'D') {
        D[t]++;
        cntD[t.size()]++;
    }else if (flag && getC(t[0]) == 'S') {
        S[t]++;
        cntS[t.size()]++;
    }
    return ret;
}


// 初始化每个概率
void initVec() {
    pNode tmp;
    int len;
    for (auto &it : L) {
        len = (int)it.first.size();
        tmp = {it.first, it.second * 1.0 / cntL[len]};
        vecL[len].push_back(tmp);
    }
    for (auto &it : D) {
        len = (int)it.first.size();
        tmp = {it.first, it.second * 1.0 / cntD[len]};
        vecD[len].push_back(tmp);
    }
    for (auto &it : S) {
        len = (int)it.first.size();
        tmp = {it.first, it.second * 1.0 / cntS[len]};
        vecS[len].push_back(tmp);
    }
    for (int i = 0; i < N; ++i) {
        if (vecL[i].empty()) continue;
        sort(vecL[i].begin(), vecL[i].end());
    }
    for (int i = 0; i < N; ++i) {
        if (vecD[i].empty()) continue;
        sort(vecD[i].begin(), vecD[i].end());
    }
    for (int i = 0; i < N; ++i) {
        if (vecS[i].empty()) continue;
        sort(vecS[i].begin(), vecS[i].end());
    }
}

// 获得训练集
void train() {
    ifstream ifs;
    ifs.open("myspace_train.txt", ios::in);
    if (!ifs.is_open()) cout << "myspace_train.txt open fail!" << endl;
    string buf;
    while (getline(ifs, buf)) {
        string s = getLDS(buf, true);
        LDS[s]++;
        cntLDS[(int)s.size()]++;
        Train.insert(s);
    }
    ifs.close();
    initVec();
    cout << "training has done!" << endl;
}

// 读取测试集
void getTestData() {
    ifstream ifs;
    ifs.open("myspace_test.txt", ios::in);
    if (!ifs.is_open()) cout << "myspace_test.txt open fail!" << endl;

    // 读取所有的测试数据
    string buf;
    while (getline(ifs, buf)) Test.insert(buf);
    ifs.close();
}

int getNum(string &s, int i, int len) {
    int num = 0;
    while (i < len && isdigit(s[i])) {
        num = num*10 + s[i] - '0';
        ++i;
    }
    return num;
}

Node getFirstNode(string lds) {
    if (produced % 100 == 0) {
        ofs_XY << produced << "," <<  cracked << endl;
    }
    double p = LDS[lds] * 1.0 / cntLDS[lds.size()];
    int len = lds.size();
    vector<int> idx;
    string s;
    for (int i = 0; i < len; i++) {
        if (isdigit(lds[i])) continue;
        int num = getNum(lds, i+1, len);
        if (lds[i] == 'L') {
            idx.push_back(1);
            s += vecL[num][0].s;
            p *= vecL[num][0].p;
        }else if (lds[i] == 'D') {
            idx.push_back(1);
            s += vecD[num][0].s;
            p *= vecD[num][0].p;
        }else {
            idx.push_back(1);
            s += vecS[num][0].s;
            p *= vecS[num][0].p;
        }
    }
    Node tmp;
    tmp.idx = idx;
    tmp.p = p;
    tmp.s = s;
    tmp.lds = lds;
    tmp.pivot = 0;
    tmp.pos = 0;
    return tmp;
}

void getNextNode(priority_queue<Node> &que, Node& f) {
    int k = f.pos-1;
    for (int i = f.pivot; i < (int)f.idx.size(); ++i) {
        int idx = f.idx[i];
        while (isdigit(f.lds[++k]));
        int num = getNum(f.lds, k+1, f.lds.size());
        char c = f.lds[k];
        if (c == 'L' && (int)vecL[num].size() == idx) continue;
        else if (c == 'D' && (int)vecD[num].size() == idx) continue;
        else if (c == 'S' && (int)vecS[num].size() == idx) continue;
        Node tmp;
        tmp.lds = f.lds;
        tmp.p = f.p;
        if (c == 'L') {
            tmp.p /= vecL[num][idx-1].p;
            tmp.p *= vecL[num][idx].p;
        }else if (c == 'D') {
            tmp.p /= vecD[num][idx-1].p;
            tmp.p *= vecD[num][idx].p;
        }else {
            tmp.p /= vecS[num][idx-1].p;
            tmp.p *= vecS[num][idx].p;
        }
        tmp.idx = f.idx;
        tmp.idx[i]++;
        string s;
        int kk = -1;

        for (int j = 0; j < (int)f.idx.size(); j++) {
            while (isdigit(f.lds[++kk]));
            num = getNum(f.lds, kk+1, f.lds.size());
            if (tmp.lds[kk] == 'L') s += vecL[num][tmp.idx[j]-1].s;
            else if (tmp.lds[kk] == 'D') s += vecD[num][tmp.idx[j]-1].s;
            else s += vecS[num][tmp.idx[j]-1].s;
        }
        tmp.s = s;
        tmp.pivot = i;
        tmp.pos = k;
        que.push(tmp);
        produced++;
        if (produced % 1000 == 0) {
            ofs_XY << produced << "," <<  cracked << endl;
        }
        if (produced % 1000000 == 0) {
            cout << produced/1000000 << " millions has produced, " << cracked << " has cracked!" << endl;
        }
        if (produced >= max_guesses) return;
    }
}

void test1() {
    cout << Test.size() << " test data start testing..." << endl;

    for (auto s : Test) {
        char c = getC(s[0]);
        string t;
        t += s[0];
        int flag = true;
        for (int i = 1; i < (int)s.size(); ++i) {
            if (c == getC(s[i])) {
                t += s[i];
            } else {
                if (c == 'L' && L.find(t) == L.end()) {
                    flag = false;
                    break;
                }else if (c == 'D' && D.find(t) == D.end()) {
                    flag = false;
                    break;
                }else if (c == 'S' && S.find(t) == S.end()) {
                    flag = false;
                    break;
                }
                c = getC(s[i]);
                t.clear();
                t += s[i];
            }
        }
        if (c == 'L' && L.find(t) == L.end()) flag = false;
        if (c == 'D' && D.find(t) == D.end()) flag = false;
        if (c == 'S' && S.find(t) == S.end()) flag = false;
        if (flag) cracked_all.insert(s);
    }
    cout << "done!" << endl;
}

// 测试
void test() {

    // test1();
    // cout << "crack_all size : " << cracked_all.size() << endl;

    ofs_XY.open("XY1.txt", ios::out);
    cout << Test.size() << " test data start testing..." << endl;
    priority_queue<Node> que;

    for (auto it : Train) {
        if (LDS.find(it) == LDS.end()) continue;
        Node tmp = getFirstNode(it);
        produced++;
        que.push(tmp);
    }

    while (!que.empty() && !Test.empty()) {
        Node f = que.top();
        que.pop();

        if (Test.count(f.s)) {
            cracked++;
            Test.erase(f.s);
        }
        if (produced < max_guesses) getNextNode(que, f);
    }

    cout << "done!" << endl;
    ofs_XY.close();
}
