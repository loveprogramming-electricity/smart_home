#include <WebServer.h>
#include <WiFi.h>
#include <ESP32Servo.h>
#include <BluetoothSerial.h>


BluetoothSerial blt;

// Replace with your network credentials
String ssid = "";
String password = "";

IPAddress local_ip(192,168,1,200);
IPAddress gateway(192,168,1,1);
IPAddress subnet(250,250,250,0);

// Set web server port number to 80
WebServer server(80);

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state

int gpio[8]={12,13,14,15,16,17,18,19};
String gpio_state[8]={"off","off","off","off","off","off","off","off"};

int gpio_cong[2]={2,4}; 
String gpio_cong_state[2]={"close","close"};
Servo Servo0;
Servo Servo1;

// Assign output variables to GPIO pins
const int  pk=gpio[0];
const int  pb=gpio[1];
const int  pn1=gpio[2];
const int  pn2=gpio[3];
const int  pn3=gpio[4];
const int  dsan=gpio[5];
const int  dben_hong=gpio[6];
const int  dennha_ve_sinh=gpio[7];
// Current time
void setup()
{
    Serial.begin(115200);
    // Initialize the output variables as outputs
    Servo0.attach(gpio_cong[0]);
    Servo1.attach(gpio_cong[1]);

    for (int i=0;i<8;i++){
      pinMode(gpio[i],OUTPUT);
    }
    // Set outputs to LOW
    for (int i=0;i<8;i++){
      digitalWrite(gpio[i],LOW);
    }
    blt.begin("<3--Smart home 101---<3");

    if (!WiFi.config(local_ip,gateway,subnet)){
      Serial.println("------------STA lỗi kết nối--------");
    }


    // Connect to Wi-Fi network with SSID and password
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        if (blt.available()){
          String input=blt.readStringUntil('\n');
          if (input.indexOf("NAME")!=-1){ // NAMENGHIA123
            ssid=input.substring(input.indexOf("NAME")+4,input.length());
            WiFi.disconnect();
            WiFi.begin(ssid, password);
          }
          else if (input.indexOf("MATKHAU")!=-1){ //MATKHAU123456
            password=input.substring(input.indexOf("MATKHAU")+7,input.length());
            WiFi.disconnect();
            WiFi.begin(ssid, password);
          }
          Serial.print("Tên:");Serial.println(ssid);Serial.print("Mật khẩu:");Serial.println(password);
        }
        delay(1000);
        Serial.print(".");
    }
    // Print local IP address and start web server
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    server.on("/",handle_OnConnect);
    server.on("/tatpk",ham_tatpk);
    server.on("/tatpb",ham_tatpb);
    server.on("/tatpn1",ham_tatpn1);
    server.on("/tatpn2",ham_tatpn2);
    server.on("/tatpn3",ham_tatpn3);
    server.on("/tatds",ham_tatds);
    server.on("/tatdbh",ham_tatdbh);
    server.on("/tatdvs",ham_tatdvs);
    server.on("/tattoanbo",ham_tattoanbo);

    server.on("/batpk",ham_batpk);
    server.on("/batpb",ham_batpb);
    server.on("/batpn1",ham_batpn1);
    server.on("/batpn2",ham_batpn2);
    server.on("/batpn3",ham_batpn3);
    server.on("/batds",ham_batds);
    server.on("/batdbh",ham_batdbh);
    server.on("/batdvs",ham_batdvs);
    server.on("/battoanbo",ham_battoanbo);

    server.on("/dongcong",ham_dongcong);
    server.on("/mocong",ham_mocong);

    server.onNotFound(handle_NotFound);
    server.begin();
    Serial.println("HTTP Sever started");
}

void loop(){
  server.handleClient(); // 192.168.1.200/tatpk
  // for(int i=0;i<8;i++){
  //   if(gpio_state[i]=="on"){
  //     digitalWrite(gpio[i],HIGH);
  //   }
  //   else{
  //     digitalWrite(gpio[i],LOW);
  //   }
  // }
  delay(200);
}

void handle_OnConnect(){
  server.send(200,"text/html",SendHTML());
}


void ham_tattoanbo(){
  for(int i=0;i<8;i++){
    digitalWrite(gpio[i],LOW);
    gpio_state[i]="off";
  }
  server.send(200,"text/html",SendHTML());
}
void ham_tatpk(){
  gpio_state[0]="off";
  digitalWrite(gpio[0],LOW);
  Serial.println("Tắt đèn phòng khách");
  server.send(200,"text/html",SendHTML());
}
void ham_tatpb(){
  gpio_state[1]="off";
  digitalWrite(gpio[1],LOW);
  Serial.println("Tắt đèn phòng bếp");
  server.send(200,"text/html",SendHTML());
}
void ham_tatpn1(){
  gpio_state[2]="off";
  digitalWrite(gpio[2],LOW);
  Serial.println("Tắt đèn phòng ngủ 1");
  server.send(200,"text/html",SendHTML());
}
void ham_tatpn2(){
  gpio_state[3]="off";
  digitalWrite(gpio[3],LOW);
  Serial.println("Tắt đèn phòng ngủ 2");
  server.send(200,"text/html",SendHTML());
}
void ham_tatpn3(){
  gpio_state[4]="off";
  digitalWrite(gpio[4],LOW);
  Serial.println("Tắt đèn phòng ngủ 3");
  server.send(200,"text/html",SendHTML());
}
void ham_tatds(){
  gpio_state[5]="off";
  digitalWrite(gpio[5],LOW);
  Serial.println("Tắt đèn sân");
  server.send(200,"text/html",SendHTML());
}
void ham_tatdbh(){
  gpio_state[6]="off";
  digitalWrite(gpio[6],LOW);
  Serial.println("Tắt đèn bên hông");
  server.send(200,"text/html",SendHTML());
}
void ham_tatdvs(){
  gpio_state[7]="off";
  digitalWrite(gpio[7],LOW);
  Serial.println("Tắt đèn nhà vệ sinh");
  server.send(200,"text/html",SendHTML());
}


void ham_battoanbo(){
  for(int i=0;i<8;i++){
    digitalWrite(gpio[i],HIGH);
    gpio_state[i]="on";
  }
  server.send(200,"text/html",SendHTML());
}
void ham_batpk(){
  gpio_state[0]="on";
  digitalWrite(gpio[0],HIGH);
  Serial.println("Bật đèn phòng khách");
  server.send(200,"text/html",SendHTML());
}
void ham_batpb(){
  gpio_state[1]="on";
  digitalWrite(gpio[1],HIGH);
  Serial.println("Bật đèn phòng bếp");
  server.send(200,"text/html",SendHTML());
}
void ham_batpn1(){
  gpio_state[2]="on";
  digitalWrite(gpio[2],HIGH);
  Serial.println("Bật đèn phòng ngủ 1");
  server.send(200,"text/html",SendHTML());
}
void ham_batpn2(){
  gpio_state[3]="on";
  digitalWrite(gpio[3],HIGH);
  Serial.println("Bật đèn phòng ngủ 2");
  server.send(200,"text/html",SendHTML());
}
void ham_batpn3(){
  gpio_state[4]="on";
  digitalWrite(gpio[4],HIGH);
  Serial.println("Bật đèn phòng ngủ 3");
  server.send(200,"text/html",SendHTML());
}
void ham_batds(){
  gpio_state[5]="on";
  digitalWrite(gpio[5],HIGH);
  Serial.println("Bật đèn sân");
  server.send(200,"text/html",SendHTML());
}
void ham_batdbh(){
  gpio_state[6]="on";
  digitalWrite(gpio[6],HIGH);
  Serial.println("Bật đèn bên hông");
  server.send(200,"text/html",SendHTML());
}
void ham_batdvs(){
  gpio_state[7]="on";
  digitalWrite(gpio[7],HIGH);
  Serial.println("Bật đèn nhà vệ sinh");
  server.send(200,"text/html",SendHTML()); 
}

void ham_dongcong(){
  Servo0.write(180);
  Servo1.write(0);
  gpio_cong_state[0]="close";
  gpio_cong_state[1]="close";
  Serial.println("Đóng cổng");
  server.send(200,"text/html",SendHTML());
}
void ham_mocong(){
  Servo0.write(90);
  Servo1.write(90);
  gpio_cong_state[0]="open";
  gpio_cong_state[1]="open";
  Serial.println("Mở cổng");
  server.send(200,"text/html",SendHTML());
}

void handle_NotFound(){
  server.send(404,"text/plain","Page Not found");
}
String SendHTML(){
  String ptr="<!DOCTYPE html>";
  ptr+="<head> <meta charset=\"utf-8\" />\n";
  ptr+="<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\" />\n";
  ptr+="<link rel=\"preconnect\" href=\"https://fonts.googleapis.com\">\n";
  ptr+="<link rel=\"preconnect\" href=\"https://fonts.gstatic.com\" crossorigin>\n";
  ptr+="<link href=\"https://fonts.googleapis.com/css2?family=Merriweather:ital,wght@0,300;0,400;0,700;0,900;1,300;1,400;1,700;1,900&display=swap\" rel=\"stylesheet\">\n";
  ptr+="<style> *{font-family:\"Merriweather\";margin:0;color:white;}\n";
  ptr+="h1{text-align:center;width:400px;margin:0px 50px 5px 100px;}\n";
  ptr+=".pk,.pb,.pn,.cong,.ds,.dbh,.dvs{text-align:center;padding:0 5px 0 5px;margin: 10px 10px 10px 10px;border: 1px solid yellow;width:180px;box-sizing:border-box;line-height:25px;border-radius:10px;}\n";
  ptr+=".container1,.container2{margin:0 220px 0 220px;}\n";
  ptr+="body{background:url(\"https://th.bing.com/th/id/OIP.3mPjN3twg-GaSzmWTaoDQwHaEP?rs=1&pid=ImgDetMain\");background-size:cover;background-repeat:no-repeat;}\n";
  ptr+=".button-off{background-color:red;text-decoration:none;border-radius:5px;}\n";
  ptr+=".button-on{background-color:green;text-decoration:none;border-radius:5px;}</style>\n";
  ptr+="<body><h1><label>Bảng điều khiển ngôi nhà thông minh</label></h1>\n";
  ptr+="<div class=\"container1\">\n";
  ptr+="<div class=\"pk\"><label>Phòng khách</label>";
  if (gpio_state[0]=="on"){
    ptr+="<a class=\"button-off\" href=\"tatpk\">Tắt</a></div>\n";
  }
  else{
    ptr+="<a class=\"button-on\" href=\"batpk\">Bật</a></div>\n";
  }
  ptr+="<div class=\"pb\"><label>Phòng bếp</label>";
  if (gpio_state[1]=="on"){
    ptr+="<a class=\"button-off\" href=\"tatpb\">Tắt</a></div>\n";
  }
  else{
    ptr+="<a class=\"button-on\" href=\"batpb\">Bật</a></div>\n";
  }
  ptr+="<div class=\"pn\"><div class=\"pn1\"><label>Phòng ngủ 1</label>";
  if (gpio_state[2]=="on"){
    ptr+="<a class=\"button-off\" href=\"tatpn1\">Tắt</a></div>\n";
  }
  else{
    ptr+="<a class=\"button-on\" href=\"batpn1\">Bật</a></div>\n";
  }
  ptr+="<div class=\"pn2\"><label>Phòng ngủ 2</label>";
  if (gpio_state[3]=="on"){
    ptr+="<a class=\"button-off\" href=\"tatpn2\">Tắt</a></div>\n";
  }
  else{
    ptr+="<a class=\"button-on\" href=\"batpn2\">Bật</a></div>\n";
  }
  ptr+="<div class=\"pn3\"><label>Phòng ngủ 3</label>";
  if (gpio_state[4]=="on"){
    ptr+="<a class=\"button-off\" href=\"tatpn3\">Tắt</a></div></div>\n";
  }
  else{
    ptr+="<a class=\"button-on\" href=\"batpn3\">Bật</a></div></div>\n";
  }
  ptr+="<div class=\"cong\"><label>Cổng</label>";
  if (gpio_cong_state[0]=="open" && gpio_cong_state[1]=="open"){
    ptr+="<a class=\"button-off\" href=\"dongcong\">Đóng</a></div>\n";
  }
  else{
    ptr+="<a class=\"button-on\" href=\"mocong\">Mở</a></div>\n";
  }
  ptr+="</div><div class=\"container2\"><div class=\"ds\"><label>Đèn sân</label>";
  if (gpio_state[5]=="on"){
    ptr+="<a class=\"button-off\" href=\"tatds\">Tắt</a></div>\n";
  }
  else{
    ptr+="<a class=\"button-on\" href=\"batds\">Bật</a></div>\n";
  }
  ptr+="<div class=\"dbh\"><label>Đèn bên hông</label> ";
  if (gpio_state[6]=="on"){
    ptr+="<a class=\"button-off\" href=\"tatdbh\">Tắt</a></div>\n";
  }
  else{
    ptr+="<a class=\"button-on\" href=\"batdbh\">Bật</a></div>\n";
  }
  ptr+="<div class=\"dvs\"><label>Đèn nhà vệ sinh</label>  ";
  if (gpio_state[7]=="on"){
    ptr+="<a class=\"button-off\" href=\"tatdvs\">Tắt</a></div>\n";
  }
  else{
    ptr+="<a class=\"button-on\" href=\"batdvs\">Bật</a></div>\n";
  }
  ptr+="</div>\n";
  ptr+="</body>\n";
  ptr+="</head>\n";
  return ptr;

}
