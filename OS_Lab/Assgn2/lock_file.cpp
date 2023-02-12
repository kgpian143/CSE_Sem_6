#include <sys/file.h>
#include <fcntl.h>
#include <unistd.h>
#include <bits/stdc++.h>
using namespace std;

int main()
{
  int fd = open("/home/vinod/Documents/IIT_Kharagpur/CSE_Sem_6/OS_Lab/Assgn2/file.txt", O_RDWR);
  flock(fd, LOCK_EX);
  cout<<getpid();
  
  // Perform operations on the file

  int x; cin>>x;

  // flock(fd, LOCK_UN);
  // close(fd);
  return 0;
}
