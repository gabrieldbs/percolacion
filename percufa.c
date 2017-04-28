
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
int* numeros_cluster(int n, int *clase);
void pc_promedio(int* red, int n, int P, double* p, double* var, int It);
double* intensidad(int* red, int n, float pmin, float pmax, int m, int It);
float* mediana_bisec(int* n, int m, int It, int pres);
double* histograma(int* red,int n,int m, int It);
double percentil(double* F, int m, float alfa);
double chi(double* x,double* y,int m);
double Ajuste_Lineal(double* x, double* y, int n, double* m, double* b);
double* ns_promedio(int* red, int n, float p, int It);
double dimension_fractal(int* red, int N, float pc, int It);
double* scaling(float* probas, int m, int S, float pc, int It);
double maximizar_cluster(int S, int m, double pmin, double pc, int It);
double* ns_promedio_sin_percolante(int* red, int n, float p, int It);

int main(int argc,char *argv[])   // Por ahora no hay argumentos por linea
{    srand((unsigned)time(NULL));

  /* Dejo esto para acordarme como tomar argumentos por linea
  if (argc==3) 
     {
       sscanf(argv[1],"%d",&n);
       sscanf(argv[2],"%f",&prob);
     }*/
  int Programa;
  sscanf(argv[1], "%x", &Programa);


// Ejercicio 1.a)
// La precision de los pc, la cantidad d iteraciones y el vector de dimensiones se toman,
// en orden, directo de la consola al correr el programa
  if(Programa == 26){
    printf("Ejecutando simulacion ejercicio 1.a)\n");
    int *red,*n, i,pres,It,*secs,secsTot;
    double *pcs, *vars;
    n = (int *) malloc(argc*sizeof(int));
    pcs = (double *) malloc((argc-4)*sizeof(double));
    vars = (double *) malloc((argc-4)*sizeof(double));
    secs = (int *) malloc((argc-4)*sizeof(int));
    red = (int *) malloc((argc-4)*(argc-4)*sizeof(int));
    sscanf(argv[2], "%d", &pres); // Precision 1/2^pres
    sscanf(argv[3], "%d", &It); // Cantidad de iteraciones
    secsTot = time(NULL);
    for(i=4;i<argc;i++){  // Tomo el vector de dimensiones
      sscanf(argv[i], "%d", &n[i-4]);
      red = (int *) realloc(red, n[i-4]*n[i-4]*sizeof(int));
      secs[i-4] = time(NULL);
      pc_promedio(red,n[i-4],pres, pcs+i-4, vars+i-4, It);
      secs[i-4] = time(NULL)-secs[i-4];
      printf("Red de %dx%d terminado\n", n[i-4], n[i-4]);
    }
    secsTot = time(NULL)-secsTot;int mins = secsTot/60;int horas = mins/60;
    FILE *fp = fopen("Ejercicio_1_a.txt","a");  // Escribo los resultados en un archivo
    fprintf(fp, "Simulacion con presicion 1/2^%d y %d iteraciones por red\n", pres, It);
    fprintf(fp, "Duracion: %d horas, %d minutos y %d segundos\n", horas, mins % 60, secsTot % 60);
    fprintf(fp, "Los resultados son: \n");
    for(i=0;i<argc-4;i++){
      fprintf(fp, "%dx%d: %g %g  en %dhs, %dmin, %dsegs\n", n[i],n[i],pcs[i],vars[i],secs[i]/3600,secs[i]/60 % 60,secs[i] % 60);
    }
    fprintf(fp, "\n");
    fclose(fp);
    free(pcs);
    free(vars);
    free(red);
    free(n);
    free(secs);
  }
  if(Programa == 443){
// Ejercicio 1.b) usando mediana_bisec (solo encuentra los pcs)
// Toma los argumentos por consola identicamente al problema 1.a)
    printf("Ejecutando simulacion ejercicio 1.b) sin histograma\n");
    int *n, i,pres, It;
    float *pcs;
    n = (int *) malloc(sizeof(int));
    sscanf(argv[2], "%d", &pres); // Precision 1/2^pres
    sscanf(argv[3], "%d", &It); // Cantidad de iteraciones
    for(i=4;i<argc;i++){  // Tomo el vector de dimensiones
      sscanf(argv[i], "%d", &n[i-4]); // Tomo las dimensiones de las redes
    }
    pcs = mediana_bisec(n, argc-4, It, pres); // Corro la simulacion
     free(n);
     free(pcs);
  }

  if(Programa == 27){
// Ejercicio 1.b) entero
// Toma los argumentos por consola como el problema 1.a) pero con cantidad de probas en lugar de presicion
    printf("Ejecutando simulacion ejercicio 1.b)\n");
    int *n, i,j, *red, m, It, *secs, secsTot;
    double *F, *pcs;
    n = (int *) malloc((argc)*sizeof(int));
    pcs = (double *) malloc((argc-4)*sizeof(double));
    red = (int *) malloc(sizeof(int));
    sscanf(argv[2], "%d", &m); // Cantidad de probas en el histograma
    sscanf(argv[3], "%d", &It); // Cantidad de iteraciones
    FILE* fp = fopen("Ejercicio_1_b.txt","a");
    fprintf(fp, "Simulacion con %d probabilidades y %d iteraciones por red\n", m, It);
    fprintf(fp, "Los resultados son: \n");
    secs = (int *) malloc((argc-4)*sizeof(int));
    secsTot = time(NULL);
    for(i=4;i<argc;i++){  // Tomo el vector de dimensiones
      sscanf(argv[i], "%d", &n[i-4]); // Tomo las dimensiones de las redes
      red = (int *) realloc(red, n[i-4]*n[i-4]*sizeof(int));
      printf("Arranco %d\n", n[i-4]);
      secs[i-4] = time(NULL);
      F = histograma(red, n[i-4], m, It);
      pcs[i-4] = percentil(F,m,0.5); // Esta operacion deberia tomar un tiempo ~log2(m) y por lo tanto lo que tarda es despreciable frente a histograma
      secs[i-4] = time(NULL)-secs[i-4];
      fprintf(fp, "%dx%d con pc = %g  (en %dhs, %dmin, %dsegs) \n", n[i-4],n[i-4],pcs[i-4],secs[i-4]/3600,secs[i-4]/60 % 60,secs[i-4] % 60);
      for(j=0;j<m-1;j++){
        fprintf(fp, "%g, ", F[j]);
      }
      fprintf(fp, "%g\n",F[m-1]);
      free(F);
    }
    secsTot = time(NULL)-secsTot;int mins = secsTot/60;int horas = mins/60;
    fprintf(fp, "Duracion total: %d horas, %d minutos y %d segundos\n\n", horas, mins % 60, secsTot % 60);
    fclose(fp);
    free(n);
    free(red);
    free(secs);
    free(pcs);
  }

//Ejercicio 1.c)
//Toma por consola lo mismo que el ejercicio 1 
if (Programa ==28){
printf("Ejecutando simulacion ejercicio 1c)\n");
FILE* fp = fopen("Ejercicio_1_c.txt","a");
    fprintf(fp, "Ajuste del Pc en funcion de sigma del Ejercicio 1a y a su vez el parametro nu\n");
    fprintf(fp, "Los resultados son: \n");
    int *red,*n, i,pres,It,l;
    double *pcs, *vars,*sigma,aj,bj,*j,*L,*a;
    a = (double *) malloc(5*sizeof(double));
    n = (int *) malloc((argc)*sizeof(int));
    pcs = (double *) malloc((argc-4)*sizeof(double));
    vars = (double *) malloc((argc-4)*sizeof(double));
    j = (double *) malloc((argc-4)*sizeof(double));
    L = (double *) malloc((argc-4)*sizeof(double));
    sigma = (double *) malloc((argc-4)*sizeof(double));

    red = (int *) malloc((argc-4)*(argc-4)*sizeof(int));
    sscanf(argv[2], "%d", &pres); // Precision 1/2^pres
    sscanf(argv[3], "%d", &It); // Cantidad de iteraciones
    for(i=4;i<argc;i++){  // Tomo el vector de dimensiones
      sscanf(argv[i], "%d", &n[i-4]);
      red = (int *) realloc(red, n[i-4]*n[i-4]*sizeof(int));
      pc_promedio(red,n[i-4],pres, pcs+i-4, vars+i-4, It);		//hasta aca lo copie del 1 le saque lo de los segundos no se porque ajja
      printf("Red de %dx%d terminado\n", n[i-4], n[i-4]);
      
	  }
   	
    for (l=4;l<argc;l++){ // aca quiero  hacer la raiz cuadrada de los sigma tengo un problema con la dimension,
	 sigma[l-4]=sqrt((double)vars[l-4]); // poruqe depende de la cantidad de redes  que le de,no se si lo que hice hace es
   }  
    aj=Ajuste_Lineal(sigma,pcs,argc-4,a,a+1); 	//aca hago el ajuste
   	//aca hago el chi
    fprintf(fp, "El ajuste lineal es  y= %gx+%g y su chi es %g  \n",*a,*(a+1),aj); //quiero que en los resultado me de el ajuste
    fprintf(fp,"El Pc de una red infinito es %g \n", *(a+1)); // y el pc infinito 
    
    for(i=4;i<argc;i++){	//ahora busco el parametro nu 
       j[i-4]=log((double) *(a+1)- pcs[i-4]);
    	L[i-4]=log( (double) n[i-4]);
       }
    bj=Ajuste_Lineal(j,L,argc-4,a+2,a+3);
    a[4]=(-1.0/((double) *(a+2)));
    
    fprintf (fp, "El parametro nu es %g \n", a[4]);
    fprintf(fp, "\n");
    fclose(fp),
    free(pcs);
    free(L);
    free(j);
    free(sigma);
    free(vars);
    free(red);
    free(n);
    free(a);
  }


if(Programa == 29){
//Ejercicio 1.d)
// Toma por consola la proba minima, la proba maxima, cantidad de probas, la cantidad de iteraciones y el vector de dimensiones
    float *probas, pmin,pmax, mmin;
    double *ns,a,b,*x,*y,tau,aj,ajmin;
    int i,j,k,l, *n, m,It, *red ;
    sscanf(argv[2], "%f", &pmin);
    sscanf(argv[3], "%f", &pmax);
    sscanf(argv[4], "%d", &m);
    sscanf(argv[5], "%d", &It);
      FILE* fp = fopen("Ejercicio_1_d.txt","a");
    fprintf(fp, "Simulacion con %d probabilidades y %d iteraciones por red\n", m, It);
    fprintf(fp, "Los resultados son: \n");
    printf("Ejecutando simulacion ejercicio 1.d)\n");
   
    n = (int *) malloc((argc-6)*sizeof(int));
    probas = (float *) malloc(m*sizeof(float));
    red = (int *) malloc(sizeof(int));
    for(i=0;i<m;i++){
      probas[i] = pmin+i*(pmax-pmin)/m;
  
    }

    for(i=0;i<argc-6;i++){
      sscanf(argv[i+6],"%d", &n[i]);
      //printf("aca arranca\n");
      red = (int *) realloc(red, n[i]*n[i]*sizeof(int));
      for(j=0;j<m;j++){
        ns = ns_promedio(red,n[i],probas[j],It);
        int len = 0;
        for (l=1;l<n[i]*n[i];l++){
          if (ns[l]>0){len++;}
        }
        x = (double *) malloc(len*sizeof(double));
        y = (double *) malloc(len*sizeof(double));
        k=0;
       for (l=1;l<n[i]*n[i];l++){
          if(ns[l]>0){
            x[k]=log((double)l);
            y[k]=log((double)ns[l]);
            k++;
          }
        }
        aj=Ajuste_Lineal(x,y,len,&a,&b);
        if ( j==0 || aj<ajmin){
         mmin= a;
         ajmin=aj;
        }
        free(ns);
        free(x);
        free(y);  
      }
      tau=mmin; 
      fprintf (fp, "El parametro tau   es %g \n", tau);
    }
  }


  if(Programa == 2){
// Ejercicio 2: Intensidad del cluster percolante
// La proba mínima, la proba máxima, la cantidad de probas a tomar entre 0 y 1, la cantidad de iteraciones  
// y el vector de dimensiones se toman, en orden, directo de la consola al correr el programa
    printf("Ejecutando simulacion ejercicio 2\n");
    int *red,*n, i,j,m,It,*secs,secsTot;
    float pmin, pmax;
    sscanf(argv[2], "%f", &pmin); // Cantidad de probas
    sscanf(argv[3], "%f", &pmax); // Cantidad de iteraciones
    sscanf(argv[4], "%d", &m); // Cantidad de probas
    sscanf(argv[5], "%d", &It); // Cantidad de iteraciones
    n = (int *) malloc((argc-6)*sizeof(int));
    secs = (int *) malloc((argc-6)*sizeof(int));
    red = (int *) malloc(sizeof(int));
    FILE *fp = fopen("Ejercicio_2.txt","a");  // Escribo los resultados en un archivo
    fprintf(fp, "Simulacion con %d probabilidades (salto de %f) entre %f y %f y %d iteraciones en cada una\n",m,1.0/(m-1),pmin,pmax,It);
    fprintf(fp, "Los resultados son: \n");
    secsTot = time(NULL);
    for(i=6;i<argc;i++){  // Tomo el vector de dimensiones
      sscanf(argv[i], "%d", &n[i-6]);
      red = (int *) realloc(red,n[i-6]*n[i-6]*sizeof(int));
      secs[i-6] = time(NULL);
      double* Fp = intensidad(red, n[i-6],pmin,pmax,m, It);
      secs[i-6] = time(NULL)-secs[i-6];
      printf("%d terminado \n", n[i-6]);
      fprintf(fp, "%dx%d en %dhs, %dmin, %dsegs\n", n[i-6],n[i-6],secs[i-6]/3600,secs[i-6]/60 % 60,secs[i-6] % 60);
      for(j=0;j<m-1;j++){
        fprintf(fp, "%g, ", Fp[j]);
      }
      fprintf(fp, "%g\n",Fp[m-1]);
      free(Fp);
    }
    secsTot = time(NULL)-secsTot;int mins = secsTot/60;int horas = mins/60;
    fprintf(fp, "Duracion total: %d horas, %d minutos y %d segundos\n", horas, mins % 60, secsTot % 60);
    fprintf(fp, "\n");
    fclose(fp);
    free(red);
    free(n);
    free(secs);
  }

  if(Programa == 3){
// Ejercicio 3: Dimension fractal
// La cantidad de iteraciones, el vector de dimensiones  y el vector de pc
// se toman, en orden, directo de la consola al correr el programa
    printf("Ejecutando simulacion ejercicio 3\n");
    int *red,*n, i,It,*secs,secsTot,len = (argc-3)/2;
    float *probas; 
    double *masas, m, b, chi, *logL;
    sscanf(argv[2], "%d", &It); // Cantidad de iteraciones
    n = (int *) malloc(len*sizeof(int));
    masas = (double *) malloc(len*sizeof(double));
    logL = (double *) malloc(len*sizeof(double));
    probas = (float *) malloc(len*sizeof(float));
    secs = (int *) malloc(len*sizeof(int));
    red = (int *) malloc(sizeof(int));
    FILE *fp = fopen("Ejercicio_3.txt","a");  // Escribo los resultados en un archivo
    fprintf(fp, "Simulacion con %d iteraciones en cada una por red \n", It);
    fprintf(fp, "Los resultados son: \n");
    secsTot = time(NULL);
    for(i=0;i<len;i++){  // Tomo el vector de dimensiones y probas
      sscanf(argv[i+3], "%d", &n[i]);
      sscanf(argv[i+3+len], "%f", &probas[i]);
      red = (int *) realloc(red,n[i]*n[i]*sizeof(int));
      secs[i] = time(NULL);
      masas[i] = dimension_fractal(red,n[i],probas[i], It);
      secs[i] = time(NULL)-secs[i];
      printf("%d terminado \n", n[i]);
      fprintf(fp, "%dx%d con pc=%f -> M = %g \nDuracion: %dhs, %dmin, %dsegs\n", n[i],n[i],probas[i],masas[i],secs[i]/3600,secs[i]/60 % 60,secs[i] % 60);
    }
    secsTot = time(NULL)-secsTot;int mins = secsTot/60;int horas = mins/60;
    fprintf(fp, "El vector M(L) es:");
    for(i=0;i<len-1;i++){
      fprintf(fp, "%g, ", masas[i]);
      masas[i]  = log(masas[i]/(n[i]*n[i]));
      logL[i]  = log((double) n[i]);
    }
    fprintf(fp, "%g\n", masas[len-1]);
    masas[i]  = log(masas[i]/(n[len-1]*n[len-1]));
    logL[i]  = log((double) n[len-1]);
    chi = Ajuste_Lineal(logL,masas,len,&m,&b);
    fprintf(fp, "Para un Chi^2 = %g, el ajuste arroja un D = %g\n", chi, 2+m);
    fprintf(fp, "Duracion total: %d horas, %d minutos y %d segundos\n", horas, mins % 60, secsTot % 60);
    fprintf(fp, "\n");
    fclose(fp);
    free(red);
    free(n);
    free(probas);
    free(masas);
    free(secs);
    free(logL);
  }

  if(Programa == 4){  // Testie un poco y esto requiere It>3000 para ser algo distinto a ruido
    // La funcion se hace 0 para p<0.28 y p>0.79 para pc=0.59, asi que un rango [0.25; 0.8]
// Ejercicio 4: Scaling
// Toma el pmin, el pmax, la cantidad de puntos, la cantidad de iteraciones,
// la probabilidad critica de L= 64 y un vector de tamaños de cluster 
    printf("Ejecutando simulacion ejercicio 4\n");
    int *S, i,j,It,m,*secs,secsTot,len = argc-7;
    float *probas, pc, pmin, pmax; 
    sscanf(argv[2], "%f", &pmin); // Proba minima
    sscanf(argv[3], "%f", &pmax); // Proba maxima
    sscanf(argv[4], "%d", &m); // Cantidad de probas
    sscanf(argv[5], "%d", &It); // Cantidad de iteraciones
    sscanf(argv[6], "%f", &pc); // Cantidad de iteraciones
    S = (int *) malloc(len*sizeof(int));
    probas = (float *) malloc(m*sizeof(float));
    secs = (int *) malloc(len*sizeof(int));
    for(i=0;i<m;i++){
      probas[i] = pmin+i*(pmax-pmin)/(m-1);
    }
    FILE *fp = fopen("Ejercicio_4.txt","a");  // Escribo los resultados en un archivo
    fprintf(fp, "Simulacion con %d probas entre %f y %f, tomando pc = %f y %d iteraciones en cada una por cada tamaño de cluster \n", m, pmin, pmax,pc, It);
    fprintf(fp, "Los resultados son: \n");
    secsTot = time(NULL);
    for(i=0;i<len;i++){  // Tomo el vector de dimensiones y probas
      sscanf(argv[i+7], "%d", &S[i]);
      secs[i] = time(NULL);
      double* Fz = scaling(probas, m, S[i], pc, It);
      secs[i] = time(NULL)-secs[i];
      printf("Clusters de tamaño %d terminado \n", S[i]);
      fprintf(fp, "Clusters de tamaño %d (en %dhs, %dmin, %dsegs)\n", S[i], secs[i]/3600,secs[i]/60 % 60,secs[i] % 60);
      for(j=0;j<m-1;j++){
        fprintf(fp, "%g, ", Fz[j]);
      }
      fprintf(fp, "%g\n", Fz[m-1]);
      free(Fz);
    }
    secsTot = time(NULL)-secsTot;int mins = secsTot/60;int horas = mins/60;
    fprintf(fp, "Duracion total: %d horas, %d minutos y %d segundos\n", horas, mins % 60, secsTot % 60);
    fprintf(fp, "\n");
    fclose(fp);
    free(probas);
    free(secs);
    free(S);
  }

  if (Programa==5){
// Ejercicio 5: Sigma
// Toma la roba minima, el pc (proba maxima, la cantidad de probas, la cantidad de iteraciones y 
// un vector de tamaños de clusters
    printf("Ejecutando simulacion ejercicio 5\n");
    int *S, i,It,m,*secs,secsTot,len = argc-6;
    float pc,pmin;
    double *probas, *logprobas, *logS, sigma, zmax,chi;
    sscanf(argv[2], "%f", &pmin); // Proba minima
    sscanf(argv[3], "%f", &pc); // Proba critica
    sscanf(argv[4], "%d", &m); // Cantidad de probas
    sscanf(argv[5], "%d", &It); // Cantidad de iteraciones
    S = (int *) malloc(len*sizeof(int));
    probas = (double *) malloc(len*sizeof(double));
    logprobas = (double *) malloc(len*sizeof(double));
    logS = (double *) malloc(len*sizeof(double));
    probas = (double *) malloc(len*sizeof(double));
    secs = (int *) malloc(len*sizeof(int));
    FILE *fp = fopen("Ejercicio_5.txt","a");  // Escribo los resultados en un archivo
    fprintf(fp, "Simulacion con %d probas entre %f y pc=%f, tomando %d iteraciones en cada una por cada tamaño de cluster \n", m,pmin,pc, It);
    fprintf(fp, "Los resultados son: \n");
    secsTot = time(NULL);
    for(i=0;i<len;i++){  // Tomo el vector de dimensiones y probas
      sscanf(argv[i+5], "%d", &S[i]);
      secs[i] = time(NULL);
      probas[i] = maximizar_cluster(S[i], m,pmin, pc, It);
      logprobas[i] = log(pc-probas[i]);
      logS[i] = log((double) S[i]);
      secs[i] = time(NULL)-secs[i];
      printf("Clusters de tamaño %d terminado \n", S[i]);
      fprintf(fp, "Clusters de tamaño %d tienen pmax=%g (en %dhs, %dmin, %dsegs)\n", S[i],probas[i], secs[i]/3600,secs[i]/60 % 60,secs[i] % 60);
    }
    chi = Ajuste_Lineal(logS,logprobas,len,&sigma,&zmax);
    sigma = -sigma;
    zmax = -exp(zmax);
    fprintf(fp, "El ajuste arroja sigma = %g y zmax=%g con un chi=%g \n", sigma, zmax, chi);
    fprintf(fp, "El vector de pmax es: \n%g", probas[0]);
    for(i=1;i<len;i++){
      fprintf(fp, " ,%g", probas[i]);
    }
    secsTot = time(NULL)-secsTot;int mins = secsTot/60;int horas = mins/60;
    fprintf(fp, "\nDuracion total: %d horas, %d minutos y %d segundos\n", horas, mins % 60, secsTot % 60);
    fprintf(fp, "\n");
    fclose(fp);
    free(probas);
    free(secs);
    free(logS);
    free(logprobas);
    free(S);
  }

 
if(Programa ==6){
// tomo proba minima , tomo proba maxima, tomo la cantidad de probas en las que voy   a partir, cantidad de iteraciones, y dimencion de la red
    float *probas, pmin,pmax;
    double *ns,*ms,msj ,*a ,msmax,*b,*x,*y,aj,*pnd,*p;
    int i,j,l, *n, m,It, *red ;
     sscanf(argv[2], "%f", &pmin);   // proba minima 
     sscanf(argv[3], "%f", &pmax);   //proba maxima
     sscanf(argv[4], "%d", &m);      //cantidad de probas
     sscanf(argv[5], "%d", &It);	//iteraciones
     // FILE* fp = fopen("Ejercicio_6.txt","a");
    //fprintf(fp, "Simulacion con probabilidades desde %d a %d con %d particiones, tomando %d 				iteraciones por red\n",pmin,pmax, m, It);
   // fprintf(fp, "Los resultados son: \n");
    printf("Ejecutando simulacion ejercicio 6)\n");
    n = (int *) malloc((argc-6)*sizeof(int));
    probas = (float *) malloc(m*sizeof(float));
    x = (double *) malloc (2* sizeof(double));
    y = (double *) malloc (2* sizeof(double));
    ms = (double *) malloc (m* sizeof(double));
    p = (double *) malloc (m* sizeof(double));
    pnd = (double *) malloc (m* sizeof(double));
    red = (int *) malloc(sizeof(int));
    for(i=0;i<m;i++){						//primero tomo vector de probas [m]
      probas[i] = pmin+i*(pmax-pmin)/(m-1);
	  }
    for(i=0;i<argc-6;i++){					//aca entiendo que voy a correr las distintas	
      sscanf(argv[i+6],"%d", &n[i]);				//redes
      red = (int *) realloc(red, n[i]*n[i]*sizeof(int));	 // genero la red [n]
      for(j=0;j<m;j++){						//corro las probas
        msj=0;							//asigno aca msj=0 asi a cada proba lo va reiniciando
	ns= ns_promedio_sin_percolante(red,n, p,It);
        for(l=0;l<n[i]*n[i];l++){				 // genero ms[j]  para cada proba
          ns[l]=(ns[l]/((double)(n[i]*n[i])));			 //aca estoy normalizando     
          msj=msj+ns[l]*l*l;
          }
        ms[j]=msj;				 // genero vector ms[j]  
        free(ns);
        }
 }
   for (i=0;i<m;i++){	
	if(i==0 || ms[i]>msmax){
		pmax=probas[i];
		msmax=ms[i];
		}
 	   
   for (i=0;i<m;i++){
	        if(p[i]<pmax){
           pn[i]=a;       // si etoy del lado negativo lo pongo en un vector pendientes negativas
           probn[i]=p[i]; // y le asigno ese valor de probas  a un vector probasnegativas
	   //aj=Ajuste_Lineal(*pn,*probn,i, &a, &b); // se que esto esta mal ...
                 }
	   aj[]=Ajuste_Lineal(*pn,*probn,i, &a, &b); // se que esto esta mal ... por hay aca esta bien 
           
           else{
            pp[i]=a;
            probp[i]=p[i];
            //bj=Ajuste_Lineal(*pp,*probp,i, &a, &b); // se que esto esta mal ...
               }
	     bj[]=Ajuste_Lineal(*pp,*probp,i, &a, &b); // se que esto esta mal ...por hay aca esta bien 
           
          }
          
	// MI IDEA  aca es que me saque 4 vectores  no se si lo estoy haceindo bien 
          for (i)
		x[i]=p
            for(j){
            
            }

        /* if(pmax<probas[i]){
	     p[i]=probas[i]-pmax;

		    }
  	    else
		    { p[i]=pmax-probas[i]; }	
	      } 
	//tengo un vecto con las pendientes pnd[m]  y un vector con las p[m]=|p[i]-pmax|, me falta pensar como le pido que compare correctamente
  */}
 free(red);
 free(n);
 free(pnd); 
 free(p);
 free(probas);

 }

  return 0;
}

//  FUNCIONES DE EJERCICIOS


// 1.a)
void pc_promedio(int* red, int n, int P, double* p, double* var, int It){   // Cuando queremos que algo nos devuelva 2+ variables, un buen truco es crearlas fuera de la funcion 
// (con cualquier valor) y pasarle a la funcion punteros a esa variables, cosa que la funcion se encargue de poner en esa direccion de memoria el valor correcto
// Si no te gusta, tranquilamente podemos devolver un array de 2 posiciones, pero personalmente preferiria utilizar la menor cantidad de memoria dinamica posible.
  double prob, step;
  int *clase;
  int k,l;
  *p = 0;
  *var = 0;
  for(k=0;k<It;k++){   // Itero It veces
    step=.25;          // Tamaño del paso inicial
    prob = 0.5;       // Probabilidad inicial
    for (l = 0; l<P; l++){
      clase=hoshenVec(red,n,prob);  // Aca es un bajon que clase no tenga utilidad. Si te sirve de consuelo, calcular clase no hace esto apreciablemente lento.
      free(clase);  
      if (percola(red,n)>0){
        prob=prob-step;     // Si percola, el pc debe estar mas abajo
      }
      else{
        prob=prob+step;     // Si no, debe estar mas arriba
      }
      step = .5*step; // Actualizo el tamaño del paso
    }

    *p = *p+prob/(double)It;           // Voy sumando los prob para obtener el prob promedio luego
    *var = *var+prob*prob/(double) It;  // Voy sumando los prob^2 para obtener la varianza luego
  }
  *var = *var-(*p)*(*p);   // Divido para obtener el promedio de los prob^2 y le resto el promedio al cuadrado -> Varianza!
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


int   percola(int *red,int n){      // Devuelve 0 si no percolo y la etiqueta de un cluster percolante si hubo percolacion
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
  	if (e>0 && e<n && etiquetas[e]){ // Me fijo si la etiqueta esta en rango y luego si esa
  		res = e;		                   // ya aparecio en la 1° fila. Si lo hizo, la devuelvo.
  		break;		// Como ya encontre coincidencia, termino el ciclo (¡con toda violencia!)
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
int* numeros_cluster(int n, int *clase){ 
  int i ,j;      
  int* res;
  res=(int *)malloc(n*n*sizeof(int)); // Creamos el resultado; un vector cuya i-esima posicion es la cantidad de clusters de tamaño i
  for(i=0;i<n*n;i++){       // Lo inicializamos en 0
    res[i]=0;
  }
  for (j=2;j<n*n;j++){  // Cada etiqueta (verdadera) i que nos crucemos tiene un tamaño clase[i], por lo que debemos sumar 1 a res[clase[i]]
    if(clase[j]>0){ // Obviamente, no queremos contar etiquetas falsas
      res[clase[j]]++;
    }
  }   
  return res;
}

// Ejercicio 1.b)

double* histograma(int* red,int n,int m, int It){
  int i,j;
  float p;
  double *res;
  int *clase;
  // probas=(float *)malloc(m*sizeof(float));       // aca la idea es armar un vector de m lugares por eso use el malloc para cada una de las probabilidades
  // Dado que las probas no son argumento de entrada ni de salida, crear este vector no tendría mucha utilidad. En todo caso, podemos reconstruirlo conociendo m
  res = (double *) malloc(m*sizeof(double)); // Como cantperco era un múltiplo de res, me ahorro crear ese vector y lo laburo todo sobre res.
  for (j=0;j<m;j++){             // primero  hago un for para las probabilidades
    p=(j+1.0)/m; // Me aseguro de tener p=1 y no p=0
    if(j==m/2){printf("Por la mitad\n");}
    int cperco=0;
    for(i=0;i<It;i++){    // Ahora quiero hacer correr  el echo de que  para cada proba itere IT veces;
      clase=hoshenVec(red,n,p);
      free(clase);
      if (percola(red,n)>0){ // la idea es si percola que sume uno 
        cperco++;     
      }
    //probas[j]=p;    // aca mi idea es que tire el vector de probas
    }
    res[j]=(double)cperco/(double)It;  // aca la idea seria que tire el vector con  la  cantidad de percola para cada casillero --- Me ahorre el siguiente for dividiendo in-situ     
  }
  return res;
}

double percentil(double* F, int m, float alfa){ // Dado un array F y su longitud m, busca el indice i tal que F[i]=alfa
  int inf, sup, med; // A lo largo del ciclo, se que el indice que busco (i) esta entre inf y sup (inf<=i<=sup)
  inf = 0;
  sup = m-1;
  med = (inf+sup)/2;
  while(inf+1<sup){
    med = (inf+sup)/2;
    if (F[med]>alfa){   // Si F[med]>alfa, entonces el indice que busco esta entre inf y med
      sup = med;
    }else{    // Si F[med]>alfa, entonces el indice que busco esta entre med y sup
      inf = med;
    }
  }
  // return (med+1.0)/m; // Sabiendo que los p estan equiespaciados en (0,1], puedo obtener el p sabiendo el indice 
  return (med+1.0+(alfa-F[med])/(F[med+1]-F[med]))/m; // Este es como el de arriba pero además interpola (estoy abusando el hecho de que med<m si alfa<1)
}


// Ejercicio 2


double* intensidad(int* red, int n, float pmin, float pmax, int m, int It){
  int i,j,perc, *clase;
  double *res, p=pmin;
  res=(double *)malloc(m*sizeof(double));
  for(i=0;i<m;i++){ 
    p = pmin+i*(pmax-pmin)/(double)(m-1);
    res[i] = 0;
    for(j=0;j<It;j++){
      clase = hoshenVec(red,n,p);  
      perc = percola(red,n);   
      if (perc>0){
        res[i] = res[i]+(double)clase[perc]/(double) It; 
      }
      free(clase);
    }
    res[i] = res[i];
  }
  return res;
}


float* mediana_bisec(int* n, int m, int It, int pres){
  int i,j,k,cantperc, *clase, *red;
  float *res,step,prob;
  red = (int *) malloc(n[0]*n[0]*sizeof(int));
  res=(float *)malloc(m*sizeof(float));  // Cada res[i] es la pc de la red de n[i]xn[i]
  for(i=0;i<m;i++){ // Calculo el pc para la red de lado n[i]
    step = 0.25;  // Paso inicial
    prob = 0.5;   // Probabilidad inicial
    printf("Casi creamos la red -> %d \n", n[i]);
    red = (int *)realloc(red,n[i]*n[i]*sizeof(int));  // Seteo las dimensiones de la red
    printf("Creamos la red \n");
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
    printf("Borro la red");
    res[i] = prob; // Al terminar, asigno el pc con un error de 1/2^pres
  }
  free(red);
  return res;
}

double chi(double* x,double* y,int m){
  int i;
  double res=0;
  for(i=0;i<m;i++){
      res=res + (y[i]-x[i])*(y[i]-x[i]);
    }
  res = res/(double)(m-2);
    return res;
  }


double* ns_promedio(int* red, int n, float p, int It){
  srand((unsigned) time(NULL));
  double *res;
  int *clase;
  int i,j, *clusters;
  res = (double *) malloc(n*n*sizeof(double));   // El tamaño de res esta definido por el tamaño de la red
  for(j=0;j<n*n;j++){
    res[j] = 0;  // Inicializo el vector en 0
  }
  for(i=0;i<It;i++){
    clase = hoshenVec(red,n,p);
    clusters = numeros_cluster(n,clase);
    free(clase);
    for(j=0;j<n*n;j++){
      res[j] = res[j]+(double)clusters[j]/(double) It;  // Le voy sumando los clusters de tamaño j a medida que los veo
    }
    free(clusters);
  } 
  return res;
}

double Ajuste_Lineal(double* x, double* y, int n, double* m, double* b){
  double sumaxy, sumay,sumax, sumaxx, res, *Yaj;
  int i;
  sumaxy = 0;
  sumay = 0;
  sumax = 0;
  sumaxx = 0;
  for(i=0;i<n;i++){
    sumax = sumax+x[i];
    sumaxy = sumaxy+x[i]*y[i];
    sumay = sumay+y[i];
    sumaxx = sumaxx+x[i]*x[i];
    //printf("%g  %g", x[i], y[i]);
  }
  //printf("%g\n", n*sumaxy-sumax*sumay);
  //printf("%g\n", n*sumaxx-sumax*sumax);
  *m = (sumaxy-sumax*sumay/(double)n)/(sumaxx-sumax*sumax/(double)n);  // Calculo la pendiente 
  *b = (sumay-(*m)*sumax)/(double)n;                        // ordenada del ajuste
  Yaj = (double *) malloc(n*sizeof(double));
  for(i=0;i<n;i++){
    Yaj[i] = (*m)*x[i]+(*b);   // Obtengo el vector de f(x) del ajuste
  }
  res = chi(y,Yaj,n);  // Calculo el error cuadratico entre los puntos y el ajuste 
  free(Yaj);          // y lo devuelvo
  return res;
}


// Ejercicio 3

double dimension_fractal(int* red, int N, float pc, int It){
  double res=0;
  int i=0,perc,*clase;
  srand((unsigned) time(NULL));
  while(i<It){
    clase = hoshenVec(red,N,pc);
    perc = percola(red,N);
    if(perc>0){ // Me aseguro que haya percolado, sino lo repito
      i++;
      res = res+(double)clase[perc]/(double) It; // Sumo la masa del cluster percolante
    }
    free(clase);
  }
  return res;
}


// Ejercicio 4

 double* scaling(float* probas, int m, int S, float pc, int It){ // Dado un 41<=S<=492, obtiene f(z)=ns(p)/ns(pc)
  double *res;    // Asumo que L = 64
  int j; 
  res = (double *) malloc(m*sizeof(double));
  int* red = (int *) malloc(64*64*sizeof(int)); // Creo la red sobre la cual voy a simular
  double* ns;
  ns = ns_promedio(red,64,pc,It); // Obtengo el ns(pc) que voy
  double nspc = ns[S];            // a usar para "normalizar"
  free(ns);
  for(j=0;j<m;j++){
    ns = ns_promedio(red, 64, probas[j], It); // Calculo el ns(probas[j])
    res[j] = ns[S]/nspc;          // y lo normalizo por ns(pc)
    free(ns);
  }
  free(red);
  return res;
 }


// Ejercicio 5
// Toma un tamaño de cluster, una cantidad de probas a tomar entre 0 y pc, el pc de 64x64 y la cantidad 
// de iteraciones. Devuelve la proba que maximiza ns (a S fijo es equivalente a maximizar f)
double maximizar_cluster(int S, int m, double pmin, double pc, int It){
  double res, *ns, proba, nsmax; 
  int *red,i,j=0;
  red = (int *) malloc(64*64*sizeof(int)); // Creo la red sobre la cual voy a simular
  nsmax = 0;
  res=0;
  for(i=0;i<m;i++){   // Tomo m probas entre pmin y pc
    proba = pmin+i*(pc-pmin)/(m-1);
    ns = ns_promedio(red,64,proba, It);
    if(ns[S]>nsmax){   // Actualizo nsmax
      nsmax = ns[S];
      res = proba;
      j=0;
    }else{
      j++;
      if(10*j>m){ // Si estuve mas del 10% de las probas sin actualizar, asumo que es 
        free(ns);
        break;    // el maximo y corto por lo sano
      }
    }
    free(ns);
  }
  free(red);
  return res;
}


// Ejercicio 6

double* ns_promedio_sin_percolante(int* red, int n, float p, int It){
  srand((unsigned) time(NULL));
  double *res;
  int *clase, etiquetas[n], etiquetasUsadas[n],e;
  int i,j,k, *clusters;
  res = (double *) malloc(n*n*sizeof(double));   // El tamaño de res esta definido por el tamaño de la red
  for(j=0;j<n*n;j++){
    res[j] = 0;  // Inicializo el vector en 0
  }
  for(i=0;i<It;i++){
    clase = hoshenVec(red,n,p);
    clusters = numeros_cluster(n,clase);
  
    for(k=0;k<n;k++){  // Este es el bloque donde descuento el percolante, es casi identico a percola,
      etiquetas[k]=0;  // con la única diferencia de que sigue buscando hasta encontrar todos los percolantes
      etiquetasUsadas[k]=0; // Acá voy a recordar que etiquetas ya conté (ver más adelante)
    }
    for(k=0;k<n;k++){
      etiquetas[*(red+k)] = 1;  // pongo 1 si esa etiqueta esta en la primer fila
    }
    for(k=0;k<n;k++){
      e = red[n*(n-1)+k];
      if (e>0 && e<n && etiquetas[e] && etiquetasUsadas[e]==0){ // Me fijo si la etiqueta esta en rango y luego si esa
        clusters[clase[e]]--;           // ya aparecio en la 1° fila y no apareció aún en la última. Si esto se cumple, 
        etiquetasUsadas[e]=1;           // tengo que descontarla y marcarla como ya vista
      }
    }

    for(j=0;j<n*n;j++){
      res[j] = res[j]+(double)clusters[j]/(double) It;  // Le voy sumando los clusters de tamaño j a medida que los veo,
    }                                                 // pero con los clusters percolantes ya descontado
    free(clusters);
    free(clase);
  } 
  return res;
}


/* Acá abajo pongo una lista de funciones (con declaracion tentativa) que faltaría hacer. Agreguemos a medida que se nos ocurran.




*/
