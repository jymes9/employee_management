#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* maximum number of employees that can be stored at once (relevant only
   to storage using an array) */
#define MAX_EMPLOYEES 200
#define MAX_NAME_LENGTH 100
#define MAX_JOB_LENGTH  100

/* Employee structure
 */
struct Employee
{
        /* Employee details */
        char name[MAX_NAME_LENGTH+1]; /* name string */
        char sex;                /* sex identifier, either 'M' or 'F' */
        int age;                 /* age */
        char job[MAX_JOB_LENGTH+1]; /* job string */

        /* pointers to previous and next employee structures in the linked list
           (for if you use a linked list instead of an array) */
        struct Employee *prev, *next;
};

/* array of employees */
static struct Employee employee_array[MAX_EMPLOYEES];

/* number of employees stored */
static void menu_add_employee(void);
static int read_line ( FILE *fp, char *line, int max_length );
static void menu_print_database(void);
void sortcode(void);
int compare_employees(const void *p, const void *q);
int find_employee(char str[]);
static void menu_delete_employee(void);
static int num_employees = 0;
/* read_line():
 *
 * Read line of characters from file pointer "fp", copying the characters
 * into the "line" string, up to a maximum of "max_length" characters, plus
 * one for the string termination character '\0'. Reading stops upon
 * encountering the end-of-line character '\n', for which '\0' is substituted
 * in the string. If the end of file character EOF is reached before the end
 * of the line, the failure condition (-1) is returned. If the line is longer
 * than the maximum length "max_length" of the string, the extra characters
 * are read but ignored. Success is returned (0) on successfully reading
 * a line.
 */
static int read_line ( FILE *fp, char *line, int max_length )
{
        int i;
        char ch;

        /* initialize index to string character */
        i = 0;

        /* read to end of line, filling in characters in string up to its
           maximum length, and ignoring the rest, if any */
        for(;;)
        {
                /* read next character */
                ch = fgetc(fp);

                /* check for end of file error */
                if ( ch == EOF )
                        return -1;

                /* check for end of line */
                if ( ch == '\n' )
                {
                        /* terminate string and return */
                        line[i] = '\0';
                        return 0;
                }

                /* fill character in string if it is not already full*/
                if ( i < max_length )
                        line[i++] = ch;
        }

        /* the program should never reach here */
        return -1;
}

/* read_string():
 *
 * Reads a line from the input file pointer "fp", starting with the "prefix"
 * string, and filling the string "string" with the remainder of the contents
 * of the line. If the start of the line does not match the "prefix" string,
 * the error condition (-1) is returned. Having read the prefix string,
 * read_string() calls read_line() to read the remainder of the line into
 * "string", up to a maximum length "max_length", and returns the result.
 */
static int read_string ( FILE *fp,
                         char *prefix, char *string, int max_length )
{
        int i;

        /* read prefix string */
        for ( i = 0; i < strlen(prefix); i++ )
                if ( fgetc(fp) != prefix[i] )
                        /* file input doesn't match prefix */
                        return -1;

        /* read remaining part of line of input into string */
        return ( read_line ( fp, string, max_length ) );
}

/* menu_add_employee():
 *
 * Add new employee to database
 */
static void menu_add_employee(void)
{
        int age;
        char agestring[4];
        if (num_employees == MAX_EMPLOYEES) {
                fprintf(stderr,"Database is full; can't add more employees.\n");
                return;
        }

        fprintf(stderr,"Enter employee name: ");

        do {
                read_line(stdin,employee_array[num_employees].name, MAX_NAME_LENGTH);
        } while (strcmp(employee_array[num_employees].name,"")==0||atoi(employee_array[num_employees].name)!=0);                       /*check for valid name input*/

        fprintf(stderr,"Enter employee gender: ");
        do {
                read_line (stdin, &employee_array[num_employees].sex, 1);
                if (employee_array[num_employees].sex == 'f') employee_array[num_employees].sex = 'F';
                if (employee_array[num_employees].sex == 'm') employee_array[num_employees].sex = 'M';
        }
        while (employee_array[num_employees].sex != 'F' && employee_array[num_employees].sex != 'M');

        fprintf(stderr,"Enter age: ");
        do {
                read_line(stdin,agestring,3);
                employee_array[num_employees].age=atoi(agestring);
                if (employee_array[num_employees].age == 0)
                        fprintf(stderr, "Warning, age is 0, this could be due to bad input\n");
        } while ((employee_array[num_employees].age > 120) || (employee_array[num_employees].age <= 0));


        fprintf(stderr,"Enter employee job: ");

        do {
                read_line(stdin,employee_array[num_employees].job, MAX_JOB_LENGTH);
        } while (strcmp(employee_array[num_employees].job,"")==0||atoi(employee_array[num_employees].job)!=0);

        num_employees++;
}

/* menu_print_database():
 *
 * Print database of employees to standard output.
 */
static void menu_print_database(void)
{
        int i;
        qsort(employee_array, num_employees, sizeof(struct Employee), compare_employees);
        //sortcode();

        for(i=0; i<num_employees; i++) {
                printf("Name: %s\n", employee_array[i].name);
                printf("Sex: %c\n", employee_array[i].sex);
                printf("Age: %i\n", employee_array[i].age);
                printf("Job: %s\n\n", employee_array[i].job);
        }
        /* fill in the code here in part 1, and add any extra functions you need */
}

/* menu_delete_employee():
 *
 * Delete new employee from database.
 */
static void menu_delete_employee(void)
{
        int i;
        char delname[MAX_NAME_LENGTH+1];
        fprintf(stderr,"Enter employee name: ");
        read_line(stdin,delname, MAX_NAME_LENGTH);
        i = find_employee(delname);
        if (i >= 0) {
                for(i; i<num_employees; i++) {
                        employee_array[i]=employee_array[i+1];
                }
                num_employees--;
                fprintf(stderr,"%s deleted ",delname);
        } else
                fprintf(stderr,"Employee not found.\n");

}

/* read file containing database of employees */
static void read_employee_database ( char *file_name )
{
        FILE *input;
        input = fopen(file_name, "r"); /*opens file as read-only*/
        if (input == NULL) {     /*exits if file cannot be opened to prevent undefined behaviour*/
                fprintf(stderr, "Could not open file, exiting\n");
                exit(EXIT_FAILURE);
        }
        char test;
        char agestring[4];
        do {
                if (num_employees!=0)
                        ungetc(test, input); /*if no employees have been added, this must be the first time the cycle has passed and does not need to ungetc*/
                /*prefix's for the data inputs*/
                char *fname = "Name: ";
                char *fsex = "Sex: ";
                char *fage = "Age: ";
                char *fjob = "Job: ";

                if (read_string(input, fname, employee_array[num_employees].name, MAX_NAME_LENGTH) == -1) {
                        fprintf(stderr, "Invalid name input with employee %i, exiting\n",num_employees+1);
                        exit(EXIT_FAILURE);   /*exits if input data is wrong*/
                }
                if (strcmp(employee_array[num_employees].name,"")==0) {
                        fprintf(stderr, "Invalid name with employee %i, exiting\n",num_employees+1);
                        exit(EXIT_FAILURE);
                }

                if (read_string(input, fsex, &employee_array[num_employees].sex, 1) == -1) {
                        fprintf(stderr, "Invalid gender input with employee %i, exiting\n",num_employees+1);
                        exit(EXIT_FAILURE);
                }
                if (employee_array[num_employees].sex == 'f') employee_array[num_employees].sex = 'F';
                if (employee_array[num_employees].sex == 'm') employee_array[num_employees].sex = 'M';
                if (employee_array[num_employees].sex != 'F' && employee_array[num_employees].sex != 'M') {
                        fprintf(stderr, "Invalid gender with employee %i, exiting\n",num_employees+1);
                        exit(EXIT_FAILURE);
                }                                                                   /*checks for valid gender input and exits if input data is wrong*/

                if (read_string(input, fage, agestring, 3) == -1) {
                        fprintf(stderr, "Invalid age input with employee %i, exiting\n",num_employees+1);
                        exit(EXIT_FAILURE);   /*exits if input data is wrong*/
                }
                employee_array[num_employees].age=atoi(agestring);
                if (employee_array[num_employees].age <= 0) {
                        fprintf(stderr, "Incorrect age, with employee %i, exiting\n",num_employees+1);
                        exit(EXIT_FAILURE);
                }


                if (read_string(input, fjob, employee_array[num_employees].job, MAX_JOB_LENGTH) == -1) {
                        fprintf(stderr, "Invalid job input with employee %i , exiting",num_employees+1);
                        exit(EXIT_FAILURE); /*exits if input data is wrong*/
                }
                if (strcmp(employee_array[num_employees].job,"")==0) {
                        fprintf(stderr, "Invalid job with employee %i, exiting\n",num_employees+1);
                        exit(EXIT_FAILURE);
                }
                num_employees++;
                /*takes in the \n*/
                if ((test = fgetc(input)) != '\n') {
                        fprintf(stderr, "Bad input file, exiting. Details: Missing '\\n' at end of file\n");
                        exit(EXIT_FAILURE);
                }
        } while ((test = fgetc(input)) != EOF); /*checks if it's the end of the file */


        fclose(input); /*closes the file*/

        /* fill in the code here in part 3, and add any extra functions you need */
}

/* codes for menu */
#define ADD_CODE    0
#define DELETE_CODE 1
#define PRINT_CODE  2
#define EXIT_CODE   3

int main ( int argc, char *argv[] )
{
        /* check arguments */
        if ( argc != 1 && argc != 2 )
        {
                fprintf ( stderr, "Usage: %s [<database-file>]\n", argv[0] );
                exit(-1);
        }

        /* read database file if provided, or start with empty database */
        if ( argc == 2 )
                read_employee_database ( argv[1] );

        for(;;)
        {
                int choice, result;
                char line[301];

                /* print menu to standard error */
                fprintf ( stderr, "\nOptions:\n" );
                fprintf ( stderr, "%d: Add new employee to database\n", ADD_CODE );
                fprintf ( stderr, "%d: Delete employee from database\n", DELETE_CODE );
                fprintf ( stderr, "%d: Print database to screen\n", PRINT_CODE );
                fprintf ( stderr, "%d: Exit database program\n", EXIT_CODE );
                fprintf ( stderr, "\nEnter option: " );

                if ( read_line ( stdin, line, 300 ) != 0 ) continue;

                result = sscanf ( line, "%d", &choice );
                if ( result != 1 )
                {
                        fprintf ( stderr, "corrupted menu choice\n" );
                        continue;
                }

                switch ( choice )
                {
                case ADD_CODE: /* add employee to database */
                        menu_add_employee();
                        break;

                case DELETE_CODE: /* delete employee from database */
                        menu_delete_employee();
                        break;

                case PRINT_CODE: /* print database contents to screen
                                    (standard output) */
                        menu_print_database();
                        break;

                /* exit */
                case EXIT_CODE:
                        break;

                default:
                        fprintf ( stderr, "illegal choice %d\n", choice );
                        break;
                }

                /* check for exit menu choice */
                if ( choice == EXIT_CODE )
                        break;
        }

        return 0;
}


int find_employee(char str[])
{
        int i;

        for (i = 0; i < num_employees; i++) {
                if (strcmp(employee_array[i].name,str)==0)
                        return i;
        }
        return -1;
}

int compare_employees(const void *p, const void *q)
{
        return strcmp(((struct Employee *) p)->name,
                      ((struct Employee *) q)->name);

}
