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

struct Room
{
    int occupancy_cnt;
    int waiting_time;
    // int guest_priority;
};

int n; // number of rooms
int x; // number of cleaning staff
int y; // number of guest

set<pair<int, int>> rooms;
bool all_room_dirty = false;

map<int, Room> room_details; // map for storing room details
map<int, time_t> guest_entry_time;


sem_t booking_room_mutex;
sem_t cleaning_room_mutex;

pthread_mutex_t booking_lock ;
pthread_mutex_t share_lock ; // lock for checking the all rooms are dirty or not 

pthread_cond_t share_lock_cond ; 
// int room_index

void *thread_cleaning(void *arg)
{
    int id = *(int *)(arg);
    cout << " Cleaning_Thread " << id << " : I am cleaning staff " << endl;
    while (1)
    {
        sem_wait(&cleaning_room_mutex);
        if (!all_room_dirty)
        {
            cout << "Cleaning_Thread " << id << ": Waiting for the cleaning " << endl;
            sleep(5) ;
            sem_post(&cleaning_room_mutex) ;
            continue;
        }

        // Critical Section Start

        while (rooms.size() < n)
        {
            int a = rand() % n + 1;
            if (rooms.find({0, a}) == rooms.end())
            {
                rooms.insert({0, a});
                room_details[a].occupancy_cnt = 0;
                room_details[a].waiting_time = 0;
                sleep(room_details[a].waiting_time / 3);
                cout << "Cleaning_Thread " << id << ": Cleaning staff cleaned the room " << a << endl;
                if( rooms.size() == n)
                    all_room_dirty = false;
            }
        }
        for( auto it : rooms )cout << it.second << " some message  " ;
        cout << endl ; 
        sem_post(&cleaning_room_mutex);

        // Critical Section Ends
    }
}

void *thread_guest(void *arg)
{
    int guest_priority = *(int *)arg;
    cout << "Guest_Thread " << guest_priority << " : I am guest thread " << endl;
    while (1)
    {
        cout <<"Guest_Thread " << guest_priority << " : " << rooms.size() << endl;
        sleep(1) ;
        while (all_room_dirty)
        {
            cout << "Guest_Thread " << guest_priority << ": All rooms are dirty, so I am waiting for cleaning staff " << endl;
            // int sleep_time = rand() % 11 + 10;
            sleep(3);
            continue;
        }

        // Critical Section Start

        sem_wait(&booking_room_mutex);
        
       // Lock the mutex
        pthread_mutex_lock(&booking_lock);
        if (rooms.empty())
        {
            all_room_dirty = true;
            sem_post(&booking_room_mutex);
            continue;
        }
        auto it = rooms.begin();
        if (it->first < guest_priority)
        {
            // cout << 2 << endl ;
            rooms.erase(it);

            time_t current_time = time(NULL);
            guest_entry_time[guest_priority] = current_time;
            if (room_details[it->second].occupancy_cnt == 0)
            {
                room_details[it->second].occupancy_cnt += 1;
                rooms.insert({guest_priority, it->second});
                pthread_mutex_unlock(&booking_lock);
                cout << "Guest_Thread " << guest_priority << ": Guest got the room " << it->second << endl;
                int rest_time = rand() % 21 + 10;
                sleep(rest_time);
                it = rooms.find({guest_priority, it->second});
                if (it != rooms.end())
                {
                    cout << "Guest_Thread " << guest_priority << ": Guest stayed for " << rest_time << " days in room  " << it->second << endl;
                    room_details[it->second].waiting_time += rest_time;
                    rooms.erase(it);
                    rooms.insert({0, it->second});
                }
            }
            else
            {
                pthread_mutex_unlock(&booking_lock);
                if (it->first > 0)
                {
                    room_details[it->second].waiting_time = current_time - guest_entry_time[it->first];
                    cout << "Guest_Thread " << guest_priority << ": Early Guest stayed for " << room_details[it->second].waiting_time << " in room  " << it->second << endl;
                }
                cout << "Guest_Thread " << guest_priority << ": Guest got the room " << it->second << endl;
                int rest_time = rand() % 21 + 10;
                sleep(rest_time);
                cout << "Guest_Thread " << guest_priority << ": Guest stayed for " << rest_time << " in room  " << it->second << endl;
            }
        }
        else 
        {
            cout<<"Guest_Thread " << guest_priority << ": Not able to get room " << endl;
            pthread_mutex_unlock(&booking_lock);
        }
        sem_post(&booking_room_mutex);

        // Critical Section Ends
        int sleep_time = rand() % 11 + 10;
        sleep(sleep_time);
    }
    cout << "Guest_Thread " << guest_priority << " : I am exiting " << endl;
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
    for (int i = 0; i < n; i++)
    {
        rooms.insert({0, i + 1});
        Room r;
        r.occupancy_cnt = 0;
        r.waiting_time = 0;
        room_details[i + 1] = r;
    }

    pthread_t cleaning_staff[x], guest[y];

    for (int i = 0; i < x; i++)
    {
        int *id = new int;
        *id = i + 1;
        pthread_create(&cleaning_staff[i], NULL, thread_cleaning, id);
        sleep(1) ;
    }
    for (int i = 0; i < y; i++)
    {
        int *priority = new int;
        *priority = i + 1;
        pthread_create(&guest[i], NULL, thread_guest, priority);
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