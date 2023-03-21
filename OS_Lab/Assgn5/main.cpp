#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

using namespace std;


struct Guest
{
    int id ;
    int rest_time ;
    int priority ; 
};

struct Room
{
    int occupancy_cnt;
    int waiting_time;
    Guest guest  ;
    // int guest_id;
};

int n; // number of rooms
int x; // number of cleaning staff
int y; // number of guest

set<pair<int, int>> rooms;
bool all_room_dirty = false;

map<int, Room> room_details; // map for storing room details
map<int, Guest> guest_details;


sem_t booking_room_mutex;
sem_t cleaning_room_mutex;

pthread_mutex_t booking_lock ;
pthread_mutex_t clean_lock ; // lock for checking the all rooms are dirty or not 

pthread_cond_t share_lock_cond ; 
// int room_index

void *thread_cleaning(void *arg)
{
    int id = *(int *)(arg);
    cout << " Cleaning_Thread " << id << " : I am cleaning staff " << endl;
    while (1)
    {
        if(!all_room_dirty)
        {
            sleep(1) ;
            continue ;
        }

        sem_wait(&cleaning_room_mutex);

        // Critical Section Start

        while (rooms.size() < n)
        {
            // lock the mutex
            pthread_mutex_lock(&clean_lock) ;
            int a = rand() % n + 1;
            bool room_cleaned = ( rooms.find({0, a}) != rooms.end() ) ;
            pthread_mutex_unlock(&clean_lock) ;
            if (!room_cleaned)
            {
                if( room_details[a].guest.priority != 0 )
                {
                    guest_details[room_details[a].guest.id].rest_time = 0 ;
                }
                rooms.insert({0, a});
                sleep(room_details[a].waiting_time / 3);
                room_details[a].occupancy_cnt = 0;
                room_details[a].waiting_time = 0;
                cout << "Cleaning_Thread " << id << ": Cleaning staff cleaned the room " << a << endl;
                
                if( rooms.size() == n )
                {
                    all_room_dirty = false;
                }
            }
            else {
                cout << "Cleaning_Thread " << id << ": Cleaning staff tried to clean the room " << a << " but it is already cleaned " << endl;
            }
        }
        sem_post(&cleaning_room_mutex);

        // Critical Section Ends
    }
}

void *thread_guest(void *arg)
{
    int guest_id = *(int *)arg;
    cout << "Guest_Thread " << guest_id << " : I am guest thread with priority : "<< guest_details[guest_id].priority << endl;
    while (1)
    {
        
       // Critical Section Start
        if( all_room_dirty )
        {
            cout << "Guest_Thread " << guest_id << ": All rooms are dirty, so I am waiting for cleaning staff " << endl;
            sleep(5) ;
            continue;
        }
         
        // pthread_mutex_lock(&share_lock);

        if (rooms.empty())
        {
            all_room_dirty = true;
            continue;
            // pthread_cond_signal(&share_lock_cond) ;
        }

        // pthread_mutex_unlock(&share_lock);


        pthread_mutex_lock(&booking_lock);

        auto it = rooms.begin();
        if(it->first < guest_details[guest_id].priority )rooms.erase(it);

        pthread_mutex_unlock(&booking_lock) ;

        sem_wait(&booking_room_mutex);
        
       // Lock the mutex
        if (it->first < guest_details[guest_id].priority )
        {
            // cout << 2 << endl ;

            time_t current_time = time(NULL);
            if (room_details[it->second].occupancy_cnt == 0)
            {
                room_details[it->second].occupancy_cnt += 1;
                room_details[it->second].guest.id = guest_id;
                rooms.insert({guest_details[guest_id].priority, it->second});
                cout << "Guest_Thread " << guest_id << ": Guest got the room " << it->second << endl;
                guest_details[guest_id].rest_time = rand() % 21 + 10;
                int rest_time = 0 ;
                while(guest_details[guest_id].rest_time -- > 0  )
                {
                    sleep(1) ;
                    rest_time ++ ;
                    room_details[it->second].waiting_time += 1 ;
                }
                cout << "Guest_Thread " << guest_id << ": Guest stayed for " << rest_time << " days in room  " << it->second << endl;
                it = rooms.find({guest_details[guest_id].priority, it->second});
                if (it != rooms.end())
                {
                    rooms.erase(it);
                    rooms.insert({0, it->second});
                }
            }
            else
            {
                if (it->first > 0)
                {
                    guest_details[room_details[it->second].guest.id].rest_time = 0 ;
                }
                cout << "Guest_Thread " << guest_id << ": Guest got the room " << it->second << endl;
                room_details[it->second].occupancy_cnt += 1;
                room_details[it->second].guest = guest_details[guest_id] ;
                guest_details[guest_id].rest_time = rand() % 21 + 10;
                int rest_time = 0 ;
                while(guest_details[guest_id].rest_time -- > 0  )
                {
                    sleep(1) ;
                    rest_time ++ ;
                    room_details[it->second].waiting_time += 1 ;
                }
                cout << "Guest_Thread " << guest_id << ": Guest stayed for " << rest_time << " days in room  " << it->second << endl;
            }
        }
        else 
        {
            cout<<"Guest_Thread " << guest_id << ": Not able to get room " << endl;
            // pthread_mutex_unlock(&booking_lock);
        }
        sem_post(&booking_room_mutex);

        // Critical Section Ends
        int sleep_time = rand() % 11 + 10;
        sleep(sleep_time);
    }
    cout << "Guest_Thread " << guest_id << " : I am exiting " << endl;
    pthread_exit(NULL) ;
}
int main()
{
    //

    srand(time(NULL));
    cout << "Enter the number of rooms  in hotel  : ";
    cin >> n;
    cout << "Enter the number of guest who comes to hotel : ";
    cin >> y;
    cout << "Enter the count of cleaning staff  : ";
    cin >> x;

    sem_init(&booking_room_mutex, 0, y);
    sem_init(&cleaning_room_mutex, 0, x);

    // mutext init
    pthread_mutex_init(&booking_lock, NULL); 
    pthread_cond_init(&share_lock_cond , NULL) ;
    for (int i = 0; i < n; i++)
    {
        rooms.insert({0, i + 1});
        Room r;
        r.occupancy_cnt = 0;
        r.waiting_time = 0;
        room_details[i + 1] = r;
    }

    pthread_t cleaning_staff[x], guest[y];

    for (int i = 0; i < y; i++)
    {
        int *id = new int;
        *id = i + 1;
        int priority = rand() % y + 1;
        guest_details[i + 1].priority = priority;
        pthread_create(&guest[i], NULL, thread_guest, id);
        sleep(1) ;
    }

    for (int i = 0; i < x; i++)
    {
        int *id = new int;
        *id = i + 1;
        pthread_create(&cleaning_staff[i], NULL, thread_cleaning, id);
        sleep(1) ;
    }
    for (int i = 0; i < x; i++)
    {
        pthread_join(cleaning_staff[i], NULL);
    }
    for (int i = 0; i < y; i++)
    {
        pthread_join(guest[i], NULL);
    }

    sem_destroy(&booking_room_mutex);
    sem_destroy(&cleaning_room_mutex);
}