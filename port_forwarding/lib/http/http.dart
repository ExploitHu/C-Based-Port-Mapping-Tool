import 'dart:async';
import 'dart:convert';
import 'package:http/http.dart' as http;
import 'dart:io';

class Post {
  static Future<Map> send(Map requestBody, String funcName) async {
    String url = 'http://192.168.32.130:20010/cfunc?funcName=' + funcName;
    // 构建请求体
    //requestBody = {"Phone": "12345678911", "Passwd": "aabbcc"};

    // 创建一个http.Client对象
    var client = http.Client();
    var responseBody;
    try {
      // 构造POST请求
      var request = http.Request('POST', Uri.parse(url));
      request.headers['Content-Type'] = 'application/json';
      request.body = json.encode(requestBody);
      print("发送请求-----");
      print(request.body);
      print("------------");
      // 发送POST请求，并设置超时时间为5秒
      var response = await client.send(request).timeout(Duration(seconds: 5));

      // 确定响应的编码格式
      var contentType = response.headers['content-type'];
      Encoding charset = utf8;

      if (contentType != null) {
        var parts = contentType.split(';');
        if (parts.length > 1) {
          var charsetPart = parts[1].trim();
          if (charsetPart.startsWith('charset=')) {
            var charsetName = charsetPart.substring('charset='.length);
            charset = Encoding.getByName(charsetName) ?? utf8;
          }
        }
      }

      // 解码响应体
      responseBody = await response.stream.bytesToString();
    } on TimeoutException catch (e) {
      return {"code": -1};
    } catch (e) {
      return {"code": -2};
    } finally {
      // 关闭http.Client对象
      print(responseBody);
      client.close();
      return json.decode(responseBody);
    }
  }
}
