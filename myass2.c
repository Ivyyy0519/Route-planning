/* Solution to comp10002 Assignment 2, 2019 semester 2.

   Authorship Declaration:

   (1) I certify that the program contained in this submission is completely
   my own individual work, except where explicitly noted by comments that
   provide details otherwise.  I understand that work that has been developed
   by another student, or by me in collaboration with other students,
   or by non-students as a result of request, solicitation, or payment,
   may not be submitted for assessment in this subject.  I understand that
   submitting for assessment work developed by or in collaboration with
   other students or non-students constitutes Academic Misconduct, and
   may be penalized by mark deductions, or by other penalties determined
   via the University of Melbourne Academic Honesty Policy, as described
   at https://academicintegrity.unimelb.edu.au.

   (2) I also certify that I have not provided a copy of this work in either
   softcopy or hardcopy or any other form to any other student, and nor will
   I do so until after the marks are released. I understand that providing
   my work to other students, regardless of my intention or any undertakings
   made to me by that other student, is also Academic Misconduct.

   (3) I further understand that providing a copy of the assignment
   specification to any form of code authoring or assignment tutoring
   service, or drawing the attention of others to such services and code
   that may have been made available via such a service, may be regarded
   as Student General Misconduct (interfering with the teaching activities
   of the University and/or inciting others to commit Academic Misconduct).
   I understand that an allegation of Student General Misconduct may arise
   regardless of whether or not I personally make use of such solutions
   or sought benefit from such actions.

   Signed by: Yuxin Yan 1067815
   Dated:     2019.09.28

*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define DEFAULT 0
#define FALSE 0
#define TRUE 1
#define COUNT 1
#define DIRRC_DIFF 1
#define INITIAL_N 1
#define GOAL_N 2
#define COORDINATE_NUM 2
#define NUM_INIANDGOAL 2
#define STA_ONE 1
#define STA_TWO 2
#define STA_THREE 3
#define STA_FOUR 4
#define STA_FIVE 5
#define MAXCELL 5
#define MAXNUM 10
#define BRACKET '['
#define INITIAL 'I'
#define GOAL 'G'
#define BLOCK '#'
#define ROUTE '*'
#define BLANK ' '
#define DOLLAR '$'
#define ROUTE_TARGET '^'
#define STATUSONE "Initial cell in the route is wrong!\n"
#define STATUSTWO "Goal cell in the route is wrong!\n"
#define STATUSTHREE "There is an illegal move in this route!\n"
#define STATUSFOUR "There is a block on this route!\n"
#define STATUSFIVE "The route is valid!\n"
#define STAGE0 "==STAGE 0=======================================\n"
#define STAGE1 "==STAGE 1=======================================\n"
#define STAGE2 "==STAGE 2======================================="
#define END "================================================"
#define LINE "------------------------------------------------\n"
#define FAIL_REPAIR "The route cannot be repaired!\n"

typedef struct{           //coordinate of a cell
        int row;
        int col;
        int count;  //member count is only used when repairing the route
}cell_t;
typedef cell_t data_t;
typedef struct node node_t;
struct node {
        data_t data;
        node_t *next;
        node_t *prev;      //can be used as double directions
};
typedef struct {
        node_t *head;
        node_t *foot;
} list_t;

void free_array(int row, char **arr);
void free_list(list_t *list);
void print_info(int row, int col, int block, cell_t initial, cell_t goal);
void print_route(list_t *list);
void print_cell(cell_t cell);
void repair_stage1(int row, int col, cell_t initial, cell_t goal, list_t *list,
        char **arr, int status);
void print_grid(int row, int col, char **arr);
void print_status(int status);
void clear_wrong_route(int row, int col, char **arr);
void read_stage2(int row, int col, cell_t initial, cell_t goal, char **arr,
        list_t *list);
void scan_printf(list_t *list);
void store_route(list_t *list);
void fail_repair_route(int row, int col, char**arr);
void full_repair_stage2(int row, int col, cell_t initial, cell_t goal, 
	list_t *list, list_t *pair, list_t *replace, char **arr);
int repair_route_single(int row, int col, list_t *list, list_t *pair,
        list_t *replace, char **arr);
int read_stage0(int *prow, int *pcol, char**arr, cell_t* pinitial,
        cell_t* pgoal, list_t *list);
int check_status(cell_t initial, cell_t goal, int row, int col,
        list_t *list, char **arr);
int inside_dim(cell_t cell, int row, int col);
int legal_move(cell_t cell1, cell_t cell2);
int check_block(list_t *list, char **arr);
int check_move(list_t *list, int row, int col);
int check_pair(char **arr, cell_t curr);
int is_empty_list(list_t *list);
int repair_route(int row, int col, list_t *pair, char **arr);
int add_queue(int row, int col, cell_t prev, list_t *pair, char **arr,
        int r_limit, int c_limit);
int search_pair_queue(list_t *pair, cell_t new);
int finish_repair_route(int row, int col, list_t *list, list_t *pair,
        list_t *replace, char **arr, cell_t initial, cell_t goal);
char **create_array(int row, int col);
char **mark_cell(cell_t initial, cell_t goal, list_t *list, list_t *pair,
        char **arr);
node_t *find_route_back_cell(int count, node_t *p, list_t *pair,
        list_t *replace);
cell_t *create_pair(int row, int col, cell_t prev, int r_limit, int c_limit,
        char**arr, cell_t*new_p);
list_t *copy_list(list_t *replace);
list_t *make_empty_list(void);
list_t *insert_at_foot(list_t *list, data_t value);
list_t *insert_at_head(list_t *list, data_t value);
list_t *find_route_back(list_t *pair, list_t *replace);
list_t *replace_new(list_t *old, list_t *new, cell_t start, cell_t end);

/*main program*/
int
main(int argc, char *argv[]) {
        list_t* list;      //list stores route
        list = make_empty_list();
        scan_printf(list);
        free_list(list);
        return 0;
}
/* scan the text and visualize the route, gather all the process*/
void
scan_printf(list_t *list){
        char new;
        int row, col, status, *prow=&row, *pcol=&col;
        cell_t initial, goal, *pinitial=&initial, *pgoal=&goal;
        printf(STAGE0);
        scanf("%dx%d\n", &row, &col);    //get the first line's dimension
        char **arr=create_array(row, col);        //malloc a 2d array
        status=read_stage0(prow, pcol, arr, pinitial, pgoal, list);
        repair_stage1(row, col, initial, goal, list, arr, status);
        if(scanf("%c", &new)==EOF){                      //no stage 2
                printf(END);
        }else{
        		scanf("%c", &new);   //read the newline char
        		read_stage2(row, col, initial, goal, arr, list);
        }
        free_array(row, arr);
}
/*read blocks and route from stdin, print the basic information */
int
read_stage0(int *prow, int *pcol, char**arr, cell_t* pinitial,
        cell_t* pgoal, list_t *list){
        int x, y, block, count=DEFAULT, status;
        int *pblock=&block;
        while (scanf("[%d,%d]\n", &x, &y)==COORDINATE_NUM){
                count++;
                if (count==INITIAL_N){  //record initial
                        pinitial->row=x;
                        pinitial->col=y;
                }else if (count==GOAL_N){  //record goal
                        pgoal->row=x;
                        pgoal->col=y;
                }else{                  //read blocks into array
                        arr[x][y]=BLOCK;
                }
        }
        *pblock=(count-NUM_INIANDGOAL);  //substract the initial and the goal
        print_info(*prow, *pcol, *pblock, *pinitial, *pgoal);
        store_route(list);
        printf("The proposed route in the grid is:\n");
        print_route(list);
        status=check_status(*pinitial, *pgoal, *prow, *pcol, list, arr);
        print_status(status);
        return status;   //return the route status for further process
}
/*mark the route, initial, goal into the array and visualize them,
then call function to repair, if still has stage 2, return true*/
void
repair_stage1(int row, int col, cell_t initial, cell_t goal, list_t *list,
        char **arr, int status){
        printf(STAGE1);
        list_t *pair=make_empty_list();      //queue storing repair pairs
        list_t *replace=make_empty_list();   //storing new replace route
        print_grid(row, col, mark_cell(initial, goal, list, pair, arr));
        if (status!=STA_FOUR){
        }else{                  //the route needs to be repaired
                if(finish_repair_route(row, col, list, pair, replace,
                        arr, initial, goal)){
                }else{
                        fail_repair_route(row, col, arr);
                }
        }
        free_list(pair);
        free_list(replace);
}
/*the processing of stage 2, keep reading new blocks and repair the old route*/
void
read_stage2(int row, int col, cell_t initial, cell_t goal, char **arr,
        list_t *list){
        int i;
        char new;
        printf(STAGE2);
        /*malloc pair list to store count and cell, replace to store the 
        right part of route to be replaced */
        list_t *pair=make_empty_list();  
        list_t *replace=make_empty_list();
        /* repair the first group, the dollar has been read*/
        for (i=0;i<row;i++){            //clear the array cells
                memset(arr[i], BLANK, col*sizeof(char));
        }
        full_repair_stage2(row, col, initial, goal, list, pair, replace, arr);
        /*keep reading the rest blocks*/
        while(scanf("%c", &new)!=EOF){
                if(new==DOLLAR){
                        scanf("\n");
                list_t *pair=make_empty_list();
                list_t *replace=make_empty_list();
                for (i=0;i<row;i++){                   //clear the array cells
                memset(arr[i], BLANK, col*sizeof(char));
                }
                full_repair_stage2(row, col, initial, goal, list, pair, 
                	replace, arr);
                }
        }
}
/*this function gathers stage2 multiple repair process*/
void
full_repair_stage2(int row, int col, cell_t initial, cell_t goal, list_t *list, 
	list_t *pair, list_t *replace, char **arr){
	//rep_suc is a flag to mark whether successfully repaired 
	int x, y, status, rep_suc;   
	while(scanf("[%d,%d]\n", &x, &y)==COORDINATE_NUM){
                        arr[x][y]=BLOCK;      //put new blocks into the array
        }
        printf("\n");
        /*print original route*/
        print_grid(row, col, mark_cell(initial, goal, list, pair, arr));
        if((status=check_status(initial, goal,row, col, list, arr))!=
        	STA_FOUR){
                printf(END);                //do not need to repair
        }else{   /*keep repairing if is still status four */
        		while(check_status(initial, goal,row, col, list, arr)==
        			STA_FOUR){
        			 free_list(pair);      
        			 free_list(replace);
        			list_t *pair=make_empty_list();
        			list_t *replace=make_empty_list();
        			mark_cell(initial, goal, list, pair, arr);
        			/*if can be repaired, check the next broken
        			start in the mark_cell, put it to pair list */
        			 if(repair_route_single(row, col, list, pair, 
        			 	 replace, arr)){
        			 	 mark_cell(initial, goal, list, pair, 
        			 	 	 arr);
        			 	 rep_suc=TRUE;         
        			 }else{
        			 	 /*mark the flag to be false*/
        			 	 fail_repair_route(row, col, arr);
        			 	 rep_suc=FALSE;
        			 	 break;
        			 }
        		}
        		if(rep_suc){
			printf(LINE);
			print_grid(row, col, arr);
			printf(LINE);
			print_route(list);
			print_status(check_status(initial, goal, row, 
				col, list, arr));
			printf(END);
		}
	}
	free_list(pair);
        	free_list(replace);
}
/*try to repair, if successfully, print the repaired grid, return TRUE*/
int
finish_repair_route(int row, int col, list_t *list, list_t *pair,
        list_t *replace, char **arr, cell_t initial, cell_t goal){
        /*check if can be repaired first*/
        if(repair_route_single(row, col, list, pair, replace, arr)){
                printf(LINE);
                print_grid(row, col, mark_cell(initial, goal, list, pair, arr));
                printf(LINE);
                print_route(list);
                /*now should be valid*/
                print_status(check_status(initial, goal, row, col, list, arr));
                return TRUE;
        }
        return FALSE;
}
/*print the grid and status when fail to repair*/
void
fail_repair_route(int row, int col, char**arr){
        printf(LINE);
        print_grid(row, col, arr);
        printf(LINE);
        printf(FAIL_REPAIR);
        printf(END);
}
/*the process of repairing single broken segment,
return true if can be repaired*/
int
repair_route_single(int row, int col, list_t *list, list_t *pair,
        list_t *replace, char **arr){
        if(repair_route(row, col, pair, arr)){     //route can be repaired
                replace=find_route_back(pair, replace);
                /*find replace route and insert it to the list*/
                replace_new(list, replace, pair->head->data, pair->foot->data);
                clear_wrong_route(row, col, arr);
                return TRUE;
        }
        return FALSE;
}
/*replace the wrong part of the original route by new route */
list_t
*replace_new(list_t *old, list_t *new, cell_t start, cell_t end){
        node_t *p_start=NULL, *p=old->head, *prev=NULL;
        list_t *copy=copy_list(new);
        while(p){       //find the start in the old list
                if((p->data.row==start.row)&&(p->data.col==start.col)){
                        p_start=p->next;
                        /*connect the head of the new to the old*/
                        p->next=copy->head;
                        break;
                }
                p=p->next;
        }
        while(p_start){        //now find the end in the old list
                if ((p_start->data.row==end.row)&&
                	(p_start->data.col==end.col)){
                        copy->foot->next=p_start;
                        break;
                }
                prev=p_start;
                p_start=p_start->next;
                free(prev);     //free all the pointers in the wrong route
                prev=NULL;
        }
        return old;
}
/*copy the list content and insert to the route list*/
list_t
*copy_list(list_t *replace){
        cell_t newdata;
        list_t *new=make_empty_list();   //make a copy of replace
        assert(replace!=NULL);
        node_t *p=replace->head;
        while(p){
                newdata=p->data;
                insert_at_foot(new, newdata);    //copy the data
                p=p->next;
        }
        return new;
}
/*find the route back to the start of the segment and
construct the replace route */
list_t
* find_route_back(list_t *pair, list_t *replace){
        //start from the previous count of the end of segment
        int count=(pair->foot->data.count)-COUNT;
        /*store the previous location of searching in pair*/
        node_t *prev=NULL;   
        while(count>DEFAULT){
                prev=find_route_back_cell(count, prev, pair, replace);
                count--;
        }
        return replace;
}
/*find the next cell of the replace route and create the replace route*/
node_t
*find_route_back_cell(int count, node_t *p, list_t *pair, list_t *replace){
        /*flags to mark whether found the cell*/
        int row, col, find_u=FALSE, find_d=FALSE, find_l=FALSE, find_r=FALSE;
        cell_t up, down, left, right, curr;
        if(is_empty_list(replace)){
                row=pair->foot->data.row, col=pair->foot->data.col;
        }else{
                row=replace->head->data.row;
                col=replace->head->data.col;
        }
        if(p==NULL){      //first search, no previous position
                p=pair->foot;
                while(p){
                //jump through the cell until find the required count
                        if((p->data.count)==count){
                                break;
                        }
                        p=p->prev;
                }
        }
        /*do not go beyond the count limit*/
        while(p&&((p->data.count)==count)){
                curr=p->data;
                if(curr.col==col){
                        if(curr.row==(row-DIRRC_DIFF)){
                                up=curr;
                                /*mark this direction's cell has been found*/
                                find_u=TRUE;
                        }else if(curr.row==(row+DIRRC_DIFF)){
                                down=curr;
                                find_d=TRUE;
                        }
                }else if (curr.row==row){
                        if(curr.col==(col-DIRRC_DIFF)){
                                left=curr;
                                find_l=TRUE;
                        }else if(curr.col==(col+DIRRC_DIFF)){
                                right=curr;
                                find_r=TRUE;
                        }
                }
                p=p->prev;
        }
        if(find_u){
                insert_at_head(replace, up);
        }else if(find_d){
                insert_at_head(replace, down);
        }else if(find_l){
                insert_at_head(replace, left);
        }else if(find_r){
                insert_at_head(replace, right);
        }
        /*beyond the required count, return this pointer as the next start*/
        return p;
}
/*print the grid and route */
void
print_grid(int row, int col, char **arr){
        int i, j;
        printf(" ");
        for (i=0;i<col;i++){       //visual the column cell (first line)
                printf("%d", i%MAXNUM);
        }
        printf("\n");
        for (i=0;i<row;i++){
                printf("%d", i%MAXNUM);
                for (j=0;j<col;j++){
                        if(arr[i][j]==ROUTE_TARGET){
                                printf("*");   //change the symbol
                        }else{
                                printf("%c", arr[i][j]);
                        }
                }
                printf("\n");
        }
}
/*create double direction queue storing count, return true if can be repaired*/
int
repair_route(int row, int col, list_t *pair, char **arr){
        node_t *p=pair->head;   //pointer to the start
        while(p){
                if(add_queue(-DIRRC_DIFF, FALSE, p->data, pair, arr,
                        row, col)){
                        return TRUE;
                }
                if(add_queue(DIRRC_DIFF, FALSE, p->data, pair, arr,
                        row, col)){
                        return TRUE;
                }
                if(add_queue(FALSE, -DIRRC_DIFF, p->data, pair, arr,
                        row, col)){
                        return TRUE;
                }
                if(add_queue(FALSE, DIRRC_DIFF, p->data, pair, arr,
                        row, col)){
                        return TRUE;
                }
                p=p->next;
        }
        return FALSE;        //can not be repaired
}
/*create a new pair of current pair according to the given directions,
return its pointer*/
cell_t
*create_pair(int row, int col, cell_t prev, int r_limit, int c_limit,
        char**arr, cell_t*new_p){
        /*inside the array and no block*/
        if ((DEFAULT<=prev.row+row)&&(prev.row+row<r_limit)
                &&(DEFAULT<=prev.col+col)&&(prev.col+col<c_limit)&&
                (arr[prev.row+row][prev.col+col]!=BLOCK)){
                new_p->row=prev.row+row;
                new_p->col=prev.col+col;
                new_p->count=prev.count+COUNT;
                return new_p;
        }
        return NULL;
}
/*put the new pairs around a cell into the queue,
if found the last pair, return true*/
int
add_queue(int row, int col, cell_t prev, list_t *pair, char **arr,
        int r_limit, int c_limit){
        cell_t new;
        cell_t *new_p=&new;
        if ((new_p=create_pair(row, col, prev, r_limit, c_limit,
                arr, new_p))!=NULL){
                /*has not been inserted into the queue before*/
                if (!search_pair_queue(pair, new)){
                        /*insert at foot, create a pointer pointing back*/
                        insert_at_foot(pair, new);
                }
                if (check_pair(arr, new)){
                        return TRUE;    //found the last pair in the queue
                }
        }
        return FALSE;
}
/* search the new pair in the queue, if already exit, return True */
int
search_pair_queue(list_t *pair, cell_t new){
        node_t *p=pair->head;
        assert(pair!=NULL&&pair->head!=NULL);
        while(p){
                if((p->data.row==new.row)&&(p->data.col==new.col)){
                        return TRUE;
                }
                p=p->next;
        }
        return FALSE;
}
/*compare the current new pair to the target route,
return true if it meets the route*/
int
check_pair(char **arr, cell_t curr){
        return (arr[curr.row][curr.col]==ROUTE_TARGET);
}
/*store the route, initial and goal cells using their symbol,
meanwhile check if there is
broken route needs to be repaired*/
char
**mark_cell(cell_t initial, cell_t goal, list_t *list, list_t *pair,
        char **arr){
        int block=FALSE;
        cell_t prev;
        node_t *p=list->head;
        assert(list!=NULL&&list->head!=NULL);
         /*check current planed route, store the route without blocks in
         the array*/
        while(p){
                if(arr[p->data.row][p->data.col]!=BLOCK){
                        if(block){
                                /*mark the goal route using different symbol*/
                                arr[p->data.row][p->data.col]=ROUTE_TARGET;
                        }else{
                                arr[p->data.row][p->data.col]=ROUTE;
                        }
                }else if((!block)&&(arr[p->data.row][p->data.col]==BLOCK)){
                        prev.count=DEFAULT;
                        block=TRUE;
                        insert_at_foot(pair, prev);
                }
        prev=p->data;
        p=p->next;
        }
        if (arr[initial.row][initial.col]!=BLOCK){
                arr[initial.row][initial.col]=INITIAL;
        }
        if (arr[goal.row][goal.col]!=BLOCK){
                arr[goal.row][goal.col]=GOAL;
        }
        return arr;
}
/*before mark the new route to the array, clean the wrong part of the route */
void
clear_wrong_route(int row, int col, char **arr){
        int i, j;
        for(i=0;i<row;i++){
                for(j=0;j<col;j++){
                        if((arr[i][j]==ROUTE_TARGET)||(arr[i][j]==ROUTE)){
                                arr[i][j]=BLANK;
                        }
                }
        }

}
/* create an empty, get it from Program written by Alistair Moffat,
as an example for the book "Programming, Problem Solving,
and Abstraction with C"*/
list_t
*make_empty_list(void) {
        list_t *list;
        list = (list_t*)malloc(sizeof(*list));
        assert(list!=NULL);
        list->head = list->foot = NULL;
        return list;
}
/*insert node at head for a double direction list, get it from Program
    written by Alistair Moffat, as an example for the book
   "Programming, Problem Solving, and Abstraction with C",
   add the prev pointer which points to the previous node*/
list_t
*insert_at_head(list_t *list, data_t value) {
        node_t *new;
        new = (node_t*)malloc(sizeof(*new));
        assert(list!=NULL && new!=NULL);
        new->data = value;
        new->next = list->head;
        new->prev=NULL;
        list->head = new;
        if (list->foot==NULL) {
                /* this is the first insertion into the list */
                list->foot = new;
        }
        return list;
}
/*insert node at foot for a double direction list, get it from Program written
     written by Alistair Moffat, as an example for the book
   "Programming, Problem Solving, and Abstraction with C"*/
list_t
*insert_at_foot(list_t *list, data_t value) {
        node_t *new;
        new = (node_t*)malloc(sizeof(*new));
        assert(list!=NULL && new!=NULL);
        new->data = value;
        new->next = NULL;
        new->prev=list->foot;
        if (list->foot==NULL) {
                /* this is the first insertion into the list */
                list->head = list->foot = new;
        } else {
                list->foot->next = new;
                list->foot = new;
        }
        return list;
}
/*print the dimension and number of blocks */
void
print_info(int row, int col, int block, cell_t initial, cell_t goal){
        printf("The grid has %d rows and %d columns.\n", row, col);
        printf("The grid has %d block(s).\n", block);
        printf("The initial cell in the grid is [%d,%d].\n", initial.row,
                initial.col);
        printf("The goal cell in the grid is [%d,%d].\n", goal.row,
                goal.col);
}

/*malloc a 2D array, and return the array pointer*/
char
** create_array(int row, int col){
        int i;
        char **arr;
        arr=(char **)malloc(row*sizeof(char*));
        assert(arr!=NULL);
        for (i=0;i<row;i++){
                arr[i]=(char*)malloc(col*sizeof(char));      //add the null
                assert(arr[i]!=NULL);
                //initialize the array cells to be blanks
                memset(arr[i], BLANK, col*sizeof(char));
        }
        return arr;
}
/* free the memory of this 2d array */
void
free_array(int row, char **arr){
        int i;
        for (i=0;i<row;i++){
                free(arr[i]);
                arr[i]=NULL;
        }
        free(arr);
        arr=NULL;
}
/*free the list */
void
free_list(list_t *list) {
        node_t *curr, *prev;
        assert(list!=NULL);
        curr = list->head;
        while (curr) {
                prev = curr;
                curr = curr->next;
                free(prev);
                prev=NULL;
        }
        free(list);
        list=NULL;
}
/* read and store the route into a linked list in stage1,
return true indicates that the
dollar has been scaned*/
void
store_route(list_t *list){
        int x, y;
        char a;
        cell_t cell;
        scanf("\n$");
        while((scanf("%c", &a)!=EOF)&&(a!=DOLLAR)){
                if (a==BRACKET){
                        scanf("%d,%d]", &x, &y);   //start of a route cell
                        cell.row=x;
                        cell.col=y;
                        /*insert route cell into list*/
                        list = insert_at_foot(list, cell);
                }
        }
}
/* print the route */
void
print_route(list_t *list){
        int count=DEFAULT;
        node_t *p=list->head;
        assert(list!=NULL&&list->head!=NULL);
        while (p){
                count+=COUNT;
                print_cell(p->data);
                if (p->next){              //this cell is not the last one
                        printf("->");
                }
                /*still have cell but reach maxmum, change a line*/
                if ((count==MAXCELL)&&(p->next)){
                        count=DEFAULT;
                        printf("\n");
                }
                p=p->next;
        }
        printf(".\n");
}
/*print a cell according to a cell_t structure */
void
print_cell(cell_t cell){
        printf("[%d,%d]", cell.row, cell.col);
}
/*check and then print the status of the route*/
int
check_status(cell_t initial, cell_t goal, int row, int col,
        list_t *list, char **arr){
        assert(list!=NULL&&list->head!=NULL&&
                list->foot!=NULL);
        if ((list->head->data.row!=initial.row)||
                (list->head->data.col!=initial.col)){
                return STA_ONE;
        }else if ((list->foot->data.row!=goal.row)||
                (list->foot->data.col!=goal.col)){
                return STA_TWO;
        }else if (!(check_move(list, row, col))){
                return STA_THREE;
        }else if (!(check_block(list, arr))){
                return STA_FOUR;
        }else{
                return STA_FIVE;
        }
}
/*print the status */
void
print_status(int status){
        if (status==STA_ONE){
                printf("%s", STATUSONE);
        }else if (status==STA_TWO){
                printf("%s", STATUSTWO);
        }else if (status==STA_THREE){
                printf("%s", STATUSTHREE);
        }else if (status==STA_FOUR){
                printf("%s", STATUSFOUR);
        }else{
                printf("%s", STATUSFIVE);
        }
}
/*check if there is any illegal move in the route */
int
check_move(list_t *list, int row, int col){
        node_t *p_curr=list->head, *p_next;
        assert(list!=NULL&&list->head!=NULL);
        while ((p_next=p_curr->next)!=NULL){
                //still have route after this step
                if (inside_dim(p_curr->data, row, col)&&
                        inside_dim(p_next->data, row, col)){
                        if (legal_move(p_curr->data, p_next->data)){
                                p_curr=p_next;
                        }else{
                                return FALSE;
                        }
                }else{
                        return FALSE;
                }
        }
        return TRUE;
}

/*return FALSE if the route is beyong the limited dimension */
int
inside_dim(cell_t cell, int row, int col){
        return ((cell.row<=row)&&(cell.col<=col));
}
/* check if the connected two moves are legal that
moves only for 1 step in 1 direction */
int
legal_move(cell_t cell1, cell_t cell2){
        if (cell1.row==cell2.row){
                return ((cell1.col-cell2.col==-DIRRC_DIFF)||
                        (cell2.col-cell1.col==-DIRRC_DIFF));
        }else{
                return ((cell1.row-cell2.row==-DIRRC_DIFF)||
                        (cell2.row-cell1.row==-DIRRC_DIFF));
        }
        return FALSE;
}
/* check if the route contains block cells */
int
check_block(list_t *list, char **arr){
        node_t *p=list->head;
        assert(list!=NULL&&list->head!=NULL);
        while (p){
                if (arr[p->data.row][p->data.col]==BLOCK){
                        return FALSE;
                }
                p=p->next;
        }
        return TRUE;
}
/*check if the list is empty, get it from Program
written by Alistair Moffat, as an example for the book
"Programming, Problem Solving, and Abstraction with C" */
int
is_empty_list(list_t *list) {
        assert(list!=NULL);
        return list->head==NULL;
}

/*Algorithms are fun! */
