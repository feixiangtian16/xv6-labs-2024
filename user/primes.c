#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void primes(int left_read_fd) {
    int prime;
    if (read(left_read_fd, &prime, sizeof(prime)) == 0) {
        close(left_read_fd);
        exit(0);
    }
    printf("prime %d\n", prime);

    int p[2];
    pipe(p);

    if (fork() == 0) {
        close(p[1]);         
        close(left_read_fd);  
        primes(p[0]);    
    } else {
        close(p[0]);        
        int n;
        while (read(left_read_fd, &n, sizeof(n)) > 0) {
            if (n % prime != 0) {
                write(p[1], &n, sizeof(n));  
            }
        }
        close(p[1]);          
        close(left_read_fd);  
        wait(0);              
    }
    exit(0);
}

int main() {
    int p[2];
    pipe(p);

    if (fork() == 0) {
        close(p[1]);    
        primes(p[0]);   
    } else {
        close(p[0]);      
        for (int i = 2; i <= 280; i++) {
            write(p[1], &i, sizeof(i));
        }
        close(p[1]);      
        wait(0);        
    }
    exit(0);
}
