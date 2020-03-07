# M5Stack用 IoT Photo Frames ESP32

Wi-Fiカメラからの画像をM5Stackに表示することが出来るフォトフレーム用ソフトです。  

一つ上のフォルダ内の[README.md](https://github.com/bokunimowakaru/iot-photo/blob/master/README.md)の注意事項をよく読んでからご利用ください。

# コンパイル済みファームウェアの書き込み方法

フォルダ[target](https://github.com/bokunimowakaru/iot-photo/tree/master/iot-photo_m5/target)内に、コンパイル済みファームウェアを保存しました。  
下記の IoT Sensor Core の情報を参考にインストールしてください（iot-sensor-coreの部分をiot-photo_m5に読み替えてください。）  

* <https://github.com/bokunimowakaru/sens/blob/master/README.pdf>

# iot-photo_m5.ino のコンパイル設定

Arduino IDEの[ツール]メニュー⇒[ボード]から、「ESP32 Dev Module」を選択してください。  
SPIFFSを使用しているので、Flash Sizeを 4MB に、Partition Schemeを「Default 4MB with spiffs」設定してください。  

* Arduino IDE：[ツール]⇒[ボード]⇒[ESP32 Dev Module]
* CPU Frequency: 160MHz(WiFi/BT)
* Flash Frequency: 80MHz
* Flash Mode: QIO
* Flash Size : 4MB
* Partition Scheme : Default 4MB with spiffs (1.2MB APP 1.5MB SPIFFS)

# ライセンス(全般)

* ライセンスについては各ソースリストならびに各フォルダ内のファイルに記載の通りです。  
* 使用・変更・配布は可能ですが、権利表示を残してください。  
* また、提供情報や配布ソフトによって生じたいかなる被害についても，一切，補償いたしません。  
* ライセンスが明記されていないファイルについても、同様です。  

	Copyright (c) 2016-2020 Wataru KUNINO <https://bokunimo.net/>

また、以下のライブラリが含まれますので、それぞれの権利者のライセンスに従ってください。  

## arduino-esp32

- arduino-esp32
	https://github.com/espressif/arduino-esp32
	espressif/arduino-esp32 is licensed under the GNU Lesser General Public License v2.1

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

- picojpeg
	https://code.google.com/p/picojpeg/
	Rich Geldreich

- 上記の統合作業ならびに改変部
	https://bokunimo.net/
	Wataru KUNINO
