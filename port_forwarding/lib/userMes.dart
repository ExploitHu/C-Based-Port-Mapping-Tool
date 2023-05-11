import 'package:flutter/material.dart';
import 'package:flutter/widgets.dart';
import 'package:port_forwarding/socketDemo.dart';

import 'eventbus/rmsgdata.dart';
import 'http/http.dart';
import 'myWidget/myDialog.dart';

class UseMes extends StatefulWidget {
  final Function() onLogOut; // 接受登录函数的回调
  final String userId;
  const UseMes({super.key, required this.userId, required this.onLogOut});

  @override
  State<UseMes> createState() => _UseMesState();
}

class _UseMesState extends State<UseMes> {
  final _formKey = GlobalKey<FormState>();
  final _oldPasswordController = TextEditingController();
  final _newPasswordController = TextEditingController();
  final _confirmPasswordController = TextEditingController();
  final _nickNameController = TextEditingController();

  void _updateUserPasswd(
      String oldPasswd, String newPasswd1, String newPasswd2) {
    Map msg = {
      "UserId": widget.userId,
      "NewPasswd": newPasswd1,
      "OldPasswd": oldPasswd
    };

    if (newPasswd1 != newPasswd2) {
      ScaffoldMessenger.of(context).showSnackBar(SnackBar(
        content: Text(
          '两次密码不一致',
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

    Post.send(msg, 'UpdateUserPasswd').then((retMap) {
      if (0 == retMap['code']) {
        ScaffoldMessenger.of(context).showSnackBar(SnackBar(
          content: Text(
            '修改密码成功',
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

  Widget _setUserMes() {
    return AlertDialog(
      title: Text(
        '修改用户信息',
        style: TextStyle(
          fontFamily: 'ALI_FONT',
          fontSize: 15,
          color: Colors.black,
        ),
      ),
      content: Container(
        height: 200,
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            const SizedBox(height: 16.0),
            Row(
              children: [
                const Text('原 密 码'),
                const SizedBox(width: 16.0),
                Expanded(
                  child: TextFormField(
                    controller: _oldPasswordController,
                    obscureText: true,
                    decoration: const InputDecoration(
                      hintText: '请输入原密码',
                    ),
                    validator: (value) {
                      if (value!.isEmpty) {
                        return '请输入原密码';
                      }
                      return null;
                    },
                  ),
                ),
              ],
            ),
            const SizedBox(height: 16.0),
            Row(
              children: [
                const Text('新 密 码'),
                const SizedBox(width: 16.0),
                Expanded(
                  child: TextFormField(
                    controller: _newPasswordController,
                    obscureText: true,
                    decoration: const InputDecoration(
                      hintText: '请输入新密码',
                    ),
                    validator: (value) {
                      if (value!.isEmpty) {
                        return '请输入新密码';
                      }
                      return null;
                    },
                  ),
                ),
              ],
            ),
            const SizedBox(height: 16.0),
            Row(
              children: [
                const Text('确认密码'),
                const SizedBox(width: 16.0),
                Expanded(
                  child: TextFormField(
                    controller: _confirmPasswordController,
                    obscureText: true,
                    decoration: const InputDecoration(
                      hintText: '请再次输入新密码',
                    ),
                    validator: (value) {
                      if (value!.isEmpty) {
                        return '请输入确认密码';
                      }
                      if (value != _newPasswordController.text) {
                        return '两次输入的密码不一致';
                      }
                      return null;
                    },
                  ),
                ),
              ],
            ),
          ],
        ),
      ),
      actions: [
        ButtonBar(
          alignment: MainAxisAlignment.spaceBetween,
          children: <Widget>[
            ElevatedButton(
              onPressed: () {
                print("取消修改密码");
                Navigator.pop(context);
              },
              child: Text(
                '取消',
                style: TextStyle(
                  fontFamily: 'ALI_FONT',
                  fontSize: 15,
                  color: Colors.black,
                ),
              ),
            ),
            ElevatedButton(
              onPressed: () {
                print(_confirmPasswordController.text);
                print(_oldPasswordController.text);
                print(_newPasswordController.text);
                Navigator.pop(context);
                _updateUserPasswd(
                    _oldPasswordController.text,
                    _newPasswordController.text,
                    _confirmPasswordController.text);

                //if (_formKey.currentState!.validate()) {
                //  // TODO: 处理密码修改逻辑
                //
                //}
              },
              child: const Text(
                '确定修改',
                style: TextStyle(
                  fontFamily: 'ALI_FONT',
                  fontSize: 15,
                  color: Colors.black,
                ),
              ),
            ),
          ],
        ),
      ],
    );
  }

  Widget _exit() {
    return AlertDialog(
      title: Text(
        '您确认要退出登录吗？',
        style: TextStyle(
          fontFamily: 'ALI_FONT',
          fontSize: 15,
          color: Colors.black,
        ),
      ),
      //content:
      actions: [
        ButtonBar(
          alignment: MainAxisAlignment.spaceBetween,
          children: <Widget>[
            ElevatedButton(
              onPressed: () {
                print(_confirmPasswordController.text);
                print(_oldPasswordController.text);
                print(_newPasswordController.text);
                //if (_formKey.currentState!.validate()) {
                //  // TODO: 处理密码修改逻辑
                //
                //}
                Navigator.pop(context);
                print("取消");
              },
              child: Text(
                '取消',
                style: TextStyle(
                  fontFamily: 'ALI_FONT',
                  fontSize: 15,
                  color: Colors.black,
                ),
              ),
            ),
            ElevatedButton(
              onPressed: () {
                print(_confirmPasswordController.text);
                print(_oldPasswordController.text);
                print(_newPasswordController.text);
                //if (_formKey.currentState!.validate()) {
                //  // TODO: 处理密码修改逻辑
                //
                //}
                Navigator.pop(context);
                print("退出登录");
                widget.onLogOut();
              },
              child: const Text(
                '退出登录',
                style: TextStyle(
                  fontFamily: 'ALI_FONT',
                  fontSize: 15,
                  color: Colors.black,
                ),
              ),
            ),
          ],
        ),
      ],
    );
  }

  @override
  Widget build(BuildContext context) {
    return Container(
      margin: EdgeInsets.fromLTRB(10, 0, 15, 0),
      child: Row(
        mainAxisAlignment: MainAxisAlignment.spaceBetween,
        children: [
          Expanded(
            child: Container(
              child: Text(
                widget.userId,
                style: TextStyle(
                  fontFamily: 'ALI_FONT',
                  fontSize: 15,
                  color: Colors.white,
                ),
              ),
            ),
            flex: 1,
          ),
          IconButton(
            onPressed: () {
              print("object");
              showDialog(
                context: context,
                builder: (BuildContext context) {
                  return _setUserMes();
                },
              );
            },
            icon: Icon(Icons.build),
          ),
          IconButton(
            onPressed: () {
              print("退出登录、切换用户");
              showDialog(
                context: context,
                builder: (BuildContext context) {
                  return _exit();
                },
              );
            },
            icon: Icon(Icons.exit_to_app),
          ),
        ],
      ),
    );
  }
}
