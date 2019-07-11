環境構築手順書　～Ubuntu編～  
=======

開発/実行環境について  
-------------------
 * OS：Ubuntu 16.04 LTS  

 * コンパイラ：gcc 　バージョン5.4.0  

 * 使用ライブラリ ：   
Newtek NDI　バージョン3.8  
OpenCV　バージョン4.1.0  
RealSense バージョン 2.0 (buildバージョン ～2.16.0)  
ZBar バージョン 0.10
開発環境のセットアップ  
--------------------
**1、パッケージマネージャーの更新**  
ターミナルを起動し、 以下のコマンドを打ちます。  
```bash
# apt update  
# apt upgrade  
```

**2、開発ツールのインストール**  
 以下のコマンドを打ち、開発ツールをインストールします。  
```bash
# apt install build-essential cmake git pkg-config  
```

zbarのインストール 
--------------------
```bash
# apt install libzbar0 libzbar0-dev
```

Boostrのインストール 
--------------------
```bash
# apt install libboost-all-dev
```
NewTek NDI SDK v3.5のインストール  
--------------------
**1、NewTek NDI SDK v3.5をダウンロード**  
 以下よりダウンロードを行います。  
https://jp.newtek.com/ndi/sdk/  
ダウンロード時に名前やメールアドレスなどの送信が必要です。  

**2、インストール**  
 ダウンロードされたInstallNDISDK\_v3\_Linux.shを、NDIをインストールしたいフォルダにコピーします。  
 ターミナル上で以下のコマンドを入力します。  
```bash
$ cd “コピーしたディレクトリ”  
$ ll InstallNDISDK_v3_Linux.sh  
$ sh ./InstallNDISDK_v3_Linux.sh  
```

**3、デプロイ**
NDIを使用したプログラム作成のためpkg-configを適用します
```bash
$ cd NDI\ SDK\ for\ Linux/lib/x86_64-linux-gnu
$ mkdir pkgconfig
```
libndi.pcという名前のファイルを作成し、以下の内容にします。
```
prefix=<NDI SDKのインストール先>
exec_prefix=${prefix}
libdir=${exec_prefix}/lib/x86_64-linux-gnu/
include_dir=${prefix}/include

Name: libNDI
Description:Multimedia transmit library
Version: 3.5.0
Libs: -L${exec_prefix}/lib/x86_64-linux-gnu -lndi
Cflags: -I${include_dir}
```
pkg-configの読み取りパスに追加します。以下の内容を`~/.profile`に追加してください。

```
export PKG_CONFIG_PATH=/<path to NDI>/lib/x86_64-linux-gnu/pkgconfig:$PKG_CONFIG_PATH
```

OpenCV v4.1.0のインストール
--------------------
**1、画像表示ライブラリと画像フォーマットのインストール**  
ターミナル上で以下のコマンドを実行します。  
```bash
# apt install libjpeg8-dev libtiff5-dev libjasper-dev libpng12-dev  
```

**2、GUI表示用のライブラリのインストール**  
ターミナル上で以下のコマンドを実行します。  
```bash
# apt install libgtk2.0-dev  
```

**3、ビデオストリーム及びフレームの処理に必要なライブラリのインストール**  
ターミナル上で以下のコマンドを実行します。  
```bash
# apt install libavcodec-dev libavformat-dev libswscale-dev libv4l-dev  
```

**4、OpenCVの最適化を行うライブラリのインストール**  
ターミナル上で以下のコマンドを実行します。  
```bash
# apt install libatlas-base-dev gfortran  
```

**5、GitHub上のOpenCV v4.1.0のリポジトリからクローンを行う**  
ターミナル上で以下のコマンドを実行します。  
```bash
$ git clone https://github.com/opencv/opencv.git  
$ cd opencv  
$ git checkout 4.1.0  
```
**6、OpenCVのインストール先フォルダを作成する**
OpenCVのバージョンアップ等のため、インストール先を指定します。
ここでは`/opt/opencv410`にインストールすると仮定します。
```bash
# mkdir /opt/opencv410
```
**7、Cmakeを実行する**  
 ターミナル上で以下のコマンドを実行します。  
```bash
$ cd ~/opencv  
$ mkdir build  
$ cd build  
$ cmake -D CMAKE_BUILD_TYPE=RELEASE \  
	-D WITH_V4L=ON \  
	-D CMAKE_INSTALL_PREFIX=/opt/opencv410 \  
```

**8、makeを行う**  
 ターミナル上で以下のコマンドを実行します。  
```bash  
$ make  
# make install  
```

**9、OpenCVを適用させる**  
 ターミナル上で以下のコマンドを実行します。  
```bash  
# echo "/opt/opencv410/lib" > /etc/ld.so.conf.d/opencv.conf 
$ sudo ldconfig  
```
pkg-configとcmakeのパスを登録します。以下の内容を`~/.profile`に追加してください。
```
$ export PKG_CONFIG_PATH=/opt/opencv410/lib/pkgconfig:$PKG_CONFIG_PATH
$ export CMAKE_PREFIX_PATH=/opt/opencv410:$CMAKE_PREFIX_PATH
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
# apt install librealsense2-dkms     
# apt install librealsense2-utils  
※インストールが成功すれば以下コマンドでRealsense Viewer  
を起動できるようになります。  
$ realsense-viewer  
```

**4、開発者用ツールのインストール**  
 ターミナル上で以下のコマンドを実行します。  
```bash
# apt install librealsense2-dev  
# apt install librealsense2-dbg  
```


実行  
---------------------------
**1、ビルド** 
cmakeを用いてMakfileを作成します。
その後プログラムのビルドを行います。
```bash
$ cd NDI-Video-Sender/ 
$ mkdir build
$ cd build
$ cmake ..
$ make
```

**2、実行**  
```bash
$ ./NDI-Video-Sender <カメラ台数> <同時送信数>
```
起動直後には、送信の1番とカメラ1番、送信の2番とカメラ2番のように同番号のものが対応付けされています。

 NDI-Video-Receiverを実行する際も同様の流れ（NDI-Video-Senderの箇所をNDI-Video-Receiverに変更となります。  

