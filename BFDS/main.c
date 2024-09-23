#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

void DebugCotacoes(double bitcoin, double ethereum, double ripple);

typedef struct Client
{

    char Cpf[11];
    char Senha[20];

    double Reais;
    double Bitcoin;
    double Ethereum;
    double Ripple;

}Client, *Cpointer;

typedef struct Moeda Moeda;
struct Moeda
{

    double Valor;
    double TaxaVenda;
    double TaxaCompra;

};

int main(int argc, char *argv[]) 
{
    setlocale(LC_ALL, "portuguese");
    
    FILE *pTxtCotacoes, *pTxtUsers;
    const char *Cotacoes = "Cotacoes.txt", *Users = "Users.txt";

    char UserCpf[11] = "";
    char UserSenha[20] = "";
    char Leitor[255];
	
    Moeda Bitcoin;
    Moeda Ethereum;
    Moeda Ripple;
    
    Bitcoin.TaxaCompra = 0.02;
    Bitcoin.TaxaVenda = 0.03;
    Ethereum.TaxaCompra = 0.01;
    Ethereum.TaxaVenda = 0.02;
    Ripple.TaxaCompra = 0.01;
    Ripple.TaxaVenda = 0.01;

    Cpointer pClients;
    pClients = (Cpointer)calloc(10, sizeof(Client));
    if(pClients == NULL) return 1;

    pTxtCotacoes = fopen(Cotacoes, "r");
    if(pTxtCotacoes == NULL) return 2;
    int index = 0;
    while(!feof(pTxtCotacoes)){
        fgets(Leitor, sizeof(Leitor), pTxtCotacoes);
        index ? sscanf(Leitor, "%lf,%lf,%lf", &Bitcoin.Valor, &Ethereum.Valor, &Ripple.Valor) : index++;
    }
    fclose(pTxtCotacoes);
    DebugCotacoes(Bitcoin.Valor, Ethereum.Valor, Ripple.Valor);
    
    pTxtUsers = fopen(Users, "r");
    if(pTxtUsers == NULL) return 3;
    index = 0;
    while (!feof(pTxtCotacoes)){
        fgets(Leitor, sizeof(Leitor), pTxtUsers);
        //if(index) sscanf(Leitor, "%s,%s,%lf,%lf,%lf,%lf", pClients[index-1] &Bitcoin.Valor, &Ethereum.Valor, &Ripple.Valor);
        index++;
    }
    

    fclose(pTxtUsers);

    system("pause");	
    //fclose(pTxtUsers);
    //fclose(pTxtCotacoes);
    free(pClients);
	return 0;

}

void DebugCotacoes(double bitcoin, double ethereum, double ripple){
    printf("Valores das criptos:\n\n   Bitcoin,  Ethereum,    Ripple;\n%10.2lf,%10.2lf,%10.2lf;\n", bitcoin, ethereum, ripple);
}

void DebugUser(char Cpf[11],char Senha[20],double Reais,double Bitcoin,double Ethereum,double Ripple){
    printf("Valores do user:\n\n                 Cpf,               Senha,               Reais,             Bitcoin,            Ethereum,              Ripple;\n%20s,%20s,%20.2lf,%20.2lf,%20.2lf,%20.2lf", Cpf, Senha, Reais, Bitcoin, Ethereum, Ripple);
}

