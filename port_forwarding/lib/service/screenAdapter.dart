import 'package:flutter_screenutil/flutter_screenutil.dart';

//预防官方更改ScreenUtil调用函数导致的代码需要大面积更改
//将其封装之后，如果官方更改类，我们则只需要更新我们自定义类ScreenAdaper
class ScreenAdapter {
  //设置高度
  static height(num value) {
    return ScreenUtil().setHeight(value);
  }

  //设置宽度
  static width(num value) {
    return ScreenUtil().setWidth(value);
  }

  //设置文字大小
  static size(num value) {
    return ScreenUtil().setSp(value);
  }

  //获取物理宽度
  static getScreenWidth() {
    return ScreenUtil().screenWidth;
  }

  //获取物理高度
  static getScreenHeight() {
    return ScreenUtil().screenHeight;
  }
}
