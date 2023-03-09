#include <bits/stdc++.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<pthread.h>

using namespace std;
#define mod 1000000007
#define total_users 37700
#define total_edges 289003
struct action
{
    int user_id;
    int action_id;
    int action_type;
    time_t time_stamp;
};
struct cmp
{ 
    bool operator()(action a, action b)
    {
        if( a.time_stamp == b.time_stamp)
        {
            if( a.action_type == b.action_type)
            {
                return a.action_id > b.action_id;
            }
            return a.action_type > b.action_type;
        }
        return a.time_stamp > b.time_stamp;
    }
};
class Node
{
public:
    int id;
    int action_cnt[3];
    int order;        // 0 for priority and 1 for chronological
    queue<action> wq; // wall queue for user
    priority_queue<action, vector<action>, cmp> fq; // feed queue for user
    Node()
    {
        // this->id = id;
        this->order = rand() % 2;
        this->action_cnt[0] = 0;
        this->action_cnt[1] = 0;
        this->action_cnt[2] = 0;
        this->wq = queue<action>();
        this->fq = priority_queue<action, vector<action>, cmp>();
    }
};
map<int, set<int>> graph;
map< pair< int, int > , int > neighbours;
vector<Node> User(total_users , Node());
map<int, int> degree;
map<int, string> actions_types;
queue<action> shared_queue;
unordered_set<int> cfq;
void printgraph()
{
    ofstream out("graph_out.txt");
    for (int i = 0; i < total_users; i++)
    {
        out << "User " << i << " : ";
        for (auto it : graph[i])
        {
            out << it << " ";
        }
        out << endl;
    }
    // for( auto it : User)
    // {
    //     out << "User " << it.first << " : " << it.second->order << endl;
    // }
    out.close();
}
/* calculate_common_neighbours 
*  calculate common neighbours of two nodes using set intersection 
*/
void calculate_common_neigbours( int u , int v)
{
    int cnt = 0 ;
    for( auto it : graph[u])
    {
        if( graph[v].find(it) != graph[v].end())
        {
            cnt++;
        }
    }
    neighbours[make_pair(u,v)] = cnt;
    neighbours[make_pair(v,u)] = cnt;
}
void *userSimulator( void *)
{
    // while (1)
    // {
        ofstream out("sns.log", ios ::app);
        out << "<---  User Simulator thread open  --->" << endl;
        set<int> nodes;// set of nodes to be selected
        while (nodes.size() < 100)
        {
            int random_node = rand() % total_users ;
            cout << random_node << endl ;
            nodes.insert(random_node);
        }
        for (auto it : nodes)
        {
            out << "User " << it << " : Do following actions : " << endl;
            // cout << "User " << it  << " : Do following actions : " << endl ;
            if (degree[it] == 0)
                continue;
            int n = log2(degree[it]) + 1;
            for (int i = 0; i < n; i++)
            {
                int r = rand() % 3;
                action act;
                act.action_id = User[it].action_cnt[r] + 1 ;
                User[it].action_cnt[r] += 1 ;
                // User[it].action_cnt[r]++;
                act.action_type = r;
                act.time_stamp = time(NULL); // time stamp
                act.user_id = it;
                User[it].wq.push(act);
                shared_queue.push(act);
                out << "\tuser action : " << actions_types[r] << " " << act.action_id << " on " << ctime(&act.time_stamp);
            }
        }
        out.close();
        // sleep(120);// 
    // }
    pthread_exit(NULL);
}

void *pushUpdate( void *) // push update to feed queue
{
    ofstream out("sns.log", ios ::app); // open file in append mode
    out << "<---  pushUpdate thread open --->" << endl;
    while (1)
    {
        if (shared_queue.empty())
            continue;
        action act = shared_queue.front();
        shared_queue.pop();
        // out << "\tPushing update to feed queue of neighbours of user " << act.user_id << endl;
        for (auto it : graph[act.user_id])
        {
            User[it].fq.push(act);
            cfq.insert(it);
            out << "\tPushing update from user : " << act.user_id << " to feed queue of user : " << it << endl;
        }
    }
    pthread_exit(NULL);
}

void *readPost( void *) // feed update to user
{
    ofstream out("sns.log", ios ::app); // open file in append mode
    out << " readPost thread open " << endl;
    while (!cfq.empty())
    {
        auto it = cfq.begin();
        while (!User[*it].fq.empty())// 
        {
            action act = User[*it].fq.top();
            User[*it].fq.pop();
            // “I read action number XX of type YY posted by user ZZ at time TT”
            out << "\tI read action number " << act.action_id << " of type " << actions_types[act.action_type] << " posted by user " << *it << " at time " << ctime(&act.time_stamp) << endl;
            // cout << "\tI read action number " << act.action_id << " of type " << actions_types[act.action_type] << " posted by user " << *it << " at time " << ctime(&act.time_stamp) << endl;
        }
    }
    pthread_exit(NULL);
}
signed main()
{
    srand(time(NULL));
    ios::sync_with_stdio(false);
    cin.tie(0);
    actions_types[0] = "post";
    actions_types[1] = "comment";
    actions_types[2] = "like";
    ofstream out_sns("sns.log");
    out_sns << "Main thread awoke " << endl;
    out_sns.close();

    ifstream inp("musae_git_edges.csv");
    string line;

    getline(inp, line);

    while (getline(inp, line))
    {
        int u, v;
        // cout << line ;
        int ind = line.find(",");
        u = (int)stoi(line.substr(0, ind));
        v = (int)stoi(line.substr(ind + 1));
        // static Node temp(1);
        if (graph.find(u) != graph.end())
        {
            // Node x(u);
            User[u].id = u;
        }
        if (graph.find(u) != graph.end())
        {
            // Node y(v);
            User[v].id = v;
        }
        degree[u]++;
        degree[v]++;
        graph[u].insert(v);
        graph[v].insert(u);
    }

    // printgraph();
    pthread_t userSimulator_thread, pushUpdate_thread[25], readPost_thread[10];
    pthread_create(&userSimulator_thread, NULL , userSimulator, NULL);
    pthread_join(userSimulator_thread, NULL);
    for (int i = 0; i < 25; i++)
    {
        pthread_create(&pushUpdate_thread[i], NULL, pushUpdate, NULL);
    }
    for (int i = 0; i < 10; i++)
    {
        pthread_create(&readPost_thread[i], NULL, readPost, NULL);
    }
    for (int i = 0; i < 25; i++)
    {
        pthread_join(pushUpdate_thread[i], NULL);
    }
    for (int i = 0; i < 10; i++)
    {
        pthread_join(readPost_thread[i], NULL);
    }
    return 0;
}
