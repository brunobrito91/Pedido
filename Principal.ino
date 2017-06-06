//Carrega a biblioteca Keypad
#include <Keypad.h>
#include <LiquidCrystal.h>
#include <Ethernet.h>

//REDE
byte _mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
IPAddress _ip(101, 100, 21, 86);
IPAddress _ipServidor(101, 100, 21, 85);

unsigned int _porta1 = 9876;

EthernetClient _cliente;
//REDE


//Definicao da quantidade de linhas e colunas
const byte LINHAS = 4;
const byte COLUNAS = 3;

//Matriz de caracteres
char matriz_teclas[LINHAS][COLUNAS] =
{
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

//Definicao dos pinos das linhas
byte PinosLinhas[LINHAS] = {45, 43, 41, 39};
//Definicao dos pinos das colunas
byte PinosColunas[COLUNAS] = {37, 35, 33};

//Variaveis de uso geral do programa
int ctrl; //Controle de Processo
int ctrlpedido; //Controle de Número de Pedido
int ctrldisplay; //Controle Display
int exc; //Contador do menu de excluir itens
int ctrlexc; //Controle de menu de excluir itens
int adc; //Controle de menu de adicionar itens
int ctrladc; //Controle de menu de adicionar itens
int i;//Controle digitação do Display
int excmais;//Controla a opção já excluida
int excmais1;//Controla a opção já excluida 1
int excmais2;//Controla a opção já excluida 2
int excmais3;//Controla a opção ja excluida 3
int excmais4;//Controla a opção ja excluida 4
int excmais5;//Controla a opção ja excluida 5
int adcmais;//Controla a opção já incluido
int adcmais1;//Controla a opção já incluido 1
int adcmais2;//Controla a opção já incluido 2
int adcmais3;//Controla a opção ja incluido 3
int adcmais4;//Controla a opção ja incluido 4
int adcmais5;//Controla a opção ja incluido 5
int j;//Controle de alocação de dados nas variaveis de exclusão
String pedido;//Armazena os pedidos
String subpedido;//Armazena os processos de pedidos


//Inicializa o teclado
Keypad meuteclado = Keypad(makeKeymap(matriz_teclas), PinosLinhas, PinosColunas, LINHAS, COLUNAS);

//Indica os pinos do Display 20x4
LiquidCrystal lcd(12, 11, 7, 6, 5, 4);

void setup() {
  Serial.begin(9600);//Excluir do Programa

  Ethernet.begin(_mac, _ip);
  conectarClienteAoServidorExterno();

  // Define o LCD com 20 colunas e 4 linhas
  lcd.begin(20, 4);

  i = 8;
  ctrl = 0;
  ctrlpedido = 0;
  ctrldisplay = 0;
  exc = 0;
  ctrlexc = 0;
  adc = 0;
  ctrladc = 0;
  pedido = "";
  excmais = 0;
  excmais1 = 0;
  excmais2 = 0;
  excmais3 = 0;
  excmais4 = 0;
  excmais5 = 0;
  adcmais = 0;
  adcmais1 = 0;
  adcmais2 = 0;
  adcmais3 = 0;
  adcmais4 = 0;
  adcmais5 = 0;
  j = 0;
}

void loop() {
  delay(10);
  while (!_cliente.connected()) {
    conectarClienteAoServidorExterno();
  }
  while ((ctrl == 0)) {
    char tecla_pressionada = meuteclado.getKey();//Verifica se alguma tecla foi pressionada
    //Inicio do Programa no Display
    lcd.setCursor(5, 0);
    lcd.print("Bem Vindo");
    lcd.setCursor(2, 1);
    lcd.print("Faca seu pedido");
    lcd.setCursor(0, 3);
    lcd.print("* Apagar");
    lcd.setCursor(9, 3);
    lcd.print("# Confirmar");

    if ((tecla_pressionada != '#') && (tecla_pressionada != '*') && (tecla_pressionada)) { //Se os valores são numericos, então escreve na variavel.
      if (tecla_pressionada != '#') {
        lcd.setCursor(i, 2);
        lcd.print(tecla_pressionada);
        i++;
        subpedido = subpedido + tecla_pressionada; //Concatena Strings
      }

    }

    if ((tecla_pressionada == '*') && (tecla_pressionada)) { //Função para apagar valores da String

      subpedido = ""; //Zera a Variavel

      if (tecla_pressionada == '*') { //Apaga Toda a terceira linha do display
        lcd.setCursor(0, 2);
        lcd.print("                ");
        i = 8;
      }

    }

    if ((tecla_pressionada == '#') && (tecla_pressionada)) { //Se os valores são numericos, então escreve na variavel.
      ctrlpedido = atoi(subpedido.c_str());//Conversão da String "subpedido" para int "ctrlpedido".

      if ((ctrlpedido >= 1) && (ctrlpedido <= 9)) { //Inclussão de um Zero nos pedidos de 1 a 9
        subpedido = ('0' + subpedido);
        pedido = pedido + subpedido;
        pedido = pedido + "#";
        subpedido = "";
        lcd.clear();
        ctrl = 1;
        ctrldisplay = 0;
      }

      if ((ctrlpedido >= 10) && (ctrlpedido <= 200)) { // Pedidos no formato correto, apenas sai do loop
        pedido = pedido + subpedido;
        pedido = pedido + "#";
        subpedido = "";
        lcd.clear();
        ctrldisplay = 0;
        ctrl = 1;
      }


      if ((ctrlpedido >= 201) && (ctrlpedido <= 299)) { //Pedidos de Bares não possui adição e nem exclusão
        pedido = pedido + subpedido;
        pedido = pedido + "#00#00#";
        lcd.clear();
        subpedido = "";
        ctrldisplay = 0;
        ctrl = 6;
        i = 8;
      }


      if ((ctrlpedido > 299)) { //Pedidos de Bares invalidos do menu
        lcd.clear();//Limpa LCD

        lcd.setCursor(3, 0); //Indica que opção digitada é invalida
        lcd.print("Opcao Invalida");
        lcd.setCursor(4, 1);
        lcd.print("Favor Refazer");
        lcd.setCursor(6, 2);
        lcd.print("o Pedido");
        delay (3000); // 3 segundos de exibição no Display

        lcd.clear();//Limpa LCD

        i = 8;//Posiciona o cursor no meio do Display

        subpedido = "";
        pedido = "";
        ctrl = 0;
      }
    }

  }









  while ((ctrl == 1)) { //Confirmação de Pedido
    char tecla_pressionada = meuteclado.getKey();//Verifica se alguma tecla foi pressionada

    if (ctrldisplay == 0) {
      lcd.setCursor(2, 0);
      lcd.print("Confirma Pedido?");
      lcd.setCursor(3, 1);
      lcd.print("# -> Confirmar");
      lcd.setCursor(3, 2);
      lcd.print("* -> Cancelar");
      ctrldisplay++;
    }


    if ((tecla_pressionada == '*') && (tecla_pressionada)) { //Cancela o pedido e retorna ao inicio

      //Informa que o pedido foi cancelado
      lcd.clear();//Limpa LCD
      lcd.setCursor(2, 1);
      lcd.print("Pedido Cancelado");
      lcd.setCursor(2, 2);
      lcd.print("Refaca o Pedido");
      delay (3000);//3 segundos de exibição no Display

      lcd.clear();//Limpa LCD

      i = 8;
      pedido = "";
      ctrldisplay = 0;
      ctrl = 0;
    }

    if ((tecla_pressionada == '#') && (tecla_pressionada)) { //Confirma o pedido
      lcd.clear();//Limpa LCD
      ctrldisplay = 0;
      ctrl = 2;
      exc = 0;
    }
  }









  while ((ctrl == 2)) { //Questionamento se exclui algum ingrediente
    char tecla_pressionada = meuteclado.getKey();//Verifica se alguma tecla foi pressionada
    subpedido = ""; //Zera a Variavel dos Sub Pedidos


    if (ctrldisplay == 0) {
      lcd.clear();//Limpa LCD
      lcd.setCursor(0, 0);
      lcd.print("Excluir Ingrediente?");
      lcd.setCursor(6, 1);
      lcd.print("# -> Sim");
      lcd.setCursor(6, 2);
      lcd.print("* -> Nao");
      ctrldisplay++;
    }

    exc = 0;

    if ((tecla_pressionada == '*') && (tecla_pressionada)) {
      lcd.clear();
      pedido = (pedido + "00#");
      ctrldisplay = 0;
      ctrl = 4;
    }

    if ((tecla_pressionada == '#') && (tecla_pressionada)) {
      lcd.clear();
      ctrl = 3;
    }
  }









  while ((ctrl == 3)) { //Opções de Exclussão, conforme o item.



    //Para itens com apenas 1 ingredientes
    if ((ctrlpedido == 17) || (ctrlpedido == 48) || (ctrlpedido == 72) || (ctrlpedido == 88) || (ctrlpedido == 93) || (ctrlpedido == 104)) {
      lcd.setCursor(4, 0);
      lcd.print("Item possui");
      lcd.setCursor(7, 1);
      lcd.print("Apenas");
      lcd.setCursor(3, 2);
      lcd.print("1 ingrediente");
      lcd.setCursor(1, 3);
      lcd.print("Impossivel exclusao");
      delay (5000);//5 segundos de exibição

      lcd.clear();//Limpa LCD

      ctrldisplay = 0;
      pedido = pedido + "00#";
      ctrl = 4;
    }








    //Para itens com apenas 2 ingredientes
    if ((ctrlpedido == 1) || (ctrlpedido == 13) || (ctrlpedido == 18) || (ctrlpedido == 19) || (ctrlpedido == 20) || (ctrlpedido == 26) || (ctrlpedido == 27) ||
        (ctrlpedido == 31) || (ctrlpedido == 32) || (ctrlpedido == 35) || (ctrlpedido == 36) || (ctrlpedido == 44) || (ctrlpedido == 49) || (ctrlpedido == 51) ||
        (ctrlpedido == 54) || (ctrlpedido == 57) || (ctrlpedido == 67) || (ctrlpedido == 69) || (ctrlpedido == 73) || (ctrlpedido == 75) || (ctrlpedido == 76) ||
        (ctrlpedido == 87) || (ctrlpedido == 89) || (ctrlpedido == 95) || (ctrlpedido == 96) || (ctrlpedido == 97) || (ctrlpedido == 101) || (ctrlpedido == 102)) {

      lcd.clear();//Limpa LCD

      lcd.setCursor(5, 0);
      lcd.print("LEMBRANDO");
      lcd.setCursor(4, 1);
      lcd.print("Item possui");
      lcd.setCursor(3, 2);
      lcd.print("2 ingredientes");
      delay (5000);//5 segundos de exibição

      lcd.clear();//Limpa LCD
      i = 8;

      while ((exc == 0)) {
        char tecla_pressionada = meuteclado.getKey();//Verifica se alguma tecla foi pressionada

        //Escreve na Tela
        lcd.setCursor(3, 0);
        lcd.print("Excluir item:");

        //Se String Subpedido estiver com qualquer valor dentro, escreverá apagar
        if ((subpedido.length() > 0)) {
          lcd.setCursor(0, 3);
          lcd.print("* Apagar");
        }

        //Escreve na Tela
        lcd.setCursor(9, 3);
        lcd.print("# Confirmar");

        //Teclas númericas digitadas, salva na String Subpedido
        if ((tecla_pressionada != '#') && (tecla_pressionada != '*') && (tecla_pressionada)) { //Escreve na Terceira linha do display
          lcd.setCursor(i, 2);
          lcd.print(tecla_pressionada);
          subpedido = subpedido + tecla_pressionada; //Concatena Strings
          i++;
        }

        //Se String Subpedido for igual 0, escrever na tela voltar
        if ((subpedido.length() == 0)) {
          lcd.setCursor(0, 3);
          lcd.print("* Voltar");
        }

        //Se * pressionado e String Subpedido vazia, retornar ao item anterior
        if ((subpedido.length() == 0) && (tecla_pressionada == '*') && (tecla_pressionada)) {
          lcd.clear();
          ctrldisplay = 0;
          exc = 3;
          ctrl = 2;
        }


        //Se * pressionado e String Subpedido com informação, apagar informações na variavel
        if ((tecla_pressionada == '*') && (tecla_pressionada) && (subpedido.length() > 0)) { //Apaga Toda a terceira linha do display
          lcd.setCursor(0, 2);
          lcd.print("                ");
          subpedido = "";
          i = 8;
        }

        //Verifica e envia a informação, ou retorna para digitar novamente.
        if ((tecla_pressionada == '#') && (tecla_pressionada)) {
          ctrlexc = atoi(subpedido.c_str());//Conversão da String "subpedido" para int "ctrlexc".

          if ((ctrlexc >= 1) && (ctrlexc <= 2)) { //Inclussão de um Zero nos pedidos de 1 a 9
            subpedido = ('0' + subpedido);
            pedido = pedido + subpedido;
            pedido = pedido + "#";
            lcd.clear();
            subpedido = "";
            ctrldisplay = 0;
            ctrl = 4;
            exc = 1;
          }

          if ((ctrlexc > 2)) {
            lcd.clear();//Limpa LCD

            //Escreve que Opção é Invalida
            lcd.setCursor(3, 0);
            lcd.print("Opcao Invalida");
            lcd.setCursor(7, 1);
            lcd.print("Digite");
            lcd.setCursor(7, 2);
            lcd.print("1 ou 2");
            delay (3000);//3 segundos de exibição

            lcd.clear();//Limpa LCD

            subpedido = "";
          }
        }


      }//Final do While excluir


    }//Final do if









    //Para itens com apenas 3 ingredientes
    if ((ctrlpedido == 5) || (ctrlpedido == 11) || (ctrlpedido == 14) || (ctrlpedido == 15) || (ctrlpedido == 16) || (ctrlpedido == 21) || (ctrlpedido == 23) ||
        (ctrlpedido == 28) || (ctrlpedido == 30) || (ctrlpedido == 33) || (ctrlpedido == 37) || (ctrlpedido == 41) || (ctrlpedido == 42) || (ctrlpedido == 46) ||
        (ctrlpedido == 47) || (ctrlpedido == 52) || (ctrlpedido == 53) || (ctrlpedido == 56) || (ctrlpedido == 60) || (ctrlpedido == 68) || (ctrlpedido == 71) ||
        (ctrlpedido == 74) || (ctrlpedido == 77) || (ctrlpedido == 78) || (ctrlpedido == 80) || (ctrlpedido == 86) || (ctrlpedido == 90) || (ctrlpedido == 94) ||
        (ctrlpedido == 99) || (ctrlpedido == 103) || (ctrlpedido == 105) || (ctrlpedido == 106) || (ctrlpedido == 107) || (ctrlpedido == 108)) {

      lcd.clear();//Limpa LCD

      lcd.setCursor(5, 0);
      lcd.print("LEMBRANDO");
      lcd.setCursor(4, 1);
      lcd.print("Item possui");
      lcd.setCursor(3, 2);
      lcd.print("3 ingredientes");
      delay (5000);//5 segundos de exibição

      lcd.clear();//Limpa LCD
      i = 8;

      while ((exc == 0)) {
        char tecla_pressionada = meuteclado.getKey();//Verifica se alguma tecla foi pressionada

        //Escreve na Tela
        lcd.setCursor(3, 0);
        lcd.print("Excluir item");

        //Se String Subpedido estiver com qualquer valor dentro, escreverá apagar
        if ((subpedido.length() > 0)) {
          lcd.setCursor(0, 3);
          lcd.print("* Apagar");
        }

        //Escreve na Tela
        lcd.setCursor(9, 3);
        lcd.print("# Confirmar");

        //Teclas númericas digitadas, salva na String Subpedido
        if ((tecla_pressionada != '#') && (tecla_pressionada != '*') && (tecla_pressionada)) { //Escreve na Terceira linha do display
          lcd.setCursor(i, 2);
          lcd.print(tecla_pressionada);
          subpedido = subpedido + tecla_pressionada; //Concatena Strings
          i++;
        }

        //Se String Subpedido for igual 0, escrever na tela voltar
        if ((subpedido.length() == 0)) {
          lcd.setCursor(0, 3);
          lcd.print("* Voltar");
        }

        //Se * pressionado e String Subpedido vazia, retornar ao item anterior
        if ((subpedido.length() == 0) && (tecla_pressionada == '*') && (tecla_pressionada)) {
          lcd.clear();
          ctrldisplay = 0;
          ctrl = 2;
          exc = 3;
        }


        //Se * pressionado e String Subpedido com informação, apagar informações na variavel
        if ((tecla_pressionada == '*') && (tecla_pressionada) && (subpedido.length() > 0)) { //Apaga Toda a terceira linha do display
          lcd.setCursor(0, 2);
          lcd.print("                ");
          subpedido = "";
          i = 8;
        }

        //Verifica e envia a informação, ou retorna para digitar novamente.
        if ((tecla_pressionada == '#') && (tecla_pressionada)) {
          ctrlexc = atoi(subpedido.c_str());//Conversão da String "subpedido" para int "ctrlexc".

          if ((ctrlexc >= 1) && (ctrlexc <= 3)) { //Inclussão de um Zero nos pedidos de 1 a 9
            subpedido = ('0' + subpedido);
            pedido = pedido + subpedido;
            excmais = atoi(subpedido.c_str());//Conversão da String "subpedido" para int "excmais".
            subpedido = "";
            lcd.clear();
            ctrldisplay = 0;
            exc = 1;
          }

          if ((ctrlexc > 3)) {
            lcd.clear();//Limpa LCD

            //Escreve que Opção é Invalida
            lcd.setCursor(3, 0);
            lcd.print("Opcao Invalida");
            lcd.setCursor(7, 1);
            lcd.print("Digite");
            lcd.setCursor(5, 2);
            lcd.print("1, 2 ou 3");
            delay (3000);//3 segundos de exibição

            lcd.clear();//Limpa LCD

            subpedido = "";
          }
        }


      }//Final do While excluir





      //Processo de exclusão de mais itens
      while (exc == 1) {
        char tecla_pressionada = meuteclado.getKey();//Verifica se alguma tecla foi pressionada

        if (ctrldisplay == 0) {
          lcd.setCursor(4, 0);
          lcd.print("Excluir Mais?");
          lcd.setCursor(7, 1);
          lcd.print("Digite");
          lcd.setCursor(0, 3);
          lcd.print("* Nao");
          i = 8;
          ctrldisplay++;
        }



        //Teclas númericas digitadas, salva na String Subpedido
        if ((tecla_pressionada != '#') && (tecla_pressionada != '*') && (tecla_pressionada)) { //Escreve na Terceira linha do display
          lcd.setCursor(i, 2);
          lcd.print(tecla_pressionada);
          subpedido = subpedido + tecla_pressionada; //Concatena Strings
          i++;
        }

        //Se String Subpedido for maior que 0, escrever Apagar
        if ((subpedido.length() > 0)) {
          lcd.setCursor(0, 3);
          lcd.print("* Apagar");
        }

        //Se String Subpedido for maior que 0, escrever Apagar
        if ((subpedido.length() > 0)) {
          lcd.setCursor(9, 3);
          lcd.print("# Confirmar");
        }

        //Se * pressionado e String Subpedido vazia, retornar ao item anterior
        if ((subpedido.length() == 0) && (tecla_pressionada == '*') && (tecla_pressionada)) {
          lcd.clear();
          pedido = pedido + "#";
          ctrldisplay = 0;
          ctrl = 4;
          exc = 2;
        }


        if ((tecla_pressionada == '*') && (tecla_pressionada) && (subpedido.length() > 0)) { //Apaga Toda a terceira linha do display
          lcd.clear();
          ctrldisplay = 0;
          subpedido = "";
          i = 8;
        }



        if ((tecla_pressionada == '#') && (tecla_pressionada)) {
          ctrlexc = atoi(subpedido.c_str());//Conversão da String "subpedido" para int "ctrlexc".

          if ((ctrlexc >= 1) && (ctrlexc <= 3) && (ctrlexc != excmais)) { //Inclussão de um Zero nos pedidos de 1 a 9
            subpedido = ('0' + subpedido);
            pedido = pedido + subpedido;
            pedido = pedido + "#";
            lcd.clear();
            subpedido = "";
            ctrldisplay = 0;
            ctrl = 4;
            exc = 2;
          }

          if ((ctrlexc == excmais) && (ctrlexc >= 1) && (ctrlexc <= 3)) {
            lcd.clear();//Limpa LCD

            //Escreve que Opção é Invalida
            lcd.setCursor(8, 1);
            lcd.print("ITEM");
            lcd.setCursor(4, 2);
            lcd.print("JA EXCLUIDO");
            delay (3000);//3 segundos de exibição

            lcd.clear();//Limpa LCD

            subpedido = "";
            ctrldisplay = 0;
          }

          if (ctrlexc > 3) {
            lcd.clear();//Limpa LCD

            //Escreve que Opção é Invalida
            lcd.setCursor(7, 1);
            lcd.print("OPCAO");
            lcd.setCursor(5, 2);
            lcd.print("INVALIDA");
            delay (3000);//3 segundos de exibição

            lcd.clear();//Limpa LCD

            subpedido = "";
            ctrldisplay = 0;
          }

        }



      }


    }//Final do if














    if ((ctrlpedido == 4) || (ctrlpedido == 9) || (ctrlpedido == 24) || (ctrlpedido == 25) || (ctrlpedido == 29) || (ctrlpedido == 38) || (ctrlpedido == 40) ||
        (ctrlpedido == 43) || (ctrlpedido == 45) || (ctrlpedido == 50) || (ctrlpedido == 55) || (ctrlpedido == 58) || (ctrlpedido == 61) || (ctrlpedido == 62) ||
        (ctrlpedido == 63) || (ctrlpedido == 64) || (ctrlpedido == 65) || (ctrlpedido == 70) || (ctrlpedido == 79) || (ctrlpedido == 81) || (ctrlpedido == 85) ||
        (ctrlpedido == 91) || (ctrlpedido == 98) || (ctrlpedido == 100) || (ctrlpedido == 109) || (ctrlpedido == 110)) {

      lcd.clear();//Limpa LCD

      lcd.setCursor(5, 0);
      lcd.print("LEMBRANDO");
      lcd.setCursor(4, 1);
      lcd.print("Item possui");
      lcd.setCursor(3, 2);
      lcd.print("4 ingredientes");
      delay (5000);//5 segundos de exibição

      lcd.clear();//Limpa LCD
      i = 8;

      while ((exc == 0)) {
        char tecla_pressionada = meuteclado.getKey();//Verifica se alguma tecla foi pressionada

        //Escreve na Tela
        lcd.setCursor(3, 0);
        lcd.print("Excluir item:");

        //Se String Subpedido estiver com qualquer valor dentro, escreverá apagar
        if ((subpedido.length() > 0)) {
          lcd.setCursor(0, 3);
          lcd.print("* Apagar");
        }

        //Escreve na Tela
        lcd.setCursor(9, 3);
        lcd.print("# Confirmar");

        //Teclas númericas digitadas, salva na String Subpedido
        if ((tecla_pressionada != '#') && (tecla_pressionada != '*') && (tecla_pressionada)) { //Escreve na Terceira linha do display
          lcd.setCursor(i, 2);
          lcd.print(tecla_pressionada);
          subpedido = subpedido + tecla_pressionada; //Concatena Strings
          i++;
        }

        //Se String Subpedido for igual 0, escrever na tela voltar
        if ((subpedido.length() == 0)) {
          lcd.setCursor(0, 3);
          lcd.print("* Voltar");
        }

        //Se * pressionado e String Subpedido vazia, retornar ao item anterior
        if ((subpedido.length() == 0) && (tecla_pressionada == '*') && (tecla_pressionada)) {
          lcd.clear();
          ctrldisplay = 0;
          ctrl = 2;
          exc = 2;
        }


        //Se * pressionado e String Subpedido com informação, apagar informações na variavel
        if ((tecla_pressionada == '*') && (tecla_pressionada) && (subpedido.length() > 0)) { //Apaga Toda a terceira linha do display
          lcd.setCursor(0, 2);
          lcd.print("                ");
          subpedido = "";
          i = 8;
        }

        //Verifica e envia a informação, ou retorna para digitar novamente.
        if ((tecla_pressionada == '#') && (tecla_pressionada)) {
          ctrlexc = atoi(subpedido.c_str());//Conversão da String "subpedido" para int "ctrlexc".

          if ((ctrlexc >= 1) && (ctrlexc <= 4)) { //Inclussão de um Zero nos pedidos de 1 a 9
            subpedido = ('0' + subpedido);
            pedido = pedido + subpedido;
            ctrldisplay = 0;
            excmais = atoi(subpedido.c_str());//Conversão da String "subpedido" para int "excmais".
            lcd.clear();
            subpedido = "";
            exc = 1;
          }

          if ((ctrlexc > 4)) {
            lcd.clear();//Limpa LCD

            //Escreve que Opção é Invalida
            lcd.setCursor(3, 0);
            lcd.print("Opcao Invalida");
            lcd.setCursor(7, 1);
            lcd.print("Digite");
            lcd.setCursor(4, 2);
            lcd.print("1, 2, 3 ou 4");
            delay (3000);//3 segundos de exibição

            lcd.clear();//Limpa LCD

            subpedido = "";
          }
        }


      }//Final do While excluir






      //Processo de exclusão de mais itens
      while (exc == 1) {
        char tecla_pressionada = meuteclado.getKey();//Verifica se alguma tecla foi pressionada

        if (ctrldisplay == 0) {
          lcd.setCursor(4, 0);
          lcd.print("Excluir Mais?");
          lcd.setCursor(7, 1);
          lcd.print("Digite");
          lcd.setCursor(0, 3);
          lcd.print("* Nao");
          i = 8;
          ctrldisplay++;
        }



        //Teclas númericas digitadas, salva na String Subpedido
        if ((tecla_pressionada != '#') && (tecla_pressionada != '*') && (tecla_pressionada)) { //Escreve na Terceira linha do display
          lcd.setCursor(i, 2);
          lcd.print(tecla_pressionada);
          subpedido = subpedido + tecla_pressionada; //Concatena Strings
          i++;
        }

        //Se String Subpedido for maior que 0, escrever Apagar
        if ((subpedido.length() > 0)) {
          lcd.setCursor(0, 3);
          lcd.print("* Apagar");
        }

        //Se String Subpedido for maior que 0, escrever Apagar
        if ((subpedido.length() > 0)) {
          lcd.setCursor(9, 3);
          lcd.print("# Confirmar");
        }

        //Se * pressionado e String Subpedido vazia, retornar ao item anterior
        if ((subpedido.length() == 0) && (tecla_pressionada == '*') && (tecla_pressionada)) {
          lcd.clear();
          pedido = pedido + "#";
          ctrldisplay = 0;
          ctrl = 4;
          exc = 2;
        }


        if ((tecla_pressionada == '*') && (tecla_pressionada) && (subpedido.length() > 0)) { //Apaga Toda a terceira linha do display
          lcd.clear();
          ctrldisplay = 0;
          subpedido = "";
          i = 8;
        }



        if ((tecla_pressionada == '#') && (tecla_pressionada)) {
          ctrlexc = atoi(subpedido.c_str());//Conversão da String "subpedido" para int "ctrlexc".


          if ((ctrlexc == excmais) && (ctrlexc >= 1) && (ctrlexc <= 4) ||
              (ctrlexc == excmais1) && (ctrlexc >= 1) && (ctrlexc <= 4)) {
            lcd.clear();//Limpa LCD

            //Escreve que Opção é Invalida
            lcd.setCursor(8, 1);
            lcd.print("ITEM");
            lcd.setCursor(4, 2);
            lcd.print("JA EXCLUIDO");
            delay (3000);//3 segundos de exibição

            lcd.clear();//Limpa LCD

            subpedido = "";
            ctrldisplay = 0;
          }


          if ((ctrlexc >= 1) && (ctrlexc <= 4) && (ctrlexc != excmais) && (ctrlexc != excmais1)) { //Inclussão de um Zero nos pedidos de 1 a 9
            if (j == 0) {
              excmais1 = atoi(subpedido.c_str());//Conversão da String "subpedido" para int "excmais1".
            }
            subpedido = ('0' + subpedido);
            pedido = pedido + subpedido;
            lcd.clear();
            subpedido = "";
            ctrldisplay = 0;
            j++;
          }



          if (ctrlexc > 4) {
            lcd.clear();//Limpa LCD

            //Escreve que Opção é Invalida
            lcd.setCursor(7, 1);
            lcd.print("OPCAO");
            lcd.setCursor(5, 2);
            lcd.print("INVALIDA");
            delay (3000);//3 segundos de exibição

            lcd.clear();//Limpa LCD

            subpedido = "";
            ctrldisplay = 0;
          }


          if (j == 2) {
            pedido = pedido + "#";
            ctrl = 4;
            exc = 2;
          }

        }//Final do if da confirmação.



      }//Final do While


    }//Final do if


    if ((ctrlpedido == 2) || (ctrlpedido == 6) || (ctrlpedido == 7) || (ctrlpedido == 12) || (ctrlpedido == 22) || (ctrlpedido == 34) || (ctrlpedido == 39) ||
        (ctrlpedido == 59) || (ctrlpedido == 82) || (ctrlpedido == 92)) {

      lcd.clear();//Limpa LCD

      lcd.setCursor(5, 0);
      lcd.print("LEMBRANDO");
      lcd.setCursor(4, 1);
      lcd.print("Item possui");
      lcd.setCursor(3, 2);
      lcd.print("5 ingredientes");
      delay (5000);//5 segundos de exibição

      lcd.clear();//Limpa LCD
      i = 8;

      while ((exc == 0)) {
        char tecla_pressionada = meuteclado.getKey();//Verifica se alguma tecla foi pressionada

        //Escreve na Tela
        lcd.setCursor(3, 0);
        lcd.print("Excluir item:");

        //Se String Subpedido estiver com qualquer valor dentro, escreverá apagar
        if ((subpedido.length() > 0)) {
          lcd.setCursor(0, 3);
          lcd.print("* Apagar");
        }

        //Escreve na Tela
        lcd.setCursor(9, 3);
        lcd.print("# Confirmar");

        //Teclas númericas digitadas, salva na String Subpedido
        if ((tecla_pressionada != '#') && (tecla_pressionada != '*') && (tecla_pressionada)) { //Escreve na Terceira linha do display
          lcd.setCursor(i, 2);
          lcd.print(tecla_pressionada);
          subpedido = subpedido + tecla_pressionada; //Concatena Strings
          i++;
        }

        //Se String Subpedido for igual 0, escrever na tela voltar
        if ((subpedido.length() == 0)) {
          lcd.setCursor(0, 3);
          lcd.print("* Voltar");
        }

        //Se * pressionado e String Subpedido vazia, retornar ao item anterior
        if ((subpedido.length() == 0) && (tecla_pressionada == '*') && (tecla_pressionada)) {
          lcd.clear();
          ctrl = 2;
          ctrldisplay = 0;
          exc = 2;
        }


        //Se * pressionado e String Subpedido com informação, apagar informações na variavel
        if ((tecla_pressionada == '*') && (tecla_pressionada) && (subpedido.length() > 0)) { //Apaga Toda a terceira linha do display
          lcd.setCursor(0, 2);
          lcd.print("                ");
          subpedido = "";
          i = 8;
        }

        //Verifica e envia a informação, ou retorna para digitar novamente.
        if ((tecla_pressionada == '#') && (tecla_pressionada)) {
          ctrlexc = atoi(subpedido.c_str());//Conversão da String "subpedido" para int "ctrlexc".

          if ((ctrlexc >= 1) && (ctrlexc <= 5)) { //Inclussão de um Zero nos pedidos de 1 a 9
            subpedido = ('0' + subpedido);
            pedido = pedido + subpedido;
            excmais = atoi(subpedido.c_str());//Conversão da String "subpedido" para int "excmais".
            lcd.clear();
            subpedido = "";
            ctrldisplay = 0;
            exc = 1;
          }

          if ((ctrlexc > 5)) {
            lcd.clear();//Limpa LCD

            //Escreve que Opção é Invalida
            lcd.setCursor(3, 0);
            lcd.print("Opcao Invalida");
            lcd.setCursor(7, 1);
            lcd.print("Digite");
            lcd.setCursor(2, 2);
            lcd.print("1, 2, 3, 4 ou 5");
            delay (3000);//3 segundos de exibição

            lcd.clear();//Limpa LCD

            subpedido = "";
          }
        }



      }//Final do While excluir

      //Processo de exclusão de mais itens
      while (exc == 1) {
        char tecla_pressionada = meuteclado.getKey();//Verifica se alguma tecla foi pressionada

        if (ctrldisplay == 0) {
          lcd.setCursor(4, 0);
          lcd.print("Excluir Mais?");
          lcd.setCursor(7, 1);
          lcd.print("Digite");
          lcd.setCursor(0, 3);
          lcd.print("* Nao");
          i = 8;
          ctrldisplay++;
        }



        //Teclas númericas digitadas, salva na String Subpedido
        if ((tecla_pressionada != '#') && (tecla_pressionada != '*') && (tecla_pressionada)) { //Escreve na Terceira linha do display
          lcd.setCursor(i, 2);
          lcd.print(tecla_pressionada);
          subpedido = subpedido + tecla_pressionada; //Concatena Strings
          i++;
        }

        //Se String Subpedido for maior que 0, escrever Apagar
        if ((subpedido.length() > 0)) {
          lcd.setCursor(0, 3);
          lcd.print("* Apagar");
        }

        //Se String Subpedido for maior que 0, escrever Apagar
        if ((subpedido.length() > 0)) {
          lcd.setCursor(9, 3);
          lcd.print("# Confirmar");
        }

        //Se * pressionado e String Subpedido vazia, retornar ao item anterior
        if ((subpedido.length() == 0) && (tecla_pressionada == '*') && (tecla_pressionada)) {
          lcd.clear();
          pedido = pedido + "#";
          ctrldisplay = 0;
          ctrl = 4;
          exc = 2;
        }


        if ((tecla_pressionada == '*') && (tecla_pressionada) && (subpedido.length() > 0)) { //Apaga Toda a terceira linha do display
          lcd.clear();
          ctrldisplay = 0;
          subpedido = "";
          i = 8;
        }



        if ((tecla_pressionada == '#') && (tecla_pressionada)) {
          ctrlexc = atoi(subpedido.c_str());//Conversão da String "subpedido" para int "ctrlexc".


          if ((ctrlexc == excmais) && (ctrlexc >= 1) && (ctrlexc <= 5) ||
              (ctrlexc == excmais1) && (ctrlexc >= 1) && (ctrlexc <= 5) ||
              (ctrlexc == excmais2) && (ctrlexc >= 1) && (ctrlexc <= 5)) {
            lcd.clear();//Limpa LCD

            //Escreve que Opção é Invalida
            lcd.setCursor(8, 1);
            lcd.print("ITEM");
            lcd.setCursor(4, 2);
            lcd.print("JA EXCLUIDO");
            delay (3000);//3 segundos de exibição

            lcd.clear();//Limpa LCD

            subpedido = "";
            ctrldisplay = 0;
          }


          if ((ctrlexc >= 1) && (ctrlexc <= 5) && (ctrlexc != excmais) && (ctrlexc != excmais1) && (ctrlexc != excmais2)) { //Inclussão de um Zero nos pedidos de 1 a 9
            if (j == 0) {
              excmais1 = atoi(subpedido.c_str());//Conversão da String "subpedido" para int "excmais1".
            }
            if (j == 1) {
              excmais2 = atoi(subpedido.c_str());//Conversão da String "subpedido" para int "excmais1".
            }
            subpedido = ('0' + subpedido);
            pedido = pedido + subpedido;
            lcd.clear();
            subpedido = "";
            ctrldisplay = 0;
            j++;
          }

          if (ctrlexc > 5) {
            lcd.clear();//Limpa LCD

            //Escreve que Opção é Invalida
            lcd.setCursor(7, 1);
            lcd.print("OPCAO");
            lcd.setCursor(5, 2);
            lcd.print("INVALIDA");
            delay (3000);//3 segundos de exibição

            lcd.clear();//Limpa LCD

            subpedido = "";
            ctrldisplay = 0;
          }


          if (j == 3) {
            pedido = pedido + "#";
            ctrl = 4;
            exc = 2;
          }

        }//Final do if da confirmação.



      }//Final do While


    }//Final do if


    if ((ctrlpedido == 3) || (ctrlpedido == 10) || (ctrlpedido == 66) || (ctrlpedido == 83) || (ctrlpedido == 84)) {

      lcd.clear();//Limpa LCD

      lcd.setCursor(5, 0);
      lcd.print("LEMBRANDO");
      lcd.setCursor(4, 1);
      lcd.print("Item possui");
      lcd.setCursor(3, 2);
      lcd.print("6 ingredientes");
      delay (5000);//5 segundos de exibição

      lcd.clear();//Limpa LCD
      i = 8;

      while ((exc == 0)) {
        char tecla_pressionada = meuteclado.getKey();//Verifica se alguma tecla foi pressionada

        //Escreve na Tela
        lcd.setCursor(3, 0);
        lcd.print("Excluir item:");

        //Se String Subpedido estiver com qualquer valor dentro, escreverá apagar
        if ((subpedido.length() > 0)) {
          lcd.setCursor(0, 3);
          lcd.print("* Apagar");
        }

        //Escreve na Tela
        lcd.setCursor(9, 3);
        lcd.print("# Confirmar");

        //Teclas númericas digitadas, salva na String Subpedido
        if ((tecla_pressionada != '#') && (tecla_pressionada != '*') && (tecla_pressionada)) { //Escreve na Terceira linha do display
          lcd.setCursor(i, 2);
          lcd.print(tecla_pressionada);
          subpedido = subpedido + tecla_pressionada; //Concatena Strings
          i++;
        }

        //Se String Subpedido for igual 0, escrever na tela voltar
        if ((subpedido.length() == 0)) {
          lcd.setCursor(0, 3);
          lcd.print("* Voltar");
        }

        //Se * pressionado e String Subpedido vazia, retornar ao item anterior
        if ((subpedido.length() == 0) && (tecla_pressionada == '*') && (tecla_pressionada)) {
          lcd.clear();
          ctrl = 2;
          ctrldisplay = 0;
          exc = 1;
        }


        //Se * pressionado e String Subpedido com informação, apagar informações na variavel
        if ((tecla_pressionada == '*') && (tecla_pressionada) && (subpedido.length() > 0)) { //Apaga Toda a terceira linha do display
          lcd.setCursor(0, 2);
          lcd.print("                ");
          subpedido = "";
          i = 8;
        }

        //Verifica e envia a informação, ou retorna para digitar novamente.
        if ((tecla_pressionada == '#') && (tecla_pressionada)) {
          ctrlexc = atoi(subpedido.c_str());//Conversão da String "subpedido" para int "ctrlexc".

          if ((ctrlexc >= 1) && (ctrlexc <= 6)) { //Inclussão de um Zero nos pedidos de 1 a 9
            subpedido = ('0' + subpedido);
            pedido = pedido + subpedido;
            excmais = atoi(subpedido.c_str());//Conversão da String "subpedido" para int "excmais".
            lcd.clear();
            subpedido = "";
            ctrldisplay = 0;
            exc = 1;
          }

          if ((ctrlexc > 6)) {
            lcd.clear();//Limpa LCD

            //Escreve que Opção é Invalida
            lcd.setCursor(3, 0);
            lcd.print("Opcao Invalida");
            lcd.setCursor(7, 1);
            lcd.print("Digite");
            lcd.setCursor(2, 2);
            lcd.print("1, 2, 3, 4, 5 ou 6");
            delay (3000);//3 segundos de exibição

            lcd.clear();//Limpa LCD

            subpedido = "";
          }
        }


      }//Final do While excluir

      //Processo de exclusão de mais itens
      while (exc == 1) {
        char tecla_pressionada = meuteclado.getKey();//Verifica se alguma tecla foi pressionada

        if (ctrldisplay == 0) {
          lcd.setCursor(4, 0);
          lcd.print("Excluir Mais?");
          lcd.setCursor(7, 1);
          lcd.print("Digite");
          lcd.setCursor(0, 3);
          lcd.print("* Nao");
          i = 8;
          ctrldisplay++;
        }



        //Teclas númericas digitadas, salva na String Subpedido
        if ((tecla_pressionada != '#') && (tecla_pressionada != '*') && (tecla_pressionada)) { //Escreve na Terceira linha do display
          lcd.setCursor(i, 2);
          lcd.print(tecla_pressionada);
          subpedido = subpedido + tecla_pressionada; //Concatena Strings
          i++;
        }

        //Se String Subpedido for maior que 0, escrever Apagar
        if ((subpedido.length() > 0)) {
          lcd.setCursor(0, 3);
          lcd.print("* Apagar");
        }

        //Se String Subpedido for maior que 0, escrever Apagar
        if ((subpedido.length() > 0)) {
          lcd.setCursor(9, 3);
          lcd.print("# Confirmar");
        }

        //Se * pressionado e String Subpedido vazia, retornar ao item anterior
        if ((subpedido.length() == 0) && (tecla_pressionada == '*') && (tecla_pressionada)) {
          lcd.clear();
          pedido = pedido + "#";
          ctrldisplay = 0;
          ctrl = 4;
          ctrldisplay = 0;
          exc = 2;
        }


        if ((tecla_pressionada == '*') && (tecla_pressionada) && (subpedido.length() > 0)) { //Apaga Toda a terceira linha do display
          lcd.clear();
          ctrldisplay = 0;
          subpedido = "";
          i = 8;
        }



        if ((tecla_pressionada == '#') && (tecla_pressionada)) {
          ctrlexc = atoi(subpedido.c_str());//Conversão da String "subpedido" para int "ctrlexc".


          if ((ctrlexc == excmais) && (ctrlexc >= 1) && (ctrlexc <= 6) ||
              (ctrlexc == excmais1) && (ctrlexc >= 1) && (ctrlexc <= 6) ||
              (ctrlexc == excmais2) && (ctrlexc >= 1) && (ctrlexc <= 6) ||
              (ctrlexc == excmais3) && (ctrlexc >= 1) && (ctrlexc <= 6)) {
            lcd.clear();//Limpa LCD

            //Escreve que Opção é Invalida
            lcd.setCursor(8, 1);
            lcd.print("ITEM");
            lcd.setCursor(4, 2);
            lcd.print("JA EXCLUIDO");
            delay (3000);//3 segundos de exibição

            lcd.clear();//Limpa LCD

            subpedido = "";
            ctrldisplay = 0;
          }


          if ((ctrlexc >= 1) && (ctrlexc <= 6) && (ctrlexc != excmais) && (ctrlexc != excmais1) && (ctrlexc != excmais2) && (ctrlexc != excmais3)) { //Inclussão de um Zero nos pedidos de 1 a 9
            if (j == 0) {
              excmais1 = atoi(subpedido.c_str());//Conversão da String "subpedido" para int "excmais1".
            }
            if (j == 1) {
              excmais2 = atoi(subpedido.c_str());//Conversão da String "subpedido" para int "excmais1".
            }
            if (j == 2) {
              excmais3 = atoi(subpedido.c_str());//Conversão da String "subpedido" para int "excmais1".
            }
            subpedido = ('0' + subpedido);
            pedido = pedido + subpedido;
            lcd.clear();
            subpedido = "";
            ctrldisplay = 0;
            j++;
          }

          if (ctrlexc > 6) {
            lcd.clear();//Limpa LCD

            //Escreve que Opção é Invalida
            lcd.setCursor(7, 1);
            lcd.print("OPCAO");
            lcd.setCursor(5, 2);
            lcd.print("INVALIDA");
            delay (3000);//3 segundos de exibição

            lcd.clear();//Limpa LCD

            subpedido = "";
            ctrldisplay = 0;
          }


          if (j == 4) {
            pedido = pedido + "#";
            ctrl = 4;
            exc = 2;
            j = 0;
          }

        }//Final do if da confirmação.



      }//Final do While


    }//Final do if

    if ((ctrlpedido == 8)) {

      lcd.clear();//Limpa LCD

      lcd.setCursor(5, 0);
      lcd.print("LEMBRANDO");
      lcd.setCursor(4, 1);
      lcd.print("Item possui");
      lcd.setCursor(3, 2);
      lcd.print("8 ingredientes");
      delay (5000);//5 segundos de exibição

      lcd.clear();//Limpa LCD
      i = 8;

      while ((exc == 0)) {
        char tecla_pressionada = meuteclado.getKey();//Verifica se alguma tecla foi pressionada

        //Escreve na Tela
        lcd.setCursor(3, 0);
        lcd.print("Excluir item:");

        //Se String Subpedido estiver com qualquer valor dentro, escreverá apagar
        if ((subpedido.length() > 0)) {
          lcd.setCursor(0, 3);
          lcd.print("* Apagar");
        }

        //Escreve na Tela
        lcd.setCursor(9, 3);
        lcd.print("# Confirmar");

        //Teclas númericas digitadas, salva na String Subpedido
        if ((tecla_pressionada != '#') && (tecla_pressionada != '*') && (tecla_pressionada)) { //Escreve na Terceira linha do display
          lcd.setCursor(i, 2);
          lcd.print(tecla_pressionada);
          subpedido = subpedido + tecla_pressionada; //Concatena Strings
          i++;
        }

        //Se String Subpedido for igual 0, escrever na tela voltar
        if ((subpedido.length() == 0)) {
          lcd.setCursor(0, 3);
          lcd.print("* Voltar");
        }

        //Se * pressionado e String Subpedido vazia, retornar ao item anterior
        if ((subpedido.length() == 0) && (tecla_pressionada == '*') && (tecla_pressionada)) {
          lcd.clear();
          ctrldisplay = 0;
          ctrl = 2;
          exc = 2;
        }


        //Se * pressionado e String Subpedido com informação, apagar informações na variavel
        if ((tecla_pressionada == '*') && (tecla_pressionada) && (subpedido.length() > 0)) { //Apaga Toda a terceira linha do display
          lcd.setCursor(0, 2);
          lcd.print("                ");
          subpedido = "";
          i = 8;
        }

        //Verifica e envia a informação, ou retorna para digitar novamente.
        if ((tecla_pressionada == '#') && (tecla_pressionada)) {
          ctrlexc = atoi(subpedido.c_str());//Conversão da String "subpedido" para int "ctrlexc".

          if ((ctrlexc >= 1) && (ctrlexc <= 8)) { //Inclussão de um Zero nos pedidos de 1 a 9
            subpedido = ('0' + subpedido);
            pedido = pedido + subpedido;
            excmais = atoi(subpedido.c_str());//Conversão da String "subpedido" para int "excmais".
            lcd.clear();
            subpedido = "";
            ctrldisplay = 0;
            exc = 1;
          }

          if ((ctrlexc > 8)) {
            lcd.clear();//Limpa LCD

            //Escreve que Opção é Invalida
            lcd.setCursor(3, 0);
            lcd.print("Opcao Invalida");
            lcd.setCursor(7, 1);
            lcd.print("Digite");
            lcd.setCursor(8, 2);
            lcd.print("1 a 8");
            delay (3000);//3 segundos de exibição

            lcd.clear();//Limpa LCD

            subpedido = "";
          }
        }


      }//Final do While excluir

      //Processo de exclusão de mais itens
      while (exc == 1) {
        char tecla_pressionada = meuteclado.getKey();//Verifica se alguma tecla foi pressionada

        if (ctrldisplay == 0) {
          lcd.setCursor(4, 0);
          lcd.print("Excluir Mais?");
          lcd.setCursor(7, 1);
          lcd.print("Digite");
          lcd.setCursor(0, 3);
          lcd.print("* Nao");
          i = 8;
          ctrldisplay++;
        }



        //Teclas númericas digitadas, salva na String Subpedido
        if ((tecla_pressionada != '#') && (tecla_pressionada != '*') && (tecla_pressionada)) { //Escreve na Terceira linha do display
          lcd.setCursor(i, 2);
          lcd.print(tecla_pressionada);
          subpedido = subpedido + tecla_pressionada; //Concatena Strings
          i++;
        }

        //Se String Subpedido for maior que 0, escrever Apagar
        if ((subpedido.length() > 0)) {
          lcd.setCursor(0, 3);
          lcd.print("* Apagar");
        }

        //Se String Subpedido for maior que 0, escrever Apagar
        if ((subpedido.length() > 0)) {
          lcd.setCursor(9, 3);
          lcd.print("# Confirmar");
        }

        //Se * pressionado e String Subpedido vazia, avança ao item posterior
        if ((subpedido.length() == 0) && (tecla_pressionada == '*') && (tecla_pressionada)) {
          lcd.clear();
          pedido = pedido + "#";
          ctrl = 4;
          ctrldisplay = 0;
          exc = 2;
        }


        if ((tecla_pressionada == '*') && (tecla_pressionada) && (subpedido.length() > 0)) { //Apaga Toda a terceira linha do display
          lcd.clear();
          ctrldisplay = 0;
          subpedido = "";
          i = 8;
        }



        if ((tecla_pressionada == '#') && (tecla_pressionada)) {
          ctrlexc = atoi(subpedido.c_str());//Conversão da String "subpedido" para int "ctrlexc".


          if ((ctrlexc == excmais) && (ctrlexc >= 1) && (ctrlexc <= 8) ||
              (ctrlexc == excmais1) && (ctrlexc >= 1) && (ctrlexc <= 8) ||
              (ctrlexc == excmais2) && (ctrlexc >= 1) && (ctrlexc <= 8) ||
              (ctrlexc == excmais3) && (ctrlexc >= 1) && (ctrlexc <= 8) ||
              (ctrlexc == excmais4) && (ctrlexc >= 1) && (ctrlexc <= 8) ||
              (ctrlexc == excmais5) && (ctrlexc >= 1) && (ctrlexc <= 8)) {
            lcd.clear();//Limpa LCD

            //Escreve que Opção é Invalida
            lcd.setCursor(8, 1);
            lcd.print("ITEM");
            lcd.setCursor(4, 2);
            lcd.print("JA EXCLUIDO");
            delay (3000);//3 segundos de exibição

            lcd.clear();//Limpa LCD

            subpedido = "";
            ctrldisplay = 0;
          }


          if ((ctrlexc >= 1) && (ctrlexc <= 8) && (ctrlexc != excmais) && (ctrlexc != excmais1) && (ctrlexc != excmais2) && (ctrlexc != excmais3) &&
              (ctrlexc != excmais4) && (ctrlexc != excmais5)) { //Inclussão de um Zero nos pedidos de 1 a 9
            if (j == 0) {
              excmais1 = atoi(subpedido.c_str());//Conversão da String "subpedido" para int "excmais1".
            }
            if (j == 1) {
              excmais2 = atoi(subpedido.c_str());//Conversão da String "subpedido" para int "excmais1".
            }
            if (j == 2) {
              excmais3 = atoi(subpedido.c_str());//Conversão da String "subpedido" para int "excmais1".
            }
            if (j == 3) {
              excmais4 = atoi(subpedido.c_str());//Conversão da String "subpedido" para int "excmais1".
            }
            if (j == 4) {
              excmais5 = atoi(subpedido.c_str());//Conversão da String "subpedido" para int "excmais1".
            }
            subpedido = ('0' + subpedido);
            pedido = pedido + subpedido;
            lcd.clear();
            subpedido = "";
            ctrldisplay = 0;
            j++;
          }

          if (ctrlexc > 8) {
            lcd.clear();//Limpa LCD

            //Escreve que Opção é Invalida
            lcd.setCursor(7, 1);
            lcd.print("OPCAO");
            lcd.setCursor(5, 2);
            lcd.print("INVALIDA");
            delay (3000);//3 segundos de exibição

            lcd.clear();//Limpa LCD

            subpedido = "";
            ctrldisplay = 0;
          }


          if (j == 6) {
            pedido = pedido + "#";
            ctrl = 4;
            ctrldisplay = 0;
            exc = 2;
            j = 0;
          }

        }//Final do if da confirmação.



      }//Final do While


    }//Final do if

  }

  while ((ctrl == 4)) { //While de Adicionar Ingrediente
    char tecla_pressionada = meuteclado.getKey();//Verifica se alguma tecla foi pressionada
    subpedido = ""; //Zera a Variavel dos Sub Pedidos

    if (ctrldisplay == 0) {
      lcd.clear();//Limpa LCD
      lcd.setCursor(0, 0);
      lcd.print("Incluir Ingrediente?");
      lcd.setCursor(6, 1);
      lcd.print("# -> Sim");
      lcd.setCursor(6, 2);
      lcd.print("* -> Nao");
      ctrldisplay++;
    }


    if ((tecla_pressionada == '*') && (tecla_pressionada)) {
      lcd.clear();
      pedido = (pedido + "00#");
      adc = 0;
      ctrldisplay = 0;
      ctrl = 6;
    }

    if ((tecla_pressionada == '#') && (tecla_pressionada)) {
      lcd.clear();
      ctrldisplay = 0;
      adc = 0;
      ctrl = 5;
    }

  }



  while ((ctrl == 5)) { //While de Adicionar Ingrediente
    char tecla_pressionada = meuteclado.getKey();//Verifica se alguma tecla foi pressionada
    subpedido = ""; //Zera a Variavel dos Sub Pedidos

    while (adc == 0) {
      char tecla_pressionada = meuteclado.getKey();//Verifica se alguma tecla foi pressionada


      //Escreve na Tela
      lcd.setCursor(3, 0);
      lcd.print("Adicionar item");

      //Se String Subpedido estiver com qualquer valor dentro, escreverá apagar
      if ((subpedido.length() > 0)) {
        lcd.setCursor(0, 3);
        lcd.print("* Apagar");
      }

      //Escreve na Tela
      lcd.setCursor(9, 3);
      lcd.print("# Confirmar");

      //Teclas númericas digitadas, salva na String Subpedido
      if ((tecla_pressionada != '#') && (tecla_pressionada != '*') && (tecla_pressionada)) { //Escreve na Terceira linha do display
        lcd.setCursor(i, 2);
        lcd.print(tecla_pressionada);
        subpedido = subpedido + tecla_pressionada; //Concatena Strings
        i++;
      }

      //Se String Subpedido for igual 0, escrever na tela voltar
      if ((subpedido.length() == 0)) {
        lcd.setCursor(0, 3);
        lcd.print("* Voltar");
      }

      //Se * pressionado e String Subpedido vazia, retornar ao item anterior
      if ((subpedido.length() == 0) && (tecla_pressionada == '*') && (tecla_pressionada)) {
        lcd.clear();
        ctrldisplay = 0;
        ctrl = 4;
        adc = 3;
      }


      //Se * pressionado e String Subpedido com informação, apagar informações na variavel
      if ((tecla_pressionada == '*') && (tecla_pressionada) && (subpedido.length() > 0)) { //Apaga Toda a terceira linha do display
        lcd.setCursor(0, 2);
        lcd.print("                ");
        subpedido = "";
        i = 8;
      }

      //Verifica e envia a informação, ou retorna para digitar novamente.
      if ((tecla_pressionada == '#') && (tecla_pressionada)) {
        ctrladc = atoi(subpedido.c_str());//Conversão da String "subpedido" para int "ctrlexc".

        if ((ctrladc >= 1) && (ctrladc <= 8)) { //Inclussão de um Zero nos pedidos de 1 a 9
          subpedido = ('0' + subpedido);
          pedido = pedido + subpedido;
          adcmais = atoi(subpedido.c_str());//Conversão da String "subpedido" para int "adcmais".
          subpedido = "";
          lcd.clear();
          ctrldisplay = 0;
          adc = 1;
        }

        if ((ctrladc > 8)) {
          lcd.clear();//Limpa LCD

          //Escreve que Opção é Invalida
          lcd.setCursor(3, 0);
          lcd.print("Opcao Invalida");
          lcd.setCursor(7, 1);
          lcd.print("Digite");
          lcd.setCursor(5, 2);
          lcd.print("1 a 8");
          delay (3000);//3 segundos de exibição

          lcd.clear();//Limpa LCD

          subpedido = "";
        }
      }

    }


    //Processo de exclusão de mais itens
    while (adc == 1) {
      char tecla_pressionada = meuteclado.getKey();//Verifica se alguma tecla foi pressionada


      if (ctrldisplay == 0) {
        lcd.setCursor(2, 0);
        lcd.print("Adicionar Mais?");
        lcd.setCursor(7, 1);
        lcd.print("Digite");
        lcd.setCursor(0, 3);
        lcd.print("* Nao");
        i = 8;
        ctrldisplay++;
      }



      //Teclas númericas digitadas, salva na String Subpedido
      if ((tecla_pressionada != '#') && (tecla_pressionada != '*') && (tecla_pressionada)) { //Escreve na Terceira linha do display
        lcd.setCursor(i, 2);
        lcd.print(tecla_pressionada);
        subpedido = subpedido + tecla_pressionada; //Concatena Strings
        i++;
      }

      //Se String Subpedido for maior que 0, escrever Apagar
      if ((subpedido.length() > 0)) {
        lcd.setCursor(0, 3);
        lcd.print("* Apagar");
      }

      //Se String Subpedido for maior que 0, escrever Apagar
      if ((subpedido.length() > 0)) {
        lcd.setCursor(9, 3);
        lcd.print("# Confirmar");
      }

      //Se * pressionado e String Subpedido vazia, avança ao item posterior
      if ((subpedido.length() == 0) && (tecla_pressionada == '*') && (tecla_pressionada)) {
        lcd.clear();
        pedido = pedido + "#";
        ctrl = 6;
        ctrldisplay = 0;
        adc = 2;
      }


      if ((tecla_pressionada == '*') && (tecla_pressionada) && (subpedido.length() > 0)) { //Apaga Toda a terceira linha do display
        lcd.clear();
        ctrldisplay = 0;
        subpedido = "";
        i = 8;
      }



      if ((tecla_pressionada == '#') && (tecla_pressionada)) {
        ctrladc = atoi(subpedido.c_str());//Conversão da String "subpedido" para int "ctrladc".


        if ((ctrladc == adcmais) && (ctrladc >= 1) && (ctrladc <= 8) ||
            (ctrladc == adcmais1) && (ctrladc >= 1) && (ctrladc <= 8) ||
            (ctrladc == adcmais2) && (ctrladc >= 1) && (ctrladc <= 8) ||
            (ctrladc == adcmais3) && (ctrladc >= 1) && (ctrladc <= 8) ||
            (ctrladc == adcmais4) && (ctrladc >= 1) && (ctrladc <= 8) ||
            (ctrladc == adcmais5) && (ctrladc >= 1) && (ctrladc <= 8)) {
          lcd.clear();//Limpa LCD

          //Escreve que Opção é Invalida
          lcd.setCursor(8, 1);
          lcd.print("ITEM");
          lcd.setCursor(3, 2);
          lcd.print("JA ADICIONADO");
          delay (3000);//3 segundos de exibição

          lcd.clear();//Limpa LCD

          subpedido = "";
          ctrldisplay = 0;
        }


        if ((ctrladc >= 1) && (ctrladc <= 8) && (ctrladc != adcmais) && (ctrladc != adcmais1) && (ctrladc != adcmais2) && (ctrladc != adcmais3) &&
            (ctrladc != adcmais4) && (ctrladc != adcmais5)) { //Inclussão de um Zero nos pedidos de 1 a 9
          if (j == 0) {
            adcmais1 = atoi(subpedido.c_str());//Conversão da String "subpedido" para int "excmais1".
          }
          if (j == 1) {
            adcmais2 = atoi(subpedido.c_str());//Conversão da String "subpedido" para int "excmais1".
          }
          if (j == 2) {
            adcmais3 = atoi(subpedido.c_str());//Conversão da String "subpedido" para int "excmais1".
          }
          if (j == 3) {
            adcmais4 = atoi(subpedido.c_str());//Conversão da String "subpedido" para int "excmais1".
          }
          if (j == 4) {
            adcmais5 = atoi(subpedido.c_str());//Conversão da String "subpedido" para int "excmais1".
          }
          subpedido = ('0' + subpedido);
          pedido = pedido + subpedido;
          lcd.clear();
          subpedido = "";
          ctrldisplay = 0;
          j++;
        }

        if (ctrladc > 8) {
          lcd.clear();//Limpa LCD

          //Escreve que Opção é Invalida
          lcd.setCursor(7, 1);
          lcd.print("OPCAO");
          lcd.setCursor(5, 2);
          lcd.print("INVALIDA");
          delay (3000);//3 segundos de exibição

          lcd.clear();//Limpa LCD

          subpedido = "";
          ctrldisplay = 0;
        }


        if (j == 6) {
          pedido = pedido + "#";
          ctrl = 6;
          ctrldisplay = 0;
          adc = 2;
          j = 0;
        }

      }//Final do if da confirmação.



    }//Final do While

  }//Final do While de Controle 5

  while ((ctrl == 6)) {
    char tecla_pressionada = meuteclado.getKey();//Verifica se alguma tecla foi pressionada
    subpedido = ""; //Zera a Variavel dos Sub Pedidos

    if (ctrldisplay == 0) {
      pedido = ("01#" + pedido);
      pedido = (pedido + "#");

      /*
                  lcd.clear();
                  lcd.setCursor(0,0);
                  lcd.print("Pedido");
                  lcd.setCursor(0,1);
                  lcd.print(pedido);
                  delay(5000);
      */
      Serial.println (pedido);
      pedido.getBytes(pacote, pedido.length);
      enviarPacoteAoServidorExterno(pacote, sizeof(pacote));

      lcd.clear();//Limpa LCD
      lcd.setCursor(3, 0);
      lcd.print("Pedido Enviado");
      lcd.setCursor(4, 1);
      lcd.print("com Sucesso");
      lcd.setCursor(0, 2);
      lcd.print("* > Finalizar Compra");
      lcd.setCursor(2, 3);
      lcd.print("# > Novo Pedido");
      ctrldisplay++;
    }

    if ((tecla_pressionada == '*') && (tecla_pressionada)) {
      lcd.clear();//Limpa LCD
      lcd.setCursor(0, 0);
      lcd.print("FAVOR SE DIRECIONAR");
      lcd.setCursor(6, 1);
      lcd.print("AO CAIXA");
      lcd.setCursor(2, 2);
      lcd.print("VOLTE SEMPRE!!!");
      delay(10000);
      ctrl = 7;
    }

    if ((tecla_pressionada == '#') && (tecla_pressionada)) {
      lcd.clear();
      pedido = "";
      exc = 0;
      ctrldisplay = 0;
      ctrl = 0;
    }

  }

  while (ctrl == 7) {
    lcd.clear();
    delay (10000);
    asm volatile ("  jmp 0");
  }

}//Final de Programa

void conectarClienteAoServidorExterno() {
  _cliente.stop();
  Serial.println("Desconectou do Servidor!");

  if (_cliente.connect(_ipServidor, _porta1)) {
    Serial.println("Conectado ao Servidor!");
  }
}

void enviarPacoteAoServidorExterno(byte pacoteEnvio[], int tamanho) {
  _cliente.write(pacoteEnvio, tamanho);
}
