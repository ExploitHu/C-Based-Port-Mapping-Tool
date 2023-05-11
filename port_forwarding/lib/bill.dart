import 'package:flutter/material.dart';
import 'package:intl/intl.dart';
import 'http/http.dart';

class Bill extends StatefulWidget {
  final String userId;
  const Bill({super.key, required this.userId});
  @override
  _BillState createState() => _BillState();
}

class _BillState extends State<Bill> {
  List items = [];
  /*[
    {
      "Bill": {
        "BillId": 10000009,
        "Money": 100,
        "CreatedTime": 1683090629,
        "BillType": 0
      },
      "Res": []
    },
    {
      "Bill": {
        "BillId": 10000010,
        "Money": 100,
        "CreatedTime": 1683090711,
        "BillType": 0
      },
      "Res": []
    },
    {
      "Bill": {
        "BillId": 10000011,
        "Money": 100,
        "CreatedTime": 1683090747,
        "BillType": 0
      },
      "Res": []
    },
    {
      "Bill": {
        "BillId": 10000012,
        "Money": 100,
        "CreatedTime": 1683090852,
        "BillType": 0
      },
      "Res": []
    },
    {
      "Bill": {
        "BillId": 10000013,
        "Money": 100,
        "CreatedTime": 1683090869,
        "BillType": 0
      },
      "Res": []
    },
    {
      "Bill": {
        "BillId": 10000014,
        "Money": 20,
        "CreatedTime": 1683094756,
        "BillType": 1
      },
      "Res": []
    },
    {
      "Bill": {
        "BillId": 10000015,
        "Money": 20,
        "CreatedTime": 1683094764,
        "BillType": 1
      },
      "Res": []
    },
    {
      "Bill": {
        "BillId": 10000016,
        "Money": 10,
        "CreatedTime": 1683095019,
        "BillType": 1
      },
      "Res": []
    },
    {
      "Bill": {
        "BillId": 10000017,
        "Money": 10,
        "CreatedTime": 1683095025,
        "BillType": 1
      },
      "Res": []
    },
    {
      "Bill": {
        "BillId": 10000018,
        "Money": 10,
        "CreatedTime": 1683095054,
        "BillType": 1
      },
      "Res": []
    },
    {
      "Bill": {
        "BillId": 10000023,
        "Money": 10,
        "CreatedTime": 1683097059,
        "BillType": 1
      },
      "Res": [
        {"Money": 0, "EndTime": 1683183459},
        {"Money": 10, "EndTime": 1683183459}
      ]
    },
    {
      "Bill": {
        "BillId": 10000024,
        "Money": 10,
        "CreatedTime": 1683117605,
        "BillType": 1
      },
      "Res": [
        {"Money": 10, "EndTime": 1683204005}
      ]
    },
    {
      "Bill": {
        "BillId": 10000025,
        "Money": 10,
        "CreatedTime": 1683117982,
        "BillType": 1
      },
      "Res": [
        {"Money": 10, "EndTime": 1683204382}
      ]
    },
    {
      "Bill": {
        "BillId": 10000027,
        "Money": 10,
        "CreatedTime": 1683128613,
        "BillType": 1
      },
      "Res": [
        {"Money": 10, "EndTime": 1683215013}
      ]
    },
    {
      "Bill": {
        "BillId": 10000028,
        "Money": 30,
        "CreatedTime": 1683177444,
        "BillType": 1
      },
      "Res": [
        {"Money": 10, "EndTime": 1683263844},
        {"Money": 10, "EndTime": 1683263844},
        {"Money": 10, "EndTime": 1683263844}
      ]
    }
  ];*/
  @override
  void initState() {
    super.initState();

    Map msg = {"UserId": widget.userId};
    Post.send(msg, 'GetBill').then((retMap) {
      if (0 == retMap['code']) {
        setState(() {
          items = [];
          for (var i = 0; i < retMap['data'].length; i++) {
            items = retMap['data'];
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

  String MyDateFormat(timestamp) {
    print(timestamp);

    //timestamp = 1620086400000; // 2021-05-04 00:00:00
    DateTime date = DateTime.fromMillisecondsSinceEpoch(timestamp);
    String formattedDate = DateFormat('yyyy-MM-dd hh:mm:ss').format(date);

    return formattedDate;
  }

  @override
  Widget build(BuildContext context) {
    return Container(
      color: Color.fromARGB(255, 230, 230, 230),
      child: ListView.builder(
        itemCount: items.length,
        itemBuilder: (BuildContext context, int index) {
          return Card(
            child: Container(
              padding: EdgeInsets.fromLTRB(10, 10, 10, 10),
              color: Color.fromARGB(255, 148, 156, 147),
              //child: ListTile(
              //  title: Text(items[index]["Bill"]["CreatedTime"].toString()),
              //),
              child: Column(
                mainAxisAlignment: MainAxisAlignment.start,
                crossAxisAlignment: CrossAxisAlignment.start,
                children: [
                  Container(
                    child: Text("账单ID:" +
                        items[index]["Bill"]["CreatedTime"].toString()),
                  ),
                  Container(
                    child: Row(
                      mainAxisAlignment: MainAxisAlignment.start,
                      children: [
                        Container(
                          width: 150,
                          child: Text(
                              "金额：" + items[index]["Bill"]["Money"].toString()),
                        ),
                        Container(
                          width: 300,
                          child: Text("创建时间：" +
                              MyDateFormat(
                                  items[index]["Bill"]["CreatedTime"] * 1000)),
                        ),
                        Container(
                          width: 150,
                          child: Text("类型：" +
                              (0 == items[index]["Bill"]["BillType"]
                                  ? "充值"
                                  : "消费")),
                        ),
                      ],
                    ),
                  ),
                ],
              ),
            ),
          );
        },
      ),
    );
  }
}
