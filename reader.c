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



#define FIFO_NAME "myfifo"
#define LOG_NAME "log.txt"
#define SIGFILE_NAME "signals.txt"
#define BUFFER_SIZE 300
#define AUX_SIZE 5

int main(void)
{
	uint8_t inputBuffer[BUFFER_SIZE];
    char aux[AUX_SIZE];
	int32_t bytesRead, returnCode, fd;
    FILE *fp;
    FILE *Sp;
    

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
			perror("read");
        }
        else
		{
			inputBuffer[bytesRead] = '\0';
             
            
            if(inputBuffer[0]==68)             
            {
                printf("Data Received \n");
                printf("Receive: %s \n", inputBuffer);
                // if (bytesRead != 0)
                {   
                    /* Open log.txt */
	                printf("saving in log.txt...\n");
	                if ( (fp = fopen(LOG_NAME, "a") ) == NULL )
                    {
                        printf("Error opening log file: %d\n", fd);
                        exit(1);
                    }
                     /* Save in log.txt */                           
                    fprintf(fp,"%s \n", inputBuffer);
                    /* Close log.txt */    
                    fclose(fp);
                }   

            }
            else
            {
                if(inputBuffer[0]==83)             
                    printf("Signal Received \n");
                    printf("Receive: %s \n", inputBuffer);
                        // if (bytesRead != 0)
                        {   
                            /* Open signal.txt */
                            printf("saving in signal.txt...\n");
                            if ( (Sp = fopen(SIGFILE_NAME, "a") ) == NULL )
                            {
                                printf("Error opening signal file \n");
                                exit(1);
                            }
                            /* Save in signal.txt */                           
                            fprintf(Sp,"%s \n", inputBuffer);
                            /* Close signal.txt */    
                            fclose(Sp);
                        }   
            }
    
            


            
		}
	}
	while (bytesRead > 0);

	return 0;
}

	// if ( (Sp = fopen(SIGFILE_NAME, "a") ) == NULL )
    // {
    //     printf("Error opening signal file \n");
    //     exit(1);
    // }
            // fprintf(Sp,"SIGN:1 \n");
                // fclose(Sp);