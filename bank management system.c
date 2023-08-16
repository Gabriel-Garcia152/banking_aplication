#include <locale.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//***main structures and global variables***//

#define MAX_HISTORY_ENTRIES 10
#define MAX_ACCOUNTS 100
#define STRING_LIMIT_SIZE 100

struct Login {
    int account_code;
    bool logged_in;
};

struct Transactions {
    char type; //'W' will stand for 'withdrawals' and 'D' for 'deposits'
    double value;
};

struct Account {
    char name[STRING_LIMIT_SIZE], last_name[STRING_LIMIT_SIZE], password[STRING_LIMIT_SIZE];
    int age;
    double balance;
    bool active;
    struct Transactions history[MAX_HISTORY_ENTRIES];
    int history_capacity;
    int last_transaction;
};

struct Login log_status;
struct Account accounts[MAX_ACCOUNTS];
int num_accounts = 0;

//***general purpose functions***//

void print_account(int account_code) {
    printf("\n\n*************************************\n\n");
    printf("Usuário: %s %s\n", accounts[account_code].name, accounts[account_code].last_name);
    printf("Idade: %i\n", accounts[account_code].age);
    printf("Seu saldo atual é de R$ %.2f\n", accounts[account_code].balance);
    printf("Código da conta: %i", account_code);
    printf("\n\n*************************************\n\n");
    
    return;
}

void print_main_menu(void) {
    printf ("[1] Criar nova conta;\n");
    printf ("[2] Atualizar informações da sua conta;\n");
    printf ("[3] Acessar transações;\n");
    printf ("[4] Verificar detalhes da sua conta;\n");
    printf ("[5] Remover uma conta existente;\n");
    printf ("[6] Saques e depósitos;\n");
    printf ("[7] Sair\n\n");
    
    return;
}

void print_update_menu(void) {
    printf("Qual informação você deseja atualizar?\n");
    printf("[1] - Nome\n");
    printf("[2] - Sobrenome\n");
    printf("[3] - Senha\n");
    printf("\n");
    
    return;
}


//checking if an input received an integer-type data to avoid unexpected situations
int check_int(const char *message, const char *error_message) {
    int var;
    while (true) {
        printf("%s", message);
        if (scanf("%i", &var) != 1) {
            printf("%s\n", error_message);
            while (getchar() != '\n');
        } else {
            while (getchar() != '\n');
            return var;
        }
    }
}

//checking if an input received a double-type data to avoid unexpected situations
double check_double(const char *message, const char *error_message) {
    double var;
    while (true) {
        printf("%s", message);
        if (scanf("%lf", &var) != 1) {
            printf("%s\n", error_message);
            while (getchar() != '\n');
        } else {
            while (getchar() != '\n');
            return var;
        }
    }
}


void exiting(void) {
    printf("\n");
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
                exit(0);
                return;
                break;
            default:
                printf("Opção inválida, por favor, tente novamente:\n");
                break;
        }
    } while (exit_confirmation != 1 && exit_confirmation != 2);
}


bool password_validating(int account_code) {
    int attempts = 0;
    
    printf("Por favor, digite sua senha: ");
    do{
        char *validating = getpass("");
        
        if(strcmp(validating, accounts[account_code].password) == 0) {
            return true;
        } else {
            system("clear");
            printf("Senha inválida, por favor tente novamente.\n");
            printf("Tentativas feitas: %i\n", attempts);
            attempts += 1;
            printf("Tentativas restantes: %i\n", 3 - attempts);
        }
    } while(attempts < 3);
    printf("Número máximo de tentativas atingido.\n");
    exiting();
    return false;
}

//deleting new-line character created by fgets
void delete_new_line(char *var) {
    size_t length = strlen(var);
    if (length > 0 && var[length - 1] == '\n') {
        var[length - 1] = '\0';
    }
    return;
}

//***main menu functions***//

void create_account(void) {
    double balance = 0.0;
    
    //storing name and last_name properly and ensuring it won't surpass the limit size for stings
    printf ("Digite seu nome: ");
    fgets (accounts[num_accounts].name, sizeof(accounts[num_accounts].name), stdin);
    delete_new_line(accounts[num_accounts].name);
    
    printf ("Digite seu sobrenome: ");
    fgets (accounts[num_accounts].last_name, sizeof(accounts[num_accounts].last_name), stdin);
    delete_new_line(accounts[num_accounts].last_name);
    
    accounts[num_accounts].age = check_int("Digite sua idade: ", "Idade inválida. Por favor, digite um valor numérico e inteiro.");
    
    if (accounts[num_accounts].age >= 18) {
        printf ("Crie uma senha: ");
        char *password = getpass("");
        strcpy(accounts[num_accounts].password, password);
        
        printf ("Confirme sua senha: ");
        char *confirm = getpass("");
        
        while (strcmp(accounts[num_accounts].password, confirm) != 0) {
            printf ("\nSenha não confirmada, por favor, tente novamente.\n");
            printf ("Crie uma senha: ");
            password = getpass("");
            strcpy(accounts[num_accounts].password, password);
            printf ("Confirme sua senha: ");
            confirm = getpass("");
        }
        
        accounts[num_accounts].active = true;
        accounts[num_accounts].history_capacity = MAX_HISTORY_ENTRIES;
        accounts[num_accounts].balance = balance;
        accounts[num_accounts].last_transaction = 0;
        accounts[num_accounts].history_capacity = MAX_HISTORY_ENTRIES;
        accounts[num_accounts].last_transaction = 0;
        
        //clearing every element to avoid unexpected behaviors
        for (int i = 0; i < MAX_HISTORY_ENTRIES; i++) {
            accounts[num_accounts].history[i].type = '\0';
            accounts[num_accounts].history[i].value = 0.0;
        }
        
        system("clear");
        printf("Conta criada com sucesso!");
        print_account(num_accounts);
        
        log_status.account_code = num_accounts;
        log_status.logged_in = true;
        num_accounts += 1;
        
        exiting();
        return;
        
    } else {
        system("clear");
        printf ("Você precisa ser maior de idade para criar uma conta.\n");
        printf ("Voltando para o menu principal...\n\n");
        return;
    }
}


void update_info(void) {
    int account_code, option;
    
    while (true) {
        account_code = check_int("Digite o código da sua conta: ", "Opção inválida. Por favor, tente novamente.");
        
        if (account_code < num_accounts && account_code >= 0) {
            if(password_validating(account_code)) {
                
                print_update_menu();
                
                while(true) {
                    
                    option = check_int("Sua opção: ", "Opção inválida. Por favor, digite um valor numérico.");
                    
                    switch (option) {
                        case 1:
                            getchar();
                            printf("Qual nome você deseja colocar em sua conta?\n");
                            fgets (accounts[account_code].name, sizeof(accounts[account_code].name), stdin);
                            system("clear");
                            printf("Nome alterado com sucesso!");
                            
                            delete_new_line(accounts[account_code].name);
                            
                            exiting();
                            return;
                            break;
                        case 2:
                            printf("Qual sobrenome você deseja colocar em sua conta?\n");
                            char last_name[STRING_LIMIT_SIZE];
                            getchar();
                            fgets (accounts[account_code].last_name, sizeof(accounts[account_code].last_name), stdin);
                            system("clear");
                            printf("Sobrenome alterado com sucesso!");
                            
                            delete_new_line(accounts[account_code].last_name);
                            
                            exiting();
                            return;
                            break;
                        case 3:
                            printf("Qual senha você deseja colocar em sua conta?\n");
                            char *password = getpass("");
                            strcpy(accounts[account_code].password, password);
                            system("clear");
                            printf("Senha alterada com sucesso!");
                            exiting();
                            return;
                            break;
                        default:
                            printf("Opção inválida, tente novamente.\n");
                            break;
                    }
                }
            }
        } else {
            printf("Conta não encontrada, tente novamente.\n");
        }
    }
}


bool account_access(void) {
    int account_code;
    
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
                printf("Para acessar sua conta");
                
                //printing account's info if password match
                if (password_validating(account_code)) {
                    
                    print_account(account_code);
                    
                    exiting();
                    log_status.account_code = account_code;
                    log_status.logged_in = true;
                    return true;
                    
                }
                
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


void account_history(void) {
    
    for (int i = 0; i < accounts[log_status.account_code].history_capacity; i++) {
        //checking if the transaction isn't empty to ensure only relevant info will be displayed
        if (accounts[log_status.account_code].history[i].type != '\0') {
            printf("%i - ", i + 1);
            if (accounts[log_status.account_code].history[i].type == 'W') {
                printf("Saque de %.2f\n", accounts[log_status.account_code].history[i].value);
            } else if (accounts[log_status.account_code].history[i].type == 'D') {
                printf("Depósito de %.2f\n", accounts[log_status.account_code].history[i].value);
            }
        }
    }

    printf("\n\n");
    exiting();
    return;
}


void delete_account(void) {
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
                    printf("Para confirmar a exclusão");
                    
                    if(password_validating(account_code)) {
                        accounts[account_code].active = false;
                        printf("Conta excluída com sucesso.");
                    }
                    
                } else if(option == 2) {
                    
                    system("clear");
                    printf("Exclusão não confirmada, retornando ao menu principal.\n\n");
                    return;
                    
                } else {
                    printf("Opção inválida, por favor, tente novamente.\n");
                }
            } while(option != 1 && option != 2);
        } else if (!accounts[account_code].active) {
            printf("Esta conta já foi desativada.\n\n");
            exiting();
            return;
        }
        
    } else {
        printf("Conta não encontrada.\n");
        printf("Voltando ao menu principal.\n\n");
        return;
    }
}


void money_move(void) {
    int option;
    double value;
    
    printf("Selecione um tipo de movimentação.\n");
    do {
        printf("Digite [1] para saques e [2] para depósitos.\n");
        option = check_int("Sua opção: ", "Opção inválida. Por favor, digite um valor numérico.");
        
        switch (option) {
            
            //withdrawals' case
            case 1:
                printf("\n");
                value = check_double("Qual valor você deseja retirar?\nR$ ", "Valor não confirmado, por favor, digite um valor númerico");
                if (value <= accounts[log_status.account_code].balance && value > 0) {
                    accounts[log_status.account_code].balance -= value;
                    
                    printf("\n");
                    printf("%.2f retirado da sua conta, saldo atual: R$ %.2f\n\n", value, accounts[log_status.account_code].balance);
                    accounts[log_status.account_code].history[accounts[log_status.account_code].last_transaction].type = 'W';
                    accounts[log_status.account_code].history[accounts[log_status.account_code].last_transaction].value = value;
                    
                    if(accounts[log_status.account_code].last_transaction < 10) {
                        accounts[log_status.account_code].last_transaction += 1;
                    }
                    
                    exiting();
                    return;
                    
                } else {
                    printf("\n");
                    printf("Saldo insuficiente para realizar o saque.\n\n");
                    exiting();
                    return;
                    
                } 
                break;
                
            //deposits' case
            case 2:
                printf("\n");
                value = check_double("Qual valor você deseja depositar?\nR$ ", "Valor não confirmado, por favor, digite um valor númerico");
                printf("\n");
                
                if (value > 0) {
                    accounts[log_status.account_code].balance += value;
                    
                    printf("Valor de R$ %.2f depositado com sucesso!\n", value);
                    printf("Saldo atual: R$ %.2f\n\n", accounts[log_status.account_code].balance);
                    accounts[log_status.account_code].history[accounts[log_status.account_code].last_transaction].type = 'D';
                    accounts[log_status.account_code].history[accounts[log_status.account_code].last_transaction].value = value;
                    
                    if(accounts[log_status.account_code].last_transaction < 10) {
                        accounts[log_status.account_code].last_transaction += 1;
                    }
                    
                    exiting();
                    return;
                    
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


void main (void) {
    setlocale (LC_ALL, "Portuguese");
    int option, exit_confirmation;
    unsigned int sleepDuration = 1;
    
    printf ("Bem vindo(a) ao nosso banco!\n");
    printf ("Para começar, por favor escolha uma das opções abaixo.\n\n");
    do {
        printf ("[1] Criar uma conta;\n[2] Realizar log-in;\n[3] Sair\n\n");
        option = check_int("Sua opção: ", "Opção inválida. Por favor, digite um valor numérico.");
        printf ("\n\n");
        switch (option) {
            //creating accounts
            case 1:
                if (num_accounts < MAX_ACCOUNTS) {
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
                
            //loggin-in
            case 2:
                system("clear");
                printf ("Iniciando log-in\n");
                sleep(sleepDuration);
                system("clear");
                account_access();
                break;
                
            //leaving
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
                if (log_status.account_code > -1 && log_status.logged_in) {
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
                system("clear");
                printf("Deseja mesmo sair?\n");
                do {
                    exit_confirmation = check_int("\nDigite [1] para 'sim' e [2] para 'não'.\n", "Opção inválida, tente novamente.");
                    
                    switch (exit_confirmation) {
                        case 1:
                            system("clear");
                            printf ("Saindo...");
                            log_status.account_code = -1;
                            log_status.logged_in = false;
                            exit(0);
                        case 2:
                            printf("Voltando ao menu principal.\n");
                            sleep(sleepDuration);
                            system("clear");
                            break;
                        default:
                            printf("Opção inválida.\nPor favor, digite uma opção existente.\n\n");
                            break;
                    }
                } while(exit_confirmation != 1 && exit_confirmation != 2);
              
                break;
            default:
                system("clear");
                printf ("Opção inválida.\nPor favor, digite uma opção existente.\n\n");
                break;
        }
    }
}
