// Include das bibliotecas padrão
#include <stdio.h>
#include <stdlib.h>

// Include das bibliotecas para o sleep funcionar
#ifdef _WIN32 || _WIN64
    #include <Windows.h>
#else
    #include <unistd.h>
#endif

// Definição do número de fases desejadas (Para apresentar setar NFasesDesejadas = 0)
#define NFasesDesejadas 3

// Definição do tamanho do texto de preecnhimento
#define TamPreenchimento 9

// Definição de quantas substâncias cabem em um frasco
#define TamSubsFrasco 4

// Definição do número máximo de frascos possíveis de existirem em um jogo: ideal: 10, setei agora 6, pois assim, serão gerados 3 Cheios e 2 Vazios na 1° rodada
#define NfrascosJogo 12

// Definição do tamanho do nome do jogador
#define TamJogador 100

// Var global que controla quantas fases o jogador venceu Limite: 2
int Vitoria=0;

// Var global que controla a fase em que o jogador está
int Fase=0;

// Var global que cserve de base o número de frascos preenchidos vigentes no jogo
int nInicialFrascosPreenchidos=3;

// Var global que cserve de base o número de frascos vazios vigentes no jogo
int nInicialFrascosVazios=3;

// Definição das structs a serem usados
typedef struct {
    int id;
    int camadas;
    char subsFrasco[TamSubsFrasco];
} Frasco;

typedef struct{
    Frasco frascos[NfrascosJogo];
} Jogo;

// Cores a serem printadas
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
    char textoPreenchimentoVermelho[TamPreenchimento]="VVVVVVVV";

    char textoPreenchimentoAmarelo[TamPreenchimento]="AAAAAAAA";

    char textoPreenchimentoRoxo[TamPreenchimento]="RRRRRRRR";

    char textoPreenchimentoCiano[TamPreenchimento]="CCCCCCCC";

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
                printf("%s\n",textoPreenchimentoVermelho);
            break;

            case 'a':
                amarelo();
                printf("%s\n",textoPreenchimentoAmarelo);
            break;

            case 'r':
                roxo();
                printf("%s\n",textoPreenchimentoRoxo);
            break;

            case 'c':
                ciano();
                printf("%s\n",textoPreenchimentoCiano);
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

        // Sorteio o id de uma substância para preencher uma dada posição do frasco (0 a 100)
        int idDaVez=rand()%100;

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

    // Coloquei em loop para garantir que um número entre 0 e 7 será gerado para garantir que sempre serão geradas cores válidas para um dado frasco
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

void checaVitoria(Jogo tabuleiro, char jogador[]){

    int nFrascosUniformes=0, nDiferencas=0;
    char subsDaVez;

    for(int i=0; i<nInicialFrascosPreenchidos + Vitoria;i++){

        for(int j=0;j<TamSubsFrasco;j++){

            // Pego, no 1° loop, a cor da 1° substância
            if (j==0){

                subsDaVez=tabuleiro.frascos[i].subsFrasco[j];
                continue;

            }

            // Caso alguma subtância do mesmo frasco tenha cor diferente, ou seja, seja diferente, dou update na variável que armazena a quanitdade de cores diferentes encontradas, nesse caso não considero n, vazio, como diferente,
            // pois como todas as cores são geradas aleatoriamente dentro de um frasco, não posso assegurar que terão 4 camadas para cada substância no fim, logo, não considero vazio como algo "diferente"
            else if(subsDaVez!=tabuleiro.frascos[i].subsFrasco[j] && tabuleiro.frascos[i].subsFrasco[j]!='n' ){

                nDiferencas++;

            }

        }

        // Caso entre as substâncias do frasco não haja nenhuma dissonância/diferença, então dou update no número de frascos que estão uniformemente coloridos/preenchidos
        if (nDiferencas==0){

            nFrascosUniformes++;

        }

    }

    // Caso o jogador tenha conseguido organizar uniformemente um número de frascos == número incial de frascos que ele tinha, dependendo da fase em que estava, significa que ele organizou todos os frascos que precisava
    if(nFrascosUniformes==nInicialFrascosPreenchidos + Vitoria){

        printf("Parabens %s, voce passou da Fase %d\n\n", jogador, Vitoria+1);

        // Printo os frascos exixtentes no tabuleiro para o jogador entender porque venceu
        for(int i=0; i<(NfrascosJogo/2 + 2*Vitoria);i++){

            printFrasco(tabuleiro.frascos[i]);

        }

        // Atualizar a variável global aqui fará com que a fase atual se encerre e o jogador passe para a próxima
        Vitoria++;

        // Atualizo o número da fase
        Fase++;

        // Dou um sleep de 4 segundos para o usuário poder ler o texto e entender que ganhou
        #ifdef _WIN32 || _WIN64
            // Windows
            Sleep(4000); // Sleep 4 segundos

            #else
            // Linux
            sleep(4); // Sleep 4 segundos

            #endif

    }

}

Jogo Transfere(Jogo tabuleiro,int idFrascoOrigin, int idFrascoDestiny, char jogador[]){

        int indiceFrascoOrigin=-1,indiceFrascoDestiny=-1;
        int primeiraCamadaVaziaDestiny, ultimaCamadaPreenchidaOrigin=5, pegouPrimeiraCamadaVaziaDestiny=5;

        // Cato, dentro do tabuleiro, os índices dos frasco de orígem e destino, respectivamente
        for(int i=(NfrascosJogo/2 + 2 * Vitoria); i>=0; i--){

            if (tabuleiro.frascos[i].id==idFrascoOrigin){
                indiceFrascoOrigin=i;

            }

            else if (tabuleiro.frascos[i].id==idFrascoDestiny){
                indiceFrascoDestiny=i;

            }

        }

        // Caso nenhum um dos ids exista, já retorno o tabuleiro sem modificações para evitar erros posteriores
        if (indiceFrascoOrigin==-1 || indiceFrascoOrigin==-1 ){

             printf("\nMovimento Inválido\n\n");

            // Se for inválido, retorno o tabuleiro sem nenhuma alteração para garantir o fluxo do programa
            return tabuleiro;

        }

        for(int i=0; i<TamSubsFrasco;i++){

            // Pego sempre a última camada que está preenchida no frasco de orígem, seto ultimaCamadaPreenchidaOrigin como 5 pois i nunca será 5, logo, se ela sair daqui como 5, significa que nenhuma camada está preenchida na orígem
            if (tabuleiro.frascos[indiceFrascoOrigin].subsFrasco[i]!='n'){

                ultimaCamadaPreenchidaOrigin=i;

            }

            // Pego a primeira camada vazia no frasco de destino, seto pegouPrimeiraCamadaVaziaDestiny como 5 pois nunca i será 5, logo, se ela sair daqui com 5, significa que nenhuma camada do destino está livre
            if(tabuleiro.frascos[indiceFrascoDestiny].subsFrasco[i]=='n' && pegouPrimeiraCamadaVaziaDestiny==5){

                primeiraCamadaVaziaDestiny=i;
                pegouPrimeiraCamadaVaziaDestiny=i;

            }

        }

        // Testo se alguma camada vazia existe em Destiny
        if(pegouPrimeiraCamadaVaziaDestiny!=5 && ultimaCamadaPreenchidaOrigin!=5){

            printf("\nMovimento Valido\n\n");

            // Transfiro para o frasco de destino o que estava no de orígem
            tabuleiro.frascos[indiceFrascoDestiny].subsFrasco[primeiraCamadaVaziaDestiny]=tabuleiro.frascos[indiceFrascoOrigin].subsFrasco[ultimaCamadaPreenchidaOrigin];

            // Esvazio aquela camada do vetor de orígem
            tabuleiro.frascos[indiceFrascoOrigin].subsFrasco[ultimaCamadaPreenchidaOrigin]='n';

        }

        else{

            // Printo se o movimento foi inválido
            printf("\nMovimento Invalido\n\n");

        }

        // Sempre após um movimento, checo se o jogador passou da fase ou não
        checaVitoria(tabuleiro, jogador);

        // Para atualizar o tabuleiro a cada movimento, é necessário que a função retorne-o com as modificações que foram realizadas para a main
        return tabuleiro;

}


int main(){

    // Declaração da função responsável por controlar acerca de todos os elementos do jogo
    Jogo tabuleiro;

    // Declaro a variável para pegar o nome do jogador
    char jogador[TamJogador];

    // Declaro a variável para pegar o desejo do jogador em jogar ou não
    char desejoJogar;

    // Declaro uma variável responsável por garantir que, na leitura de strings, o \n lixo não seja empecilho
    char trash;

    // Declaro a variável responsável por controlar quantos frascos gerados foram válidos, pois desejo assegurar que a fase tenha o número exato de frascos que eu estipulei
    int frascosValidosGerados=0;

    // Declaro as variáveis responsáveis por armazenar os ids dos frascos que o usuário deseja mover
    int idFrascoOrigin, idFrascoDestiny;

    // Variável para controlar o índice dos frascos gerados em tabuleiro
    int frascoGerado=0;

    // Controlo quantas camadas de substância foram geradas
    int Ncamadas;

    // Vetor para controlar od indices já sorteados
    int idsJaSOrteados[NfrascosJogo];

    // Pego o nome do jogador
    printf("Qual seu nome ? ");
    scanf("%99[^\n]s", jogador);

    // Pego o \n lixo que vem de cima
    scanf("%c", &trash);

    // Confirma se o jogador deseja jogar
    printf("Deseja jogar ? [s/n]: ");
    scanf("%c", &desejoJogar);

    // Posteriormente os frascos serão printados, logo, dou uma quebra de linha entre este texto e os frascos do jogo.
    printf("\n");

    // INÍCIO DO JOGO

    while (Vitoria<NFasesDesejadas && (desejoJogar=='s' || desejoJogar=='S')){

        // Declaro a variável responsável por identificar quando uma fase é concluída
        int valorInicialVitoria=Vitoria;

        // Incialização do vetor de ids já sorteados, id==0 não é aceitável
        for (int i = 0; i < NfrascosJogo; i++){

            idsJaSOrteados[i]=0;

        }

        //Gero os frascos a serem preenchidos
        for(frascoGerado=0; frascoGerado<nInicialFrascosPreenchidos + Vitoria; frascoGerado++){

            // Reseto Ncamadas a cada loop
            Ncamadas=0;

            // Crio o Id do frasco
            tabuleiro.frascos[frascoGerado].id=geraId(idsJaSOrteados,frascoGerado);

            // Recheio cada frasco gerado com substâncias a
            for(int j=0; j<TamSubsFrasco; j++){

                tabuleiro.frascos[frascoGerado].subsFrasco[j]=geraSubs();

                Ncamadas++;
            }

            // Seto quantas camadas preenchidas cada Frasco tem
            tabuleiro.frascos[frascoGerado].camadas=Ncamadas;

        }

        // Gero os frascos vazios
        for(int i=0; i<nInicialFrascosVazios+Vitoria; i++){

            // Reseto Ncamadas a cada loop
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

        // Printo a fase em que o jogador está:
        printf("\nFASE %d\n\n", Fase+1);

        // ÍNÍCIO DA FASE

        while (valorInicialVitoria==Vitoria) {

            // Printo os frascos exixtentes no tabuleiro
            for(int i=0; i<(NfrascosJogo/2 + 2*Vitoria);i++){

                printFrasco(tabuleiro.frascos[i]);

            }

            // Garanto que sempre estes texto ficarão na mesma cor: branco
            reset();

            // Pego de qual frasco o jogador deseja transferir
            printf("Digite o id do frasco de onde agua sera transferida: ");
            scanf("%d%*c", &idFrascoOrigin);

            // Pego para qual frasco o jogador deseja transferir
            printf("\nDigite o id do frasco para onde agua sera transferida: ");
            scanf("%d%*c", &idFrascoDestiny);

            // Tento efetuar a transferência
            tabuleiro=Transfere(tabuleiro,idFrascoOrigin,idFrascoDestiny, jogador);

            // Dou um sleep de 0,5 segundo para o usuário poder ler o resultado de seu movimento, se foi válido ou não

            #ifdef _WIN32 || _WIN64
                // Windows
                Sleep(500); // Sleep 0,5 segundo

            #else
                // Linux
                usleep(500*1000);  // Sleep 0,5 segundo (500 milisegundos)

            #endif

        }

        // Caso Vitoria != 0, significa que mudou de Fase, logo, é necessário perguntar novamente se a pessoa quer ou não jogar, da 2° Fase em diante, esse ciclo se autosustenta
        if (Vitoria!=0){

            // Garanto que sempre estes texto ficarão na mesma cor: branco
            reset();

            // Evito que se o cara ganhou ele seja perguntado
            if(Vitoria==NFasesDesejadas){
                break;
            }

            // Confirma se o jogador deseja jogar
            printf("Deseja continuar jogando ? [s/n]: ");
            scanf("%c", &desejoJogar);

        }

    }

    // Se o loop do jogo quebrou porque o usuário passou das 3 fases, coloco um vídeo para comemorar sua vitória
    if(Vitoria==NFasesDesejadas){

        printf("\n");
        printf("Voce Conluiiu Todas As Fases PARABENS...");

        #ifdef _WIN32 || _WIN64
            // Windows
            system("start https://www.youtube.com/watch?v=z37Vhpa5X5I&ab_channel=Gambiarra");

        #else
            // Linux
            system("xdg-open https://www.youtube.com/watch?v=z37Vhpa5X5I&ab_channel=Gambiarra");

        #endif

    }

    // Se o loop do jogo quebrou porque o usuário não quis mais jogar, coloco um vídeo para demonstrar minha tristeza com o tal fato
    else{

        printf("\n");
        printf("Que pena, o jogo era legal...");

        #ifdef _WIN32 || _WIN64

            // Windows
            Sleep(500);
            system("start https://www.youtube.com/watch?v=gjD85J3ZOIs&ab_channel=AnisulOny");

        #else

            // Linux
            usleep(500*1000);
            system("xdg-open https://www.youtube.com/watch?v=gjD85J3ZOIs&ab_channel=AnisulOny");

        #endif

    }

   return 0;

}
