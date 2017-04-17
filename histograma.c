float* histograma(int* red, int* n, float* probas, int* m, int It) 
 
Dado un vector de probas (longitud m) y una red de nxn, se fija para 
cada probas[i] si percola It veces, 
poniendo en resultado[i] el porcentaje de veces que percolo


// El vector de probas  con un paso de 0.01, me agarro la duda si esta bien el paso


float* histograma(int* red,int* n,int* m, int It){
int i,j;
float p;

float *probas;
int *clase;
probas=(float *)malloc(m*sizeof(float));       \\ aca la idea es armar un vector de m lugares por eso use el malloc para cada una de las probabilidades
cantperco=(int *)malloc(m*sizeof(int));        \\ aca la idea es armar una tira tambien de m lugares para poner en cada lugar la  cantidad de veces que percola
for (j=0;j<m;j++){			       \\ primero  hago un for para las probabilidades
		p=j/m;
	
		int cperco=0;
		for(i=0;i<It;i++){		\\ Ahora quiero hacer correr  el echo de que  para cada proba itere IT veces;
		  clase=hoshenVec(red,n,p);
		  free(clase);
		      if (percola(*red,n)>0){	\\ la idea es si percola que sume uno 
		      cperco++;			\\
		                 }
		probas[j]=p;		\\ aca mi idea es que tire el vector de probas
		cantperco[j]=cperco;	\\ aca la idea seria que tire el vector con  la  cantidad de percola para cada casillero, 		
	 	}
for(l=0;l<m;l++){
	res[l]= cantperco[l]/It;
		}

/*
Dentro del mismo lo que tenia pensado era que me tire como resultado el valos del pc
para eso mi idea era hacer un while que tome hasta que pase 0.5 y despues me de el
valor de la tira de probas que corresponde a ese valor  
*/
int l=0,a=0;
while(res[l]<0.5){
	pc=probas[l];  \\este va a a ser mi primera opcion para pc
	a=l;  		\\ esta va a  ser la posicion 
	l++; //no se si esto hacce falta dentro del while	
	}	
pcc=probas[a+1]; 	\\tomo la siguiente por las dudas
float m;		\\en todo esto la idea es compara las dos y ver cual es mayor
if(res[a]-0.5<0){
	m=-(res[a]-0.5);
	}
else{	
	m=res[a]-0.5;			
	}
float n;
if(res[a+1]-0.5<0){
	n=-(res[a+1]-0.5);
	}
else{	
	n=res[a+1]-0.5;			
	}
if(m<n){		\\ si la segunda  esta mas cerca entonces cambio el pc
	pc=pcc;	
	}
else{
	pc=pc;		\\sino lo dejo igual esto no hace falta
	}
return pc
free(probas);
return res;
}

