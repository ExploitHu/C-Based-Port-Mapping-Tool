import 'package:flutter/material.dart';
import 'package:flutter/widgets.dart';
import 'package:port_forwarding/accountNumberRes.dart';
import 'package:port_forwarding/menu.dart';
import 'package:port_forwarding/operatingstation.dart';
import 'package:port_forwarding/service/screenAdapter.dart';

import 'userMes.dart';

class Body extends StatefulWidget {
  final Function() onLogOut; // 接受登录函数的回调
  final String userId;
  const Body({super.key, required this.userId, required this.onLogOut});

  @override
  State<Body> createState() => _BodyState();
}

class _BodyState extends State<Body> {
  int _counter = 0;
  var widgetWith = ScreenAdapter.width(375);
  var accountnumberHeight = ScreenAdapter.height(100);
  void _increment() {
    setState(() {
      _counter++;
    });
  }

  @override
  Widget build(BuildContext context) {
    return Container(
      child: Row(
        children: [
          Container(
            child: Container(
              child: Column(
                children: [
                  Container(
                    width: widgetWith,
                    child: Menu(userId: widget.userId),
                  ),
                  Expanded(
                    child: Container(
                      width: widgetWith,
                      height: 10,
                      color: Colors.black,
                      child: AccountNumberRes(
                        userId: widget.userId,
                      ),
                    ),
                    flex: 1,
                  ),
                  Container(
                    height: accountnumberHeight,
                    width: widgetWith,
                    color: Color.fromARGB(255, 36, 153, 161),
                    child: UseMes(
                      userId: widget.userId,
                      onLogOut: widget.onLogOut,
                    ),
                  ),
                ],
              ),
            ),
          ),
          Expanded(
            child: Container(
              //alignment: Alignment.topCenter,
              height: ScreenAdapter.height(1150),
              child: Operatingstation(userId: widget.userId),
            ),
            flex: 1,
          ),
        ],
      ),
    );
  }
}
