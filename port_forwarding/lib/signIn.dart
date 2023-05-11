import 'package:flutter/material.dart';

import 'http/http.dart';

class SignIn extends StatefulWidget {
  final Function() onLogin; // 接受登录函数的回调
  SignIn({required this.onLogin});
  @override
  _SignInState createState() => _SignInState();
}

class _SignInState extends State<SignIn> {
  final GlobalKey<FormState> _formKey = GlobalKey<FormState>();
  TextEditingController _usernameController = TextEditingController();
  TextEditingController _passwordController = TextEditingController();

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
              TextFormField(
                controller: _usernameController,
                decoration: InputDecoration(
                  labelText: '电话或电子邮箱',
                  border: OutlineInputBorder(),
                ),
                validator: (value) {
                  if (value!.isEmpty) {
                    return 'Please enter your username.';
                  }
                  return null;
                },
              ),
              SizedBox(height: 16.0),
              TextFormField(
                controller: _passwordController,
                obscureText: true,
                decoration: InputDecoration(
                  labelText: '密码',
                  border: OutlineInputBorder(),
                ),
                validator: (value) {
                  if (value!.isEmpty) {
                    return 'Please enter your password.';
                  }
                  if (value.length < 6) {
                    return 'Password must be at least 6 characters long.';
                  }
                  return null;
                },
              ),
              SizedBox(height: 16.0),
              Container(
                child: Row(
                  crossAxisAlignment: CrossAxisAlignment.center,
                  mainAxisAlignment: MainAxisAlignment.center,
                  children: [
                    ElevatedButton(
                      onPressed: () async {
                        if (_formKey.currentState!.validate()) {
                          // 注册代码
                          String username = _usernameController.text;
                          String password = _passwordController.text;
                          // 这里可以调用注册接口
                          Map sigInMsg = _usernameController.text.contains('@')
                              ? {
                                  "Email": _usernameController.text,
                                  "Passwd": _passwordController.text
                                }
                              : {
                                  "Phone": _usernameController.text,
                                  "Passwd": _passwordController.text
                                };
                          Map httpPost = await Post.send(sigInMsg, 'SignIn');
                          if (0 == httpPost['code']) {
                            print("登录");

                            // Login success
                            showDialog(
                              context: context,
                              builder: (context) => AlertDialog(
                                title: Text('成功'),
                                content: Text('注册成功！'),
                                actions: <Widget>[
                                  TextButton(
                                    onPressed: () {
                                      Navigator.pop(context);
                                      widget.onLogin();
                                      print("确定");
                                    },
                                    child: Text(
                                      '去登陆',
                                      style: TextStyle(
                                        fontFamily: 'ALI_FONT',
                                      ),
                                    ),
                                  ),
                                ],
                              ),
                            );
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
                        }
                      },
                      child: Text(
                        '注册',
                        style: TextStyle(
                          fontFamily: 'ALI_FONT',
                          fontSize: 20,
                        ),
                      ),
                    ),
                    SizedBox(width: 40.0),
                    ElevatedButton(
                      onPressed: () {
                        widget.onLogin();
                      },
                      child: Text(
                        '去登录',
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
}
