// 单例类的关联模块
import 'package:event_bus/event_bus.dart';

class Rmsgdata {
  final String data;

  Rmsgdata(this.data);
}

EventBus eventBus = EventBus();
