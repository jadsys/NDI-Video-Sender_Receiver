NDI Video Sender / NDI Video Receiver
=======

![image](https://github.com/jadsys/NDI-Video-Sender_Receiver/wiki/images/image_01.png)

** NDI Video Sender / NDI Video Receiver ** は、Windows/Ubuntuで動作する  
[NewTek NDI]プロトコルのサンプルプログラムで、ローカルエリアネットワーク経由で  
複数のコンピュータ間でビデオを送受信できます。  
  UVC規格のWebカメラ、IPカメラ、Depthカメラ(RealSense D435)からの映像をNDIプロトコルを  
介して送受信することが可能です。
  
NDI™ is a trademark of NewTek, Inc.  
  
[NewTek NDI]: http://NDI.NewTek.com/
  
System requirements & Installations
-------------------
Windowsのシステム要件および構築の手順については  
"Installation manual (Windows) .pdf"をご参照下さい。  
  
Linux（Ubuntu）のシステム要件および構築の手順については  
"Installation manual (Ubuntu) .pdf"をご参照下さい。  
  
How to use
--------------------
  
### NDI Video Sender
  
1.Webカメラ or IPカメラ or Depthカメラ(RealSense)を接続する。  
  
2.カメラの送信設定を行う。  
  ソースフォルダ内にある「NdiVideoSender.config」を、実行ファイル(.exe)と  
  同じフォルダにコピーし、中身を編集する。  
  (編集内容については同ファイル内のコメント及び例をご参照下さい。  
   最大6個のカメラ設定を入れることが可能です)  
  
3.プログラム実行時の起動引数に、2.で編集したカメラ番号を指定する。  
  (実行の方法は「Installation manual」をご参照下さい)  
  
4.終了時は「Ctrl＋C」を押下する。  
  
  ※IPカメラも指定することが可能です。その場合、Camera_Flag_CAMXを  
    「false」に設定し、Camera_ID_CAMXにカメラURLを指定して下さい。  
  
  
### NDI Video Receiver
	
1.カメラの受信設定を行う。  
  ソースフォルダ内にある「NdiVideoReceiver.config」を、実行ファイル(.exe)と  
  同じフォルダにコピーし、中身を編集する。  
  (Resources_ID_CHXの値はSenderで設定したResources_ID_CAMXを指定して下さい)  
  
2.プログラム実行時の起動引数に、2.で編集したカメラ番号を指定する。  
  (実行の方法は「Installation manual」をご参照下さい)  
  
3.終了時は「Ctrl＋C」を押下する。  
  
  
License
-------

[GPL](LICENSE)

The NDI DLL file (`Processing.NDI.Lib.x64.dll`) is provided by NewTek,   
Inc under the NDI® SDK License Agreement.  
Please review the original license when distributing program.  
