#include <FastLED.h>
#include <WiFi.h>

//WiFi設定
const char* ssid     = "XXXXX"; 
const char* password = "YYYYY";

//LED設定
#define NUM_LEDS 256
#define DATA_PIN 5
#define PT_NUM 21

int led_mode = 1;//LEDの動作モード（0:手動,1:璃奈＋ボードオート,2:全種オート）
int led_pt = 4;//選択中のLEDパターン
int led_last= -1;//最後に表示したLEDパターン


CRGB leds[NUM_LEDS];
CRGB ledColor = CRGB(255, 0, 125);//表情用に使う色
CRGB ledWhite = CRGB(0, 0, 0);
CRGB led_0 = CRGB(255, 255, 255); //0 white #ffffff rgb(255,255,255)
CRGB led_1 = CRGB(255, 165, 0); //1 orange  #FFA500 rgb(255,165,0)
CRGB led_2 = CRGB(255, 0, 0); //2 red   #ff0000 rgb(255,0,0)
CRGB led_3 = CRGB(0, 128, 0); //3 green #008000 rgb(0,128,0)
CRGB led_4 = CRGB(0, 255, 0); //4 lime  #00ff00 rgb(0,255,0)
CRGB led_5 = CRGB(0, 0, 255); //5 blue  #0000ff rgb(0,0,255)
CRGB led_6 = CRGB(255, 255, 0); //6 yellow #ffff00 rgb(255,255,0)
CRGB led_7 = CRGB(128, 128, 128); //7 gray #808080 rgb(128,128,128)
CRGB led_8 = CRGB(255, 193, 181); //(128,128,128); //8 肌色 moccasin #FFE4B5 rgb(255,228,181)
CRGB led_9 = CRGB(0, 0, 0); //9 black #000000 rgb(0,0,0)
CRGB led_A = CRGB(255, 0, 125); //A 薄いピンク
CRGB led_B = CRGB(255, 0, 255); //B fuchsia #ff00ff rgb(255,0,255)
CRGB led_C = CRGB(102, 51, 0); //CRGB(198,159,119); //C 茶色
CRGB led_D = CRGB(43, 0, 128); //D 濃い青
CRGB led_E = CRGB(50, 50, 50); //E 濃いgray
CRGB led_F = CRGB(165, 42, 42); //F brown #a52a2a rgb(165, 42, 42)
CRGB led_G = CRGB(250, 250, 210); //G lightgoldenrodyellow #fafad2 rgb(250, 250, 210)
CRGB led_H = CRGB(224, 255, 255);
CRGB led_I = CRGB(0, 255, 255);
CRGB led_J = CRGB(178, 34, 34);
CRGB led_K = CRGB(255,100,100);//(255, 160, 122);
CRGB led_L = CRGB(102, 205, 170);
CRGB led_M = CRGB(196, 158, 146);
CRGB led_N = CRGB(219, 235, 205);
CRGB led_O = CRGB(152,0,152);

//電波表示用パターン
char dataWiFi[4][NUM_LEDS+1] = {
  "0000000000000000007000000000070000770000000077000077707777077700077777777777777077777777777777777777777777777777077770000007777000700077770007000000777777770000000777777777700000077000000770000000007777000000000007777770000000000077770000000000000770000000",
  "000000000000000000700000000007000077000000007700007770777707770007777777777777707777777777777777777777777777777707777000000777700070007777000700000077777777000000077777777770000007700000077000000000RRRR00000000000RRRRRR00000000000RRRR0000000000000RR0000000",
  "00000000000000000070000000000700007700000000770000777077770777000777777777777770777777777777777777777777777777770777700000077770007000RRRR0007000000RRRRRRRR0000000RRRRRRRRRR000000RR000000RR000000000RRRR00000000000RRRRRR00000000000RRRR0000000000000RR0000000",
  "000000000000000000R0000000000R0000RR00000000RR0000RRR0RRRR0RRR000RRRRRRRRRRRRRR0RRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR0RRRR000000RRRR000R000RRRR000R000000RRRRRRRR0000000RRRRRRRRRR000000RR000000RR000000000RRRR00000000000RRRRRR00000000000RRRR0000000000000RR0000000"
  };

//ボード用パターン
char data[PT_NUM][NUM_LEDS+1] = {
  //璃奈
    "44444AAAA444444444494AAAA4449944449AAAAA0A0A9444444A0AAAAAA094444AAAA7AAAAAAAA444A7A55AAA55AAAA4AAAA500555005AA4AA50A0000A0A57AAAA9500A0000A05A44A50A0000A00509AAA950000000005444A5000A0A00057AA4AAA5000A00057AAAAA7555555577AA4AA4588766678854A445585565585544A"
    ,

    "44444AAAA444444444494AAAA4449944449AAAAA0A0A9444444A0AAAAAA094444AAAA7AAAAAAAA444A7A55AAA55AAAA4AAAA500555005AA4AA50A0000A0A57AAAA9500A000A005A44A50A0000A00509AAA950000000005444A5000A0A00057AA4AAA5000A00057AAAAA7555555577AA4AA4588766678854A445585565585544A"
    ,
    "55555AAAA55555555555AAAAA55555555555AAAA0A0AA555555A0AAAAAA0A55555A227AAAAAAAA555A7AAAAAAAA66AA55A0AA8A8A8A8AAA5AAA888888888AAAAAAA8899889988AA55AA8909889098AAA5AA8899889988A5A55A8888888888AA5A5AA88288288AAA55AABB8822888BA555A5500888800555A555000B66B00055A"
    ,
    "55555AAAA55555555555AAAAA55555555555AAAA0A0AA555555A0AAAAAA0A55555A227AAAAAAAA555A7AAAAAAAA66AA55A0AA8A8A8A8AAA5AAA888888888AAAAAAA8899889988AA55AA8909889098AAA5AA8899889988A5A55A8888888888AA5A5AA88288288AAA55AABB8822888BA555A5500888800555A555000B66B00055A"
  //以下ボード
  ,//d1:4
  "9999999999999999999999999999999999999999999999999RR9999999999RR99RR9999999999RR99RR9999999999RR99RR9999999999RR99RR9999999999RR9999999999999999999999999999999999999999999999999999R99999999R9999999R999999R999999999RRRRRR9999999999999999999999999999999999999"
  ,//d2
  "9999999999999999999999999999999999999999999999999RR9999999999RR9999RR999999RR99999999R9999R99999999RR999999RR9999RR9999999999RR9999999999999999999999999999999999R9R99999999R9R999999999999999999999R999999R999999999RRRRRR999999999999999999999999999999999999"
  ,//d3
  "9999999999999999999999999999999999999999999999999RR9999999999RR99RR9999999999RR99RR9999999999RR99RR9999999999RR99RR9999999999RR99999999999999999999999999999999999999999999999999999999RR9999999999999R99R999999999999R99R9999999999999RR99999999999999999999999"
  ,//d4
  "99999999999999999999999999999999R99999999999999R9RR9999999999RR99RR9999999999RR99RR9999999999RR99RR9999999999RR99RR9999999999RR9999999999999999999999999999999999999999999999999999R99999999R9999999R999999R999999999RRRRRR9999999999999999999999999999999999999"
  ,//d5
  "9999999999999999999999999999999999999999999999999RR9999999999RR99RR9999999999RR99RR9999999999RR99RR9999999999RR99RR9999999999RR9999999999999999999999999999999999999999999999999999999RRRR99999999999R9999R9999999999R9999R99999999999R99R9999999999999RR9999999"
  ,//d6
  "999999999999999999999999999999999999999999999999999RR9999999R999999R999999R99R9999R99R999999R999999R9999999R9999999R99999999R9999999999999999999999R99999999R999999999999999999999999999999999999999R999999R999999999RRRRRR999999999999999999999999999999999999"
  ,//d7
  "9999999999999999999999999999999999999999999999999RR9999999999RR9999RR999999RR99999999R9999R99999999RR999999RR9999RR9999999999RR9999999999999999999999999999999999R9R99999999R9R999999RRRRRR9999999999R9999R9999999999R9999R99999999999R99R9999999999999RR9999999"
  ,//d8
  "99999999999999999999999999999999999999999999999999RR99999999RR999R9RR999999R9RR99RRRR999999RRRR99R9R99999999RR999999999999999R9R9R999999999999999999999999999999999999999999999999999RRRRRR999999999R999999R999999999999999999999999999999999999999999999999999"
  ,//d9
  "999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999R99999999999999R9RRRR999999RRRR999999999999999999R9R99999999R9R9R9R9999999999R9R99999RRRRRR9999999999R9999R9999999999R9999R99999999999R99R9999999999999RR9999999"
  ,//d10
  "999999999999999999999999999999999999999999999999RR999999999999RR99RR99999999RR999999R999999R999999RR99999999RR99RR999999999999RR999999999999999999999999999999999R9R99999999R9R9R9R9999999999R9R9999R999999R999999999RRRRRR999999999999999999999999999999999999"
  ,//d11
  "999999999999999999999999999999999999999999999999RR999999999999RR99RR99999999RR999999R999999R999999RR99999999RR99RR999999999999RR999999999999999999R9R999999R9R999R9R99999999R9R999999RRRRRR9999999999R9999R9999999999R9999R99999999999R99R9999999999999RR9999999"
  ,//d12
  "999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999R99999999999999R9RRRR999999RRRR999999999999999999999999999999999999999999999999999999999999999999999R999999R999999999R9999R99999999999RRRR999999999999999999999"
  ,//d13：１６
  "999999999999999999999999999999999999999999999999999999999999999999999999999999999RR9999999999RR99RRRR999999RRRR999RR99999999RR9999999999999999999999999999999999R9R9999999999R9R99999RRRRRR9999999999R9999R9999999999R9999R99999999999R99R9999999999999RR9999999"
  ,//以下，書き換え枠
  "XXX",//17
  "XXX",//18
  "XXX",//19
  "XXX" //20 
};

WiFiServer server(80);


void setup() {
  //LEDの初期設定
  delay( 3000 ); // power-up safety delay
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(50);

  connectWiFi();
}

void connectWiFi(){  
//WiFi接続処理
    Serial.begin(115200);
    pinMode(5, OUTPUT);      // set the LED pin mode

    delay(10);

    // We start by connecting to a WiFi network

    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    //WiFi.begin(ssid);
    WiFi.begin(ssid, password);

    int ct = 0;
    int pt = 0;
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        ct++;
        //Serial.print(".");
        Serial.print("ct:");
        Serial.println(ct);
        Serial.print("pt:");
        Serial.println(pt);

        setCOLOR(dataWiFi[pt]); 
        pt++;
        if(pt>3){pt=0;}       
        if(ct>10){
          led_pt = 9;
          showLED();
          return;
          }
    }
    //接続処理
    led_mode = 0;//手動にする
    led_pt = 0;
    showLED();
    

    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    
    server.begin();

}

int value = 0;

int checkWiFi(){
//WiFi経由のコマンド受付処理
  WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("New Client.");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
            client.print("TSUNAGARU-CONNECTED!<br>");
            client.print("led_mode = ");
            client.print(led_mode);
            client.print(" led_pt = ");
           client.print(led_pt);
           client.print("<BR>");
            
             // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        //明るさ設定
        if (currentLine.startsWith("GET /S")) {
          int d = currentLine.charAt(6)-'0';
          Serial.println(""); 
          Serial.print("S = "); 
          Serial.println(d); 
          if(d==0){d=10;}
          FastLED.setBrightness(20*d);
          led_last= -1;//パターン変更なしでも明るさの変更がされるよう調整
        }
        //ボード色設定
        else if (currentLine.startsWith("GET /C")) {
          int d = currentLine.charAt(6)-'0';
          Serial.println(""); 
          Serial.print("C = "); 
          Serial.println(d); 
          switch(d){
            case 0:
              ledColor =  CRGB(255, 0, 125); //('C0')">ピンク(0)
              break;
            case 1:
              ledColor =  CRGB(0, 255, 0); //('C1')">緑(1)  #00ff00
              break;
            case 2:
              ledColor =  CRGB(0, 0, 255); //('C2')">青(2)  #0000ff
              break;
            case 3:
              ledColor =  CRGB(255, 0, 0); //('C3')">赤(3)  #ff0000
              break;
            case 4:
              ledColor =  CRGB(0, 255, 255);//('C4')">水色(4)水色
              break;
            case 5:
              ledColor =  CRGB(255, 255, 0); //('C5')">黄色(5) yellow #ffff00
              break;
            case 6:
              ledColor =  CRGB(255, 165, 0); //('C6')">オレンジ(6) orange  #FFA500
            }         
            led_last= -1;//パターン変更なしでも明るさの変更がされるよう調整
        }
        //int led_mode = 2;//LEDの動作モード（0:手動,1:璃奈＋ボードオート,2:全種オート）
        else if (currentLine.endsWith("GET /R")) {
          led_mode = 1; 
        }
        else if (currentLine.endsWith("GET /M")) {
          led_mode = 0;                     
        }
        else if (currentLine.endsWith("GET /A")) {
          led_mode = 2;             
        }
        //int led_pt = 4;//選択中のLEDパターン
        else if (currentLine.endsWith("GET /F0")) {
          led_pt = 0;
          }
        else if (currentLine.endsWith("GET /F1")) {
          led_pt = 4;
          }
        else if (currentLine.endsWith("GET /F2")) {
          led_pt = 5;
          }
        else if (currentLine.endsWith("GET /F3")) {
          led_pt = 6;
          }
        else if (currentLine.endsWith("GET /F4")) {
          led_pt = 7;
          }
        else if (currentLine.endsWith("GET /F5")) {
          led_pt = 8;
          }
        else if (currentLine.endsWith("GET /F6")) {
          led_pt = 9;
          }
        else if (currentLine.endsWith("GET /F7")) {
          led_pt = 10;
          }
        else if (currentLine.endsWith("GET /F8")) {
          led_pt = 11;
          }
        else if (currentLine.endsWith("GET /F9")) {
          led_pt = 12;
          }
        else if (currentLine.endsWith("GET /F10")) {
          led_pt = 13;
          }
        else if (currentLine.endsWith("GET /F11")) {
          led_pt = 14;
          }
        else if (currentLine.endsWith("GET /F12")) {
          led_pt = 15;
          }
        else if (currentLine.endsWith("GET /F13")) {
          led_pt = 16;
          }
        else if (currentLine.endsWith("GET /rina")) {
          led_pt = 0;
          }  
        else if (currentLine.startsWith("GET /P1/")) {
          for(int i=0; i<256 ;i++){
            data[17][i]=currentLine.charAt(i+8);
            led_pt = 17;
          }
        }
        else if (currentLine.startsWith("GET /P2/")) {
          for(int i=0; i<256 ;i++){
            data[18][i]=currentLine.charAt(i+8);
            led_pt = 18;
          }
        }
        else if (currentLine.startsWith("GET /P3/")) {
          for(int i=0; i<256 ;i++){
            data[19][i]=currentLine.charAt(i+8);
            led_pt = 19;
          }
        }
        else if (currentLine.startsWith("GET /P4/")) {
          for(int i=0; i<256 ;i++){
            data[20][i]=currentLine.charAt(i+8);
            led_pt = 20;
          }
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
    return 1; 
  }
  return 0;
}

void loop(){
//WiFiからのコマンド受付
 checkWiFi();
 
 showLED();
}

void showLED(){
  int last_pt = PT_NUM;//2:全種オートの時

  //LEDの動作モード（0:手動,1:璃奈＋ボードオート,2:全種オート）
  if(led_mode == 0){
     setLED(led_pt); 
     return;
    }
  else if(led_mode == 1){
    last_pt = 16;
    } 
 
  //int k = 0;
  for(int k=led_pt;k<last_pt;k++){   
    setLED(k); 
    Serial.print("setLED ");
    Serial.println(k);
  
    for(int l=0;l<20000;l++){
      if(checkWiFi()==1){
        return;
        }
    }
   // delay(1000);
  }
}

void setLED(int k){
  //パターンの変更がなければ何もしない
  if(led_last==k){
    return;
    }
  led_last=k;
  
  //パターン未登録の場合はスキップして次のパターンへ
  if(data[k][0] == 'X'){
    setLED(k+1);
    }
  setCOLOR(data[k]);
}

void setCOLOR(char d[]){
  for (int i = 0 ; i < NUM_LEDS ; i++) {
    if (d[i] == 'R') {//ボードパターン用
        leds[i] = ledColor;
      }
    else if (d[i] == '0') {
        leds[i] = led_0;
      }
      else if (d[i] == '1') {
        leds[i] = led_1;
      }
      else if (d[i] == '2') {
        leds[i] = led_2;
      }
      else if (d[i] == '3') {
        leds[i] = led_3;
      }
      else if (d[i] == '4') {
        leds[i] = led_4;
      }
      else if (d[i] == '5') {
        leds[i] = led_5;
      }
      else if (d[i] == '6') {
        leds[i] = led_6;
      }
      else if (d[i] == '7') {
        leds[i] = led_7;
      }
      else if (d[i] == '8') {
        leds[i] = led_8;
      }
      else if (d[i] == '9') {
        leds[i] = led_9;
      }
      else if (d[i] == 'A') {
        leds[i] = led_A;
      }
      else if (d[i] == 'B') {
        leds[i] = led_B;
      }
      else if (d[i] == 'C') {
        leds[i] = led_C;
      }
      else if (d[i] == 'D') {
        leds[i] = led_D;
      }
      else if (d[i] == 'E') {
        leds[i] = led_E;
      }
      else if (d[i] == 'F') {
        leds[i] = led_F;
      }
      else if (d[i] == 'G') {
        leds[i] = led_G;
      }
      else if (d[i] == 'H') {
        leds[i] = led_H;
      }
      else if (d[i] == 'I') {
        leds[i] = led_I;
      }
      else if (d[i] == 'J') {
        leds[i] = led_J;
      }
      else if (d[i] == 'K') {
        leds[i] = led_K;
      }
      else if (d[i] == 'L') {
        leds[i] = led_L;
      }
      else if (d[i] == 'M') {
        leds[i] = led_M;
      }
      else if (d[i] == 'N') {
        leds[i] = led_N;
      }
      else if (d[i] == 'O') {
        leds[i] = led_O;
      }
      else {
        leds[i] = ledWhite;
      }
    }
    FastLED.show();
  }
