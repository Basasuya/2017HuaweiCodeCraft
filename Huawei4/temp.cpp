const int V=440, E=V*2, maxint=0x3F3F3F3F;

struct etype
{
    int t, c, u;
    etype *next, *pair;
    etype() {}
    etype(int T, int C, int U, etype* N): t(T), c(C), u(U), next(N) {}
    void* operator new(unsigned, void* p){return p;}
} *e[V], Te[E+E], *Pe;

int s, t, piS, cost;
bool v[V];

int aug(int no, int m)
{
    if (no == t) return cost += flow * m, m;
    vis[no] = true;
    int l = m;
    for (int i = head[no]; ~i; i = edge[i].next)
        if ( (edge[i].cap-edge[i].flow) && !edge[i].cost && !vis[edge[i].to])
        {
            int d = aug(edge[i].to, l < (edge[i].cap-edge[i].flow) ? l : (edge[i].cap-edge[i].flow) );
            edge[i].flow += d,
            edge[i^1].flow -= d,
            l -= d;
            if (!l) return m;
        }
    return m - l;
}

bool modlabel()
{
    int d=INF;
    for(int i=s;i<=t;++i)if(vis[i])
        for(int j=head[i]; ~j; j = edge[j].next)
            if( (edge[j].cap-edge[j].flow) && !vis[edge[j].to] && edge[j].cost<d )
              d = edge[j].cost;
    if(d == INF) return false;
    for(int i=s;i<=t;++i)if(vis[i])
        for(int j=head[i]; ~j; j = edge[j].next)
            edge[j].cost -=d,edge[j^1].cost += d;
    flow += d;
    return true;
}
void solvemcmf() {
  do do memset(v,0,sizeof(v));
  while(aug(1,maxint));
  while(modlabel());

}
