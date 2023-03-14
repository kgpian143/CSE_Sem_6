#include <stdio.h>
#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/wait.h>

const int N = 5e5;
const int M = 8e8;
int start_node;
int end_node;
using namespace std;

struct edge
{
    int st;
    int end;
};
// vector<int> graph[N];

map<pair<int, int>, vector<int>> Path;
bool check(edge e, int a, int b)
{
    if (e.st > b || e.st < a)
        return false;
    if (e.end > b || e.end < a)
        return false;
    return true;
}
void make_graph(edge *graph_edges, int n, map<int, vector<int>> &graph)
{
    for (int i = 0; i < n; i++)
    {
        if (check(graph_edges[i], start_node, end_node))
        {
            graph[graph_edges[i].st].push_back(graph_edges[i].end);
            graph[graph_edges[i].end].push_back(graph_edges[i].st);
        }
    }
    // for (int i = start_node; i <=  end_node; i++)
    // {
    //     cout << i << " --- > ";
    //     for (auto it : graph[i])
    //     {
    //         cout << it << " ";
    //     }
    //     cout << endl;
    // }
}

void print_graph(map<int, vector<int>> graph)
{
    for( auto it : graph )
    {
        for( auto itr : graph )
        {
            if( itr.first == it.first )continue; 
            int i = it.first ;
            int j = itr.first ; 
            
            if( Path[{ i , j }].size() == 1 )
            {
                cout << "There is no path between " << i << " and " << j << endl << endl  ;
                continue;
            }
            else 
            {
                cout << " Distance from " << i << " to " << j << " is : " << Path[{ i , j }][0] << endl;
            }
            cout << "Path : ";
            for( int k = 1 ; k < Path[{ i , j }].size() ; k++ )
            {
                cout << Path[{ i , j }][k] << " -> " ;
            }
            cout << i << endl << endl ;
        }
    }
}

void dijikstra(int src, map<int, vector<int>> graph )
{
    // int n = end_node - start_node + 1;
    map<int, int> parent;
    map<int, int> distance;
    map<int, bool> vis;
    distance[src] = 0;
    for (auto it : graph )
    {
        if (it.first == src)
            continue;
        distance[it.first] = INT_MAX;
        vis[it.first] = false;
    }
    set<pair<int, int>> pq;
    pq.insert({0, src});
    auto it1 = pq.begin();
    // cout << it1->first << " " << it1->second << endl ;
    while (!pq.empty())
    {
        auto it = pq.begin();
        pq.erase(it);
        int v = it->second;
        int d = it->first;
        // cout << v << " " << d << endl ;
        if (vis[v])
            continue;
        vis[v] = true;
        for (auto child : graph[v])
        {
            if (child == v)
                continue;
            if (distance[child] > (d + 1))
            {
                // cout << " aese hi " << it->first << " " <<  distance[child] << endl ;
                distance[child] = d + 1;
                parent[child] = v;
                pq.insert({d + 1, child});
            }
        }
    }
    for (auto it : graph )
    {
        if (src == it.first)
            continue;
        Path[{src, it.first}].push_back(distance[it.first]);
        if (distance[it.first] == INT_MAX)
        {
            // 
            continue;
        }
        //
        // cout << " Path : ";
        int k = it.first;
        while (k != src)
        {
            // cout << k << " -> ";
            Path[{src, it.first}].push_back(k);
            k = parent[k];
        }
        // cout << i << "--> " << distance[i] <<  endl ;
    }
}
int main(int argc, char **argv)
{
    int shmid1 = shmget(atoi(argv[1]), M, IPC_CREAT | 0666);
    edge *graph_edges = (edge *)shmat(shmid1, NULL, 0);

    int shmid3 = shmget(atoi(argv[2]), N, IPC_CREAT | 0666);
    int *degrees = (int *)shmat(shmid3, NULL, 0);

    int shmid4 = shmget(atoi(argv[3]), 1, IPC_CREAT | 0666);
    int *total_nodes = (int *)shmat(shmid4, NULL, 0);

    int shmid5 = shmget(atoi(argv[4]), 1, IPC_CREAT | 0666);
    int *total_edges = (int *)shmat(shmid5, NULL, 0);
    // cout << " Total nodes  : " << *total_nodes << endl ;
    start_node = (((atoi(argv[5]) - 1) * (*total_nodes + 1))) / 10;
    end_node = ((atoi(argv[5]) * (*total_nodes + 1))) / 10 - 1;

    // cout << start_node << " " << end_node << endl ;
    int a = start_node;
    int b = end_node;
    int c = *total_nodes;
    int d = *total_edges;
    map<int, vector<int>> graph;
    make_graph(graph_edges, *total_edges, graph);
    for (int i = start_node; i < end_node; i++)
    {
        dijikstra(i, graph);
    }
    print_graph(graph) ;
    while (1)
    {
        for (int i = 0; i < 50; i++)
            cout << endl;
        sleep(10);
        int m = *total_nodes - c;
        if (m == 0)
        {
            // cout << "vinod" << endl  ;
            continue;
        }
        // cout << "vinod" << endl  ;
        a = (((atoi(argv[5]) - 1) * (m))) / 10 + c + 1;
        b = ((atoi(argv[5]) * (m))) / 10 + c;
        for (int i = d + 1; i <= *total_edges; i++)
        {
            if (check(graph_edges[i], a, b))
            {
                graph[graph_edges[i].st].push_back(graph_edges[i].end);
                graph[graph_edges[i].end].push_back(graph_edges[i].st);
            }
        }
        for( int i = a  ; i <= b ; i++ )
        {
             dijikstra( i ,graph ) ;
             for( auto it : graph[i])
             {
                for( auto itr : graph[i])
                {
                    if( it == itr )continue;
                    if( Path[{ it , itr}][0] > Path[{ i , it }][0] + Path[{ i , itr}][0] )
                    {
                        Path[{ it , itr}].clear() ;
                        Path[{ it , itr}].push_back(Path[{ i , it }][0] + Path[{ i , itr}][0]) ;
                        for( int j = 1 ; j < Path[{i , itr}].size() ; j++ )
                        {
                            Path[{ it , itr}].push_back(Path[{i , itr}][j]) ;
                        }
                        Path[{ it , itr}].push_back(i) ;
                        for( int j = Path[{i , itr}].size() - 1 ; j >= 1 ; j-- )
                        {
                            Path[{ it , itr}].push_back(Path[{i , itr}][j]) ;
                        }
                    }
                }
             }
        }
        print_graph(graph) ;
    }

    // dijikstra( start_node + 1 , graph) ;
    shmdt(graph_edges);
    shmdt(total_nodes);
    shmdt(total_edges);
    shmdt(degrees);
    return 0;
}