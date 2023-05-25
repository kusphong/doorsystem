#include<SoftwareSerial.h> 
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Firebase_ESP_Client.h>
#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>

#define BLYNK_TEMPLATE_ID "TMPLHzDMIB3I"
#define BLYNK_TEMPLATE_NAME "Quickstart TemplateCopy"
#define BLYNK_AUTH_TOKEN "MLJrBBWVlLw-CbDrdEYf1G-R6c1DZBsO"

// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial
int currentId = -1;  // Biến tạm để lưu trữ giá trị ID hiện tại
String inputString = "";  // Chuỗi để lưu trữ các kí tự nhận được từ cổng UART
//int currentId1 = -1;
//String inputString1 = "";
int pinValue;
bool isXoaIdVanTayRunning = 0;
char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "TB1_ACTVN";
char pass[] = "1234567@9";

//#define WIFI_SSID "TB1_ACTVN"
//#define WIFI_PASSWORD "1234567@9"
#define API_KEY "AIzaSyBkDYB1ukNEMYRDYCzYNzKTx6EHPmDHz_M"
#define DATABASE_URL "https://sanbui-6b560-default-rtdb.firebaseio.com/"  //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app
#define USER_EMAIL "test@123.com"
#define USER_PASSWORD "123456"

FirebaseData fbdo;
FirebaseAuth firebase_auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;

unsigned long count = 0;

int i;
// This function will be called every time Slider Widget
// in Blynk app writes values to the Virtual Pin V1

// This function will be called every time Slider Widget
// in Blynk app writes values to the Virtual Pin V1

BLYNK_WRITE(V1)
{
  pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
  // You can also use:
  // String i = param.asStr();
  // double d = param.asDouble();
  
  Serial.write(pinValue);
  
}
BLYNK_WRITE(V0)
{
  
}
void setup() {
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  //Blynk.begin(WIFI_SSID, WIFI_PASSWORD);
  WiFi.begin(ssid, pass);
  Serial.print("Connecting to Wi-Fi");
  unsigned long ms = millis();
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);
  config.api_key = API_KEY;
  firebase_auth.user.email = USER_EMAIL;
  firebase_auth.user.password = USER_PASSWORD;
  config.database_url = DATABASE_URL;
  config.token_status_callback = tokenStatusCallback;  // see addons/TokenHelper.h
#if defined(ESP8266)
  fbdo.setBSSLBufferSize(2048 /* Rx buffer size in bytes from 512 - 16384 */, 2048 /* Tx buffer size in bytes from 512 - 16384 */);
#endif
  // Limit the size of response payload to be collected in FirebaseData
  fbdo.setResponseSize(2048);
   Firebase.begin(&config, &firebase_auth);
   Firebase.reconnectWiFi(true);
   Firebase.setDoubleDigits(5);
  config.timeout.serverResponse = 10 * 1000;
}


void loop() {
  Blynk.run();
while (Serial.available() > 0) {
    char incomingByte = Serial.read();  // Đọc một kí tự từ cổng UART

    if (incomingByte == '\n') {  // Nếu đã nhận được một chuỗi hoàn chỉnh
      if (inputString.startsWith("ID:")) // Nếu chuỗi bắt đầu bằng "ID:"
      {
        String idString = inputString.substring(3);  // Lấy phần tử sau "ID:" để chuyển đổi thành số nguyên
        int idValue = idString.toInt();  // Chuyển đổi chuỗi thành số nguyên

        if (idValue > 0) {  // Nếu giá trị ID hợp lệ
          Serial.println("Received ID: ");
          Serial.print(idValue);
          // Xử lý giá trị ID ở đây
          currentId = idValue;
          String numberString = String(currentId);
          if (Firebase.ready()) {
          String path = "/data/id" + numberString;
          Serial.printf("Set int... %s\n", Firebase.RTDB.setInt(&fbdo, path, currentId) ? "ok" : fbdo.errorReason().c_str());
        }else {
            Serial.println("Firebase connection error.");
          }
        }
      }
      
            if(inputString.startsWith("IDX:")) {  // Nếu chuỗi bắt đầu bằng "ID:"
        String idString = inputString.substring(4);  // Lấy phần tử sau "ID:" để chuyển đổi thành số nguyên
        int idValue = idString.toInt();  // Chuyển đổi chuỗi thành số nguyên

        if (idValue > 0) {  // Nếu giá trị ID hợp lệ
          Serial.println("/DELETE ID: ");
          Serial.print(idValue);
          // Xử lý giá trị ID ở đây
          currentId = idValue;
          String numberString = String(currentId);
          String path = "/data/id" + numberString;
          if (Firebase.RTDB.deleteNode(&fbdo, path)) {
          Serial.println("Remove success");
          } else {
          Serial.println("Remove failed");
  }
        }
      }
      inputString = "";  // Đặt lại chuỗi để lưu trữ các kí tự nhận được cho lần nhận tiếp theo
    } else {
      inputString += incomingByte;  // Thêm kí tự nhận được vào chuỗi để xử lý sau
    }
    if(incomingByte == 2)
    {
    Serial.println("dongcua");    
    delay(500);
    Blynk.virtualWrite(V0,"Cửa đang đóng");
    }

    if(incomingByte == 3)
    {
    Serial.println("mocua");
    Blynk.virtualWrite(V0,"Cửa đang mở");
    delay(500);    
    }
  }
}
