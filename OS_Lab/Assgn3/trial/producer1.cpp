#include <stdio.h>
#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/wait.h>

const int N = 4e4;
const int M = 8e8;
int *total_edges;
int *total_nodes;
using namespace std;

struct Node
{
    int val;
    struct Node *next;
};

int main(int argc, char **argv)
{
    srand(time(NULL)); // seed the random number generator with the current time
    // key_t k1 = 235, k2 = 236;
    // int *total_nodes;
    // int atoi(argv[1]) = shmget(k1, 40000 , IPC_CREAT | 0666);
    // struct Node **graph_nodes = (struct Node **)shmat(atoi(argv[1]), NULL, 0);
    // int argv[2] = shmget(k2, M, IPC_CREAT | 0666);
    // struct Node *graph_edges = (struct Node *)shmat(atoi(argv[1]), NULL, 0);
    // int shmid3 = shmget(k3, N, IPC_CREAT | 0666);
    
    

    key_t k1 = 240, k2 = 236 , k3 = 237 , k4 =  238 , k5 = 239 ;
    // int *max_node ;
    int shmid1 = shmget(k1, N , IPC_CREAT | 0666);
    struct Node *graph_nodes = (struct Node *)shmat(shmid1, NULL, 0);
    int shmid2 = shmget(k2, M, IPC_CREAT | 0666);
    struct Node *graph_edges = (struct Node *)shmat(shmid2, NULL, 0);
    int shmid3 = shmget(k3, N, IPC_CREAT | 0666);
    int *degrees = (int *)shmat(shmid3, NULL, 0);
    int shmid4 = shmget(k4, 1 , IPC_CREAT | 0666);
    total_nodes = (int *)shmat(shmid4, NULL, 0);
    int shmid5 = shmget(k5, 1 , IPC_CREAT | 0666);
    total_edges = (int *)shmat(shmid5, NULL, 0);

    // int *degrees = (int *)shmat(atoi(argv[2]), NULL, 0);
    // total_nodes = (int *)shmat(atoi(argv[3]), NULL, 0);
    // total_edges = (int *)shmat(atoi(argv[4]), NULL, 0);
    // int m = rand() % 21 + 10;
    // cout << m << endl;
     for( int i = 0 ; i < 4e4; i++ )
    {
        struct Node *p = graph_nodes[i].next ;
        if( p == NULL)break ;
        //  cout << graph_nodes[i].val << endl ; 
        cout << " degree  : " << degrees[i] << endl ;
        // cout << i << " --->  " ;
        // while( p != NULL)
        // {
        //     cout << p->val << " " ;
        //     p = p->next ;
        // }
        cout << endl ;
    }

    // for (int i = 0; i < m; i++)
    // {
    //     int k = rand() % 20 + 1;
    //     cout << k << endl;
    //     int curr_ver = *total_nodes + 1;
    //     vector<double> probabilities;
    //     for (int i = 0; i < *total_nodes; i++)
    //     {
    //         double prob = degrees[i] * (0.5) / (*total_edges);
    //         probabilities.push_back(prob);
    //         cout << prob << " ";
    //     }
    //     std::random_device rd;
    //     std::mt19937 gen(rd());
    //     std::discrete_distribution<> d(probabilities.begin(), probabilities.end());
    //     unordered_set<int> numbers;
    //     while (numbers.size() < k)
    //     {
    //         int number = d(gen);
    //         if (numbers.find(number) == numbers.end())
    //         {
    //             numbers.insert(number);
    //         }
    //     }
    //     for (auto it : numbers)
    //     {
    //         cout << it << " ";
    //         int st = it;
    //         int end = curr_ver;
    //         struct Node i, j;
    //         i.val = st;
    //         i.next = NULL;
    //         j.val = end;
    //         j.next = NULL;
    //         int curr_edge = 2 * (*total_edges);
    //         graph_edges[curr_edge] = i;
    //         degrees[st]++;
    //         degrees[end]++;
    //         // graph_edges++ ;
    //         graph_edges[curr_edge + 1] = j;
    //         if (graph_nodes[end] == NULL)
    //             graph_nodes[end] = graph_edges + curr_edge;
    //         else
    //         {
    //             struct Node *temp = graph_nodes[end];
    //             graph_nodes[end] = graph_edges + curr_edge;
    //             graph_nodes[end]->next = temp;
    //             // i.next = temp ;
    //         }
    //         if (graph_nodes[st] == NULL)
    //             graph_nodes[st] = graph_edges + curr_edge + 1;
    //         else
    //         {
    //             struct Node *temp = graph_nodes[st];
    //             graph_nodes[st] = graph_edges + curr_edge + 1;
    //             graph_nodes[st]->next = temp;
    //         }
    //         *total_edges = *total_edges + 1;
    //     }
    //     *total_nodes = *total_nodes + 1;
    // }
    // for (int i = 0; i < 300000; i++)
    // {
    //     struct Node *p = graph_nodes[i];
    //     if (p == NULL)
    //         break;
    //     cout << " degree  : " << degrees[i] << endl;
    //     cout << i << " --->  ";
    //     while (p != NULL)
    //     {
    //         cout << p->val << " ";
    //         p = p->next;
    //     }
    //     cout << endl;
    // }
    // struct Node *ed = graph_edges ;
    // while( ed != NULL) 
    // {
    //     cout << ed->val << endl ;
    //     ed  = ed->next ;
    // }
    for( int i = 0 ; i < 5000 ; i++ )cout<<graph_edges[i].next->val << endl;
    // cout << "total nodes : " <<  *total_nodes << endl ;
    for( int i = 0 ; i < 4000 ; i++ )cout << degrees[i] << endl ;
    shmdt(graph_edges);
    shmdt(graph_nodes); 
    shmdt(degrees);
    shmdt(total_nodes);
    shmdt(total_edges);
    //    shmctl( atoi(argv[1]) , IPC_RMID , NULL);
    //    shmctl( atoi(argv[1]) , IPC_RMID , NULL);
    return 0;
}