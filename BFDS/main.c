#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <stdbool.h>
#include <unistd.h>

typedef struct Extrato{
    
    char Usuario[21];
    char cpf[12];
    char TipoMoeda[11];
    char TipoTransacao;
    //+ ou -
    double Quantidade;
    double ValorDaMoedaNaTroca;
    
}Extrato, **EPointer;


typedef struct Client{

    char Nome[21];
    char Cpf[12];
    char Senha[7];

    int ExtratoIndice;
    //isso daqui não é pra mexer é só da função de extrato
    double Reais;
    double Bitcoin;
    double Ethereum;
    double Ripple;

}Client, *CPointer;

typedef struct Moeda{

    char Nome[11];
    double Valor;
    double TaxaVenda;
    double TaxaCompra;

}Moeda, *MPointer;


void DebugCotacoes(MPointer PCriptos);
void DebugUser(CPointer pClients, int index);
void SaveCotacoes(MPointer pCriptos, const char *nomeArquivo);
void SaveUsers(CPointer pClients, const char *nomeArquivo);
void SaveExtrato(EPointer ppExtrato, char *nomeArquivo);
int PedirSenha(char userSenha[7]);
void ConsultarSaldo();
void ConsultarExtrato();
bool AdicionarExtrato(EPointer ppExtrato, MPointer pCriptos, char *nomeMoeda, CPointer pClients, int userIndex, char TipoTransação, double quantidade);
void DepositarReais();
void SacarReais();
void ComprarCriptomoedas();
void VenderCriptomoedas();
void AtualizarCotacoes();


int main(int argc, char *argv[]) 
{
    setlocale(LC_ALL, "portuguese");
    
    FILE *pTxtCotacoes, *pTxtUsers, *pTxtExtrato;
    const char *Cotacoes = "Cotacoes.bin", *Users = "Users.bin", *Extratos = "Extrato.bin";


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
    if(pClients == NULL) return 1;
    
    MPointer pCriptos;
    pCriptos = (MPointer)calloc(3, sizeof(Moeda));
    if(pCriptos == NULL) return 1;


    EPointer ppExtrato;
    ppExtrato = (EPointer)calloc(10, sizeof(Extrato *));
    if(ppExtrato == NULL) return 1;
    for( index = 0; index < 10; i++){
        ppExtrato[index] = (Extrato *)calloc(100, sizeof(Extrato));
    }

    pTxtCotacoes = fopen(Cotacoes, "rb");
    if(pTxtCotacoes == NULL) return 2;
    fread(pCriptos, sizeof(Moeda), 3, pTxtCotacoes);
    fclose(pTxtCotacoes);
    //DebugCotacoes(bitcoin.Valor, ethereum.Valor, ripple.Valor);//debug*/
    
    pTxtUsers = fopen(Users, "rb");
    if(pTxtUsers == NULL) return 2;
    fread(pClients, sizeof(Client), 10, pTxtUsers);
    fclose(pTxtUsers);

    //teste
    SaveExtrato(ppExtrato, Extratos);
    
    pTxtExtrato = fopen(Extratos, "rb");
    if(pTxtExtrato == NULL) return 3;
    for (i = 0; i < 10; i++)
    {
        fread(ppExtrato[i], sizeof(Extrato), 100, pTxtExtrato);
    }
    fclose(pTxtExtrato);

    //teste de Adicionar Extrato
    AdicionarExtrato(ppExtrato, pCriptos, "Reais", pClients, userIndex, "D", 10);
    DebugExtrato(ppExtrato, -1);
    system("pause");

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
        else if(respostaUser[0] == '5') ComprarCriptomoedas();
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
        SaveExtrato(ppExtrato, Extratos);
    }


    system("pause");
    free(pClients);
    free(pCriptos);
    for(i = 0; i < 10; i++){
        free(ppExtrato[i]);
    }
    free(ppExtrato);
	return 0;

}

void DebugCotacoes(MPointer pCriptos){
    printf("Valores das criptos:\n\n   Bitcoin,  Ethereum,    Ripple;\n%10.2lf,%10.2lf,%10.2lf;\n", pCriptos[0].Valor, pCriptos[1].Valor, pCriptos[2].Valor);
}

void DebugUser(CPointer pClients, int index){
    //-1 ira mostrar a tabela inteira
    if(index == -1){
        int i;
        for(i = 0; i < 10; i++){
            printf("| nome: |%20s| |,| Cpf: |%11s| |,| Senha: |%6s| |,| Reais: |%15.2lf| |,| Bitcoin: |%15.2lf| |,| Ethereum: |%15.2lf| |,| Ripple: |%15.2lf| |;\n", pClients[i].Nome, pClients[i].Cpf, pClients[i].Senha, pClients[i].Reais, pClients[i].Bitcoin, pClients[i].Ethereum, pClients[i].Ripple);
        }
    }
    else if(index < 10 && index > -1){
        printf("| nome: |%20s| |,| Cpf: |%11s| |,| Senha: |%6s| |,| Reais: |%15.2lf| |,| Bitcoin: |%15.2lf| |,| Ethereum: |%15.2lf| |,| Ripple: |%15.2lf| |;\n\n", pClients[index].Nome, pClients[index].Cpf, pClients[index].Senha, pClients[index].Reais, pClients[index].Bitcoin, pClients[index].Ethereum, pClients[index].Ripple);
    }
    else{
        perror("index fornecido para \'DebugUser\' inválido");
    }
}

void DebugExtrato(EPointer ppExtrato, int index){
    int i,j;
    if(index == -1){
        printf("Porque Fez isso?????!!\n");
        sleep(2);
        for(i = 0; i < 10; i++){
            for (j = 0; j < 100; j++)
            {
                printf("| Usuario: |%20s| |,| Cpf: |%11s| |,| TipoMoeda: |%10s| |,| TipoTransacao: |%c| |,| Quantidade: |%15.2lf| |,| ValorDaMoedaNaTroca: |%15.2lf|;\n", ppExtrato[i][j].Usuario, ppExtrato[i][j].cpf, ppExtrato[i][j].TipoMoeda, ppExtrato[i][j].TipoTransacao, ppExtrato[i][j].Quantidade, ppExtrato[i][j].ValorDaMoedaNaTroca);
            }
            printf("\n\n");
        }
    }
    else if(index > -1 && index < 10){
            for(j = 0; j < 100; j++){
                printf("| Usuario: |%20s| |,| Cpf: |%11s| |,| TipoMoeda: |%10s| |,| TipoTransacao: |%c| |,| Quantidade: |%15.2lf| |,| ValorDaMoedaNaTroca: |%15.2lf|;\n", ppExtrato[index][j].Usuario, ppExtrato[index][j].cpf, ppExtrato[index][j].TipoMoeda, ppExtrato[index][j].TipoTransacao, ppExtrato[index][j].Quantidade, ppExtrato[index][j].ValorDaMoedaNaTroca);
            }
    }
    else{
        perror("index fornecido para \'DebugExtrato\' inválido");
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

void SaveExtrato(EPointer ppExtrato, char *nomeArquivo){
    int i;
    FILE* destino;
    destino = fopen(nomeArquivo, "wb");
    if(destino == NULL){
        perror("falha ao abrir o arquivo destino no \"SaveExtrato\"");
    }
    else{
        for(i = 0; i < 10; i++){
            fwrite(ppExtrato[i], sizeof(Extrato), 100, destino);
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

void ConsultarSaldo(){

}

bool AdicionarExtrato(EPointer ppExtrato, MPointer pCriptos, char *nomeMoeda, CPointer pClients, int userIndex, char TipoTransação, double quantidade){
    //TipoTransação pode ser D de deposito, + quando o usuario ganhar a respectiva "double quantidade", e - quando perder.
    int criptoIndex = -1, i;
    bool achou = false;
    if (strcmp(nomeMoeda, "Reais") != 0)
    {
        for (i = 0; i < 3; i++)
        {
            if(strcmp(pCriptos[i].Nome, nomeMoeda) == 0){
                achou = true;
                criptoIndex = i;
                break;
            }
        }
        if(!achou){
            perror("\'nomeMoeda\'escrito, na função \'AdicionarExtrato\', não encontrado");
            return false;
        }
    }
    
    strcpy(ppExtrato[userIndex][pClients[userIndex].ExtratoIndice].Usuario, pClients[userIndex].Nome);
    strcpy(ppExtrato[userIndex][pClients[userIndex].ExtratoIndice].cpf, pClients[userIndex].Cpf);
    ppExtrato[userIndex][pClients[userIndex].ExtratoIndice].TipoTransacao = TipoTransação;

    if(criptoIndex != -1){
        strcpy(ppExtrato[userIndex][pClients[userIndex].ExtratoIndice].TipoMoeda, pClients[userIndex].Nome);
        ppExtrato[userIndex][pClients[userIndex].ExtratoIndice].ValorDaMoedaNaTroca = pCriptos[criptoIndex].Valor;
    }
    else{
        strcpy(ppExtrato[userIndex][pClients[userIndex].ExtratoIndice].TipoMoeda, nomeMoeda);
        ppExtrato[userIndex][pClients[userIndex].ExtratoIndice].ValorDaMoedaNaTroca = 1;
    }

    ppExtrato[userIndex][pClients[userIndex].ExtratoIndice].Quantidade = quantidade;

    pClients[userIndex].ExtratoIndice++;
    return true;
}

void ConsultarExtrato(){

}

void DepositarReais(){

}

void SacarReais(){

}

void ComprarCriptomoedas(){

}

void VenderCriptomoedas(){

}

void AtualizarCotacoes(){

}