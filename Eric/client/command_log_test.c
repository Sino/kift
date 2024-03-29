#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <stdio.h>
#include <sys/stat.h>
#include "libft/libft.h"


/*
**This function is given a string and it will create/append 
**the file command_log.txt with the date and time along with the
**string entered beneath it
*/


int    command_logs(char *command)
{
    time_t			rawtime;
	struct tm *timeinfo;
	char			*date;
    int             fd;
    char            *str;

    time (&rawtime);
	timeinfo = localtime (&rawtime);
	date = asctime (timeinfo);
    fd = open("command_log.txt", O_APPEND | O_WRONLY);
    if (fd < 0)
    {
        system("touch command_log.txt && chmod 777 command_log.txt");
        fd = open("command_log.txt", O_APPEND | O_WRONLY);
        if (fd < 0)
            return (-1);
    }
    str = ft_strdup(date);
    str = ft_strjoin(str, "\t");
    str = ft_strjoin(str, command);
    str = ft_strjoin(str, "\n");
    printf("%s", str);
    write(fd, str, ft_strlen(str));
    close(fd);
    return (1);
}

int main(int argc, char **argv)
{
    if (command_logs(argv[1]) < 0)
        return (-1);

    return (0);
}