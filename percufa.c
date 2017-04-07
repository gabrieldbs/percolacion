#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define P     16             // 1/2^P, P=16
#define Z     27000        // iteraciones
#define N     30             // lado de la red simulada


void  llenar(int *red,int n, int m, float prob);
int*   hoshenVec(int *red,int n, float prob);
int   actualizar(int *red,int *clase,int s,int frag);
void  etiqueta_falsa(int *red,int *clase,int s1,int s2);
void  corregir_etiqueta(int *red,int *clase,int n);
int   percola(int *red,int n);
void  print_red(int* red, int n_fil, int n_col);
int etiqueta_verdadera(int *clase, int s);
//int* MasPiola(int *red, int n, int *clase);


int main(int argc,char *argv[])   // Los argumentos por linea son dimension n y proba p
{
  int    i,j,k,l,*red,*clase,n,z;
  float  prob,denominador;
  prob = 0.5;

  n=N;
  z=Z;
  n = 10;

  if (argc==3) 
     {
       sscanf(argv[1],"%d",&n);
       sscanf(argv[2],"%f",&prob);
     }
 
  red=(int *)malloc(n*n*sizeof(int));
  
float s=0;
float p=0;
  for(k=0;k<Z;k++){
  	srand(k);
  	float step=.5;
  	prob = 0.5;
  	for (l = 0; l<P; l++){
  		step = .5*step;
  		clase=hoshenVec(red,n,prob);
  		/*print_red(red,n,n);
  		printf("\n");
  		printf("%d\n", percola(red,n));*/
  		if (percola(red,n)){
  			prob=prob-step;
  		}
  		else{
  			prob=prob+step;
  		}
  		free(clase);
  		
  	}
  		p=p+prob;
  		s=s+prob*prob;
  		//printf("%g\n",prob );
  }
  p = p/Z;
  s=s/Z-p*p;
printf("Promedio %g\n Varianza %g\n",p,s);
 /* 
  int* clase = hoshenVec(red, n, prob);
  print_red(red, n,n);
  int sum=0;
  int clusters=-1;
  int zeros=0;
  int ocupados=0;
  for(i=0;i<n*n;i++){
    if(clase[i]>0) {
      // printf("Etiqueta %d: %d\n", i, clase[i]);
      sum = sum+clase[i];
      clusters++;
    }
    if(red[i]==0){zeros++;}
    else {ocupados++;}
  }
  // printf("La red es de %d nodos; con %d nodos vacios y %d nodos ocupados (contando con hoshenVec)\n", n*n, clase[0],sum-clase[0]);  // Chequeo que efectivamente conte bien
  // printf("Contando manualmente, encontramos %d nodos vacios y %d ocupados\n", zeros, ocupados);
  // printf("Hay %d clusters, cuyo tamaño medio es %f \n", clusters, (float)(sum-clase[0])/clusters); */
  free(red);
  //free(clase);

  return 0;
}


int   actualizar(int *red,int *clase,int s,int frag){  // Actualizar etiqueta en *red cuando tengo a lo sumo 1 vecino con etiqueta s
  int res = frag;
  if (s==0){    // Mi vecino no tiene etiqueta -> No tengo etiquetas vecinas -> Creo una nueva
    *red = frag;
    res++;      // Actualizo el nombre de la proxima etiqueta
    clase[frag] = 1;
    // printf("Creamos la nueva etiqueta %d\n", frag);
  }else{        // Mi vecino tiene etiqueta -> Se la copio
    *red = s;

    // El proximo if no es realmente necesario, lo pongo para que los // printf sean más claros. 
    //En realidad puede directamente ponerse sv = etiqueta_verdadera(clase,s) y hacer clase[sv]++
    // En particular, tambien puedo asignar *red = sv para ahorrarme un paso luego en corregir_etiqueta (insisto, no es importante)
    if(clase[s]<0){   // Mi vecino tiene etiqueta falsa
      // printf("Agregariamos este nodo a %d, pero es una etiqueta falsa ", s);
      int sv = etiqueta_verdadera(clase,s);     // Busco la verdadera etiqueta
      clase[sv]++;                              // Le sumo un nodo
      // printf("así que en su lugar lo agregamos a %d, que ahora tiene %d nodos \n", sv, clase[sv]);
    }else{            // Mi vecino tiene etiqueta verdadera -> Al toque!
      clase[s]++;                              // Le sumo un nodo
      // printf("Agregamos este nodo a %d, ahora tiene %d nodos \n", s, clase[s]);
    }
  }
  return res;
}

void  etiqueta_falsa(int *red,int *clase,int s1,int s2){

  int s1v = etiqueta_verdadera(clase,s1);
  int s2v = etiqueta_verdadera(clase,s2);

  // Al terminar este bloque, ya esto seguro que las clases de s1v y 2sv son positivas (porque son verdaderas)
  // Me fijo cual de las 2 etiquetas VERDADERAS es menor
  // printf("Etiquetas %d (%d nodos) y %d (%d nodos) en conflicto  -----> ", s1v, clase[s1v], s2v, clase[s2v]);
  if (s1v>s2v){   // s1v es la menor -> Va a ser la verdadera
    *red = s2v;
    clase[s2v]++;                       // Agrego los nuevos nodos a mi 
    clase[s2v] = clase[s1v]+clase[s2v]; // contador de s2v (la etiqueta verdadera)
    clase[s1v]=-s2v;    // Ahora s1v es falsa y su verdadera es s2v
    clase[s1] = -s2v;
    // printf("La nueva etiqueta es %d con %d nodos\n",s2v,clase[s2v]);
  }else{
    if (s1v==s2v){ // Son la misma etiqueta, no pasa una
      *red = s1v;
      clase[s1v]++;
      // printf("Son la misma etiqueta, ahora hay %d \n",clase[s1v]);
    }else{ // s1v es la menor -> Va a ser la verdadera
      *red = s1v;
      clase[s1v]++;                       // Agrego los nuevos nodos a mi 
      clase[s1v] = clase[s1v]+clase[s2v]; // contador de s2v (la etiqueta verdadera)
      clase[s2v]=-s1v;    // Ahora s2v es falsa y su verdadera es s1v
      clase[s2] = -s1v;
      // printf("La nueva etiqueta es %d con %d nodos\n",s1v,clase[s1v]);
    }
  }
}


void  corregir_etiqueta(int *red,int *clase,int n){
  int i;
  for(i=2;i<n*n;i++){         // En caso de que alguna etiqueta no haya sido actualizada, lo hago ahora [Este paso creo que es innecesario dada la forma de etiquetar, pero no cuesta nada]
    etiqueta_verdadera(clase,i);
  }
  for(i=0;i<n*n;i++){
    if (clase[red[i]]<0){   // Si su etiqueta es falsa, tengo que poner la verdadera; que es -clase[s] por definicion (ver etiqueta_verdadera y hoshenVec)
      red[i] = -clase[red[i]];
    }
  }
}


int   percola(int *red,int n){
  int etiquetas[n*n];     // Sé que a lo sumo habra n etiquetas en la primera fila
  int i, res;
  res = 0;
  for(i=0;i<n*n;i++){
  	etiquetas[i]=0;
  }
  for(i=0;i<n;i++){
  		etiquetas[*(red+i)] = 1;
  }
  for(i=0;i<n;i++){
  	if (red[n*(n-1)+i]>0 & etiquetas[*(red+(n-1)*n+i)]){
  		res = 1;
  		//break;
  	}
  }
  return res;  // Si salí del while por j>=n, entonces claramente no hay percolacion (res=0 <-> No hay coincidencia)
}


int etiqueta_verdadera(int *clase, int s){    // Actualizo la etiqueta verdadera de una falsa y devuelvo la etiqueta verdadera
  int res = s;
  while (clase[res]<0){    // Avanzo hasta encontrar la etiqueta verdadera
    res = -clase[res];
  }
  if (res!=s){
    clase[s] = -res;      // Dejo el recuerdo de que s es falsa (dejandolo negativo) y la hago apuntar directo a la verdadera 
  }
  return res;
}
// OBS: Si s ya es una etiqueta verdadera, devuelve s sin hacer nada -> Al toque!


int* hoshenVec(int *red,int n, float prob)    // En este hoshen la convención es: - Si s es una etiqueta verdadera, clase[s] es la cantidad de nodos con etiqueta s
{                                             //                                  - Si s es una etiqueta falsa, clase[s] = -etiqueta_verdadera(clase,s) (osea, apunta a su verdadera etiqueta con un -)

                                              // OBS: Aunque me encantaría que se cumpliera la segunda condicion, la posta es que no estoy seguro así que tomo recaudos al respecto
  /*
    Esta funcion implementa en algoritmo de Hoshen-Kopelman.
    Recibe el puntero que apunta a la red y asigna etiquetas 
    a cada fragmento de red. 
  */

  int i,j,k,s1,s2,frag;
  int *clase;

  frag=0;
  
  clase=(int *)malloc(n*n*sizeof(int));

  for(k=0;k<n*n;k++) *(clase+k)=frag;
  
  // primer elemento de la red

  s1=0;
  frag=2;
  int s = rand();
  // printf("(0,0):   ");
  if (s<prob*RAND_MAX){
    // printf("El primero esta ocupado\n");
    frag=actualizar(red,clase,s1,frag);
  }else{
    clase[0]++;
    red[0] = 0;
    // printf("Nodo vacio\n");
  }
  
  // primera fila de la red

  for(i=1;i<n;i++) 
    {
      // printf("(0,%d):   ", i);
      int s = rand();
      if (s<prob*RAND_MAX) 
         {
           s1=*(red+i-1);  
           frag=actualizar(red+i,clase,s1,frag);
         }else{
           clase[0]++;
           red[i] = 0;
           // printf("Nodo vacio\n");
         }
    }
  

  // el resto de las filas 

  for(i=n;i<n*n;i=i+n)
    {

      int s = rand();

      // primer elemento de cada fila
      // printf("(%d,0):   ", i/n);
      if (s<prob*RAND_MAX) 
         {
           s1=*(red+i-n); 
           frag=actualizar(red+i,clase,s1,frag);
         }else{
          clase[0]++;
          red[i]=0;
          // printf("Nodo vacio\n");
        }

      for(j=1;j<n;j++){
        // printf("(%d,%d):   ", i/n, j);
        int s = rand();
	if (s<prob*RAND_MAX)
	  {
	    s1=*(red+i+j-1); 
        s2=*(red+i+j-n);

	    if (s1*s2>0)
	      {      
		etiqueta_falsa(red+i+j,clase,s1,s2);
	      }
	    else 
	      { if (s1!=0) frag=actualizar(red+i+j,clase,s1,frag);
	        else       frag=actualizar(red+i+j,clase,s2,frag);
	      }
	  }else {
      red[i+j] = 0;
      clase[0]++;
      // printf("Nodo vacio\n");
    }
    }
  }


  corregir_etiqueta(red,clase,n);

  return clase;
}

void llenar(int* red, int n_fil, int n_col, float p){
  int i,j;
  for(i=0;i<n_fil;i++){
    for(j=0;j<n_col;j++){
      float a = rand();
        if (a<=p*RAND_MAX){
          red[i*n_fil+j] = 1;
        }else{
          red[i*n_fil+j] = 0;
        }
    }
  }
}

void print_red(int* red, int n_fil, int n_col){
  int i,j;
  for(i=0;i<n_fil;i++){
    for(j=0;j<n_col;j++){
      printf("%3d ", red[i*n_fil+j]);
    }
    printf("\n");
  }
}

