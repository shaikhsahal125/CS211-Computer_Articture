#include<stdio.h>
#include<stdlib.h>

double** transpose(double**, int, int);
double** inverse(double**, int);

double** transpose(double** matrix, int m, int n){
  double** xt;
  xt = (double**) malloc((m+1) * sizeof(double*));
  for (int i = 0; i < (m+1); i++){
    xt[i] = (double*) malloc(n * sizeof(double));
  }

  for (int i = 0; i < n; i++){
    for (int j = 0; j < (m+1); j++){
      xt[j][i] = matrix[i][j];
    }
  }
  return xt;
}

double** inverse(double** matrix, int m){
  double ** aug;
  aug = (double**) malloc((m+1) * sizeof(double*));
  for (int i = 0; i < (m+1); i++){
    aug[i] = (double*) malloc((m+1) * sizeof(double));
  }
  //create an identity matrix
  for (int i = 0; i < (m+1); i++){
    for (int j = 0; j < (m+1); j++){
      if (i == j){
	aug[i][j] = 1;
      } else {
	aug[i][j] = 0;
      }
    }
  }
  
  for (int i = 0; i < (m+1); i++){
    for (int j = 0; j < (m+1); j++){
      double n = matrix[i][j];
      if (i == j){
	for (int k = 0; k < (m+1); k++){
	  matrix[i][k] = matrix[i][k] / n;
	  aug[i][k] = aug[i][k] / n;
	}
      }  else if (i > j){
	for (int k = 0; k < (m+1); k++){
	  matrix[i][k] = matrix[i][k] - matrix[j][k] * n;
	  aug[i][k] = aug[i][k] - aug[j][k] * n;
	}
      } 
    }
  }
  for (int i = m; i >= 0; i--){
    for (int j = m; j >= 0; j--){
      double nb = matrix[i][j];
      if (i < j){
	for (int k = m; k >= 0; k--){
	  matrix[i][k] = matrix[i][k] - (matrix[j][k] * nb);
	  aug[i][k] = aug[i][k] - (aug[j][k] * nb);
	}
      }
    }
  }
  return aug;
}

int main(int argc, char** argv){
  if (argc != 3){
    printf("Invalid input\n");
    return 0;
  }
  FILE * fp1;
  FILE * fp2;
  int m, n, n2;

  fp1 = fopen(argv[1], "r");
  if (fp1 == NULL) return 0;

  fp2 = fopen(argv[2], "r");
   if (fp2 == NULL) return 0;

  fscanf(fp1, "%d", &m);
  fscanf(fp1, "%d", &n);
  fscanf(fp2, "%d", &n2);

  double** matrixX;
  double* y;
  double** transposeX;
  double** inversextx;
  double** xtx;
  double** invx;
  double* w;
  double** newX;
  double* y2;

  matrixX = (double**) malloc(n * sizeof(double*));
  y = (double*) malloc(n * sizeof(double));
  transposeX = (double**) malloc((m+1) * sizeof(double*));
  inversextx = (double**) malloc((m+1) * sizeof(double*));
  xtx = (double**) malloc((m+1)* sizeof(double*));
  invx = (double**) malloc((m+1)* sizeof(double*));
  w = (double*) malloc((m+1) * sizeof(double));
  newX = (double**) malloc(n2 * sizeof(double*));
  y2 = (double*) malloc(n * sizeof(double));

  for (int i = 0; i < n; i++){
    matrixX[i] = (double*) malloc((m+1) * sizeof(double));
  }

  for (int i = 0; i < (m+1); i++){
    transposeX[i] = (double*) malloc(n * sizeof(double));
  }

  for (int i = 0; i < (m+1); i++){
    inversextx[i] = (double*) malloc((m+1) * sizeof(double));
  }

  for (int i = 0; i < (m+1); i++){
    xtx[i] = (double*) malloc((m+1) * sizeof(double));
  }

  for (int i = 0; i < (m+1); i++){
    invx[i] = (double*) malloc(n * sizeof(double));
  }

  for (int i = 0; i < n2; i++){
    newX[i] = (double*) malloc((m+1) * sizeof(double));
  }
	  
  //get full training data
  for (int i = 0; i < n; i++){
    matrixX[i][0] = 1;
    for (int j = 0; j < m; j++){
      fscanf(fp1, "%lf,", &matrixX[i][j+1]);
    }
    fscanf(fp1, "%lf\n", &y[i]);
  }
  // get test data
  for (int i = 0; i < n2; i++){
    newX[i][0] = 1;
    for (int j = 0; j < m; j++){
      fscanf(fp2, "%lf,", &newX[i][j+1]);
    }
     fscanf(fp2, "\n");
  }
  //get transpose of X
  transposeX = transpose(matrixX, m, n);

 //multiplication of X^T and X
  for (int i = 0; i < (m+1); i++){
    for (int j = 0; j < (m+1); j++){
      xtx[i][j] = 0;
      for (int k = 0; k < n; k++){
	xtx[i][j] = xtx[i][j] + transposeX[i][k] * matrixX[k][j];
      }
    }
  }
  //get inverse
  inversextx = inverse(xtx, m);
 
  //inverse * xTrans.
  for (int i = 0; i < (m+1); i++){
    for (int j = 0; j < n; j++){
      invx[i][j] = 0;
      for (int k = 0; k < (m+1); k++){
	invx[i][j] = invx[i][j] + inversextx[i][k] * transposeX[k][j];
      }
    }
  }

  //get w
  for (int i = 0; i < (m+1); i++){
    w[i] = 0;
    for (int j = 0; j < n; j++){
      w[i] = w[i] + invx[i][j] * y[j];
    }
  }

  //finally predict prize 
  for (int i = 0; i < n2; i++){
    y2[i] = 0;
    for (int j = 0; j < (m+1); j++){
      y2[i] = y2[i] + newX[i][j] * w[j];
    }
  }

  //prints all data
  for (int i = 0; i < n2; i++){
    printf("%d\n",(int) y2[i]);
  }
  
  //free-up allocated memory
  for (int i = 0; i < (m+1); i++){
    free(transposeX[i]);
    free(inversextx[i]);
    free(xtx[i]);
    free(invx[i]);
  }
  for (int i = 0; i < n; i++){
    free(matrixX[i]);
  }
  for (int i = 0; i < n2; i++){
    free(newX[i]);
  }
  free(y);
  free(y2);
  free(transposeX);
  free(inversextx);
  free(xtx);
  free(invx);
  free(matrixX);
  free(newX);
  fclose(fp1);
  fclose(fp2);
  return 0;
}

