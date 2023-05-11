import 'package:flutter/material.dart';

import 'http/http.dart';

class Login extends StatefulWidget {
  final Function() onSignIn; // 接受登录函数的回调
  final Function() onLoginSuccess; // 接受登录函数的回调
  final Function(String) onSetUserId; // 接受设置UserId的回调
  Login(
      {required this.onSignIn,
      required this.onLoginSuccess,
      required this.onSetUserId});
  @override
  _LoginState createState() => _LoginState();
}

class _LoginState extends State<Login> {
  final _formKey = GlobalKey<FormState>();
  final _usernameController = TextEditingController();
  final _passwordController = TextEditingController();
  bool _isLoading = false;

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      body: Container(
        //width: 400,
        padding: EdgeInsets.fromLTRB(300, 0, 300, 0),
        //alignment: Alignment.bottomRight,
        color: Color.fromARGB(255, 110, 190, 190),
        child: Form(
          key: _formKey,
          child: Column(
            mainAxisAlignment: MainAxisAlignment.center,
            children: <Widget>[
              Padding(
                padding: EdgeInsets.symmetric(horizontal: 16),
                child: TextFormField(
                  controller: _usernameController,
                  decoration: InputDecoration(
                    labelText: '电话号码或电子邮箱',
                    border: OutlineInputBorder(),
                  ),
                  validator: (value) {
                    if (value == null || value.isEmpty) {
                      return 'Please enter username';
                    }
                    return null;
                  },
                ),
              ),
              SizedBox(height: 16.0),
              Padding(
                padding: EdgeInsets.symmetric(horizontal: 16),
                child: TextFormField(
                  controller: _passwordController,
                  obscureText: true,
                  decoration: InputDecoration(
                    labelText: '密码',
                    border: OutlineInputBorder(),
                  ),
                  validator: (value) {
                    if (value == null || value.isEmpty) {
                      return 'Please enter password';
                    }
                    return null;
                  },
                ),
              ),
              SizedBox(height: 16),
              Container(
                child: Row(
                  crossAxisAlignment: CrossAxisAlignment.center,
                  mainAxisAlignment: MainAxisAlignment.center,
                  children: [
                    ElevatedButton(
                      onPressed: _isLoading ? null : _handleLogin,
                      child: _isLoading
                          ? CircularProgressIndicator()
                          : Text(
                              '登录',
                              style: TextStyle(
                                fontFamily: 'ALI_FONT',
                                fontSize: 20,
                              ),
                            ),
                    ),
                    SizedBox(width: 40.0),
                    ElevatedButton(
                      onPressed: () {
                        widget.onSignIn();
                      },
                      child: Text(
                        '去注册',
                        style: TextStyle(
                          fontFamily: 'ALI_FONT',
                          fontSize: 20,
                        ),
                      ),
                    ),
                  ],
                ),
              ),
            ],
          ),
        ),
      ),
    );
  }

  void _handleLogin() async {
    if (_formKey.currentState!.validate()) {
      setState(() {
        _isLoading = true;
      });

      // Simulate login process
      //await Future.delayed(Duration(seconds: 2));
      var map = _usernameController.text.contains('@')
          ? {
              "Email": _usernameController.text,
              "Passwd": _passwordController.text
            }
          : {
              "Phone": _usernameController.text,
              "Passwd": _passwordController.text
            };
      print(map);
      Map httpPost = await Post.send(map, 'Login');
      if (0 == httpPost['code']) {
        // Login success
        print("登录");

        widget.onSetUserId(httpPost['data']['Id'].toString());
        widget.onLoginSuccess();
      } else {
        // Login failed
        showDialog(
          context: context,
          builder: (context) => AlertDialog(
            title: Text('错误'),
            content: Text('账号或密码错误'),
            actions: <Widget>[
              TextButton(
                onPressed: () => Navigator.pop(context),
                child: Text('确定'),
              ),
            ],
          ),
        );
      }

      setState(() {
        _isLoading = false;
      });
    }

    print(_usernameController);
    print(_passwordController);
  }
}
