/*
 * This is a very minimal shell. It finds an executable in the
 * PATH, then loads it and executes it (using execv).
 *
 * The focus on this exercise is to use fork, PATH variables,
 * and execv.
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

#define MAX_ARGS		64
#define MAX_ARG_LEN	16
#define MAX_LINE_LEN	80
#define WHITESPACE	" ,\t\n"
#define TRUE 1

struct command_t {
   char *name;
   int argc = 0;
   char *argv[MAX_ARGS];
};

/* Function prototypes */
int parseCommand(char *, struct command_t *);
void printPrompt();
void readCommand(char *);
void freeMemory(struct command_t *);
void listDirectory();
void echoMessage(struct command_t *);

int main(int argc, char *argv[]) {
   int pid;
   int status;
   char cmdLine[MAX_LINE_LEN];
   struct command_t command;

   while (TRUE) {
      printPrompt();
      /* Read the command line and parse it */
      readCommand(cmdLine);
      parseCommand(cmdLine, &command);
      command.argv[command.argc] = NULL;

      if (strlen(command.name) > 1){
         printf("Commands are single characters.\n");
      }
      else{

         /* quit */
         if (!strcmp(command.name, "Q")){
            printf("Exiting shell...\n");
            freeMemory(&command);
            break;
         }

         /* matches each command to it's action or function */
         /* the command name pass is used to bypass creating an execution thread */
         switch (*command.name){

            /* copy */
            case 'C':
               strcpy(command.name, "cp");
               strcpy(command.argv[0], "cp");
               break;

            /* delete */
            case 'D':
               strcpy(command.name, "rm");
               strcpy(command.argv[0], "rm");
               break;

            /* echo */
            case 'E':
               /* if no message given skip to next command */
               if (command.argc < 2){
                  strcpy(command.name, "pass");
                  break;
               }

               strcpy(command.name, "echo");
               strcpy(command.argv[0], "echo");

               /* combine each individual argument into 1 string in argv[1] */
               if (command.argc > 2){
                  echoMessage(&command);
               }
               break;

            /* help manual */
            case 'H':
               strcpy(command.name, "more");
               strcpy(command.argv[0], "more");
               command.argv[1] = (char *) malloc(MAX_ARG_LEN);
               strcpy(command.argv[1], "shellDhelp.txt");
               command.argv[2] = NULL;
               break;

            /* list contents */
            case 'L':
               strcpy(command.name, "pass");
               listDirectory();
               break;

            /* create a file */
            case 'M':
               strcpy(command.name, "nano");
               strcpy(command.argv[0], "nano");
               break;

            /* print to console */
            case 'P':
               strcpy(command.name, "more");
               strcpy(command.argv[0], "more");
               break;

            /* clear shell display */
            case 'W':
               strcpy(command.name, "clear");
               strcpy(command.argv[0], "clear");
               break;

            /* executes given program in current directory */
            case 'X':
               /* ensures a program name was provided */
               if (command.argc < 2){
                  printf("No file specified.\n");
                  strcpy(command.name, "pass");
               }
               if (command.argc >= 2){
                  strcpy(command.name, "./");
                  strcat(command.name, command.argv[1]);
                  strcpy(command.argv[0], command.name);
               }

               command.argv[1] = NULL;
               break;

            /* not found command */
            default:
               printf("Invalid command\n");
               strcpy(command.name, "pass");
         }

         /* Create a child process to execute the command */
         pid = fork();
         if ((pid  == 0)) {
            /* checks for special cases that have already been executed */
            if (!strcmp(command.name, "pass")){
               exit(0);
            }
            /* Child executing command */
            execvp(command.name, command.argv);
            fprintf(stderr, "Failed to execute command: %s\n", command.name);
            exit(0);
         }
      }
      /* Wait for the child to terminate */
      wait(NULL);
      freeMemory(&command);
   }

   /* Shell termination */
   printf("\n\n shell: Terminating successfully\n");
   return 0;
}

/* End basic shell */

/* build the message arguments into a single string */
void echoMessage(struct command_t *cmd){
   size_t totalChars = cmd->argc;

   /* find the number of characters in the message */
   for (int i = 1; i < cmd->argc; i++){
      totalChars += strlen(cmd->argv[i]);
   }

   /* allocate a temporary string to build the message into */
   char *message = (char *) malloc(totalChars + 2);
   for (int i = 1; i < cmd->argc; i++){
      strcat(message, cmd->argv[i]);

      /* don't add a space after the last word */
      if ((i + 1) < cmd->argc){
         strcat(message, " ");
      }
   }

   /* allocate enough room in argv[1] to store the combined string */
   free(cmd->argv[1]);
   cmd->argv[1] = (char *) malloc(strlen(message) + 3);
   strcpy(cmd->argv[1], message);
   cmd->argv[2] = NULL;
}


/* format and output the current primary working directory (pwd)
   and the directory contents */
void listDirectory(){
   int pid;
   /* add a newline before printing pwd */
   printf("\n");

   /* create a thread to print the pwd */
   pid = fork();
   if(pid == 0){
      char *cmd = (char *) malloc(1 + strlen("pwd"));
      strcpy(cmd, "pwd");
      char *args[] = {cmd, NULL};
      execvp(cmd, args);
   }

   /* after pwd thread is done add another newline */
   wait(NULL);
   printf("\n");

   /* show the contents of the pwd */
   pid = fork();
   if(pid == 0){
      char *cmd = (char *) malloc(1 + strlen("ls"));
      char *arg = (char *) malloc(1 + strlen("-l"));;
      strcpy(cmd, "ls");
      strcpy(arg, "-l");
      char *args[] = {cmd, arg, NULL};
      execvp(cmd, args);
   }

   /* ensures directory contents are printed before asking for next command */
   wait(NULL);
}


/* Free the dynamic memory from malloc */
void freeMemory(struct command_t *cmd){
   free(cmd->name);
   /*printf("freed name\n");*/
   for(int i = 0; i <= cmd->argc; i = i + 1){
      free(cmd->argv[i]);
      /*printf("freed argv[%d]\n", i);*/
   }
}

/* Parse Command function */

/* Determine command name and construct the parameter list.
 * This function will build argv[] and set the argc value.
 * argc is the number of "tokens" or words on the command line
 * argv[] is an array of strings (pointers to char *). The last
 * element in argv[] must be NULL. As we scan the command line
 * from the left, the first token goes in argv[0], the second in
 * argv[1], and so on. Each time we add a token to argv[],
 * we increment argc.
 */
int parseCommand(char *cLine, struct command_t *cmd) {
   int argc;
   char **clPtr;
   char *extractedString;
   /* Initialization */
   clPtr = &cLine;	/* cLine is the command line */
   argc = 0;

   /* Fill argv[] */
   /* implemented with strcpy to avoid overwriting malloc pointer */
   while ((extractedString = strsep(clPtr, WHITESPACE)) != NULL) {
      cmd->argv[argc] = (char *) malloc(MAX_ARG_LEN);
      strcpy(cmd->argv[argc], extractedString);
      ++argc;
   }

   /* Set the command name and argc */
   cmd->argc = argc-1;
   cmd->name = (char *) malloc(MAX_ARG_LEN);
   strcpy(cmd->name, cmd->argv[0]);

   return 1;
}

/* End parseCommand function */

/* Print prompt and read command functions - Nutt pp. 79-80 */

void printPrompt() {
   /* Build the prompt string to have the machine name,
    * current directory, or other desired information
    */
   char promptString[] = "linux kcd33|>";
   printf("%s ", promptString);
}

void readCommand(char *buffer) {
   /* This code uses any set of I/O functions, such as those in
    * the stdio library to read the entire command line into
    * the buffer. This implementation is greatly simplified,
    * but it does the job.
    */
   fgets(buffer, 80, stdin);
}

/* End printPrompt and readCommand */
