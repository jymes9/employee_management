/***************************************************************************
*   Written by James Mutumba                                              *
*                                                                         *
*   This program maintains an employee database, with each employee       *
*   represented by his/her name, sex, age and job description.            *
*   ***********************************************************************/


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
static struct Employee *employee_list = NULL; /*pointer to the first employee in the list*/

/*Function Prototypes*/
static int read_line ( FILE *fp, char *line, int max_length );
static int read_string ( FILE *fp,
                         char *prefix, char *string, int max_length );
static void menu_add_employee(void);
static void menu_print_database(void);

/*******************************************************************************
 *   read_line():                                                              *
 *                                                                             *
 * Read line of characters from file pointer "fp", copying the characters      *
 * into the "line" string, up to a maximum of "max_length" characters, plus    *
 * one for the string termination character '\0'. Reading stops upon           *
 * encountering the end-of-line character '\n', for which '\0' is substituted  *
 * in the string. If the end of file character EOF is reached before the end   *
 * of the line, the failure condition (-1) is returned. If the line is longer  *
 * than the maximum length "max_length" of the string, the extra characters    *
 * are read but ignored. Success is returned (0) on successfully reading       *
 * a line.                                                                     *
 ******************************************************************************/

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

/*****************************************************************************
*                              read_string():                               *
*                                                                           *
* Reads a line from the input file pointer "fp", starting with the "prefix" *
* string, and filling the string "string" with the remainder of the contents*
* of the line. If the start of the line does not match the "prefix" string, *
* the error condition (-1) is returned. Having read the prefix string,      *
* read_string() calls read_line() to read the remainder of the line into    *
* "string", up to a maximum length "max_length", and returns the result.    *
*****************************************************************************/
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

/*******************************************************************************************
*               menu_add_employee():                                                      *
*                                                                                         *
* Function that adds new employees to the database in their correct positions and keeps   *
*  an ordered list.                                                                       *
*  Creates a new node pointer and allocates memory to it dynamically.                     *
*  Using the Surname as a sorting parameter it cycles through the list and ensures the    *
*  new employee is placed in the right position to maintain the ordered list.             *
*  Checks the input to ensure its valid before adding it to the list.                     *
*******************************************************************************************/

static void menu_add_employee(void)
{
        struct Employee *new,*cur,*prev;                           /*sets up node pointers for new employee and current and previous positions*/
        char agestring[4];
        new = (struct Employee *) malloc (sizeof(struct Employee)); /*allocates a block of memory for the new employee dynamically*/
        /*adds in the data*/
        fprintf(stderr, "Employee Name [Surname, other names]: ");
        do {
                read_line(stdin, new->name, MAX_NAME_LENGTH);
        } while (strcmp(new->name,"")==0||atoi(new->name)!=0);                       /*check for valid name input*/

/*loop that checks surnames to determine the alphabetic position to place the new employee*/
        for (cur = employee_list, prev = NULL;
             cur != NULL && strcmp(new->name, cur->name) > 0;
             prev = cur, cur = cur->next)
                ;
        fprintf(stderr, "Employee Gender [Enter F or M]: ");
        do {
                read_line (stdin, &new->sex, 1);
                if (new->sex == 'f') new->sex = 'F';
                if (new->sex == 'm') new->sex = 'M';
        }
        while (new->sex != 'F' && new->sex != 'M');             /*check for valid gender input*/

        fprintf(stderr, "Employee Age: ");

        do {
                read_line(stdin,agestring,3);
                new->age=atoi(agestring);       /*convert agestring into an integer*/
                if (new->age <= 0)
                        fprintf(stderr, "Incorrect age, please try again\n");
        } while ((new->age <= 0));                              /*check for valid gender input*/

        fprintf(stderr, "Employee job: ");
        do {
                read_line(stdin, new->job, MAX_JOB_LENGTH);
        } while (strcmp(new->job,"")==0||atoi(new->job)!=0);                         /*check for valid gender input*/


        new->next = cur;              /*links the new employee to the next employee in the correct position determined by the cur pointer */
        if (prev == NULL)
                employee_list = new;  /*checks if there are no employees before the new employees in the list then assigns them the first position*/
        else
                prev->next = new;     /*links the new employee to the previous employee in the correct position completing the double link*/
}


/********************************************************************
*         menu_print_database():                                   *
*                                                                  *
* Prints the  database of employees to standard output.            *
*  It uses the pointer to the first employee (employee_list) and   *
*  initialises it to the cur pointer which then cycles through     *
* the list printing the details for each employee                  *
********************************************************************/

static void menu_print_database(void)
{
        struct Employee *cur = employee_list; /*current node initialised to pointer of the first employee */
        if (employee_list == NULL) /*displays message if there are no employees in the list*/
                fprintf(stderr, "No entries");
        else {
                do {                      /*loops through the list printing details for each employee*/
                        printf("Name: %s\n", &cur->name);
                        printf( "Sex: %c\n", cur->sex);
                        printf( "Age: %i\n", cur->age);
                        printf("Job: %s\n\n", &cur->job);
                        cur = cur->next;
                } while (cur != NULL);
        }
}
/* menu_delete_employee():
 *
 * Delete new employee from database.
 */
static void menu_delete_employee(void)
{
        /* fill in the code here in part 2, and add any extra functions you need */
}


/* read file containing database of employees */
static void read_employee_database ( char *file_name )
{

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
