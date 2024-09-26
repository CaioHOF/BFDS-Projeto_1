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
    int quantDadosPegosDoSscanf;
	
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

    pTxtCotacoes = fopen(Cotacoes, "r");
    if(pTxtCotacoes == NULL) return 2;
    fgets(leitor, sizeof(leitor), pTxtCotacoes);
    fgets(leitor, sizeof(leitor), pTxtCotacoes);
    quantDadosPegosDoSscanf = sscanf(leitor, "%lf,%lf,%lf", &bitcoin.Valor, &ethereum.Valor, &ripple.Valor);
    fclose(pTxtCotacoes);
    DebugCotacoes(bitcoin.Valor, ethereum.Valor, ripple.Valor);
    
    pTxtUsers = fopen(Users, "r");
    if(pTxtUsers == NULL) return 3;
    index = 0;
    fgets(leitor, sizeof(leitor), pTxtUsers);
    while (fgets(leitor, sizeof(leitor), pTxtUsers)){
        printf("\nleitor:\n%s", leitor);
        quantDadosPegosDoSscanf = sscanf(leitor,"%[^,],%[^,],%[^,],%lf,%lf,%lf,%lf", pClients[index].Nome, pClients[index].Cpf, pClients[index].Senha, &pClients[index].Reais, &pClients[index].Bitcoin, &pClients[index].Ethereum, &pClients[index].Ripple);
        printf("quantDadosPegosDoSscanf: %d\n\n", quantDadosPegosDoSscanf);
        DebugUser(pClients[index].Nome,pClients[index].Cpf,pClients[index].Senha,pClients[index].Reais,pClients[index].Bitcoin,pClients[index].Ethereum,pClients[index].Ripple);
        index++;
    }
    fclose(pTxtUsers);

    system("pause");	
    //SaveUsers(pClients, Users);
    //fclose(pTxtUsers);
    //SaveCotacoes(bitcoin.Valor, ethereum.Valor, ripple.Valor, Cotacoes);
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

void SaveCotacoes(double bitcoin, double ethereum, double ripple, char *nomeArquivo){
    FILE* destino;
    destino = fopen(nomeArquivo, "w");
    fprintf(destino,"Bitcoin,Ethereum,Ripple\n");
    fprintf(destino,"%.2lf,%.2lf,%.2lf\n", bitcoin, ethereum, ripple);
    fclose(destino);
}

void SaveUsers(Cpointer pClients, char *nomeArquivo){
    FILE* destino;
    destino = fopen(nomeArquivo, "w");
    fprintf(destino,"Nome,Cpf,Senha,Reais,Bitcoin,Ethereum,Ripple\n");
    int c;
    for (c = 0; c < 10; c++)
    {
        fprintf(destino, "%s,%s,%s,%.2lf,%.2lf,%.2lf,%.2lf\n", pClients[c].Nome, pClients[c].Cpf, pClients[c].Senha, pClients[c].Reais, pClients[c].Bitcoin, pClients[c].Ethereum, pClients[c].Ripple);
    }
    fclose(destino);
}