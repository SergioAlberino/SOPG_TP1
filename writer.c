#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <unistd.h>
#include <stdint.h>
#include <signal.h>

#define FIFO_NAME "myfifo"
#define DATA_PREFIX "DATA:"
#define BUFFER_SIZE 300
// int SIGMESSAGE;

char outputBuffer[BUFFER_SIZE];
char aux[BUFFER_SIZE];
uint32_t bytesWrote;
int32_t returnCode, fd;

void Signal_receive(int sig)
{
  FILE *Sp;

  /* Open signals.txt */
	printf("capture signal...\n");
   
    if (sig==SIGUSR1)
    {
        strcpy(outputBuffer, "SIGN:1 ");
    }

    if (sig==SIGUSR2)
    {
       strcpy(outputBuffer, "SIGN:2 ");
    }

/* Write buffer to named fifo. Strlen - 1 to avoid sending \n char */       
            if ((bytesWrote = write(fd, outputBuffer, strlen(outputBuffer)-1)) == -1)
            {
                perror("write");
            }
            else
            {
                printf("writer: wrote %d bytes\n", bytesWrote);
            }
//         SIGMESSAGE=0;
}


int main(void)
{


pid_t pid;

    pid=getpid();      //Process ID of itself
    printf("el pid es:  %d  \n", pid);

    /* Create named fifo. -1 means already exists so no action if already exists */
    if ( (returnCode = mknod(FIFO_NAME, S_IFIFO | 0666, 0) ) < -1 )
    {
        printf("Error creating named fifo: %d\n", returnCode);
        exit(1);
    }

    /* Open named fifo. Blocks until other process opens it */
	printf("waiting for readers...\n");
	if ( (fd = open(FIFO_NAME, O_WRONLY) ) < 0 )
    {
        printf("Error opening named fifo file: %d\n", fd);
        exit(1);
    }
    
    /* open syscalls returned without error -> other process attached to named fifo */
	printf("got a reader--type some stuff\n");


	struct sigaction sa;
	sa.sa_handler = Signal_receive;
	//sa.sa_flags = 0; //SA_RESTART;
	sigemptyset(&sa.sa_mask);
    sigaction(SIGUSR1,&sa,NULL);
    sigaction(SIGUSR2,&sa,NULL);
 

    /* Loop forever */
	while (1)
	{

        /* Get some text from console */
		if (fgets(outputBuffer, BUFFER_SIZE, stdin)!=0)
        {
           
            strcpy(aux, DATA_PREFIX);
            strcat(aux, outputBuffer);
            strcpy(outputBuffer, aux);
           
        
            /* Write buffer to named fifo. Strlen - 1 to avoid sending \n char */       
            if ((bytesWrote = write(fd, outputBuffer, strlen(outputBuffer)-1)) == -1)
            {
                perror("write");
            }
            else
            {
                printf("writer: wrote %d bytes\n", bytesWrote);
            }
        // SIGMESSAGE=0;
        }


	}
	return 0;
}
