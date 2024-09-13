#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

typedef struct Client Client, *Cp;
struct Client
{
    char VtCpf[11];
    char VtSenha[20];
    double Reais;
    double Ethereum;
    double Bitcoin;
    double Ripple;

};

typedef struct Cotacao Cotacao;
struct Cotacao
{
    double Valor;
    double TaxaVenda;
    double TaxaCompra;
};




int main(int argc, char *argv[]) 
{
    setlocale(LC_ALL, "portuguese");
    char UserCpf[11] = "";
    char UserSenha[20] = ""; 
	Cotacao Bitcoin;
    Cotacao Ripple;
    Cotacao Ethereum;
    Bitcoin.TaxaCompra = 0.02;
    Bitcoin.TaxaVenda = 0.03;
    Ripple.TaxaCompra = 0.01;
    Ripple.TaxaVenda = 0.01;
    Ethereum.TaxaCompra = 0.01;
    Ethereum.TaxaVenda = 0.02;

    system("pause");	
	return 0;
}









