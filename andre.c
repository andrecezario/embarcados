#include "andre.h"
// Atividade 01 - Sistemas Embarcados 2019.2

enum spaces_house { ROOM = 0, KITCHEN = 1, BEDROOM = 2, BWC = 3};
enum voice_command {VOICE_COMMAND1 = 0x14, VOICE_COMMAND2 = 0xB};
enum voice_code {VOICE_CODE1 = 0x05, VOICE_CODE2 = 0x06};
enum assignment {LIGHTING = 0, SOUND = 1, AIR_CONDITIONING = 2, WINDOWS = 3, DOOR = 4, TV = 5};

#define VOICE '1'
#define APP '0'
#define SET_BIT(byte, bit) byte = byte | (1 << bit)    

int smart_house_device_init(device *home_d, char *user, char wifi, unsigned int content)
{
    strcpy(home_d->user, user);
    home_d->connection_wifi = wifi;
    home_d->mode = '1';
    home_d->current_command.content = content;

    return 0;
}

void house_init(my_house *house)
{
    memset(house->room, 0, sizeof(house->room));
    memset(house->bwc, 0, sizeof(house->bwc));
    memset(house->bedroom, 0, sizeof(house->bedroom));
    memset(house->kitchen, 0, sizeof(house->kitchen));

    space_init(house->room);
    space_init(house->kitchen);
    space_init(house->bedroom);
    space_init(house->bwc);
}

void app_command_init(command *cmd)
{
    cmd->app_command.lighting = 0;
    cmd->app_command.sound = 0;
    cmd->app_command.air_conditioning = 0;
    cmd->app_command.windows = 0;
    cmd->app_command.door = 0;
    cmd->app_command.tv = 0;
}

void space_init(char *space_house) 
{
    space_house[LIGHTING] = '0';
    space_house[SOUND] = '0';
    space_house[AIR_CONDITIONING] = '0';
    space_house[WINDOWS] = '0';
    space_house[DOOR] = '0';
    space_house[TV] = '0';
}

int smart_house_device_isconnected(device *home_d) 
{
   if(home_d->connection_wifi == '1'){
       return 1;
   }
   return 0;
}

int check_voice_code(unsigned int code) {
   if(code == VOICE_CODE1 || code == VOICE_CODE2){
       return 1;
   }
   return 0;
}

void smart_house_device_print(device *home_d)
{
    printf("Username: %s\n", home_d->user);
    switch (home_d->connection_wifi){
    case '0':
        printf("device System is not Connected.\n");
        break;
    case '1':
        printf("device System is Connected!\n");
        break;
    default:
        printf("[ERROR] something is wrong =(.\n");
        break;
    }
}

void smart_house_device_execute(device *home_d, my_house *house, char mode, command *cmd){
    if(smart_house_device_isconnected(home_d)) {
        char *space_house;
        //cmd->app_command = {0};
        switch (mode) 
        {
        case VOICE: 
           if(check_voice_code(cmd->voice_command.val)) {
               switch (cmd->voice_command.interpret)
               {
               case VOICE_COMMAND1: //"ligar luzes da sala e abrir janela da cozinha"
                   house->room[LIGHTING] = '1';
                   house->kitchen[WINDOWS] = '1';
                   break;
                
               case VOICE_COMMAND2: //01011 == "ligar luz do banheiro e fechar a porta"
                   house->bwc[LIGHTING] = '1';
                   house->bwc[DOOR] = '0';
                   break;
               
               default:
                   break;
               }
           }    
           else {
               printf("this voice code is not valid!\n");
           }
            break;

        case APP:       
            switch (cmd->app_command.place_code)
            {
            
            case ROOM:
                space_house = house->room;
                break;

            case KITCHEN:
                space_house = house->kitchen;
                break;

            case BEDROOM:
                 space_house = house->bedroom;
                break;
            
            case BWC:
                 space_house = house->bwc;
                break;
            
            default:
               
                break;
            }

            space_house[LIGHTING] = cmd->app_command.lighting+'0';
            space_house[SOUND] = cmd->app_command.sound+'0';
            space_house[AIR_CONDITIONING] = cmd->app_command.air_conditioning+'0';
            space_house[WINDOWS] = cmd->app_command.windows+'0';
            space_house[DOOR] = cmd->app_command.door+'0';
            space_house[TV] = cmd->app_command.tv+'0';
            break;
        
        default:
            printf("this command is not valid!");
            break;
        }


    }
    else {
        printf("wifi is not Connected!\n");
    }
}

void smart_house_device_print_binary_content(unsigned int cmd){

}

void smart_house_device_monitor(my_house *house){
    printf("## MONITOR\n");
    printf("# SALA");
    smart_house_space_print(house->room);
    printf("---------- * ----------\n");
    printf("# COZINHA");
        smart_house_space_print(house->kitchen);
    printf("---------- * ----------\n");
    printf("# QUARTO");
        smart_house_space_print(house->bedroom);
    printf("---------- * ----------\n");
    printf("# BANHEIRO");
    smart_house_space_print(house->bwc);
    printf("---------- * ----------\n");

    /* Elabore aqui um printf que siga a representacao abaixo: */
    // ## MONITOR 
    // # (AMBIENTE): 
    // >> ILUMINÇÃO: (1 ou 0)
    // >> SOM AMBIENTE: (1 ou 0)
    // >> AR CONDICIONADO: (1 ou 0)
    // >> JANELAS: (1 ou 0)
    // >> PORTA: (1 ou 0)
    // >> TV: (1 ou 0)
    // ---------- * ----------
}

void smart_house_space_print(char *space) {
    printf("\n>> ILUMINACAO: %c"
    "\n>> SOM AMBIENTE: %c"
    "\n>> AR CONDICIONADO: %c"
    "\n>> JANELAS: %c"
    "\n>> PORTA: %c"
    "\n>> TV: %c\n", space[LIGHTING], space[SOUND], space[AIR_CONDITIONING], space[WINDOWS], space[DOOR], space[TV]);
}

int main(int argc, char const *argv[])
{
    // Declare
    device home_d;
    my_house house;
    command cmd = {0};

    // Inicialize
    smart_house_device_init(&home_d, "Andre", '1', cmd.content);
    house_init(&house);

    // Verifique
    smart_house_device_print(&home_d);
    //smart_house_device_monitor(&house);
    
    // Obs.: Lembre-se sempre verificar se o dispositivo esta conectado ao wi-fi antes de executar alguma acao
    // Obs.: Seu codigo voz eh 101, sempre verifique antes de executar um comando de voz

    // EXECUTE:
    // Comando de voz    
    /// (codigo de voz) : 101
    /// (comando) : 10100 == "ligar luzes da sala e abrir janela da cozinha"

    cmd.content = 0;
    //cmd.app_command = 0;

    cmd.voice_command.val = 0x05; //0x005->0b101 
    cmd.voice_command.interpret = 0x14; //0x014->0b10100

    smart_house_device_execute(&home_d, &house, VOICE, &cmd);
    smart_house_device_monitor(&house);
 
    // EXECUTE:
        // Comando de app
        // (ambiente) sala
        /// ligar luzes + som ambiente + abrir janelas + fechar porta
    app_command_init(&cmd);
    cmd.app_command.place_code = ROOM;
    cmd.app_command.lighting = 1;
    cmd.app_command.sound = 1;
    cmd.app_command.windows = 1;
    cmd.app_command.door = 0;

    //cmd.content = (1<<LIGHTING) | (1<<SOUND) | (1<<WINDOWS) | (0<<DOOR);
    //printf("\n%x",cmd.content);

    smart_house_device_execute(&home_d, &house, APP, &cmd);
    smart_house_device_monitor(&house);

    
    // EXECUTE:
    // Comando de voz
    /// (codigo de voz) : 110
    /// (comando) : 01011 == "ligar luz do banheiro e fechar a porta"

    cmd.voice_command.val = 0x06; //0x006->0b110 
    cmd.voice_command.interpret = 0x0B; //0x0B->0b1011

    smart_house_device_execute(&home_d, &house, VOICE, &cmd);
    smart_house_device_monitor(&house);


    /* EXECUTE:
        // Comando de app
        // (ambiente): quarto
        /// ligar luz +  ar condicionado + fechar janela + abrir porta
    // smart_house_device_execute();
    // smart_house_device_monitor();
    */
    app_command_init(&cmd);
    cmd.app_command.place_code = BEDROOM;
    cmd.app_command.lighting = 1;
    cmd.app_command.air_conditioning = 1;
    cmd.app_command.windows = 0;
    cmd.app_command.door = 1;

    smart_house_device_execute(&home_d, &house, APP, &cmd);
    smart_house_device_monitor(&house);

    // EXECUTE:
    // Comando de voz
    /// (codigo de voz) : 101
    /// (comando) : 01011 == "ligar luz do banheiro e fechar a porta"

    return 1;
}
