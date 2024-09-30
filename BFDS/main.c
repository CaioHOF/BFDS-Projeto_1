#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>

typedef struct Extrato{
    
    char Usuario[21];
    char cpf[12];
    char TipoMoeda[11];
    char TipoTransacao;
    //+ ou -
    double TaxaTransacao;
    double Quantidade;
    double ValorDaMoedaNaTroca;
    int IDNumDaTransacao;
    struct tm DataTransacao;

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


//funções do sistema
void DebugCotacoes(MPointer pCriptos, int index);

void DebugUser(CPointer pClients, int index);

void DebugExtrato(EPointer ppExtrato, int index);

void SaveCotacoes(MPointer pCriptos, const char *nomeArquivo);

void SaveUsers(CPointer pClients, const char *nomeArquivo);

void SaveExtratos(EPointer ppExtrato, const char *nomeArquivo);

int PedirSenha(char userSenha[7]);

void limparTerminal();

//Funções do Usuario
void ConsultarSaldo(CPointer pClients, int userIndex);

void ConsultarExtrato(EPointer ppExtrato, int index);

bool AdicionarExtrato(EPointer ppExtrato, MPointer pCriptos, char *nomeMoeda, CPointer pClients, int userIndex, char TipoTransacao, double quantidade);

void DepositarReais(CPointer pClients, int userIndex, EPointer ppExtrato);

void SacarReais(CPointer pClients, int userIndex, char userSenha[7], EPointer ppExtrato);

void ComprarCriptomoedas(CPointer pClients, int userIndex, MPointer bitcoin, MPointer ethereum, MPointer ripple, EPointer ppExtrato, char userSenha[7]);

void VenderCriptomoedas(CPointer pClients, int userIndex, MPointer PCriptos, EPointer ppExtrato);

void AtualizarCotacoes(MPointer pCriptos);


int main(int argc, char *argv[]) 
{
    setlocale(LC_ALL, "portuguese");
    setenv("TZ","America/Sao_Paulo",1);
    tzset();
    srand(time(NULL));
    
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
    for( index = 0; index < 10; index++){
        ppExtrato[index] = (Extrato *)calloc(100, sizeof(Extrato));
        if (ppExtrato[index] == NULL) return 1;
    }

    pTxtCotacoes = fopen(Cotacoes, "rb");
    if(pTxtCotacoes == NULL) return 2;
    fread(pCriptos, sizeof(Moeda), 3, pTxtCotacoes);
    fclose(pTxtCotacoes);
    
    pTxtUsers = fopen(Users, "rb");
    if(pTxtUsers == NULL) return 2;
    fread(pClients, sizeof(Client), 10, pTxtUsers);
    fclose(pTxtUsers);

    pTxtExtrato = fopen(Extratos, "rb");
    if(pTxtExtrato == NULL) return 2;
    for (index = 0; index < 10; index++)
    {
        fread(ppExtrato[index], sizeof(Extrato), 100, pTxtExtrato);
    }
    fclose(pTxtExtrato);

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
        limparTerminal();
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

        if(respostaUser[0] == '1') ConsultarSaldo(pClients, userIndex);
        else if(respostaUser[0] == '2') ConsultarExtrato(ppExtrato, userIndex);
        else if(respostaUser[0] == '3') DepositarReais(pClients, userIndex, ppExtrato);
        else if(respostaUser[0] == '4') SacarReais(pClients, userIndex, userSenha, ppExtrato);
        else if(respostaUser[0] == '5') ComprarCriptomoedas(pClients, userIndex, &pCriptos[0], &pCriptos[1], &pCriptos[2], ppExtrato, userSenha);
        else if(respostaUser[0] == '6') VenderCriptomoedas(pClients, userIndex, pCriptos, ppExtrato);
        else if(respostaUser[0] == '7') AtualizarCotacoes(pCriptos);
        else if(respostaUser[0] == '8') {
            break;
        }
        else if(respostaUser[0] == 'C'){
            srand(time(NULL));
            double n = rand();
            for(i = 0; i < 100; i++){
                n = (rand() %100100) / 100.0;
                pClients[userIndex].Reais += n;
                AdicionarExtrato(ppExtrato,pCriptos,"Reais",pClients,userIndex,'D',n);
            }
        } 
        else if(respostaUser[0] == 'c'){
            srand(time(NULL));
            double n = (rand() %100100) / 100.0;
            pClients[userIndex].Reais += n;
            AdicionarExtrato(ppExtrato,pCriptos,"Reais",pClients,userIndex,'D',n);
        } 
        else{
                printf(".");
                sleep(1);
                printf(".");
                sleep(1);
                printf(".\n");
                sleep(2);
                printf("?");
                sleep(1);
                printf("?");
                sleep(1);
                printf("?!\n");
                sleep(1);
            continue;
        }
        SaveCotacoes(pCriptos, Cotacoes);
        SaveUsers(pClients, Users);
        SaveExtratos(ppExtrato, Extratos);
        printf("Pressione Enter para continuar...");
        getchar();
    }
    free(pClients);
    free(pCriptos);
    for(i = 0; i < 10; i++){
        free(ppExtrato[i]);
    }
    free(ppExtrato);
	return 0;

}

void DebugCotacoes(MPointer pCriptos, int index){
    if(index == -1){
        int i;
        for(i = 0; i < 3; i++){
            printf("| nome: |%10s| |,| valor: |%10.2lf| |,| TaxaVenda: |%3.2lf| |,| TaxaCompra: |%3.2lf| |;\n", pCriptos[i].Nome, pCriptos[i].Valor, pCriptos[i].TaxaVenda, pCriptos[i].TaxaCompra);
        }

    }
    else if(index > -1 && index < 3){
        printf("| nome: |%10s| |,| valor: |%10.2lf| |,| TaxaVenda: |%3.2lf| |,| TaxaCompra: |%3.2lf| |;\n", pCriptos[index].Nome, pCriptos[index].Valor, pCriptos[index].TaxaVenda, pCriptos[index].TaxaCompra);
    }
    else{
        perror("index fornecido para \'DebugCotacoes\' inválido");
    }
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
        printf("Porque Fez isso?!\n");
        sleep(2);
        for(i = 0; i < 10; i++){
            for (j = 0; j < 100; j++)
            {
            printf("| Usuario: |%15s| |,| Cpf: |%11s| |,| IDNumeTransacao: |%03d| |,| TipoMoeda: |%10s| |,| TipoTransacao: |%c| |,| TaxaTransacao: |%1.2lf| |,| Quantidade: |%5.2lf| |,| ValorDaMoedaNaTroca: |%5.2lf|,| Data: |%02d/%02d/%02d|,| Hora: |%02d:%02d:%02d|;\n", ppExtrato[i][j].Usuario, ppExtrato[i][j].cpf, ppExtrato[i][j].IDNumDaTransacao, ppExtrato[i][j].TipoMoeda, ppExtrato[i][j].TipoTransacao, ppExtrato[i][j].TaxaTransacao, ppExtrato[i][j].Quantidade, ppExtrato[i][j].ValorDaMoedaNaTroca, ppExtrato[i][j].DataTransacao.tm_mday, ppExtrato[i][j].DataTransacao.tm_mon, ppExtrato[i][j].DataTransacao.tm_year, ppExtrato[i][j].DataTransacao.tm_hour, ppExtrato[i][j].DataTransacao.tm_min, ppExtrato[i][j].DataTransacao.tm_sec);
            }
            printf("\n\n");
        }
    }
    else if(index > -1 && index < 10){
            for(j = 0; j < 100; j++){
            printf("| Usuario: |%15s| |,| Cpf: |%11s| |,| IDNumeTransacao: |%03d| |,| TipoMoeda: |%10s| |,| TipoTransacao: |%c| |,| TaxaTransacao: |%1.2lf| |,| Quantidade: |%5.2lf| |,| ValorDaMoedaNaTroca: |%5.2lf|,| Data: |%02d/%02d/%02d|,| Hora: |%02d:%02d:%02d|;\n", ppExtrato[index][j].Usuario, ppExtrato[index][j].cpf, ppExtrato[index][j].IDNumDaTransacao, ppExtrato[index][j].TipoMoeda, ppExtrato[index][j].TipoTransacao, ppExtrato[index][j].TaxaTransacao, ppExtrato[index][j].Quantidade, ppExtrato[index][j].ValorDaMoedaNaTroca, ppExtrato[index][j].DataTransacao.tm_mday, ppExtrato[index][j].DataTransacao.tm_mon, ppExtrato[index][j].DataTransacao.tm_year, ppExtrato[index][j].DataTransacao.tm_hour, ppExtrato[index][j].DataTransacao.tm_min, ppExtrato[index][j].DataTransacao.tm_sec);
            }
    }
    else{
        perror("index fornecido para \'DebugExtrato\' inválido");
    }
    getchar();
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

void SaveExtratos(EPointer ppExtrato, const char *nomeArquivo){
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

void ConsultarSaldo(CPointer pClients, int userIndex){

    limparTerminal();

    printf("===== Consulta de Saldo =====\n\n");
    printf("Nome: %s\n", pClients[userIndex].Nome);
    printf("Saldo em Reais: R$ %.2lf\n", pClients[userIndex].Reais);
    printf("Saldo em Bitcoin: %.8lf BTC\n", pClients[userIndex].Bitcoin);
    printf("Saldo em Ethereum: %.8lf ETH\n", pClients[userIndex].Ethereum);
    printf("Saldo em Ripple: %.8lf XRP\n", pClients[userIndex].Ripple);

    getchar();



}

bool AdicionarExtrato(EPointer ppExtrato, MPointer pCriptos, char *nomeMoeda, CPointer pClients, int userIndex, char TipoTransacao, double quantidade){
    //EPointer, Mpointer, Cpointer são os ponteiros das struct que estamos usando;
    //NomeMoeda é qual moeda ta sendo usada na transação, garanta que ele foi escrito corretamente, caso o contrario o programa ira falar "'nomeMoeda'escrito, na função 'AdicionarExtrato', não encontrado";
    //userIndex é o index do user é uma variavel da main que é pega na função login;
    //TipoTransação é um char que tem que ser escrito com aspas simples ou seja ' '. Ele pode ser D de deposito, S de saque, V de venda ou C de compra;
    //quantidade é a quantidade que o user esta movimentando na transação(usar apenas valores absolutos ou seja nao podem ser negativos, o tipo da operação é definido por outra variavel);
    int criptoIndex = -1, i, num;
    bool achou = false;
    if(TipoTransacao != 'D'&&TipoTransacao != 'S'&&TipoTransacao != 'V'&&TipoTransacao != 'C'){
        perror("\'TipoTransacao\'escrito, na função \'AdicionarExtrato\', não permitido");
        return false;
    }
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

    time_t t;
    time(&t);
    
    strcpy(ppExtrato[userIndex][pClients[userIndex].ExtratoIndice].Usuario, pClients[userIndex].Nome);
    strcpy(ppExtrato[userIndex][pClients[userIndex].ExtratoIndice].cpf, pClients[userIndex].Cpf);
    ppExtrato[userIndex][pClients[userIndex].ExtratoIndice].TipoTransacao = TipoTransacao;

    switch (ppExtrato[userIndex][pClients[userIndex].ExtratoIndice].TipoTransacao)
    {
    case 'D':
        ppExtrato[userIndex][pClients[userIndex].ExtratoIndice].TaxaTransacao = 0;
        break;
    case 'S':
        ppExtrato[userIndex][pClients[userIndex].ExtratoIndice].TaxaTransacao = 0;
        break;
    case 'V':
        ppExtrato[userIndex][pClients[userIndex].ExtratoIndice].TaxaTransacao = pCriptos[criptoIndex].TaxaVenda;
        ppExtrato[userIndex][pClients[userIndex].ExtratoIndice].TaxaTransacao = pCriptos[criptoIndex].TaxaVenda;
        break;
    case 'C':
        ppExtrato[userIndex][pClients[userIndex].ExtratoIndice].TaxaTransacao = pCriptos[criptoIndex].TaxaCompra;
        ppExtrato[userIndex][pClients[userIndex].ExtratoIndice].TaxaTransacao = pCriptos[criptoIndex].TaxaCompra;
        break;
    default:
        perror("\'nomeMoeda\'escrito, na função \'AdicionarExtrato\', não encontrado. DADOS FORAM CORROMPIDOS");
        return false;
        break;
    }

    if(criptoIndex != -1){
        strcpy(ppExtrato[userIndex][pClients[userIndex].ExtratoIndice].TipoMoeda, pCriptos[criptoIndex].Nome);            //AQUI
        strcpy(ppExtrato[userIndex][pClients[userIndex].ExtratoIndice].TipoMoeda, pCriptos[criptoIndex].Nome);
        ppExtrato[userIndex][pClients[userIndex].ExtratoIndice].ValorDaMoedaNaTroca = pCriptos[criptoIndex].Valor;
    }
    else{
        strcpy(ppExtrato[userIndex][pClients[userIndex].ExtratoIndice].TipoMoeda, nomeMoeda);
        ppExtrato[userIndex][pClients[userIndex].ExtratoIndice].ValorDaMoedaNaTroca = 1;
    }

    ppExtrato[userIndex][pClients[userIndex].ExtratoIndice].Quantidade = quantidade;
    num = pClients[userIndex].ExtratoIndice -1;
    if(num < 0) num = 99;
    ppExtrato[userIndex][pClients[userIndex].ExtratoIndice].IDNumDaTransacao = ppExtrato[userIndex][num].IDNumDaTransacao +1;

    ppExtrato[userIndex][pClients[userIndex].ExtratoIndice].DataTransacao = *localtime(&t);
    ppExtrato[userIndex][pClients[userIndex].ExtratoIndice].DataTransacao.tm_mon += 1;
    ppExtrato[userIndex][pClients[userIndex].ExtratoIndice].DataTransacao.tm_year += 1900;

    pClients[userIndex].ExtratoIndice++;
    if(pClients[userIndex].ExtratoIndice == 100) pClients[userIndex].ExtratoIndice = 0;
    return true;
}

void ConsultarExtrato(EPointer ppExtrato, int index){
    if(index > -1 && index < 10){
        int j;
        for(j = 0; j < 100; j++){
            printf("| Usuario: |%15s| |,| Cpf: |%11s| |,| IDNumeTransacao: |%03d| |,| TipoMoeda: |%10s| |,| TaxaTransacao: |%1.2lf| |,| TipoTransacao: |%c| |,| Quantidade: |%5.2lf| |,| ValorDaMoedaNaTroca: |%5.2lf|,| Data: |%02d/%02d/%02d|,| Hora: |%02d:%02d:%02d|;\n", ppExtrato[index][j].Usuario, ppExtrato[index][j].cpf, ppExtrato[index][j].IDNumDaTransacao, ppExtrato[index][j].TipoMoeda, ppExtrato[index][j].TipoTransacao, ppExtrato[index][j].TaxaTransacao, ppExtrato[index][j].Quantidade, ppExtrato[index][j].ValorDaMoedaNaTroca, ppExtrato[index][j].DataTransacao.tm_mday, ppExtrato[index][j].DataTransacao.tm_mon, ppExtrato[index][j].DataTransacao.tm_year, ppExtrato[index][j].DataTransacao.tm_hour, ppExtrato[index][j].DataTransacao.tm_min, ppExtrato[index][j].DataTransacao.tm_sec);
        }
    }
    else{
        perror("index da função \'ConsultarExtrato\' inválido \n");
    }
    getchar();
}

void DepositarReais(CPointer pClients, int userIndex, EPointer ppExtrato){

    limparTerminal();

    double deposito;
    printf("Quantos reais gostaria de depositar?: ");
    scanf(" %lf", &deposito);
    
    if(deposito <= 0) { 
        printf("[ERRO]! Valor inválido.\n O saque deve ser maior que zero.\n");
        getchar(); 
        return;
    }


    pClients[userIndex].Reais += deposito;
    
    AdicionarExtrato(ppExtrato, NULL, "Reais", pClients, userIndex, 'D', deposito);

    printf("Você depositou R$ %.2lf\nTotal na conta: R$ %.2lf\n",deposito, pClients[userIndex].Reais); 

    getchar();



}

void SacarReais(CPointer pClients, int userIndex, char userSenha[7], EPointer ppExtrato){


    limparTerminal();


    int pedirSenhaSaque = PedirSenha(userSenha);
    double saque;
    
    if(pedirSenhaSaque == 1){  
        printf("Quantos reais gostaria de sacar?: ");
        scanf(" %lf", &saque);

        if(saque <= 0) { 
            printf("[ERRO]! Valor inválido.\n O saque deve ser maior que zero.\n");
            getchar();
            return;
        }

        if(pClients[userIndex].Reais < saque){  
            printf("Em sua conta não a saldo suficiente para realizar essa operação!\n");

            getchar();
        }
        else{
            pClients[userIndex].Reais = pClients[userIndex].Reais - saque;  

            printf("R$ %.2lf sacados da sua conta!\n", saque);
            printf("Saldo Atual: R$ %.2lf\n", pClients[userIndex].Reais);
            AdicionarExtrato(ppExtrato, NULL, "Reais", pClients, userIndex, 'S', saque);

            getchar();
        }
    }
    else{
        printf("Senha incorreta! Operação cancelada.\n");
        getchar();

    }




}

void ComprarCriptomoedas(CPointer pClients, int userIndex, MPointer bitcoin, MPointer ethereum, MPointer ripple, EPointer ppExtrato, char userSenha[7]){

    char respostaUser[20];
    double valorCompra, valorFinal;
    double taxa, valorCripto;
    int opcaoMoeda;
    int pedirSenhaSaque = PedirSenha(userSenha);

    limparTerminal();


    if(pedirSenhaSaque == 1){ 
    printf("Qual criptomoeda deseja comprar?!\n");
    printf("1 - Bitcoin\n");
    printf("2 - Ethereum\n");
    printf("3 - Ripple\n");
    scanf("%d", &opcaoMoeda);

    limparTerminal();

    printf("Digite o valor que você deseja investir (em reais):\n");
    scanf("%lf", &valorCompra);

    limparTerminal();

    if(valorCompra <= 0){
        printf("O valor deve ser maior que zero!\n");
        getchar();
        return;
    }

    if (valorCompra > pClients[userIndex].Reais) {
        printf("Saldo insuficiente para realizar a operação!.\n");
        getchar();
        return;
    }

    switch(opcaoMoeda) {
    case 1: 
        taxa = valorCompra * bitcoin->TaxaCompra;  
        valorCripto = (valorCompra - taxa) / bitcoin->Valor;  
        printf("Com o valor investido você comprará %.10lf Bitcoins, com uma taxa de R$%.2lf\nO total da transação será de R$%.2lf.\nConfirmar? (s/n)\n", valorCripto, taxa, (valorCompra+taxa));
        break;
    case 2: 
        taxa = valorCompra * ethereum->TaxaCompra;  
        valorCripto = (valorCompra - taxa) / ethereum->Valor;  
        printf("Com o valor investido você comprará %.10lf Ethereums, com uma taxa de R$%.2lf\nO total da transação será de R$%.2lf.\nConfirmar? (s/n)\n", valorCripto, taxa, (valorCompra+taxa));
        break;
    case 3: 
        taxa = valorCompra * ripple->TaxaCompra;  
        valorCripto = (valorCompra - taxa) / ripple->Valor;  
        printf("Com o valor investido você comprará %.10lf Ripples, com uma taxa de R$%.2lf\nO total da transação será de R$%.2lf.\nConfirmar? (s/n)\n", valorCripto, taxa, (valorCompra+taxa));
        break;
    default:
        printf("Opção inválida.\n");
        return;
    }

    
    scanf(" %s", respostaUser);
    if (respostaUser[0] == 's') {
        pClients[userIndex].Reais -= (valorCompra + taxa); 


        switch(opcaoMoeda) {
            case 1:
                pClients[userIndex].Bitcoin += valorCripto;
                break;
            case 2:
                pClients[userIndex].Ethereum += valorCripto;
                break;
            case 3:
                pClients[userIndex].Ripple += valorCripto;
                break;
        }

        switch(opcaoMoeda) {
        case 1: 
            AdicionarExtrato(ppExtrato, bitcoin, "Bitcoin", pClients, userIndex, 'C', valorCripto); 
            break;
        case 2: 
            AdicionarExtrato(ppExtrato, ethereum, "Ethereum", pClients, userIndex, 'C', valorCripto);
            break;
        case 3: 
            AdicionarExtrato(ppExtrato, ripple, "Ripple", pClients, userIndex, 'C', valorCripto); 
            break;
        default:
            printf("[ERRO] Extrato Inválido.\n");
            getchar();
            return;
        }

        limparTerminal();
        printf("Compra realizada com sucesso!\n");
        getchar();
    } else {
        limparTerminal();
        printf("Compra cancelada!\n");
        getchar();
    }
    }
    else{
        printf("Senha incorreta! Operação cancelada.\n");
        getchar();

    }


}

void VenderCriptomoedas(CPointer pClients, int userIndex, MPointer pCriptos, EPointer ppExtrato){
    char respostaUser[20];
    double valorVenda, valorFinal;
    double taxa, valorConvertido;
    int opcaoMoeda;

    limparTerminal();

    printf("Qual criptomoeda deseja vender?\n");
    printf("1 - Bitcoin\n");
    printf("2 - Ethereum\n");
    printf("3 - Ripple\n");
    scanf("%d", &opcaoMoeda);

    limparTerminal();

    switch(opcaoMoeda) {
    case 1: 

        printf("Digite o valor de Bitcoin que você deseja converter para reais:\n");
        scanf("%lf", &valorVenda);
        if (valorVenda > pClients[userIndex].Bitcoin) {
            printf("Saldo insuficiente para realizar a operação.\n");
            getchar();
            return;
        } 
        else if (valorVenda <0){
            printf("Valor inválido, tente novamente.");
            getchar();
            return;
        }

        
        else{
            valorConvertido = valorVenda * (pCriptos[0].Valor);
            taxa = valorConvertido * (pCriptos[0].TaxaVenda);
            valorFinal = valorConvertido - taxa;
            printf("Com o valor investido você transformará %.2lf Bitcoins em R$:%.2lf Reais, com uma taxa de R$:%.2lf.\nO total da transação será de R$:%.2lf.\nConfirmar? (s/n)\n",valorVenda, valorFinal, taxa, (valorConvertido));
            scanf(" %s", respostaUser);
            if (respostaUser[0] == 's') {
                pClients[userIndex].Reais += (valorFinal);
                pClients[userIndex].Bitcoin -= (valorVenda); 
                limparTerminal();
                printf("Compra realizada com sucesso!\n");
                getchar();   
                break; 
            }
        else {
            limparTerminal();
            printf("Compra cancelada!\n");
            getchar();
            break;
        }
    }
    limparTerminal();

    case 2: 
        printf("Digite o valor de Ethereum que você deseja converter para reais:\n");
        scanf("%lf", &valorVenda);
        if (valorVenda > pClients[userIndex].Ethereum) {
            printf("Saldo insuficiente para realizar a operação.\n");
            getchar();
            return;
        }
            else if (valorVenda <0){
            printf("Valor inválido, tente novamente.");
            getchar();
            return;
        }
        else{
            valorConvertido = valorVenda * (pCriptos[1].Valor);
            taxa = valorConvertido * (pCriptos[1].TaxaVenda);
            valorFinal = valorConvertido - taxa;
            printf("Com o valor investido você transformará %.2lf Ethereum em R$:%.2lf Reais, com uma taxa de R$:%.2lf.\nO total da transação será de R$:%.2lf.\nConfirmar? (s/n)\n",valorVenda, valorFinal, taxa, (valorConvertido));
            scanf(" %s", respostaUser);
            if (respostaUser[0] == 's') {
                pClients[userIndex].Reais += (valorFinal);
                pClients[userIndex].Ethereum -= (valorVenda); 
                limparTerminal();
                printf("Compra realizada com sucesso!\n");
                getchar();   
                break; 
            }
        else {
            limparTerminal();
            getchar();
            break;
        }
    }
    limparTerminal();

    case 3: 
        printf("Digite o valor de Ripple que você deseja converter para reais:\n");
        scanf("%lf", &valorVenda);
        if (valorVenda > pClients[userIndex].Ripple) {
            printf("Saldo insuficiente para realizar a operação.\n");
            getchar();
            return;
        }
        else if (valorVenda <0){
            printf("Valor inválido, tente novamente.");
            getchar();
            return;
        }
        else{
            valorConvertido = valorVenda * (pCriptos[2].Valor);
            taxa = valorConvertido * (pCriptos[2].TaxaVenda);
            valorFinal = valorConvertido - taxa;
            printf("Com o valor investido você transformará %.2lf Ripple em R$:%.2lf Reais, com uma taxa de R$:%.2lf.\nO total da transação será de R$:%.2lf.\nConfirmar? (s/n)\n",valorVenda, valorFinal, taxa, (valorConvertido));
            scanf(" %s", respostaUser);
            if (respostaUser[0] == 's') {
                pClients[userIndex].Reais += (valorFinal);
                pClients[userIndex].Ripple -= (valorVenda); 
                limparTerminal();
                printf("Compra realizada com sucesso!\n");
                getchar();   
                break; 
            }
        else {
            limparTerminal();
            printf("Compra cancelada!\n");
            getchar();
            break;
        }
    }
        limparTerminal();

    default:
        printf("Opção inválida.\n");
        printf("\nPressione Enter para voltar ao menu principal\n");
        return;
        }
    switch(opcaoMoeda){
        case 1:
        AdicionarExtrato(ppExtrato, pCriptos, pCriptos[0].Nome, pClients, userIndex, 'V', valorFinal);
        break;
        case 2:
        AdicionarExtrato(ppExtrato, pCriptos, pCriptos[1].Nome, pClients, userIndex, 'V', valorFinal);
        break;
        case 3:
        AdicionarExtrato(ppExtrato, pCriptos, pCriptos[2].Nome, pClients, userIndex, 'V', valorFinal);
        break;
        default:
        printf("Erro, Extrato Inválido.");
        getchar();
    }
    limparTerminal();
}

void AtualizarCotacoes(MPointer pCriptos){
    srand(time(NULL));
    int CotacaoBTC, CotacaoETH ,CotacaoXRP;
    char respostaUser[20];
    
    limparTerminal();
    
    while (true){
        printf("A cotação das moedas é: %.2lf Reais/Bitcoin, %.2lf Reais/Ethereum, %.2lf Reais/Ripple.\n", pCriptos[0].Valor, pCriptos[1].Valor, pCriptos[2].Valor);
        printf("Deseja atualizar? (s/n) \n");
        scanf("%s", respostaUser);
        if (respostaUser[0] == 's'){
            printf("Processando novos valores...\n");
            sleep(4);

            int CotacaoBTC = (rand() % 11) - 5;
            int CotacaoETH = (rand() % 11) - 5;
            int CotacaoXRP = (rand() % 11) - 5;

            pCriptos[0].Valor = pCriptos[0].Valor +(CotacaoBTC*pCriptos[0].Valor *0.01);
            pCriptos[1].Valor = pCriptos[1].Valor +(CotacaoETH*pCriptos[1].Valor *0.01);
            pCriptos[2].Valor = pCriptos[2].Valor +(CotacaoXRP*pCriptos[2].Valor *0.01);

            printf("A nova cotação das moedas é: %.2lf Reais/Bitcoin, %.2lf Reais/Ethereum, %.2lf Reais/Ripple.\n", pCriptos[0].Valor, pCriptos[1].Valor, pCriptos[2].Valor);
            getchar();
            return;
        }
        else if (respostaUser[0] == 'n'){
            getchar();
            return;
        }
        else{
            printf("Resposta inválida! Tente novamente.\n");
        }
    }
    limparTerminal();
}


void limparTerminal() {
    printf("\033[H\033[J");
}