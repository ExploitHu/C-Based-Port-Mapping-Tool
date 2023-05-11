AlertDialog(
                                      title: Text('购买端口'),
                                      content: Column(
                                        mainAxisSize: MainAxisSize.min,
                                        children: [
                                          Row(
                                            mainAxisAlignment:
                                                MainAxisAlignment.spaceBetween,
                                            children: [
                                              Text('端口数量：'),
                                              Row(
                                                children: [
                                                  IconButton(
                                                    icon: Icon(Icons.remove),
                                                    onPressed: () {
                                                      setState(() {
                                                        decrementPortNumber();
                                                      });
                                                    },
                                                  ),
                                                  Text(portNumber.toString()),
                                                  IconButton(
                                                    icon: Icon(Icons.add),
                                                    onPressed:
                                                        incrementPortNumber,
                                                  ),
                                                ],
                                              ),
                                            ],
                                          ),
                                          SizedBox(height: 16),
                                          Text('总价：\$$totalPrice'),
                                        ],
                                      ),
                                      actions: [
                                        TextButton(
                                          onPressed: () =>
                                              Navigator.of(context).pop(),
                                          child: Text('取消'),
                                        ),
                                        ElevatedButton(
                                          onPressed: () {
                                            // 将购买端口数量提交到服务器进行购买
                                            // ...
                                            Navigator.of(context).pop();
                                          },
                                          child: Text('购买'),
                                        ),
                                      ],
                                    ));
                          