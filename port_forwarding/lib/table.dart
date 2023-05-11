import 'dart:convert';
import 'dart:io';
import 'dart:typed_data';

import 'package:flutter/material.dart';

import 'http/http.dart';
import 'socket/socketService.dart';

class MyTable extends StatefulWidget {
  final String userId;
  const MyTable({super.key, required this.userId});
  @override
  _MyTableState createState() => _MyTableState();
}

class _MyTableState extends State<MyTable> {
  static SocketService socketService = SocketService();
  String serMsg = "";
  bool serStatus = false;
  List<List> data = [];
  List portForwardData = [];
  void updateCell(int rowIndex, int columnIndex, String value) {
    setState(() {
      data[rowIndex][columnIndex] = value;
    });
    print(data[1]);
  }

  bool checkIpPortFormat(String inputString) {
    RegExp regex = RegExp(r'^\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}:\d{1,5}$');
    return regex.hasMatch(inputString);
  }

  void submitRow(int rowIndex) {
    // 提交该行数据到服务器或其他操作...

    Map msg = {
      "Name": data[rowIndex][1],
      "IntranetAddress": data[rowIndex][2],
      "Id": int.parse(data[rowIndex][0])
    };

    print('提交行数据：' + msg.toString());
    Post.send(msg, 'UpdatePortForward').then((retMap) {
      if (0 == retMap['code']) {
        for (var i = 0; i < portForwardData.length; i++) {
          if (portForwardData[i]['Id'].toString() ==
              data[rowIndex][0].toString()) {
            portForwardData[i]['IntranetAddress'] = data[rowIndex][2];
          }
        }
        ScaffoldMessenger.of(context).showSnackBar(SnackBar(
          content: Text(
            '修改成功',
            textAlign: TextAlign.center,
            style: TextStyle(
              fontFamily: 'ALI_FONT',
              color: Colors.black,
            ),
          ),
          backgroundColor: Colors.blue,
          behavior: SnackBarBehavior.floating,
          elevation: 0,
          width: 200,
        ));
      } else {
        ScaffoldMessenger.of(context).showSnackBar(SnackBar(
          content: Text(
            '无法连接到服务器',
            textAlign: TextAlign.center,
            style: TextStyle(
              fontFamily: 'ALI_FONT',
              color: Colors.black,
            ),
          ),
          backgroundColor: Color.fromARGB(255, 253, 125, 125),
          behavior: SnackBarBehavior.floating,
          elevation: 0,
          width: 200,
        ));
      }
    });
  }

  void connectedServer() {
    if (!socketService.isConnected) {
      socketService.connectToServer(_handleServerData);

      print('未连接');
    } else {
      print('连接socket状态正常');
      print(socketService.isConnected);
    }
  }

  void serSendMsg(String msg) {
    connectedServer();
    socketService.sendMessage(msg);
  }

  //打印收到的消息
  void _handleServerData(Uint8List data) {
    String message = utf8.decode(data);
    print('received from server: $message');
    this.serMsg = message;
    this.serStatus = true;
  }

  Future<String> waitForFalse() async {
    int num = 10;
    while (!serStatus) {
      await Future.delayed(Duration(milliseconds: 1000));
      if (serStatus) {
        serStatus = false;
        return serMsg;
        break;
      }
      num--;
      if (0 == num) break;
    }
    return "";
  }

/**
     print('-----------------------|||||');
    serSendMsg("你好！");
    String msg = await waitForFalse();
    print(msg);
    print('-----------------------|||||');
 */
  Future<void> onOff(int rowIndex) async {
    bool execType = data[rowIndex][4] == '开启' ? false : true;
    print(execType ? "关闭" : "开启" + data[rowIndex][1]);

    if (execType) {
      print("开启--");
      print(this.portForwardData[rowIndex]["IntranetAddress"]);
      print(this.portForwardData[rowIndex]["ExternalProgramAdPort"]);

      if (!checkIpPortFormat(
          this.portForwardData[rowIndex]["IntranetAddress"])) {
        ScaffoldMessenger.of(context).showSnackBar(SnackBar(
          content: Text(
            '请检查本地地址端口',
            textAlign: TextAlign.center,
            style: TextStyle(
              fontFamily: 'ALI_FONT',
              color: Colors.black,
            ),
          ),
          backgroundColor: Color.fromARGB(255, 253, 125, 125),
          behavior: SnackBarBehavior.floating,
          elevation: 0,
          width: 200,
        ));
        return;
      }

      Map msg = {"Id": int.tryParse(data[rowIndex][0])};
      Post.send(msg, 'RestartPortForward').then((retMap) async {
        if (0 == retMap['code']) {
          /**
 {
    "data": {
        "NetworkAdPort": "192.168.32.130:20016",
        "ProgramAdPort": "192.168.32.130:20015",
        "Id": 12
    },
    "code": 0
}
           */
          for (var i = 0; i < portForwardData.length; i++) {
            if (portForwardData[i]['Id'] == retMap['data']['Id']) {
              portForwardData[i]['ExternalNetworkAddress'] =
                  retMap['data']['NetworkAdPort'];
              portForwardData[i]['ExternalProgramAdPort'] =
                  retMap['data']['ProgramAdPort'];
            }
          }
          updateData(portForwardData);
          setState(() {
            data[rowIndex][4] = '开启';
          });
          print('-----------------------|||||');
          serSendMsg('0 ' +
              retMap['data']['Id'].toString() +
              ' ' +
              retMap['data']['ProgramAdPort'] +
              ' ' +
              this.portForwardData[rowIndex]["IntranetAddress"]);
          String msg = await waitForFalse();
          print(msg);
          print('0 ' +
              retMap['data']['Id'].toString() +
              ' ' +
              retMap['data']['ProgramAdPort'] +
              ' ' +
              this.portForwardData[rowIndex]["IntranetAddress"]);
          print('-----------------------|||||');
          ScaffoldMessenger.of(context).showSnackBar(SnackBar(
            content: Text(
              '开启成功',
              textAlign: TextAlign.center,
              style: TextStyle(
                fontFamily: 'ALI_FONT',
                color: Colors.black,
              ),
            ),
            backgroundColor: Colors.blue,
            behavior: SnackBarBehavior.floating,
            elevation: 0,
            width: 200,
          ));
        } else {
          ScaffoldMessenger.of(context).showSnackBar(SnackBar(
            content: Text(
              '错误代码：' +
                  retMap['code'].toString() +
                  "  错误信息：" +
                  retMap['data']['text'],
              textAlign: TextAlign.center,
              style: TextStyle(
                fontFamily: 'ALI_FONT',
                color: Colors.black,
              ),
            ),
            backgroundColor: Color.fromARGB(255, 253, 125, 125),
            behavior: SnackBarBehavior.floating,
            elevation: 0,
            width: 200,
          ));
        }
      });
    } else {
      print("关闭--");
      Map msg = {"Id": int.tryParse(data[rowIndex][0])};
      Post.send(msg, 'ClosePortForward').then((retMap) async {
        if (0 == retMap['code']) {
          print('-----------------------|||||');
          serSendMsg('1 ' +
              this.portForwardData[rowIndex]['Id'].toString() +
              ' ' +
              '0.0.0.0:1' +
              ' ' +
              '0.0.0.0:1');
          String msg = await waitForFalse();
          print(msg);
          print('-----------------------|||||');

          setState(() {
            data[rowIndex][4] = '关闭';
            for (var i = 0; i < portForwardData.length; i++) {
              if (portForwardData[i]['Id'] == data[rowIndex][0]) {
                portForwardData[i]['StatusStatus'] = '关闭';
              }
            }
          });
          ScaffoldMessenger.of(context).showSnackBar(SnackBar(
            content: Text(
              '关闭成功',
              textAlign: TextAlign.center,
              style: TextStyle(
                fontFamily: 'ALI_FONT',
                color: Colors.black,
              ),
            ),
            backgroundColor: Colors.blue,
            behavior: SnackBarBehavior.floating,
            elevation: 0,
            width: 200,
          ));
        } else {
          ScaffoldMessenger.of(context).showSnackBar(SnackBar(
            content: Text(
              '错误代码：' +
                  retMap['code'].toString() +
                  "  错误信息：" +
                  retMap['data']['text'],
              textAlign: TextAlign.center,
              style: TextStyle(
                fontFamily: 'ALI_FONT',
                color: Colors.black,
              ),
            ),
            backgroundColor: Color.fromARGB(255, 253, 125, 125),
            behavior: SnackBarBehavior.floating,
            elevation: 0,
            width: 200,
          ));
        }
      });
    }
  }

  void updateData(portForwardData) {
    setState(() {
      this.portForwardData = portForwardData;

      data = [];
      for (var i = 0; i < portForwardData.length; i++) {
        print(portForwardData[i]['IntranetAddress'].toString());
        data.add([
          portForwardData[i]['Id'].toString(),
          portForwardData[i]['Name'].toString(),
          portForwardData[i]['IntranetAddress'].toString(),
          portForwardData[i]['ExternalNetworkAddress'].toString(),
          0 == portForwardData[i]['Status'] ? "开启" : "关闭",
        ]);
      }
    });
  }

  @override
  void initState() {
    super.initState();
    connectedServer();
    Map msg = {"UserId": int.parse(widget.userId)};
    Post.send(msg, 'GetPortForward').then((retMap) {
      if (0 == retMap['code']) {
        portForwardData = retMap['data'];
        print(widget.userId);
        print(retMap);
        setState(() {
          //数据处理
          //this.dBalance = retMap['data']['Balance'].toDouble();
          /*
          
                  {
            "Id": 21,
            "Name": "映射名称",
            "IntranetAddress": "1",
            "CreateTime": 1683122613,
            "ExternalNetworkAddress": "1",
            "ExternalProgramAdPort": "ip地址:端口",
            "EndTime": 1683183459,
            "Money": 10,
            "BillId": 10000023,
            "Status": -1
        },
           */
          data = [];
          for (var i = 0; i < retMap['data'].length; i++) {
            data.add([
              retMap['data'][i]['Id'].toString(),
              retMap['data'][i]['Name'].toString(),
              retMap['data'][i]['IntranetAddress'].toString(),
              retMap['data'][i]['ExternalNetworkAddress'].toString(),
              0 == retMap['data'][i]['Status'] ? "开启" : "关闭",
            ]);
            print(data);
          }
        });
      } else {
        ScaffoldMessenger.of(context).showSnackBar(SnackBar(
          content: Text(
            '无法连接到服务器',
            textAlign: TextAlign.center,
            style: TextStyle(
              fontFamily: 'ALI_FONT',
              color: Colors.black,
            ),
          ),
          backgroundColor: Color.fromARGB(255, 253, 125, 125),
          behavior: SnackBarBehavior.floating,
          elevation: 0,
          width: 200,
        ));
      }
    });
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      backgroundColor: Color.fromARGB(31, 80, 80, 80),
      body: SingleChildScrollView(
        scrollDirection: Axis.vertical,
        child: SingleChildScrollView(
          scrollDirection: Axis.horizontal,
          child: DataTable(
            columns: [
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*              DataColumn(
                label: Text(
                  'ID',
                  style: TextStyle(
                    fontFamily: 'ALI_FONT',
                  ),
                ),
              ),*/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
              DataColumn(
                  label: Text(
                '名称',
                style: TextStyle(
                  fontFamily: 'ALI_FONT',
                ),
              )),
              DataColumn(
                  label: Text(
                '本地地址端口',
                style: TextStyle(
                  fontFamily: 'ALI_FONT',
                ),
              )),
              DataColumn(
                  label: Text(
                '映射地址端口',
                style: TextStyle(
                  fontFamily: 'ALI_FONT',
                ),
              )),
              DataColumn(
                  label: Text(
                '状态',
                style: TextStyle(
                  fontFamily: 'ALI_FONT',
                ),
              )),
              DataColumn(
                  label: Text(
                '操作',
                style: TextStyle(
                  fontFamily: 'ALI_FONT',
                ),
              )),
            ],
            rows: List.generate(
              data.length,
              (rowIndex) => DataRow(
                cells: [
////////////////////////////////////////////////////////////////////////////////////////////////////////////
                  //DataCell(Text(data[rowIndex][0].toString())),
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                  DataCell(
                    Row(
                      children: [
                        Expanded(
                          child: TextFormField(
                            initialValue: data[rowIndex][1].toString(),
                            onChanged: (value) =>
                                updateCell(rowIndex, 1, value),
                          ),
                        ),
                      ],
                    ),
                  ),

                  DataCell(
                    Row(
                      children: [
                        Expanded(
                          child: TextFormField(
                            initialValue: data[rowIndex][2].toString(),
                            onChanged: (value) =>
                                updateCell(rowIndex, 2, value),
                          ),
                        ),
                      ],
                    ),
                  ),
                  DataCell(Text(data[rowIndex][3].toString())),
                  DataCell(Text(data[rowIndex][4].toString())),
                  DataCell(Row(
                    children: [
                      ElevatedButton(
                        onPressed: () => submitRow(rowIndex),
                        child: Text(
                          '提交修改',
                          style: TextStyle(
                            fontFamily: 'ALI_FONT',
                          ),
                        ),
                      ),
                      SizedBox(width: 10),
                      ElevatedButton(
                        onPressed: () => onOff(rowIndex),
                        child: Text(
                          data[rowIndex][4] == '开启' ? "关闭" : "开启",
                          style: TextStyle(
                            fontFamily: 'ALI_FONT',
                          ),
                        ),
                      ),
                    ],
                  )),
                ],
              ),
            ),
          ),
        ),
      ),
    );
  }
}
