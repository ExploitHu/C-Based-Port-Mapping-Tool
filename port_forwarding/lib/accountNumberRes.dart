import 'package:flutter/material.dart';
import 'package:flutter/widgets.dart';
import 'package:fluttertoast/fluttertoast.dart';
import 'package:port_forwarding/service/screenAdapter.dart';

import 'http/http.dart';
import 'myWidget/progressBar.dart';

class AccountNumberRes extends StatefulWidget {
  final String userId;
  const AccountNumberRes({super.key, required this.userId});
  @override
  _AccountNumberResState createState() => _AccountNumberResState();
}

class _AccountNumberResState extends State<AccountNumberRes> {
  late double dBalance;
  int nTotalPort = 10;
  int nUsePort = 5;
  //int nTotalBandwidth = 100;
  //int nUseBandwidth = 53;
  final codeController = TextEditingController();

  int portNumber = 0;
  double pricePerPort = 1000.0;

  double get totalPrice => portNumber * pricePerPort;

  @override
  void initState() {
    dBalance = 0.0;
    super.initState();

    Map msg = {"UserId": widget.userId};
    Post.send(msg, 'GetUserBalance').then((retMap) {
      if (0 == retMap['code']) {
        setState(() {
          this.dBalance = retMap['data']['Balance'].toDouble();
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
      print('获取余额');
    });

    msg = {};
    Post.send(msg, 'GetResUnitPrice').then((retMap) {
      if (0 == retMap['code']) {
        setState(() {
          this.pricePerPort = retMap['data']['PortUnitPrice'].toDouble();
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
      print('获取余额');
    });
    this._updatePortStatus();
  }

  void _recharge(String secretKey) {
    Map msg = {
      "CardSecret": secretKey,
      "UserId": widget.userId,
    };
    Post.send(msg, 'RechargeWithCardCode').then((retMap) {
      if (0 == retMap['code']) {
        setState(() {
          this.dBalance = retMap['data']['Balance'].toDouble();
        });
        ScaffoldMessenger.of(context).showSnackBar(SnackBar(
          content: Text(
            '充值成功，充值金额：' + retMap['data']['Money'].toString(),
            textAlign: TextAlign.center,
            style: TextStyle(
              fontFamily: 'ALI_FONT',
              color: Colors.black,
            ),
          ),
          backgroundColor: Colors.blue,
          behavior: SnackBarBehavior.floating,
          elevation: 0,
          width: 300,
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
          width: 300,
        ));
      }
    });
  }

  void _updateBlance(double nBalance) {
    setState(() {
      this.dBalance = nBalance;
    });
  }

  void _updatePortStatus() {
    Map msg = {"UserId": widget.userId};
    Post.send(msg, 'GetResStatus').then((retMap) {
      if (0 == retMap['code']) {
        setState(() {
          this.nTotalPort = retMap['data']['Total'].toInt();
          this.nUsePort = retMap['data']['Use'].toInt();
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
      print('获取端口状态');
    });
  }

  @override
  Widget build(BuildContext context) {
    return Container(
      alignment: Alignment.bottomCenter,
      color: Color.fromARGB(255, 211, 246, 212),
      child: Column(
        mainAxisAlignment: MainAxisAlignment.end,
        children: [
          Container(
            margin: EdgeInsets.only(top: ScreenAdapter.height(20)),
            child: Row(
              mainAxisAlignment: MainAxisAlignment.spaceBetween,
              children: [
                Container(
                  margin: EdgeInsets.only(left: 10),
                  child: Column(
                    crossAxisAlignment: CrossAxisAlignment.start,
                    children: [
                      Text(
                        '余额',
                        style: TextStyle(
                          fontFamily: 'ALI_FONT',
                        ),
                      ),
                      Text(
                        '￥' + this.dBalance.toString(),
                        style: TextStyle(
                          fontFamily: 'ALI_FONT',
                        ),
                      ),
                    ],
                  ),
                ),
                Container(
                  margin: EdgeInsets.only(right: 20),
                  child: TextButton(
                    child: Text(
                      '充值',
                      style: TextStyle(
                        fontFamily: 'ALI_FONT',
                        fontSize: 30,
                        color: Color.fromARGB(255, 12, 0, 0),
                      ),
                    ),
                    onPressed: () {
                      print(this.dBalance);
                      showDialog(
                        context: context,
                        builder: (context) => AlertDialog(
                          title: Text('请输入充值卡密'),
                          content: TextField(
                            controller: codeController,
                            decoration: InputDecoration(
                              hintText: '请输入充值卡密',
                            ),
                          ),
                          actions: [
                            TextButton(
                              onPressed: () => Navigator.of(context).pop(),
                              child: Text('取消'),
                            ),
                            ElevatedButton(
                              onPressed: () {
                                final code = codeController.text.trim();
                                if (code.isNotEmpty) {
                                  // 将卡密提交到服务器进行充值
                                  print("调用充值接口");
                                  this._recharge(codeController.text);
                                  Navigator.of(context).pop();
                                  codeController.text = "";
                                }
                              },
                              child: Text('确定'),
                            ),
                          ],
                        ),
                      );
                    },
                  ),
                ),
              ],
            ),
          ),
          Container(
            margin: EdgeInsets.only(top: ScreenAdapter.height(20)),
            //height: 10,
            color: Colors.blueAccent,
            child: Column(
              children: [
                Container(
                  child: Row(
                    mainAxisAlignment: MainAxisAlignment.spaceBetween,
                    children: [
                      Container(
                        margin: EdgeInsets.only(left: 10),
                        child: Text(
                          '$nUsePort/$nTotalPort个',
                          style: TextStyle(
                            fontFamily: 'ALI_FONT',
                            fontSize: 15,
                            color: Colors.white,
                          ),
                        ),
                      ),
                      Container(
                        margin: EdgeInsets.only(right: 10),
                        child: TextButton(
                          child: Text(
                            '购买端口',
                            style: TextStyle(
                              fontFamily: 'ALI_FONT',
                              fontSize: 15,
                              color: Colors.white,
                            ),
                          ),
                          onPressed: () async {
                            print('购买端口');
                            await showDialog(
                              context: context,
                              builder: (context) => StatefulBuilder(
                                builder: (context, setState) {
                                  return AlertDialog(
                                    title: Text('购买端口'),
                                    content: Column(
                                      mainAxisSize: MainAxisSize.min,
                                      children: [
                                        Row(
                                          mainAxisAlignment:
                                              MainAxisAlignment.spaceBetween,
                                          children: [
                                            Text('端口数量：'),
                                            Row(
                                              children: [
                                                IconButton(
                                                  icon: Icon(Icons.remove),
                                                  onPressed: () {
                                                    setState(() {
                                                      if (portNumber > 0)
                                                        portNumber--;
                                                    });
                                                  },
                                                ),
                                                Text(portNumber.toString()),
                                                IconButton(
                                                  icon: Icon(Icons.add),
                                                  onPressed: () {
                                                    setState(() {
                                                      if ((dBalance -
                                                              (portNumber + 1) *
                                                                  pricePerPort) >
                                                          0) portNumber++;
                                                    });
                                                  },
                                                ),
                                              ],
                                            ),
                                          ],
                                        ),
                                        SizedBox(height: 16),
                                        Text(
                                            '单价：\￥$pricePerPort     总价：\￥$totalPrice'),
                                      ],
                                    ),
                                    actions: [
                                      TextButton(
                                        onPressed: () {
                                          Navigator.of(context).pop();
                                          portNumber = 0;
                                        },
                                        child: Text('取消'),
                                      ),
                                      ElevatedButton(
                                        onPressed: () {
                                          // 将购买端口数量提交到服务器进行购买
                                          var resList = [];
                                          for (var i = 0; i < portNumber; i++) {
                                            resList.add(
                                                {"LeaseTime": 60 * 60 * 24});
                                          }
                                          Map msg = {
                                            "Port": resList,
                                            "UserId": widget.userId
                                          };
                                          if (0 != portNumber)
                                            Post.send(msg, 'BuyRes')
                                                .then((retMap) {
                                              if (0 == retMap['code']) {
                                                _updatePortStatus();
                                                this._updateBlance(
                                                    retMap['data']['Balance']
                                                        .toDouble());

                                                ScaffoldMessenger.of(context)
                                                    .showSnackBar(SnackBar(
                                                  content: Text(
                                                    '购买成功',
                                                    textAlign: TextAlign.center,
                                                    style: TextStyle(
                                                      fontFamily: 'ALI_FONT',
                                                      color: Colors.black,
                                                    ),
                                                  ),
                                                  backgroundColor: Colors.blue,
                                                  behavior:
                                                      SnackBarBehavior.floating,
                                                  elevation: 0,
                                                  width: 300,
                                                ));
                                              } else {
                                                ScaffoldMessenger.of(context)
                                                    .showSnackBar(SnackBar(
                                                  content: Text(
                                                    '错误代码：' +
                                                        retMap['code']
                                                            .toString() +
                                                        "  错误信息：" +
                                                        retMap['data']['text'],
                                                    textAlign: TextAlign.center,
                                                    style: TextStyle(
                                                      fontFamily: 'ALI_FONT',
                                                      color: Colors.black,
                                                    ),
                                                  ),
                                                  backgroundColor:
                                                      Color.fromARGB(
                                                          255, 253, 125, 125),
                                                  behavior:
                                                      SnackBarBehavior.floating,
                                                  elevation: 0,
                                                  width: 300,
                                                ));
                                              }
                                            });
                                          Navigator.of(context).pop();
                                          portNumber = 0;
                                        },
                                        child: Text('购买'),
                                      ),
                                    ],
                                  );
                                },
                              ),
                            );
                          },
                        ),
                      ),
                    ],
                  ),
                ),
                Container(
                  margin: EdgeInsets.fromLTRB(10, 0, 20, 0),
                  child: ProgressBar(
                    value: nTotalPort != 0 ? nUsePort / nTotalPort : 0,
                    label: '',
                  ),
                ),
              ],
            ),
          ),
          SizedBox(
            height: 10,
          ), /*
          Container(
            margin: EdgeInsets.only(top: ScreenAdapter.height(5)),
            //height: 10,
            color: Colors.blueAccent,
            child: Column(
              children: [
                Container(
                  child: Row(
                    mainAxisAlignment: MainAxisAlignment.spaceBetween,
                    children: [
                      Container(
                        margin: EdgeInsets.only(left: 10),
                        child: Text(
                          '$nUseBandwidth M/$nTotalBandwidth M',
                          style: TextStyle(
                            fontFamily: 'ALI_FONT',
                            fontSize: 15,
                            color: Colors.white,
                          ),
                        ),
                      ),
                      Container(
                        margin: EdgeInsets.only(right: 10),
                        child: TextButton(
                          child: Text(
                            '购买带宽',
                            style: TextStyle(
                              fontFamily: 'ALI_FONT',
                              fontSize: 15,
                              color: Colors.white,
                            ),
                          ),
                          onPressed: () {
                            print('购买带宽');
                            setState(() {
                              this.nUseBandwidth--;
                            });
                          },
                        ),
                      ),
                    ],
                  ),
                ),
                Container(
                  margin: EdgeInsets.fromLTRB(10, 0, 20, 0),
                  child: ProgressBar(
                    value: nUseBandwidth / nTotalBandwidth,
                    label: '',
                  ),
                ),
              ],
            ),
          ),
        */
        ],
      ),
    );
  }
}
