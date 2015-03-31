#include <stdio.h>

int main(int argc, char * argv[])
{
    FILE *      fpLeft = NULL;
    FILE *      fpRight = NULL;

    if (argc != 3) {
        fprintf(stderr, "command line: compare <file1> <file2>\n");
        exit(1);
    }

    fpLeft = fopen(argv[1], "rt");
    if (fpLeft == NULL) {
        fprintf(stderr, "Cannot open the file '%s'\n", argv[1]);
        exit(1);
    }

    fpRight = fopen(argv[2], "rt");
    if (fpRight == NULL) {
        fprintf(stderr, "Cannot open the file '%s'\n", argv[2]);
        exit(1);
    }

    while (1) {
        char    rgchLeft[1024];
        char    rgchRight[1024];
        int     i;

        /*
         *  Read a line from each of the files.
         */

        if (fgets(rgchLeft, sizeof(rgchLeft), fpLeft) == NULL) {
            if (feof(fpLeft)) {
                if ((fgets(rgchRight, sizeof(rgchRight), fpRight) == NULL) &&
                    feof(fpRight)) {
                    fprintf(stderr, "pass");
                    exit(0);
                }
            }
            
            fprintf(stderr, "fail");
            exit(-1);
        }

        if (fgets(rgchRight, sizeof(rgchRight), fpRight) == NULL) {
            fprintf(stderr, "fail");
            exit(-1);
        }

        /*
         *  Remove trailing whitespace
         */

        while(isspace(rgchLeft[strlen(rgchLeft)-1])) {
            rgchLeft[strlen(rgchLeft)-1] = 0;
        }

        while(isspace(rgchRight[strlen(rgchRight)-1])) {
            rgchRight[strlen(rgchRight)-1] = 0;
        }

        /*
         *  Compare the strings
         */

        if (strcmp(rgchLeft, rgchRight) != 0) {
            fprintf(stderr, "fail");
            exit(-1);
        }
    }
}
