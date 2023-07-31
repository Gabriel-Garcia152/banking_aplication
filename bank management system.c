#include <locale.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//***main structures and global variables***//

struct Login {
    int account_code;
    bool logged_in;
};

struct Transactions {
    char type; //'W' will stand for 'withdraws' and 'D' for 'deposits'
    float value;
};

struct Account {
    char name[100], last_name[100], password[100];
    int age;
    float balance;
    bool active;
    struct Transactions* history;
    int history_capacity;
};

struct Login log_status;
struct Account* accounts;
int num_accounts = 0, last_transaction = 0;

#define MAX_HISTORY_ENTRIES 10
#define MAX_ACCOUNTS 1

//***general purpose function***//

int print_account(int account_code) {
    printf("\n\n*************************************\n\n");
    printf("Usuário: %s %s\n", accounts[account_code].name, accounts[account_code].last_name);
    printf("Idade: %i\n", accounts[account_code].age);
    printf("Seu saldo atual é de R$ %.2f\n", accounts[account_code].balance);
    printf("Código da conta: %i", account_code);
    printf("\n\n*************************************\n\n");
    
    return 1;
}

void print_main_menu() {
    printf ("[1] Criar nova conta;\n");
    printf ("[2] Atualizar informações da sua conta;\n");
    printf ("[3] Acessar transações;\n");
    printf ("[4] Verificar detalhes da sua conta;\n");
    printf ("[5] Remover uma conta existente;\n");
    printf ("[6] Saques e depósitos;\n");
    printf ("[7] Sair\n\n");
}

void print_update_menu() {
    printf("Qual informação você deseja atualizar?\n");
    printf("[1] - Nome\n");
    printf("[2] - Sobrenome\n");
    printf("[3] - Senha\n");
    printf("\n");
}


int check_int(const char *message, const char *error_message) {
    int var;
    while (true) {
        printf("%s", message);
        if (scanf("%i", &var) != 1) {
            printf("%s\n", error_message);
            while (getchar() != '\n'); // Clear input buffer
        } else {
            while (getchar() != '\n'); // Clear input buffer
            return var;
        }
    }
}


void exiting() {
    printf("Deseja voltar ao menu principal?\n");
    printf("Digite [1] para voltar ao menu ou [2] para fechar o programa:\n");
    
    int exit_confirmation;
    
    do {
        scanf("%i", &exit_confirmation);
        switch (exit_confirmation) {
            case 1:
                system("clear");
                printf("Voltando ao menu principal...\n\n");
                sleep(1);
                system("clear");
                return;
                break;
            case 2:
                system("clear");
                printf("Fechando o programa...");
                
                for (int i = 0; i < num_accounts; i++) {
                    free(accounts[i].history);
                }
                free(accounts);
                
                exit(0);
                return;
                break;
            default:
                printf("Opção inválida, por favor, tente novamente:\n");
                break;
        }
    } while (exit_confirmation != 1 && exit_confirmation != 2);
}

void resize_history(struct Account* account) {
    int new_size = MAX_HISTORY_ENTRIES * 2;
    
    struct Transactions* new_history = (struct Transactions*)realloc(account->history, new_size * sizeof(struct Transactions));
    if (new_history == NULL) {
        printf("Erro: Falha na realocação de memória para o histórico de transações.\n");
    } else {
        account -> history = new_history;
        account -> history_capacity = new_size;
    }
}

//***main menu functions***//

int create_account(void) {
    float balance = 0.0;
    
    accounts[num_accounts].history = NULL;
    accounts[num_accounts].history = (struct Transactions*)malloc(MAX_HISTORY_ENTRIES * sizeof(struct Transactions));
    if (accounts[num_accounts].history == NULL) {
        printf("Erro: Falha na alocação de memória para contas.\n");
        exit(1);
    }
    
    printf ("Digite seu nome: ");
    getchar();
    fgets (accounts[num_accounts].name, sizeof(accounts[num_accounts].name), stdin);
    printf ("Digite seu sobrenome: ");
    fgets (accounts[num_accounts].last_name, sizeof(accounts[num_accounts].last_name), stdin);
    accounts[num_accounts].age = check_int("Digite sua idade: ", "Idade inválida. Por favor, digite um valor numérico e inteiro.");
    
    size_t len_name = strlen(accounts[num_accounts].name);
    if (len_name > 0 && accounts[num_accounts].name[len_name - 1] == '\n') {
        accounts[num_accounts].name[len_name - 1] = '\0';
    }
    
    size_t len_last_name = strlen(accounts[num_accounts].last_name);
    if (len_last_name > 0 && accounts[num_accounts].last_name[len_last_name - 1] == '\n') {
        accounts[num_accounts].last_name[len_last_name - 1] = '\0';
    }
    
    if (accounts[num_accounts].age >= 18) {
        printf ("Crie uma senha: ");
        char *password = getpass("");
        strcpy(accounts[num_accounts].password, password);
        
        printf ("Confirme sua senha: ");
        char *confirm = getpass("");
        
        while (strcmp(accounts[num_accounts].password, confirm) != 0) {
            printf ("\nSenha não confirmada, por favor, tente novamente.\n");
            printf ("Crie uma senha: ");
            char *password = getpass("");
            strcpy(accounts[num_accounts].password, password);
            printf ("Confirme sua senha: ");
            char *confirm = getpass("");
        }
        
        accounts[num_accounts].active = true;
        accounts[num_accounts].history_capacity = MAX_HISTORY_ENTRIES;
        accounts[num_accounts].balance = balance;
        
        system("clear");
        printf("Conta criada com sucesso!");
        print_account(num_accounts);
        
        log_status.account_code = num_accounts;
        log_status.logged_in = true;
        num_accounts += 1;
        
        exiting();
        return 1;
        
    } else {
        system("clear");
        printf ("Você precisa ser maior de idade para criar uma conta.\n");
        printf ("Voltando para o menu principal...\n\n");
        return 0;
    }
}


void update_info(void) {
    int account_code, option;
    
    while (true) {
        printf("Digite o código da sua conta: ");
        scanf("%i", &account_code);
        
        if (account_code < num_accounts && account_code >= 0) {
            do {
                printf("Digite sua senha: ");
                char *validation = getpass("");
                printf("\n\n");
                
                if(strcmp(accounts[account_code].password, validation) != 0) {
                    printf("Senha inválida, tente novamente.\n");
                    continue;
                }
                
                print_update_menu();
                
                while(true) {
                    
                    option = check_int("Sua opção: ", "Opção inválida. Por favor, digite um valor numérico.");
                    
                    switch (option) {
                        case 1:
                            getchar();
                            printf("Qual nome você deseja colocar em sua conta?\n");
                            fgets (accounts[account_code].name, sizeof(accounts[account_code].name), stdin);
                            printf("Nome alterado com sucesso!");
                            
                            size_t len = strlen(accounts[account_code].name);
                            if (len > 0 && accounts[account_code].name[len - 1] == '\n') {
                                accounts[account_code].name[len - 1] = '\0';
                            }
                            
                            exiting();
                            return;
                            break;
                        case 2:
                            printf("Qual sobrenome você deseja colocar em sua conta?\n");
                            char last_name[100];
                            getchar();
                            fgets (accounts[account_code].last_name, sizeof(accounts[account_code].last_name), stdin);
                            printf("Sobrenome alterado com sucesso!");
                            
                            size_t len_last_name = strlen(accounts[account_code].last_name);
                            if (len_last_name > 0 && accounts[account_code].last_name[len_last_name - 1] == '\n') {
                                accounts[account_code].last_name[len_last_name - 1] = '\0';
                            }
                            
                            exiting();
                            return;
                            break;
                        case 3:
                            printf("Qual senha você deseja colocar em sua conta?\n");
                            char *password = getpass("");
                            strcpy(accounts[account_code].password, password);
                            printf("Senha alterada com sucesso!");
                            exiting();
                            return;
                            break;
                        default:
                            printf("Opção inválida, tente novamente.\n");
                            break;
                    }
                }
            } while (true);
        } else {
            printf("Conta não encontrada, tente novamente.\n");
        }
    }
}


bool account_access(void) {
    int account_code, attempts = 0;
    
    while (true) {
        account_code = check_int("Digite o código de sua conta: ", "Código incorreto, por favor, digite um valor numérico");
        
        system("clear");
        
        if (account_code < 0 || account_code >= num_accounts) {
            printf("Conta não encontrada, por favor, tente novamente.\n");
            continue;
        }
        
        if (accounts[account_code].active) {
            printf("Bem vindo(a) %s %s!\n", accounts[account_code].name, accounts[account_code].last_name);
            if (account_code != log_status.account_code || log_status.logged_in == false) {
                printf("Para acessar sua conta, por favor digite sua senha: ");
                do {
                    char *validation = getpass("");
                    
                    if (strcmp(validation, accounts[account_code].password) == 0) {
                        
                        print_account(account_code);
                        
                        exiting();
                        log_status.account_code = account_code;
                        log_status.logged_in = true;
                        return true;
                        
                    } else {
                        attempts++;
                        system("clear");
                        printf("Acesso negado. Senha incompátivel.\n");
                        printf("Tentativas feitas %i\n", attempts);
                        printf("Tente novamente: ");
                        
                    }
                } while(attempts < 3);
                
                system("clear");
                printf("Você excedeu o número máximo de tentativas de login.\n");
                printf("Voltanto ao menu principal...\n\n");
                return false;
            
            } else {
                print_account(account_code);
                  
                exiting();
                log_status.account_code = account_code;
                log_status.logged_in = true;
                return true;
            }
                
        } else if (!accounts[account_code].active) {
            
            printf("Esta conta foi desativada.\n");
            exiting();
            
        }
    }
}


void account_history() {
    int size = MAX_HISTORY_ENTRIES;
    
    for(int i = 0; i < size; i++) {
        printf("%i - ", i);
        if (accounts[log_status.account_code].history[i].type == 'W') {
            printf("Saque de %.2f\n", accounts[log_status.account_code].history[i].value);
        } else if (accounts[log_status.account_code].history[i].type == 'D') {
            printf("Depósito de %.2f\n", accounts[log_status.account_code].history[i].value);
        }
    }
    printf("\n\n");
    exiting();
    return;
}


bool delete_account(void) {
    int account_code, option, attempts = 0;;
    
    printf("Por favor, digite o número da conta que deseja excluir.\n");
    printf("Número da conta: ");
    scanf("%i", &account_code);
    printf("\n\n");
    
    if(account_code <= num_accounts - 1) {
        if(accounts[account_code].active) {
            printf("%s %s deseja mesmo excluir esta conta?\n", accounts[account_code].name, accounts[account_code].last_name);
            print_account(account_code);
            
            do {
                printf("Digite [1] para 'sim' ou [2] para 'não'\n");
                option = check_int("Sua opção: ", "Opção inválida. Por favor, digite um valor numérico.");
                
                if(option == 1) {
                    
                    system("clear");
                    printf("Para confirmar a exclusão, for favor, digite sua senha: ");
                    
                    do {
                        char *validation = getpass("");
                        
                        if(strcmp(validation, accounts[account_code].password) == 0) {
                            attempts = 0;
                            accounts[account_code].active = false;
                            
                            system("clear");
                            printf("Conta excluída com sucesso!\n\n");
                            log_status.logged_in = false;
                            
                            free(accounts[account_code].history);
                            accounts[account_code].history = NULL;
                            
                            exiting();
                            return true;
                        } else {
                            
                                attempts++;
                                system("clear");
                                printf("Acesso negado. Senha incompátivel.\n");
                                printf("Tentativas feitas %i\n", attempts);
                                printf("Tente novamente: ");
                           
                        }
                    } while(attempts < 3);
                    
                    system("clear");
                    printf("Você excedeu o número de tentativas.\n");
                    printf("Voltanto ao menu principal...\n\n");
                    return false;
                    
                } else if(option == 2) {
                    system("clear");
                    printf("Exclusão não confirmada, retornando ao menu principal.\n\n");
                    return false;
                    
                } else {
                    printf("Opção inválida, por favor, tente novamente.\n");
                }
            } while(option != 1 && option != 2);
        } else if (!accounts[account_code].active) {
            printf("Esta conta já foi desativada.\n\n");
            exiting();
            return false;
        }
        
    } else {
        printf("Conta não encontrada.\n");
        printf("Voltando ao menu principal.\n\n");
        return false;
    }
}


int money_move(void) {
    int option;
    float value;
    
    if (last_transaction >= MAX_HISTORY_ENTRIES) {
        // Resize the history array to accommodate more transactions
        resize_history(&accounts[log_status.account_code]);
    }
    
    printf("Selecione um tipo de movimentação.\n");
    do {
        printf("Digite [1] para saques e [2] para depósitos.\n");
        option = check_int("Sua opção: ", "Opção inválida. Por favor, digite um valor numérico.");
        switch (option) {
            case 1:
                printf("\n");
                printf("Qual valor você deseja retirar?\nR$ ");
                scanf("%f", &value);
                if (value <= accounts[log_status.account_code].balance && value > 0) {
                    accounts[log_status.account_code].balance -= value;
                    
                    printf("\n");
                    printf("%.2f retirado da sua conta, saldo atual: R$ %.2f\n\n", value, accounts[log_status.account_code].balance);
                    accounts[log_status.account_code].history[last_transaction].type = 'W';
                    accounts[log_status.account_code].history[last_transaction].value = value;
                    last_transaction += 1;
                    exiting();
                    return 1;
                    
                } else {
                    printf("\n");
                    printf("Saldo insuficiente para realizar o saque.\n\n");
                    exiting();
                    return 1;
                    
                } 
                break;
            case 2:
                printf("\n");
                printf("Qual valor você deseja depositar?\nR$ ");
                scanf("%f", &value);
                printf("\n");
                
                if (value > 0) {
                    accounts[log_status.account_code].balance += value;
                    
                    printf("Valor de R$ %.2f depositado com sucesso!\n", value);
                    printf("Saldo atual: R$ %.2f\n\n", accounts[log_status.account_code].balance);
                    accounts[log_status.account_code].history[last_transaction].type = 'D';
                    accounts[log_status.account_code].history[last_transaction].value = value;
                    last_transaction += 1;
                    exiting();
                    return 1;
                    
                } else {
                    printf("Valor inválido, tente novamente.\n\n");
                }
                break;
            default:
                printf("\n");
                printf("Opção inválida, por favor, tente novamente.\n");
                break;
        }
    } while(true);
}


int main (void) {
    setlocale (LC_ALL, "Portuguese");
    int option, exit_confirmation;
    
    unsigned int sleepDuration = 1;
    
    accounts = (struct Account*)malloc(MAX_ACCOUNTS * sizeof(struct Account));
    if (accounts == NULL) {
        printf("Erro: Falha na alocação de memória para o array de contas.\n");
        return 1;
    }
    
    printf ("Bem vindo(a) ao nosso banco!\n");
    printf ("Para começar, por favor escolha uma das opções abaixo\n");
    do {
        printf ("[1] Criar uma conta;\n[2] Realizar log-in;\n[3] Sair\n");
        option = check_int("Sua opção: ", "Opção inválida. Por favor, digite um valor numérico.");
        printf ("\n\n");
        switch (option) {
            case 1:
                if (num_accounts < 100) {
                        system("clear");
                        printf ("Criando uma nova conta...\n\n");
                        sleep(sleepDuration);
                        system("clear");
                        create_account();
                        
                    } else {
                        printf ("Infelizmente, nosso sistema de contas está cheio no momento.\n");
                        printf ("Por favor, tente novamente mais tarde...\n");
                    }
                break;
            case 2:
                system("clear");
                printf ("Iniciando log-in\n");
                sleep(sleepDuration);
                system("clear");
                account_access();
                break;
            case 3:
                system("clear");
                printf ("Saindo...");
                exit(0);
                break;
            default:
                system("clear");
                printf ("Opção inválida.\nPor favor, digite uma opção existente.\n\n");
                break;
        }
    } while(!log_status.logged_in);
    
    system("clear");
    
    while(true) {
        printf("Por favor, escolha uma das opções abaixo:\n\n");
        print_main_menu();
        
        option = check_int("Sua opção: ", "Opção inválida. Por favor, digite um valor numérico.");
        printf ("\n");
        
        switch (option) {
            case 1:
                system("clear");
                printf ("Criando uma nova conta\n");
                sleep(sleepDuration);
                system("clear");
                create_account();
                break;
            case 2:
                system("clear");
                printf ("Atualizando informações\n");
                sleep(sleepDuration);
                system("clear");
                update_info();
                break;
            case 3:
                 if (log_status.account_code != -1 && log_status.logged_in) {
                    system("clear");
                    printf ("Acessando o histórico de transações.\n");
                    sleep(sleepDuration);
                    system("clear");
                    account_history();
                } else {
                    system("clear");
                    printf("Você precisa realizar o login antes de acessar o histórico de transações.");
                    sleep(sleepDuration);
                    system("clear");
                    account_access();
                }
                break;
            case 4:
                system("clear");
                printf ("Verificando detalhes de sua conta\n");
                sleep(sleepDuration);
                system("clear");
                account_access();
                break;
            case 5:
                system("clear");
                delete_account();
                break;
            case 6:
                if (log_status.account_code != -1 && log_status.logged_in) {
                    system("clear");
                    printf ("Acessando área de saques e depósitos...\n");
                    sleep(sleepDuration);
                    system("clear");
                    money_move();
                } else {
                    system("clear");
                    printf("Você precisa realizar o login antes de acessar a área de saques e depósitos.");
                    sleep(sleepDuration);
                    system("clear");
                    account_access();
                }
                break;
            case 7:
                printf ("Deseja mesmo sair?\n");
                printf ("Digite [1] para 'sim' e [2] para 'não'.\n");
                scanf ("%i", &exit_confirmation);
              
                if (exit_confirmation == 1) {
                    system("clear");
                    printf ("Saindo...");
                    log_status.account_code = -1;
                    log_status.logged_in = false;
                    
                    for (int i = 0; i < num_accounts; i++) {
                        free(accounts[i].history);
                    }
                    free(accounts);
                    
                    exit(0);
                }
              
                break;
            default:
                system("clear");
                printf ("Opção inválida.\nPor favor, digite uma opção existente.\n\n");
                break;
        }
    }

    return 0;
}
