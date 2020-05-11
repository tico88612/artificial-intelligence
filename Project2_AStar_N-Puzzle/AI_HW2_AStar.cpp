/*
    Author: Jerry Yang C.H. (tico88612)
    Date: 2020/4/25
*/
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef pair<ll, ll> pii;
typedef pair<double,double> pdd;
#define SQ(i) ((i)*(i))
#define MEM(a, b) memset(a, (b), sizeof(a))
#define SZ(i) int(i.size())
#define FOR(i, j, k, in) for (int i=j ; i<k ; i+=in)
#define RFOR(i, j, k, in) for (int i=j ; i>=k ; i-=in)
#define REP(i, j) FOR(i, 0, j, 1)
#define REP1(i,j) FOR(i, 1, j+1, 1)
#define RREP(i, j) RFOR(i, j, 0, 1)
#define ALL(_a) _a.begin(),_a.end()
#define MP make_pair
#define PB push_back
#define EB emplace_back
#define X first
#define Y second
#ifdef tmd
#define debug(...) do{\
    fprintf(stderr,"%s - %d (%s) = ",__PRETTY_FUNCTION__,__LINE__,#__VA_ARGS__);\
    _do(__VA_ARGS__);\
}while(0)
template<typename T>void _do(T &&_x){cerr<<_x<<endl;}
template<typename T,typename ...S> void _do(T &&_x,S &&..._t){cerr<<_x<<" ,";_do(_t...);}
template<typename _a,typename _b> ostream& operator << (ostream &_s,const pair<_a,_b> &_p){return _s<<"("<<_p.X<<","<<_p.Y<<")";}
template<typename It> ostream& _OUTC(ostream &_s,It _ita,It _itb)
{
    _s<<"{";
    for(It _it=_ita;_it!=_itb;_it++)
    {
        _s<<(_it==_ita?"":",")<<*_it;
    }
    _s<<"}";
    return _s;
}
template<typename _a> ostream &operator << (ostream &_s,vector<_a> &_c){return _OUTC(_s,ALL(_c));}
template<typename _a> ostream &operator << (ostream &_s,set<_a> &_c){return _OUTC(_s,ALL(_c));}
template<typename _a> ostream &operator << (ostream &_s,deque<_a> &_c){return _OUTC(_s,ALL(_c));}
template<typename _a,typename _b> ostream &operator << (ostream &_s,map<_a,_b> &_c){return _OUTC(_s,ALL(_c));}
template<typename _t> void pary(_t _a,_t _b){_OUTC(cerr,_a,_b);cerr<<endl;}
#define IOS()
#else
#define debug(...)
#define pary(...)
#define endl '\n'
#define IOS() ios_base::sync_with_stdio(0);cin.tie(0)
#endif

const ll MOD = 1000000007LL;
const ll INF = 0x3f3f3f3f3f3f3f3fLL;
const int iNF = 0x3f3f3f3f;
const ll MAXN = 10000000;

int n;
typedef struct State {
    int id;
    int data[4][4];
    int level;
    int prevIndex;
    int f;
    State (){
        id = -1;
        level = -1;
        prevIndex = -1;
        f = -1;
    }
    bool operator == (State b){
        REP(i, n) {
            REP(j, n) {
                if((*this).data[i][j] != b.data[i][j]){
                    return false;
                }
            }
        }
        return true;
    }
    bool operator < (State b){
        return (*this).f < b.f;
    }

    string toString(){
        string ans;
        REP(i, n){
            REP(j, n){
                ans += to_string(data[i][j]);
                ans += " ";
            }
        }
        return ans;
    }
} State ;

struct CompareF{
    bool operator()(State const &p1, State const &p2) {
        return p1.f > p2.f;
    }
};

State totalAllState[MAXN];
int totalStateN = 2;
unordered_set<string> myStateSet;
priority_queue<State, vector<State>, CompareF> pq;

int h(State st, State ed) {
    int ret = 0;
    int gx = 0, gy = 0, x = 0, y = 0;
    for(int i = 1; i < n * n; i++){
        for(int j = 0; j < n; j++){
            for(int k = 0; k < n; k++){
                if (st.data[j][k] == i){
                    x = j;
                    y = k;
                }
                if (ed.data[j][k] == i) {
                    gx = j;
                    gy = k;
                }
            }
        }
        ret += abs(gx - x) + abs(gy - y);
    }
    return ret;
}

int f(State st, State ed, int level)
{
    return h(st, ed) + (level + 1);
}

vector<State> generate_child(State st){
    int i, j;
    int judge = 0;
    for(i = 0; i < n; i++){
        for(j = 0; j < n; j++){
            if(st.data[i][j] == 0){
                judge = 1;
                break;
            }
        }
        if(st.data[i][j] == 0 && judge == 1)
            break;
    }
    int di[4] = {0, 1, 0, -1};
    int dj[4] = {1, 0, -1, 0};
    vector<State> re;
    for(int k = 0; k < 4; k++){
        if(i + di[k] < 0 || i + di[k] >= n || j + dj[k] < 0 || j + dj[k] >= n){
            continue;
        }
        State s = st;
        swap(s.data[i][j], s.data[i + di[k]][j + dj[k]]);
        re.EB(s);
//        for(int l = 0 ; l < n; l++){
//            for(int m = 0 ; m < n; m++){
//                cout << s.data[l][m] << " \n"[m == n - 1];
//            }
//        }
    }
    return re;
}



/********** Good Luck :) **********/
int main()
{
    // IOS();
    cin >> n;
    cout << "Start State:" << '\n';
    REP(i, n){
        REP(j, n){
            cin >> totalAllState[0].data[i][j];
        }
    }
    myStateSet.insert(totalAllState[0].toString());
    

    cout << "End State:" << '\n';
    REP(i, n) {
        REP(j, n) {
            cin >> totalAllState[1].data[i][j];
        }
    }
    totalAllState[0].id = 0;
    totalAllState[0].f = f(totalAllState[0], totalAllState[1], -1);
    // myStateSet.insert(totalAllState[1].toString());
    
    pq.push(totalAllState[0]);
    // cout << SZ(pq) << '\n';
    stack<int> finalPath;
    while(SZ(pq)){
        State cur_node = pq.top();
        pq.pop();
        int nowLevel = cur_node.level;
        int nowId = cur_node.id;
        // cout << SZ(pq) << '\n';
        // cout << h(cur_node, totalAllState[1]) << '\n';
        if (h(cur_node, totalAllState[1]) == 0){
            int t = nowId;
            while(t != 0){
                finalPath.push(t);
                t = totalAllState[t].prevIndex;
            }
            finalPath.push(0);
            break;
        }
        vector<State> children = generate_child(cur_node);
        for(int i = 0; i < SZ(children); i++){
            State now = children[i];
            if (myStateSet.find(now.toString()) == myStateSet.end()){
                if (totalStateN >= MAXN){
                    cout << "Not in " << MAXN << " Completes." << '\n';
                    return 1;
                }
                myStateSet.insert(now.toString());
                now.f = f(now, totalAllState[1], nowLevel + 1);
                now.level = nowLevel + 1;
                now.prevIndex = nowId;
                now.id = totalStateN;
                totalAllState[now.id] = now;
                totalStateN++;
                pq.push(now);
            }
        }
    }
    while(SZ(finalPath)){
        int q = finalPath.top();
        finalPath.pop();
        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                cout << totalAllState[q].data[i][j] << " \n"[j == n - 1];
            }
        }
        cout << "----------------" << '\n';
    }
    cout << "DONE" << '\n';
    cout << "Generate " << totalStateN << '\n';
    return 0;
}
/*
3
1 8 2
0 4 3
7 6 5
1 2 3
4 5 6
7 8 0


4
0 2 6 3
1 9 7 4
5 15 11 8
3 10 14 12
1 2 3 4
5 6 7 8
9 10 11 12
13 14 15 0

3
4 1 8
3 0 5
7 6 2
1 2 3
4 5 6
7 8 0

3
4 1 8
3 0 5
7 6 2
0 1 2
3 4 5
6 7 8
*/