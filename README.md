NDI Video Sender / NDI Video Receiver
=======

![image](https://github.com/jadsys/NDI-Video-Sender_Receiver/wiki/images/image_01.png)

** NDI Video Sender / NDI Video Receiver ** は、Windows/Ubuntuで動作する
[NewTek NDI]プロトコルのサンプルプログラムで、ローカルエリアネットワーク経由で
複数のコンピュータ間でビデオを送受信できます。

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

１．WebカメラをUSB接続する。

２．ソースフォルダにある「NdiVideoSender.config」を編集する。
　　（編集内容については同ファイル内のコメント及び例を参照のこと）

３．プログラムの引数に編集したカメラ番号を指定して実行する。
　※実行の方法はInstallation manual(Windows)及びInstallation manual(Ubuntu)
　　をご参照下さい。（NDI-Video-Sender及びNDI-Video-Receiverどちらも同じ方法です）
　※windowsではNDI-Video-Sender\x64\Releaseの中のNDI-Video-Sender.exeを
　　コマンド・プロンプト上から実行することも可能です。ただし、NdiVideoSender.config
　　も必要となります。
　　その場合は以下のようにコマンドを打ち込みます。　
　　>cd 実行ファイルまでの場所（Cドライブ直下ならcd c:\）
　　>NDI-Video-Sender.exe X(Xは起動引数)

４．Ctrl＋Cを押せば終了します。

　※IPカメラも指定することが可能です。その場合、Camera_Flag_CAMXを「false」に
　　設定し、Camera_ID_CAMXにURLを指定して下さい。


### NDI Video Receiver

１．ソースフォルダにある「NdiVideoReceiver.config」を編集する。
　※Resources_ID_CHXの値はNdiVideoSender.configのResources_ID_CAMXと一致していることを
　　確認して下さい。

２．プログラムの引数にNDI-Video-Senderで指定した起動引数と同じ番号を指定して実行する。
　※実行の方法はInstallation manual(Windows)及びInstallation manual(Ubuntu)
　　をご参照下さい。（NDI-Video-Sender及びNDI-Video-Receiverどちらも同じ方法です）
　※windowsではNDI-Video-Receiver\x64\Releaseの中のNDI-Video-Sender.exeを
　　コマンド・プロンプト上から実行することも可能です。ただし、NdiVideoReceiver.config
　　も必要となります。
　　その場合は以下のようにコマンドを打ち込みます。　
　　>cd 実行ファイルまでの場所（Cドライブ直下ならcd c:\）
　　>NDI-Video-Receiver.exe X(Xは起動引数)

３．Ctrl＋Cを押せば終了します。

License
-------

[GPL](LICENSE)

The NDI DLL file (`Processing.NDI.Lib.x64.dll`) is provided by NewTek, 
Inc under the NDI® SDK License Agreement.
Please review the original license when distributing program.
