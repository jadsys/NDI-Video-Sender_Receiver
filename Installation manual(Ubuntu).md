環境構築手順書　～Ubuntu編～  
=======

開発/実行環境について  
-------------------
 * OS：Ubuntu 16.04 LTS  

 * コンパイラ：gcc 　バージョン5.4.0  

 * 使用ライブラリ ：   
Newtek NDI　バージョン3.5  
OpenCV　バージョン3.2.0  
RealSense バージョン 2.0 (buildバージョン ～2.16.0)  

開発環境のセットアップ  
--------------------
**1、パッケージマネージャーの更新**  
ターミナルを起動し、 以下のコマンドを打ちます。  
```bash
$ sudo apt-get update  
$ sudo apt-get upgrade  
```

**2、開発ツールのインストール**  
 以下のコマンドを打ち、開発ツールをインストールします。  
```bash
$ sudo apt-get install build-essential cmake git pkg-config  
```

NewTek NDI SDK v3.5のインストール  
--------------------
**1、NewTek NDI SDK v3.5をダウンロード**  
 以下よりダウンロードを行います。  
https://jp.newtek.com/ndi/sdk/  
ダウンロード時に名前やメールアドレスなどの送信が必要です。  

**2、インストール**  
 ダウンロードされたInstallNDISDK_v3_Linux.shを、NDIをインストールしたいフォルダにコピーします。  
 ターミナル上で以下のコマンドを入力します。  
```bash
$ cd “コピーしたディレクトリ”  
$ ll InstallNDISDK_v3_Linux.sh  
$ sh ./InstallNDISDK_v3_Linux.sh  
```

OpenCV v3.2.0のインストール
--------------------
**1、画像表示ライブラリと画像フォーマットのインストール**  
 ターミナル上で以下のコマンドを実行します。  
```bash
$ sudo apt-get install libjpeg8-dev libtiff5-dev libjasper-dev libpng12-dev  
```

**2、GUI表示用のライブラリのインストール**  
 ターミナル上で以下のコマンドを実行します。  
```bash
$ sudo apt-get install libgtk2.0-dev  
```

**3、ビデオストリーム及びフレームの処理に必要なライブラリのインストール**  
 ターミナル上で以下のコマンドを実行します。  
```bash
$ sudo apt-get install libavcodec-dev libavformat-dev libswscale-dev libv4l-dev  
```

**4、OpenCVの最適化を行うライブラリのインストール**  
 ターミナル上で以下のコマンドを実行します。  
```bash
$ sudo apt-get install libatlas-base-dev gfortran  
```

**5、GitHub上のOpenCV v3.2.0のリポジトリからクローンを行う**  
 ターミナル上で以下のコマンドを実行します。  
```bash
$ git clone https://github.com/Itseez/opencv.git  
$ cd opencv  
$ git checkout 3.2.0  
$ cd ..  
$ git clone https://github.com/Itseez/opencv_contrib.git  
$ cd opencv_contrib  
$ git checkout 3.2.0  
```

**6、Cmakeを実行する**  
 ターミナル上で以下のコマンドを実行します。  
```bash
$ cd ~/opencv  
$ mkdir build  
$ cd build  
$ cmake -D CMAKE_BUILD_TYPE=RELEASE \  
	-D WITH_V4L=ON \  
	-D CMAKE_INSTALL_PREFIX=/usr/local \  
	-D INSTALL_C_EXAMPLES=ON \  
	-D INSTALL_PYTHON_EXAMPLES=ON \  
	-D OPENCV_EXTRA_MODULES_PATH=~/opencv_contrib/modules \  
	-D BUILD_EXAMPLES=ON ..  
```

**7、makeを行う**  
 ターミナル上で以下のコマンドを実行します。  
```bash  
$ make -j4  
$ sudo make install  
```

**8、OpenCVを適用させる**  
 ターミナル上で以下のコマンドを実行します。  
```bash  
$ sudo /bin/bash -c 'echo "/usr/local/lib" > /etc/ld.so.conf.d/opencv.conf'  
$ sudo ldconfig  
$ echo PKG_CONFIG_PATH=$PKG_CONFIG_PATH:/usr/local/lib/pkgconfig >> ~/bash.bashrc  
$ echo export PKG_CONFIG_PATH >> ~/bash.bashrc && cat ~/bash.bashrc  
```

RealSense SDK 2.0のインストール  
---------------------------
**1、サーバーの公開鍵を登録**  
 ターミナル上で以下のコマンドを実行します。  
```bash
$ sudo apt-key adv --keyserver keys.gnupg.net --recv-key C8B3A55A6F3EFCDE || \  
sudo apt-key adv --keyserver hkp://keyserver.ubuntu.com:80 --recv-key C8B3A55A6F3EFCDE  
```

**2、リポジトリリストにサーバーの登録**  
 ターミナル上で以下のコマンドを実行します。  
```bash
$ sudo add-apt-repository "deb http://realsense-hw-public.s3.amazonaws.com/Debian/apt-repo xenial main" -u  
```

**3、実行用ツールのインストール**  
 ターミナル上で以下のコマンドを実行します。  
```bash
$ sudo apt-get install librealsense2-dkms     
$ sudo apt-get install librealsense2-utils  
※インストールが成功すれば以下コマンドでRealsense Viewer  
を起動できるようになります。  
$ realsense-viewer  
```

**4、開発者用ツールのインストール**  
 ターミナル上で以下のコマンドを実行します。  
```bash
$ sudo apt-get install librealsense2-dev  
$ sudo apt-get install librealsense2-dbg  
```

**5、RealSense SDKのbuildバージョン確認**  
実行時にmakeするファイルが2.15.0以下と2.16.0以上で異なるため、インストールしたSDKのバージョン確認を行います。  
```bash
$ sudo apt-cache policy librealsense2-dev    
$ sudo apt-cache policy librealsense2-dbg  
```
以下のように`インストールされているバージョン:`がbuildバージョンとなります。
```bash
librealsense2-dev:
  インストールされているバージョン: 2.16.0-0~realsense0.84
  候補:               2.16.0-0~realsense0.85
  バージョンテーブル:
     2.16.0-0~realsense0.85 500
        500 http://realsense-hw-public.s3.amazonaws.com/Debian/apt-repo xenial/main amd64 Packages
 *** 2.16.0-0~realsense0.84 500
        500 http://realsense-hw-public.s3.amazonaws.com/Debian/apt-repo xenial/main amd64 Packages
        100 /var/lib/dpkg/status
     2.15.0-0~realsense0.83 500
        500 http://realsense-hw-public.s3.amazonaws.com/Debian/apt-repo xenial/main amd64 Packages  
		(以下省略)  
```

実行  
---------------------------
**1、実行前準備**  
 ターミナル上でNDIをインストールしたディレクトリに移動します。  
```bash
$ cd “インストールしたディレクトリ”/”NDI SDK for Linux”/examples/C++  
```


 以下のコマンドでC++フォルダの中にMakefile及びMakefile.projがあることを確認します。  
```bash
$ ll Makefile*
```

 Makefile.projをvimまたはテキストエディターなどで編集します。  
 3行目のCXXFLAGSを以下のように変更して下さい。  
```d
CXXFLAGS   = -std=c++11 -pthread -MMD -I../../../include
```

 5行目を以下のように変更して下さい。  
```d
LDLIBS     = -lm -lndi -ldl -lpthread -lopencv_core -lopencv_imgcodecs -lopencv_highgui \
             -lopencv_videoio -lopencv_imgproc -lrealsense2
```

 30行目も以下のように変更して下さい。  
```d
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^ $(LDLIBS)
```

 続いて実行用のフォルダを作ります。  
``` bash
$ mkdir　NDI-Vidoe-Sender
```
 Makefileを編集します。  
 8行目のPROJECTSを以下のように変更して下さい。  
```d
PROJECTS = \
                      NDI-Video-Sender \
```

**2、実行**  
**RealSense SDKのbuildバージョンによってソースファイルが異なりますので注意が必要です。**  
* バージョン2.15以下の場合  
`NDI-Video-Sender2.15`のフォルダ内のソースファイル及びコンフィグファイルをご利用下さい。  

* バージョン2.16以上の場合  
`NDI-Video-Sender-2.16`のフォルダ内のソースファイル及びコンフィグファイルをご利用下さい。  

 先程作りましたフォルダに実行したいソースファイル及びコンフィグファイルをコピーします。  
```bash
$ cd “コピーしたいソースファイルのあるディレクトリ”  
$ cp * “インストールしたディレクトリ”/”NDI SDK for Linux”/examples/C++/NDI-Video-Sender/  
```

 コピーが完了したら、再度C++フォルダに戻ります。  
```bash
$ cd “インストールしたディレクトリ”/”NDI SDK for Linux”/examples/C++
```

 以下コマンドでMakeを実行します。  
```bash
$ make  
```
 エラーが発生しなければビルド成功です。  
 ※以下のようなエラーが発生する場合、RealSense SDKのbuildバージョンと一致しているか再確認して下さい。
 ```bash
Realsense.cpp:156:54: error: no match for call to ‘(rs2::colorizer) (rs2::depth_frame)’  
                depth_frame = color_map(frameset.get_depth_frame());  
                                                                  ^
 ```
 
 ソースファイルをおいたフォルダに移動します。  
```bash
$ cd NDI-Video-Sender  
```
 以下コマンドで実行します。  
```bash
$ ./NDI-Video-Sender X（Xには送信したいカメラ番号を指定）  
```

 NDI-Video-Receiverを実行する際も同様の流れ（NDI-Video-Senderの箇所をNDI-Video-Receiverに変更となります。  

**3、ソースの修正後再度実行するまで**  
 make済みのソースファイルを修正した場合、make clean後再度makeする必要があります。  
```bash
$ make clean  
$ make  
```
