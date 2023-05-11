import 'package:flutter/material.dart';

class ProgressBar extends StatelessWidget {
  final double value;
  final String label;
  final Color bgColor;
  final Color valueColor;

  ProgressBar(
      {required this.value,
      required this.label,
      this.bgColor = Colors.greenAccent,
      this.valueColor = Colors.red});

  @override
  Widget build(BuildContext context) {
    return Column(
      children: [
        LinearProgressIndicator(
          backgroundColor: this.bgColor,
          valueColor: AlwaysStoppedAnimation<Color>(this.valueColor),
          value: value,
        ),
        SizedBox(height: 8),
        Container(
          child: label == "" ? null : Text(label),
          color: Colors.yellow,
        )
      ],
    );
  }
}
