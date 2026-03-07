"""技术指标计算与买卖信号判断"""


def calc_ma(closes, period):
    """计算移动平均线"""
    if len(closes) < period:
        return None
    return sum(closes[-period:]) / period


def calc_ema(closes, period):
    """计算指数移动平均"""
    if len(closes) < period:
        return None
    multiplier = 2 / (period + 1)
    ema = sum(closes[:period]) / period
    for price in closes[period:]:
        ema = (price - ema) * multiplier + ema
    return ema


def calc_macd(closes):
    """计算 MACD，返回 (dif, dea, macd_hist)"""
    if len(closes) < 26:
        return None, None, None
    # 逐步计算完整的 EMA 序列
    ema12 = [sum(closes[:12]) / 12]
    for p in closes[12:]:
        ema12.append((p - ema12[-1]) * 2 / 13 + ema12[-1])
    ema26 = [sum(closes[:26]) / 26]
    for p in closes[26:]:
        ema26.append((p - ema26[-1]) * 2 / 27 + ema26[-1])
    # 对齐：ema12 从 index 14 开始与 ema26 对齐
    offset = 26 - 12
    dif_series = [ema12[i + offset] - ema26[i] for i in range(len(ema26))]
    if len(dif_series) < 9:
        return dif_series[-1] if dif_series else None, None, None
    dea = sum(dif_series[:9]) / 9
    for d in dif_series[9:]:
        dea = (d - dea) * 2 / 10 + dea
    dif = dif_series[-1]
    return dif, dea, (dif - dea) * 2


def calc_rsi(closes, period=14):
    """计算 RSI"""
    if len(closes) < period + 1:
        return None
    gains, losses = 0, 0
    for i in range(-period, 0):
        diff = closes[i] - closes[i - 1]
        if diff > 0:
            gains += diff
        else:
            losses -= diff
    avg_gain = gains / period
    avg_loss = losses / period
    if avg_loss == 0:
        return 100
    rs = avg_gain / avg_loss
    return 100 - 100 / (1 + rs)


def analyze(klines):
    """综合技术分析，返回 {signal: '买入'|'卖出'|'观望', reasons: [...], score: int}"""
    if len(klines) < 26:
        return {"signal": "观望", "reasons": ["历史数据不足"], "score": 0}

    closes = [k["close"] for k in klines]
    score = 0
    reasons = []

    # 1. 均线分析
    ma5 = calc_ma(closes, 5)
    ma10 = calc_ma(closes, 10)
    ma20 = calc_ma(closes, 20)
    price = closes[-1]

    if ma5 and ma10:
        if ma5 > ma10 and closes[-2] and calc_ma(closes[:-1], 5) <= calc_ma(closes[:-1], 10):
            score += 30
            reasons.append("MA5上穿MA10金叉")
        elif ma5 < ma10 and closes[-2] and calc_ma(closes[:-1], 5) >= calc_ma(closes[:-1], 10):
            score -= 30
            reasons.append("MA5下穿MA10死叉")

    if ma20 and price > ma20:
        score += 10
        reasons.append("价格在MA20上方")
    elif ma20:
        score -= 10
        reasons.append("价格在MA20下方")

    # 2. MACD
    dif, dea, hist = calc_macd(closes)
    if dif is not None and dea is not None:
        prev_closes = closes[:-1]
        prev_dif, prev_dea, _ = calc_macd(prev_closes)
        if prev_dif is not None and prev_dea is not None:
            if dif > dea and prev_dif <= prev_dea:
                score += 25
                reasons.append("MACD金叉")
            elif dif < dea and prev_dif >= prev_dea:
                score -= 25
                reasons.append("MACD死叉")
        if hist and hist > 0:
            score += 5
        elif hist:
            score -= 5

    # 3. RSI
    rsi = calc_rsi(closes)
    if rsi is not None:
        if rsi < 30:
            score += 20
            reasons.append(f"RSI={rsi:.0f} 超卖")
        elif rsi > 70:
            score -= 20
            reasons.append(f"RSI={rsi:.0f} 超买")

    # 4. 近期趋势（5日涨幅）
    if len(closes) >= 6:
        trend = (closes[-1] - closes[-6]) / closes[-6] * 100
        if -5 < trend < -2:
            score += 10
            reasons.append(f"近5日回调{trend:.1f}%，可能存在低吸机会")
        elif trend > 5:
            score -= 5
            reasons.append(f"近5日涨{trend:.1f}%，注意追高风险")

    # 综合判断
    if score >= 25:
        signal = "建议买入 ✅"
    elif score <= -25:
        signal = "建议卖出 🔴"
    else:
        signal = "观望 ⏸️"

    if not reasons:
        reasons.append("无明显信号")

    return {"signal": signal, "reasons": reasons, "score": score}
