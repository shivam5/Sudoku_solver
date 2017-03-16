#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <vector>
#include <stack>
#include <algorithm>
#include <queue>
#include <time.h>
#include <unistd.h>
#include <ios>

#define SIZE 9
#define sqrtSIZE 3
#define UNASSIGNED 0
#define NO_HEURISTIC 0
#define MRV 1
#define LCV 2
#define MACH 3

using namespace std;

int BACK = 0;

vector <int> DOMAINS[SIZE*SIZE];

struct arcs {
	int row1;
	int col1;
	int row2;
	int col2;
};

// Finds an unassigned position in sudoku and returns true, otherwise returns false
bool find_unassigned_variable (int sudoku[SIZE][SIZE], int &row, int &col){
	int i,j;

	for (int j=col; j<SIZE; j++){
		if (sudoku[row][j] == UNASSIGNED){
			col = j;
			return true;
		}
	}

	for (int i=row+1; i<SIZE; i++){
		for(int j=0; j<SIZE; j++){
			if (sudoku[i][j] == UNASSIGNED){
				row = i;
				col = j;
				return true;
			}

		}
	}
	return false;

}

int remaining_values(int sudoku[SIZE][SIZE], int row, int col){
	int x = 0, boxrow, boxcol;
	int arr[SIZE];

	for (int k=0; k<SIZE; k++)
		arr[k]=0;
			
	for(int k=0; k<SIZE; k++){
		if (sudoku[row][k] != UNASSIGNED)
			arr[sudoku[row][k] - 1] = 1;
		if (sudoku[k][col] != UNASSIGNED)
			arr[sudoku[k][col] - 1] = 1;
	}

	boxrow = row - (row%sqrtSIZE);
	boxcol = col - (col%sqrtSIZE);

	for (int k=boxrow; k<(boxrow+sqrtSIZE); k++){
		for (int l=boxcol; l<(boxcol+sqrtSIZE); l++){	
			if (sudoku[k][l] != UNASSIGNED)
				arr[sudoku[k][l] - 1] = 1;
		}			
	}

	for (int k=0; k<SIZE; k++){
		if (arr[k]==0)
			x++;
	}

	return x;
}


int find_unassigned_variable_MRV (int sudoku[SIZE][SIZE], int &row, int &col){
	int min=SIZE+1;
	int x;

	for (int i=0; i<SIZE; i++){
		for(int j=0; j<SIZE; j++){
			if (sudoku[i][j] != UNASSIGNED)
				continue;

			x = remaining_values(sudoku, i, j);
			
			if (x <= min){
				min = x;
				row = i;
				col = j;
			}
			if (min == 0)
				return 0;

		}
	}

	if (min == SIZE + 1)
		return -1;
	else
		return 1;
}

bool row_constraint (int sudoku[SIZE][SIZE], int row, int col, int value){
	for(int i=0; i<SIZE; i++){
		if (i == col)
			continue;
		if (sudoku[row][i] == value)
			return false;
	}

	return true;
}

bool column_constraint (int sudoku[SIZE][SIZE], int row, int col, int value){
	for(int i=0; i<SIZE; i++){
		if (i == row)
			continue;
		if (sudoku[i][col] == value)
			return false;
	}

	return true;
}

bool box_constraint (int sudoku[SIZE][SIZE], int row, int col, int value){
	int boxrow = row - (row%sqrtSIZE);
	int boxcol = col - (col%sqrtSIZE);

	for (int i=boxrow; i<(boxrow+sqrtSIZE); i++){
		for (int j=boxcol; j<(boxcol+sqrtSIZE); j++){
			if (i == row && j==col)
				continue;
			if (sudoku[i][j] == value)
				return false;
		}			
	}

	return true;
}

// Checks whether the current assignment is consistent with all the constraints, then returns true, otherwise false
bool is_assignment_consistent (int sudoku[SIZE][SIZE], int row, int col, int value){
	if (row_constraint(sudoku, row, col, value) && column_constraint(sudoku, row, col, value) && box_constraint(sudoku, row, col, value) )
		return true;

	return false;
}

void swap_values (int row, int col, int index1, int index2){
	int index = row*SIZE+col;
	int temp = DOMAINS[index][index1];
	DOMAINS[index][index1] = DOMAINS[index][index2];
	DOMAINS[index][index2] = temp;
}


void remove_from_vector (int row, int col, int x){
	int i=row*SIZE+col;
	DOMAINS[i].erase(remove(DOMAINS[i].begin(), DOMAINS[i].end(), x), DOMAINS[i].end());
}

bool check_consistent_in_domain_j(int sudoku[SIZE][SIZE], int indexi, int indexj, int x){
	int row = indexi/SIZE;
	int col = indexi%SIZE;
	int temp = sudoku[row][col];

	sudoku[row][col] = x;

	for (int i=0; i<DOMAINS[indexj].size(); i++){
		if (is_assignment_consistent(sudoku, indexj/SIZE, indexj%SIZE, DOMAINS[indexj][i])){
			sudoku[row][col] = temp;
			return true;
		}
	}

	sudoku[row][col] = temp;
	return false;
}

bool REVISE (int sudoku[SIZE][SIZE], arcs x){
	bool revised = false;
	int row1 = x.row1;
	int col1 = x.col1;
	int indexi = x.row1*SIZE+x.col1 ;
	int indexj = x.row2*SIZE+x.col2 ;

	for (int i=0; i<DOMAINS[indexi].size(); i++){
		int x = DOMAINS[indexi][i];

		if(!check_consistent_in_domain_j(sudoku, indexi, indexj, x)){
			remove_from_vector(row1, col1, x);
			revised = true;
		}
	}

	return revised;
}

bool MAC (int sudoku[SIZE][SIZE], int row, int col, int val){
	DOMAINS[row*SIZE+col].clear();
	DOMAINS[row*SIZE+col].push_back(val);

	// Make a queue
	queue <arcs> q;
	// Add all arcs (Xi, Xj) where Xj are neighbours of Xi.
	arcs x;
	x.row2 = row;
	x.col2 = col;

	for(int k=0; k<SIZE; k++){
		if (sudoku[row][k] == UNASSIGNED && k!=col){
			x.row1 = row;
			x.col1 = k;
			q.push(x);
		}
		if (sudoku[k][col] == UNASSIGNED && k!=row){
			x.row1 = k;
			x.col1 = col;
			q.push(x);
		}

	}

	int boxrow = row - (row%sqrtSIZE);
	int boxcol = col - (col%sqrtSIZE);

	for (int k=boxrow; k<(boxrow+sqrtSIZE); k++){
		for (int l=boxcol; l<(boxcol+sqrtSIZE); l++){	
			if (sudoku[k][l] == UNASSIGNED && k!=row && l!=col ){
				x.row1 = k;
				x.col1 = l;
				q.push(x);
			}
		}			
	}

	//Run the main algo
	while (!q.empty()){
		x = q.front();
		q.pop();
		if ( REVISE(sudoku, x) ){

			row = x.row1;
			col = x.col1;

			if (DOMAINS[row*SIZE+col].size() == 0){
				return false;
			}


			arcs y;
			y.row2 = row;
			y.col2 = col;

			for(int k=0; k<SIZE; k++){

				if (sudoku[row][k] == UNASSIGNED && k!=col ){
					if (row != x.row2 || k != x.col2){
						y.row1 = row;
						y.col1 = k;
						q.push(y);
					}
				}

				if (sudoku[k][col] == UNASSIGNED && k!=row){
					if (k != x.row2 || col != x.col2){
						y.row1 = k;
						y.col1 = col;
						q.push(y);
					}
				}

			}

			int boxrow = row - (row%sqrtSIZE);
			int boxcol = col - (col%sqrtSIZE);

			for (int k=boxrow; k<(boxrow+sqrtSIZE); k++){
				for (int l=boxcol; l<(boxcol+sqrtSIZE); l++){	

					if (sudoku[k][l] == UNASSIGNED && (k!=row && l!=col) ){
						if (k != x.row2 || l != x.col2){
							y.row1 = k;
							y.col1 = l;
							q.push(y);
						}
					}
				}			
			}



		}

	}
	return true;
}

void copy_from_domains(vector <int> tempdomains[SIZE*SIZE]){
	for (int i=0; i< SIZE*SIZE; i++){
		tempdomains[i].clear();
		for (int j=0; j<DOMAINS[i].size(); j++)
			tempdomains[i].push_back(DOMAINS[i][j]);
	}
}

void copy_to_domains(vector <int> tempdomains[SIZE*SIZE]){
	for (int i=0; i< SIZE*SIZE; i++){
		DOMAINS[i].clear();
		for (int j=0; j<tempdomains[i].size(); j++)
			DOMAINS[i].push_back(tempdomains[i][j]);
	}
}



int find_CV (int sudoku[SIZE][SIZE], int row, int col, int x){
	int total = 0;

	for(int k=0; k<SIZE; k++){
		if (sudoku[row][k] == UNASSIGNED && k!=col)
			if ( is_assignment_consistent(sudoku, row, k, x) )
				total++;
		if (sudoku[k][col] == UNASSIGNED && k!=row)
			if ( is_assignment_consistent(sudoku, k, col, x) )
				total++;
	}

	int boxrow = row - (row%sqrtSIZE);
	int boxcol = col - (col%sqrtSIZE);

	for (int k=boxrow; k<(boxrow+sqrtSIZE); k++){
		for (int l=boxcol; l<(boxcol+sqrtSIZE); l++){	
			if (k==row || l==col)
				continue;

			if (sudoku[k][l] == UNASSIGNED )
			if ( is_assignment_consistent(sudoku, k, l, x) )
				total++;
		}			
	}

	return total;
}

void sort_LCV (int sudoku[SIZE][SIZE], int row, int col){

	int index = row*SIZE+col;
	int n = DOMAINS[index].size();
	int arr[n];

	for (int i=0; i<n ; i++){
		int x = DOMAINS[index][i];
		if (!is_assignment_consistent(sudoku, row, col, x)){

			arr[i] = SIZE*3+1;
		}
		else{
			arr[i] = find_CV(sudoku, row, col, x);
		}
	}

	int min, loc, temp;

    for(int i=0; i<n-1; i++) {
        min = arr[i];
        loc = i;

        for(int j=i+1; j<n; j++){
            if(min>arr[j]){
                min = arr[j];
                loc = j;
            }
        }
 		
 		swap_values (row, col, i, loc);
        temp = arr[i];
        arr[i] = arr[loc];
        arr[loc] = temp;
    }
}

void initialize_domains (int sudoku[SIZE][SIZE]){
	// Erase all the vectors
	for (int i=0; i<SIZE*SIZE; i++)
		DOMAINS[i].clear();

	for (int i=0; i<SIZE; i++){
		for (int j=0; j<SIZE; j++){		

			if (sudoku[i][j] == UNASSIGNED ){
				for (int k=1; k<SIZE+1; k++){
					DOMAINS[i*SIZE+j].push_back(k);
				}
			}
			else{
				DOMAINS[i*SIZE+j].push_back(sudoku[i][j]);
			}

		}
	}		
}


bool start_MAC (int sudoku[SIZE][SIZE]){
	for (int i=0; i<SIZE; i++){
		for (int j=0; j<SIZE; j++){		

			if (sudoku[i][j] != UNASSIGNED ){
				if (!MAC(sudoku, i, j, sudoku[i][j]) )
					return false;				
			}

		}
	}	

	return true;
}


bool BS_MAC (int sudoku[SIZE][SIZE], int row, int col, int algo = 0){

	int x = find_unassigned_variable_MRV(sudoku, row, col);
	if (x == -1)
		return true;
	else if (x == 0)
		return false;

	sort_LCV (sudoku, row, col);

	for (int i=0; i<DOMAINS[row*SIZE+col].size() ; i++){
		int x = DOMAINS[row*SIZE+col][i];
		if (is_assignment_consistent(sudoku, row, col, x)){
			vector <int> tempdomains[SIZE*SIZE];

			copy_from_domains(tempdomains);

			if (!MAC(sudoku, row, col, x) ){
				copy_to_domains(tempdomains);
				continue;
			}

			sudoku[row][col] = x;

			if (BS_MAC(sudoku, row, col))
				return true;

			BACK++;
			copy_to_domains(tempdomains);
			sudoku[row][col] = UNASSIGNED;
		}
	}
	return false;
}


bool BS_II (int sudoku[SIZE][SIZE], int row, int col, int algo = 0){

	int x = find_unassigned_variable_MRV(sudoku, row, col);
	if (x == -1)
		return true;
	else if (x == 0)
		return false;

	sort_LCV (sudoku, row, col);

	for (int i=0; i<DOMAINS[row*SIZE+col].size() ; i++){
		int x = DOMAINS[row*SIZE+col][i];
		if (is_assignment_consistent(sudoku, row, col, x)){

			sudoku[row][col] = x;

			if (BS_II(sudoku, row, col))
				return true;

			BACK++;
			sudoku[row][col] = UNASSIGNED;
		}
	}
	return false;
}

bool BS_I (int sudoku[SIZE][SIZE], int row, int col){

	int x = find_unassigned_variable_MRV(sudoku, row, col);
	if (x == -1)
		return true;
	else if (x == 0)
		return false;


	for (int i=0; i<DOMAINS[row*SIZE+col].size() ; i++){
		int x = DOMAINS[row*SIZE+col][i];
		if (is_assignment_consistent(sudoku, row, col, x)){

			sudoku[row][col] = x;

			if (BS_I(sudoku, row, col))
				return true;

			BACK++;
			sudoku[row][col] = UNASSIGNED;
		}
	}
	return false;
}


bool BS (int sudoku[SIZE][SIZE], int row, int col){

	if (!find_unassigned_variable(sudoku, row, col))
		return true;

	for (int i=0; i<DOMAINS[row*SIZE+col].size() ; i++){
		int x = DOMAINS[row*SIZE+col][i];
		if (is_assignment_consistent(sudoku, row, col, x)){

			sudoku[row][col] = x;

			if (BS(sudoku, row, col))
				return true;

			BACK++;
			sudoku[row][col] = UNASSIGNED;

		}
	}
	return false;
}

// Credits - copied from stack overflow
void process_mem_usage(double& vm_usage, double& resident_set)
{

   vm_usage     = 0.0;
   resident_set = 0.0;

   // 'file' stat seems to give the most reliable results
   //
   ifstream stat_stream("/proc/self/stat",ios_base::in);

   // dummy vars for leading entries in stat that we don't care about
   //
   string pid, comm, state, ppid, pgrp, session, tty_nr;
   string tpgid, flags, minflt, cminflt, majflt, cmajflt;
   string utime, stime, cutime, cstime, priority, nice;
   string O, itrealvalue, starttime;

   // the two fields we want
   //
   unsigned long vsize;
   long rss;

   stat_stream >> pid >> comm >> state >> ppid >> pgrp >> session >> tty_nr
               >> tpgid >> flags >> minflt >> cminflt >> majflt >> cmajflt
               >> utime >> stime >> cutime >> cstime >> priority >> nice
               >> O >> itrealvalue >> starttime >> vsize >> rss; // don't care about the rest

   stat_stream.close();

   long page_size_kb = sysconf(_SC_PAGE_SIZE) / 1024; // in case x86-64 is configured to use 2MB pages
   vm_usage     = vsize / 1024.0;
   resident_set = rss * page_size_kb;
}



int main(int argc, char *argv[]){

	clock_t start_time, current_time;
	start_time = clock();
	
	if (argc!=4){
		printf("The syntax is programname <input file> <output file> <algo> where algo should be a number\n");
	}
	else{

		int algo;

		if (argv[3][0] >= '0' && argv[3][0] <= '9')
			algo = argv[3][0]-'0';
		else
			algo = MACH;

		ifstream fin;
		fin.open(argv[1]);

		ofstream fout;
		fout.open(argv[2]);

		string st;
		int sudoku[SIZE][SIZE];

		int x=0, i, j;

		while ( fin>> st){
			for (i=0; i<SIZE; i++){
				for(j=0; j<SIZE; j++){
					x = i*SIZE+j;
					if (st[x] == '.'){
						sudoku[i][j] = UNASSIGNED;
					}
					else
						sudoku[i][j] = st[x] - '0';
				}
			}

			initialize_domains(sudoku);


			if (algo == NO_HEURISTIC){
				if ( BS(sudoku, 0, 0) ){
					//print_sudoku(sudoku);
					for (int i=0; i<SIZE; i++){
						for (int j=0; j<SIZE; j++){
							fout<<sudoku[i][j];
						}
					}	
					fout<<"\n";
				}
				else{
					fout<<"Unsolvable sudoku\n";
				}
			}

			else if (algo == MRV){
				if ( BS_I(sudoku, 0, 0) ){
					for (int i=0; i<SIZE; i++){
						for (int j=0; j<SIZE; j++){
							fout<<sudoku[i][j];
						}
					}	
					fout<<"\n";
				}
				else{
					fout<<"Unsolvable sudoku\n";
				}				
			}

			else if (algo == LCV){
				if ( BS_II(sudoku, 0, 0) ){
					for (int i=0; i<SIZE; i++){
						for (int j=0; j<SIZE; j++){
							fout<<sudoku[i][j];
						}
					}	
					fout<<"\n";
				}
				else{
					fout<<"Unsolvable sudoku\n";
				}

			}

			else if (algo == MACH) {
				if (!start_MAC(sudoku)){
					fout<<"Unsolvable sudoku\n";
				}

				else{
					if ( BS_MAC(sudoku, 0, 0) ){
						for (int i=0; i<SIZE; i++){
							for (int j=0; j<SIZE; j++){
								fout<<sudoku[i][j];
							}
						}	
						fout<<"\n";
					}
					else{
						fout<<"Unsolvable sudoku\n";
					}
				}
			}


		}
		fout.close();
		fin.close();

	}

	current_time = clock();
	float time = (float) (current_time - start_time)/ CLOCKS_PER_SEC;

	double vm, rss;
   	process_mem_usage(vm, rss);
   	

	cout<<"The time taken is : "<< time<<" seconds.\n";
	cout<<"The number of backtracks are : "<<BACK<<endl;
	cout << "Virtual Memory: " << vm << "; Resident Set Space : " << rss << endl;	
	return 0;
}