/* CÓDIGO B */
#include <stdio.h> // Inclui a biblioteca padrão de entrada e saída.
#include <stdlib.h> // Inclui a biblioteca padrão de funções úteis, como alocação dinâmica de memória.
#include <string.h> // Inclui a biblioteca de manipulação de strings.
#include <fcntl.h>  // Inclui a biblioteca para operações de controle de arquivos.
#include <sys/stat.h> // Inclui a biblioteca para manipulação de informações de arquivos.
#include <mqueue.h> // Inclui a biblioteca de filas de mensagens POSIX.
#include <unistd.h> // Inclui a biblioteca para funções de sistema Unix.

#define FILA_A_PARA_B "/fila_a_para_b" // Define um nome para a fila de mensagens de A para B.
#define FILA_B_PARA_A "/fila_b_para_a" // Define um nome para a fila de mensagens de B para A.
#define FILA_ORDEM "/fila_ordem" // Define um nome para uma fila de ordens.

int main(){
mqd_t fila_a_para_b, fila_b_para_a, fila_ordem; // declara variáveis para as filas de mensagens e atributos.
struct mq_attr attr; // declara uma estrutura para atributos da fila.
int n1, n2, soma; // declara variáveis para números e soma.

    attr.mq_flags = 0;          // define as flags da fila de mensagens.
    attr.mq_maxmsg = 10;        // define o número máximo de mensagens na fila.
    attr.mq_msgsize = sizeof(int); // define o tamanho das mensagens na fila.
    attr.mq_curmsgs = 0;        // inicializa o número de mensagens na fila.

    // Abre as filas de mensagens
    if ((fila_a_para_b = mq_open(FILA_A_PARA_B, O_RDONLY)) < 0){
        perror("mq_open"); // se a abertura da fila A para B falhar, exibe uma mensagem de erro e sai.
        exit(1);
    }
    if ((fila_b_para_a = mq_open(FILA_B_PARA_A, O_WRONLY)) < 0){
        perror("mq_open"); // Se a abertura da fila B para A falhar, exibe uma mensagem de erro e sai.
        exit(1);
    }
    if ((fila_ordem = mq_open(FILA_ORDEM, O_RDONLY)) < 0){
        perror("mq_open"); // Se a abertura da fila de ordens falhar, exibe uma mensagem de erro e sai.
        exit(1);
    }

    while (1) {
        // Aguarde a ordem do processo A
        int ordem;
        if (mq_receive(fila_ordem, (char *)&ordem, sizeof(int), NULL) < 0){
            perror("mq_receive"); // Se a recepção da ordem de A falhar, exibe uma mensagem de erro e sai.
            exit(1);
        }

        // Aguarde n1 do processo A
        if (mq_receive(fila_a_para_b, (char *)&n1, sizeof(int), NULL) < 0){
            perror("mq_receive"); // Se a recepção de n1 de A falhar, exibe uma mensagem de erro e sai.
            exit(1);
        }

        // Aguarde n2 do processo A
        if (mq_receive(fila_a_para_b, (char *)&n2, sizeof(int), NULL) < 0){
            perror("mq_receive"); // Se a recepção de n2 de A falhar, exibe uma mensagem de erro e sai.
            exit(1);
        }

        soma = n1 + n2; // realize a soma dos números recebidos.

        printf("Processo B recebeu: %d e %d\n", n1, n2); // exibe os números recebidos.

        // Envie a soma de volta para o processo A
        if (mq_send(fila_b_para_a, (const char *)&soma, sizeof(int), 0) < 0){
            perror("mq_send"); // se o envio da soma para A falhar, exibe uma mensagem de erro e sai.
            exit(1);
        }

        // Aguarde 1 segundo
        sleep(1); // pausa a execução por 1 segundo.
    }

    mq_close(fila_a_para_b); // fecha a fila A para B.
    mq_close(fila_b_para_a); // fecha a fila B para A.
    mq_close(fila_ordem);    // fecha a fila de ordens.

    return 0;
}
