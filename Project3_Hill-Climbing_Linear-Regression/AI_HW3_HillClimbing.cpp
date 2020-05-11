/*
    Author: Jerry Yang C.H. (tico88612)
    Date: 2020/5/1
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

typedef struct Point {
    double x1, x2, x3, x4, x5, y;
} Point;

double getVarTotal(const vector<Point> &Data, double w0, double w1, double w2, double w3, double w4, double w5) {
    double ret = 0;
    for (Point t : Data) {
        double tmp = t.y - (w0 + w1 * t.x1 + w2 * t.x2 + w3 * t.x3 + w4 * t.x4 + w5 * t.x5);
        tmp = pow(tmp, 2.0);
        // tmp /= (double) SZ(Data);
        ret += tmp;
    }
    return ret;
}

/********** Good Luck :) **********/
int main()
{
    //IOS();
    random_device rd;                        // 種子產生器
    mt19937 gen = std::mt19937(rd());        //使用mt19937引擎
    normal_distribution<double> dis(0.0, 2.0);        //使用平均分佈
    auto randfun = std::bind(dis, gen);
    int n;
    cin >> n;
    vector<Point> enter(n);
    REP(i, n){
        cin >> enter[i].x1 >> enter[i].x2 >> enter[i].x3 >> enter[i].x4 >> enter[i].x5 >> enter[i].y;
    }
    double w0 = 0.0, w1 = 0.0, w2 = 0.0, w3 = 0.0, w4 = 0.0, w5 = 0.0;

    int epochs = 100000000;
    double best_loss = 10000000000000.0;

    for (int i = 0; i < epochs; i++) {
        double w0d = randfun(), w1d = randfun(), w2d= randfun(), w3d = randfun(), w4d = randfun(), w5d = randfun();
        w0 += w0d;
        w1 += w1d;
        w2 += w2d;
        w3 += w3d;
        w4 += w4d;
        w5 += w5d;

        double new_loss = 0;
        new_loss = getVarTotal(enter, w0, w1, w2, w3, w4, w5);
        if (new_loss < best_loss) {
            best_loss = new_loss;
        }
        else {
            w0 -= w0d;
            w1 -= w1d;
            w2 -= w2d;
            w3 -= w3d;
            w4 -= w4d;
            w5 -= w5d;
        }
    }
    printf("y = %7.4lf + %7.4lf * x1 + %7.4lf * x2 + %7.4lf * x3 + %7.4lf * x4 + %7.4lf * x5\n", w0, w1, w2, w3, w4, w5);
    // double total = 0;
    double maxx = -99999999;
    Point maxp = Point();
    double minn = 99999999;
    Point minp = Point();
    double average = 0.0;
    for (Point p: enter) {
        double tmp = w0 + w1 * p.x1 + w2 * p.x2 + w3 * p.x3 + w4 * p.x4 + w5 * p.x5 - p.y;
        average += abs(tmp);
        if (abs(tmp) > maxx) {
            maxx = abs(tmp);
            maxp = p;
        }
        if (abs(tmp) < minn) {
            minn = abs(tmp);
            minp = p;
        }
    }
    average /= n;
    printf("maximum: %lf (%lf, %lf, %lf, %lf, %lf, %lf)\n", maxx, maxp.x1, maxp.x2, maxp.x3, maxp.x4, maxp.x5, maxp.y);
    printf("minimum: %lf (%lf, %lf, %lf, %lf, %lf, %lf)\n", minn, minp.x1, minp.x2, minp.x3, minp.x4, minp.x5, minp.y);
    printf("average: %lf\n", average);
    printf("%lf\n", best_loss);
    return 0;
}
/*
y = -7.8586 +  0.1805 * x1 +  1.3588 * x2 +  1.9825 * x3 +  0.1061 * x4 +  0.9896 * x5
1067.662829
*/
/*
x1 x2 x3 x4 x5 Y
20
149 118 15.8 107 4.3 221.16
214 89 37.7 79 4 254.79
216 49 38.6 50 5.3 174.77
183 105 29 109 2.6 238.56
214 37 20.2 64 9.4 144.55
86 84 23.5 46 2 185.14
214 61 22.2 108 8.1 174.48
137 97 43.6 54 0.9 242.04
138 32 27.1 101 4.7 124.23
104 94 34.6 48 0.9 200.39
176 117 43.5 54 2.7 281.39
199 120 45 72 9.4 295.52
167 68 39.3 50 9.8 206.13
105 64 13.4 91 7.5 143.14
83 28 27.4 82 1.8 119.84
101 57 21.8 101 6.3 147.56
197 40 44.6 63 10 185.70
193 83 33.5 97 5.5 208.00
83 35 36.4 104 5.3 140.65
83 111 27.2 96 1 226.17
*/