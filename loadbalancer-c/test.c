#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

// simple for loop
#define for_(i, j, n) for(i = j; i < n; i++)

int a = 0;
#define N 20
char *NODES[N];
char *STRX[100];
int EDGES[N][N];
int W = 5;
int WEIGHT = 200;

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
    int i = 0, j = 0;

    for_(i, 0, W){
        int count = count_edge(end_node);
        int r = rand() % count + 1;
        int w = 0;
        for_(j, 0, N){
            if (EDGES[end_node][j] == 1){
                w++;
            }
            if (w == r)
                break;
        }
        end_node = j;
    }
    return end_node;
}

void deledge(int n1, int n2){
    if (count_edge(n1) <= 1 || count_edge(n2) <= 1)
        return;
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
void send_req(int sample_node, int x){
    printf("Send req to %d / %d\n", sample_node, x);
    return;
}

void biased_random_sample(int x){

    // random_sample

    int sample_node = random_sample();
    printf("%d,%d\n",sample_node, x );

    int load_n = (int)( (float)x / WEIGHT * N);

    decrement(sample_node, load_n);

    send_req(sample_node, x);

    increment(sample_node, load_n);

    return;
}

int main(){
    prerun();
    int i, j;

    // for_(i, 0, N){
    //     printf("%d\n", random_sample());
    // }

    // biased_random_sample(50);

    int rs = 0;
    for(i=0; i < 10; i++){
        rs = random_sample();
        printf("%d,", rs);
    }


    return 0;
}