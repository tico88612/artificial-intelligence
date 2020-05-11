/*
    Author: Jerry Yang C.H. (tico88612)
    Date: 2020/4/3
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
// const ll MAXN =

class Edge;

class Node {
   public:
    string name;
    vector<Edge> children;
    Node() {
        name = "";
    }
    Node(string n) : name(n) {}
    void add_child(Node b,int cost);
    friend ostream &operator<<(ostream &out, const Node &c);
};

class Edge {
   public:
    Node source, destination;
    int cost;
    Edge(Node s, Node t, int c) : source(s), destination(t), cost(c) {}
    friend ostream &operator<<(ostream &out, const Edge &c);
};

void Node::add_child(Node b, int cost){
    children.push_back(Edge(*this, b, cost));
}

ostream &operator<<(ostream &out, const Edge &c) {
    out << c.source.name << " " << c.destination.name << " " << c.cost;
    return out;
}

ostream &operator<<(ostream &out, const Node &c) {
    out << c.name << '\n';
    REP(i, SZ(c.children)){
        out << c.children[i] << '\n';
    }
    return out;
}

string cityName[] = {
    "Avignon",
    "Brest",
    "Brodeaux",
    "Caen",
    "Calais",
    "Dijon",
    "Grenoble",
    "Limoges",
    "Lyon",
    "Marseille",
    "Montpellier",
    "Nancy",
    "Nantes",
    "Nice",
    "Paris",
    "Rennes",
    "Strasbourg",
    "Toulouse"
};
vector<string> vecOfStr(cityName, cityName + sizeof(cityName) / sizeof(std::string));

map<string, Node> m;

vector<string> nowPath, smallPath;

int nowWeight, smallWeight = iNF;

bool DLS(string src, string dest, int limit){
    if (src == dest) {
        nowPath.push_back(src);
        if(nowWeight < smallWeight){
            smallWeight = nowWeight;
            smallPath = nowPath;
        }
        nowPath.pop_back();
        return true;
    }
    if(limit <= 0){
        return false;
    }
    bool Judge = false;
    for (auto i = m[src].children.begin(); i != m[src].children.end(); i++) {
        nowPath.push_back(src);
        nowWeight += (*i).cost;
        if(Judge == true){
            bool tmp = DLS((*i).destination.name, dest, limit - 1);
        }
        else{
            Judge = DLS((*i).destination.name, dest, limit - 1);
        }
        // if (DLS((*i).destination.name, dest, limit - 1) == true) {
        //     nowPath.pop_back();
        //     nowWeight -= (*i).cost;
        //     return true;
        // }
        nowWeight -= (*i).cost;
        nowPath.pop_back();
    }
    return Judge;
}

bool IDDFS(string src, string dest, int MAX_DEPTH)
{
    for(int i = 0; i < MAX_DEPTH; i++){
        bool result = DLS(src, dest, i);
        if(result){
            // cout << i << " YEEE" << '\n';
            return true;
        }
    }
    return false;
}

/********** Good Luck :) **********/
int main()
{
    // IOS();
    for (string i : vecOfStr) {
        m[i] = Node(i);
    }
    int n;
    cin >> n;
    while(n--){
        string s, t;
        int c;
        cin >> s >> t >> c;
        m[s].add_child(m[t], c);
        m[t].add_child(m[s], c);
    }
    int M;
    cin >> M;
    while(M--){
        string a, b;
        int c;
        cin >> a >> b >> c;
        smallWeight = iNF;
        smallPath.clear();
        bool judge = IDDFS(a, b, c);
        
        if(judge){
            if (SZ(smallPath)) {
                REP(i, SZ(smallPath)) {
                    cout << smallPath[i] << (i == SZ(smallPath) - 1 ? "\n" : " > ");
                }
                cout << "Weight = " << smallWeight << '\n';
            } else {
                cout << "Not Found" << '\n';
            }
        } else {
            cout << "Not Found" << '\n';
        }
    }
    return 0;
}

/*
28
Avignon Grenoble 227
Avignon Lyon 216
Avignon Marseille 99
Avignon Montpellier 121
Brest Rennes 244
Brodeaux Limoges 220
Brodeaux Nantes 329
Brodeaux Toulouse 253
Caen Calais 120
Caen Paris 241
Caen Rennes 176
Calais Nancy 534
Calais Paris 297
Dijon Lyon 192
Dijon Nancy 201
Dijon Paris 313
Dijon Strasbourg 335
Grenoble Lyon 104
Limoges Lyon 389
Limoges Paris 396
Limoges Toulouse 313
Marseille Nice 188
Montpellier Toulouse 240
Nancy Paris 372
Nancy Strasbourg 145
Nantes Limoges 329
Nantes Rennes 329
Paris Rennes 348
2
Avignon Grenoble 2
Brest Nice 8
*/