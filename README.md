# Aplicação Cliente/Servidor de Cadastro de Pessoas

Este projeto implementa uma aplicação cliente-servidor para gerenciar um cadastro de pessoas. O servidor, desenvolvido em Java, é responsável por processar as requisições e persistir os dados. O cliente, desenvolvido em C, fornece uma interface de linha de comando para o usuário interagir com o sistema.

## Funcionalidades

- **Servidor Multithread**: O servidor Java é capaz de atender a múltiplos clientes simultaneamente, criando uma nova thread para cada conexão.
- **Cliente Interativo**: O cliente C oferece um menu simples para que o usuário possa escolher entre inserir uma nova pessoa ou listar as existentes.
- **Persistência de Dados**: As informações das pessoas são salvas em um arquivo `pessoas.json` no lado do servidor.
- **Comunicação via JSON**: O protocolo de comunicação utiliza o formato JSON para serializar e desserializar os dados.

## Tecnologias Utilizadas

#### Servidor
- **Linguagem**: Java (JDK 8 ou superior)
- **Biblioteca**: `org.json` (fornecida no arquivo `json.jar`)
- **Rede**: Sockets Java (`java.net`)

#### Cliente
- **Linguagem**: C
- **Biblioteca**: `json-c`
- **Rede**: Sockets POSIX (`arpa/inet.h`)

## Como Compilar e Executar

Você precisará de dois terminais: um para o servidor e outro para o cliente.

### 1. Servidor (Java)

No **primeiro terminal**:

```bash
# Compile o código (o arquivo json.jar deve estar no mesmo diretório)
javac -cp .:json.jar Servidor.java

# Execute o servidor
java -cp .:json.jar Servidor
```

### 2. Cliente (C)

No **segundo terminal**:

```bash
# Compile o código, linkando a biblioteca json-c
gcc cliente.c -o cliente -ljson-c

# Execute o cliente
./cliente
```

## Arquivos do Projeto

O projeto é composto pelos seguintes arquivos-fonte:

- `Servidor.java`: Código-fonte do servidor em Java.
- `cliente.c`: Código-fonte do cliente em C.
- `json.jar`: Biblioteca de dependência do servidor.
- `.gitignore`: Arquivo para controle de versão.
- `pessoas.json`: Arquivo de dados (exemplo).