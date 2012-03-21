#include <stdio.h>
#include <stdlib.h>

#include <math.h>
#include <matheval.h>

int main(int argc,char** argv)
{
	char fm[256];
	memset((char*)fm,0,256);
	
	printf("\nVýpočet odchylky veličin daných explicitním vzorcem (v. 1.0)\n============================================================\n\n");
	
	printf("Zadejte základní vzorec pro výpočet výsledné veličiny:\n");
	fgets((char*)fm,255,stdin);
	fm[strlen((char*)fm)-1] = 0;
	
	void* formula = evaluator_create((char*)fm);
	if (!formula){
		printf("Byl zadán neplatný vzorec!\n");
		return 1;
	}
	
	char** var_names;
    int count;
     
    evaluator_get_variables (formula, &var_names, &count);
    double* dev = (double*)malloc(sizeof(double)*count);
    double* val = (double*)malloc(sizeof(double)*count);
    
    char number[50];
	double TotalSigma;
    
    while (1){
		printf("\nZadejte hodnotu a odchylku pro jednotlivé dílčí veličiny (nebo nuly pokud výsledná odchylka na dané veličině nezávisí):\n\n"); 
		
		memset(dev,0,sizeof(double)*count);
		memset(val,0,sizeof(double)*count);
		TotalSigma = 0;
		
		for (int i = 0;i < count;++i){
		  memset((char*)number,0,50);
		  printf("%s = ",var_names[i]);
		  fgets((char*)number,50,stdin);
		  val[i] = atof((char*)number);
		  
		  memset((char*)number,0,50);
		  printf("Sigma %s = ",var_names[i]);
		  fgets((char*)number,50,stdin);
		  dev[i] = atof((char*)number); 
		  
		  printf("\n");
		}
		
		for (int i = 0;i < count;++i){
		   void* df = evaluator_derivative(formula,var_names[i]);  
		   TotalSigma += pow(dev[i],2)*pow(evaluator_evaluate(df,count,var_names,val),2);
		   evaluator_destroy(df);
		}
		
		printf("Hodnota výsledné veličiny je: %f\n",evaluator_evaluate(formula,count,var_names,val));
		printf("Odchylka výsledné veličiny je: %f\nBear loves Crocodile!\n",sqrt(TotalSigma));
		
		printf("\nPokračovat s novým zadáním do stejného vzorce? (A/N)\n");
		if (fgetc(stdin) != 'A')
		  break;
		fgetc(stdin);
	}
    evaluator_destroy(formula);
    free(dev);
	free(val);
	
	return 0;
}
						
