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
typedef struct  {
    int id;
    int cor;
    char preenchimento[TamPreenchimento];
} Substancia;

typedef struct {
    int id;
    int camadas;
    Substancia subsFrasco[TamSubsFrasco];
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

/*
    Lembrete das cores:

    frasco.subsFrasco[i].id = 0 => cor = Vermelho
    frasco.subsFrasco[i].id = 1 => cor = Amarelo
    frasco.subsFrasco[i].id = 2 => cor = Roxo
    frasco.subsFrasco[i].id = 3 => cor = Ciano 

*/

// Função necessária para resetar a cor e imprimir outra cor depois
void reset () {
  printf("\033[0m");
}

void printSubs(int indiceSub, Frasco frascoToPrint){

    for(int i=0; i<4; i++){

        printf("%s\n",frascoToPrint.subsFrasco[indiceSub].preenchimento);
    
    }
    
    // Garantindo que o texto printado saia devidamente colorido
    switch (frascoToPrint.subsFrasco[indiceSub].id){

        case 0:
            vermelho();
        break;

        case 1:
            amarelo();
        break;

        case 2:
            roxo();
        break;

        case 3:
            ciano();
        break;    
    }

}

Frasco Transfere(Frasco frascoOrigin, Frasco frascoDestiny,int idFrascoOrigin, int idFrascoDestiny){



}


int main(){

    Jogo tabuleiro;

    char textoPreenchimento[TamPreenchimento]="XXXXXXXX";

    char jogador[TamJogador];

    char desejoJogar;

    int idsJaSOrteados[TamSubsFrasco];

    // Sorteio o id de uma substância para preencher uma dada posição do frasco (0 a 10) 
    int idDaVez=rand()%10;

    // Pego o nome do jogador
    printf("Digite seu nome: ");
    scanf("%(TamJogador-1)[^\n]s", jogador);

    // Confirma se o jogador deseja jogar
    printf("Deseja jogar ? [S/N]: ");
    scanf("%c", &desejoJogar);

    for(int i=0; i<TamSubsFrasco; i++){

        // Sorteio o id de uma substância para preencher uma dada posição do frasco (0 a 10) 
        int idDaVez=rand()%10;


    }
    
    while (Vitoria<=4 && desejoJogar=='s' || desejoJogar=='S')
    {
        
    }
        



}

