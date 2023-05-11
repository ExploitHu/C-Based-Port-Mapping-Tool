import 'package:flutter/material.dart';

class AliIcon {
  static const _kFontFam = 'ALI_ICON';
  static const String? _kFontPkg = null;
  static const Map _icon = {
    'title': IconData(0xe992, fontFamily: _kFontFam, fontPackage: _kFontPkg),
    'map': IconData(0xe660, fontFamily: _kFontFam, fontPackage: _kFontPkg),
    'statistics':
        IconData(0xe856, fontFamily: _kFontFam, fontPackage: _kFontPkg),
    'mailbox': IconData(0xe68a, fontFamily: _kFontFam, fontPackage: _kFontPkg),
    'userMsg': IconData(0xe6a2, fontFamily: _kFontFam, fontPackage: _kFontPkg),
    'bill': IconData(0xe65c, fontFamily: _kFontFam, fontPackage: _kFontPkg)
  };

  static IconData getIcon(String name) {
    return _icon[name];
  }
}
