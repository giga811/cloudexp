/*
 * LB server side program
 * Using pthread, compile option $gcc -lpthread
 * for linux it is $gcc -pthread
 * gcc lb-server-brs.c $(pkg-config --libs --cflags libcurl) -pthread -o server
 * usage:
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>

#include <curl/curl.h>
#include <time.h>


#define BUFFER_SIZE 1024
#define PORT 22641

// simple for loop
#define for_(i, j, n) for(i = j; i < n; i++)

// pthread send recv function
void *send_recv_message(void *socket);
void round_robin(int x);
void biased_random_sample(int x);
void send_req(int index, int x);

// some globals for lb
int a = 0;
#define N 20
char *NODES[N];
char *STRX[100];
int EDGES[N][N];
int W = 5;

int RR_INDEX = 0;


// function assigning values for globals
char* itoa(int val, int base){
    static char buf[32] = {0};

    int i = 30;

    for(; val && i ; --i, val /= base)
        buf[i] = "0123456789abcdef"[val % base];

    return &buf[i+1];
}

void prerun(){
    int index = 0, i = 0, j = 0;
    // rand
    srand(time(NULL));
    // strx
    for (i = 0; i < 100; i++){
        STRX[i] = (char *)calloc(3, sizeof(char));
        strcpy(STRX[i], itoa(i+1, 10));
    }
    // nodes
    for (i = 0; i < 21; i ++){
        if ( i+3 == 18)
            continue;
        char *str;
        str = (char *)calloc(20, sizeof(char));
        strcpy(str, "10.0.100.");
        strcat(str, itoa(i+3, 10));
        NODES[index] = str;
        index ++;
    }
    // edges
    for_(i, 0, N){
        for_(j, 0, N){
            if(i == j){
                EDGES[i][j] = 0;
            } else {
                EDGES[i][j] = 1;
            }

        }
    }
    printf("Prerun Finished.\n");
}


int main(int argc, char *argv[])
{
    // prerun
    prerun();

    // Declaring variables
    int srcSocket;    // server socket
    int dstSocket;    // 相手
    /* sockaddr_in 構造体 */
    struct sockaddr_in srcAddr;
    struct sockaddr_in dstAddr;
    int dstAddrSize = sizeof(dstAddr);
    int port = PORT;
    int sock_optval = 1;
    // pthread element
    pthread_t tid;


    /* リスニングソケットを作成 */
    srcSocket = socket(AF_INET, SOCK_STREAM, 0);

    /* ソケットオプション設定 */
    if ( setsockopt(srcSocket, SOL_SOCKET, SO_REUSEADDR,
        &sock_optval, sizeof(sock_optval)) == -1 ){
        perror("setsockopt");
        exit(1);
    }
    /* アドレスファミリ・ポート番号・IPアドレス設定 */
    srcAddr.sin_family = AF_INET;
    srcAddr.sin_port = htons(port);
    srcAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    // bind
    if ( bind(srcSocket, (struct sockaddr *)&srcAddr, sizeof(srcAddr)) < 0 ){
     perror("bind");
     exit(1);
    }

    // Listen
    if ( listen(srcSocket, SOMAXCONN) == -1 ){
     perror("listen");
     exit(1);
    }
    printf("Listen...\n");

    // The loop
    while(1){
        // Accept
        dstSocket = accept(srcSocket, (struct sockaddr *) &dstAddr, &dstAddrSize);
        if(dstSocket == -1){
            perror("accept");
            exit(1);
        }
        printf("Connected from %s\n", inet_ntoa(dstAddr.sin_addr));

        if( pthread_create(&tid, NULL, send_recv_message, (void *)dstSocket) != 0){
            perror("pthread");
            exit(1);
        }
    }
    return 0;
}

void *send_recv_message(void *socket){
    // printf("Thread function.\n");
    pthread_detach(pthread_self());

    int dstSocket = (int) socket;

    // send recv 処理
    int numrcv;
    char *buffer;
    buffer = (char *)calloc(BUFFER_SIZE, sizeof(char));
    /* パケット受信 */
    numrcv = recv(dstSocket, buffer, BUFFER_SIZE, 0);

    // copy first line of buffer
    // printf("Thread function 2.\n");
    int i = 0;
    int x = 0;
    char tmp[20];
    char *firstline;
    firstline = (char *)calloc(80, sizeof(char));
    while (buffer[i] != '\n'){
        firstline[i] = buffer[i];
        i++;
    }
    sscanf(firstline, "%s /%d %s", &tmp, &x, &tmp);
    printf("Scanned x is %d\n", x);

    biased_random_sample(x);

    /* パケット送信 */
    char message[] = "HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\nOK\n";
    send(dstSocket, message, strlen(message), 0);
    // send(dstSocket, buffer, numrcv, 0);

    // thread exit
    close(dstSocket);
    free(buffer);
    free(firstline);
    pthread_exit((void *) 0);
}

void send_req(int index, int x){
    CURL *curl;
    CURLcode res;

    char *url;
    url = (char *)calloc(80, sizeof(char));
    strcpy(url, "http://");
    strcat(url, NODES[index]);
    strcat(url, "/exp/exp1/req/");
    strcat(url, STRX[x-1]);

    curl = curl_easy_init();
    if(curl) {
        printf("Sending request to %s\n", url);
        curl_easy_setopt(curl, CURLOPT_URL, url);
        /* example.com is redirected, so we tell libcurl to follow redirection */
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);
        /* Check for errors */
        if(res != CURLE_OK)
          fprintf(stderr, "curl_easy_perform() failed: %s\n",
                  curl_easy_strerror(res));

        /* always cleanup */
        curl_easy_cleanup(curl);
    }
    free(url);
    return;

}

void round_robin(int x){
    if (RR_INDEX >= N){
        RR_INDEX = 0;
    }
    printf("RR index=%d, x=%d\n", RR_INDEX, x);
    send_req(RR_INDEX, x);
    RR_INDEX ++;
    return;
}

/*
 * BRS functions
 */
void print_edge(){
    int i, j;
    for_(i, 0, N){
        for_(j, 0, N){
            printf("%d", EDGES[i][j]);
        }
        putchar('\n');
    }
    return;
}

int count_edge(int this_node){
    int i, count = 0;
    for_(i, 0, N){
        if (EDGES[this_node][i] == 1)
            count++;
    }
    return count;
}
int random_sample(){
    int start_node = rand() % N;
    int end_node = start_node;
    int i = 0;
    for_(i, 0, W){
        int r = rand() % N;
        int walk = 0;
        while( EDGES[end_node][walk] != 1)
            walk ++;
        while( r != 0){
            walk++;
            if( walk >= N)
                walk = 0;
            if( EDGES[end_node][walk] == 1)
                r--;
        }
        end_node = walk;
    }
    return end_node;
}

void deledge(int n1, int n2){
    if (n1 == n2)
        return;
    EDGES[n1][n2] = 0;
    EDGES[n2][n1] = 0;
    return;
}
void addedge(int n1, int n2){
    if (n1 == n2)
        return;
    EDGES[n1][n2] = 1;
    EDGES[n2][n1] = 1;
    return;
}
void increment(int this_node, int n){
    int i;
    if (n > N-1 - count_edge(this_node))
        n = N-1 - count_edge(this_node);

    for_(i, 0, n){
        int sample_node = this_node;
        while ( sample_node == this_node){
            sample_node = random_sample();
        }
        addedge(this_node, sample_node);
    }
    return;
}
void decrement(int this_node, int n){
    int i;
    if (n >= count_edge(this_node))
        n = count_edge(this_node) - 1;
    for_(i, 0, n){
        int sample_node = this_node;
        while ( sample_node == this_node){
            sample_node = random_sample();
        }
        deledge(this_node, sample_node);
    }
    return;
}
void send_req_test(int sample_node, int x){
    printf("Send req to %d / %d\n", sample_node, x);
    return;
}

void biased_random_sample(int x){

    // random_sample

    int sample_node = random_sample();

    int load_n = (int)( x / 200.0 * N);

    decrement(sample_node, load_n);

    send_req(sample_node, x);

    increment(sample_node, load_n);

    return;
}