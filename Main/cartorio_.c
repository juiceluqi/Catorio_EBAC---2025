///////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
/////#######///#/////#///#######////#######////#######///////
///////###/////#/////#/////###/////#//////////#/////////////
///////###/////#/////#/////###/////#//////////######///////
////#//###/////#/////#/////###/////#//////////#///////////
////#####///////#####////#######////#######////#######///
////////////////////////////////////////////////////////
///////////////////////////////////////////////////////


#include <stdio.h> //Biblioteca de comunica��o com o usu�rio.
#include <stdlib.h> //Biblioteca de memoria e loca��o.
#include <locale.h> //Biblioteca de loca��o de texto de regi�o.
#include <string.h> //Biblioteca responsavel pelas strings.
#include <wchar.h> // Biblioteca para manipula��o de caracteres wide


int confirmaAcao(const char *mensagem) { //Fun��o responsavel pela confirma��o de a��es.
	
    char resposta;
    
    printf("%s (S/N): ", mensagem);
    scanf(" %c", &resposta);

    if (resposta == 'N' || resposta == 'n') {
        printf("\nCart�rio > Opera��o cancelada!\n\n");
        system("pause");
        return 0; // Retorno de cancelamento
    }
    
    return 1; // Retorno para continuar
}


int registro() {
	
    setlocale(LC_ALL, "Portuguese"); // Habilita acentos

    printf("\n\nCart�rio > Registro de usu�rio\n\n");

	//Cria��o de variaveis.
    char arquivo[40];
    char cpf[40];
    char nome[40];
    char sobrenome[40];
    char cargo[40];

    printf("Digite um CPF para cadastro: "); //Coletando informa��o de usu�rio.
    printf("\nDigite: 0, para cancelar est� a��o.\n\n");
    scanf("%s", cpf); // Captura CPF
    
    if (cpf[0] == '0' && cpf[1] == '\0') { 
    	
        printf("\nCart�rio > Opera��o cancelada!\n\n");
        system("pause");
        
        return 0;
    }
    
    strcpy(arquivo, cpf);
    
    // Cadastro no arquivo "registrados.txt"
    FILE *file = fopen("registrados.txt", "a");

    if (file == NULL) {
        printf("Cart�rio > Erro ao criar arquivo!\n");
        return 1;
    }

    fprintf(file, "%s,", cpf);
    getchar(); // Remove o '\n' deixado pelo scanf

    printf("Digite um nome para o cadastro: ");
    fgets(nome, sizeof(nome), stdin);
    nome[strcspn(nome, "\n")] = 0; // Remove a quebra de linha

    fprintf(file, "%s,", nome);

    printf("Digite um sobrenome para o cadastro: ");
    fgets(sobrenome, sizeof(sobrenome), stdin);
    sobrenome[strcspn(sobrenome, "\n")] = 0; // Remove a quebra de linha

    fprintf(file, "%s,", sobrenome);

    printf("Digite um cargo para o cadastro: ");
    fgets(cargo, sizeof(cargo), stdin);
    cargo[strcspn(cargo, "\n")] = 0; // Remove a quebra de linha

    fprintf(file, "%s\n", cargo);

    fclose(file);

    printf("\nCart�rio > Usu�rio cadastrado com sucesso!\n");

    // Atualiza os �ltimos 5 registros no arquivo "recentes.txt"
    FILE *recentFile = fopen("recentes.txt", "r");
    char registros[5][200]; // Matriz para armazenar os 5 registros
    int i = 0;

    // L� os registros existentes em "recentes.txt"
    while (fgets(registros[i], sizeof(registros[i]), recentFile) && i < 5) {
        i++;
    }
    fclose(recentFile);

	int j = 0;
    // Move os registros para "deslocar" os mais antigos
    for (j = 4; j > 0; j--) {
        strcpy(registros[j], registros[j - 1]);
    }

    // Adiciona o novo registro no in�cio
    sprintf(registros[0], "%s,%s,%s,%s\n", cpf, nome, sobrenome, cargo);

    // Escreve os 5 registros mais recentes no arquivo "recentes.txt"
    recentFile = fopen("recentes.txt", "w");
    for (j = 0; j < 5 && strlen(registros[j]) > 0; j++) {
        fputs(registros[j], recentFile);
    }
    fclose(recentFile);

    system("pause");
    return 0;
}

int consulta() {

    setlocale(LC_ALL, "Portuguese");

    printf("\nCart�rio > Consultar usu�rio\n\n");

    char cpf[40];
    char linha[200]; // Leitura do arquivo
    int cpfEncontrado = 0; // Flag para verificar se o CPF foi encontrado

    while (1) { // Loop para manter a consulta ativa
        printf("Digite o ID de usu�rio (ou '0' para sair): ");
        scanf("%s", cpf);

        // Se o usu�rio digitar '0', sai da consulta
        if (cpf[0] == '0' && cpf[1] == '\0') {
            printf("Cart�rio > Consulta finalizada.\n");
            break;
        }

        FILE *file = fopen("registrados.txt", "r");
        if (file == NULL) {
            printf("\nErro ao abrir o arquivo de registros.\n");
            break;
        }

        char nomes[5][50] = {"Vazio", "Vazio", "Vazio", "Vazio", "Vazio"};
        char sobrenomes[5][50] = {"Vazio", "Vazio", "Vazio", "Vazio", "Vazio"};
        char cargo[5][50] = {"Vazio", "Vazio", "Vazio", "Vazio", "Vazio"};
        char cpfs[5][20] = {"Vazio", "Vazio", "Vazio", "Vazio", "Vazio"};

        int totalRegistros = 0;

        // L� os registros do arquivo e mant�m apenas os �ltimos 5
        while (fgets(linha, sizeof(linha), file)) {
            linha[strcspn(linha, "\n")] = 0;
            char *token = strtok(linha, ",");
            int campo = 0;

            while (token != NULL) {
                switch (campo) {
                    case 0: // CPF
                        strcpy(cpfs[totalRegistros % 5], token);
                        break;
                    case 1: // Nome
                        strcpy(nomes[totalRegistros % 5], token);
                        break;
                    case 2: // Sobrenome
                        strcpy(sobrenomes[totalRegistros % 5], token);
                        break;
                	case 3: // Cargo
                        strcpy(cargo[totalRegistros % 5], token);
                        break;
                }
                campo++;
                token = strtok(NULL, ",");
            }
            totalRegistros++;
        }
        fclose(file);

        // Vari�vel de flag para verificar se o CPF foi encontrado
        cpfEncontrado = 0;

		int i = 0;
        // Verifica se o CPF existe entre os 5 mais recentes
        for (i = 0; i < 5; i++) {
            if (strcmp(cpf, cpfs[i]) == 0) {
                printf("\n\nCart�rio > Usu�rio encontrado: Nome: %s Sobrenome: %s | Cargo: %s | CPF: %s\n\n", nomes[i], sobrenomes[i], cargo[i], cpfs[i]);
                cpfEncontrado = 1; // Marca que o CPF foi encontrado
                break;
            }
        }
        
        // Exibe a lista de usu�rios
        printf("\n======= �ltimos 5 Usu�rios Registrados =======\n");
        
        for (i = 0; i < 5; i++) {
            printf("%d. Nome: %s | CPF: %s\n", i + 1, nomes[i], cpfs[i]);
        }

        if (!cpfEncontrado) {
            printf("\nO ID de usu�rio n�o existe no sistema.\n");

            // Op��o para registrar o usu�rio caso n�o tenha sido encontrado
            if (confirmaAcao("\n > Deseja registrar este usu�rio?\n\n")) {
                registro();  // Chama a fun��o de registro
            }
        }

        // Op��o de realizar uma nova consulta ou voltar
        if (!confirmaAcao("\n > Deseja realizar uma nova consulta?\n\n")) {
            break; // Sai do loop e finaliza a fun��o
        }
    }

    return 0;
}

//Fun��o que habilita uma confirma��o de delete.
int confirmaDeletacao(char *cpf) {
	
    char resposta[3];

    printf("\nTem certeza que deseja deletar o usu�rio com o CPF: %s? (Sim/N�o): ", cpf);
    scanf("%s", resposta);

    // Verifica se a resposta � "Sim" (case-insensitive)
    if (strcasecmp(resposta, "Sim") == 0) {
        return 1; // Usu�rio confirmou a exclus�o
    } else {
        printf("\nCart�rio > Dele��o cancelada!\n\n");
        return 0; // Usu�rio cancelou a exclus�o
    }
}

int deletar() {
	
    setlocale(LC_ALL, "Portuguese");

    char cpf[40];

    printf("\nCart�rio > Deletar usu�rio\n\n");

    printf("Digite o ID de usu�rio para ser deletado: ");
    scanf("%s", cpf);

    if (cpf[0] == '0' && cpf[1] == '\0') {
        printf("\nCart�rio > Opera��o cancelada!\n\n");
        system("pause");
        return 0;
    }

    // Verifica se o usu�rio existe no arquivo de registros
    FILE *file = fopen("registrados.txt", "r");
    if (file == NULL) {
        printf("\nCart�rio > Nenhum usu�rio registrado!\n\n");
        system("pause");
        return 0;
    }

    FILE *tempFile = fopen("temp.txt", "w");
    if (tempFile == NULL) {
        printf("\nCart�rio > Erro ao criar arquivo tempor�rio!\n\n");
        fclose(file);
        system("pause");
        return 0;
    }

    char linha[200];
    int usuarioEncontrado = 0;

    // L� o arquivo de registros e copia para o arquivo tempor�rio, exceto o CPF a ser deletado
    while (fgets(linha, sizeof(linha), file)) {
        linha[strcspn(linha, "\n")] = 0; // Remove a quebra de linha

        char *token = strtok(linha, ",");
        char cpfTemp[40];
        strcpy(cpfTemp, token); // Armazena o CPF do registro atual

        if (strcmp(cpf, cpfTemp) != 0) {
            // Se o CPF n�o for o que estamos buscando, copia a linha para o arquivo tempor�rio
            fprintf(tempFile, "%s\n", linha);
        } else {
            usuarioEncontrado = 1; // Marca que o usu�rio foi encontrado
        }
    }

    fclose(file);
    fclose(tempFile);

    // Se o usu�rio n�o foi encontrado
    if (!usuarioEncontrado) {
        printf("\nCart�rio > Usu�rio n�o encontrado!\n\n");
        remove("temp.txt");
        system("pause");
        return 0;
    }

    // Confirma��o para deletar
    if (!confirmaDeletacao(cpf)) {
        // Se o cliente cancelar a exclus�o, remove o arquivo tempor�rio e retorna
        remove("temp.txt");
        system("pause");
        return 0;
    }

    // Substitui o arquivo original pelo arquivo tempor�rio
    remove("registrados.txt");
    rename("temp.txt", "registrados.txt");

    // Agora, atualiza o arquivo de registros recentes
    FILE *recentesFile = fopen("recentes.txt", "r");
    FILE *tempRecentesFile = fopen("temp_recentes.txt", "w");

    if (recentesFile == NULL || tempRecentesFile == NULL) {
        printf("\nCart�rio > Erro ao abrir o arquivo de registros recentes!\n\n");
        system("pause");
        return 0;
    }

    // L� os registros recentes e copia para o arquivo tempor�rio, exceto o CPF deletado
    while (fgets(linha, sizeof(linha), recentesFile)) {
        linha[strcspn(linha, "\n")] = 0; // Remove a quebra de linha

        char *token = strtok(linha, ",");
        char cpfRecentes[40];
        strcpy(cpfRecentes, token); // Armazena o CPF dos registros recentes

        if (strcmp(cpf, cpfRecentes) != 0) {
            // Se o CPF n�o for o que estamos buscando, copia a linha para o arquivo tempor�rio
            fprintf(tempRecentesFile, "%s\n", linha);
        }
    }

    fclose(recentesFile);
    fclose(tempRecentesFile);

    // Substitui o arquivo de registros recentes pelo arquivo tempor�rio
    remove("recentes.txt");
    rename("temp_recentes.txt", "recentes.txt");

    printf("\nCart�rio > Usu�rio deletado com sucesso!\n");
    system("pause");
    return 0;
}

int main() {
	
    int opcao;
    char cpf[40];

    while (1) {
    	
        system("cls");
        
        setlocale(LC_ALL, "Portuguese");

        printf("\n\n--> Cart�rio da EBAC <--\n\n");
        
        printf("\nEscolha a op��o desejada no menu a seguir!\n\n");
        
        printf("\tSe��o: Usuarios\n\n");
        
        printf("\t1 > Registrar\n");
        printf("\t2 > Consultar\n");
        printf("\t3 > Deletar\n");
        printf("\t4 > Sair\n");

        printf("\n\nDigite uma op��o: ");
		
        if (scanf("%d", &opcao) != 1) {
        	
            printf("\nCart�rio > Op��o inv�lida! Use uma op��o valida.\n");
            while (getchar() != '\n');
            system("pause");
            continue;
        }

        system("cls");

        switch (opcao) {
            case 1:
            	
                registro();
                break;
            case 2:
            	
                consulta();
                break;
            case 3:
            	
                deletar();
                break;
            case 4:
            	
            	sleep(3); //Adiciona um Delay na mensagem.
                printf("\nCart�rio > Obrigado por usar o Cart�rio da EBAC!\n");
                
                return 0;
            default:
            	
                printf("Cart�rio > Por favor! Selecione uma op��o dispon�vel.\n"); //Verifica��o de op��es disponiveis. (Tira letras e numeros invalidos).
                system("pause");
                
                break;
        }
    }
}

