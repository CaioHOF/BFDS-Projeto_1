#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

void DebugCotacoes(double bitcoin, double ethereum, double ripple);
void DebugUser(char Nome[21], char Cpf[12],char Senha[7],double Reais,double Bitcoin,double Ethereum,double Ripple);

typedef struct Client
{

    char Nome[21];
    char Cpf[12];
    char Senha[7];

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

    char userCpf[12] = "";
    char userSenha[7] = "";
    char leitor[256];

    int i;
    int index;
	
    Moeda bitcoin;
    Moeda ethereum;
    Moeda ripple;
    
    bitcoin.TaxaCompra = 0.02;
    bitcoin.TaxaVenda = 0.03;
    ethereum.TaxaCompra = 0.01;
    ethereum.TaxaVenda = 0.02;
    ripple.TaxaCompra = 0.01;
    ripple.TaxaVenda = 0.01;

    Cpointer pClients;
    pClients = (Cpointer)calloc(10, sizeof(Client));
    if(pClients == NULL) return 1;

    //isso aqui serve para ter um valor teste coerente com os dados armazenados para usar enquanto não se tem a funcao de pegar os dados
    strcpy(pClients[0].Nome, "Josias");
    strcpy(pClients[0].Cpf, "00000000011");
    strcpy(pClients[0].Senha, "000001");
    pClients[0].Reais = 0;
    pClients[0].Bitcoin = 0;
    pClients[0].Ethereum = 0;
    pClients[0].Ripple = 0;


    pTxtCotacoes = fopen(Cotacoes, "r");
    if(pTxtCotacoes == NULL) return 2;
    fgets(leitor, sizeof(leitor), pTxtCotacoes);
    fgets(leitor, sizeof(leitor), pTxtCotacoes);
    sscanf(leitor, "%lf,%lf,%lf", &bitcoin.Valor, &ethereum.Valor, &ripple.Valor);
    fclose(pTxtCotacoes);
    DebugCotacoes(bitcoin.Valor, ethereum.Valor, ripple.Valor);
    
    pTxtUsers = fopen(Users, "r");
    if(pTxtUsers == NULL) return 3;
    index = 0;
    fgets(leitor, sizeof(leitor), pTxtUsers);
    while (fgets(leitor, sizeof(leitor), pTxtUsers)){
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

void DebugUser(char Nome[21], char Cpf[12],char Senha[7],double Reais,double Bitcoin,double Ethereum,double Ripple){
    printf("| nome: |%20s| |,| Cpf: |%11s| |,| Senha: |%6s| |,| Reais: |%.2lf| |,| Bitcoin: |%.2lf| |,| Ethereum: |%.2lf| |,| Ripple: |%.2lf| |;\n\n", Nome, Cpf, Senha, Reais, Bitcoin, Ethereum, Ripple);
}

void SaveCotacoes(){

}

void SaveUser(){
    
}