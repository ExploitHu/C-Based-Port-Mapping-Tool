import 'dart:io';
import 'dart:convert';
import 'dart:typed_data';
import 'package:flutter/material.dart';
import 'package:archive/archive.dart';

class SocketService {
  static final SocketService _singleton = SocketService._internal();

  factory SocketService() {
    return _singleton;
  }

  SocketService._internal();

  late Socket socket;
  bool isConnected = false;

  //连接到socket服务
  void connectToServer(Function onHandleServerData) async {
    try {
      socket = await Socket.connect('localhost', 8888);
      isConnected = true;
      print('connected to server');
      socket.listen(onHandleServerData as void Function(Uint8List event)?,
          onError: _handleServerError, onDone: _handleServerDisconnect);
    } catch (e) {
      print('error connecting to server: $e');
    }
  }

  //打印收到的消息
  void _handleServerData(Uint8List data) {
    String message = utf8.decode(data);
    print('received from server: $message');
  }

  //打印error
  void _handleServerError(error) {
    print('server error: $error');
    socket.destroy();
    isConnected = false;
  }

  //打印断开连接
  void _handleServerDisconnect() {
    print('disconnected from server');
    socket.destroy();
    isConnected = false;
  }

  //发送消息
  void sendMessage(String message) {
    if (socket != null && isConnected) {
      socket.write(message + '\n');
    }
  }
}
