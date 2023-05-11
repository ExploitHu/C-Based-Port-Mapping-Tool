import 'package:flutter/material.dart';
import 'package:flutter/widgets.dart';
import 'package:port_forwarding/accountNumberRes.dart';
import 'package:port_forwarding/res/aliIcon.dart';
import 'package:port_forwarding/service/screenAdapter.dart';

import 'eventbus/rmsgdata.dart';

class Menu extends StatefulWidget {
  final String userId;
  const Menu({super.key, required this.userId});

  @override
  State<Menu> createState() => _MenuState();
}

class _MenuState extends State<Menu> {
  var _pageIndex = 1;

  var _iconWidth = ScreenAdapter.width(60);
  var _textWidth = ScreenAdapter.width(98);
  var _textSize = ScreenAdapter.size(24);

  @override
  Widget build(BuildContext context) {
    print(_iconWidth);

    return Container(
      color: Color.fromARGB(96, 208, 177, 22),
      child: Column(children: [
        Container(
          margin: EdgeInsets.only(top: 20),
          alignment: Alignment.topCenter,
          //color: Colors.pinkAccent,
          child: Column(
            children: [
              MaterialButton(
                onPressed: () {
                  _pageIndex = 1;
                  eventBus.fire(_pageIndex);
                  print('点击端口映射');
                },
                child: Container(
                  padding: EdgeInsets.fromLTRB(0, 10, 0, 10),
                  child: Row(
                    mainAxisAlignment: MainAxisAlignment.center,
                    children: [
                      Container(
                        width: _iconWidth,
                        child: Icon(AliIcon.getIcon('map')),
                      ),
                      Container(
                        margin: EdgeInsets.only(left: 10),
                        width: _textWidth,
                        alignment: Alignment.center,
                        child: Text(
                          '端口映射',
                          style: TextStyle(
                            fontFamily: 'ALI_FONT',
                            fontSize: _textSize,
                          ),
                        ),
                      ),
                    ],
                  ),
                ),
              ),
              /*MaterialButton(
                onPressed: () {
                  _pageIndex = 2;
                  eventBus.fire(_pageIndex);
                  print('点击端口映射');
                },
                child: Container(
                  padding: EdgeInsets.fromLTRB(0, 10, 0, 10),
                  child: Row(
                    mainAxisAlignment: MainAxisAlignment.center,
                    children: [
                      Container(
                        width: _iconWidth,
                        child: Icon(AliIcon.getIcon('statistics')),
                      ),
                      Container(
                        margin: EdgeInsets.only(left: 10),
                        width: _textWidth,
                        alignment: Alignment.center,
                        child: Text(
                          '流量统计',
                          style: TextStyle(
                            fontFamily: 'ALI_FONT',
                            fontSize: _textSize,
                          ),
                        ),
                      ),
                    ],
                  ),
                ),
              ),*/
              Container(
                height: 20,
                child: Divider(
                  height: 1,
                  indent: 30,
                  color: Color.fromARGB(255, 131, 130, 130),
                ),
              ),
/*
              MaterialButton(
                onPressed: () {
                  _pageIndex = 3;
                  eventBus.fire(_pageIndex);
                  print('点击端口映射');
                },
                child: Container(
                  padding: EdgeInsets.fromLTRB(0, 10, 0, 10),
                  child: Row(
                    mainAxisAlignment: MainAxisAlignment.center,
                    children: [
                      Container(
                        width: _iconWidth,
                        child: Icon(AliIcon.getIcon('mailbox')),
                      ),
                      Container(
                        margin: EdgeInsets.only(left: 10),
                        width: _textWidth,
                        alignment: Alignment.center,
                        child: Text(
                          '信    箱',
                          style: TextStyle(
                            fontFamily: 'ALI_FONT',
                            fontSize: _textSize,
                          ),
                        ),
                      ),
                    ],
                  ),
                ),
              ),
              
              MaterialButton(
                onPressed: () {
                  _pageIndex = 4;
                  eventBus.fire(_pageIndex);
                  print('点击端口映射');
                },
                child: Container(
                  padding: EdgeInsets.fromLTRB(0, 10, 0, 10),
                  child: Row(
                    mainAxisAlignment: MainAxisAlignment.center,
                    children: [
                      Container(
                        width: _iconWidth,
                        child: Icon(AliIcon.getIcon('userMsg')),
                      ),
                      Container(
                        margin: EdgeInsets.only(left: 10),
                        width: _textWidth,
                        alignment: Alignment.center,
                        child: Text(
                          '账号信箱',
                          style: TextStyle(
                            fontFamily: 'ALI_FONT',
                            fontSize: _textSize,
                          ),
                        ),
                      ),
                    ],
                  ),
                ),
              ),*/
              MaterialButton(
                onPressed: () {
                  _pageIndex = 2;
                  eventBus.fire(_pageIndex);
                  print('点击端口映射');
                },
                child: Container(
                  padding: EdgeInsets.fromLTRB(0, 10, 0, 10),
                  child: Row(
                    mainAxisAlignment: MainAxisAlignment.center,
                    children: [
                      Container(
                        width: _iconWidth,
                        child: Icon(AliIcon.getIcon('bill')),
                      ),
                      Container(
                        margin: EdgeInsets.only(left: 10),
                        width: _textWidth,
                        alignment: Alignment.center,
                        child: Text(
                          '购买记录',
                          style: TextStyle(
                            fontFamily: 'ALI_FONT',
                            fontSize: _textSize,
                          ),
                        ),
                      ),
                    ],
                  ),
                ),
              ),
            ],
          ),
        ),
      ]),
    );
  }
}
/*
        Expanded(
          child: Container(
            child: AccountNumber(),
          ),
          flex: 1,
        )
*/
/**
 ElevatedButton(
        child: Text("这是一个按钮"),
        onPressed: () {
          print('点击按钮');
          //发出事件
          eventBus.fire('otherPage' + pageIndex.toString());
          pageIndex++;
        },
      ),
 */