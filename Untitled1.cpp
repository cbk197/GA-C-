#include<iostream>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>

typedef struct  listRank{
	int index;
	double cost;
	struct listRank *next;
} listRank;

typedef struct linkedList{
	listRank * tail;
	listRank * head;
}linkedList;
//push into list. 
int _dimension;
double **arrcost;
int count =0;
bool push(linkedList* L , listRank *R){
	listRank *tmp;
	listRank *tmp1 = new listRank;
	tmp = L->head;
	
	if(L->head==NULL){
		
		tmp1->cost = R->cost;
		tmp1->index = R->index;
		tmp1->next = NULL; 
		L->head = tmp1;
		L->tail = tmp1;
		return true;
	}else{
		
		while(tmp != NULL){
			if(tmp->cost < R->cost  ){
				tmp1->next = tmp->next;
				tmp1->cost = tmp->cost;
				tmp1->index = tmp->index;
				tmp->next = tmp1;
				tmp->cost= R->cost;
				tmp->index = R->index;
				if(tmp1->next == NULL){
					L->tail = tmp1;
				}
				return true;
			}
			
			if(tmp->next !=NULL){
				tmp = tmp->next;
			}else{
				tmp1->cost = R->cost;
				tmp1->index = R->index;
				tmp1->next = NULL;
				L->tail->next = tmp1;
				L->tail = tmp1;
				return true;
			}
		}
		
		
		
	}
}

bool push1(linkedList* L , listRank *R){
	listRank *tmp;
	listRank *tmp1 = new listRank;
	tmp = L->head;
	

		while(tmp != NULL){
			if(tmp->cost == R->cost ) {
				free(tmp1);
				return false;
			}
			if(tmp->cost < R->cost  ){
				tmp1->next = tmp->next;
				tmp1->cost = tmp->cost;
				tmp1->index = tmp->index;
				tmp->next = tmp1;
				tmp->cost= R->cost;
				tmp->index = R->index;
				if(tmp1->next == NULL){
					L->tail = tmp1;
				}
				return true;
			}
			
			if(tmp->next !=NULL){
				tmp = tmp->next;
			}else{
				tmp1->cost = R->cost;
				tmp1->index = R->index;
				tmp1->next = NULL;
				L->tail->next = tmp1;
				L->tail = tmp1;
				return true;
			}
		}
		
}

double pow1(double a, int b){
	if(b == 1){
		return a;
	}else{
			if(b % 2 == 1){
				double re = pow1(a,(b-1)/2);
				return re*re*a;
			}else{
				double re = pow1(a,b/2);
				return re*re;
			}
	}
}
// read integer in line and not read '\n'
int readInteger(FILE *f){
	char c=' '; 
	int _Num=0;
	while(c<48 || c >57){
		if(!feof(f)){
			c = fgetc(f);
		}else{
			return _Num;
		}
	
	}
	
	while(c > 47 && c <58){
		_Num = _Num*10 + (int)c-48;
		if(!feof(f)){
			c = fgetc(f);
			if(c == 13 || c == 10){
				fseek(f,-1,SEEK_CUR);
			}	
		}else{
			return _Num;
		}
		
	}
	return _Num;
}

// move pointer file to nextline 
void fNewline(FILE *f){
	char c ; 
	c = fgetc(f);
	while(c !=10 && c !=13){
		c = fgetc(f);
	}
}

// read data from file 
int** readData(){
	int n; 
	int **array;
	FILE *f;
	char fileName[100] = "testdata.txt";
	f = fopen(fileName,"rt");
	if(f==NULL){
		printf("openfile error");
		exit(0);
	}
	char c ;
	int countN=0;
	while(countN <3){
		c =fgetc(f);
		if(c == 10 || c == 13){
			countN++;
		};
	};
	
	n = readInteger(f);
	_dimension = n ; 
	array = new int*[n];
	for(int i = 0 ; i< n ; i++){
		array[i] = new int[2];
	};
	fNewline(f);
	fNewline(f);
	int _temp;
	for(int i=0 ; i< n ; i++ ){
		_temp = readInteger(f);
		array[i][0] = readInteger(f);
		array[i][1] =  readInteger(f);
		fNewline(f);
		if(i == n-1 && feof(f) ){
			printf("\n data file error ");
			exit(0);
		}
	}
	fclose(f);
	return array;
	
	
		
}
//check number existed in array . return true if existed otherwise return false
bool checkExisted(int *arr, int size, int num){
	for(int i =0 ;i< size ; i++){
		if(num == arr[i]) return true;
	}
	return false;
}

// create array random n element 
int *createArr(int n){
	int *arr = new int[n];
	int tmp ;
	
	for(int i =0 ; i < n ; i ++){
		
		tmp = rand() % n;
		while(checkExisted(arr,i,tmp)){
			tmp = rand() % n;
		}
		arr[i] = tmp;
	}
	return arr;
}
// initial population with number of element 
int ** initialPopul(int n ){
	int ** arr = new int*[n];
	for(int i = 0 ; i < n ; i++){
		arr[i] = createArr(_dimension);
	};
	return arr;
}

//cost of element
double getCost(int **arr, int *el){  // arr (arrData) array contain coordinate of node , el is a solution for problem
	double cost = 0;
	double tmp;
	for(int i =0 ; i< _dimension-1 ; i++){
		cost += arrcost[el[i]][el[i+1]];
	}
	cost +=arrcost[el[_dimension-1]][el[0]];
	return cost;
};

//rank population
linkedList* rankPopul(int **arrData,int **arr, int n) { //n : number of element of population
	linkedList *L = new linkedList;
	listRank *R = new listRank;
	L->head = L->tail = NULL;
	double cost;
	for(int i= 0 ; i< n ; i++ ){
		cost = getCost(arrData, arr[i]);
		R->cost = cost;
		R->index = i;
		R->next = NULL;
		push(L,R);
	}
	free(R);
	return L;
}

//crossover 2 point. 
int *crossover(int *par1, int *par2){
	int  index1 ; 
	int index2; 
	
	
	index1 = rand() % (_dimension);
	while(index1 > _dimension -3){
		index1 = rand() % (_dimension);
	}
	index2 = index1 + rand() % (_dimension- index1-1) +1 ;
	int *offs1 = new int[_dimension];
	for(int i=0 ; i < _dimension; i++){
		offs1[i] = -1;
	}
	
	for(int i=index1 ; i < index2; i++){
		offs1[i] = par1[i];
	};
	int tmp = index2;
	int tmp1 = 0;
	while(tmp < _dimension ){
		if(!checkExisted(offs1,_dimension,par2[tmp1])){
				offs1[tmp] = par2[tmp1];
				tmp++;
			
		}
		tmp1++;
	}
	tmp =0;
	while(tmp < index1 ){
		if(!checkExisted(offs1,_dimension,par2[tmp1])){
				offs1[tmp] = par2[tmp1];
				tmp++;
			
		}
		tmp1++;
	}
//	for(int i =0; i< _dimension; i++){
//		if(!checkExisted(offs1,_dimension,par2[i])){
//			if(tmp <_dimension){
//				offs1[tmp] = par2[i];
//				tmp++;
//			}else{
//				tmp =0;
//				offs1[tmp] = par2[i];
//				tmp++;
//			}
//			
//		}
//	};
//	for(int i =0; i<index2; i++){
//		if(!checkExisted(offs1,_dimension,par2[i])){
//			if(tmp<_dimension){
//				offs1[tmp] = par2[i];
//				tmp++;
//			}else{
//				tmp =0;
//				offs1[tmp] = par2[i];
//				tmp++;
//			}
//			
//		}
//	};
	return offs1;
	
};

//create new popuation 
//n : number of offspring need create 
//arr : population parent 
// nPar : number of population parent
//crossover 50% random from 50% parent berter and 50% random all parent 
void createOffs(int n , int **arr, linkedList *L,int** arrData, int nPar){ 
	int** arrOffs = new int*[n];
//	index2 = index1 + 1 + index2 % (_dimension-index1-2);
	int par1, par2;
	listRank *R;
	R = L->head;
//	int tmp = nPar/2;
//	for(int i = 0; i< tmp ; i++){
//		R->next;
//	};
//	int arrBest[nPar- tmp]; // array contain index of 50% parent best
//	for(int i =0; i < nPar-tmp; i++){
//		arrBest[i] = R->index;
//		R = R->next;
//	};
//	for(int i = 0; i< n/2; i++){
//		par1 = rand() % (nPar - tmp);
//		
//		par2 =  rand() % (nPar - tmp);
//		while(par2 == par1){
//			par2 =  rand() % (nPar - tmp);
//		}
//		
//		arrOffs[i] = crossover(arr[arrBest[par1]],arr[arrBest[par2]],index1,index2);
//	}
//
//	tmp = n/2;
	for(int i = 0 ; i< n; i ++){
		par1 = rand() % nPar;
		par2 = rand() % nPar;
		while(par2 == par1){
			par2 =  rand() % nPar ;
		};
		arrOffs[i] = crossover(arr[par1], arr[par2]);
	}
		
	
	for (int i =0 ; i< n ; i++){
		double cost = getCost(arrData,arrOffs[i]);
		R = L->head;
		if(cost < R->cost){
			
			listRank *l;
			l= L->head;
			listRank *l1 = new listRank;
			l1->cost = cost;
			l1->index = l->index;
			l1->next = NULL;
			if(push1(L,l1)){
				free(arr[l->index]);
				arr[l->index] = arrOffs[i];
				L->head = l->next;
				free(l);
			}else{
				free(arrOffs[i]);
			}
		
			free(l1);
		}else{
			free(arrOffs[i]);
		}
		
	}
	free(arrOffs);
	
}


main(){
	srand(time(NULL)); 
	listRank *r;
	int **arrData;
	arrData = readData();
	arrcost = new double*[_dimension];
	for(int  i=0 ; i< _dimension; i++){
		arrcost[i] = new double[_dimension];
	};
	for(int i = 0 ; i < _dimension ; i++){
		for(int j = 0 ; j < _dimension ; j++){
			arrcost[i][j] = 0;
		}
	}
	
	for(int i = 0 ; i < _dimension ; i++){
		for(int j = 0 ; j < _dimension ; j++){
			if(arrcost[j][i]==0){
				arrcost[i][j] = arrcost[j][i] = sqrt(pow(arrData[i][0]- arrData[j][0],2)+pow(arrData[i][1]-arrData[j][1],2));
			}
		}
	}
	int numPopul;
	printf("type number of population :");
	fflush(stdin);
	scanf("%d",&numPopul);
	int **arr;
	arr = initialPopul(numPopul);
	linkedList* L = rankPopul(arrData,arr,numPopul);
	int loop = 0;
	clock_t begin, end; \
	double time_spent = 0.0;
	begin = clock(); 
	while(loop < 1000){
		createOffs(numPopul*7/10,arr,L,arrData,numPopul);
		loop++;
	};
	end = clock();
	time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
	printf("time spent 1000 loop: %lf \n", time_spent);
	printf("cost best and worst  : %lf  %lf\n", L->tail->cost,L->head->cost);
	for(int i = 0 ; i < _dimension ; i++){
		printf("  %d     ",arr[L->tail->index][i] );
	}
	printf("\n");
	begin = clock(); 
	while(loop < 3000){
		createOffs(numPopul*7/10,arr,L,arrData,numPopul);
		loop++;
	};
	end = clock();
	time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
	printf("time spent 3000 loop : %lf \n", time_spent);
	printf("cost best and worst : %lf  %lf\n", L->tail->cost,L->head->cost);
	for(int i = 0 ; i < _dimension ; i++){
		printf(" %d     ",arr[L->tail->index][i] );
	}
	printf("\n");
	begin = clock(); 
	while(loop < 5000){
		createOffs(numPopul*7/10,arr,L,arrData,numPopul);
		loop++;
	};
	end = clock();
	time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
	printf("time spent 5000 loop: %lf \n", time_spent);
	printf("cost best and worst : %lf  %lf\n", L->tail->cost,L->head->cost);
	for(int i = 0 ; i < _dimension ; i++){
		printf(" %d     ",arr[L->tail->index][i] );
	}
	printf("\n");
	begin = clock(); 
	while(loop < 10000){
		createOffs(numPopul*7/10,arr,L,arrData,numPopul);
		loop++;
	};
	end = clock();
	time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
	printf("time spent 10000 loop: %lf \n", time_spent);
	printf("cost best and worst : %lf  %lf\n", L->tail->cost,L->head->cost);
	for(int i = 0 ; i < _dimension ; i++){
		printf("  %d     ",arr[L->tail->index][i] );
	}
	printf("\n");
	begin = clock(); 
	while(loop < 15000){
		createOffs(numPopul*7/10,arr,L,arrData,numPopul);
		loop++;
	};
	end = clock();
	time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
	printf("time spent 15000 loop: %lf \n", time_spent);
	printf("cost best and worst : %lf  %lf\n", L->tail->cost,L->head->cost);
	for(int i = 0 ; i < _dimension ; i++){
		printf("  %d     ",arr[L->tail->index][i] );
	}
	printf("\n");
	begin = clock(); 
	while(loop < 20000){
		createOffs(numPopul*7/10,arr,L,arrData,numPopul);
		loop++;
	};
	end = clock();
	time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
	printf("time spent 20000 loop: %lf \n", time_spent);
	printf("cost best and worst : %lf  %lf\n", L->tail->cost,L->head->cost);
	for(int i = 0 ; i < _dimension ; i++){
		printf("  %d     ",arr[L->tail->index][i] );
	}
	printf("\n");
	begin = clock(); 
	while(loop < 30000){
		createOffs(numPopul*7/10,arr,L,arrData,numPopul);
		loop++;
	};
	end = clock();
	time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
	printf("time spent 30000 loop: %lf \n", time_spent);
	printf("cost best and worst : %lf  %lf\n", L->tail->cost,L->head->cost);
	for(int i = 0 ; i < _dimension ; i++){
		printf("  %d     ",arr[L->tail->index][i] );
	}
	printf("\n");
	begin = clock(); 
	while(loop < 40000){
		createOffs(numPopul*7/10,arr,L,arrData,numPopul);
		loop++;
	};
	end = clock();
	time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
	printf("time spent 40000 loop: %lf \n", time_spent);
	printf("cost best and worst : %lf  %lf\n", L->tail->cost,L->head->cost);
	for(int i = 0 ; i < _dimension ; i++){
		printf("  %d     ",arr[L->tail->index][i] );
	}
	printf("\n");
	begin = clock(); 
	while(loop < 50000){
		createOffs(numPopul*7/10,arr,L,arrData,numPopul);
		loop++;
	};
	end = clock();
	time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
	printf("time spent 50000 loop: %lf \n", time_spent);
	printf("cost best and worst : %lf  %lf\n", L->tail->cost,L->head->cost);
	for(int i = 0 ; i < _dimension ; i++){
		printf("  %d     ",arr[L->tail->index][i] );
	}
	printf("\n");
	begin = clock(); 
	while(loop < 60000){
		createOffs(numPopul*7/10,arr,L,arrData,numPopul);
		loop++;
	};
	end = clock();
	time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
	printf("time spent 60000 loop: %lf \n", time_spent);
	printf("cost best and worst : %lf  %lf\n", L->tail->cost,L->head->cost);
	for(int i = 0 ; i < _dimension ; i++){
		printf("  %d     ",arr[L->tail->index][i] );
	}
	printf("\n");
}
