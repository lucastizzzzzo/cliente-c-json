#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <json-c/json.h>

typedef struct {
    int id;
    char cpf[12];
    char nome[60];
    int  idade;
    char profissao[60];
} Pessoa;

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

    Pessoa p;
    int escolha;

// Loop de leitura dos dados da pessoa
    while (1) {

        printf("Digite 1 para inserir uma nova pessoa\n");
        printf("Digite 2 para listar todas pessoas\n");

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

        } elsif (escolha == 2) {
            
            printf("Listando todas as pessoas...\n");
        } else {
            printf("Opção inválida. Tente novamente.\n");
            continue;
        }
   
    }
    
    return 0;     

}


