#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <wait.h>
#include <sys/types.h>
/*void sigchld(int sig){
	wait();
	printf("Child stopped/ terminated\n");
}*/
int main(){

	pid_t backg[100];
	char cmd[100][200];
	char status[100][20];
	int bg_cnt=0;
	signal(SIGINT,SIG_IGN);
	signal(SIGQUIT,SIG_IGN);
	signal(SIGTTOU,SIG_IGN);
	signal(SIGTTIN,SIG_IGN);
	signal(SIGTSTP,SIG_IGN);
	while(1){
		printf("$$");
		char str[200];
		fgets(str,200,stdin);
		int ret, count = 0;;
		int i = 0;
		char *p;
		char *array[10];
		//printf("%d",strlen(str));
		str[strlen(str) - 1] = '\0';
		while(str[i]==' '&&i<strlen(str)){
			i++;
		}
		if(i==strlen(str))
			continue;
		if(str[0]=='\0')
			continue;
		p = strtok (str," "); 

		i=0;
		while (p != NULL)
		{
			array[i++] = p;
			p = strtok (NULL," ");
			count++;
		}
		array[i]=NULL;
		
		for(i=0;i<bg_cnt;i++){
			if(waitpid(backg[i],NULL,WNOHANG)>0)
			{
				strcpy(status[i],"Completed.");
			}	
		}
		if(strcmp(str,"exit")==0){
			printf("Exiting...\n");
			exit(0);
		}
		else if(strcmp(array[0],"start")==0){
			if(array[1]!=NULL){
				if(kill(atoi(array[1]),SIGCONT)==-1)
					printf("Can't Start.\n");
			}
			else
				printf("Invalid number of arguments.\n");
		}
		else if(strcmp(array[0],"stop")==0){
			if(array[1]!=NULL)
			{
				if(kill(atoi(array[1]),SIGSTOP)==-1)
					printf("Can't Stop.\n");
			}	
			else
				printf("Invalid number of arguments.\n");
		}
		else if(strcmp(array[0],"jobs")==0&&array[1]==NULL){
			printf("Background processes : \n");
			for(i=0;i<bg_cnt;i++){
					printf("#%d\t%d\t%s\t%s\n",i+1,backg[i],cmd[i],status[i]);
			}
		}
		else{
			ret = fork();
			if(ret<0){
				perror("Fork failed\n");
			}
			else if(ret==0){
				if(strcmp(array[count-1],"&")==0)
				{	
						//sleep(1);
						signal(SIGINT,SIG_DFL);
						signal(SIGQUIT,SIG_DFL);
						signal(SIGTTOU,SIG_IGN);
						//printf("fg group is %d\n", tcgetpgrp(0));	
						setpgid(getpid(),getpid());
						array[count-1]=NULL;
						//printf("bg group is %d\n", getpid());
						int run = execvp(array[0],array);
						if(run==-1)
							printf("Command invalid to run in background.\n");	
						exit(0);
				}
				else{
						//sleep(1);
						signal(SIGTTOU,SIG_IGN);
						signal(SIGTSTP,SIG_DFL);
						//printf("fg group is %d\n", tcgetpgrp(0));	
						setpgid(getpid(),getpid());		
						tcsetpgrp(0,getpid());
						//printf("fg group is %d\n", tcgetpgrp(0));	
						int run = execvp(array[0],array);
						if(run==-1)
							printf("Command invalid to run in foreground.\n");
						exit(0);
				}		
			}
			else{
				if(strcmp(array[count-1],"&")==0)
				{
					backg[bg_cnt]=ret;
					strcpy(cmd[bg_cnt],str);
					strcpy(status[i],"Running.");
					bg_cnt++;
					//printf("Copied to jobs\n");
					continue;		
				}
				else{
					int status;
					waitpid(ret,&status,WUNTRACED);
					//printf("Command execution completed.\n");
					//sleep(2);
					tcsetpgrp(0,getpid());
				}
			}
		}
	}
	return 0;
}
