void *thread_cleaning(void *arg)
{
    char buffer[100];
    int id = *(int *)(arg);

    pthread_mutex_lock(&clean_lock);

    // cout << " Cleaning_Thread " << id << " : I am cleaning staff " << endl;
    sprintf(buffer, "Cleaning_Thread %d : I am cleaning staff \n", id);
    printFunc(buffer);
    // fwrite(buffer, 1, strlen(buffer), outputfile);

    pthread_mutex_unlock(&clean_lock);

    while (1)
    {
        if (!all_room_dirty)
        {
            sleep(1);
            continue;
        }

        sem_wait(&cleaning_room_mutex);

        // Critical Section Start

        while (room_size < n)
        {
            // lock the mutex
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
                sleep(room_details[a].waiting_time / 3 );
                room_details[a].occupancy_cnt = 0;
                room_details[a].waiting_time = 0;
                sprintf(buffer, "Cleaning_Thread %d : Cleaning staff cleaned the room %d \n", id, a);
                printFunc(buffer);
                
                pthread_mutex_lock(&clean_lock1);

                // cout << "Cleaning_Thread " << id << ": Cleaning staff cleaned the room " << a << endl;
                // fwrite(buffer, 1, strlen(buffer), outputfile);

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
                // cout << "Cleaning_Thread " << id << ": Cleaning staff tried to clean the room " << a << " but it is already cleaned " << endl;
            }
        }
        sem_post(&cleaning_room_mutex);

        // Critical Section Ends
    }
}