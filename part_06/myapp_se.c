#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <selinux/selinux.h>
#include <unistd.h>

void get_new_context(const char *, char **);

int main(int argc, char *argv[]) {
    char *name;
    FILE *fp;
    int n;
    errno = 0;
    char *context;
    
    getcon(&context);
    char *p = strstr(context, ":myapp_se_t:");
    
    // Check the current security context of the process
    if(!p) {
        freecon(context);
        get_new_context("myapp_se_t", &context);
        
        // Set context to prepare the domain transition
        setexeccon(context);
        free(context);
        
        // Execute the program with the new security context
        execvp(argv[0], argv);
        
        perror("execvp");
        return 0;
    }
    freecon(context);
    
    if ( argc > 1 && strcmp(argv[1],"-r") == 0 ) {
    
        fp = fopen ("/tmp/myapp_se.123456", "r");
        if (!fp) {
            perror ("fopen");
            return errno;
        }
        
        n = fscanf (fp, "%m[ a-zA-Z]", &name);
        if (n == 1) {
            printf ("Your name is %s\n", name);
            free(name);
        }
        else if (errno != 0) {
            perror("fscanf");
        } 
        else {
            fprintf(stderr, "Invalid name\n");
            fclose (fp);
            return -1;
        }
        fclose (fp);
        return errno;
    }
    
    printf ("%s", "Name: ");
    n = scanf ("%m[ a-zA-Z]", &name);
    if (n == 1) {
        printf ("Your name is %s\n", name);
    }
    else if (errno != 0) {
        perror("scanf");
        return errno;
    } 
    else {
        fprintf(stderr, "Invalid name\n");
        return -1;
    }
    
    fp = fopen ("/tmp/myapp_se.123456", "w");
    fprintf (fp, "%s", name);
    fclose (fp);
    free(name);
    
    return 0;
}

void get_new_context(const char *domain, char **pcontext_new) {
    
    char *context;
    char *p, *level;
    
    getcon(&context);
    p = rindex(context, ':');
    int len = strlen(p);
    level = malloc(len + 1);
    strcpy(level, p);
    *p = '\0';
    p = rindex(context, ':');
    *(p + 1) = '\0';
    len += strlen(context) + strlen(domain);
    *pcontext_new = malloc(len + 1);
    strcpy(*pcontext_new, context);
    strcat(*pcontext_new, domain);
    strcat(*pcontext_new, level);
    
    freecon(context);
    free(level);
}
