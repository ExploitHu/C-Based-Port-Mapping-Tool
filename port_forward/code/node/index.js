const http = require('http')
const querystring = require('querystring');
const ffi = require('ffi');
const ref = require('ref')
const express = require('express');
const app = express();
const jsonObj = require('./jsonObj.js')
const errors = require('./error');
const { log } = require('console');
const { json } = require('body-parser');

const dllpath = '/port_forward/code/node/lib/libui.so'

function sleep(ms) {
    return new Promise(resolve=>setTimeout(resolve, ms))
}



var mylib = ffi.Library(dllpath, {
    //'InitUIFunc': ['int', ['int']],
    "ExecUI": ['int', ['string', 'string', 'char **', 'int *']],
    //"ReconnectDB": ['int', []],
    "MyFreeMemory": ['int', ['char *']]
});

function dealCfunction(res, method, query, bodyJson){
    
    let funcName;
    if ('funcName' in query) funcName = query.funcName;
    else return jsonObj.errors(errors.ERR_URL_MISS_PARAMETER);

    let outBuf = ref.alloc('char *');
    let outLen = ref.alloc('int');

    console.debug('----------------------------');
    console.debug('query',query);
    console.debug('bodyJson',bodyJson);
    console.debug('funcName',funcName);
    console.debug('----------------------------');

    let ret = mylib.ExecUI(funcName, JSON.stringify(bodyJson), outBuf, outLen);
    console.debug("退出--ExecUI***********************************************************************************************************")


/*  switch(method){
        case "GET":
            res.write(jsonObj.customErrors(-2,"内部错误",-2,"内部错误"));
            break;
        case "POST":
            res.write(jsonObj.customErrors(-2,"内部错误",-2,"内部错误"));
            break;
        case "PUT":
            res.write(jsonObj.customErrors(-2,"内部错误",-2,"内部错误"));
            break;
        case "DELETE":
            res.write(jsonObj.customErrors(-2,"内部错误",-2,"内部错误"));
            break;
        
        
        default:
            res.write(jsonObj.customErrors(404,"404!请求不存在!","","请求错误"));
            break;
    }*/

    let outStr = ref.readPointer(outBuf, 0, outLen.deref());
    let outJson = JSON.parse(outStr.toString());

    console.debug('-------------outJson----------------');
    console.debug(outJson);
    console.debug('---------------------------------');

    if(0 != ret){
        return jsonObj.customErrors(outJson.Status.StatusCode,"函数调用出错", funcName, outJson.Status.Msg2Client);
    }

    mylib.MyFreeMemory(outStr)
    return jsonObj.success(outJson.Data != null ? outJson.Data : {})
}

const server = http.createServer(async (req, res)=> {
    console.log('收到请求！');

    let body = '';
    req.on('data', (chunk) => {
        body += chunk;
    });
    
    req.on('end', () => {
        console.debug('请求体：', body=='');
        
        let bodyJson = body!=''?JSON.parse(body):'';
        let method = req.method;                          // POST | DELETE | PUT | GET 
        let url = req.url;                                //  /aaaa?cfunc=functionName&asc=11
        let path = url.split('?')[0];                     //  /aaaa
        let query = querystring.parse(url.split('?')[1]); //  { cfunc: 'functionName', asc: '11' }
        console.log('method',method);
        console.log('url',url);
        console.log('path', path);
        console.log('query', query);
        console.log('bodyJson', bodyJson);

        switch (path) {
            case '/cfunc' :
                res.writeHead(200, {'Content-Type': 'application/json'});
                res.write(dealCfunction(res, method, query, bodyJson));
                break;
        
            default:
                res.writeHead(404, {'Content-Type': 'application/json'});
                res.write(jsonObj.customErrors(404,"404!请求不存在!","","请求错误"));
                break;
        }
        
        res.end();
    });
});



server.listen(20010, ()=>{
    console.log('server running at port 20010');
})