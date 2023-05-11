import 'package:flutter/material.dart';

class MyDialog extends StatelessWidget {
  final String title;
  final String message;
  final String positiveButtonText;
  final String negativeButtonText;
  final Function()? onPositivePressed;
  final Function()? onNegativePressed;

  MyDialog({
    required this.title,
    required this.message,
    required this.positiveButtonText,
    required this.negativeButtonText,
    this.onPositivePressed,
    this.onNegativePressed,
  });

  @override
  Widget build(BuildContext context) {
    return AlertDialog(
      title: Text(title),
      content: Text(message),
      actions: [
        TextButton(
          onPressed: () {
            onNegativePressed;
            Navigator.pop(context);
          },
          child: Text(negativeButtonText),
        ),
        TextButton(
          onPressed: onPositivePressed ?? () => Navigator.pop(context),
          child: Text(positiveButtonText),
        ),
      ],
    );
  }
}
