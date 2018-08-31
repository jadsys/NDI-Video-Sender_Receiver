環境構築手順書　～Windows編～
=======

開発/実行環境について
-------------------
 * OS：Windows 10 Pro 64bit  

 * IDE：Visual Studio 2017 Community  

 * 使用ライブラリ ：   
Newtek NDI　バージョン3.5  
OpenCV　バージョン3.2.0  
RealSense バージョン 2.0(buildバージョン2.16.0 )  

Visual Studio 2017 Communityのインストール  
--------------------
**1、Visual Studio 2017 Communityのダウンロード**  
以下よりダウンロードを行います。  
https://visualstudio.microsoft.com/ja/thank-you-downloading-visual-studio/?sku=Community&rel=15  

**2、インストール**  
 Visual Studio Instllerを起動します。  
続行ボタンを押します。  
![image](https://github.com/jadsys/NDI-Video-Sender_Receiver/wiki/images/windows1.jpg)

エディションの選択画面が出るのでCommunityを選択します。  
![image](https://github.com/jadsys/NDI-Video-Sender_Receiver/wiki/images/windows2.jpg)

ワークロード画面が出ましたら、  
①C++によるデスクトップ開発にチェックを入れます。  
②Windows SDKのチェックを入れます。  
③インストールボタンを押します。  
![image](https://github.com/jadsys/NDI-Video-Sender_Receiver/wiki/images/windows3.jpg)

インストールが完了するまでしばらく待ちます。  

NewTek NDI SDK v3.5のインストール
--------------------
**1、NewTek NDI SDK v3.5をダウンロード**  
 以下よりダウンロードを行います。  
https://jp.newtek.com/ndi/sdk/  
ダウンロード時に名前やメールアドレスなどの送信が必要です。

**2、インストール**  
インストーラーを起動します。  
I accept the agreementにチェックを入れてNextをクリックします。  
![image](https://github.com/jadsys/NDI-Video-Sender_Receiver/wiki/images/windows4.jpg)

Installを押してしばらく待ちます。  
![image](https://github.com/jadsys/NDI-Video-Sender_Receiver/wiki/images/windows5.jpg)

`C:\Program Files\`の中に`NewTek`フォルダがればインストールが完了です。  

**3、システムの環境変数にPATHを通す**    
システムのプロパティから環境変数をクリックします。  
![image](https://github.com/jadsys/NDI-Video-Sender_Receiver/wiki/images/windows6.jpg)

システムの環境変数のPATHを選択し、編集ボタンをクリックします。  
![image](https://github.com/jadsys/NDI-Video-Sender_Receiver/wiki/images/windows7.jpg)

新規ボタンから次の一行を記述します。  
```bash
C:\Program Files\NewTek\NewTek NDI SDK\Bin\x64\
```
![image](https://github.com/jadsys/NDI-Video-Sender_Receiver/wiki/images/windows8.jpg)

OpenCV v3.2.0のインストール  
--------------------
**1、OpenCV v3.2.0をダウンロード**    
以下よりOpenCV 3.2.0をダウンロードします。  
https://opencv.org/releases.html  

**2、インストール**  
 インストーラーを起動します。  
`Extract to:`と表示されましたら、パスを以下のように設定します。
```bash  
c:\
```
![image](https://github.com/jadsys/NDI-Video-Sender_Receiver/wiki/images/windows9.jpg)


Extractボタンで展開します。  
展開が完了後`C:\`上に`opencv`というフォルダがあるので、フォルダ名を`opencv320`と変更して下さい。  
`opencv320`フォルダの直下に`build`フォルダが有るか確認して下さい。  

**3、システムの環境変数にPATHを通す**  
システムのプロパティから環境変数をクリックします。  
![image](https://github.com/jadsys/NDI-Video-Sender_Receiver/wiki/images/windows10.jpg)

システムの環境変数のPATHを選択し、編集ボタンをクリックします。    
![image](https://github.com/jadsys/NDI-Video-Sender_Receiver/wiki/images/windows11.jpg)

新規ボタンから次の一行を記述します。  
```bash
C:\opencv320\build\x64\vc14\bin\  
```
![image](https://github.com/jadsys/NDI-Video-Sender_Receiver/wiki/images/windows12.jpg)

RealSense SDK 2.0のインストール
---------------------------
**1、RealSense SDK v2.0をダウンロード**  
以下よりRealSense SDKをダウンロードします。  
https://github.com/IntelRealSense/librealsense/releases  

※buildバージョンによって実行できるプログラムが異なるため気をつけて下さい。

**2、インストール**  
 インストーラーを起動します。 I accept the agreementにチェックを入れてNextをクリックします。  
![image](https://github.com/jadsys/NDI-Video-Sender_Receiver/wiki/images/windows13.jpg)

すべてのオプションにチェックを入れ、Nextをクリックします。  
![image](https://github.com/jadsys/NDI-Video-Sender_Receiver/wiki/images/windows14.jpg)

Nextを押します。  
![image](https://github.com/jadsys/NDI-Video-Sender_Receiver/wiki/images/windows15.jpg)

Installを押してしばらく待ちます。  
![image](https://github.com/jadsys/NDI-Video-Sender_Receiver/wiki/images/windows16.jpg)

`C:\Program Files (x86)`の中に`Intel RealSense SDK 2.0`フォルダがればインストールが完了です。

**3、システムの環境変数にPATHを通す**   
システムのプロパティから環境変数をクリックします。  
![image](https://github.com/jadsys/NDI-Video-Sender_Receiver/wiki/images/windows17.jpg)

システムの環境変数のPATHを選択し、編集ボタンをクリックします。  
![image](https://github.com/jadsys/NDI-Video-Sender_Receiver/wiki/images/windows18.jpg)

新規ボタンから次の一行を記述します。
```bash
C:\Program Files (x86)\Intel RealSense SDK 2.0\bin\x64\  
```
![image](https://github.com/jadsys/NDI-Video-Sender_Receiver/wiki/images/windows19.jpg)

**4、インストールしたbuildバージョンの確認**  
アプリと機能より確認することが可能です。  
![image](https://github.com/jadsys/NDI-Video-Sender_Receiver/wiki/images/windows44.jpg)

プロジェクトの実行
---------------------------
**1、ソリューションファイルを実行します。**  
NDI-Video-SenderはRealSense SDKのbuildバージョンによって実行できるものが異なります。  
NDI-Video-Receiverはどのバージョンでも実行可能です。  
* buildバージョンが`2.15.0`以下の場合  
`NDI-Video-Sender2.15`を開き、中にある`NDI-Video-Sender.sln`を実行します。  
![image](https://github.com/jadsys/NDI-Video-Sender_Receiver/wiki/images/windows45.JPG)

* buildバージョンが`2.16.0`以上の場合  
`NDI-Video-Sender2.16`を開き、中にある`NDI-Video-Sender.sln`を実行します。  
![image](https://github.com/jadsys/NDI-Video-Sender_Receiver/wiki/images/windows46.JPG)

プロジェクトソリューションを自前で用意する場合もRealSense SDKのバージョンによって、利用するソースファイルが異なるため、必ず確認して下さい。

Visual Studio環境整備  
---------------------------
**1、Visual Studioにインクルードディレクトリのパスを通す**  
 メニューバーのプロジェクトからプロパティを開きます。  
![image](https://github.com/jadsys/NDI-Video-Sender_Receiver/wiki/images/windows20.jpg)

VC++ ディレクトリタブを選択します。  
インクルード ディレクトリを選択し＜編集...＞から編集を行います。  
![image](https://github.com/jadsys/NDI-Video-Sender_Receiver/wiki/images/windows21.jpg)

①フォルダマークをクリックします。  
②...ボタンが出現するのでクリックします。  
![image](https://github.com/jadsys/NDI-Video-Sender_Receiver/wiki/images/windows22.jpg)

NDIのインクルードディレクトリを選択します。パスは以下の通りです。
```bash  
C:\Program Files\NewTek\NewTek NDI SDK\Include
```  
![image](https://github.com/jadsys/NDI-Video-Sender_Receiver/wiki/images/windows23.jpg)

続いて再度フォルダマークを押し、...からOpenCVのインクルードディレクトリを選択します。パスは以下の通りです。  
```bash
C:\opencv320\build\include  
```
![image](https://github.com/jadsys/NDI-Video-Sender_Receiver/wiki/images/windows24.jpg)

再度フォルダマークを押し、...からRealSenseのインクルードディレクトリを選択します。パスは以下の通りです。  
```bash
C:\Program Files (x86)\Intel RealSense SDK 2.0\include  
```
![image](https://github.com/jadsys/NDI-Video-Sender_Receiver/wiki/images/windows25.jpg)

**2、ライブラリディレクトリのパスを通す**  
 インクルード ディレクトリと同じようにでライブラリ ディレクトリのパスを通します。  
NDIのライブラリディレクトリのパスは以下の通りです。  
```bash
C:\Program Files\NewTek\NewTek NDI SDK\Lib\x64  
```
![image](https://github.com/jadsys/NDI-Video-Sender_Receiver/wiki/images/windows26.jpg)

 OpenCVのライブラリディレクトリは以下の通りです。  
```bash
C:\opencv320\build\x64\vc14\lib  
```
![image](https://github.com/jadsys/NDI-Video-Sender_Receiver/wiki/images/windows27.jpg)

 RealSenseのライブラリディレクトリは以下の通りです。  
```bash
C:\Program Files (x86)\Intel RealSense SDK 2.0\lib\x64  
```
![image](https://github.com/jadsys/NDI-Video-Sender_Receiver/wiki/images/windows28.jpg)

 **3、追加の依存ファイルの追加**  
リンカーから入力を選択し、追加の依存ファイルを選択し＜編集...＞から編集を行います。  
![image](https://github.com/jadsys/NDI-Video-Sender_Receiver/wiki/images/windows29.jpg)

以下の3行を追記します。 
 ```bash
opencv_world320.lib  
Processing.NDI.Lib.x64.lib  
realsense2.lib
```  
![image](https://github.com/jadsys/NDI-Video-Sender_Receiver/wiki/images/windows30.jpg)

**4、その他の設定**  
C/C++タブから詳細設定タブを開きます。  
指定の警告を無効にするを選択し、`4996`を追加します。  
![image](https://github.com/jadsys/NDI-Video-Sender_Receiver/wiki/images/windows31.jpg)

また、プリコンパイル済みヘッダータブを選択します。  
プリコンパイル済みヘッダーを使用からプリコンパイル済みヘッダーを使用しないに変更します。  
![image](https://github.com/jadsys/NDI-Video-Sender_Receiver/wiki/images/windows32.jpg)

更に、コマンドラインタブを選択します。  
以下の追加のオプションで`/source-charset:utf-8`を入力します。  
![image](https://github.com/jadsys/NDI-Video-Sender_Receiver/wiki/images/windows33.jpg)

Ctrl＋Alt+Eキーを押し、例外設定を開きます。  
Win32 Exceptionsを選択し、Control-Cのチェックを外します。  
![image](https://github.com/jadsys/NDI-Video-Sender_Receiver/wiki/images/windows34.jpg)

Visual Studio上で実行  
---------------------------
**1、実行前準備**  
 プロジェクトのプロパティを開きます。  
デバックタブを選択し、コマンド引数を指定します。  
指定する値はカメラの番号1～6です。  
![image](https://github.com/jadsys/NDI-Video-Sender_Receiver/wiki/images/windows35.jpg)

ReleaseまたはDebugのx64に設定されていることを確認して下さい。  
![image](https://github.com/jadsys/NDI-Video-Sender_Receiver/wiki/images/windows36.jpg)

**2、実行**   
ローカル Windowsデバッガーをクリックします。  
デバッグが始まるので、しばらく待ちます。 
エラーなく送信または受信できれば無事導入は完了です。  
![image](https://github.com/jadsys/NDI-Video-Sender_Receiver/wiki/images/windows37.jpg)

以下のようにDLLが見つからない場合、環境変数のPATHの設定が間違っています。  
今一度以下のように設定されているか確認して下さい。  
```bash
C:\Program Files\NewTek\NewTek NDI SDK\Bin\x64;  
C:\opencv320\build\x64\vc14\bin  
```
![image](https://github.com/jadsys/NDI-Video-Sender_Receiver/wiki/images/windows38.jpg)  
![image](https://github.com/jadsys/NDI-Video-Sender_Receiver/wiki/images/windows39.jpg)

それでもDLLが見つからない場合、ソースファイルのフォルダに直接DLLを置きます。  

実行ファイルを直接実行する  
---------------------------
**1、exeファイルと同じ場所にコンフィグファイルを配置**  
NDI-Video-Senderの場合：  
コンフィグファイルの場所  
```bash
NDI-Video-Sender\NDI-Video-Sender\NdiVideoSender.config  
```
![image](https://github.com/jadsys/NDI-Video-Sender_Receiver/wiki/images/windows40.jpg)

コンフィグを以下のフォルダに配置して下さい。  
```bash
\NDI-Video-Sender\x64\Release  
```
または  
```bash
\NDI-Video-Sender\x64\Debug  
```
![image](https://github.com/jadsys/NDI-Video-Sender_Receiver/wiki/images/windows41.jpg)

NDI-Video-ReceiverもNDI-Video-Senderと同様に配置します。  
コンフィグファイルを編集します。  

**2、実行**  
コマンド・プロンプトを開きます。  
以下のコマンドを入力します。  
```bash
cd “フォルダまでの場所”\NDI-Video-Sender\x64\Release  
```
![image](https://github.com/jadsys/NDI-Video-Sender_Receiver/wiki/images/windows42.jpg)

exeファイル名を入力し、コンフィグファイルで指定したカメラ番号に対応する起動引数を入力しEnterを押します。  
![image](https://github.com/jadsys/NDI-Video-Sender_Receiver/wiki/images/windows43.jpg)
 
停止する際はCtrl+Cを押します。  
