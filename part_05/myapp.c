#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int main(int argc, char *argv[]) {
    char *name;
    FILE *fp;
    int n;
    errno = 0;
    
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
