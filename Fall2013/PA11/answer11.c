
#include "pa11.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
            IMPORTANT INFORMATION
  
This assignment comes with the following files:
+ "README", please read all of this before asking questions.
+ "pa11.c", do not edit this file. (See note below.)
+ "pa11.h", do not edit this header file.
+ "answer11.c", the file you must SUBMIT at the assignment deadline.
+ "tester", a binary (encrypted) file that runs tests on a compiled 
  executable.

You are responsible for testing your code as you write it. The tester
binary will run some tests; however, it is specifically designed to be
too slow to be executed in an edit-compile-test loop. You must set up
this loop yourself. This is EXTREMELY important if you want to learn
how to code faster and more accurately, which is why we do this.

TO TEST YOU CODE

You should set up your own "main(...)" function, in which you create
data an manipulate it with specific functions. For example, you can
run the "isValidState(...)" function as follows:

int main(int argc, char * * argv)
{
   isValidState("ABCDE");
   return 0;
}

 */


int charcmp(const void * a, const void * b)
{
    return *((char*)a) - *((char*)b);
}

/** 
 * Return TRUE iff 'state' is a valid puzzle state.
 *
 * To do this, you have to check:
 * strlen(state) == 16
 * That 'state' only contains characters from "123456789ABCDEF-"
 * That 'state' contains no repeated characters
 *
 * The easiest way to do this is to:
 * (1) Copy 'state' into a buffer that you create
 * (2) Sort the characters in your buffer. (Use qsort.)
 * (3) Check that buffer is equal to "-123456789ABCDEF"
 */
int isValidState(const char * state)
{
    int len = strlen(state);
    if(len != 16) return FALSE;
    char buffer[17];
    strcpy(buffer, state);
    qsort(buffer, len, sizeof(char), charcmp);
    const char * valid = "-123456789ABCDEF";
    if(strcmp(valid, buffer) == 0)
	return TRUE;
    return FALSE;
}

/** 
 * Return TRUE iff all characters in 'moves' are in "RLUD"
 *
 * To do this you must iterate over the string, and check that
 * each character is one of { 'R', 'L', 'U', 'D' }
 */ 
int isValidMoveList(const char * moves)
{
    int len = strlen(moves);
    int i;
    for(i = 0; i <  len; ++i)
	if(moves[i] != 'R' && moves[i] != 'L' && 
	   moves[i] != 'U' && moves[i] != 'D')
	    return FALSE;
    return TRUE;
}

/**
 * Prints the puzzle-state as a 2d matrix. 
 *
 * This function is for debugging purposes only. It is strongly 
 * recommended that you write it; however, it is will not be graded.
 */
void printPuzzle(const char * state)
{
    int row, col;
    for(row = 0; row < SIDELENGTH; ++row) {
	for(col = 0; col < SIDELENGTH; ++col) {
	    printf(" %c", state[row*SIDELENGTH + col]);
	}
	printf("\n");
    }
}


/**
 * Apply move 'm' to puzzle-state 'state'. Return TRUE iff success.
 *
 * You can assume that 'm' is one of { 'U', 'D', 'L', 'R' }, and 
 * that 'state' is a valid state, which means it contains a '-' 
 * character.
 *
 * The simplest way to write this function is as follows:
 * (1) Find the row and column of the space character '-'
 *     (1.a) Search 'state' for the 'position' of the '-' character
 *     (1.b) The 'row' will be 'position / SIDELENGTH'
 *     (1.c) The 'col' will be 'position % SIDELENGTH'
 * (2) Look at the move 'm', and calculate the _new_ row and col
 *     (2.a) If 'm' == 'U', then 'new_row = row-1' and 'new_col = col'
 *     etc.
 * (3) Check that 'new_row' and 'new_col' are in bounds. They must
 *     be >= 0 and < SIDE_LENGTH. If they are _not_ in bounds, then
 *     return FALSE
 * (4) Calculate the 'target_position' for the '-' character. It will
 *     be 'new_row * SIDELENGTH + new_col'
 * (5) Swap the characters at 'position' and 'target_position'
 */
int move(char * state, char m)
{
    int dx = 0;
    int dy = 0;
    switch(m) {
    case 'U': dy = -1; break;
    case 'D': dy = 1; break;
    case 'L': dx = -1; break;
    case 'R': dx = 1; break;
    default:
	fprintf(stderr, "Invalid move '%c'!\n", m);
	return FALSE;
    }
    char * strpos = strstr(state, "-");
    if(strpos == NULL) {
	fprintf(stderr, "Invalid state '%s'!\n", state);
	return FALSE;
    }
    int pos = strpos - state;
    int row = pos / SIDELENGTH;
    int col = pos % SIDELENGTH;
    if(row + dy < 0 || row + dy >= SIDELENGTH)
	return FALSE;
    if(col + dx < 0 || col + dx >= SIDELENGTH)
	return FALSE;

    // Now we know we have a valid move...
    int target_pos = (row+dy)*SIDELENGTH + (col+dx);

    // So swap 'pos' with 'target_pos'
    state[pos] = state[target_pos];
    state[target_pos] = '-';

    return TRUE;
}

/**
 * Apply the moves in 'movelist' to state. 
 *
 * If the sequence is invalid, then print "I" on a single line.
 * Otherwise, print the new state after moves have been applied.
 *
 * To do this, you need:
 * (1) A for-loop to iterate over characters 'm' in 'movelist'
 *     (1.a) Call move(state, m)
 *     (1.b) If 1.a fails, then print "I" on a single line
 * (2) If you never failed in (1), then print the final state on a 
 *     single line.
 */
void processMoveList(char * state, const char * movelist)
{
#ifdef MYDEBUG
    printf("\nApply sequence '%s' to initial state:\n", movelist);
    printPuzzle(state);
#endif

    int valid = TRUE;
    int ind = 0;
    int movelist_len = strlen(movelist);
    for(ind = 0; ind < movelist_len && valid; ++ind) {
	if(!move(state, movelist[ind])) {
	    valid = FALSE;
	} else {
#ifdef MYDEBUG
	    printf("\nApply move '%c':\n", movelist[ind]);
	    printPuzzle(state);
#endif
	}
    }
	    
    if(!valid) {
	printf("I\n");
    } else {
#ifdef MYDEBUG
	printf("\nFinal state: \n");
	printPuzzle(state);
#else
	printf("%s\n", state);
#endif
    }
}

/**
 * Initialise a new MoveTree
 */
MoveTree * MoveTree_create(const char * state, const char * moves)
{
    MoveTree * node = malloc(sizeof(MoveTree));
    node -> state = strdup(state);
    node -> moves = strdup(moves);
    node -> left = NULL;
    node -> right = NULL;
    return node;
}

/**
 * Destroy a tree -- must be safe to call MoveTree_destroy(NULL)
 */
void MoveTree_destroy(MoveTree * node)
{
    if(node == NULL)
	return;
    MoveTree_destroy(node -> left);
    MoveTree_destroy(node -> right);
    free(node -> state);
    free(node -> moves);
    free(node);
}

/**
 * Insert a new node into a tree, keeping in mind that:
 * (1) We do not allow duplicate states.
 * (2) If we attempt to insert a duplicate state, then we keep the
 *     node with the shortest move sequence.
 */
MoveTree * MoveTree_insert(MoveTree * node, const char * state,
			   const char * moves)
{
    if(node == NULL) {
#ifdef MYTEST
	if(strcmp(state, FINAL_STATE) == 0) {
	    printf("FINAL FOUND\n");
	}
#endif
	return MoveTree_create(state, moves);
    }
	
    int cmp = strcmp(state, node -> state);
    
    if(cmp < 0) 
	node -> left = MoveTree_insert(node -> left, state, moves);
    else if(cmp > 0)
	node -> right = MoveTree_insert(node -> right, state, moves);
    else if(strlen(moves) < strlen(node -> moves)) 
	strcpy(node -> moves, moves); // overwrite with shorter seq
	
    return node;
}

/**
 * Search the tree (rooted at 'node') for a node whose
 * state is 'state'
 */
MoveTree * MoveTree_find(MoveTree * node, const char * state)
{
    if(node == NULL)
	return NULL;
    int cmp = strcmp(node -> state, state);
    if(cmp < 0)
	return MoveTree_find(node -> right, state);
    else if(cmp > 0)
	return MoveTree_find(node -> left, state);
    
    return node; // We found it!
}

/**
 * Print the moves in a move-tree.
 * This function is supplied to you.
 */
void MoveTree_print(MoveTree * node)
{
    if(node == NULL)
	return;
    printf("%s\n", node -> state);
    MoveTree_print(node -> left);
    MoveTree_print(node -> right);
}

void generateAllHelper(MoveTree * root, int n_moves, 
		       const char * state, 
		       char * movelist, int ind)
{    
    const char possibleMoves[] = { 'U', 'D', 'L', 'R' };
    int numPossibleMoves = sizeof(possibleMoves) / sizeof(char);
    if(ind == n_moves) {
	// Base case
	return; // nothing to do
    } 

    // Recursive case
    int i;
    for(i = 0; i < numPossibleMoves; ++i) {
	char m = possibleMoves[i];
	char * dup_state = strdup(state);
	if(move(dup_state, m)) {
	    movelist[ind] = m;
	    movelist[ind+1] = '\0'; // Terminates the movelist
	    MoveTree_insert(root, dup_state, movelist);
	    generateAllHelper(root, n_moves, dup_state, movelist, 
			      ind + 1);
	} else {
	    // Move unimportant
	}
	free(dup_state);
    }
}

/**
 * Generate all possible states from 'state' after 0 to 'n_moves'.
 * Returns a 'MoveTree' which contains all of the unique states
 * reachable from 'state' after '0..n_moves' (inclusive). If the same
 * state can be reached by different move sequences, the prefer the
 * shorter of the two sequences.
 * 
 * To write this function you need to create a list of unique states.
 * To do this, you must store the states in a tree, so that when you
 * insert a new state, you can _quickly_ check if it already exists.
 *
 * You will need to create a 'helper' function. When I wrote this code
 * I used the function signature:
 * 
 * void generateAllHelper(MoveTree * root, // Root of the tree
 *         int n_moves,        // maximum length of a movelist
 *         const char * state, // the current state of the puzzle
 * 	   char * movelist,    // the current movelist
 *         int ind)            // the next index to write in movelist
 *
 * This recursive helper function works as follows:
 * (1) The base case: ind == n_moves. Do _nothing_ and return
 * (2) The recursive case:
 *    (2.a) Iterate over the possible moves { 'U', 'D', 'L', 'R'}
 *        (2.a.i) Duplicate 'state' and store it in 'dup_state'
 *                You do this because you don't want to corrupt 
 *                'state' for any other move sequences.
 *        (2.a.ii) Try to apply move 'm' to 'dup_state. If it doesn't
 *                 work, then do nothing and free 'dup_state'
 *        (2.a.iii) If the move was applied, then insert 'dup_state'
 *                  and 'movelist' into the tree 'root'. Be careful...
 *                  MoveTree_insert(...) expects 'movelist' to be a 
 *                  valid c-string, so you _must_ make sure that the
 *                  null-byte is correctly set.
 *        (2.a.iv) Recursively call generateAllHelper(...) with the
 *                 duplicated state, mutated movelist, and updated ind
 *        (2.a.v) Free 'dup_state'
 *
 * Once you have your helper function written, then you can:
 * (1) Create a 'movelist' buffer that can hold a string of length 
 *     n_moves. (Don't forget the null byte.)
 * (2) Create a MoveTree, with 'state' and an empty move list.
 * (3) Call your helper function.
 * (4) Return the tree.
 *
 * This is the most complex function to write... make sure you break
 * it down, and TEST EACH PART.
 */
MoveTree * generateAll(char * state, int n_moves)
{ 
    MoveTree * tree = MoveTree_create(state, "");
    char * movelist = malloc((n_moves+1) * sizeof(char));
    movelist[n_moves] = '\0';
    generateAllHelper(tree, n_moves, state, movelist, 0);
    free(movelist);
    return tree;
}

/**
 * To solve the puzzle:
 * (1) Generate all the possible states up to MAX_SEARCH_DEPTH
 * (2) Search the possibilities for FINAL_STATE
 * (3) If FINAL_STATE isn't found, return NULL
 * (4) If FINAL_STATE is found, then return a copy of the moves.
 */
char * solve(char * state)
{
    MoveTree * tree = generateAll(state, MAX_SEARCH_DEPTH);
    MoveTree * answer = MoveTree_find(tree, FINAL_STATE);
    char * movelist = NULL;
    if(answer != NULL)
	movelist = strdup(answer -> moves);
    MoveTree_destroy(tree);
    return movelist;
}
