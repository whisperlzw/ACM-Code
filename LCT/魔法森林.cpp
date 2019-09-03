#include <bits/stdc++.h>

using namespace std;

typedef long long ll;

const int N = 300005;

int fa[N], ch[N][2], sta[N], maxx[N];
ll v[N];
bool rev[N];

inline bool get(int x)
{
    return ch[fa[x]][1] == x;
}

inline bool is_root(int x)
{
    return (!fa[x] || ch[fa[x]][1] != x && ch[fa[x]][0] != x);
}

inline void pushr(int x)
{
    swap(ch[x][0], ch[x][1]);
    rev[x] ^= 1;
}

inline void push_up(int x)
{
    int t = v[maxx[ch[x][1]]] > v[maxx[ch[x][0]]]? maxx[ch[x][1]]: maxx[ch[x][0]];
    maxx[x] = v[t] > v[x]? t: x;
}

inline void push_down(int x)
{
    if(rev[x]){
        pushr(ch[x][0]);
        pushr(ch[x][1]);
        rev[x] = 0;
    }
}

inline void rotate(int x)
{
    int y = fa[x], z = fa[y];
    int u = get(x);
    ch[y][u] = ch[x][u^1], fa[ch[x][u^1]] = y;
    if(!is_root(y))
        ch[z][get(y)] = x;
    fa[x] = z;
    ch[x][u^1] = y, fa[y] = x;
    push_up(y), push_up(x);
}

inline void splay(int x)
{
    int pos = 0;
    sta[++pos] = x;
    for(int i = x; !is_root(i); i = fa[i])
        sta[++pos] = fa[i];
    while(pos)
        push_down(sta[pos--]);
    while(!is_root(x)){
        int y = fa[x];
        if(!is_root(y))
            get(x) == get(y)? rotate(y): rotate(x);
        rotate(x);
    }
}

inline void access(int x)
{
    for(int y = 0; x; y = x, x = fa[x])
        splay(x), ch[x][1] = y, push_up(x);
}

inline void make_root(int x)
{
    access(x);splay(x);
    pushr(x);
}

inline void split(int x, int y)
{
    make_root(x);
    access(y);splay(y);
}

inline int find_root(int x)
{
    access(x);splay(x);
    while(ch[x][0]){
        push_down(x);
        x = ch[x][0];
    }
    splay(x);
    return x;
}

int a[N], b[N];

void link(int id)
{
    make_root(a[id]);
    make_root(b[id]);
    fa[a[id]] = id;
    fa[b[id]] = id;
}

int main()
{
    int n, m;
    scanf("%d%d", &n, &m);
    int line = 0;
    ll sum = 0;
    for(int i = n + 1; i <= n + m; i ++){
        int x, y, z;
        scanf("%d%d%d", &x, &y, &z);
        a[i] = x, b[i] = y, v[i] = z;
        maxx[i] = i;
        make_root(x);
        if(find_root(y) != x){
            link(i);
            ++line;
            sum += z;
        }
        else {
            split(x, y);
            int k  = maxx[y];
            if(v[k] > z){
                splay(k);
                fa[ch[k][0]] = fa[ch[k][1]] = 0;
                ch[k][0] = ch[k][1] = 0;
                link(i);
                sum -= v[k] - z;
            }
        }
    }
    if(line == n - 1)
        printf("%lld\n", sum);
    else
        printf("orz\n");
    return 0;
}