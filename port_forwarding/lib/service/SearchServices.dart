import 'dart:convert';
import 'Storage.dart';

class SearchServices {
  static setSearchData(keywords) async {
    //获取本地存储里面的数据

    //判断本地存储是否有数据
    //如果有数据 读取本地存储的数据 判断本地存储中是否有输入的数据
    //有-》更新本地数据
    //没有-》return

    //本地没有数据-》直接写入

    //读取本地数据
    String? searchList = await Storage.getString('searchList');
    //本地数据不为空
    if (searchList != null) {
      //将本地数据转化为List类型
      List searchListData = json.decode(searchList);
      //遍历本地数据 检查是否存在keywords
      var hasData = searchListData.any((v) {
        return v == keywords;
      });
      //本地数据不存在keywords
      if (!hasData) {
        print(keywords);
        //在数据中加入keywords
        searchListData.add(keywords);
        //将数据写回本地
        await Storage.setString('searchList', json.encode(searchListData));
      }
      //测试
      print(searchListData);
    }
    //本地数据为空
    else {
      List tempList = [];
      tempList.add(keywords);
      //将数据写回本地
      await Storage.setString('searchList', json.encode(tempList));
      //测试
      print(tempList);
    }
  }

  static getHistoryList() async {
    String? searchList = await Storage.getString('searchList');
    if (searchList != null) {
      List searchListData = json.decode(searchList);

      return searchListData;
    }
    return [];
  }

  static clearHistoryList() async {
    await Storage.remove('searchList');
  }

  static removeHistoryData(keywords) async {
    //注意：新版shared_preferences的可空类型
    String? searchList = await Storage.getString('searchList');
    if (searchList != null) {
      List searchListData = json.decode(searchList);
      searchListData.remove(keywords);
      await Storage.setString('searchList', json.encode(searchListData));
    }
  }
}
