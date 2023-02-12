#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_PIPES 100
#define MAX_LOOPS 100
#define TOLERANCE 0.01

double R[MAX_PIPES];
double flow[MAX_PIPES];
double loop_direction[MAX_LOOPS];
int loop_pipe[MAX_LOOPS][MAX_PIPES];
int m;

void read_data(char *filename)
{
    FILE *fp = fopen(filename, "r");
    char buffer[1024];

    // Read R values
    fgets(buffer, 1024, fp);
    char *token = strtok(buffer, ",");
    int i = 0;
    while (token != NULL)
    {
        R[i] = atof(token);
        token = strtok(NULL, ",");
        i++;
    }

    // Read flow values
    fgets(buffer, 1024, fp);
    token = strtok(buffer, ",");
    i = 0;
    while (token != NULL)
    {
        flow[i] = atof(token);
        token = strtok(NULL, ",");
        i++;
    }

    // Read loops
    for (int j = 0; j < MAX_LOOPS; j++)
    {
        fgets(buffer, 1024, fp);
        token = strtok(buffer, ",");
        loop_direction[j] = atof(token);
        i = 0;
        while (token != NULL)
        {
            loop_pipe[j][i] = atoi(token);
            token = strtok(NULL, ",");
            i++;
        }
        m = i - 1;
    }

    fclose(fp);
}

double process(int loop_num)
{
    double rQQ = 0, r2Q = 0;
    for (int i = 0; i < m; i++)
    {
        int pipe_num = loop_pipe[loop_num][i];
        rQQ += R[pipe_num] * loop_direction[loop_num] * flow[pipe_num] * flow[pipe_num];
        r2Q += 2 * R[pipe_num] * flow[pipe_num];
    }
    double del_Q = -rQQ / r2Q;
    for (int i = 0; i < m; i++)
    {
        int pipe_num = loop_pipe[loop_num][i];
        flow[pipe_num] += loop_direction[loop_num] * del_Q;
    }
    return del_Q;
}

void solve_network_flow()
{
    read_data("hardy_data.txt");
    double del_Q = 100;
    int i = 0;
    while (fabs(del_Q) > TOLERANCE)
    {
        if (i >= MAX_LOOPS)
        {
            i = 0;
        }
        del_Q = process(i);
        i++;
    }
}

int main()
{
    solve_network_flow();
    for (int i = 0; i < MAX_PIPES; i++)
    {
        printf("Pipe %d: Flow = %f\n", i + 1, flow[i]);
    }
    FILE *fp = fopen("output.txt", "w");
    for (int i = 0; i < MAX_PIPES; i++)
    {
        fprintf(fp, "Pipe %d: Flow = %f\n", i + 1, flow[i]);
    }
    fclose(fp);
    return 0;
}
