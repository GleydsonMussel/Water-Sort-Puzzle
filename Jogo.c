// Include das bibliotecas padrão
#include <stdio.h>
#include <stdlib.h>

// Include das bibliotecas para o sleep funcionar
#ifdef _WIN32 || _WIN64
    #include <Windows.h>
#else
    #include <unistd.h>
#endif

// Definição do tamanho do texto de preecnhimento
#define TamPreenchimento 9

// Definição de quantas substâncias cabem em um frasco
#define TamSubsFrasco 4

// Definição do número máximo de frascos possíveis de existirem em um jogo: ideal: 10, setei agora 6, pois assim, serão gerados 3 Cheios e 2 Vazios na 1° rodada
#define NfrascosJogo 10

// Definição do número de frascos vazios máximo existente em cada jogo
#define TamFrascosVazios 2

// Definição do tamanho do nome do jogador
#define TamJogador 100

// Var global que controla quantas fases o jogador venceu Limite: 2
int Vitoria=0;

// Var global que controla a fase em que o jogador está
int Fase=0;

// Var global que cserve de base o número de frascos preenchidos vigentes no jogo
int nInicialFrascosPreenchidos=3;

// Var global que cserve de base o número de frascos vazios vigentes no jogo
int nInicialFrascosVazios=2;

// Definição das structs a serem usados
/*
typedef struct  {
    //int id;
    char cor;
    //char preenchimento[TamPreenchimento];
} Substancia;
*/
typedef struct {
    int id;
    int camadas;
    char subsFrasco[TamSubsFrasco];
} Frasco;

typedef struct{
    Frasco frascos[NfrascosJogo];
} Jogo;

// Cores a serem printadas:
void vermelho () {
  printf("\033[1;31m");
}

void amarelo () {
  printf("\033[1;33m");
}

void roxo () {
  printf("\033[0;35m");
}

void ciano () {
  printf("\033[0;36m");
}

// Função necessária para resetar a cor e imprimir outra cor depois
void reset () {
  printf("\033[0m");
}

void printFrasco(Frasco frascoToPrint){

    // Seto o texto de preenchimento padrão a ser usado pelo frascos 
    char textoPreenchimento[TamPreenchimento]="XXXXXXXX";

    // Seto o texto de preenchimento padrão para indicar nada a ser usado pelo frascos 
    char textoPreenchimentoVazio[TamPreenchimento]="00000000";

    // Garanto que sempre que o id do frasco for impresso, este seja branco para facilitar identificação
    reset();

     // Printo o id do frasco de uma vez para facilitar identificação
    printf("Id do frasco: %d\n\n", frascoToPrint.id);

    // Printo o texto de preenchimento a ser colorido de acordo com o número de camadas que eu tenho no frasco
    for(int i=frascoToPrint.camadas-1; i>=0; i--){

        switch (frascoToPrint.subsFrasco[i]){

            case 'v':
                vermelho();
                printf("%s\n",textoPreenchimento);
            break;

            case 'a':
                amarelo();
                printf("%s\n",textoPreenchimento);
            break;

            case 'r':
                roxo();
                printf("%s\n",textoPreenchimento);
            break;

            case 'c':
                ciano();
                printf("%s\n",textoPreenchimento);
            break;  

            case 'n':
                reset();
                printf("%s\n",textoPreenchimentoVazio);
            break;  

        }

        
    
    }

    // Printo a quebra de linha de um frasco para outro, pois, geralmente, os frascos são printados todos de uma vez
    printf("\n");
     
}

int geraId(int idsJaSorteados[], int indiceIdJaSorteado){

    for (int i=0; i<=nInicialFrascosPreenchidos + Vitoria; i++){

        // Var para controlar se o id gerado é válido
        int idNaoSorteado=1;

        // Sorteio o id de uma substância para preencher uma dada posição do frasco (0 a 10) 
        int idDaVez=rand()%10;

        // Confiro se o id já foi gerado anteriormente
        for(int j=0; j<=nInicialFrascosPreenchidos + Vitoria; j++){
            
            // Se o id do frasco já foi gerado ou é zero, quebro o loop interno e gero outro número para testar
            if(idDaVez==idsJaSorteados[j] || idDaVez==0){

                idNaoSorteado=0;
                break;

            }

            // Se não, é considerado válido
            else{

                idNaoSorteado=1;

            }
        }

        if (idNaoSorteado){

            idsJaSorteados[indiceIdJaSorteado]=idDaVez;
            return idDaVez;
            
        }
            
    }
    
}

char geraSubs(){

    // Coloquei em loop para garantir que um número entre 0 e 7 será gerado
    for(int i=0; i<100;i++){

        int corDaVez=rand()%10;

        if (corDaVez==0 || corDaVez==1){
            return 'v';
        }

        else  if (corDaVez==2 || corDaVez==3){
            return 'a';
        }

        else  if (corDaVez==4 || corDaVez==5){
            return 'r';
        }

        else  if (corDaVez==6 || corDaVez==7){
            return 'c';
        }

    }

}

Jogo Transfere(Jogo tabuleiro,int idFrascoOrigin, int idFrascoDestiny){

        int indiceFrascoOrigin,indiceFrascoDestiny;
        int primeiraCamadaVaziaDestiny, ultimaCamadaPreenchidaOrigin=5, pegouPrimeiraCamadaVaziaDestiny=5;

        // Cato, dentro do tabuleiro, os índices dos frasco de orígem e destino, respectivamente
        for(int i=(NfrascosJogo/2 + 2 * Vitoria); i>=0; i--){

            if (tabuleiro.frascos[i].id==idFrascoOrigin){
                indiceFrascoOrigin=i;
                // Testo se entrou aqui
                printf("\nEntrei em Transfere no For do idFrascoOrigin\n");
    
            }

            else if (tabuleiro.frascos[i].id==idFrascoDestiny){
                indiceFrascoDestiny=i;
                // Testo se entrou aqui
                printf("\nEntrei em Transfere no For do idFrascoDestiny\n");
                
            }

        }

        for(int i=0; i<TamSubsFrasco;i++){
            
            // Pego sempre a última camada que está preenchida no frasco de orígem, seto ultimaCamadaPreenchidaOrigin como 5 pois i nunca será 5, logo, se ela sair daqui como 5, significa que nenhuma camada está preenchida na orígem
            if (tabuleiro.frascos[indiceFrascoOrigin].subsFrasco[i]!='n'){

                ultimaCamadaPreenchidaOrigin=i;
                printf("\ni Origin: %d\n",i);

            }
            
            // Pego a primeira camada vazia no frasco de destino, seto pegouPrimeiraCamadaVaziaDestiny como 5 pois nunca i será 5, logo, se ela sair daqui com 5, significa que nenhuma camada do destino está livre
            if(tabuleiro.frascos[indiceFrascoDestiny].subsFrasco[i]=='n' && pegouPrimeiraCamadaVaziaDestiny==5){

                primeiraCamadaVaziaDestiny=i;
                pegouPrimeiraCamadaVaziaDestiny=i;
                printf("\ni Destiny: %d\n",i);

            }

        }

        // Testo se alguma camada vazia existe em Destiny
        if(pegouPrimeiraCamadaVaziaDestiny!=5 && ultimaCamadaPreenchidaOrigin!=5){

            printf("\nMovimento Válido\n\n");

            // Transfiro para o frasco de destino o que estava no de orígem
            tabuleiro.frascos[indiceFrascoDestiny].subsFrasco[primeiraCamadaVaziaDestiny]=tabuleiro.frascos[indiceFrascoOrigin].subsFrasco[ultimaCamadaPreenchidaOrigin];

            // Esvazio aquela camada do vetor de orígem
            tabuleiro.frascos[indiceFrascoOrigin].subsFrasco[ultimaCamadaPreenchidaOrigin]='n';
            
        }
        
        else{

            printf("\n\nMovimento inválido\n\n");

        }

        // Sempre após um movimento, checo se o usuário passou da fase ou não
        //checaVitoria();

        // Para atualizar o tabuleiro a cada movimento, é necessário que a função retorne-o com as modificações que foram realizadas
        return tabuleiro;
        
}

/*int checaVitoria(){


}*/


int main(){

    Jogo tabuleiro;

    char jogador[TamJogador];

    char desejoJogar;

    char trash;

    int frascosValidosGerados=0;

    int turnos=0;

    int idFrascoOrigin, idFrascoDestiny;

    // Variável para controlar o índice dos frascos gerados em tabuleiro
    int frascoGerado=0;

    // Controlo quantas camadas de substância foram geradas
    int Ncamadas; 

    // Vetor para controlar od indices já sorteados
    int idsJaSOrteados[NfrascosJogo];

    // Incialização do vetor de ids já sorteados, id==0 não é aceitável
    for (int i = 0; i < NfrascosJogo; i++){

        idsJaSOrteados[i]=0;

    }
    
    // Confirma se o jogador deseja jogar
    printf("Deseja jogar ? [S/N]: ");
    scanf("%c", &desejoJogar);

    // Pego o \n lixo que vem de cima
    scanf("%c", &trash);

    // Pego o nome do jogador
    printf("Qual seu nome ? ");
    scanf("%99[^\n]s", jogador);

    // Posteriormente os frascos serão printados, logo, dou uma quebra de linha entre este texto e os frascos do jogo.
    printf("\n");
    
    //Gero os frascos a serem preenchidos
    for(frascoGerado=0; frascoGerado<=nInicialFrascosPreenchidos + Vitoria; frascoGerado++){

        Ncamadas=0;

        // Crio o Id do frasco
        tabuleiro.frascos[frascoGerado].id=geraId(idsJaSOrteados,frascoGerado);

        // Recheio cada frasco gerado com substâncias
        for(int j=0; j<TamSubsFrasco; j++){

            tabuleiro.frascos[frascoGerado].subsFrasco[j]=geraSubs();

            Ncamadas++;
        }

        // Seto quantas camadas preenchidas cada Frasco tem
        tabuleiro.frascos[frascoGerado].camadas=Ncamadas;

    }

    // Gero os frascos vazios
    for(int i=0; i<nInicialFrascosVazios+Vitoria; i++){

        Ncamadas=0;

        // Crio o Id do frasco
        tabuleiro.frascos[frascoGerado].id=geraId(idsJaSOrteados,frascoGerado);

        // Recheio cada frasco gerado com vazio
        for(int j=0; j<TamSubsFrasco; j++){

            tabuleiro.frascos[frascoGerado].subsFrasco[j]='n';

            Ncamadas++;
        }

        // Seto quantas camadas preenchidas cada Frasco tem
        tabuleiro.frascos[frascoGerado].camadas=Ncamadas;

        // Dou update nos frascos gerados
        frascoGerado++;

    }

    while (Vitoria<=4 && desejoJogar=='s' || desejoJogar=='S') {
        
        if(turnos==0){

            // Printo a fase em que o jogador está:
            printf("FASE %d\n\n", Fase+1);

        }

        // Printo os frascos exixtentes no tabuleiro
        for(int i=0; i<=(NfrascosJogo/2 + 2*Vitoria);i++){

            printFrasco(tabuleiro.frascos[i]);

        }
        
         // Garanto que sempre estes texto ficarão na mesma cor: branco
        reset();
        
        // Pego de qual frasco o jogador deseja transferir
        printf("Digite o id do frasco de onde água será transferida: ");
        scanf("%d", &idFrascoOrigin);
       
        // Pego para qual frasco o jogador deseja transferir
        printf("\nDigite o id do frasco para onde água será transferida: ");
        scanf("%d", &idFrascoDestiny);

        // Tento efetuar a transferência
        tabuleiro=Transfere(tabuleiro,idFrascoOrigin,idFrascoDestiny);

        // Dou um sleep de 1 segundo para o usuário poder ler o resultado de seu movimento, se foi válido ou não

        #ifdef _WIN32 || _WIN64
            // Windows
            Sleep(3000); // Sleep 1 segundo
            //Sleep(500); // Sleep 0,5 segundo

        #else
            // Linux
            sleep(3); // Sleep 1 segundo
            //usleep(500*1000);  // Sleep 0,5 segundo (500 milisegundos)
            
        #endif

        // Testando se os frascos estão sendo gerados corretamente

        /*
        printf("Id do frasco %d: %d\n\n",turnos, tabuleiro.frascos[turnos].id);

        printf("Camadas do frasco %d: %d\n\n",turnos, tabuleiro.frascos[turnos].camadas);

        for(int i=0; i<tabuleiro.frascos[turnos].camadas;i++){

            printf("Cor do frasco %d em %d: %c\n",turnos, i, tabuleiro.frascos[turnos].subsFrasco[i]);

        }
        */

       
       

        if (turnos==NfrascosJogo/2 ){
            break;
        }

        turnos++; 

    }

   return 0;
        



}
    
