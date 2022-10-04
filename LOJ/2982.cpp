#include <iostream>
#include <string.h>
#include <random>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iomanip>
#include <algorithm>
#include <math.h>
#include <cmath>
#include <vector>
#include <stack>
#include <queue>
#include <array>
#include <bitset>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <complex>
#include <valarray>
#include <memory>
#include <cassert>
using namespace std;
//#include <ext/pb_ds/assoc_container.hpp>
//#include <ext/pb_ds/tree_policy.hpp>
//using namespace __gnu_pbds;
//template <class T> using Tree = tree<T, null_type, less<T>, rb_tree_tag, tree_order_statistics_node_update>;
typedef pair<int, int> pii;
typedef pair<int, string> pis;
typedef pair<int, short> pish;
typedef pair<string, string> pss;
typedef pair<int, char> pic;
typedef pair<pii, int> piii;
typedef pair<double, double> pdd;
typedef pair<double, int> pdi;
typedef pair<float, float> pff;
typedef long long ll;
typedef long double ld;
typedef unsigned long long ull;
typedef unsigned int uint;
typedef unsigned short ushort;
typedef pair<uint, uint> puint;
typedef pair<ll, ll> pll;
typedef pair<pll, ll> plll;
typedef pair<pll, ld> plld;
typedef pair<ld, int> pldi;
typedef pair<int, ll> pil;
typedef pair<ll, int> pli;
typedef pair<ull, ull> pull;
typedef pair<ld, ld> pld;
typedef complex<double> cd;
//#define max(n, m) ((n>m)?n:m)
//#define min(n, m) ((n<m)?n:m)
#define f first
#define s second
#define input() ios_base::sync_with_stdio(0);cin.tie(0);cout.tie(0);
//#define eps 1e-8
//#define eps2 1e-15
#define leni(x) sizeof(x)/sizeof(int)
#define v(i,j,k) for(i=j;i<k;i++)
//#define cin fin
//#define cout fout

ifstream fin("/Users/bobby/Downloads/3.in");
ofstream fout("/Users/bobby/Downloads/universalBroadcastOutputs/my3.ans");

const ld eps = 1e-7;
vector<ld> elim(vector<vector<ld>> a, vector<ld> b)
{
    int n = (int)a.size();
    for(int i = 0; i < n; i++)
    {
        int cur = i;
        for(int j = i; j < n; j++)
            if(a[j][i] != 0)
            {
                cur = j;
                break;
            }
        swap(a[i], a[cur]); swap(b[i], b[cur]);
        for (int j = i+1; j < n; j++)
            a[i][j] /= a[i][i];
        b[i] /= a[i][i];
        for(int j = 0; j < n; j++)
            if(j != i && a[j][i] != 0)
            {
                for(int k = i+1; k < n; k++)
                    a[j][k] -= a[i][k]*a[j][i];
                b[j] -= b[i]*a[j][i];
            }
    }
    return b;
}
bool check(vector<vector<ld>> a, vector<ld> b, vector<vector<ld>> ans)
{
    for(int i = 0; i < a.size(); i ++)
    {
        ld cur = 0;
        for(int j = 0; j < a[i].size(); j ++)
            cur += ans[i][j]*a[i][j];
        if(abs(cur-b[i]) > eps) return false;
    }
    return true;
}

int n;
ld nums[11][11], r[11], mult[11];
const ld mv = 0.823;
int main()
{
    input();
    int t;
    fin >> t;
    while(t--)
    {
        fin >> n;
        int totCnt = 0;
        vector<vector<vector<ld>>> ansans;
        for(int i = 0; i < n; i ++)
        {
            for(int j = 0; j < n; j ++)
            {
                fin >> nums[i][j];
                nums[i][j] += mv;
                if(j%2 == 0) nums[i][j] -= eps/10000000;
                else nums[i][j] += eps/10000000;
            }
            fin >> r[i];
        }
        //try all different combinations of ±r
        for(int i = 0; i < (1<<n); i ++)
        {
            vector<ld> curR;
            bool bad = false;
            for(int j = 0; j < n; j ++)
            {
                if((i & (1<<j)) && abs(r[j]) <= eps)
                {
                    bad = true;
                    break;
                }
                curR.push_back((i & (1<<j))?r[j]:-r[j]);
            }
            if(bad) continue;
            vector<vector<ld>> coef;
            for(int j = 0; j < n; j ++)
            {
                vector<ld> cur;
                for(int k = 0; k < n; k ++)
                    cur.push_back(nums[j][k]);
                coef.push_back(cur);
            }
            vector<ld> q = elim(coef, curR);
            curR.clear();
            for(int j = 0; j < n; j ++)
                curR.push_back(1);
            vector<ld> p = elim(coef, curR);
            //now that we have p and q, solve for d
            ld a = 0, b = 0, c = 0;
            for(int j = 0; j < n; j ++)
                a += (p[j]*p[j]), b += (2.0*p[j]*q[j]), c += (q[j]*q[j]);
            c --;
            a = round(a*1e14)/1e14; b = round(b*1e14)/1e14; c = round(c*1e14)/1e14;
            //quadratic formula
            vector<ld> ds;
            if(b*b-4*a*c >= 0 || abs(b*b-4*a*c) <= 1e-13)
                ds = {(-b + sqrt(abs(b*b-4*a*c)))/(2*a), (-b - sqrt(abs(b*b-4*a*c)))/(2*a)};
            //check d1 and d2
            for(auto d : ds)
            {
                vector<vector<ld>> ans;
                curR.clear();
                for(int j = 0; j < n; j ++)
                {
                    curR.push_back(((i & (1<<j))?r[j]:-r[j]) + d);
                    vector<ld> cur;
                    for(int k = 0; k < n; k ++)
                        cur.push_back(d*p[k]+q[k]);
                    ans.push_back(cur);
                }
                bool b = check(coef, curR, ans);
                //if b, that means that the planes are a valid solution
                if(b)
                {
                    //find tangent point given the planes and the spheres
                    vector<vector<ld>> ansss;
                    for(int j = 0; j < n; j ++)
                    {
                        ld kk = 0;
                        for(int k = 0; k < n; k ++)
                            kk += nums[j][k]*ans[j][k];
                        kk -= d;
                        vector<ld> curAn;
                        for(int k = 0; k < n; k ++)
                            curAn.push_back(nums[j][k]-kk*ans[j][k]);
                        ansss.push_back(curAn);
                    }
                    bool bad = false;
                    for(auto it : ansans)
                    {
                        bool curB = true;
                        for(int j = 0; j < n; j ++)
                            for(int k = 0; k < n; k ++)
                                if(abs(it[j][k]-ansss[j][k]) > 5e-7)
                                {
                                    curB = false;
                                    break;
                                }
                        if(curB) { bad = true; break; }
                    }
                    if(!bad)
                    {
                        totCnt ++;
                        ansans.push_back(ansss);
                    }
                }
                if(abs(abs(ds[1])-abs(ds[0])) <= eps) break;
            }
        }
        fout << totCnt << '\n';
        for(auto it2 : ansans)
        {
            for(auto it : it2)
                for(int i = 0; i < it.size(); i ++)
                    fout << fixed << setprecision(14) << it[i]-mv << ((i==it.size()-1)?'\n':' ');
        }
        fout << '\n';
    }
}
