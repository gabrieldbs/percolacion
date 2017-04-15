
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>



void  llenar(int *red,int n, int m, float prob);
int*   hoshenVec(int *red,int n, float prob);
int   actualizar(int *red,int *clase,int s,int frag);
void  etiqueta_falsa(int *red,int *clase,int s1,int s2);
void  corregir_etiqueta(int *red,int *clase,int n);
int   percola(int *red,int n);
void  print_red(int* red, int n_fil, int n_col);
int etiqueta_verdadera(int *clase, int s);
int* numeros_cluster(int n, int *clase, int* n_max);
void pc_promedio(int* red, int n, int P, float* p, float* var, int It);
//int* MasPiola(int *red, int n, int *clase);


int main(int argc,char *argv[])   // Por ahora no hay argumentos por linea
{

  /* Dejo esto para acordarme como tomar argumentos por linea
  if (argc==3) 
     {
       sscanf(argv[1],"%d",&n);
       sscanf(argv[2],"%f",&prob);
     }
  */



  return 0;
}

//  FUNCIONES DE EJERCICIOS


// 1.a)
void pc_promedio(int* red, int n, int P, float* p, float* var, int It){   // Cuando queremos que algo nos devuelva 2+ variables, un buen truco es crearlas fuera de la funcion 
// (con cualquier valor) y pasarle a la funcion punteros a esa variables, cosa que la funcion se encargue de poner en esa direccion de memoria el valor correcto
// Si no te gusta, tranquilamente podemos devolver un array de 2 posiciones, pero personalmente preferiria utilizar la menor cantidad de memoria dinamica posible.
  float prob, step;
  int *clase;
  int k,l;
  *p = 0;
  *var = 0;
  for(k=0;k<It;k++){   // Itero It veces
    srand((unsigned)time(NULL));
    step=.25;          // Tamaño del paso inicial
    prob = 0.5;       // Probabilidad inicial
    for (l = 0; l<P; l++){
      clase=hoshenVec(red,n,prob);  // Aca es un bajon que clase no tenga utilidad. Si te sirve de consuelo, calcular clase no hace esto apreciablemente lento.
      free(clase);  
      if (percola(red,n)){
        prob=prob-step;     // Si percola, el pc debe estar mas abajo
      }
      else{
        prob=prob+step;     // Si no, debe estar mas arriba
      }
    }
    step = .5*step; // Actualizo el tamaño del paso

    *p = *p+prob;           // Voy sumando los prob para obtener el prob promedio luego
    *var = *var+prob*prob;  // Voy sumando los prob^2 para obtener la varianza luego
  }
  *p = *p/It;           // Divido para obtener el promedio
  *var = *var/It-(*p)*(*p);   // Divido para obtener el promedio de los prob^2 y le resto el promedio al cuadrado -> Varianza!
}


int   actualizar(int *red,int *clase,int s,int frag){  // Actualizar etiqueta en *red cuando tengo a lo sumo 1 vecino con etiqueta s
  int res = frag;
  int sv;
  if (s==0){    // Mi vecino no tiene etiqueta -> No tengo etiquetas vecinas -> Creo una nueva
    *red = frag;
    res++;      // Actualizo el nombre de la proxima etiqueta
    clase[frag] = 1;
    // printf("Creamos la nueva etiqueta %d\n", frag);
  }else{        // Mi vecino tiene etiqueta -> Se la copio
    sv = etiqueta_verdadera(clase,s);  // Busco la etiqueta verdadera de mi vecino
    clase[sv]++;                       // Ahora tiene un nodo mas
    *red = sv;                         // Y el nodo tiene esa etiqueta
  }
  return res;
}

void  etiqueta_falsa(int *red,int *clase,int s1,int s2){

  int s1v = etiqueta_verdadera(clase,s1);
  int s2v = etiqueta_verdadera(clase,s2);
  if (s1v>s2v){   // s1v es la menor -> Va a ser la verdadera
    *red = s2v;
    clase[s2v]++;                       // Agrego los nuevos nodos a mi 
    clase[s2v] = clase[s1v]+clase[s2v]; // contador de s2v (la etiqueta verdadera)
    clase[s1v]=-s2v;    // Ahora s1v es falsa y su verdadera es s2v
    clase[s1] = -s2v;
  }else{
    if (s1v==s2v){ // Son la misma etiqueta, no pasa una
      *red = s1v;
      clase[s1v]++;
    }else{ // s1v es la menor -> Va a ser la verdadera
      *red = s1v;
      clase[s1v]++;                       // Agrego los nuevos nodos a mi 
      clase[s1v] = clase[s1v]+clase[s2v]; // contador de s2v (la etiqueta verdadera)
      clase[s2v]=-s1v;    // Ahora s2v es falsa y su verdadera es s1v
      clase[s2] = -s1v;
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
  int etiquetas[n];     // Sé que a lo sumo habra n etiquetas en la primera fila, todas menores a n
  int i, res, e;
  res = 0;
  for(i=0;i<n;i++){
  	etiquetas[i]=0;
  }
  for(i=0;i<n;i++){
  	etiquetas[*(red+i)] = 1;  // pongo 1 si esa etiqueta esta en la primer fila
  }
  for(i=0;i<n;i++){
	e =red[n*(n-1)+i]; // Guardo la etiqueta en una variable auxiliar para facilitar el manejo
  	if (e>0 && e<n && etiquetas[e]){ // Me fijo si la etiqueta esta en rango y luego
  		res = 1;		 // si esa etiqueta ya aparecio en la 1° fila
  		break;		// Como ya encontre coincidencia, termino el ciclo
  	}
  }
  return res;  
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
    frag=actualizar(red,clase,s1,frag);
  }else{
    clase[0]++;
    red[0] = 0;
  }
  
  // primera fila de la red

  for(i=1;i<n;i++) 
    {
      int s = rand();
      if (s<prob*RAND_MAX) 
         {
           s1=*(red+i-1);  
           frag=actualizar(red+i,clase,s1,frag);
         }else{
           clase[0]++;
           red[i] = 0;
         }
    }
  

  // el resto de las filas 

  for(i=n;i<n*n;i=i+n)
    {

      int s = rand();

      // primer elemento de cada fila
      if (s<prob*RAND_MAX) 
         {
           s1=*(red+i-n); 
           frag=actualizar(red+i,clase,s1,frag);
         }else{
          clase[0]++;
          red[i]=0;
        }

      for(j=1;j<n;j++){
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
 /*
 Para el  punto dos
 */
int* numeros_cluster(int n, int *clase, int* n_max){  // La funcion recibe como argumento un puntero a n_max cuya contenido no importa, sino que solo
  int i ,j;                                           // es un contenedor para que al final de la función tendrá el tamaño del máximo cluster        
  int* res;                                           // Si no hicieramos esto, ¿como sabriamos la longitud de res fuera de la funcion?
  for(i=2;i<n*n;i++){   // Buscamos el valor de n_max, correspondiente
    if(clase[i]>*n_max){ // al tamaño del cluster mas grande
      *n_max=clase[i];
    }
  }
res=(int *)malloc((*n_max)*sizeof(int)); // Creamos el resultado; un vector cuya i-esima posicion es la cantidad de clusters de tamaño i
  for(i=0;i<*n_max;i++){       // Lo inicializamos en 0
    res[i]=0;
  }
  for (j=2;j<n*n;j++){  // Cada etiqueta (verdadera) i que nos crucemos tiene un tamaño clase[i], por lo que debemos sumar 1 a res[clase[i]]
    if(clase[j]>0){ // Obviamente, no queremos contar etiquetas falsas
      res[clase[j]]++;
    }
  }   
  return res;
}


// Ejercicio 2

float* masa_percolante(int* red, int n, float* p, int m, int It){
  int i,j,perc, cantperc, *clase;
  float *res;
  res=(float *)malloc(m*sizeof(float));
  for(i=0;i<m;i++){
    res[i] = 0;
    cantperc=0;
    while(cantperc<It){               // Es importante que todos los p[i] tengan la misma
      clase = hoshenVec(red,n,p[i]);  // la misma cantidad de puntos para el promedio
      perc = percola(red,n);          // OJO que p[i]=0 da ciclo infinito!
      if (perc>0){
        res[i] = res[i]+clase[perc];  // Sumo la masa del cluster percolante actual y la
        cantperc++;                   // cantidad de percolaciones para promediar luego
      }
    }
    res[i] = res[i]/cantperc;   // Divido para obtene el promedio
    free(clase);
  }
  return res;
}


float* mediana_bisec(int* red, int* n, int m, int It, int pres){
  int i,j,k,cantperc, *clase;
  float *res,step,prob;
  res=(float *)malloc(m*sizeof(float));  // Cada res[i] es la pc de la red de n[i]xn[i]
  for(i=0;i<m;i++){ // Calculo el pc para la red de lado n[i]
    step = 0.25;  // Paso inicial
    prob = 0.5;   // Probabilidad inicial
    for(j=0;j<pres;j++){ 
      cantperc = 0; // Cuento la cantidad de veces que percola
      for(k=0;k<It;k++){
        clase = hoshenVec(red,n[i],prob); 
        free(clase);
        if(percola(red,n[i])>0){
          cantperc++;
        }
      }
      if(2*cantperc>It){ // Si percola mas de la mitad de las veces, me pase con la roba
        prob = prob-step;
      }else{ // Sino, me quede corto
        prob = prob+step;
      }
      step = 0.5*step; // Actualizo el paso
    }
    res[i] = prob; // Al terminar, asigno el pc con un error de 1/2^pres
  }
  return res;
}

/* Acá abajo pongo una lista de funciones (con declaracion tentativa) que faltaría hacer. Agreguemos a medida que se nos ocurran.

float* histograma(int* red, int* n, float* probas, int* m, int It)  
Dado un vector de probas (longitud m) y una red de nxn, se fija para cada probas[i] si percola It veces, poniendo en resultado[i] el porcentaje de veces que percolo

float mediana(float* probas, float* F, int n)
Busca el indice j cuyo F[j] es más cercano a 1/2 y devuelve el valor probas[j] (n es la longitud de los vectores)

float mediana_bisec(int* red, int* n, int m, int It, int pres)
Esta es la mia: 
Dada una red de nxn, arranca en p=1/2 y obtiene F(p) viendo el porcentaje de percolaciones de It iteraciones. En la (i+1)-esima iteracion, si el F(p) en la i-esima
es menor a 1/2, entonces vuelve a hacerlo con p = p+1/2^i; sino lo hace con p = p-1/2^i. El proceso termina cuando i<pres (pues tengo precision de 1/2^pres).

float* masa_percolante(int* red, int n, float* p, int m, int It)
Mediante It iteraciones, obtiene la masa media M para una red de nxn en funcion de los p[i].

float* dimension_fractal(int* red, int* N, float* pc, int m,int It)
Mediante It iteraciones, obtiene un vector de masas medias M cuyo M[i] es la masa media para una red de N[i]xN[i] y proba critica pc[i] (los 3 arrays de longitud m)
Basicamente, aplicar la funcion masa_percolante pero para cada una de los m tamaños de redes.


*/
