#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;

struct solution{
    vector<int> sol;
    vector<vector<int>> path;

    solution(vector<int> _sol, vector<vector<int>> _path){
        sol = _sol;
        path = _path;
    }
};

ofstream out("output.out");

vector<int> v(17), target(17);
vector<vector<int>> quads = {{1,2,5,6}, {3,4,7,8}, {9,10,13,14}, {11,12,15,16}};
vector<solution> posibilities;
map<vector<int>, int> used;
vector<int> placeableTiles = {1,2,3,5,6,7,9,10,11};


/// O(16)
vector<int> getTiles(vector<int> v){
    vector<int> res;
    for(vector<int> q : quads){
        int r = 0;
        for(int k : q){
            if(v[k] == 0) continue;

            if(r == 0) r = v[k];
            else if(v[k] != r){
                r = 0;
                break;
            }
        }
        res.push_back(r);
    }
    return res;
}

/// O(4)
inline int canPlace(vector<int> current, vector<int> toPlace, int i){
    int step = 1;
    for(int k : toPlace){
        if(k != 0 && current[i] != 0) return 0;
        i += ((step % 2 == 1) ? 1 : 3);
        step++;
    }
    return 1;
}

/// O(1)
inline vector<int> adjacent(int i){
    vector<int> r;
    if(i-4 >= 1) r.push_back(i-4);
    if(i%4 != 0) r.push_back(i+1);
    if(i%4 != 1) r.push_back(i-1);
    if(i+4 <= 16) r.push_back(i+4);
    return r;
}

/// O(16)
vector<int> place(vector<int> current, vector<int> toPlace, int i){
    int step = 1;
    vector<int> result = current;
    vector<int> justPlaced(17);
    vector<int> toRemove(17);
    for(int k : toPlace){
        if(k != 0){
            int ok = 1;
            vector<int> adj = adjacent(i);
            for(int l : adj)
                if(result[l] == k && justPlaced[l] == 0) toRemove.push_back(l), ok = 0;

            if(ok)
                result[i] = k, justPlaced[i] = 1;
        }
        i += ((step % 2 == 1) ? 1 : 3);
        step++;
    }
    for(int k : toRemove)
        result[k] = 0;
    return result;
}

/// O(180 ^ steps)
void generateTiles(vector<int> current, int steps, vector<vector<int>> path){
    /*if(current == target){
        cout << steps;
        exit(0);
    }*/

    path.push_back(current);

    if(steps == 5){
        if(used[current] == 0){
            posibilities.push_back(solution(current, path));
            used[current] = 1;
        }
        return;
    }

    vector<int> toPlace = getTiles(current);
    for(int k : placeableTiles){
        if(canPlace(current, toPlace, k)){
            vector<int> res = place(current, toPlace, k);
            generateTiles(res, steps+1, path);
        }
    }
}

void write(vector<int> v){
    for(int i=1; i<=16; i++){
        out << v[i] << ' ';
        if(i % 4 == 0) out << endl;
    }
}

int isNull(vector<int> v){
    for(int i=1; i<=16; i++)
        if(v[i] != 0) return 1;
    return 0;
}

inline int zeros(vector<int> v){
    int r = 0;
    for(int i=1; i<=16; i++)
        r += (v[i] == 0);
    return r;
}

bool cmp(solution p1, solution p2){
    return zeros(p1.sol) > zeros(p2.sol);
}

int main(){
    for(int i=1; i<=16; i++)
        cin >> v[i];
    /*for(int i=1; i<=16; i++)
        cin >> target[i];*/


    generateTiles(v, 0, {});


    sort(posibilities.begin(), posibilities.end(), cmp);

    int n = posibilities.size();
    cout << n;
    for(int i=0; i < n; i++){
        write(posibilities[i].sol);
        for(vector<int> v : posibilities[i].path){
            out << endl;
            write(v);
        }
        out << "\n\n##########################\n\n";
    }
}
