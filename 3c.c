/* TERCEIRO CÓDIGO DE CONTROLE */
#include <stdio.h> // Inclui a biblioteca padrão de entrada e saída.
#include <stdlib.h> // Inclui a biblioteca padrão de funções úteis, como alocação dinâmica de memória.
#include <string.h> // Inclui a biblioteca de manipulação de strings.
#include <fcntl.h>  // Inclui a biblioteca para operações de controle de arquivos.
#include <sys/stat.h> // Inclui a biblioteca para manipulação de informações de arquivos.
#include <mqueue.h> // Inclui a biblioteca de filas de mensagens POSIX.

#define FILA_A_PARA_B "/fila_a_para_b" // Define um nome para a fila de mensagens de A para B.
#define FILA_B_PARA_A "/fila_b_para_a" // Define um nome para a fila de mensagens de B para A.
#define FILA_ORDEM "/fila_ordem" // Define um nome para uma fila de ordens.

int main(){
    struct mq_attr attr; // declara uma estrutura para atributos da fila.
    attr.mq_flags = 0;          // define as flags da fila de mensagens.
    attr.mq_maxmsg = 10;        // define o número máximo de mensagens na fila.
    attr.mq_msgsize = sizeof(int); // define o tamanho das mensagens na fila.
    attr.mq_curmsgs = 0;        // inicializa o número de mensagens na fila.

    mqd_t fila_a_para_b, fila_b_para_a, fila_ordem; // declara variáveis para as filas de mensagens.

    // Crie as filas de mensagens
    fila_a_para_b = mq_open(FILA_A_PARA_B, O_CREAT | O_RDWR, 0666, &attr); // Cria a fila A para B.
    if (fila_a_para_b < 0){
        perror("mq_open"); // se a criação da fila A para B falhar, exibe uma mensagem de erro e sai.
        exit(1);
    }

    fila_b_para_a = mq_open(FILA_B_PARA_A, O_CREAT | O_RDWR, 0666, &attr); // Cria a fila B para A.
    if (fila_b_para_a < 0){
        perror("mq_open"); // se a criação da fila B para A falhar, exibe uma mensagem de erro e sai.
        exit(1);
    }

    fila_ordem = mq_open(FILA_ORDEM, O_CREAT | O_RDWR, 0666, &attr); // cria a fila de ordens.
    if (fila_ordem < 0){
        perror("mq_open"); // se a criação da fila de ordens falhar, exibe uma mensagem de erro e sai.
        exit(1);
    }

    // Feche as filas de mensagens após criá-las
    mq_close(fila_a_para_b); // fecha a fila A para B.
    mq_close(fila_b_para_a); // fecha a fila B para A.
    mq_close(fila_ordem);    // fecha a fila de ordens.

    return 0;
}