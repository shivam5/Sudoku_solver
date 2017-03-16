#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define SIZE 9
#define sqrtSIZE 3

using namespace std;

int main(int argc, char *argv[]){

	if (argc!=3){
		printf("The syntax is programname <input file> <output file>\n");
	}
	else{

		system ("cd MiniSat_v1.14 && make");

		ifstream fin;
		fin.open(argv[1]);

		ofstream fout;
		fout.open(argv[2]);

		string st;

		int no_variables = SIZE*SIZE*SIZE;
		int no_clauses = 0;

		while ( fin>> st){

			no_clauses = 0;

			ofstream fout1;
			fout1.open("temp.txt");

			// Cell constraint taking or of all values (atleast one from 1-9 is there in a cell)
			for (int row=1; row<SIZE+1; row++){
				for (int col=1; col<SIZE+1; col++){
					for (int val=1; val<SIZE+1; val++){
						fout1<<row<<col<<val<<" ";
					}
					fout1<<"0\n";
					no_clauses++;
				}
			}

			// Row constraints
			for (int row=1; row<SIZE+1; row++){
				for (int col1=1; col1<SIZE+1; col1++){
					for (int col2=col1+1; col2<SIZE+1; col2++){
						for (int val=1; val<SIZE+1; val++){
							fout1<<"-"<<row<<col1<<val<<" "<<"-"<<row<<col2<<val<<" 0\n";
							no_clauses++;							
						}
					}
				}
			}


			// Column constraints
			for (int col=1; col<SIZE+1; col++){
				for (int row1=1; row1<SIZE+1; row1++){
					for (int row2=row1+1; row2<SIZE+1; row2++){
						for (int val=1; val<SIZE+1; val++){
							fout1<<"-"<<row1<<col<<val<<" "<<"-"<<row2<<col<<val<<" 0\n";
							no_clauses++;							
						}
					}
				}
			}

			//Box constraints

			for (int i=0; i<sqrtSIZE; i++){
				for (int j=0; j<sqrtSIZE; j++){
					int x = i*sqrtSIZE;
					int y = j*sqrtSIZE;
					int row1,col1,row2,col2;

					row1=x;
					col1=y;

					for (int a=1; a<sqrtSIZE; a++){
						for (int b=1; b<sqrtSIZE; b++){
							row2 = x+a;
							col2 = y+b;
							for (int val=1; val<SIZE+1; val++){
								fout1<<"-"<<row1+1<<col1+1<<val<<" "<<"-"<<row2+1<<col2+1<<val<<" 0\n";
								no_clauses++;															
							}
						}
					}

					row1 = x;
					col1 = y+1;

					row2 = x+1;
					col2 = y;
					for (int val=1; val<SIZE+1; val++){
						fout1<<"-"<<row1+1<<col1+1<<val<<" "<<"-"<<row2+1<<col2+1<<val<<" 0\n";
						no_clauses++;															
					}

					row2 = x+1;
					col2 = y+2;
					for (int val=1; val<SIZE+1; val++){
						fout1<<"-"<<row1+1<<col1+1<<val<<" "<<"-"<<row2+1<<col2+1<<val<<" 0\n";
						no_clauses++;															
					}

					row2 = x+2;
					col2 = y;
					for (int val=1; val<SIZE+1; val++){
						fout1<<"-"<<row1+1<<col1+1<<val<<" "<<"-"<<row2+1<<col2+1<<val<<" 0\n";
						no_clauses++;															
					}

					row2 = x+2;
					col2 = y+2;
					for (int val=1; val<SIZE+1; val++){
						fout1<<"-"<<row1+1<<col1+1<<val<<" "<<"-"<<row2+1<<col2+1<<val<<" 0\n";
						no_clauses++;															
					}


					row1 = x;
					col1 = y+2;

					row2 = x+1;
					col2 = y;
					for (int val=1; val<SIZE+1; val++){
						fout1<<"-"<<row1+1<<col1+1<<val<<" "<<"-"<<row2+1<<col2+1<<val<<" 0\n";
						no_clauses++;															
					}

					row2 = x+1;
					col2 = y+1;
					for (int val=1; val<SIZE+1; val++){
						fout1<<"-"<<row1+1<<col1+1<<val<<" "<<"-"<<row2+1<<col2+1<<val<<" 0\n";
						no_clauses++;															
					}

					row2 = x+2;
					col2 = y;
					for (int val=1; val<SIZE+1; val++){
						fout1<<"-"<<row1+1<<col1+1<<val<<" "<<"-"<<row2+1<<col2+1<<val<<" 0\n";
						no_clauses++;															
					}

					row2 = x+2;
					col2 = y+1;
					for (int val=1; val<SIZE+1; val++){
						fout1<<"-"<<row1+1<<col1+1<<val<<" "<<"-"<<row2+1<<col2+1<<val<<" 0\n";
						no_clauses++;															
					}


					row1 = x+1;
					col1 = y;

					row2 = x+2;
					col2 = y+1;
					for (int val=1; val<SIZE+1; val++){
						fout1<<"-"<<row1+1<<col1+1<<val<<" "<<"-"<<row2+1<<col2+1<<val<<" 0\n";
						no_clauses++;															
					}

					row2 = x+2;
					col2 = y+2;
					for (int val=1; val<SIZE+1; val++){
						fout1<<"-"<<row1+1<<col1+1<<val<<" "<<"-"<<row2+1<<col2+1<<val<<" 0\n";
						no_clauses++;															
					}


					row1 = x+1;
					col1 = y+1;

					row2 = x+2;
					col2 = y;
					for (int val=1; val<SIZE+1; val++){
						fout1<<"-"<<row1+1<<col1+1<<val<<" "<<"-"<<row2+1<<col2+1<<val<<" 0\n";
						no_clauses++;															
					}

					row2 = x+2;
					col2 = y+2;
					for (int val=1; val<SIZE+1; val++){
						fout1<<"-"<<row1+1<<col1+1<<val<<" "<<"-"<<row2+1<<col2+1<<val<<" 0\n";
						no_clauses++;															
					}


					row1 = x+1;
					col1 = y+2;

					row2 = x+2;
					col2 = y;
					for (int val=1; val<SIZE+1; val++){
						fout1<<"-"<<row1+1<<col1+1<<val<<" "<<"-"<<row2+1<<col2+1<<val<<" 0\n";
						no_clauses++;															
					}

					row2 = x+2;
					col2 = y+1;
					for (int val=1; val<SIZE+1; val++){
						fout1<<"-"<<row1+1<<col1+1<<val<<" "<<"-"<<row2+1<<col2+1<<val<<" 0\n";
						no_clauses++;															
					}



				}
			}

			// Adding unary constraints
			for (int i=1; i<SIZE+1 ; i++){
				for(int j=1; j<SIZE+1 ; j++){
					int x = (i-1)*SIZE+(j-1);
					if (st[x] != '.'){
						int val = st[x] - '0';
						fout1<<i<<j<<val<<" 0\n";
						no_clauses++;
					}
				}
			}

			fout1.close();

			ofstream fout2;
			fout2.open("cnf.txt");

			ifstream fin2;
			fin2.open("temp.txt");

			fout2<<"p cnf "<<no_variables<<" "<<no_clauses<<endl;

			while (fin2){
				getline(fin2, st);
				fout2<<st<<endl;
			}

			fout2.close();
			fin2.close();

			// Call minisat from cnf.txt
			system ("./MiniSat_v1.14/minisat cnf.txt satsolution.txt");


			// Parse output from minisat
			ifstream fin3;
			fin3.open("satsolution.txt");

			fin3 >> st;
			int x;

			if (st == "UNSAT"){
				fout<<"Unsolvable sudoku\n";
			}
			else {

				while (fin3 >> x){
					if (abs(x)<111)
						continue;
					if (x>0){
						int val = x%10;
						fout<<val;
					}
				}

			}

			//Next sudoku will be read
			fout<<endl;
		}

		remove("temp.txt");
		remove("cnf.txt");
		remove("satsolution.txt");

		fout.close();
		fin.close();

	}



	return 0;
}