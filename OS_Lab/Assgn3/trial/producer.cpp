#include <stdio.h>
#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/wait.h>

const int N = 4e4;
const int M = 8e8;
// int *total_edges;
// int *total_nodes;
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
    
    

     key_t k1 = 235, k2 = 236 , k3 = 237 , k4 =  238 , k5 = 239 ;
    // int *max_node ;
    int shmid1 = shmget(k1, N , IPC_CREAT | 0666);
    if( shmid1 == -1 )
    {
        perror("Shmget failure \n");
        exit(EXIT_FAILURE);
    }
    struct Node **graph_nodes = (struct Node **)shmat(shmid1, NULL, 0);
    if( graph_nodes == ( struct Node **)-1)
    {
        perror("Shmat error ");
        exit(EXIT_FAILURE) ;
    }
    int shmid2 = shmget(k2, M, IPC_CREAT | 0666);
    if( shmid2 == -1 )
    {
        perror("Shmget failure \n");
        exit(EXIT_FAILURE);
    }
    struct Node *graph_edges = (struct Node *)shmat(shmid2, NULL, 0);
    int shmid3 = shmget(k3, N, IPC_CREAT | 0666);
    if( shmid3 == -1 )
    {
        perror("Shmget failure \n");
        exit(EXIT_FAILURE);
    }
    int *degrees = (int *)shmat(shmid3, NULL, 0);
    int shmid4 = shmget(k4, 1 , IPC_CREAT | 0666);
    if( shmid4 == -1 )
    {
        perror("Shmget failure \n");
        exit(EXIT_FAILURE);
    }
    int *total_nodes = (int *)shmat(shmid4, NULL, 0);
    int shmid5 = shmget(k5, 1 , IPC_CREAT | 0666);
    if( shmid5 == -1 )
    {
        perror("Shmget failure \n");
        exit(EXIT_FAILURE);
    }
    int *total_edges = (int *)shmat(shmid5, NULL, 0);
    cout << *total_edges << " total nodes  :  " << *total_nodes << endl ;
    if( graph_nodes[0] != NULL)
    {
        // cout << 1 << ;
        // cout << graph_nodes[0] << endl ;
        cout << (graph_nodes[1]->val) << endl ;

    }
    // int *degrees = (int *)shmat(atoi(argv[2]), NULL, 0);
    // total_nodes = (int *)shmat(atoi(argv[3]), NULL, 0);
    // total_edges = (int *)shmat(atoi(argv[4]), NULL, 0);
    
    // for( int i = 0 ; i < 300000 ; i++ )
    // {
    //     // struct Node *p = ( struct Node *)malloc( sizeof(struct Node)) ;
    //     // p = graph_nodes[i] ;
    //     // if( p == NULL)break ;
    //     // cout << " degree  : " << degrees[i] << endl ;
    //     // cout << i << " --->  " ;
    //     // while( p != NULL)
    //     // {
    //     //     cout << p->val << " " ;
    //     //     p = p->next ;
    //     // }
    //     // cout << endl ;
    //     cout << graph_nodes[0]->val << endl ;
    // }

    //     // for (int i = 0; i < 300000; i++)
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
    // for( int i = 0 ; i < 5000 ; i++ )cout<<graph_edges[i].val << endl;
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