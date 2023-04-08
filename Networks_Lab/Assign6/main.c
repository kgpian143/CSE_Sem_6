#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/ip_icmp.h>
#include <time.h>
#include <fcntl.h>
#include <signal.h>
#include <time.h>

#define PACKET_SIZE 64
#define MAX_SIZE 100
#define PORT_NO 0
#define DEST_PORT 32164
#define PING_SLEEP_RATE 1000000

struct send_packet
{
    struct icmphdr hdr;
};

struct receive_packet
{
    struct icmphdr icmp_hdr ; 
    struct icmphdr ip_hdr ; 
};
// Function to calculate the round-trip time (RTT) of a packet
double calculate_rtt(struct timeval *sent_time, struct timeval *recv_time)
{
    double rtt = (double)(recv_time->tv_sec - sent_time->tv_sec) * 1000.0;
    rtt += (double)(recv_time->tv_usec - sent_time->tv_usec) / 1000.0;
    return rtt;
}

double min( double a, double b )
{
    return a < b ? a : b;
}
uint16_t in_cksum(uint16_t *addr, int len)
{
    int nleft = len;
    uint32_t sum = 0;
    uint16_t *w = addr;
    uint16_t answer = 0;
    while (nleft > 1)
    {
        sum += *w++;
        nleft -= 2;
    }
    if (nleft == 1)
    {
        *(unsigned char *)(&answer) = *(unsigned char *)w;
        sum += answer;
    }
    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);
    answer = ~sum;
    return (answer);
}
// Function to perform a ping command and estimate the latency and bandwidth of each link in the path
void ping_net_info(char *host, int n, time_t t1)
{
    struct hostent *host_ip;
    struct sockaddr_in target_addr, r_addr;
    int sockfd;
    struct timespec time_start, time_end, tfs, tfe;
    double rtt_min = 1000000, rtt_max = 0, rtt_sum = 0;

    // Get the IP address of the given host
    if ((host_ip = gethostbyname(host)) == NULL)
    {
        perror("gethostbyname() failed");
        exit(EXIT_FAILURE);
    }

    // print the IP address of the host
    printf("PING %s (%s) %d bytes of data.\n", host, inet_ntoa(*(struct in_addr *)host_ip->h_addr), PACKET_SIZE);

    // Fill in the target address structure
    memset(&target_addr, 0, sizeof(target_addr));
    target_addr.sin_family = AF_INET;
    memcpy(&target_addr.sin_addr.s_addr, host_ip->h_addr, host_ip->h_length);
    target_addr.sin_port = htons(PORT_NO);

    // Create a raw socket for ICMP packets
    if ((sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0)
    {
        perror("socket() failed");
        exit(EXIT_FAILURE);
    }
    clock_gettime(CLOCK_MONOTONIC, &tfs);
    // Set the timeout for the socket to 2 seconds
    struct timeval timeout;
    timeout.tv_sec = 2;
    timeout.tv_usec = 0;

    struct send_packet send_pkt;
    int flag, msg_count = 0, msg_received_count = 0;

    // setsockopt function is used to manipulate options for the socket referred to by the file descriptor sockfd
    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char *)&timeout, sizeof(timeout)) < 0)
    {
        perror("setsockopt() failed");
        exit(EXIT_FAILURE);
    }

    // setsockopt function to change ttl value

    int ttl = 1;
    int dest_flag = 0 ;
    while (1)
    {
        if (setsockopt(sockfd, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl)) < 0)
        {
            perror("setsockopt() failed");
            exit(EXIT_FAILURE);
        }

        // Send a ping packet
        double bandwidth = 0;
        for (int i = 0; i < n; i++)
        {
            flag = 1;
            char buffer[PACKET_SIZE];
            memset(buffer, 0, PACKET_SIZE);
            if (i != 0)
            {
                int k = sizeof(send_pkt);
                for (int j = k; j < PACKET_SIZE; j++)
                {
                    buffer[j] = '\0';
                }
            }

            memset(&send_pkt, 0, sizeof(send_pkt));
            send_pkt.hdr.type = ICMP_ECHO;
            send_pkt.hdr.un.echo.id = getpid();
            send_pkt.hdr.un.echo.sequence = msg_count++;
            send_pkt.hdr.checksum = 0;
            memcpy(buffer, &send_pkt, sizeof(send_pkt));
            send_pkt.hdr.checksum = in_cksum((uint16_t *)buffer, PACKET_SIZE);
            memcpy(buffer, &send_pkt, sizeof(send_pkt));
            usleep(PING_SLEEP_RATE);

            // send packet
            clock_gettime(CLOCK_MONOTONIC, &time_start);
            if (sendto(sockfd, buffer , sizeof(buffer), 0, (struct sockaddr *)&target_addr, sizeof(target_addr)) <= 0)
            {
                printf("\nPacket Sending Failed!\n");
                flag = 0;
            }

            // receive packet
            struct receive_packet recv_pkt ; 
            int addr_len = sizeof(r_addr);

            if (recvfrom(sockfd, buffer, sizeof(buffer), 0,
                         (struct sockaddr *)&r_addr, &addr_len) <= 0 &&
                msg_count > 1)
            {
                printf("\nPacket receive failed!\n");
            }

            else
            {
                memcpy( &recv_pkt , buffer , sizeof(recv_pkt)) ;
                clock_gettime(CLOCK_MONOTONIC, &time_end);

                double timeElapsed = ((double)(time_end.tv_nsec - time_start.tv_nsec)) / 1000000.0;
                double rtt_msec = (time_end.tv_sec - time_start.tv_sec) * 1000.0 + timeElapsed;

                rtt_min = min(rtt_min, rtt_msec);
                // if packet was not sent, don't receive
                if (flag)
                {
                    printf("**************Packet received************** \n");

                    printf("IP of sender : %s \n", inet_ntoa(r_addr.sin_addr));
                    printf("Rtt : %lf ms \n", rtt_msec);
                    
                    if( i != 0 ) 
                    {
                         // calculate bandwidth
                        bandwidth = (56 * 8 ) / ((rtt_msec - rtt_min  ) / 1000); 
                    }

                    if (!(send_pkt.hdr.type != ICMP_ECHO && send_pkt.hdr.type != ICMP_ECHOREPLY && send_pkt.hdr.type != ICMP_TIME_EXCEEDED))
                    {
                        // print  error packet received
                        printf("Error packet received... \n");
                        // print packet info
                        printf("Packet info: \n");
                        printf("Type: %d \n", send_pkt.hdr.type);
                        printf("Code: %d \n", send_pkt.hdr.code);
                        printf("Checksum: %d \n", send_pkt.hdr.checksum);
                        printf("ID: %d \n", send_pkt.hdr.un.echo.id);
                        printf("Sequence: %d \n", send_pkt.hdr.un.echo.sequence);
                        // source and destination ip addresses
                        printf("Source IP: %s \n", inet_ntoa(r_addr.sin_addr));
                        printf("Destination IP: %s \n", inet_ntoa(target_addr.sin_addr));
                    
                    }
                    else
                    {
                        // print packet info
                        msg_received_count++;
                        if( strcmp( inet_ntoa(r_addr.sin_addr) , inet_ntoa(target_addr.sin_addr) ) == 0 )
                        {
                            printf("Packet reached at destination IP : %s\n",inet_ntoa(r_addr.sin_addr) ) ;
                            dest_flag = 1 ;
                        }
                        else 
                        printf("Packet received: from destination IP : %s \n", inet_ntoa(r_addr.sin_addr));
                    }
                }
            }
        }
        double latency = rtt_min/2 - rtt_sum ;
        rtt_sum += latency ;
        printf("Latency = %.2f ms\n", latency);
        printf("Bandwidth = %.2f Mbps\n", bandwidth);
        if( dest_flag == 1 )
        {
            printf("Destination IP reached \n") ;
            break ;
        }
        ttl++;
    }

    close(sockfd);
}
// Driver code
int main()
{
    char host[100];

    printf("Enter the website address to ping: ");
    scanf("%s", host);

    int n;
    time_t t1;
    printf("Enter the number of packets to send: ");
    scanf("%d", &n);
    printf("Enter the time interval between packets: ");
    scanf("%ld", &t1);

    if (n < 5)
        n = 5;

    ping_net_info(host, n, t1);

    return 0;
}
