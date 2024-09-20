#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

typedef struct Client
{

    char VtCpf[11];
    char VtSenha[20];

    double Reais;
    double Ethereum;
    double Bitcoin;
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

    int IDUserCpf;
    int IDUserVtSenha;
    int IDUserReais;
    int IDUserEthereum;
    int IDUserBitcoin;
    int IDUserRipple;
    int IDCotacaoEthereum;
    int IDCotacaoBitcoin;
    int IDCotacaoRipple;

    char UserCpf[11] = "";
    char UserSenha[20] = "";
    char Leitor[255];
	
    Moeda Bitcoin;
    Moeda Ripple;
    Moeda Ethereum;
    
    Bitcoin.TaxaCompra = 0.02;
    Bitcoin.TaxaVenda = 0.03;
    Ripple.TaxaCompra = 0.01;
    Ripple.TaxaVenda = 0.01;
    Ethereum.TaxaCompra = 0.01;
    Ethereum.TaxaVenda = 0.02;

    Cpointer pClients;
    pClients = (Cpointer)calloc(10, sizeof(Client));
    if(pClients == NULL) return 1;

    pTxtCotacoes = fopen(Cotacoes, "r");
    if(pTxtCotacoes == NULL) return 2;
    
    pTxtUsers = fopen(Users, "r");
    if(pTxtUsers == NULL) return 3;

    char lixoUm, lixoDois, lixoTres;
    int index = 0;
    while(!feof(pTxtCotacoes)){
        fgets(Leitor, sizeof(Leitor), pTxtCotacoes);
        if(index == 0){
            sscanf(Leitor, "%s,%s,%s", &lixoUm, &lixoDois, &lixoTres);
            index++;
        }
        else{
            sscanf(Leitor, "%lf,%lf,%lf", &Bitcoin.Valor, &Ethereum.Valor, &Ripple.Valor);
        }
    }
    fclose(pTxtCotacoes);


    system("pause");	
    //fclose(pTxtUsers);
    //fclose(pTxtCotacoes);
    free(pClients);
	return 0;
}






