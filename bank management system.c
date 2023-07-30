#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

#define MAX_ACCOUNTS 100

//***main structures**//

struct Login {
    int account_code;
    bool logged_in;
};

struct Account {
    char name[100], last_name[100], password[100];
    int age;
    float balance;
    bool active;
};

struct Login log_status;
struct Account accounts[MAX_ACCOUNTS];
bool run = true;
int num_accounts = 0;

//***general porpose function***//

void clear_terminal() {
    printf("\033[H\033[2J");
}

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
    printf("\n");
    printf("Qual informação você deseja atualizar?\n");
    printf("[1] - Nome\n");
    printf("[2] - Sobrenome\n");
    printf("[3] - Senha\n");
    printf("\n");
}

void exiting() {
    printf("Deseja voltar ao menu principal?\n");
    printf("Digite [1] para voltar ao menu ou [2] para fechar o programa:\n");
    
    int exit_confirmation;
    unsigned int sleepDuration = 1;
    
    do {
        scanf("%i", &exit_confirmation);
        switch (exit_confirmation) {
            case 1:
                clear_terminal();
                printf("Voltando ao menu principal...\n\n");
                sleep(sleepDuration);
                clear_terminal();
                return;
                break;
            case 2:
                clear_terminal();
                printf("Fechando o programa...");
                run = false;
                exit(0);
                return;
                break;
            default:
                printf("Opção inválida, por favor, tente novamente:\n");
                break;
        }
    } while (exit_confirmation != 1 && exit_confirmation != 2);
}

//***main menu functions***//

int create_account(void) {
    float balance = 0.0;
    
    clear_terminal();

    printf ("Digite seu nome: ");
    scanf ("%s", accounts[num_accounts].name);
    printf ("Digite seu sobrenome: ");
    scanf ("%s", accounts[num_accounts].last_name);
    printf ("Digite sua Idade: ");
    scanf ("%i", &accounts[num_accounts].age);
    
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
        
        clear_terminal();
        printf("Conta criada com sucesso!");
        print_account(num_accounts);
        
        log_status.account_code = num_accounts;
        log_status.logged_in = true;
        num_accounts += 1;
        
        exiting();
        return 1;
        
    } else {
        clear_terminal();
        printf ("Você precisa ser maior de idade para criar uma conta.\n");
        printf ("Voltando para o menu principal...\n\n");
        return 0;
    }
}


void update_info(void) {
    int account_code, option;
    
    clear_terminal();
    
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
                    
                    scanf("%i", &option);
                    
                    switch (option) {
                        case 1:
                            printf("Qual nome você deseja colocar em sua conta?\n");
                            char name[100];
                            scanf("%s", name);
                            strcpy(accounts[account_code].name, name);
                            printf("Nome alterado com sucesso!");
                            exiting();
                            return;
                            break;
                        case 2:
                            printf("Qual sobrenome você deseja colocar em sua conta?\n");
                            char last_name[100];
                            scanf("%s", last_name);
                            strcpy(accounts[account_code].last_name, last_name);
                            printf("Sobrenome alterado com sucesso!");
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
    
    clear_terminal();
    
    printf("Digite o código de sua conta: ");
    while (true) {
        scanf("%i", &account_code);
        
        clear_terminal();
        
        if (account_code < 0 || account_code > num_accounts) {
            printf("Conta não encontrada, por favor, tente novamente.");
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
                        clear_terminal();
                        printf("Acesso negado. Senha incompátivel.\n");
                        printf("Tentativas feitas %i\n", attempts);
                        printf("Tente novamente: ");
                        
                    }
                } while(attempts < 3);
                
                clear_terminal();
                printf("Você excedeu o número de tentativas.\n");
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


bool delete_account(void) {
    int account_code, option, attempts = 0;
    
    clear_terminal();
    
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
                scanf("%i", &option);
                
                if(option == 1) {
                    
                    clear_terminal();
                    printf("Para confirmar a exclusão, for favor, digite sua senha: ");
                    
                    do {
                        char *validation = getpass("");
                        
                        if(strcmp(validation, accounts[account_code].password) == 0) {
                            accounts[account_code].active = false;
                            
                            clear_terminal();
                            printf("Conta excluída com sucesso!\n\n");
                            log_status.logged_in = false;
                            exiting();
                            return true;
                        } else {
                            
                                attempts++;
                                clear_terminal();
                                printf("Acesso negado. Senha incompátivel.\n");
                                printf("Tentativas feitas %i\n", attempts);
                                printf("Tente novamente: ");
                           
                        }
                    } while(attempts < 3);
                    
                    clear_terminal();
                    printf("Você excedeu o número de tentativas.\n");
                    printf("Voltanto ao menu principal...\n\n");
                    return false;
                    
                } else if(option == 2) {
                    clear_terminal();
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
    
    clear_terminal();
    
    printf("Selecione um tipo de movimentação.\n");
    do {
        printf("Digite [1] para saques e [2] para depósitos.\n");
        scanf("%i", &option);
        switch (option) {
            case 1:
                printf("\n");
                printf("Qual valor você deseja retirar?\n");
                scanf("%f", &value);
                if (value <= accounts[log_status.account_code].balance && value > 0) {
                    accounts[log_status.account_code].balance -= value;
                    
                    printf("\n");
                    printf("%.2f retirado da sua conta, saldo atual: R$ %.2f\n", value, accounts[log_status.account_code].balance);
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
                    
                    printf("Valor de %.2f depositado com sucesso!\n", value);
                    printf("Saldo atual: R$ %.2f\n\n", accounts[log_status.account_code].balance);
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
    
    log_status.logged_in = false;
    unsigned int sleepDuration = 1;
    
    printf ("Bem vindo(a) ao nosso banco!\n");
    printf ("Para começar, por favor escolha uma das opções abaixo\n");
    do {
        printf ("[1] Criar uma conta;\n[2] Realizar log-in;\n[3] Sair\n");
        scanf ("%i", &option);
        printf ("\n\n");
        switch (option) {
            case 1:
                if (num_accounts < 100) {
                        printf ("Criando uma nova conta...\n\n");
                        sleep(sleepDuration);
                        create_account();
                        
                    } else {
                        printf ("Infelizmente, nosso sistema de contas está cheio no momento.\n");
                        printf ("Por favor, tente novamente mais tarde...\n");
                    }
                break;
            case 2:
                printf ("Iniciando log-in\n");
                sleep(sleepDuration);
                account_access();
                break;
            case 3:
                printf ("Saindo...");
                return 0;
                run = false;
                break;
            default:
                clear_terminal();
                printf ("Opção inválida.\nPor favor, digite uma opção existente.\n\n");
                break;
        }
    } while(!log_status.logged_in);
    
    clear_terminal();
    
    while(run) {
        printf("Por favor, escolha uma das opções abaixo:\n\n");
        print_main_menu();
        
        scanf ("%i", &option);
        printf ("\n");
        
        switch (option) {
            case 1:
                printf ("Criando uma nova conta\n");
                sleep(sleepDuration);
                create_account();
                break;
            case 2:
                printf ("Atualizando informações\n");
                sleep(sleepDuration);
                update_info();
                break;
            case 3:
                 printf ("Acessando transações\n");
                 break;
            case 4:
                printf ("Verificando detalhes de sua conta\n");
                sleep(sleepDuration);
                account_access();
                break;
            case 5:
                delete_account();
                break;
            case 6:
                if (log_status.account_code != -1 && log_status.logged_in) {
                    printf ("Acessando área de saques e depósitos...\n");
                    sleep(sleepDuration);
                    money_move();
                } else {
                    printf("Você precisa realizar o login antes de acessar a área de saques e depósitos.");
                    sleep(sleepDuration);
                    account_access();
                }
                break;
            case 7:
                printf ("Deseja mesmo sair?\n");
                printf ("Digite [1] para 'sim' e [2] para 'não'.\n");
                scanf ("%i", &exit_confirmation);
              
                if (exit_confirmation == 1) {
                    clear_terminal();
                    printf ("Saindo...");
                    log_status.account_code = -1;
                    log_status.logged_in = false;
                    exit(0);
                }
              
                break;
            default:
                clear_terminal();
                printf ("Opção inválida.\nPor favor, digite uma opção existente.\n\n");
                break;
        }
    }

    return 0;
}
