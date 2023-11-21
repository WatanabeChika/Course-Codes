function getWebPageName(){
    var path = window.location.pathname;
    var pageName = path.split('/').pop();
    return decodeURIComponent(pageName);
}

function getdata(d, par) {
    let job = getWebPageName();
    let data = new Array();
    let i = 0;
    while (d[i]) {
        if (d[i]['职业'] !== job) {
            ++i;
            continue;
        }
        var obj = {value: d[i][par], name: d[i]['代号']}
        data.push(obj);
        ++i;
    }
    return data;
}

// function get_illustrator() {
//     let data = new Array();
//     let ori = {'Infukun': 9, '下野宏铭': 24, 'NoriZC': 14, 'Liduke': 6, '唯@W': 7, 'alchemaniac': 4, '龙崎いち': 25, '幻象黑兔': 12, 'aZLing4': 8, 'LLC': 5, 'KuroBlood': 5, '温泉瓜': 5, 'Skade': 21, 'Ubisoft': 4, 'Chuzenji': 5, 'LM7': 4, 'm9nokuro': 10, 'IRIS_呓': 5, '一千': 4, 'STAR影法师': 3, 'Anmi': 2, '将': 3, 'カワグチ': 2, 
//                 'Namie': 2, 'Jacknife': 3, 'TOKI': 8, 'YUJI': 3, '熊太': 2, 'LAL!ROLE': 2, '〇亻': 2, 'Cenm0': 3, 'れおえん': 2, '我妻洛酱': 4, '伍秋秋秋秋': 3, 'umo_yszx': 2, '九日九号': 2, '板板': 2, 'アシマ': 2, '阿鬼': 5, 'HUG': 2, 'Lanzi': 2, '织布机loom': 2, '一立里子': 5, '其他': 49};
//     key = Object.keys(ori);
//     values = Object.values(ori);
//     let i = 0;
//     while (key[i]) {
//         var obj = {value: values[i], name: key[i]}
//         data.push(obj);
//         ++i;
//     }
//     return data;
// }

function judge_value_toint(h) {
    let i = 0;
    while (h[i]) {
        let j = parseInt(h[i].value);
        if (j !== j) {
            h[i].value = 0;
        }
        else {
            h[i].value = j;
        }
        ++i;
    }
    return h;
}

function judge_value_todate(d) {
    let i = 0;
    while (d[i]) {
        var j = String(d[i].value);
        var year = j.indexOf('年') == -1 ? '2023' : j.indexOf('年');
        var month = j.indexOf('月') == -1 ? '0' : j.slice(j.indexOf('年')+1, j.indexOf('月'));
        var day = j.slice(j.indexOf('月')+1, j.indexOf('日'));
        d[i].value = month == '0' ? '未知' : year+'-'+month+'-'+day
        ++i;
    }
    var dataset = new Array();
    i = 0;
    while (d[i]) {
        var exist = false;
        for (let j = 0; j < dataset.length; ++j) {
            if (dataset[j][0] == d[i].value) {
                exist = true;
                break;
            }
        }
        if (!exist) {
            dataset.push([d[i].value, []]);
        }
        ++i;
    }
    i = 0;
    while (d[i]) {
        for (let j = 0; j < dataset.length; ++j) {
            if (dataset[j][0] == d[i].value) {
                var index = j;
                break;
            }
        }
        dataset[index][1].push(d[i].name);
        ++i;
    }
    return dataset;
}

function extract_string(h) {
    let i = 0;
    while (h[i]) {
        const slashIndex = h[i].value.indexOf('/');

        if (slashIndex !== -1) {
            h[i].value = h[i].value.substring(slashIndex + 1);
        } 
        ++i;
    }
    return h;  
}

// High to low
function data_sort(d) {
    let res = d.sort(function(a,b){
        return parseInt(b.value) - parseInt(a.value);
    })
    return res;
}

$.getJSON('/static/ArkOperators.json').done(function(data) {
    echarts.init(document.getElementById('height')).setOption({
        title: {
            x: 'left',
            y: 'center',
            text: '身高'
        },
        tooltip: {
            trigger: 'axis',
        },
        xAxis: [{
            type: 'category',
            data: data_sort(judge_value_toint(getdata(data, '身高'))).map(item => item.name),
            axisLabel: {
                rotate: 30
            }
        }],
        yAxis: [{
            axisLabel: {
                formatter: x => x+'cm'
            }
        }],
        dataZoom: [{
            top: 'top',
            start: 0,
            end: 50
        }],
        series: [{
            colorBy: 'data',
            showBackground: true,
            large: true,
            type: 'bar',
            data: data_sort(judge_value_toint(getdata(data, '身高'))).map(item => item.value),
            label: {
                show: true,
                position: 'top',
                formatter: function(params) {
                    if (params.value == 0) return '未知';
                    else return params.value
                }
            }
        }]
    })
    echarts.init(document.getElementById('health')).setOption({
        title: {
            x: 'left',
            y: 'center',
            text: '满级生命'
        },
        tooltip: {
            trigger: 'axis',
        },
        xAxis: [{
            type: 'category',
            data: data_sort(judge_value_toint(extract_string(getdata(data, '生命上限')))).map(item => item.name),
            axisLabel: {
                rotate: 30
            }
        }],
        yAxis: [{
            axisLabel: {
                formatter: x => x
            }
        }],
        dataZoom: [{
            top: 'top',
            start: 0,
            end: 50
        }],
        series: [{
            colorBy: 'data',
            showBackground: true,
            large: true,
            type: 'bar',
            data: data_sort(judge_value_toint(extract_string(getdata(data, '生命上限')))).map(item => item.value),
            label: {
                show: true,
                position: 'top',
                formatter: function(params) {
                    if (params.value == 0) return '未知';
                    else return params.value
                }
            }
        }]
    })
    echarts.init(document.getElementById('attack')).setOption({
        title: {
            x: 'left',
            y: 'center',
            text: '攻击'
        },
        tooltip: {
            trigger: 'axis',
        },
        xAxis: [{
            type: 'category',
            data: data_sort(judge_value_toint(extract_string(getdata(data, '攻击')))).map(item => item.name),
            axisLabel: {
                rotate: 30
            }
        }],
        yAxis: [{
            axisLabel: {
                formatter: x => x
            }
        }],
        dataZoom: [{
            top: 'top',
            start: 0,
            end: 50
        }],
        series: [{
            colorBy: 'data',
            showBackground: true,
            large: true,
            type: 'bar',
            data: data_sort(judge_value_toint(extract_string(getdata(data, '攻击')))).map(item => item.value),
            label: {
                show: true,
                position: 'top',
                formatter: function(params) {
                    if (params.value == 0) return '未知';
                    else return params.value
                }
            }
        }]
    })
    echarts.init(document.getElementById('defense')).setOption({
        title: {
            x: 'left',
            y: 'center',
            text: '防御/法抗'
        },
        tooltip: {
            trigger: 'axis',
        },
        xAxis: [{
            type: 'category',
            data: data_sort(judge_value_toint(extract_string(getdata(data, '防御')))).map(item => item.name),
            axisLabel: {
                rotate: 30
            }
        }],
        yAxis: [{
            position: 'left',
            axisLabel: {
                formatter: x => x
            }
        },
        {
            position: 'right',
            axisLabel: {
                formatter: x => x
            }
        }],
        dataZoom: [{
            top: 'top',
            start: 0,
            end: 50
        }],
        series: [{
            yAxisIndex: 0,
            colorBy: 'data',
            showBackground: true,
            large: true,
            type: 'bar',
            data: data_sort(judge_value_toint(extract_string(getdata(data, '防御')))).map(item => item.value),
            label: {
                show: true,
                position: 'top',
                formatter: function(params) {
                    if (params.value == 0) return '0';
                    else return params.value
                }
            }
        },
        {
            yAxisIndex: 1,
            colorBy: 'data',
            showBackground: true,
            large: true,
            type: 'bar',
            data: data_sort(judge_value_toint(extract_string(getdata(data, '法术抗性')))).map(item => item.value),
            label: {
                show: true,
                position: 'top',
                formatter: function(params) {
                    if (params.value == 0) return '0';
                    else return params.value
                }
            }
        }]
    })
    echarts.init(document.getElementById('birth')).setOption({
        title: {
            x: 'center',
            y: 'top',
            text: '生日'
        },
        tooltip: {
            formatter: function(params) {
               var str = params.value[0].slice(5) + '\n\n';
               let i = 0;
               while (params.value[1][i]) {
                str = str + params.value[1][i] + '\n\n';
                ++i;
               }
               return str;
            }
        },
        calendar: [{
            left: 'center',
            top: 'middle',
            cellSize: ['auto', 30],
            dayLabel: { show: false },
            yearLabel: {
                position: 'top',
                
            },
            range: 2023,
            itemStyle: {
                color: 'rgba(166, 239, 244, 1)',
            }
        }],
        emphasis: {
            focus: 'self',
            label: {
                fontWeight: 'bolder',
                fontSize: 20,
                color: 'rgba(167, 15, 222, 1)'
            }
        },
        series: [{
            type: 'scatter',
            coordinateSystem: 'calendar',
            symbolSize: 0,
            label: {
                show: true,
                formatter: function (params) {
                    var d = echarts.number.parseDate(params.value[0]);
                    return d.getDate();
                },
                color: '#000'
            },
            data: judge_value_todate(getdata(data, '生日'))
        }]
    })
    // echarts.init(document.getElementById('illustrator')).setOption({
    //     title: {
    //         x: 'center',
    //         y: 'top',
    //         text: '画师'
    //     },
    //     tooltip: {
    //         formatter: function (params) {
    //             return params.name + '<br/>' + params.value;
    //         }
    //     },
    //     series: [{
    //         type: 'pie',
    //         radius: '75%',
    //         center: ['50%', '50%'],
    //         data: get_illustrator(data, '画师').map(item => {
    //             return {
    //                 name: item.name,
    //                 value: item.value
    //             };
    //         }),
    //         label: {
    //             show: true,
    //             formatter: '{b}: {c}',
    //             color: '#000',
    //             fontSize: 12
    //         },
    //         emphasis: {
    //             scale: true,
    //             label: {
    //                 show: true,
    //                 fontSize: 16,
    //                 fontWeight: 'bold',
    //                 color: 'rgba(167, 15, 222, 1)'
    //             }
    //         },
    //     }]
    // })    
})