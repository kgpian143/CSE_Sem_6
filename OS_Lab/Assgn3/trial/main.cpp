#include <stdio.h>
#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/wait.h>

const int N =  4e4 ;
const int M =  8e8 ;

using namespace std;

struct Node
{
    int val;
    struct Node *next;
};

int main()
{
    key_t k1 = 235, k2 = 236 , k3 = 237 , k4 =  238 , k5 = 239 ;
    // int *max_node ;
    int shmid1 = shmget(k1, N , IPC_CREAT | 0666);
    struct Node **graph_nodes = (struct Node **)shmat(shmid1, NULL, 0);
    int shmid2 = shmget(k2, M, IPC_CREAT | 0666);
    struct Node *graph_edges = (struct Node *)shmat(shmid2, NULL, 0);
    int shmid3 = shmget(k3, N, IPC_CREAT | 0666);
    int *degrees = (int *)shmat(shmid3, NULL, 0);
    int shmid4 = shmget(k4, 1 , IPC_CREAT | 0666);
    int *max_node = (int *)shmat(shmid4, NULL, 0);
    int shmid5 = shmget(k5, 1 , IPC_CREAT | 0666);
    int *total_edges = (int *)shmat(shmid5, NULL, 0);
    *total_edges= 0 ; 
    *max_node= 0 ; 
    memset( graph_nodes , 0 , N );
    memset( graph_edges , 0 , M);
    memset( degrees , 0 , N );
    FILE *fp = fopen("graph.txt", "r");
    // char edge[50];
    char read_line[100];
    int read_size;
    while (( fgets(read_line, 100 , fp )) != NULL )
    {
        // printf("%s\n", read_line);
        int st , end; 
        sscanf( read_line , "%d %d" , &st , &end) ;
        // printf("%d %d\n",st,end) ;
        *max_node = max( *max_node , st ) ;
        *max_node = max( *max_node , end ) ;
        struct Node i , j ;
        i.val = st ; 
        i.next = NULL ;
        j.val = end ;
        j.next = NULL ;
        int curr_edge = 2 * ( *total_edges )  ;
        graph_edges[curr_edge] = i ; 
        degrees[st]++ ;
        degrees[end]++ ;
        // graph_edges++ ;
        graph_edges[curr_edge + 1] = j ;
        if( graph_nodes[end] == NULL ) graph_nodes[end] = graph_edges  + curr_edge  ;
        else 
        {
            struct Node* temp = graph_nodes[end]; 
            graph_nodes[end] = graph_edges + curr_edge ;
            graph_nodes[end]->next = temp ;
            // i.next = temp ;
        }
        if( graph_nodes[st] == NULL ) graph_nodes[st] = graph_edges  + curr_edge + 1  ;
        else 
        {
            struct Node* temp = graph_nodes[st]; 
            graph_nodes[st] = graph_edges + curr_edge +  1 ;
            graph_nodes[st]->next = temp ;
        }
        *total_edges  = *total_edges + 1  ;
        // for( int i = 0 ; i < 100 ; i++ )
    }
    // for( int i = 0 )
    fclose(fp) ;
    // cout << *max_node << endl ;
    // cout << graph_nodes[0]->val << endl ;
    // for( int i = 0 ; i < 300000 ; i++ )
    // {
    //     struct Node *p = graph_nodes[i] ;
    //     if( p == NULL)break ;
    //     cout << " degree  : " << degrees[i] << endl ;
    //     cout << i << " --->  " ;
    //     while( p != NULL)
    //     {
    //         cout << p->val << " " ;
    //         p = p->next ;
    //     } 
    //     cout << endl ;
    // }
    if( fork() == 0 )
    {
        // prroducer 
        char *args[] = {"./producer" , (char *)NULL } ;
        execvp( *args , args ) ;
        exit(EXIT_SUCCESS) ;
    }
    else 
    {
        wait(NULL) ;
        sleep(2);
        int consumer_count = 20 ;
        for( int i = 0 ; i < consumer_count ; i++ )
        {
            if( fork() == 0 )
            {
                char *args[] = {"./customer"  , (char *)NULL } ;
                execvp(*args , args ) ;
                exit(EXIT_SUCCESS);    
            }
        }
    }
    cout << 1 << endl ;
    int y ;
    cin >> y ;
   shmdt(graph_edges);
   shmdt(graph_edges);
   shmdt(max_node);
   shmdt(total_edges);
   shmdt(degrees);
   shmctl(shmid1 , IPC_RMID , NULL);
   shmctl(shmid2 , IPC_RMID , NULL);
   shmctl(shmid3 , IPC_RMID , NULL);
   shmctl(shmid4 , IPC_RMID , NULL);
   shmctl(shmid5 , IPC_RMID , NULL);
//    shmctl(shmid5 , IPC_RMID , NULL);
   return 0 ;
}