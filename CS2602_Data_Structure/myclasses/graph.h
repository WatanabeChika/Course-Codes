#ifndef GRAPH_INCLUDED
#define GRAPH_INCLUDED

#include <iostream>

using namespace std;

class OutofBound{};

template <class elemtype>
class myqueue {
    private:
        elemtype *array;
        elemtype front, rear;
        int maxsize, len;
    public:
        myqueue(int s = 100) {
            array = new char[s];
            front = rear = 0;
            maxsize = s;
            len = 0;
        }
        bool isEmpty() const {
            return (len == 0);
        }
        bool isFull() const {
            return (len == maxsize-1);
        }
        elemtype seeTop() const {
            if (isEmpty()) return;
            return front;
        }
        void enqueue(const elemtype& x) {
            if (isFull()) return;
            array[rear] = x;
            if (rear == maxsize-1) rear = 0;
            else ++rear;
            ++len;
        }
        void dequeue() {
            if (isEmpty()) return;
            if (front == maxsize-1) front = 0;
            else ++front;
            --len;
        }
        ~myqueue() {
            delete []array;
        }
};

template<class edgeType>
struct edgeNode {
    int dest;
    edgeType weight;
    edgeNode *link = NULL;

    edgeNode(int d, edgeType w, edgeNode* l): dest(d), weight(w), link(l) {};
};

template<class verType, class edgeType>
struct verNode {
    verType data;
    edgeNode<edgeType> *adj = NULL;
};

template<class verType, class edgeType>
class graph {
    private:
        bool directed;
        int verts, edges;
        int maxVertex;
        verNode<verType, edgeType> *verList;

        struct EulerNode {
            int num;
            EulerNode *next = NULL;
            EulerNode(int n, EulerNode *l = NULL): num(n), next(l) {}
        }
        struct PrimNode {
            int from, to;
            edgeType weight;
            PrimNode(int f, int t, edgeType w): from(f), to(t), weight(w) {}
        }

        void DFS(int start, bool visited[]) const;
        verNode<verType, edgeType> *clone();
        EulerNode *EulerCircuit(int start, EulerNode *&end);

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
        int BFS() const;

        bool connected() const;
        void EulerCircuit(int start);

        void Prim() const;

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

// 深度优先搜索（递归）
template<class verType, class edgeType>
void graph<verType, edgeType>::DFS(int start, bool visited[]) const
{
    cout << verList[start].data << " ";
    visited[start] == true;
    edgeNode<edgeType> *p = verList[start].adj;

    while (p) {
        if (!visited[p->dest])
            DFS(p->dest, visited);
        p = p->link;
    }
    
}

template<class verType, class edgeType>
void graph<verType, edgeType>::DFS() const
{
    bool visited[verts] = {false};

    for (int i = 0; i < verts; ++i) {
        if (!visited[i]) {
            DFS(i, visited);
            // cout << endl;
        }
    }
}

// 广度优先搜索（非递归）
template<class verType, class edgeType>
int graph<verType, edgeType>::BFS() const
{
    bool visited[verts] = {false};
    myqueue<int> que;
    edgeNode<edgeType> *p;
    int start, count = 0;

    for (int i = 0; i < verts; ++i) {
        if (visited[i]) 
            continue;
        que.enqueue(i);
        ++count;
        while (!que.isEmpty()) {
            start = que.seeTop();
            que.dequeue();
            if (visited[start])
                continue;
            cout << verList[start].data << " ";
            visited[start] = true;
            p = verList[start].adj;
            while (p) {
                if (!visited[p->dest])
                    que.enqueue(p->dest);
                p = p->link;
            }
        }
        // cout << endl;
    }
    return count;
}

template<class verType, class edgeType>
bool graph<verType, edgeType>::connected() const
{
    return (BFS() == 1);
}

template<class verType, class edgeType>
verNode<verType, edgeType> *graph<verType, edgeType>::clone()
{
    verNode<verType, edgeType> *tmp = new verNode[verts];
    edgeNode<edgeType> *p;

    for (int i = 0; i < verts; ++i) {
        tmp[i].data = verList[i].data;
        p = verList[i].adj;
        while (p) {
            tmp[i].adj = new edgeNode(p->dest, p->weight, tmp[i].adj);
            p = p->link;
        }
    }
    return tmp;
}

// 判断并找出欧拉回路
template<class verType, class edgeType>
graph<verType, edgeType>::EulerNode *graph<verType, edgeType>::EulerCircuit(int start, EulerNode *&end)
{
    EulerNode *beg;
    int nextnode;

    beg = end = new EulerNode(start);
    while (verList[start].adj) {
        nextnode = (verList[start].adj)->data;
        beg->next = new EulerNode(nextnode);
        removeEdge(start, nextnode);
        removeEdge(nextnode, start);
        start = nextnode;
        end->next = new EulerNode(start);
        end = end->next;
    }
    return beg;
} 

template<class verType, class edgeType>
void graph<verType, edgeType>::EulerCircuit(int start) 
{
    if (directed || !connected()) return;
    edgeNode<edgeType> *r;
    int degree;
    EulerNode *beg, *end, *p, *q, *tb, *te;
    verNode<verType, edgeType> *tmp = clone();

    for (int i = 0; i < verts; ++i) {
        degree = 0;
        r = verList[i].adj;
        while (r) {
            ++degree;
            r = r->link;
        }
        if (degree == 0 || degree % 2 != 0) return;
    }
    // 拼接
    beg = EulerCircuit(start, end);
    while (true) {
        p = beg;
        while (!p->next) {
            if (verList[p->next->num].adj) 
                break;
            else 
                p = p->next;
        }
        if (!p->next) 
            break;
        q = p->next;
        tb = EulerCircuit(q->num, te);
        te->next = q->next;
        p->next = tb;
        delete q;
    }
    // 输出
    delete []verList;
    verList = tmp;
    while (beg) {
        cout << verList[beg->num].data << " ";
        p = beg;
        beg = beg->next;
        delete p;
    }
    // cout << endl;
}

// 最小代价生成树
template<class verType, class edgeType>
void graph<verType, edgeType>::Prim() const
{
    int source[verts] = -1;
    edgeType dist[verts] = 999999; // if edgeType == int
    bool selected[verts] = false;
    graph<verType, edgeType>::PrimNode treeEdges[verts-1];
    edgeType sum;
    int cnt, min, selVert;
    edgeNode<edgeType> *p;

    source[0] = 0;
    selVert = 0;
    dist[0] = 0;
    selected[0] = true;
    cnt = 1;

    while (cnt < verts) {
        p = verList[selVert].adj;
        while (p) {
            if (!selected[p->dest] && dist[p->dest] > p->weight) {
                source[p->dest] = selVert;
                dist[p->dest] = p->weight;
            }
            p = p->link;
        }
        for (int i = 0; i < verts; ++i) {
            if (!selected[i]) {
                min = i;
                break;
            }
        }
        for (int i = min+1; i < verts; ++i) {
            if (!selected[i] && dist[i] < dist[min]) {
                min = i;
            }
        }

        selVert = min;
        selected[min] = true;
        treeEdges[cnt-1] = graph<verType, edgeType>::PrimNode(source[min], min, dist[min]);
        ++cnt;
    }
}



#endif
