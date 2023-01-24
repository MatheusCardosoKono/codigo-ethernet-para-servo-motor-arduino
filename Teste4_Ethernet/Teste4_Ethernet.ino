#include <SPI.h>
#include <Ethernet.h>
#include <Servo.h>

byte mac[] = {
  0xDe, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};

IPAddress ip(169, 254, 132, 174);     //Define o endereço IP
IPAddress gateway(169,254,132,173);    //Define o gateway
IPAddress subnet(255, 255, 0, 0);  //Define a máscara de rede

EthernetServer server(80);
Servo microservo9g;

int bot = 0;
int botao = 0;

void setup() {
  Serial.begin(9600);
  while (!Serial){
    ;
  }
  microservo9g.attach(7);// Define que o servo esta ligado a porta digital 9
  microservo9g.write(0);// Move o servo para a posicao inicial (cancela fechada)

  Ethernet.begin(mac,ip);
  server.begin();
  Serial.print("Server is at ");
  Serial.println(Ethernet.localIP());
}

void loop() {
  EthernetClient client = server.available();
  if(client){
    Serial.println("New client");

    boolean currentLineIsBlank = true;

    String DatEth;
    

    String msg = "Porta Fechada";

    while (client.connected()){
      if(client.available()){

        char c = client.read();
        Serial.write(c);

        DatEth.concat(c);
        if(DatEth.endsWith("/doorop")) {bot = 1; msg = "Porta Aberta";}
        if(DatEth.endsWith("/doorcl")) {bot = 0; msg = "Porta Fechada";}

        if(c == '\n' && currentLineIsBlank){
          /*************************Código HTML*************************/
          client.println("<!DOCTYPE html>");
          client.println("<html lang=\"pt-br\">");
          client.println("<head>");
          client.println("<title>SSL</title>");
          client.println("<meta http-equiv=\"refresh\" content=\"2\" charset=\"UTF-8\">");
          client.println("</head>");
          client.println("<body style=\"background-color: #DDD;\">");
          client.println("<br>");
          client.println("<form align=\"center\">");
          client.println("<label style=\"font-size: 18pt;\">"+msg+"</label>");
          client.println("<br>");
          client.println("<br>");
          client.println("<button type=\"submit\" formaction=\"doorop\">Abrir</button>");//Button HTML5
          client.println("<button type=\"submit\" formaction=\"doorcl\">Trancar</button>");//Button HTML5
          client.println("</form>");
          client.println("<br>");
          client.println("</body>");
          client.println("</html>");
          /*************************Código HTML*************************/

          /********LED********/
          if(bot == 1){
            microservo9g.write(180);            
          }

          if(bot == 0){
            microservo9g.write(10);
          }

          break;
        }

        if(c == '\n') {
          currentLineIsBlank = true;
        }
        else if (c != '\r') {
          currentLineIsBlank = false;
        }

      }

    }

    delay(1);

    client.stop();
    Serial.println("Client disconnected");

  }

}
