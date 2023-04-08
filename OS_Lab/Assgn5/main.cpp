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
    int id;
    int rest_time;
    int priority;
};

struct Room
{
    int occupancy_cnt;
    int waiting_time;
    Guest guest;
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

pthread_mutex_t booking_lock;
pthread_mutex_t clean_lock, clean_lock1; // lock for checking the all rooms are dirty or not
pthread_mutex_t print_lock;              // lock for printing the output

int room_size = 0;

FILE *outputfile = fopen("hotel.log", "wb");

void printFunc(char *buff)
{
    pthread_mutex_lock(&print_lock);

    int buff_size = sizeof(char) * strlen(buff);
    fwrite(buff, sizeof(char), buff_size, outputfile);
    fwrite(buff, sizeof(char), buff_size, stdout);

    pthread_mutex_unlock(&print_lock);
}

void *thread_cleaning(void *arg)
{
    char buffer[100];
    int id = *(int *)(arg);

    sprintf(buffer, "Cleaning_Thread %d : I am cleaning staff \n", id);
    printFunc(buffer);

    while (1)
    {
        if (!all_room_dirty)
        {
            sleep(1);
            continue;
        }

        sem_wait(&cleaning_room_mutex);

        while (room_size < n)
        {
            pthread_mutex_lock(&clean_lock);
            int a = rand() % n + 1;
            bool room_cleaned = (rooms.find({0, a}) != rooms.end());
            if (!room_cleaned)
            {
                rooms.insert({0, a});
                pthread_mutex_unlock(&clean_lock);
                if (room_details[a].guest.priority != 0)
                {
                    guest_details[room_details[a].guest.id].rest_time = 0;
                }
                sleep(room_details[a].waiting_time / 3);
                room_details[a].occupancy_cnt = 0;
                room_details[a].waiting_time = 0;
                sprintf(buffer, "Cleaning_Thread %d : Cleaning staff cleaned the room %d \n", id, a);
                printFunc(buffer);

                pthread_mutex_lock(&clean_lock1);

                room_size++;

                pthread_mutex_unlock(&clean_lock1);
                if (room_size == n)
                {
                    all_room_dirty = false;
                }
            }
            else
            {
                pthread_mutex_unlock(&clean_lock);
            }
        }
        sem_post(&cleaning_room_mutex);
    }
}


void *thread_guest(void *arg)
{
    int guest_id = *(int *)arg;
    char buffer[100];

    sprintf(buffer, "Guest_Thread %d : I am guest thread with priority : %d \n", guest_id, guest_details[guest_id].priority);
    printFunc(buffer);

    while (1)
    {

        if (all_room_dirty)
        {
            sprintf(buffer, "Guest_Thread %d : All rooms are dirty, so I am waiting for cleaning staff \n", guest_id);
            printFunc(buffer);
            sleep(rand() % 11 + 10);
            continue;
        }


        if (rooms.empty())
        {
            all_room_dirty = true;
            room_size = 0;
            continue;
        }


        pthread_mutex_lock(&booking_lock);

        auto it = rooms.begin();
        if (it->first < guest_details[guest_id].priority)
            rooms.erase(it);

        pthread_mutex_unlock(&booking_lock);

        sem_wait(&booking_room_mutex);

        if (it->first < guest_details[guest_id].priority)
        {

            time_t current_time = time(NULL);
            if (room_details[it->second].occupancy_cnt == 0)
            {
                room_details[it->second].occupancy_cnt += 1;
                room_details[it->second].guest.id = guest_id;
                rooms.insert({guest_details[guest_id].priority, it->second});
                sprintf(buffer, "Guest_Thread %d : Guest got the room %d \n", guest_id, it->second);
                printFunc(buffer);

                guest_details[guest_id].rest_time = rand() % 21 + 10;
                int rest_time = 0;
                while (guest_details[guest_id].rest_time-- > 0)
                {
                    sleep(1);
                    rest_time++;
                    room_details[it->second].waiting_time += 1;
                }
                sprintf(buffer, "Guest_Thread %d : Guest stayed for %d days in room %d \n", guest_id, rest_time, it->second);
                printFunc(buffer);
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
                    guest_details[room_details[it->second].guest.id].rest_time = 0;
                }

                sprintf(buffer, "Guest_Thread %d : Guest got the room %d \n", guest_id, it->second);
                printFunc(buffer);

                room_details[it->second].occupancy_cnt += 1;
                room_details[it->second].guest = guest_details[guest_id];
                guest_details[guest_id].rest_time = rand() % 21 + 10;
                int rest_time = 0;
                while (guest_details[guest_id].rest_time-- > 0)
                {
                    sleep(1);
                    rest_time++;
                    room_details[it->second].waiting_time += 1;
                }

                sprintf(buffer, "Guest_Thread %d : Guest stayed for %d days in room %d \n", guest_id, rest_time, it->second);
                printFunc(buffer);
            }
        }
        else
        {
            sprintf(buffer, "Guest_Thread %d : Not able to get room \n", guest_id);
            printFunc(buffer);
        }
        sem_post(&booking_room_mutex);

        int sleep_time = rand() % 11 + 10;
        sleep(sleep_time);
    }
    sprintf(buffer, "Guest_Thread %d : I am exiting \n", guest_id);
    printFunc(buffer);
    pthread_exit(NULL);
}

int main()
{
    // setvbuf is used to disable the output buffering

    setvbuf(outputfile, NULL, _IONBF, 0);

    // srand is used to generate random number

    srand(time(NULL));

    cout << "Enter the number of rooms  in hotel  : ";
    cin >> n;
    cout << "Enter the number of guest who comes to hotel : ";
    cin >> y;
    cout << "Enter the count of cleaning staff  : ";
    cin >> x;

    sem_init(&booking_room_mutex, 0, y);
    sem_init(&cleaning_room_mutex, 0, x);

    // mutex init
    pthread_mutex_init(&booking_lock, NULL);
    pthread_mutex_init(&print_lock, NULL);
    pthread_mutex_init(&clean_lock1, NULL);
    pthread_mutex_init(&clean_lock, NULL);

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
        sleep(1);
    }

    for (int i = 0; i < x; i++)
    {
        int *id = new int;
        *id = i + 1;
        pthread_create(&cleaning_staff[i], NULL, thread_cleaning, id);
        sleep(1);
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

    pthread_mutex_destroy(&booking_lock);
    pthread_mutex_destroy(&print_lock);
    pthread_mutex_destroy(&clean_lock1);
    pthread_mutex_destroy(&clean_lock);

}