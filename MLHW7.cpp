// MLHW7.cpp : 定義主控台應用程式的進入點。
//

#include "stdafx.h"
#include <iostream>
#include<fstream>
#include<math.h>


using namespace std;

void BubbleSort(double list[], int size);
void DecisionTree(double data[][3], double datasize, int number);
double branch[100][6];
int branchindex = 0;

int _tmain(int argc, _TCHAR* argv[])
{
	char theline [100] ;
	
	double data[100][3];	// train data with size 100
	double test[1000][3];	// test data with size 1000

	//read file
	fstream fin;
	fin.open("train.txt",ios::in);

	//分割字串存入二維陣列
	char *token = NULL;
	char *next_token = NULL;
	char seps[]   = " ,\t\n";
	int a=0;
	while(fin.getline(theline,sizeof(theline),'\n')){
		int b = 0;
		token = strtok_s( theline, seps, &next_token);
		while (token != NULL){
			data[a][b] = atof(token);
            token = strtok_s( NULL, seps, &next_token);
			b++;
		}
		a++;
	}
	DecisionTree(data,100,1); // 1 = root
	for(int i=0; i<branchindex; i++){
		for(int j=0; j<4; j++){
			cout<<branch[i][j]<<" ";
		}
		cout<<""<<endl;
	}
	cout<<branchindex<<endl;

	
	system("pause");
	return 0;
}

void BubbleSort(double list[],int size){
	for(int i=0; i<size; i++){
		for(int j=0; j<size-i-1; j++){
			if(list[j]>list[j+1]){
				double temp = list[j];
				list[j] = list[j+1];
				list[j+1] = temp;
			}
		}
	}
}

void DecisionTree(double data[][3], double datasize, int number){
	
	// terminal
	int terminal = 1;	// 0:end 1:continue
	double y = data[0][2];
	double coumt = 0;
	for(int i=0; i<datasize; i++){
		if(data[i][2]==y){
			coumt++;
		}
	}
	if(coumt==datasize){
		terminal = 0;	// include datasize==1
	}
	if(terminal==0){
	}else{
		double x[2][100] ;
		for(int i=0; i<2; i++){
			for(int j=0; j<datasize; j++){
				x[i][j] = data[j][i];
			}
			for(int j=datasize; j<100; j++){
				x[i][j] = 10000;			//sort to the end if datasize<100
			}
		}

		double best[5]={0,0,10000,0,0};  // 0:xi  1:theda  2:gini index

		// learn branching criteria
		for(int xi=0; xi<2; xi++){
			BubbleSort(x[xi],datasize);	// first, sort
			
			for(int i=0; i<datasize-1; i++){
				// set theda
				double median = (double)(x[xi][i]+x[xi][i+1])/(double)2;

				// calculate impurity of D1, D2
				double D1size = 0;
				double D2size = 0;
				double D1right = 0;
				double D1wrong = 0;
				double D1impurity = 0;
				double D2right = 0;
				double D2wrong = 0;
				double D2impurity = 0;
				for(int j=0; j<datasize; j++){
					if(data[j][xi]<median){
						D1size++;
						if(data[j][2]==1){
							D1right++;
						}else{
							D1wrong++;
						}
					}else{
						D2size++;
						if(data[j][2]==1){
							D2right++;
						}else{
							D2wrong++;
						}
					}
				}	// end j
				
				D1right = D1right/D1size;
				D1wrong = D1wrong/D1size;
				D2right = D2right/D2size;
				D2wrong = D2wrong/D2size;
				D1impurity = 1 - (D1right*D1right)- (D1wrong*D1wrong);
				D2impurity = 1 - (D2right*D2right) - (D2wrong*D2wrong);

				// calculate b(x)
				double bx = D1size*D1impurity + D2size*D2impurity;
				if(bx<best[2]){
					best[0] = xi;
					best[1] = median;
					best[2] = bx;
					best[3] = D1size;
					best[4] = D2size;
				}
			}
		}	// end learn branching criteria, save the criteria in best

		// save the branch
		for(int i=0; i<3; i++){
			branch[branchindex][i] = best[i];
		}
		branch[branchindex][3] = number;
		branchindex++;
		branch[branchindex][4] = best[3];
		branch[branchindex][5] = best[4];

		// split data to two parts
		double tree1[100][3];
		double tree2[100][3];
		// initial
		for(int i=0; i<100; i++){
			for(int j=0; j<3; j++){
				tree1[i][j] = 0;
				tree2[i][j] = 0;
			}
		}
		double tree1index = 0;
		double tree2index = 0;
		for(int i=0; i<datasize; i++){
			int xi = best[0];
			if(data[i][xi]<best[1]){
				int index = tree1index;
				for(int j=0; j<3; j++){
					tree1[index][j] = data[i][j];
				}
				tree1index++;
			}else{
				int index = tree2index;
				for(int j=0; j<3; j++){
					tree2[index][j] = data[i][j];
				}
				tree2index++;
			}
		}
		// build subtree
		if(tree1index==0){

		}else{
			int numberleft = number * 2 ;
			DecisionTree(tree1, tree1index, numberleft);
		}
		if(tree2index==0){

		}else{
			int numberright = number * 2 + 1;
			DecisionTree(tree2, tree2index, numberright);
		}
	}
}