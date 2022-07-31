#include <stdio.h>
#include <stdlib.h>
// Definição do tamanho do texto de preecnhimento
#define TamPreenchimento 9

// Definição de quantas substâncias cabem em um frasco
#define TamSubsFrasco 4

// Definição do número máximo de frascos possíveis de existirem em um jogo
#define NfrascosJogo 5

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

            // Coleto o index onde tem a última substância no frasco de orígem
            while(tabuleiro.frascos[indiceFrascoOrigin].subsFrasco!='v'){

                indiceUltimaSubsOrigin--;
            }

            // Coleto o index onde tem a última substância no frasco de destino
              while(tabuleiro.frascos[indiceFrascoDestiny].subsFrasco!='v'){

                indiceUltimaSubsDestiny--;
            }

            if (tabuleiro.frascos[indiceFrascoOrigin].subsFrasco[indiceUltimaSubsOrigin]==tabuleiro.frascos[indiceFrascoDestiny].subsFrasco[indiceUltimaSubsDestiny]){

                // Transfiro para o frasco de destino o que estava no de orígem
                tabuleiro.frascos[indiceFrascoDestiny].subsFrasco[indiceUltimaSubsDestiny+1]=tabuleiro.frascos[indiceFrascoOrigin].subsFrasco[indiceUltimaSubsOrigin];

                // Esvazio aquela camada do vetor de orígem
                tabuleiro.frascos[indiceFrascoOrigin].subsFrasco[indiceUltimaSubsOrigin]='v';
            }
            
        }

}


int main(){

    Jogo tabuleiro;

    char textoPreenchimento[TamPreenchimento]="XXXXXXXX";

    char jogador[TamJogador];

    char desejoJogar;

    int frascosValidosGerados=0;

    // Controlo quantas camadas de substância foram geradas
    int Ncamadas; 
    // Vetor para controlar od indices já sorteados
    int idsJaSOrteados[NfrascosJogo]=0;

    // Pego o nome do jogador
    printf("Digite seu nome: ");
    scanf("%(TamJogador-1)[^\n]s", jogador);

    // Confirma se o jogador deseja jogar
    printf("Deseja jogar ? [S/N]: ");
    scanf("%c", &desejoJogar);

    //Gero os frascos a serem preenchidos
    for(int i=0; i=<NfrascosJogo/2 + Vitoria; i++){

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

    
    while (Vitoria<=4 && desejoJogar=='s' || desejoJogar=='S')
    {
        
    }
        



}
