<!DOCTYPE html>
<head>
    <meta charset="utf-8">
    <title>sitemap</title>
</head>

<body>
    <!-- <div id="main" style="width:1300px; height:650px"></div> -->
    <div id="main" style="width:1300px; height:650px"></div>
    <script src="http://echarts.baidu.com/build/dist/echarts.js"></script>
    <script type="text/javascript">
require.config({paths: {echarts:'http://echarts.baidu.com/build/dist'}});
require(['echarts', 'echarts/chart/force'],
        function (ec) {
            var mychart = ec.init(document.getElementById('main'));
var option = {
    title : {
        text: 'sitemap',
        subtext: 'as liuyang\'s wiki',
        x:'right',
        y:'bottom'
    },
    tooltip : {
        trigger: 'item',
        formatter: '{a} : {b}'
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
            useWorker: false,
            minRadius : 15,
            maxRadius : 25,
            gravity: 1.1,
            scaling: 1.1,
            roam: 'move',
FAKEDATA
        }
    ]
};
            mychart.setOption(option);
        }
       );
    </script>
</body>
