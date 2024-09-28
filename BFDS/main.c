#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <stdbool.h>
#include <unistd.h>

typedef struct Client
{

    char Nome[21];
    char Cpf[12];
    char Senha[7];

    double Reais;
    double Bitcoin;
    double Ethereum;
    double Ripple;

}Client, *CPointer;

typedef struct Moeda
{

    char* Nome;
    double Valor;
    double TaxaVenda;
    double TaxaCompra;

}Moeda, *MPointer;


void DebugCotacoes(double bitcoin, double ethereum, double ripple);
void DebugUser(CPointer pClients, int index);
void SaveCotacoes(MPointer pCriptos, const char *nomeArquivo);
void SaveUsers(CPointer pClients, const char *nomeArquivo);
int PedirSenha(char userSenha[7]);
void ConsultarSaldo();
void ConsultarExtrato();
void DepositarReais();
void SacarReais();
void ComprarCriptomoedas(CPointer pClients, int userIndex, MPointer bitcoin, MPointer ethereum, MPointer ripple);
void VenderCriptomoedas();
void AtualizarCotacoes();
void limparTerminal();

int main(int argc, char *argv[]) 
{
    setlocale(LC_ALL, "portuguese");
    
    FILE *pTxtCotacoes, *pTxtUsers;
    const char *Cotacoes = "Cotacoes.bin", *Users = "Users.bin";

    char userCpf[12] = "";
    char userSenha[7] = "";
    char leitor[256];
    char respostaUser[20];

    int pedirSenha;
    int userIndex = -1;
    int i;
    int index;
    int quantDadosPegosDoSscanf;

    CPointer pClients;
    pClients = (CPointer)calloc(10, sizeof(Client));

    MPointer pCriptos;
    pCriptos = (MPointer)calloc(3, sizeof(Moeda));


    pTxtCotacoes = fopen(Cotacoes, "rb");
    if(pTxtCotacoes == NULL) return 2;
    fread(pCriptos, sizeof(Moeda), 3, pTxtCotacoes);
    fclose(pTxtCotacoes);
    //DebugCotacoes(bitcoin.Valor, ethereum.Valor, ripple.Valor);//debug*/
    
    pTxtUsers = fopen(Users, "rb");
    if(pTxtUsers == NULL) return 3;
    fread(pClients, sizeof(Client), 10, pTxtUsers);
    fclose(pTxtUsers);

    DebugUser(pClients, -1);


    //login
    bool usuarioEncontrado, cpfIgual;
    while (true){
        printf("digite seu cpf:\n");
        scanf(" %s", respostaUser);
        if(strlen(respostaUser) > 11){
            printf("CPF digitado inválido, quantidade de caracteres excedida:\n");
            while (true)
            {
                printf("deseja continuar?\'s/n\'\n");
                scanf(" %s", respostaUser);
                if(respostaUser[0] == 'n') break;
                else if (respostaUser[0] == 's') break;
                printf("resposta não reconhecida\n");
            }
            if(respostaUser[0] == 'n'){
                free(pClients);
                return 0;
            }
            continue;
        }
        else if(strlen(respostaUser) < 11){
            printf("CPF digitado inválido, quantidade mínima de caracteres não preenchida:\n");
            while (true)
            {
                printf("deseja continuar?\'s/n\'\n");
                scanf(" %s", respostaUser);
                if(respostaUser[0] == 'n') break;
                else if (respostaUser[0] == 's') break;
                printf("resposta não reconhecida\n");
            }
            if(respostaUser[0] == 'n'){
                free(pClients);
                return 0;
            }
            continue;
        }
        usuarioEncontrado = false;
        for (index = 0; index < 10; index++)
        {
            cpfIgual = true;
            for (i = 0; i < 11; i++)
            {
                if (pClients[index].Cpf[i] != respostaUser[i]) cpfIgual = false;
            }
            if(cpfIgual){
                userIndex = index;
                strcpy(userCpf, pClients[userIndex].Cpf);
                strcpy(userSenha, pClients[userIndex].Senha);
                usuarioEncontrado = true;
            }
        }
        if (usuarioEncontrado)
        {
            break;
        }
        else{
            printf("Usuário não encontrado.\n");
            while (true){
                printf("deseja continuar?\'s/n\'\n");
                scanf(" %s", respostaUser);
                if(respostaUser[0] == 'n') break;
                else if (respostaUser[0] == 's') break;
                printf("resposta não reconhecida\n");
            }
            if(respostaUser[0] == 'n'){
                free(pClients);
                return 0;
            }
        }
    }
    if(userIndex == -1 || userCpf == ""){
        free(pClients);
        return 0;
    }
    pedirSenha = PedirSenha(userSenha);
    if(pedirSenha == -1){
        free(pClients);  
        return 0;
    } 
    else if(!pedirSenha){
        while (!pedirSenha)
        {
            printf("a senha informada está incorreta");
            while (true){
                printf("deseja continuar?\'s/n\'\n");
                scanf(" %s", respostaUser);
                if(respostaUser[0] == 'n') break;
                else if (respostaUser[0] == 's') break;
                printf("resposta não reconhecida\n");
            }
            if(respostaUser[0] == 'n'){
                free(pClients);
                return 0;
            }
            pedirSenha = PedirSenha(userSenha);
            if(pedirSenha == -1){
                free(pClients);
                return 0;
            }
        }
    }
    //menu
    while(true){
        system("clear");
        printf("Bem vindo %s!!\n\n", pClients[userIndex].Nome);
        sleep(2);
        printf("     _-+={Menu}=+-_     \n\n");
        printf("  {1} - Consultar saldo   \n");
        printf(" {2} - Consultar extrato  \n");
        printf("  {3} - Depositar reais   \n");
        printf("    {4} - Sacar reais   \n");
        printf("{5} - Comprar criptomoedas   \n");
        printf("{6} - Vender criptomoedas   \n");
        printf(" {7} - Atualizar cotação   \n");
        printf("       {8} - Sair   \n\n");

        printf("Selecione sua opção:\n");
        scanf(" %s", respostaUser);
        sleep(1);
        if(strlen(respostaUser) > 1){
            printf("\n formatação incorreta, digite apenas o numero referente a função a ser utilizada\n");
            continue;
        }

        if(respostaUser[0] == '1') ConsultarSaldo();
        else if(respostaUser[0] == '2') ConsultarExtrato();
        else if(respostaUser[0] == '3') DepositarReais();
        else if(respostaUser[0] == '4') SacarReais();
        else if(respostaUser[0] == '5') ComprarCriptomoedas(pClients, userIndex, &pCriptos[0], &pCriptos[1], &pCriptos[2]);
        else if(respostaUser[0] == '6') VenderCriptomoedas();
        else if(respostaUser[0] == '7') AtualizarCotacoes();
        else if(respostaUser[0] == '8') {
            SaveCotacoes(pCriptos, Cotacoes);
            SaveUsers(pClients, Users);
            break;
        }
        else{
            for(i = 0; i < 4; i++){
                printf(".");
                sleep(1);
            }
            printf("\n");
            for(i = 0; i < 3; i++){
                printf("?");
                sleep(1);
            }
            printf("!\n");
            continue;
        }
        SaveCotacoes(pCriptos, Cotacoes);
        SaveUsers(pClients, Users);
    }


    system("pause");
    free(pClients);
	return 0;

}

void DebugCotacoes(double bitcoin, double ethereum, double ripple){
    printf("Valores das criptos:\n\n   Bitcoin,  Ethereum,    Ripple;\n%10.2lf,%10.2lf,%10.2lf;\n", bitcoin, ethereum, ripple);
}

void DebugUser(CPointer pClients, int index){
    //-1 ira mostrar a tabela inteira
    if(index == -1){
        int i;
        for(i = 0; i < 10; i++){
            printf("| nome: |%20s| |,| Cpf: |%11s| |,| Senha: |%6s| |,| Reais: |%.2lf| |,| Bitcoin: |%.2lf| |,| Ethereum: |%.2lf| |,| Ripple: |%.2lf| |;\n", pClients[i].Nome, pClients[i].Cpf, pClients[i].Senha, pClients[i].Reais, pClients[i].Bitcoin, pClients[i].Ethereum, pClients[i].Ripple);
        }
    }
    else if(index < 9 && index > -1){
        printf("| nome: |%20s| |,| Cpf: |%11s| |,| Senha: |%6s| |,| Reais: |%.2lf| |,| Bitcoin: |%.2lf| |,| Ethereum: |%.2lf| |,| Ripple: |%.2lf| |;\n\n", pClients[index].Nome, pClients[index].Cpf, pClients[index].Senha, pClients[index].Reais, pClients[index].Bitcoin, pClients[index].Ethereum, pClients[index].Ripple);
    }
    else{
        printf("index inválido");
    }
}

void SaveCotacoes(MPointer pMoeda, const char* nomeArquivo){
    FILE* destino;
    destino = fopen(nomeArquivo, "wb");
    if(destino == NULL){
        perror("falha ao abrir o arquivo destino no \"SaveCotacoes\"");
    }
    else{
        fwrite(pMoeda, sizeof(Moeda), 3, destino);
        fclose(destino);
    }
}

void SaveUsers(CPointer pClients, const char *nomeArquivo){
    FILE* destino;
    destino = fopen(nomeArquivo, "wb");
    if(destino == NULL){
        perror("falha ao abrir o arquivo destino no \"SaveUsers\"");
    }
    else{
        fwrite(pClients, sizeof(Client), 10, destino);
        fclose(destino);
    }
}

int PedirSenha(char userSenhaCerta[7]){
    //retorna 1 caso o usuario tenha acertado a senha, 0 caso tenha errado e -1 caso o usuario deseja encerrar a tentativa;
    char respostaUser[20];
    while(true){
        printf("Digite sua senha:\n");
        scanf(" %s", respostaUser);
        if(strlen(respostaUser) > 6){
            printf("Senha digitada inválida, quantidade de caracteres excedida:\n");
            while (true)
            {
                printf("deseja continuar?\'s/n\'\n");
                scanf(" %s", respostaUser);
                if(respostaUser[0] == 'n') break;
                else if (respostaUser[0] == 's') break;
                printf("resposta não reconhecida\n");
            }
            if(respostaUser[0] == 'n'){
                return -1;
            }
            continue;
        }
        else if(strlen(respostaUser) < 6){
            printf("Senha digitada inválida, quantidade mínima de caracteres não preenchida:\n");
            while (true)
            {
                printf("deseja continuar?\'s/n\'\n");
                scanf(" %s", respostaUser);
                if(respostaUser[0] == 'n') break;
                else if (respostaUser[0] == 's') break;
                printf("resposta não reconhecida\n");
            }
            if(respostaUser[0] == 'n'){
                return -1;
            }
            continue;
        }
        int i;
        for(i = 0; i < 6; i++){
            if(respostaUser[i] != userSenhaCerta[i]) return 0;
        }
        return 1;
    }
}

void ConsultarSaldo(){

}
void ConsultarExtrato(){

}
void DepositarReais(){

}
void SacarReais(){

}
void ComprarCriptomoedas(CPointer pClients, int userIndex, MPointer bitcoin, MPointer ethereum, MPointer ripple){
    char respostaUser[20];
    double valorCompra, valorFinal;
    double taxa, valorConvertido;
    int opcaoMoeda;

    limparTerminal();

    printf("Qual criptomoeda deseja comprar?!\n");
    printf("1 - Bitcoin\n");
    printf("2 - Ethereum\n");
    printf("3 - Ripple\n");
    scanf("%d", &opcaoMoeda);

    limparTerminal();

    printf("Digite o valor que você deseja investir (em reais):\n");
    scanf("%lf", &valorCompra);

    limparTerminal();

    if (valorCompra > pClients[userIndex].Reais) {
        printf("Saldo insuficiente para realizar a operação!.\n");
        printf("\nPressione Enter para voltar ao menu principal\n");
        getchar();
        getchar();
        return;
    }

    switch(opcaoMoeda) {
    case 1: 
        taxa = valorCompra * bitcoin->TaxaCompra;  
        valorConvertido = (valorCompra - taxa) / bitcoin->Valor;  
        printf("Com o valor investido você comprará %.10lf Bitcoins, com uma taxa de R$%.2lf\nO total da transação será de R$%.2lf.\nConfirmar? (s/n)\n", valorConvertido, taxa, (valorCompra+taxa));
        break;
    case 2: 
        taxa = valorCompra * ethereum->TaxaCompra;  
        valorConvertido = (valorCompra - taxa) / ethereum->Valor;  
        printf("Com o valor investido você comprará %.10lf Ethereums, com uma taxa de R$%.2lf\nO total da transação será de R$%.2lf.\nConfirmar? (s/n)\n", valorConvertido, taxa, (valorCompra+taxa));
        break;
    case 3: 
        taxa = valorCompra * ripple->TaxaCompra;  
        valorConvertido = (valorCompra - taxa) / ripple->Valor;  
        printf("Com o valor investido você comprará %.10lf Ripples, com uma taxa de R$%.2lf\nO total da transação será de R$%.2lf.\nConfirmar? (s/n)\n", valorConvertido, taxa, (valorCompra+taxa));
        break;
    default:
        printf("Opção inválida.\n");
        printf("\nPressione Enter para voltar ao menu principal\n");
        return;
    }

    
    scanf(" %s", respostaUser);
    if (respostaUser[0] == 's') {
        pClients[userIndex].Reais -= (valorCompra + taxa); 
        switch(opcaoMoeda) {
            case 1:
                pClients[userIndex].Bitcoin += valorConvertido;
                break;
            case 2:
                pClients[userIndex].Ethereum += valorConvertido;
                break;
            case 3:
                pClients[userIndex].Ripple += valorConvertido;
                break;
        }
        limparTerminal();
        printf("Compra realizada com sucesso!\n");
        printf("\nPressione Enter para voltar ao menu principal\n");
        getchar();
        getchar();
    } else {
        limparTerminal();
        printf("Compra cancelada!\n");
        printf("\nPressione Enter para voltar ao menu principal\n");
        getchar();
        getchar();
    }

}
void VenderCriptomoedas(){

}
void AtualizarCotacoes(){

}

void limparTerminal() {
    printf("\033[H\033[J");
}