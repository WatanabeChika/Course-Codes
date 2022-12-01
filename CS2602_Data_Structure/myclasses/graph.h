#ifndef GRAPH_INCLUDED
#define GRAPH_INCLUDED

#include <iostream>

using namespace std;

class OutofBound{};

template<class edgeType>
struct edgeNode {
    int dest;
    edgeType weight;
    edgeNode *link;
};

template<class verType, class edgeType>
struct verNode {
    verType data;
    edgeNode<edgeType> *adj;
};

template<class verType, class edgeType>
class graph {
    private:
        bool directed;
        int verts, edges;
        int maxVertex;
        verNode<verType, edgeType> *verList;

        void DFS(int start, bool visited[]) const;
    public:
        graph(bool d, int max = 1000);

        int numOfVerts() const {return verts;}
        int numOfEdges() const {return edges;}
        int getVert(verType ver) const;
        bool existEdge(verType ver1, verType ver2) const;

        void insertVert(verType ver);
        void insertEdge(verType ver1, verType ver2, edgeType edge); // ver1 -> ver2 if directed
        void removeVert(verType ver);
        void removeEdge(verType ver1, verType ver2);

        void DFS() const;
        void WFS() const;

        ~graph();
};

template<class verType, class edgeType>
graph<verType, edgeType>::graph(bool d, int max)
{
    directed = d;
    verts = 0; edges = 0;
    maxVertex = max;
    verList = new verNode<verType, edgeType>[maxVertex];
}

template<class verType, class edgeType>
graph<verType, edgeType>::~graph()
{
    edgeNode<edgeType> *p;
    for (int i = 0; i < verts; ++i) {
        while (verList[i].adj) {
            p = verList[i].adj;
            verList[i].adj = p->link;
            delete p;
        }
    }
    delete []verList;
}

template<class verType, class edgeType>
int graph<verType, edgeType>::getVert(verType ver) const
{
    for (int i = 0; i < verts; ++i) {
        if (verList[i] == ver)
            return i;
    }
    return -1;
}

template<class verType, class edgeType>
bool graph<verType, edgeType>::existEdge(verType ver1, verType ver2) const
{
    int i = getVert(ver1);
    int j = getVert(ver2);
    if (i == -1 || j == -1) 
        throw OutofBound();
    
    edgeNode<edgeType> *p, *q;
    p = verList[i].adj;
    q = verList[j].adj;
    while (p) {
        if (p->dest == j)
            return true;
        p = p->link;
    }
    while (q) {
        if (q->dest == i)
            return true;
        q = q->link;
    }
    return false;
}

template<class verType, class edgeType>
void graph<verType, edgeType>::insertVert(verType ver)
{
    verNode<verType, edgeType> newver = ver;
    newver.adj = NULL;
    newver.data = ver;
    verList[verts] = newver;
    ++verts;
}

template<class verType, class edgeType>
void graph<verType, edgeType>::insertEdge(verType ver1, verType ver2, edgeType edge)
{
    int ver1_pos = getVert(ver1);
    int ver2_pos = getVert(ver2);
    if (ver1_pos == -1 || ver2_pos == -1)
        return;
    
    edgeNode<edgeType> *p;
    p->dest = ver2_pos;
    p->weight = edge;
    p->link = verList[ver1_pos].adj;
    verList[ver1_pos].adj = p;
    ++edges;

    if (!directed) {
        edgeNode<edgeType> *q;
        q->dest = ver1_pos;
        q->weight = edge;
        q->link = verList[ver2_pos].adj;
        verList[ver2_pos].adj = q;
    }
}

template<class verType, class edgeType>
void graph<verType, edgeType>::removeVert(verType ver)
{
    int ver_pos = getVert(ver);
    if (ver_pos == -1)
        return;
    
    int count = 0;
    edgeNode<edgeType> *p, *q;
    p = verList[ver_pos].adj;
    while (p) {
        q = p;
        p = p->link;
        delete q;
        ++count;
    }

    for (int i = 0; i < verts; ++i) {
        p = verList[i].adj;
        q = NULL;
        while (p) {
            if (p->dest == ver_pos) 
                break;
            q = p;
            p = p->link;
        }
        if (!p) continue;
        if (!q) verList[i].adj = p->link;
        else q->link = p->link;
        delete p;
        ++count;
    }

    for (int j = 0; j < verts; ++j) {
        p = verList[j].adj;
        while (p) {
            if (p->dest > ver_pos) 
                --p->dest;
            p = p->link;
        }
    }
    if (directed) edges -= count;
    else edges -= count/2;

    for (int k = ver_pos+1; k < verts; ++k)
        verList[k-1] = verList[k];
    --verts;
}

template<class verType, class edgeType>
void graph<verType, edgeType>::removeEdge(verType ver1, verType ver2)
{
    int ver1_pos = getVert(ver1);
    int ver2_pos = getVert(ver2);
    if (ver1_pos == -1 || ver2_pos == -1)
        return;

    edgeNode<edgeType> *p, *q;
    p = verList[ver1_pos].adj;
    q = NULL;

    while (p) {
        if (p->dest == ver2_pos)
            break;
        p = p->link;
    }
    if (!q) verList[ver1_pos].adj = p->link;
    else q->link = p->link;
    delete p;

    if (!directed) {
        p = verList[ver2_pos].adj;
        q = NULL;
        while (p) {
            if (p->dest == ver1_pos)
                break;
            p = p->link;
        }
        if (!q) verList[ver2_pos].adj = p->link;
        else q->link = p->link;
        delete p;
    }

    --edges;
}   

template<class verType, class edgeType>
void graph<verType, edgeType>::DFS(int start, bool visited[]) const
{

}

template<class verType, class edgeType>
void graph<verType, edgeType>::DFS() const
{
    
}

#endif
