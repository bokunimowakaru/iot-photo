# IoT Photo Frames ESP32 (M5Stack, SSD1331, ILI9341)

Wi-Fiカメラからの画像を表示することが出来るフォトフレーム用ソフトです。  

ブログ記事：TTGO T-Camera+M5StackでWi-Fiカメラ
* https://bokunimo.net/blog/esp/420/

有機ELディスプレイSSD1331用と、液晶ディスプレイILI9341の2種類を用意しました。  
M5 Stackには、iot-photo_m5を使用ください。

* 有機EL用(SSD1331)：[iot-photo_oled](https://github.com/bokunimowakaru/iot-photo/tree/master/iot-photo_oled)
* M5Stack用(ILI9341)：[iot-photo_m5](https://github.com/bokunimowakaru/iot-photo/tree/master/iot-photo_m5)
コンパイル済みファームウェアあり

## 対応カメラ

TTGO T-Camera に対応しています。詳細ならびにソフトウェアについては、下記をご覧ください。

* https://github.com/bokunimowakaru/iot-camera

## コンパイル方法

Arduino IDEに、 [arduino-esp32](https://github.com/espressif/arduino-esp32/releases) を組み込んで、コンパイルを行います。arduino-esp32のバージョンは 1.0.2 を使用しました。  
M5Stack用については、コンパイル済みソフトウェア、 [iot-photo_m5/target](https://github.com/bokunimowakaru/iot-photo/tree/master/iot-photo_m5/target) を用意しました。詳細については [iot-photo_m5/target/README.md](https://github.com/bokunimowakaru/iot-photo/blob/master/iot-photo_m5/README.md) をご覧ください。  

コンパイル時に必要なライブラリ：  
* arduino-esp32：https://github.com/espressif/arduino-esp32/releases

Arduino IDEの[ツール]メニュー⇒[ボード]から、「ESP32 Dev Module」を選択してください。  
SPIFFSを使用しているので、Flash Sizeを 4MB に、Partition Schemeを「Default 4MB with spiffs」設定してください。  

* Arduino IDE：[ツール]⇒[ボード]⇒[ESP32 Dev Module]
* Flash Size : 4MB
* Partition Scheme : Default 4MB with spiffs (1.2MB APP 1.5MB SPIFFS)

## 使用方法

はじめにSTAモード（SSID=1234ABCD、パスワード=password）で動作を開始しようとします。  
無線LANアクセスポイントが見つからなかったら、APモード（同じSSIDとパスワード）で動作します。  

## SSIDの変更方法

iot-photo.ino の下記の部分を変更してください。  

	#define SSID "1234ABCD" 						// 無線LANアクセスポイントのSSID
	#define PASS "password" 						// パスワード
	#define SSID_AP "1234ABCD"						// 本機の無線アクセスポイントのSSID
	#define PASS_AP "password"						// パスワード


# ご注意・外部から不正に侵入される可能性があります

本ソフトウェアの配布にあたり、SSIDとパスワードを公開しています。運用時はSSIDやパスワードの変更が必要です。  

by <https://bokunimo.net>


--------------------------------------------------------------------------------
以下は M5Stack用のディスプレイドライバ部の説明です。  
--------------------------------------------------------------------------------

## Adafruit ILI9341 Library FORKED by bokunimo.net

M5 Stack上で動作するArduino用ライブラリを利用しています。
Adafruit GFX API を使って液晶を制御することが出来ます。
以下のソースを組み合わせて作成しました。

- Adafruit-ILI9341
	https://travis-ci.org/adafruit/Adafruit_ILI9341
	Limor Fried/Ladyada

- Adafruit-GFX-Library
	https://github.com/ehubin/Adafruit-GFX-Library
	Limor Fried/Ladyada

- JPEGDecoder
	https://github.com/MakotoKurauchi/JPEGDecoder
	Makoto Kurauchi

- 上記の統合作業ならびに改変部
	https://bokunimo.net/
	Wataru KUNINO
