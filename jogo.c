#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

int l, c, linha, coluna, jogador, ganhou, jogadas, opcao;
char jogo[3][3];
sem_t sem; 

void* jogador_func(void* arg) {
    int jogador_local = *(int*)arg;

    while (ganhou == 0 && jogadas < 9) {
        sem_wait(&sem); 

        if (jogadas >= 9 || ganhou != 0) {
            sem_post(&sem); 
            break;
        }

        printf("\n\n\t 0   1   2\n\n");
        for (l = 0; l < 3; l++) {
            for (c = 0; c < 3; c++) {
                if (c == 0)
                    printf("\t");
                printf(" %c ", jogo[l][c]);
                if (c < 2)
                    printf("|");
                if (c == 2)
                    printf("  %d", l);
            }
            if (l < 2)
                printf("\n\t-----------");
            printf("\n");
        }


        do {
            printf("JOGADOR %d: Digite a linha e a coluna que deseja jogar:\n", jogador_local);
            scanf("%d%d", &linha, &coluna);
        } while (linha < 0 || linha > 2 || coluna < 0 || coluna > 2 || jogo[linha][coluna] != ' ');

        jogo[linha][coluna] = (jogador_local == 1) ? '0' : 'X';
        jogadas++;

        if ((jogo[0][0] == '0' && jogo[0][1] == '0' && jogo[0][2] == '0') || 
            (jogo[1][0] == '0' && jogo[1][1] == '0' && jogo[1][2] == '0') ||
            (jogo[2][0] == '0' && jogo[2][1] == '0' && jogo[2][2] == '0') ||
            (jogo[0][0] == '0' && jogo[1][0] == '0' && jogo[2][0] == '0') ||
            (jogo[0][1] == '0' && jogo[1][1] == '0' && jogo[2][1] == '0') ||
            (jogo[0][2] == '0' && jogo[1][2] == '0' && jogo[2][2] == '0') ||
            (jogo[0][0] == '0' && jogo[1][1] == '0' && jogo[2][2] == '0') ||
            (jogo[0][2] == '0' && jogo[1][1] == '0' && jogo[2][0] == '0')) {
            printf("\nParabens! O jogador 1 venceu!\n");
            ganhou = 1;
        }

        if ((jogo[0][0] == 'X' && jogo[0][1] == 'X' && jogo[0][2] == 'X') || 
            (jogo[1][0] == 'X' && jogo[1][1] == 'X' && jogo[1][2] == 'X') ||
            (jogo[2][0] == 'X' && jogo[2][1] == 'X' && jogo[2][2] == 'X') ||
            (jogo[0][0] == 'X' && jogo[1][0] == 'X' && jogo[2][0] == 'X') ||
            (jogo[0][1] == 'X' && jogo[1][1] == 'X' && jogo[2][1] == 'X') ||
            (jogo[0][2] == 'X' && jogo[1][2] == 'X' && jogo[2][2] == 'X') ||
            (jogo[0][0] == 'X' && jogo[1][1] == 'X' && jogo[2][2] == 'X') ||
            (jogo[0][2] == 'X' && jogo[1][1] == 'X' && jogo[2][0] == 'X')) {
            printf("\nParabens! O jogador 2 venceu!\n");
            ganhou = 2;
        }

        sem_post(&sem); 
    }

    return NULL;
}

void mostrar_instrucoes() {
    printf("\nInstrucoes do Jogo:\n");
    printf("1. O jogo e para dois jogadores.\n");
    printf("2. Os jogadores se alternam para marcar uma celula vazia no tabuleiro.\n");
    printf("3. O primeiro jogador a alinhar tres de seus simbolos (horizontalmente, verticalmente ou diagonalmente) vence.\n");
    printf("4. Se todas as celulas estiverem preenchidas e nenhum jogador tiver alinhado tres simbolos, o jogo termina em empate.\n");

    int sair_instrucoes;
    do {
        printf("\nDigite 0 para voltar ao menu: ");
        scanf("%d", &sair_instrucoes);
    } while (sair_instrucoes != 0);
}

void mostrar_menu() {
    do {
        printf("\nMenu:\n");
        printf("1. Comecar Jogo\n");
        printf("2. Instrucoes\n");
        printf("0. Sair\n");
        printf("Digite sua opcao: ");
        scanf("%d", &opcao);

           switch(opcao) {
            case 1:
                return; 
            case 2:
                mostrar_instrucoes();
                break;
            case 0:
                printf("Saindo...\n");
                exit(0);
            default:
                printf("Opcao invalida. Tente novamente.\n");
        }
    } while (opcao != 1);
}

int main() {
    pthread_t thread1, thread2; 
    int jogador1 = 1, jogador2 = 2;

    mostrar_menu(); 

    do {
        jogador = 1;
        ganhou = 0;
        jogadas = 0;

        for (l = 0; l < 3; l++) {
            for (c = 0; c < 3; c++) {
                jogo[l][c] = ' ';
            }
        }

        sem_init(&sem, 0, 1); 

        pthread_create(&thread1, NULL, jogador_func, &jogador1); 
        pthread_create(&thread2, NULL, jogador_func, &jogador2); 

        pthread_join(thread1, NULL); 
        pthread_join(thread2, NULL);

        sem_destroy(&sem); 

        if (ganhou == 0) {
            printf("\nO jogo finalizou sem ganhador!\n");
        }

        printf("\nDigite 1 para jogar novamente:\n");
        printf("Digite 2 para ir ao menu:\n");
        scanf("%d", &opcao); 
    } 
    while (opcao == 1);
    while (opcao == 2)
    {
        mostrar_menu(); 
    }
    return 0;
}