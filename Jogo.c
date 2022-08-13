#include <stdio.h>
#include <stdlib.h>
// Definição do tamanho do texto de preecnhimento
#define TamPreenchimento 9

// Definição de quantas substâncias cabem em um frasco
#define TamSubsFrasco 4

// Definição do número máximo de frascos possíveis de existirem em um jogo
#define NfrascosJogo 10

// Definição do tamanho do nome do jogador
#define TamJogador 100

// Var global que controla quantas fases o jogador venceu Limite: 2
int Vitoria=0;

// Var global que controla a fase em que o jogador está
int Fase=0;

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

    // Seto o texto de preenchimento padrão a ser usado
    char textoPreenchimento[TamPreenchimento]="XXXXXXXX";

    // Printo o id do frasco de uma vez para facilitar identificação
    printf("Id do frasco: %d\n\n", frascoToPrint.id);

    // Printo o texto de preenchimento a ser colorido de acordo com o número de camadas que eu tenho no frasco
    for(int i=0; i<frascoToPrint.camadas; i++){

        printf("%s\n",textoPreenchimento);

        switch (frascoToPrint.subsFrasco[i]){

            case 'v':
                vermelho();
            break;

            case 'a':
                amarelo();
            break;

            case 'r':
                roxo();
            break;

            case 'c':
                ciano();
            break;    

        }
    
    }

    // Printo a quebra de linha de um frasco para outro, pois, geralmente, os frascos são printados todos de uma vez
    printf("\n");
     
}

int geraId(int idsJaSorteados[], int indiceIdJaSorteado){

    for (int i=0; i<=NfrascosJogo/2 + Vitoria; i++){

        // Var para controlar se o id gerado é válido
        int idNaoSorteado=1;

        // Sorteio o id de uma substância para preencher uma dada posição do frasco (0 a 10) 
        int idDaVez=rand()%10;

        // Confiro se o id já foi gerado anteriormente
        for(int j=0; j<=NfrascosJogo/2 + Vitoria; j++){
            
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

void Transfere(Jogo tabuleiro,int idFrascoOrigin, int idFrascoDestiny){

        int indiceFrascoOrigin,indiceFrascoDestiny;

        // Cato, dentro do tabuleiro, o índice do frasco de orígem
        for(int i=NfrascosJogo/2 + Vitoria; i>=0; i--){

            if (tabuleiro.frascos[i].id==idFrascoOrigin){
                indiceFrascoOrigin=i;
                break;
            }

        }

        // Cato, dentro do tabuleiro, o índice do frasco de destino
        for(int i=NfrascosJogo/2 + Vitoria; i>=0; i--){

            if (tabuleiro.frascos[i].id==idFrascoDestiny){
                indiceFrascoDestiny=i;
                break;
            }

        }

        // Consideradno que já há algo no frasco
        if(tabuleiro.frascos[indiceFrascoDestiny].camadas<=3){

            int indiceUltimaSubsOrigin=TamSubsFrasco-1,indiceUltimaSubsDestiny=TamSubsFrasco-1;

            // Coleto o index onde tem a última substância no frasco de orígem n == vazio, nada
            while(tabuleiro.frascos[indiceFrascoOrigin].subsFrasco[indiceUltimaSubsOrigin]=='n'){

                indiceUltimaSubsOrigin--;
            }

            // Coleto o index onde tem a última substância no frasco de destino
              while(tabuleiro.frascos[indiceFrascoDestiny].subsFrasco[indiceFrascoDestiny]=='n'){

                indiceUltimaSubsDestiny--;
            }

            if (tabuleiro.frascos[indiceFrascoOrigin].subsFrasco[indiceUltimaSubsOrigin]==tabuleiro.frascos[indiceFrascoDestiny].subsFrasco[indiceUltimaSubsDestiny]){

                // Transfiro para o frasco de destino o que estava no de orígem
                tabuleiro.frascos[indiceFrascoDestiny].subsFrasco[indiceUltimaSubsDestiny+1]=tabuleiro.frascos[indiceFrascoOrigin].subsFrasco[indiceUltimaSubsOrigin];

                // Esvazio aquela camada do vetor de orígem
                tabuleiro.frascos[indiceFrascoOrigin].subsFrasco[indiceUltimaSubsOrigin]='n';
            }
            
        }

}


int main(){

    Jogo tabuleiro;

    char jogador[TamJogador];

    char desejoJogar;

    char trash;

    int frascosValidosGerados=0;

    int turnos=0;

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
    for(int i=0; i<=NfrascosJogo/2 + Vitoria; i++){

        Ncamadas=0;

        tabuleiro.frascos[i].id=geraId(idsJaSOrteados,i);

        for(int j=0; j<TamSubsFrasco; j++){

            tabuleiro.frascos[i].subsFrasco[j]=geraSubs();

            Ncamadas++;
        }

        // Seto quantas camadas preenchidas cada Frasco tem
        tabuleiro.frascos[i].camadas=Ncamadas;

    }

    // Gero as substâncias para rechear cada frasco 

    while (Vitoria<=4 && desejoJogar=='s' || desejoJogar=='S') {
        
        // Testando se os frascos estão sendo gerados corretamente

        /*
        printf("Id do frasco %d: %d\n\n",turnos, tabuleiro.frascos[turnos].id);

        printf("Camadas do frasco %d: %d\n\n",turnos, tabuleiro.frascos[turnos].camadas);

        for(int i=0; i<tabuleiro.frascos[turnos].camadas;i++){

            printf("Cor do frasco %d em %d: %c\n",turnos, i, tabuleiro.frascos[turnos].subsFrasco[i]);

        }
        */


       printFrasco(tabuleiro.frascos[turnos]);
       
       //printf("\n\n");

        if (turnos==NfrascosJogo/2 - 1){
            break;
        }
        

        turnos++; 

    }
    

   return 0;
        



}
