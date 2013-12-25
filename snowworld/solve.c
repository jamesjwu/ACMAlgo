#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <stdbool.h>
typedef struct elem{
  int to;
  int from;
  double snow;
} edge;

typedef edge* edgeList;

edgeList getEdgeList(int e);
double **adjacencyMatrix(int n, edgeList edges, int e);
void freeMatrix(double **adjMat, int n);
void solve(edgeList edges, int e, double** adjMat, int n, double alpha);

int main(){
  int n, e;
  double alpha;
  scanf("%d %d %lf", &n, &e, &alpha);
  edgeList edges = getEdgeList(e);
  double** adjMat = adjacencyMatrix(n, edges, e);
  solve(edges, e, adjMat, n, alpha);

  freeMatrix(adjMat, n);
  return 0;
}

/* Solves and prints out solution */
int findMax(edgeList edges, int e, double** adjMat, int n, int i, int* unavail) {
    double maxsnow = 0;
    int maxcity = 0;
    for (int j = 0; j < n; j++) {
        if (unavail[j] != 1) {
            if (maxsnow < adjMat[i][j]){
                maxsnow = adjMat[i][j];
                maxcity = j;
            }
        }
    }
    if (maxsnow == 0) return INT_MAX;
    //printf("%d\n", maxcity);
    return maxcity;
}
bool has_avail(int* unavail, int n) {
    for (int i = 0; i < n; i++) {
        if (unavail[i] == 0) return true;
    }
    return false;
}
int* solve_edges(edgeList edges, int e, double** adjMat, int n, int prev,
                 int i, int* path, int* unavail, double* sum,double* maxsum,
                 double alpha, double* prevsnow, bool back, bool backprev) {
    if (backprev == true && path[0] >= 1) adjMat[path[path[0]-2]][path[0]-1] = *prevsnow;
    int k = findMax(edges, e, adjMat, n,i, unavail);
    //printf("%d\n",k);
//    printf("i is %d\n", i);
    unavail[i] = 1;
    path[0] = path[0] + 1;
    path[path[0]] = i;
    if (k == INT_MAX) {
        if (*maxsum < *sum) {
            *maxsum = *sum;
            for (int l = 0; l <= path[0]; l++) {
                printf("%d\n", path[l]);
            }
        }
//        printf("path length is %d\n", path[0]);
        if (path[0] < 2) return path;
        *prevsnow = adjMat[prev][i];
        adjMat[prev][i] = 0;
        unavail[i] = 0;
        path[0]=path[0]-2;
        *sum = 0;
        return solve_edges(edges, e, adjMat, n, path[path[0]-2], path[path[0]-1], path, unavail, sum, maxsum, alpha, prevsnow, true, false);
    }
    *sum = *sum + adjMat[i][k] * pow(alpha, (double)(path[0] - 1));
    if (back == true) return solve_edges(edges,e, adjMat, n,i,k, path, unavail, sum, maxsum,alpha, prevsnow, false, true);
    else return solve_edges(edges, e, adjMat, n,i,k,path,unavail, sum, maxsum,alpha,prevsnow, false, false);
}

void solve(edgeList edges, int e, double** adjMat, int n, double alpha){
    //stack anna = stack_new();
    //queue sonya = queue_new();
    double sum = 0;
    int* path = calloc((n+1),sizeof(int));
    int* unavail = malloc(n*sizeof(int));
    if (path == NULL || unavail == NULL) exit(1);
//    printf("Hi\n");
    double maxsum = 0;
    double prevsnow = 1;
    path = solve_edges(edges, e, adjMat, n, INT_MIN, 0, path, unavail, (double*)&sum, (double*)&maxsum,alpha, &prevsnow, false,false);
    //for (int k = 0; k <= path[0]; k++) {
    //    printf("%d\n", path[k]);
    //}
    free(path);
    free(unavail);
}

/* Given functions */

/* Reads from stdin and forms an edgeList of type edgeList */
edgeList getEdgeList(int e){
  edgeList edges = (edge *)malloc(e*sizeof(edge));
  int a, b;
  double c;
  for(int i = 0; i < e; i++){
    scanf("%d %d %lf", &a, &b, &c);
    edges[i].to = a;
    edges[i].from = b;
    edges[i].snow = c;
  }
  return edges;
}

/* Creates an adjacency matrix from the edgeList */
double **adjacencyMatrix(int n, edgeList edges, int e){
  double **adjMat;
  if( (adjMat = (double **)malloc(n*sizeof(double*)) ) == NULL)
    exit(1);
  for(int i = 0; i < n; i++){
      if((adjMat[i] = (double *)calloc(n,sizeof(double))) == NULL)
      exit(1);
  }
  int to, from;
  double snow;
  for(int i = 0; i < e; i++){
    to = edges[i].to;
    from = edges[i].from;
    snow = edges[i].snow;
    adjMat[to][from] = snow;
    adjMat[from][to] = snow;
  }
  return adjMat;
}

/* Deletes what was allocated (using delete) */
void freeMatrix(double **adjMat, int n){
  if(adjMat == NULL) return;
  for(int i = 0; i < n; i++){
    if(adjMat[i] != NULL)
      free(adjMat[i]);
  }
  free(adjMat);
}
