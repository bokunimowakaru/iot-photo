/*******************************************************************************
HTMLコンテンツ取得
                                          Copyright (c) 2016-2019 Wataru KUNINO
*******************************************************************************/

// #define TIMEOUT 3000                     // タイムアウト 3秒
// extern boolean SD_CARD_EN

#include <WiFi.h>                           // ESP32用WiFiライブラリ

// #define CAMERA_BUF_SIZE
// extern static uint8_t camera_buf[];
// extern static int camera_buf_len=0;

static boolean httpGet_file = true;

int httpGet(char *url,int max_size){
    File file;
    WiFiClient client;                      // Wi-Fiクライアントの定義
    int i,j,t=0,size=0;                     // 変数i,j,t=待ち時間,size=保存容量
    char c,to[33],s[257];                   // 文字変数、to=アクセス先,s=汎用
    char *cp;
    int headF=0;                            // ヘッダフラグ(0:HEAD 1:EOL 2:DATA)
    unsigned long time;                     // 時間測定用

    if(!isgraph(url[0])) return 0;          // URLが無い時は処理を行わない
    cp=strchr(url,'/');                     // URL内の区切りを検索
    if(!cp){                                // 区切りが無かった場合は
        strncpy(to,url,32);                 // 入力文字はホスト名のみ
        strncpy(s,"/index.html",32);        // 取得ファイルはindex.html
    }else if(cp - url <= 32){               // 区切り文字までが32文字以下のとき
        strncpy(to,url,cp-url);             // 区切り文字までがホスト名
        strncpy(s,cp,64);                   // 区切り文字以降はファイル名
    }
    for(i=0;i<32;i++)if(!isgraph(to[i]))to[i]='\0'; to[32]=0;
    for(i=0;i<64;i++)if(!isgraph(s[i]))s[i]='\0'; s[64]=0;
    if( max_size <= 0 ) max_size = 32767;
    Serial.println("HTTP://" + String(to) + String(s));
//  Serial.print("Max Size  : ");
//  Serial.println(max_size);
    Serial.println("Recieving... ");
    i=0;
    while( !client.connect(to,80) ){        // 外部サイトへ接続を実行する
        i++; if( i >= TIMEOUT / 1000){      // 失敗時のリトライ処理 1/10
            Serial.println("ERROR: Failed to connect");
            Serial.println("HTTP Host: " + String(to));
            Serial.println("HTTP file: " + String(s));
            
            return 0;
        }
        Serial.println("WARN: Retrying... " + String(i) );
        delay(100);                         // 100msのリトライ待ち時間
    }
    if( httpGet_file ){
        if( SD_CARD_EN ) file = SD.open(s,"w"); // 保存のためにファイルを開く
        else file = SPIFFS.open(s,"w");         // 保存のためにファイルを開く
        if(file==0){
            Serial.println("ERROR: FALIED TO OPEN. Please format SPIFFS or SD.");
            client.flush();                     // ESP32用 ERR_CONNECTION_RESET対策
            client.stop();                      // クライアントの切断
            return 0;                           // ファイルを開けれなければ戻る
        }
    }
    client.print("GET ");                   // HTTP GETコマンドを送信
    client.print(s);                        // 相手先ディレクトリを指定
    client.println(" HTTP/1.1");            // HTTPプロトコル
    client.print("Host: ");                 // ホスト名を指定
    client.print(to);                       // 相手先ホスト名
    client.println();                       // ホスト名の指定を終了
    client.println("Connection: close");    // セッションの都度切断を指定
    client.println(); 
    
    // 以下の処理はデータの受信完了まで終了しないので、その間に届いたデータを
    // 損失してしまう場合があります。
    // Wi-Fiカメラの初期版では、送信完了まで20秒くらいかかります。
    time=millis();
    i=0;j=0;
    t=0;
    while(t<TIMEOUT){
        if(client.available()){             // クライアントからのデータを確認
            t = 1000;                       // 一度、接続したときのタイムアウトは1秒
            c=client.read();                // TCPデータの読み取り
            
            if(headF==0){                   // ヘッダの処理
                s[i]=c;
                i++;
                s[i]='\0';
                if(i>255) i=255;
                if(c=='\n'){                // 行端ならフラグを変更
                    headF=1;
                    if(strncmp(s,"Content-Length:",15)==0){
                        max_size = atoi(&s[15]);
                    //  Serial.print("("+String(max_size)+")");
                    }
                    i=0;
                }
                Serial.print((char)c);
                continue;
            }else if(headF==1){             // 前回が行端の時(連続改行＝ヘッダ終了)
                if(c=='\n'){                // 今回も行端ならヘッダ終了
                    headF=2;
                    Serial.println("[END]");
                }else{
                    if(c!='\r'){
                        headF=0;
                        s[0]=c;
                        i=1;
                        s[1]='\0';
                        Serial.print((char)c);
                    }
                }
                continue;
            }else if(headF==2){
                // 複数バイトread命令を使用する
                // int WiFiClient::read(uint8_t *buf, size_t size)
                s[0]=c; size++;             // 既に取得した1バイト目を代入
                i=client.read((uint8_t *)s+1,255);  // 255バイトを取得
                // 戻り値はrecvが代入されている
                // int res = recv(fd(), buf, size, MSG_DONTWAIT);
                if(i>0){                            // 受信データがある時
                    if( httpGet_file ) file.write((const uint8_t *)s, i+1);
                    else{
                        if(camera_buf_len + i + 1 <= CAMERA_BUF_SIZE){
                            memcpy(&camera_buf[camera_buf_len], (const uint8_t *)s, i+1);
                            camera_buf_len += i+1;
                        }else{
                            memcpy(&camera_buf[camera_buf_len], (uint8_t *)s, CAMERA_BUF_SIZE - camera_buf_len);
                            camera_buf_len = CAMERA_BUF_SIZE;
                            break;
                        }
                    }
                    size += i;
                } else {
                    if( httpGet_file ) file.write(c);
                    else{
                        camera_buf[camera_buf_len]=c;
                        camera_buf_len++;
                    }
                }
                if( size > j ){
                    Serial.print('.');
                //  oled.print('.');
                    j += 512;
                }
                if(max_size > 0 && size >= max_size) break;
                continue; 
            }
        }
        if (!client.connected()) break;
        t++;
        delay(1);
    }
    if( httpGet_file ){
        if(i) file.write((const uint8_t *)s, i); 
        file.close();                           // ファイルを閉じる
    }
    client.stop();                          // クライアントの切断
    Serial.println();
    Serial.print(size);                     // 保存したファイルサイズを表示
    Serial.print("Bytes, ");
    Serial.print(millis()-time);
    Serial.println("ms, Done");
    return size;
}

int httpGetBuf(char *url,int max_size){
    int ret;
    camera_buf_len=0;
    httpGet_file=false;
    ret=httpGet(url,max_size);
    httpGet_file=true;
    return ret;
}
