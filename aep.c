#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define FILE_NAME "usuarios.txt"
#define KEY 0xAA  // Criptografa usando Hexadecimal

typedef struct {
    int id;
    char nome[50];
} Usuario; //criação de uma estrutura chamada usuário, para evitar criação de múltiplas strings

void criptografar(char *info) {
    while (*info) 
    {
        *info ^= KEY;
        info++;
    }
} // criação de uma criptografia simples

void adicionarUsuario() {
    FILE *file = fopen(FILE_NAME, "ab"); // abre o arqivo para leitura e escrita em binario
    Usuario usuario; // criação da variavel usuário com a estrutura de Usuario, definido no começo do código
    printf("Digite o ID do usuario: ");
    scanf("%d", &usuario.id); //leitura do id
    printf("Digite o nome do usuario: ");
    scanf("%s", usuario.nome); //leitura do nome
    criptografar((char *)&usuario); // (char*) transforma a variavel usuario.id e usuario.nome em um único char e criptografa
    fwrite(&usuario, sizeof(Usuario), 1, file); 
    //escreve no arquivo a variavel usuário que agora é um ponteiro que permite a escrita byte a byte
    //sizeof(Usuario) pega o tamanho em bytes do Usuario(typedef) para separação correta no arquivo txt
    // o 1 representa quantas vezes vamos fazer isso
    // file é o nome da váriavel escolhida, utilizado em ingles por padrão, mas pode ser em portugues também
    fclose(file);//fecha o arquivo
    printf("Usuario adicionado com sucesso!\n");
}

void alterarUsuario() { // funções iguais as da função alterarUsuario não serão explicadas novamente
    FILE *file = fopen(FILE_NAME, "rb+"); // leitura e escrita em binario
    Usuario usuario;
    int id;
    printf("Digite o ID do usuario a ser alterado: ");
    scanf("%d", &id);
    while (fread(&usuario, sizeof(Usuario), 1, file)) {
        criptografar((char *)&usuario);
        if (usuario.id == id) { // compara o id solicitado com os do arquivo
            printf("Digite o novo nome do usuario: ");
            scanf("%s", usuario.nome);
            criptografar((char *)&usuario);
            fseek(file, -sizeof(Usuario), SEEK_CUR);
            // file indica o nome do arquivo
            // -sizof(Usuario) indica o número de caracteres que devem ser lidos
            // SEEK_CUR determina a posição de referência onde deve começar a ser lido
            fwrite(&usuario, sizeof(Usuario), 1, file);
            fclose(file);
            printf("Usuario alterado com sucesso!\n");
            return;
        }
        criptografar((char *)&usuario); 
    }
    fclose(file);
    printf("Usuario nao encontrado!\n");
}

void excluirUsuario() { // funções iguais as da função alterarUsuario não serão explicadas novamente
    FILE *file = fopen(FILE_NAME, "rb"); // abre o arquivo em leitura binaria
    FILE *tempFile = fopen("temp.txt", "wb"); // cria um arquivo temporario em escrita binaria  
    Usuario usuario;
    int id;
    printf("Digite o ID do usuario a ser excluido: ");
    scanf("%d", &id);
    while (fread(&usuario, sizeof(Usuario), 1, file)) {
        criptografar((char *)&usuario);
        if (usuario.id != id) { 
            fwrite(&usuario, sizeof(Usuario), 1, tempFile);
        }
    }
    fclose(file);
    fclose(tempFile);
    remove(FILE_NAME); // exclui o arquivo originar
    rename("temp.txt", FILE_NAME); //renomeia temp.txt para usuario.txt
    printf("Usuario excluido com sucesso!\n");
}

void listarUsuarios() { // funções iguais as da função alterarUsuario não serão explicadas novamente
    FILE *file = fopen(FILE_NAME, "rb");
    Usuario usuario;
    printf("Lista de Usuarios:\n");
    while (fread(&usuario, sizeof(Usuario), 1, file)) {
        criptografar((char *)&usuario);
        printf("ID: %d, Nome: %s\n", usuario.id, usuario.nome);
    }
    fclose(file);
}

void exibirMenu() {
    printf("\n1. Adicionar Usuario\n2. Alterar Usuario\n3. Excluir Usuario\n4. Listar Usuarios\n5. Sair\nEscolha uma opcao: ");
}

int main() {
    int opcao;
    do 
    {
        exibirMenu();
        scanf(" %d", &opcao);
        
        switch (opcao) {
                case 1:
                adicionarUsuario();
                break;
                    case 2:
                    alterarUsuario();
                    break;
                        case 3:
                        excluirUsuario();
                        break;
                            case 4:
                            listarUsuarios();
                            break;
                                case 5:
                                printf("Saindo...\n");
                                break;
            default:
                printf("Opcao invalida!\n");
        }
    } 
    while (opcao != 5);

    return 0;
}
