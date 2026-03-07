"""定时任务入口 - SCF 定时触发器调用，每日收盘后刷新缓存"""
from cache import refresh_cache


def main_handler(event, context):
    meta = refresh_cache()
    return {"code": 0, "msg": f"缓存刷新完成，共 {meta['stock_count']} 只股票"}
