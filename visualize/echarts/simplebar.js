require.config({paths: {echarts:'http://echarts.baidu.com/build/dist'}});
require(['echarts', 'echarts/chart/bar'],
        function (ec) {
            var mychart = ec.init(document.getElementById('main'));
            var option = {
                tooltip: {show:true},
                legend: {data:["sales"]},
                xAxis: [{type: 'category', data: ["A", "B", "C"] }],
                yAxis: [{type: 'value'}],
                series: [{"name":"num", "type":"bar",
                    "data": [5, 20, 40]}]
            };
            mychart.setOption(option);
        }
       );

