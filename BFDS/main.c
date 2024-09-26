#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <stdbool.h>

void DebugCotacoes(double bitcoin, double ethereum, double ripple);
void DebugUser(Cpointer pClients, int index);
void SaveCotacoes(double bitcoin, double ethereum, double ripple, char *nomeArquivo);
void SaveUsers(Cpointer pClients, char *nomeArquivo);
int PedirSenha(char userSenha[7]);

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

    int userIndex = -1;
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
        printf("\nleitor:\n%s", leitor);//debug
        quantDadosPegosDoSscanf = sscanf(leitor,"%[^,],%[^,],%[^,],%lf,%lf,%lf,%lf", pClients[index].Nome, pClients[index].Cpf, pClients[index].Senha, &pClients[index].Reais, &pClients[index].Bitcoin, &pClients[index].Ethereum, &pClients[index].Ripple);
        printf("quantDadosPegosDoSscanf: %d\n\n", quantDadosPegosDoSscanf);//debug
        index++;
    }
    fclose(pTxtUsers);

    //login
    bool usuarioEncontrado, cpfIgual;
    int pedirSenha;
    char respostaUser[20];
    while (true)
    {
        while (true)
        {
            printf("digite seu cpf:\n");
            scanf(" %s", respostaUser);
            if(strlen(respostaUser) > 11){
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
                    break;
                }
                continue;
            }
            else if(strlen(respostaUser) < 11){
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
                    break;
                }
                continue;
            }
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
                break;
            }
        }
    }
    if(userIndex == -1 || userCpf == ""){
        return 0;
    }
    pedirSenha = PedirSenha(userSenha);
    if(pedirSenha == -1) return 0;
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
                break;
            }
            pedirSenha = PedirSenha(userSenha);
            if(pedirSenha == -1) return 0;
        }
        if(respostaUser[0] == 'n'){
            return 0;
        }
    }
    //menu
    while(true){
        /*menu*/
    }

    system("pause");	
    free(pClients);
	return 0;

}

void DebugCotacoes(double bitcoin, double ethereum, double ripple){
    printf("Valores das criptos:\n\n   Bitcoin,  Ethereum,    Ripple;\n%10.2lf,%10.2lf,%10.2lf;\n", bitcoin, ethereum, ripple);
}

void DebugUser(Cpointer pClients, int index){
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

void SaveCotacoes(double bitcoin, double ethereum, double ripple, char *nomeArquivo){
    FILE* destino;
    destino = fopen(nomeArquivo, "w");
    if(destino == NULL){
        perror("falha ao abrir o arquivo destino no \"SaveCotacoes\"");
    }
    else{
        fprintf(destino,"Bitcoin,Ethereum,Ripple\n");
        fprintf(destino,"%.2lf,%.2lf,%.2lf\n", bitcoin, ethereum, ripple);
        fclose(destino);
    }
}

void SaveUsers(Cpointer pClients, char *nomeArquivo){
    FILE* destino;
    destino = fopen(nomeArquivo, "w");
    if(destino == NULL){
        perror("falha ao abrir o arquivo destino no \"SaveUsers\"");
    }
    else{
        fprintf(destino,"Nome,Cpf,Senha,Reais,Bitcoin,Ethereum,Ripple\n");
        int c;
        for (c = 0; c < 10; c++)
        {
            fprintf(destino, "%s,%s,%s,%.2lf,%.2lf,%.2lf,%.2lf\n", pClients[c].Nome, pClients[c].Cpf, pClients[c].Senha, pClients[c].Reais, pClients[c].Bitcoin, pClients[c].Ethereum, pClients[c].Ripple);
        }
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