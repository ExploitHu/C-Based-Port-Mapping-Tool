import 'package:flutter/material.dart';
import 'package:flutter/widgets.dart';
import 'package:port_forwarding/socketDemo.dart';
import 'package:port_forwarding/table.dart';

import 'bill.dart';
import 'eventbus/rmsgdata.dart';

class Operatingstation extends StatefulWidget {
  final String userId;
  const Operatingstation({super.key, required this.userId});

  @override
  State<Operatingstation> createState() => _OperatingstationState();
}

class _OperatingstationState extends State<Operatingstation> {
  late final _event;
  int pageNum = 1;
  @override
  void initState() {
    // TODO: implement initState
    super.initState();
    //3.监听相应类型的事件
    _event = eventBus.on<int>().listen((event) {
      //print("收到消息");
      setState(() {
        print("收到消息");
        this.pageNum = event;
      });
    });
  }

  @override
  Widget build(BuildContext context) {
    if (pageNum == 3)
      return Container(
        alignment: Alignment.topCenter,
        color: Colors.yellow,
        //child: SocketDemo(),
        //child: Text("操作台" + pageNum.toString()),
      );
    if (pageNum == 1) return MyTable(userId: widget.userId);
    if (pageNum == 2) return Bill(userId: widget.userId);
    return Container(
      alignment: Alignment.topCenter,
      color: Color.fromARGB(255, 59, 248, 255),
      child: Text("操作台" + pageNum.toString()),
    );
  }

  @override
  void dispose() {
    // TODO: implement dispose
    super.dispose();
    _event.dispose();
  }
}
