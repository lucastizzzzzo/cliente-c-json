#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <json-c/json.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 4096

typedef struct {
    int id;
    char cpf[12];
    char nome[60];
    int  idade;
    char profissao[60];
} Pessoa;


// Desserializa JSON array recebido do servidor e imprime
void listar_pessoas(const char *json_str) {
    struct json_object *jarray = json_tokener_parse(json_str);
    if (!jarray) {
        printf("Erro ao parsear JSON recebido do servidor.\n");
        return;
    }

    int n = json_object_array_length(jarray);
    if (n == 0) {
        printf("Nenhuma pessoa cadastrada.\n");
    } else {
        printf("Lista de pessoas cadastradas:\n");
        for (int i = 0; i < n; i++) {
            struct json_object *jobj = json_object_array_get_idx(jarray, i);
            int id = json_object_get_int(json_object_object_get(jobj, "id"));
            const char *cpf = json_object_get_string(json_object_object_get(jobj, "cpf"));
            const char *nome = json_object_get_string(json_object_object_get(jobj, "nome"));
            int idade = json_object_get_int(json_object_object_get(jobj, "idade"));
            const char *profissao = json_object_get_string(json_object_object_get(jobj, "profissao"));

            printf("ID: %d | CPF: %s | Nome: %s | Idade: %d | Profissão: %s\n",
                   id, cpf, nome, idade, profissao);
        }
    }

    json_object_put(jarray); // libera memória
}

// Função para serializar Pessoa em JSON
char* pessoa_para_json(Pessoa *p) {
    struct json_object *jobj = json_object_new_object();

    json_object_object_add(jobj, "id", json_object_new_int(p->id));
    json_object_object_add(jobj, "cpf", json_object_new_string(p->cpf));
    json_object_object_add(jobj, "nome", json_object_new_string(p->nome));
    json_object_object_add(jobj, "idade", json_object_new_int(p->idade));
    json_object_object_add(jobj, "profissao", json_object_new_string(p->profissao));

    // Retorna uma string JSON (não precisa liberar manualmente se usar diretamente)
    return strdup(json_object_to_json_string_ext(jobj, JSON_C_TO_STRING_PRETTY));
}


int main(){


    char *ip = "127.0.0.1";
    char buffer[BUFFER_SIZE];
    int porta = 1500;

    printf("Cliente iniciado na porta %d\n", porta);


    int sockfd;
    struct sockaddr_in servaddr;

   sockfd = socket(AF_INET, SOCK_STREAM, 0);
    
    if (sockfd < 0) {
        perror("Erro ao criar socket");
        exit(1);
    }

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(porta);
    servaddr.sin_addr.s_addr = inet_addr(ip);

    if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        perror("Erro ao conectar ao servidor");
        close(sockfd);
        exit(1);
    }

    printf("Cliente conectado ao servidor %s:%d ✅\n", ip, porta);


    while (1) {

        printf("\nDigite 1 para inserir uma nova pessoa\n");
        printf("Digite 2 para listar todas pessoas\n");
        printf("Ou 0 para sair");
        
        Pessoa p;
        int escolha;

        scanf("%d", &escolha);
        if (escolha == 1) {

            printf("Digite o ID: ");
            scanf("%d", &p.id);
            
            printf("Digite o CPF: ");
            scanf("%s", p.cpf);
            
            printf("Digite o Nome: ");
            scanf(" %[^\n]", p.nome); // Lê uma string com espaços
            
            printf("Digite a Idade: ");
            scanf("%d", &p.idade);
            
            printf("Digite a Profissão: ");
            scanf(" %[^\n]", p.profissao); // Lê uma string com espaços
            
            //Pasear pessoa como json para um servidor localhost na porta 1500
            printf("Pessoa inserida com sucesso!\n");

            // Criar socket 
            int sockfd;
            struct sockaddr_in servaddr;

            sockfd = socket(AF_INET, SOCK_STREAM, 0);
            if (sockfd < 0) {
                perror("Erro ao criar socket");
                exit(1);
            }

            char *json_str = pessoa_para_json(&p);

            // Envia comando INSERIR + JSON
            snprintf(buffer, BUFFER_SIZE, "INSERIR %s", json_str);
            send(sockfd, buffer, strlen(buffer), 0);

            printf("Pessoa enviada ao servidor!\n");
            free(json_str);

        } else if (escolha == 2) {
           
            strcpy(buffer, "LISTAR");
            send(sockfd, buffer, strlen(buffer), 0);

            // Recebe resposta do servidor
            memset(buffer, 0, BUFFER_SIZE);
            int len = recv(sockfd, buffer, BUFFER_SIZE - 1, 0);
            if (len > 0) {
                buffer[len] = '\0';
                listar_pessoas(buffer); // Desserializa e imprime
            } else {
                printf("Erro ao receber dados do servidor.\n");
            }
        }else if (escolha == 0) {
            printf("Saindo...\n");
            break;  

        }
        
        else {
            printf("Opção inválida. Tente novamente.\n");
            continue;
        }
   
    }

    close(sockfd);
    return 0;     

}


