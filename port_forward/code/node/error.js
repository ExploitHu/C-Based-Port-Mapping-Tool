const errors = {
    'ERR_INTERNAL_SERVER_ERROR': {
        num: 999,
        Error: '详细',
        code: '报错代码',
        text: '简略信息'
    },
    'ERR_URL_MISS_PARAMETER': {
        num: -1,
        Error: 'JS ERROR:url中缺少参数',
        code: 'if (\'xxxxx\' in query)',
        text: '少参'
    },
};
module.exports = errors;