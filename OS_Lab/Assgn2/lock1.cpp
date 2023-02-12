#include <sys/file.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <bits/stdc++.h>
using namespace std;

int main()
{
  int fd = open("/home/vinod/Documents/IIT_Kharagpur/CSE_Sem_6/OS_Lab/Assgn2/file.txt", O_RDWR);
  cout << "initial: " << getpid() << endl;
  // flock(fd, LOCK_EX);
  // Perform operations on the file

  if (flock(fd, LOCK_EX | LOCK_NB) == 0)
  {
    cout << "The file is not locked by another process." << endl;
    flock(fd, LOCK_UN);
  }
  else
  {
    cout << "The file is locked by another process." << endl;
  }

  string cmd = "lsof -t " + string("/home/lenovo/Semester6/OS_lab/A2/demo.txt");
  FILE *pipe = popen(cmd.c_str(), "r");
  if (!pipe)
  {
    perror("Error running lsof");
    return 1;
  }

  char buffer[256];
  vector<string> result;
  while (fgets(buffer, sizeof(buffer), pipe) != nullptr)
  {
    result.push_back(buffer);
  }
  pclose(pipe);

  if (result.empty())
  {
    cout << "No process is using the file." << endl;
    return 0;
  }

  // result.pop_back();
  result.pop_back();
  // cout << "PID of process that has the file open: " << result << endl;
  for (auto it : result)
    cout << it << endl;

  // cout<<"last: "<<getppid()<<endl;
  int x;
  cin >> x;

  // flock(fd, LOCK_UN);
  close(fd);
  return 0;
}
