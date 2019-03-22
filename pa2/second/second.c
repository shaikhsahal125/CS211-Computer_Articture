#include<stdio.h>
#include<stdlib.h>
int isavailable(int**, int, int, int);
int solve(int **);
int empty(int**);

int isavailable(int** matrix, int row, int col, int value){
  //check for rows 
  for (int i = 0; i < 9; i++){
    if (matrix[row][i] == value){
      return 0;
    }
  }
  //check for cols
  for (int i = 0; i < 9; i++){
    if (matrix[i][col] == value){
      return 0;
    }
  }

  //check for cells
  if ((row < 3) && (col < 3)){
    for (int i = 0; i < 3; i++){
      for (int j = 0; j < 3; j++){
	if (matrix[i][j] == value){
	  return 0;
	}
      }
    }
    return 1;
  }
  if ((row < 3) && ((col >= 3) && (col < 6))){
    for (int i = 0; i < 3; i++){
      for (int j = 3; j < 6; j++){ 
	if (matrix[i][j] == value){
	  return 0;
	}
      }
    }
    return 1;
  }
  if ((row < 3) && ((col >= 6) && (col < 9))){
    for (int i = 0; i < 3; i++){
      for (int j = 6; j < 9; j++){
	if (matrix[i][j] == value){
	  return 0;
	}
      }
    }
    return 1;
  }
  if (((row >= 3) && (row < 6)) && (col < 3)){
    for (int i = 3; i < 6; i++){
      for (int j = 0; j < 3; j++){
	if (matrix[i][j] == value){
	  return 0;
	}
      }
    }
    return 1;
  }
  if (((row >= 3) && (row < 6)) && ((col >= 3) && (col < 6))){
    for (int i = 3; i < 6; i++){
      for (int j = 3; j < 6; j++){
	if (matrix[i][j] == value){
	  return 0;
	}
      }
    }
    return 1;
  }
  if (((row >= 3) && (row < 6)) && ((col >= 6) && (col < 9))){
    for (int i = 3; i < 6; i++){
      for (int j = 6; j < 9; j++){
	if (matrix[i][j] == value){
	  return 0;
	}
      }
    }
    return 1;
  }
  if (((row >= 6) && (row < 9)) && (col < 3)){
    for (int i = 6; i < 9; i++){
      for (int j = 0; j < 3; j++){
	if (matrix[i][j] == value){
	  return 0;
	}
      }
    }
    return 1;
  }
  if (((row >= 6) && (row < 9)) && ((col >= 3) && (col < 6))){
    for (int i = 6; i < 9; i++){
      for (int j = 3; j < 6; j++){
	if (matrix[i][j] == value){
	  return 0;
	}
      }
    }
    return 1;
  }
  if (((row >= 6) && (row < 9)) && ((col >= 6) && (col < 9))){
    for (int i = 6; i < 9; i++){
      for (int j = 6; j < 9; j++){
	if (matrix[i][j] == value){
	  return 0;
	}
      }
    }
    return 1;
  }
  return 1;
}

int empty(int** matrix){
  for (int i = 0; i < 9; i++){
    for (int j = 0; j < 9; j++){
      if (matrix[i][j] == 0){
	return 1;
      }
    }
  }
  return 0;
}


int solve(int** matrix){
  if (!empty(matrix)){
    return 1;
  } else {  
    for (int i = 0; i < 9; i++){
      for (int j = 0; j < 9; j++){
	if (matrix[i][j] == 0){
	  int markoff = 0;
	  int a[10] = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	for (int k = 1; k <= 9; k++){
	  if (!isavailable(matrix, i, j, k)){
	    a[k] = 1;
	    markoff++;
	  }
	}
	if(markoff == 8){
	  for (int s = 1; s <= 9; s++){
	    if (a[s] == 0){
	      matrix[i][j] = s;
	    }
	  }
	}
	}
      }
    }
  }
  if (solve(matrix)){
    return 1;
  }
  return 0;
}

int main(int argc, char** argv){
  if (argc != 2){
    printf("invalid input\n");
    return 0;
  }
  FILE * fp;
  int **matrix;
  matrix = (int**) malloc(9 * sizeof(int*));
  for (int i = 0; i < 9; i++){
    matrix[i] = (int*) malloc(9 * sizeof(int));
  }

  fp = fopen(argv[1], "r");
 
  for (int i = 0; i < 9; i++){
    for (int j = 0; j < 9; j++){
      fscanf(fp, "%d\t", &matrix[i][j]);
    }
    fscanf(fp, "\n");
  }

  if (solve(matrix)){ 
    for (int i = 0; i < 9; i++){
      for (int j = 0; j < 9; j++){
	printf("%d\t", matrix[i][j]);
      }
      printf("\n");
    }
  } else {
    printf("no-solution");
  }
  for (int i = 0; i < 9; i++){
    free(matrix[i]);
  }
  free(matrix);
  fclose(fp);
  return 0;
}
