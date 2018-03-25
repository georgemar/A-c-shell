/* 
 * File:   mysh4.c
 * Author: georgemaroulis
 *
 * Created on 18 Οκτωβρίου 2016, 12:21 πμ
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#define MAX_LEN 1024

char com[MAX_LEN];
char *end = "exit\0";
int main(void) {
    do {
        com[0]='\0';
        int col=0;
        char **args;
        char **args2;
        int j=0;
        int i=0;
        int *awc;
        int ac;
        char *pp;
        pp=(char *)malloc(sizeof(char)*2);
        pp[0]='|';
        pp[1]='\0';
        char *keno;
        keno=(char *)malloc(sizeof(char));
        keno[0]='\0';
        int pipc=0;
        int flag=0;
        int flag1=0;
        int entol=1;
        int a=0;
        int b=0;
        int c=0;
        int *clen;
        char ***commands;
        int ncflg=0;
        j = 0;
        i = 0;
        ac=0;
        fflush(stdin);
        printf("$ ");
        fgets(com, MAX_LEN , stdin);
        com[strlen(com)-1]='\0';
        args2=(char **)malloc(sizeof(char *));
        args2[0]=(char *)malloc(sizeof(char));
        args2[0][0]='\0';
        while (com[i] != '\0') {
            if ( com[i] != '\t' && com[i] != ' ' && com[i] != '|'  ) {
                args2[j][c]=com[i];
                c++;
                args2[j]=(char *)realloc(args2[j],sizeof(char)*(c+1));
                args2[j][c]='\0';
            } else {
                if (com[i] == '|' ) {
                    j++;
                    args2=(char **)realloc(args2,sizeof(char *)*(j+1));
                    args2[j]=(char *)malloc(sizeof(char)*2);
                    args2[j][0]='|';
                    args2[j][1]='\0';
                    c=0;
                    j++;
                    args2=(char **)realloc(args2,sizeof(char *)*(j+1));
                    args2[j]=(char *)malloc(sizeof(char));
                    args2[j][0]='\0';
                } else {
                    j++;
                    args2=(char **)realloc(args2,sizeof(char *)*(j+1));
                    args2[j]=(char *)malloc(sizeof(char));
                    args2[j][0]='\0';
                    c=0;
                    j++;
                    args2=(char **)realloc(args2,sizeof(char *)*(j+1));
                    args2[j]=(char *)malloc(sizeof(char));
                    args2[j][0]='\0';
                }
            }
            i++;
        }
        j++;
        args2=(char **)realloc(args2,sizeof(char *)*(j+1));
        args2[j]=NULL;
        i=0;

        flag=0;
        args=(char **)malloc(sizeof(char *));
        col=0;
        for (i=0; i<j; i++) {
            if (strcmp(keno,args2[i]) != 0 && flag == 0 ) {
                args[0]=(char *)malloc(sizeof(char)*(strlen(args2[i])+1));
                strcpy(args[0],args2[i]);
                flag=1;
                col++;
            } else if (strcmp(keno,args2[i]) != 0 && flag != 0 ) {
                args=(char **)realloc(args,sizeof(char *)*(col+1));
                args[col]=(char *)malloc(sizeof(char)*(strlen(args2[i])+1));
                strcpy(args[col],args2[i]);
                col++;
            }
        }
        args=(char **)realloc(args,sizeof(char *)*(col+1));
        args[col]=NULL;
        j=0;
        clen=(int *)malloc(sizeof(int));
        clen[0]=0;
        for (i=0; i<col; i++){
            if ( strcmp(pp,args[i]) == 0 ){
                entol++;
                j++;
                clen=(int *)realloc(clen,sizeof(int)*(j+1));
                clen[j]=0;
            } else {
                clen[j]++;
            }
        }
        commands=(char ***)malloc(sizeof(char **)*entol);
        for (i=0; i<entol; i++ ){
            commands[i]=(char **)malloc(sizeof(char *)*clen[i]+1);
            commands[i][clen[i]]=NULL;
        }
        i=0;
        while (i<col) {
            if ( strcmp (pp,args[i]) != 0 ) {
                commands[a][b]=(char *)malloc(sizeof(char)*(strlen(args[i])));
                strcpy(commands[a][b],args[i]);
                b++;
            } else {
                a++;
                b=0;
            }
            i++;
        }
        if (entol > 1) {
            int **fd;
            fd=(int **)malloc(sizeof(int *)*(entol-1));
            for (i=0; i<entol-1; i++){
                fd[i]=(int *)malloc(sizeof(int)*2);
                pipe(fd[i]);
            }
            entol=entol-1;
            for (i=0; i<=entol; i++){
                pid_t pid=fork();
                if (pid < 0 ){
                    return 1;
                } else if (pid == 0) {
                    if (i==0){
                        close(fd[0][0]);
                        dup2(fd[0][1],1);
                        execvp(commands[i][0],commands[0]);
                        goto end;
                    } else if (i<entol && i>0){
                        close(fd[i-1][1]);
                        dup2(fd[i-1][0],0);
                        dup2(fd[i][1],1);
                        execvp(commands[i][0],commands[i]);
                        goto end;
                    } else if (i==entol){
                        close(fd[i-1][1]);
                        dup2(fd[i-1][0],0);
                        execvp(commands[i][0],commands[i]);
                        goto end;
                    }
                } else {
                    if (i<entol){
                        close(fd[i][1]);
                    }
                    int stat;
                    while ( (pid=waitpid(-1, &stat, 0)) > 0 ) {
                
                    }
                    fflush(stdin);
                    fflush(stdout);
                }
            }
        } else if (entol == 1) {
            if (commands[0][0] == NULL) {
                goto end2;
            }
            pid_t pid = fork();
            if (pid < 0) {
                return 1;
            } else if ( pid == 0 ) {
                execvp(commands[0][0],commands[0]);
                break;
            } else {
                int stat;
                while ( (pid=waitpid(-1, &stat, 0)) > 0 ) {
                
                }
                char *cd = "cd";
                if ( args[0] != NULL && strcmp(cd,args[0]) == 0 ) {
                    chdir(args[1]);
                }
            }
        }
        end2:;
    } while (strcmp(com, end) != 0) ; 
    end:;
    return EXIT_SUCCESS;
}