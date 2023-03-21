#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

using namespace std ; 

int main()
{
    time_t curr_time = time(NULL ) ;
    cout << curr_time << endl ;
    sleep(5) ;
    curr_time = time(NULL) ;
    cout << curr_time << endl ; 

}