import 'package:flutter/material.dart';
import 'package:bitsdojo_window/bitsdojo_window.dart';
import 'package:port_forwarding/res/aliIcon.dart';
import 'body.dart';
import 'login.dart';
import 'service/screenAdapter.dart';
import 'package:flutter_screenutil/flutter_screenutil.dart';

import 'signIn.dart';

void main() async {
  // Add this line
  await ScreenUtil.ensureScreenSize();
  runApp(MyApp());

  doWhenWindowReady(() {
    var initialSize =
        Size(ScreenAdapter.width(1700), ScreenAdapter.height(950));
    appWindow.minSize = initialSize;
    appWindow.size = initialSize;
    appWindow.alignment = Alignment.center;
    appWindow.show();
  });
}

class MyApp extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      debugShowCheckedModeBanner: false,
      title: 'Flutter_ScreenUtil',
      theme: ThemeData(
        primarySwatch: Colors.blue,
      ),
      home: HomePage(title: 'FlutterScreenUtil Demo'),
    );
  }
}

class HomePage extends StatefulWidget {
  const HomePage({Key? key, required this.title}) : super(key: key);

  final String title;

  @override
  _HomePageState createState() => _HomePageState();
}

class _HomePageState extends State<HomePage> {
  //static var titleColor = Colors.yellow;
  var _loginStatus = false;
  var _loginSigInPage = 'login';
  late String _userId;

  //var loginStatus = false;

  void _loginSuccess() {
    print("登录成功");
    setState(() {
      _loginStatus = true;
    });
  }

  void _switchLoginPage() {
    setState(() {
      _loginStatus = false;
      _loginSigInPage = 'login';
    });
  }

  void _switchSigInPage() {
    setState(() {
      _loginSigInPage = 'sigIn';
    });
  }

  void _setUserId(String userId) {
    this._userId = userId;
  }

  @override
  void initState() {
    super.initState();
    //this._userId = "10001045";
    //this._loginStatus = true;
  }

  @override
  Widget build(BuildContext context) {
    //Set the fit size (fill in the screen size of the device in the design)
    //If the design is based on the size of the 360*690(dp)
    ScreenUtil.init(context, designSize: const Size(1920, 1080));
    return Scaffold(
      body: Column(
        children: _loginStatus
            ? [
                WindowTitleBarBox(
                  //color: Colors.blue,
                  child: Row(
                    mainAxisAlignment: MainAxisAlignment.end,
                    children: [
                      Expanded(
                        flex: 1,
                        child: MoveWindow(
                            child: Container(
                                padding: EdgeInsets.only(
                                    left: ScreenAdapter.width(10)),
                                child: Row(children: [
                                  Icon(AliIcon.getIcon('title')),
                                  Text("基于C++的端口映射工具")
                                ]),
                                color: Colors.white)),
                      ),
                      MinimizeWindowButton(),
                      MaximizeWindowButton(),
                      CloseWindowButton(),
                    ],
                  ),
                ),
                Expanded(
                  child: Body(
                    userId: _userId,
                    onLogOut: _switchLoginPage,
                  ),
                  flex: 1,
                ),
              ]
            : [
                WindowTitleBarBox(
                  //color: Colors.blue,
                  child: Row(
                    mainAxisAlignment: MainAxisAlignment.end,
                    children: [
                      Expanded(
                        flex: 1,
                        child: MoveWindow(
                            child: Container(
                                padding: EdgeInsets.only(
                                    left: ScreenAdapter.width(10)),
                                child: Row(children: [
                                  Icon(AliIcon.getIcon('title')),
                                  Text("基于C++的端口映射工具")
                                ]),
                                color: Colors.white)),
                      ),
                      MinimizeWindowButton(),
                      MaximizeWindowButton(),
                      CloseWindowButton(),
                    ],
                  ),
                ),
                Expanded(
                  //child: SignIn(),
                  child: _loginSigInPage == 'login'
                      ? Login(
                          onSignIn: _switchSigInPage,
                          onLoginSuccess: _loginSuccess,
                          onSetUserId: _setUserId,
                        )
                      : SignIn(
                          onLogin: _switchLoginPage,
                        ),
                  flex: 1,
                ),
              ],
      ),
    );
  }
}
