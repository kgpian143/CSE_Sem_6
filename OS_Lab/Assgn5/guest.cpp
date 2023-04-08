void *thread_guest(void *arg)
{
    int guest_id = *(int *)arg;
    char buffer[100];

    // cout << "Guest_Thread " << guest_id << " : I am guest thread with priority : " << guest_details[guest_id].priority << endl;
    sprintf(buffer, "Guest_Thread %d : I am guest thread with priority : %d \n", guest_id, guest_details[guest_id].priority);
    printFunc(buffer);
    // fwrite(buffer, 1, strlen(buffer), outputfile);
    while (1)
    {

        // Critical Section Start
        if (all_room_dirty)
        {
            // cout << "Guest_Thread " << guest_id << ": All rooms are dirty, so I am waiting for cleaning staff " << endl;
            sprintf(buffer, "Guest_Thread %d : All rooms are dirty, so I am waiting for cleaning staff \n", guest_id);
            printFunc(buffer);
            // fwrite(buffer, 1, strlen(buffer), outputfile);
            sleep(rand() % 11 + 10);
            continue;
        }

        // pthread_mutex_lock(&share_lock);

        if (rooms.empty())
        {
            all_room_dirty = true;
            room_size = 0;
            continue;
            // pthread_cond_signal(&share_lock_cond) ;
        }

        // pthread_mutex_unlock(&share_lock);

        pthread_mutex_lock(&booking_lock);

        auto it = rooms.begin();
        if (it->first < guest_details[guest_id].priority)
            rooms.erase(it);

        pthread_mutex_unlock(&booking_lock);

        sem_wait(&booking_room_mutex);

        // Lock the mutex
        if (it->first < guest_details[guest_id].priority)
        {
            // cout << 2 << endl ;

            time_t current_time = time(NULL);
            if (room_details[it->second].occupancy_cnt == 0)
            {
                room_details[it->second].occupancy_cnt += 1;
                room_details[it->second].guest.id = guest_id;
                rooms.insert({guest_details[guest_id].priority, it->second});
                // cout << "Guest_Thread " << guest_id << ": Guest got the room " << it->second << endl;
                sprintf(buffer, "Guest_Thread %d : Guest got the room %d \n", guest_id, it->second);
                printFunc(buffer);
                // fwrite(buffer, 1, strlen(buffer), outputfile);

                guest_details[guest_id].rest_time = rand() % 21 + 10;
                int rest_time = 0;
                while (guest_details[guest_id].rest_time-- > 0)
                {
                    sleep(1);
                    rest_time++;
                    room_details[it->second].waiting_time += 1;
                }
                // cout << "Guest_Thread " << guest_id << ": Guest stayed for " << rest_time << " days in room  " << it->second << endl;
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
                // cout << "Guest_Thread " << guest_id << ": Guest got the room " << it->second << endl;
                sprintf(buffer, "Guest_Thread %d : Guest got the room %d \n", guest_id, it->second);
                printFunc(buffer);
                // fwrite(buffer, 1, strlen(buffer), outputfile);

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
                // cout << "Guest_Thread " << guest_id << ": Guest stayed for " << rest_time << " days in room  " << it->second << endl;
                sprintf(buffer, "Guest_Thread %d : Guest stayed for %d days in room %d \n", guest_id, rest_time, it->second);
                printFunc(buffer);
                // fwrite(buffer, 1, strlen(buffer), outputfile);
            }
        }
        else
        {
            // cout << "Guest_Thread " << guest_id << ": Not able to get room " << endl;
            sprintf(buffer, "Guest_Thread %d : Not able to get room \n", guest_id);
            printFunc(buffer);
            // fwrite(buffer, 1, strlen(buffer), outputfile);

            // pthread_mutex_unlock(&booking_lock);
        }
        sem_post(&booking_room_mutex);

        // Critical Section Ends
        int sleep_time = rand() % 11 + 10;
        sleep(sleep_time);
    }
    // cout << "Guest_Thread " << guest_id << " : I am exiting " << endl;
    sprintf(buffer, "Guest_Thread %d : I am exiting \n", guest_id);
    printFunc(buffer);
    // fwrite(buffer, 1, strlen(buffer), outputfile);

    pthread_exit(NULL);
}