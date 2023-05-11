module.exports = {

    success(data) {
        return JSON.stringify({
            "data": data,
            "code": 0
        });
    },


    /*
    {
      "data": {
        "Error": "AdmConProtoError: JSON Decoding: 'utf8' codec can't decode byte 0xb4 in position 2: unexpected code byte",        //错误详情
        "code": "Client.Ibap.Proto.JSONDecoding",                                                                                   //错误语句
        "text": "JSON Decoding: 'utf8' codec can't decode byte 0xb4 in position 2: unexpected code byte"                            //错误简介
      },
      "code": -1   //错误编号
    }
    */
    //自定义错误对象
    customErrors(code, dataError = "", dataCode = "", dataText = "") {
        return JSON.stringify({
            "data": {
                "Error": dataError,
                "code": dataCode,
                "text": dataText
            },
            "code": code
        });
    },
    // error.js定义错误对象
    errors(errorObj) {
        return JSON.stringify({
            "data": {
                "Error": errorObj.Error,
                "code": errorObj.code,
                "text": errorObj.text
            },
            "code": errorObj.num
        });
    },

    /*
    {
            "Filter": {
                "StartTime": "2023-02-18 00:00:00",
                "EndTime": "",
                "SumCount": 9
            },
            "Header": {
                "CmdCode": "GetShadowStreamSum"
            }
        }
    */
    cfuncInput(funcName, filterObj) {
        return {
            "Data": filterObj,
            "Header": {
                "CmdCode": funcName
            }
        };
    }

}
