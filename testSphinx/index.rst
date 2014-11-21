============
tool and lib
============

性能测试工具
============

ab
--
    apache ab
    性能测试工具
    apache组建,需要下载apache安装.简单方便
    ::
        ab -n 10 -c 10 http://www.google.com/
    地址最后要加"/",不然无法正常运行.

loadRunner
----------
    性能测试工具.
    比较强大,复杂得多.lib.ustc有书专门介绍.比较复杂

python lib
==========
gevent
------
    libevent封装,同时支持协程,高并发,异步编程库.
    应该比较强大.

manager tool
============
webmin
------
    web界面的状态管理与配置
    直接使用系统的用户名密码登录,默认端口10000
    配置管理,与监控

dispatch
========
部署与管理工具
func
----
    fedora下面的工具,
    python有库支持
puppet
------
    集中配置管理工具
fabric
------
    基于python的工具.有推荐说比puppet好用.留待以后尝试.
nagios
------
    企业级标准IT架构监控.
    看到有公司在实际使用(较多).还不错.
    有web端,方便使用.

system monitor
==============
iftop
-----
    简单网络监控工具
    cli输出,没有看到生成日志的选项
    因此方便作为实时监督观看使用
iftraf
-----
    网络监控工具
    略微复杂一些
    有很方便的过滤功能,图形化的配置界面使用方便(比tcpdump)
    可以生成日志文件,这样方便用其他工具分析结果
    一样有cli界面
    link_:http://blog.chinaunix.net/space.php?uid=272339&do=blog&id=28464

figure plot
===========
matplotlib
----------
    ...
rrdtool
-------
    据说较复杂...
