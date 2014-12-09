<!DOCTYPE html>
<head>
    <meta charset="utf-8">
    <link rel="stylesheet" href="sitemap.css" type="text/css">
    <title>sitemap</title>
</head>

<body>
    <!-- <div id="main" style="width:1300px; height:650px"></div> -->
    <div id="main" style="width:100%; height:100%"></div>
    <script src="http://echarts.baidu.com/build/dist/echarts.js"></script>
    <script type="text/javascript">
require.config({paths: {echarts:'http://echarts.baidu.com/build/dist'}});
require(['echarts', 'echarts/chart/force'],
        function (ec) {
            var mychart = ec.init(document.getElementById('main'));
var option = {
    animation: false,
    title : {
        text: 'sitemap',
        subtext: 'liuyang1\'s wiki',
        x:'right',
        y:'bottom'
    },
    tooltip : {
        trigger: 'item',
        formatter: '{a} : {b}'
    },
    toolbox: {
        show: true,
        feature: {
            restore: {show: true},
            saveAsImage: {show: true},
            // dataZoom: {show: true,}
        }
    },
    series : [
        {
            type:'force',
            name : "sitemap",
            ribbonType: false,
            itemStyle: {
                normal: {
                    label: {
                        show: true,
                        textStyle: {
                            color: '#333'
                        }
                    },
                    nodeStyle : {
                        brushType : 'both',
                        borderColor : 'rgba(255,215,0,0.4)',
                        borderWidth : 1
                    },
                    linkStyle: {
                        type: 'curve'
                    }
                },
                emphasis: {
                    label: {
                        show: false
                        // textStyle: null      // 默认使用全局文本样式，详见TEXTSTYLE
                    },
                    nodeStyle : {
                        //r: 30
                    },
                    linkStyle : {}
                }
            },
            large: true,
            useWorker: true,
            steps: 1,
            minRadius : 10,
            maxRadius : 25,
            gravity: 1,
            scaling: 1.1,
            roam: 'scale',
            draggable: false,

FAKEDATA
        }
    ]
};
            mychart.setOption(option);
            var ecConfig = require('echarts/config');
            function focus(param) {
                var data = param.data;
                var links = option.series[0].links;
                if (data.source == undefined || data.target == undefined) {
                    console.log('select node ' + data.name +' ' + data.link);
                    // location.href = data.link; // open in currently tab
                    open(data.link); // open new window or new tab
                } else {
                    console.log('select edge');
                }
            }
            mychart.on(ecConfig.EVENT.CLICK, focus)
        }
       );
    </script>
</body>
