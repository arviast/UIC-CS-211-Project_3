#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

int debugMode;

typedef struct mazeStruct
{
//    char arr[32][32];   /* allows for a maze of size 30x30 plus outer walls */
    char **arr;
//    char visited[32][32];  // allows to create new maze for visited
    char **visited;
    int xsize, ysize;
    int xstart, ystart;
    int xend, yend;
} maze;

typedef struct Node
{
    int xOfElement;  // x coordination of node
    int yOfElement;  // y coordination of node
    struct Node* Next;
}stack;

// Check if empty
int checkIfEmpty(stack *top)
{
    if(top == NULL) {
        return 1;
    }
    return 0;
}

// Push function with debug mode
void push(stack **top, int x, int y) {
    stack *tmp = (stack *) malloc(sizeof(stack));
    tmp->xOfElement = x;
    tmp->yOfElement = y;
    tmp->Next = *top;
    *top = tmp;
    
    if(debugMode == TRUE) {
        printf("Pushed element with coordinates: (%d and %d)\n", x, y);
    }
}

// initializing the stack
void initialize(stack **top, int xstart, int ystart)  {
    stack *temp = (stack*) malloc(sizeof(stack));
    temp->xOfElement = xstart;
    temp->yOfElement = ystart;
    temp->Next = NULL;
    *top = temp;
    push(top, xstart, ystart);
}

// Pop element with debug mode
void pop(stack **top) {
    stack *temp = *top;
    if(debugMode == TRUE) {
        printf("Popped element with coordinates: (%d %d)\n", (temp)->xOfElement, (temp)->yOfElement);
    }
    if(*top != NULL) {
        *top = (*top)->Next;
        free(temp);
    }
}

// Top function (works)
stack top (stack *top)
{
    printf("top is %d %d", top->xOfElement, top->yOfElement);
    return (*top);
}

// free and clear the stack
void clearStack(stack **top,int xstart, int ystart) {
    stack *temp = *top;
    while(temp != NULL) {
        pop(&temp);
    }
    free(temp);
}

// recursion function for reverse linked list
void reverseDisplay(stack *top){
    if(top->Next == NULL){
        return;
    }
    reverseDisplay(top->Next);
    printf("(%d,%d) ", top->xOfElement, top->yOfElement);
}


int main (int argc, char **argv)
{
  maze m1;
  int xpos, ypos;
  int i,j;

  stack *Top = NULL;   // declaring Head(Top)
    
  FILE *src;
  
    // Checks if there is parameter "-d"
    // ...
    char *fileTemp = NULL;  // for debugging mode
    
    int f;
    for(f = 0; f <argc; f++) {
        if(strcmp(argv[f], "-d") == 0) {
            debugMode = TRUE;
        }
        else {
            fileTemp = argv[f];
        }
    }
    
    // this is for flag in front and at the end of text file name
    argv[1] = fileTemp;

  /* verify the proper number of command line arguments were given */
  if(argc < 2) {
     fprintf(stderr, "Usage: %s <input file name>\n", argv[0]);
     exit(-1);
  }
   
  /* Try to open the input file. */
  if ( ( src = fopen( argv[1], "r" )) == NULL )
  {
    fprintf (stderr, "Can't open input file: %s", argv[1] );
    exit(-1);
  }
    
    // Printing message for Debug mode
    if(debugMode == TRUE) {
        printf("Debugging INFORMATION ON\n");
    }
    
  /* read in the size, starting and ending positions in the maze */
    fscanf (src, "%d %d", &m1.xsize, &m1.ysize);
    while (m1.xsize  <=  0 || m1.ysize <= 0)
      {
        fprintf(stderr, "Invalid: Maze sizes must be greater than 0\n");
        fscanf (src, "%d %d", &m1.xsize, &m1.ysize);
      }
    printf ("Maze becomes size: %d x %d\n", m1.xsize, m1.ysize);

    
    while ((fscanf (src, "%d %d", &m1.xstart, &m1.ystart))){
        if (feof(src)) {
            fprintf(stderr,"Reached the end of the file. No 3 Valid Inputs. Exiting ...\n");
            fclose(src);
            exit(-1);
        }
        if(m1.xstart  > m1.xsize || m1.ystart > m1.ysize || m1.xstart < 0 || m1.ystart < 0){
            fprintf(stderr,"Invalid: Maze start at %d %d\n", m1.xstart, m1.ystart);
        }
        else {
            break;
        }
    }
    printf ("Starting position is at position: %d, %d\n", m1.xstart, m1.ystart);
    
    while (fscanf (src, "%d %d", &m1.xend, &m1.yend)){
        if (feof(src)) {
            fprintf(stderr,"Reached the end of the file. No 3 Valid Inputs. Exiting ...\n");
            fclose(src);
            exit(-1);
        }
        if(m1.xend  > m1.xsize || m1.yend > m1.ysize || m1.xend < 0 || m1.yend < 0){
            fprintf(stderr,"Invalid: Maze end at %d %d\n", m1.xend, m1.yend);
        }
        else {
            break;
        }
    }
    printf ("Ending is at position: %d, %d\n", m1.xend, m1.yend);
    
    // dynamically allocate the size of arr and visited board
    m1.arr = (char **) malloc(sizeof(char *) * (m1.xsize + 2));
    m1.visited = (char **) malloc(sizeof(char *) * (m1.xsize + 2));
    for(i = 0; i < m1.xsize + 2; i++){
        m1.arr[i] = (char *) malloc(sizeof(char) * (m1.ysize + 2));
        m1.visited[i] = (char *) malloc(sizeof(char) * (m1.ysize + 2));
    }


  /* initialize the maze to empty */
    for (i = 0; i < m1.xsize+2; i++) {
        for (j = 0; j < m1.ysize+2; j++) {
            m1.arr[i][j] = '.';
            m1.visited[i][j] = '0';
        }
    }
 
  /* mark the borders of the maze with *'s */
  for (i=0; i < m1.xsize+2; i++)
    {
     m1.arr[i][0] = '*';
     m1.arr[i][m1.ysize+1] = '*';
     m1.visited[i][0] = '1';
     m1.visited[i][m1.ysize+1] = '1';
        
    }
  for (i=0; i < m1.ysize+2; i++)
    {
     m1.arr[0][i] = '*';
     m1.arr[m1.xsize+1][i] = '*';
     m1.visited[0][i] = '1';
     m1.visited[m1.xsize+1][i] = '1';
    }

  /* mark the starting and ending positions in the maze */
  m1.arr[m1.xstart][m1.ystart] = 's';
  m1.arr[m1.xend][m1.yend] = 'e';
    
  // Declaring new int's for my tracking
  int xcur = m1.xstart, ycur = m1.ystart;
    
   // initializing my stack with the xstart ystart coordinates
  initialize(&Top, m1.xstart, m1.ystart);
    
//   mark the blocked positions in the maze with *'s
  while (fscanf (src, "%d %d", &xpos, &ypos) != EOF)
    {
        if (xpos == m1.xstart && ypos == m1.ystart) {
              fprintf(stderr,"Invalid: attempting to block starting position %d %d\n", xpos, ypos);
        }
        else if (xpos == m1.xend && ypos == m1.yend) {
              fprintf(stderr, "Invalid: attempting to block ending position %d %d\n", xpos, ypos);
        }
        else {
          //  printf("attempting block the start position %d %d\n", xpos, ypos);
            m1.arr[xpos][ypos] = '*';
            m1.visited[xpos][ypos] = '1';
        }
    }
    m1.visited[m1.xstart][m1.ystart] = '1';  // mark start position as VISITED
    
  /* print out the initial maze */
  for (i = 0; i < m1.xsize+2; i++)
    {
     for (j = 0; j < m1.ysize+2; j++)
       printf ("%c", m1.arr[i][j]);
     printf("\n");
    }
    
    // DFS algorithm start
    while(!checkIfEmpty(Top)) {
        
        if (Top->xOfElement == m1.xend && Top && Top->yOfElement == m1.yend) {
            printf("Solution has found!!\n");
            break;
        }
        // Right check
        if(m1.visited[xcur][ycur+1] == '0') {
            ycur++;
            push(&Top, xcur, ycur);
            m1.visited[xcur][ycur] = '1';
        }
        // Down check
        else if(m1.visited[xcur+1][ycur] == '0') {
            xcur++;
            push(&Top, xcur, ycur);
            m1.visited[xcur][ycur] = '1';
            
        }
        // Left check
        else if(m1.visited[xcur][ycur-1] == '0') {
            ycur--;
            push(&Top, xcur, ycur);
            m1.visited[xcur][ycur] = '1';
        }
        //Up check
        else if(m1.visited[xcur-1][ycur] == '0') {
            xcur--;
            push(&Top, xcur, ycur);
            m1.visited[xcur][ycur] = '1';
        }
        // if no move then pop till find the possible move
        else {
            pop(&Top);
            if(checkIfEmpty(Top)){
                break;
            }
            xcur = Top->xOfElement;
            ycur = Top->yOfElement;
        }
    }
    
    //  Printing that Maze doesnt have a solution
    if(checkIfEmpty(Top)) {
        printf("\nThe Maze has no solution\n");
        exit(-1);
    }
    
    // Printing the solutions reversely
    printf("\n");
    stack *temp = Top;
    reverseDisplay(temp);
    printf("\n");
    
    int h;
    for(h = 0; h < m1.xsize + 2; h++) {
        free(m1.arr[h]);
        free(m1.visited[h]);
    }
    free(m1.arr);
    free(m1.visited);
    
    free(Top);
    return 0;
}

