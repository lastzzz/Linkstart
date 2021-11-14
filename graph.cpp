#include<iostream>
#include<vector>
#include<stack>
using namespace std;

#define MAXNODE 20                  // 定义最大节点数
#define MAX 2000
#define STARTNODE 1                 // 定义最开始节点

int map[MAXNODE + 1][MAXNODE + 1];  // 定义最大矩阵，且起始点为map[1][1]

void dfs(int start, int n){
    int visit[MAXNODE], s_top;      //创建一个标记节点是否访问过的一维数组
    for (int i = 0; i <= MAXNODE; ++i){     // 初始化visit数组
        visit[i] = 0;
    }
    stack<int> stk;                 //利用栈来实现深度优先遍历
    visit[start] = 1;               //将开始节点定义为访问过
    cout << start << " ";           //访问开始节点
    for (int i = 1; i <= n; ++i){                   //将节点1的邻接节点压入栈。
        if (!visit[i] && map[i][start] == 1){                        
            visit[i] = 1;
            stk.push(i);
        }
    }

    while (!stk.empty()){
        s_top = stk.top();
        visit[s_top] = 1;
        cout << s_top << " ";
        stk.pop();
        for (int i = 1; i < n; i++){                    //进行深度优先遍历。
            if (!visit[i] && map[i][s_top] == 1){
                visit[i] = 1;
                stk.push(i);
            }
        }
    }
}

int main(int argc, const char *argv[]){
    int num_edge, num_node;
    int x, y;
    cout << "Please input number of nodes and edges." << endl;
    cin >> num_node >> num_edge;                    //定义节点数，以及邻接的节点数
    for (int i = 0; i < num_node; i++){             //初始化图
        for (int j = 0; j < num_node; ++j){
            map[i][j] = 0;
        }
    }

    for (int i = 1; i <= num_edge; i++){
        cin >> x >> y;                              //定义邻接节点
        map[x][y] = map[y][x] = 1;

    }

    dfs(STARTNODE, num_node);                       //从开始节点开始深度优先遍历
    return 0;
}