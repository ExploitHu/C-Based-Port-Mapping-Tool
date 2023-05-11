/**import 'dart:io';
import 'dart:convert';
import 'dart:typed_data';
import 'package:flutter/material.dart';
import 'package:archive/archive.dart';
import './socket/socketService.dart';

class SocketDemo extends StatefulWidget {
  @override
  _SocketDemoState createState() => _SocketDemoState();
}

class _SocketDemoState extends State<SocketDemo> {
  final TextEditingController _controller = new TextEditingController();
  static SocketService socketService = SocketService();
  void function1() {
    if (!socketService.isConnected) {
      socketService.connectToServer(_handleServerData);
      print('未连接');
    } else {
      print(socketService.isConnected);
    }
    socketService.sendMessage('Hello World!');
  }

  //打印收到的消息
  void _handleServerData(Uint8List data) {
    String message = utf8.decode(data);
    print('received from server: $message');
  }

  Widget _buildSendButton() {
    return TextButton(
      child: Text('Send'),
      onPressed: function1,
    );
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text('Socket Demo'),
      ),
      body: Center(
        child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          children: <Widget>[
            Text(
              'Connected',
              style: TextStyle(fontSize: 20),
            ),
            SizedBox(height: 20),
            TextField(
              controller: _controller,
              decoration: InputDecoration(
                hintText: 'Enter message...',
                border: OutlineInputBorder(),
              ),
            ),
            SizedBox(height: 20),
            Row(
              mainAxisAlignment: MainAxisAlignment.center,
              children: <Widget>[
                _buildSendButton(),
                SizedBox(width: 20),
                _buildSendButton(),
              ],
            ),
          ],
        ),
      ),
    );
  }
}
 */