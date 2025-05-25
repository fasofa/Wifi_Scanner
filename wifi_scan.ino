#include <WiFi.h>              // รวมไลบรารี WiFi เพื่อใช้ในการเชื่อมต่อกับเครือข่าย WiFi
#include <Wire.h>              // รวมไลบรารี Wire สำหรับสื่อสารผ่านโปรโตคอล I2C
#include <Adafruit_GFX.h>      // รวมไลบรารี Adafruit_GFX สำหรับวาดกราฟิกพื้นฐานบนจอ
#include <Adafruit_SSD1306.h>  // รวมไลบรารี Adafruit_SSD1306 สำหรับควบคุมจอ OLED รุ่น SSD1306

// สร้างอินสแตนซ์ของอ็อบเจ็กต์ OLED โดยไม่ใช้พินรีเซ็ต (ระบุ -1)
Adafruit_SSD1306 OLED(-1);

// ประกาศตัวแปรเก็บดัชนีของเครือข่าย WiFi ที่จะแสดงในขณะนี้
int currentNetwork = 0;
// ประกาศตัวแปรเก็บจำนวนเครือข่าย WiFi ที่พบจากการสแกน
int numNetworks = 0;

// -----------------------
// ฟังก์ชัน setup() จะถูกรันเพียงครั้งเดียวตอนเริ่มระบบ
// -----------------------
void setup() {
  Serial.begin(115200);  // เปิดการสื่อสารผ่าน Serial monitor ด้วยความเร็ว 115200 bps สำหรับการดีบัก

  // เริ่มต้นการทำงานของจอ OLED ด้วยแหล่งจ่ายไฟแบบ VCC ที่ถูกสวิตช์ (Switch Cap VCC) และที่อยู่ I2C 0x3C
  OLED.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  OLED.clearDisplay();               // ล้างหน้าจอ OLED เพื่อเตรียมการแสดงผลใหม่
  OLED.setTextColor(SSD1306_WHITE);  // ตั้งค่าสีข้อความเป็นสีขาว (เหมาะกับพื้นหลังสีดำ)
  OLED.setTextSize(1);               // กำหนดขนาดของข้อความให้เป็น 1 (ขนาดมาตรฐาน)
  OLED.setCursor(0, 0);              // กำหนดตำแหน่งเริ่มต้นที่มุมบนซ้ายของหน้าจอ
  OLED.println("Scan WiFi...");      // แสดงข้อความ "Scan WiFi..." บอกให้ทราบว่ากำลังสแกนเครือข่าย
  OLED.display();                    // ส่งข้อมูลไปอัปเดตบนจอ OLED

  WiFi.mode(WIFI_STA);  // ตั้งค่าโหมดการทำงานของ WiFi เป็น Station (STA) เพื่อเชื่อมต่อกับเราเตอร์
  WiFi.disconnect();    // ตัดการเชื่อมต่อ WiFi ที่อาจเกิดขึ้นอยู่ก่อนหน้าเพื่อให้เริ่มการสแกนใหม่ได้ชัดเจน
  delay(1000);          // หน่วงเวลา 1 วินาที เพื่อให้มั่นใจว่าทุกอย่างรีเซ็ตเรียบร้อยก่อนเริ่มงานต่อไป
}

// -----------------------
// ฟังก์ชัน loop() จะทำงานซ้ำเรื่อยๆ หลังจาก setup()
// -----------------------
void loop() {
  // ถ้า currentNetwork เท่ากับ 0 หมายความว่าเป็นการเริ่มต้นรอบการสแกนใหม่
  if (currentNetwork == 0) {
    OLED.clearDisplay();           // ล้างจอ OLED เพื่อเตรียมแสดงข้อความข้อมูลใหม่
    OLED.setCursor(0, 0);          // ตั้งตำแหน่ง cursor ที่มุมบนซ้าย
    OLED.println("Scan WiFi...");  // แสดงข้อความ "Scan WiFi..." ระหว่างการสแกนหาเครือข่าย
    OLED.display();                // อัปเดตข้อมูลบนจอ OLED

    // สแกนหาเครือข่าย WiFi ใกล้เคียงและเก็บจำนวนเครือข่ายที่พบลงในตัวแปร numNetworks
    numNetworks = WiFi.scanNetworks();

    // ตรวจสอบว่าพบเครือข่าย WiFi หรือไม่
    if (numNetworks == 0) {
      OLED.clearDisplay();             // หากไม่พบ ให้ล้างหน้าจอก่อนแสดงข้อความแจ้งเตือน
      OLED.setCursor(0, 0);            // ตั้งตำแหน่ง cursor ใหม่ที่มุมบนซ้าย
      OLED.println("Not Found WiFi");  // แสดงข้อความ "Not Found WiFi" แจ้งว่าหาเครือข่ายไม่พบ
      OLED.display();                  // อัปเดตจอ OLED ให้แสดงข้อความ
      delay(5000);                     // หน่วงเวลา 5 วินาทีเพื่อให้ผู้อ่านข้อความก่อนเริ่มรอบใหม่
      return;                          // จบการทำงานของ loop() ครั้งนี้และกลับไปเริ่มใหม่ในรอบถัดไป
    }
  }

  // เมื่อพบเครือข่าย WiFi หน้าจอจะถูกอัปเดตเพื่อแสดงข้อมูลของเครือข่ายปัจจุบัน
  OLED.clearDisplay();   // ล้างหน้าจอ OLED เพื่อแสดงข้อมูลใหม่
  OLED.setCursor(0, 0);  // กำหนดตำแหน่ง cursor ที่มุมบนซ้าย
  // แสดงหมายเลขของเครือข่ายที่กำลังแสดง (currentNetwork + 1) จากจำนวนเครือข่ายทั้งหมด (numNetworks)
  OLED.printf("WiFi %d/%d\n", currentNetwork + 1, numNetworks);
  // แสดงชื่อเครือข่าย (SSID) ของเครือข่ายที่ตรวจจับได้ ณ ดัชนี currentNetwork
  OLED.printf("SSID: %s\n", WiFi.SSID(currentNetwork).c_str());
  // แสดงความแรงสัญญาณ (RSSI) ที่ได้รับ สำหรับเครือข่ายปัจจุบัน (หน่วย dBm)
  OLED.printf("RSSI: %d dBm\n", WiFi.RSSI(currentNetwork));
  // ตรวจสอบและแสดงประเภทของการเข้ารหัสว่าเป็นแบบ "Open" หรือ "Secured"
  OLED.printf("Enc: %s\n", WiFi.encryptionType(currentNetwork) == WIFI_AUTH_OPEN ? "Open" : "Secured");
  OLED.display();  // อัปเดตข้อมูลบนจอ OLED ให้แสดงข้อมูลเครือข่ายล่าสุด

  currentNetwork++;                     // เพิ่มค่าดัชนี currentNetwork เพื่อเปลี่ยนไปเครือข่ายถัดไป
  if (currentNetwork >= numNetworks) {  // ถ้าดัชนี currentNetwork เกินจำนวนเครือข่ายที่พบ
    currentNetwork = 0;                 // รีเซ็ตค่าดัชนีกลับไปเริ่มจากเครือข่ายแรก
    delay(1000);                        // หน่วงเวลา 1 วินาทีเพื่อให้ผู้ใช้มีช่วงดูข้อมูลก่อนเริ่มการสแกนใหม่
  } else {
    delay(3500);  // หากยังมีเครือข่ายเหลือให้หน่วงเวลา 3.5 วินาทีเพื่อแสดงข้อมูลของเครือข่ายถัดไป
  }
}