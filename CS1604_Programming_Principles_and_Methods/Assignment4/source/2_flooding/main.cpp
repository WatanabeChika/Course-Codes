#include <iostream>
#include <Queue.h>
#include <string>
#include <Vector.h>
#include <Grid.h>

using namespace std;
typedef pair<int, int> point;

int n, m;

// Generate the map.
void gen_map(Grid<int>& map, Grid<int>& sign){
    for (int i = 0; i < map.numRows(); ++i){
        for (int j = 0; j < map.numCols(); ++j){
            int heit;
            cin >> heit;
            map[i][j] = heit;
            sign[i][j] = 0;
        }
    }
}

// Initialize the flooded sources.
void init_map(Queue<point>& sources, const Vector<point>& begin, const int& h, const Grid<int>& map, Grid<int>& sign, int& all){
    point thiss;
    for (int i = 0; i < begin.size(); ++i){
        thiss = begin[i];
        if (map[thiss.first][thiss.second] <= h){
            sources.enqueue(thiss);
            sign[thiss.first][thiss.second] = 1;
            ++all;
        }
    }
    
}

// Judge a point should be flooded or not.
void jug_to_fl(Vector<point>& flows, const int& h, Queue<point>& flooded, int& all, const Grid<int>& map, Grid<int>& sign){
    point thats;
    for (int i = flows.size()-1; i >= 0; --i){
        thats = flows[i]; 
        if (thats.first >= n || thats.second >= m || map[thats.first][thats.second] > h || sign[thats.first][thats.second] == 1)
            continue;
        ++all;
        sign[thats.first][thats.second] = 1;
        flooded.enqueue(thats);
    }
}

// Judge the point's position
int jug_pos(const point& star){
    if (star.first == 0){
        if (star.second == 0) return 0;
        if (star.second == m-1) return 1;
        else return 2;
    }
    if (star.first == n-1){
        if (star.second == 0) return 3;
        if (star.second == m-1) return 4;
        else return 5;
    }
    else {
        if (star.second == 0) return 6;
        if (star.second == m-1) return 7;
        else return 8;
    }
}

// Search the map to get the flooded areas.
void srch_map(Queue<point>& flooded, int& all, const int& h, const Grid<int>& map, Grid<int>& sign){
    point right, left, up, down, star;
    Vector<point> flows;
    while (!flooded.isEmpty()){
        star = flooded.dequeue(); 
        right.first = star.first; right.second = star.second + 1;
        down.first = star.first + 1; down.second = star.second;
        left.first = star.first; left.second = star.second - 1;
        up.first = star.first - 1; up.second = star.second;
        switch (jug_pos(star)){
        case 0:
            flows = {right, down};
            jug_to_fl(flows, h, flooded, all, map, sign);
            break;
        case 1:
            flows = {left, down};
            jug_to_fl(flows, h, flooded, all, map, sign);
            break;
        case 2:
            flows = {left, down, right};
            jug_to_fl(flows, h, flooded, all, map, sign);
            break;
        case 3:
            flows = {right, up};
            jug_to_fl(flows, h, flooded, all, map, sign);
            break;
        case 4:
            flows = {left, up};
            jug_to_fl(flows, h, flooded, all, map, sign);
            break;
        case 5:
            flows = {left, up, right};
            jug_to_fl(flows, h, flooded, all, map, sign);
            break;
        case 6:
            flows = {down, up, right};
            jug_to_fl(flows, h, flooded, all, map, sign);
            break;
        case 7:
            flows = {left, up, down};
            jug_to_fl(flows, h, flooded, all, map, sign);
            break;
        case 8:
            flows = {left, up, right, down};
            jug_to_fl(flows, h, flooded, all, map, sign);
            break;
            }
        flows.clear();
    }
}

// main function
int main(){
    Vector<point> begin_sources;
    Queue<point> flooded;
    int k,h,s1,s2,all;
    cin >> n >> m >> k >> h;
    Grid<int> map(n, m), sign(n, m);
    for (int i = 0; i < k; ++i){
        cin >> s1 >> s2;
        point source(s1, s2);
        begin_sources.add(source);
    }
    gen_map(map, sign);
    init_map(flooded, begin_sources, h, map, sign, all);
    srch_map(flooded, all, h, map, sign);
    cout << all << endl;
    return 0;
}