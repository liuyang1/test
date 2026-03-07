"""indicator.py 单元测试 - 纯计算逻辑，不依赖网络"""
import sys, os
sys.path.insert(0, os.path.join(os.path.dirname(__file__), "..", "src"))

from indicator import calc_ma, calc_ema, calc_macd, calc_rsi, analyze


class TestMA:
    def test_basic(self):
        assert calc_ma([1, 2, 3, 4, 5], 3) == 4.0  # (3+4+5)/3

    def test_insufficient_data(self):
        assert calc_ma([1, 2], 5) is None


class TestMACD:
    def test_insufficient_data(self):
        dif, dea, hist = calc_macd(list(range(10)))
        assert dif is None

    def test_sufficient_data(self):
        # 30个递增数据，应该产生正的DIF
        closes = list(range(1, 31))
        dif, dea, hist = calc_macd(closes)
        assert dif is not None
        assert dif > 0  # 上升趋势，短期EMA > 长期EMA


class TestRSI:
    def test_insufficient_data(self):
        assert calc_rsi(list(range(5))) is None

    def test_all_up(self):
        # 持续上涨，RSI应接近100
        closes = list(range(1, 20))
        rsi = calc_rsi(closes)
        assert rsi > 90

    def test_all_down(self):
        # 持续下跌，RSI应接近0
        closes = list(range(20, 1, -1))
        rsi = calc_rsi(closes)
        assert rsi < 10


class TestAnalyze:
    def _make_klines(self, closes):
        return [{"close": c, "open": c, "high": c + 0.1, "low": c - 0.1, "volume": 1000} for c in closes]

    def test_insufficient_data(self):
        result = analyze(self._make_klines([1, 2, 3]))
        assert result["signal"] == "观望"
        assert "历史数据不足" in result["reasons"]

    def test_uptrend_positive_score(self):
        # 模拟温和上升趋势：横盘后小幅上涨（不触发超买）
        closes = [10] * 20 + [10, 10.1, 10, 9.9, 10, 10.2, 10.1, 10, 10.3, 10.2,
                               10.1, 10.3, 10.4, 10.3, 10.5, 10.6, 10.5, 10.7, 10.8, 10.9]
        result = analyze(self._make_klines(closes))
        # 温和上涨应该不会是强烈负分
        assert result["score"] >= 0

    def test_downtrend_negative_score(self):
        # 模拟温和下跌趋势：横盘后小幅下跌（不触发超卖）
        closes = [20] * 20 + [20, 19.9, 20, 20.1, 19.9, 19.8, 19.9, 20, 19.7, 19.8,
                               19.9, 19.7, 19.6, 19.7, 19.5, 19.4, 19.5, 19.3, 19.2, 19.1]
        result = analyze(self._make_klines(closes))
        assert result["score"] <= 0
