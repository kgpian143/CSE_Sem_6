#include <bits/stdc++.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <setjmp.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>

using namespace std;
static sigjmp_buf env;
static volatile sig_atomic_t jump_active = 0;
int counter = 0;
int saved_stdout ;
int saved_stdin ;
int move_to_beginning(int count, int key) {
    rl_beg_of_line(count, key);
    return 1;
}
void sigint_handler(int signo)
{
    if (!jump_active)
    {
        return;
    }
    dup2(saved_stdout, STDOUT_FILENO); /* repeat until done ....         */
    dup2(saved_stdin, STDIN_FILENO); 
    siglongjmp(env, 42);
}
void sighandler(int sig_num)
{
    // Reset handler to catch SIGTSTP next time
    signal(SIGTSTP, sighandler);
    // printf("Cannot execute Ctrl+Z\n");
    dup2(saved_stdout, STDOUT_FILENO); /* repeat until done ....         */
    dup2(saved_stdin, STDIN_FILENO); 
    printf("\n[%d]+   Stopped         ", counter);
    counter++;
    siglongjmp(env, 42);
}

int takeInput(char* str)
{
    char* buf;
  
    buf = readline("\n>>> ");
    if (strlen(buf) != 0) {
        add_history(buf);
        strcpy(str, buf);
        printf("%s", str);
        return 0;
    } else {
        return 1;
    }
}

void parse(string cmd, vector<string> &argv, int &inp, int &outp, vector<int> &pipes)
{
    string str;
    for (int i = 0; cmd[i] != '\0'; i++)
    {
        if ((i == cmd.size() - 1) && (cmd[i] == '&'))
            continue;
        if (cmd[i] == '|')
        {
            if (!str.empty())
                argv.push_back(str);
            pipes.push_back(argv.size());
            str.clear();
            continue;
        }
        if (cmd[i] == '<')
        {
            if (!str.empty())
                argv.push_back(str);
            inp = argv.size();
            str.clear();
        }
        else if (cmd[i] == '>')
        {
            if (!str.empty())
                argv.push_back(str);
            outp = argv.size();
            // cout << outp << endl ;
            str.clear();
        }
        else if (cmd[i] == ' ' || cmd[i] == '\n' || cmd[i] == '\t')
        {
            while (cmd[i] == ' ' || cmd[i] == '\n' || cmd[i] == '\t')
                i++;
            if (!str.empty())
                argv.push_back(str);
            i--;
            str.clear();
        }
        else
            str.push_back(cmd[i]);
    }
    if (!str.empty())
        argv.push_back(str);
    // argv.push_back(NULL);
}

void execute(string cmd)
{
    vector<string> argv;
    vector<int> pipes;
    pid_t pid;
    int status;
    int inp = -1;
    int outp = -1;
    parse(cmd, argv, inp, outp, pipes);
    if (argv[0] == "cd")
    {
        char path[1000];
        if (argv.size() == 1)
        {
            strcpy(path, "/home/vinod");
        }
        else
        {
            strcpy(path, argv[1].c_str());
        }
        int res = chdir(path);
        if (res != 0)
        {
            cout << "bash: cd: " << argv[1] << ": No such file or directory" << endl;
        }
        return;
    }
    if (pipes.size() != 0)
    {
        int ind = 0;
        for (int i = 0; i < pipes.size(); i++)
        {
        }
    }
    // const char *cstr = argv[0].c_str();
    char **arr = new char *[argv.size() + 1];
    for (int i = 0; i < argv.size(); i++)
    {
        arr[i] = new char[argv[i].length() + 1];
        strcpy(arr[i], argv[i].c_str());
    }
    int saved_stdout = dup(STDOUT_FILENO);
    arr[argv.size()] = NULL;
    if (inp != -1)
    {
        int fd = open(argv[inp].c_str(), O_RDONLY);
        dup2(fd, STDIN_FILENO);
    }
    if (outp != -1)
    {
        // cout << outp << endl ;
        int fd = open(argv[outp].c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
        dup2(fd, STDOUT_FILENO);
    }
    // for( int i = 0 ; i < argv.size() ;i++ )cout << argv[i] << " " ;

    if ((pid = fork()) < 0)
    { /* fork a child process           */
        cout << "*** ERROR: forking child process failed\n";
        exit(1);
    }
    else if (pid == 0)
    { /* for the child process:         */
        if (execvp(*arr, arr) < 0)
        { /* execute the command  */
            printf("*** ERROR: exec failed\n");
            exit(1);
        }
    }
    else
    {
        if (cmd.back() == '&')
            return;                  /* for the parent:      */
        while (wait(&status) != pid) /* wait for completion  */
            ;
    }
}
int main()
{

    signal(SIGINT, sigint_handler);
    signal(SIGTSTP, sighandler);
    // struct sigaction s;
    // s.sa_handler = sigint_handler;
    // sigemptyset(&s.sa_mask);
    // s.sa_flags = SA_RESTART;
    // sigaction(SIGINT, &s, NULL);
    // sigaction(SIGTSTP, &s, NULL);
    
    string line; /* the input line                 */
                 //  char  *argv[64];              /* the command line argument      */
    saved_stdout = dup(STDOUT_FILENO);
    saved_stdin = dup(STDIN_FILENO);
    // rl_bind_keyseq("\033\001", move_line_to_beginning);
    rl_bind_keyseq("\001", move_to_beginning);
    char *command ;
    while ((command = readline("$ ")) != NULL)
    {
        if (sigsetjmp(env, 1) == 42)
        {
            cout << endl;
        }
        jump_active = 1;
        dup2(saved_stdout, STDOUT_FILENO); /* repeat until done ....         */
        dup2(saved_stdin, STDIN_FILENO);   /* repeat until done ....         */
        cout << "$ ";                      /*   display a prompt             */
        getline(cin, line);
        //   cout << line  << endl ;        /*   read in the command line     */

        if (line == "exit") /* is it an "exit"?     */
            exit(0);        /*   exit if it is                */
        execute(line);      /* otherwise, execute the command */
    }
}