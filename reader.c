#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdint.h>
#include <signal.h>
#include <unistd.h>
#include <signal.h>

#define FIFO_NAME "myfifo"
#define LOG_NAME "log.txt"
#define SIGFILE_NAME "signals.txt"
#define BUFFER_SIZE 50


void Signal_receive(int sig)
{
    FILE *Sp;
    /* Open signals.txt */
	printf("saving in signals.txt...\n");
	if ( (Sp = fopen(SIGFILE_NAME, "a") ) == NULL )
    {
        printf("Error opening signal file \n");
        exit(1);
    }
  
    if (sig==SIGUSR1)
    {
        printf("saving SIGN:1 \n");
        fprintf(Sp,"SIGN:1 \n");
        //fclose(Sp);
    }

    if (sig==SIGUSR2)
    {
        printf("saving SIGN:2 \n");
        fprintf(Sp,"SIGN:2 \n");
        //fclose(Sp);
    }     
}

int main(void)
{
	uint8_t inputBuffer[BUFFER_SIZE];
	int32_t bytesRead, returnCode, fd;
    FILE *fp;
    pid_t pid;

	struct sigaction sa;
	sa.sa_handler = Signal_receive;
	//sa.sa_flags = 0; //SA_RESTART;
	//sigemptyset(&sa.sa_mask);
    sigaction(SIGUSR1,&sa,NULL);
    sigaction(SIGUSR2,&sa,NULL);
 
    pid=getpid();      //Process ID of itself
    printf("el pid es:  %d  \n", pid);

    /* Open log.txt */
	printf("opening log.txt...\n");
	if ( (fp = fopen(LOG_NAME, "a") ) == NULL )
    {
        printf("Error opening log file: %d\n", fd);
        exit(1);
    }

    /* Create named fifo. -1 means already exists so no action if already exists */
    if ( (returnCode = mknod(FIFO_NAME, S_IFIFO | 0666, 0) ) < -1  )
    {
        printf("Error creating named fifo: %d\n", returnCode);
        exit(1);
    }
   
    /* Open named fifo. Blocks until other process opens it */
	printf("waiting for writers...\n");
	if ( (fd = open(FIFO_NAME, O_RDONLY) ) < 0 )
    {
        printf("Error opening named fifo file: %d\n", fd);
        exit(1);
    }
    
    /* open syscalls returned without error -> other process attached to named fifo */
	printf("got a writer\n");

    /* Loop until read syscall returns a value <= 0 */
	do
	{
                /* read data into local buffer */
		if ((bytesRead = read(fd, inputBuffer, BUFFER_SIZE)) == -1)
        {
			printf("saving the World");
            perror("read");
        }
        else
		{
			inputBuffer[bytesRead] = '\0';
			printf("saving in log %s \n", inputBuffer);
            if (bytesRead != 0)
                fprintf(fp,"DATA:%s \n", inputBuffer);    
		}
	}
	while (bytesRead > 0);

	return 0;
}

