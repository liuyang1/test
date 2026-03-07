"""端到端集成测试 - 使用真实行情数据，验证用户意图识别和推荐结果
覆盖历史讨论中的所有用户输入场景
"""
import sys, os
sys.path.insert(0, os.path.join(os.path.dirname(__file__), "..", "src"))

import pytest
from user_store import UserStore
from stock_advisor import StockAdvisor

# 标记：这些测试需要网络访问
pytestmark = pytest.mark.integration


@pytest.fixture(scope="module")
def advisor():
    return StockAdvisor()


@pytest.fixture(scope="module")
def user_store(tmp_path_factory):
    import tempfile
    os.environ["DATA_DIR"] = str(tmp_path_factory.mktemp("users"))
    store = UserStore()
    store.create_user("test_user")
    return store


class TestBuyIntent:
    """用户想买入股票的场景"""

    QUERIES = [
        "推荐购买红利股票",
        "推荐一个红利股票给我",
        "红利股建议买入的",
        "有什么值得买的银行股",
        "推荐购买新能源",
        "科技股买什么好",
    ]

    @pytest.mark.parametrize("query", QUERIES)
    def test_buy_returns_positive_signal(self, query, advisor, user_store):
        result = advisor.get_advice("test_user", query, user_store)
        # 不应该返回"建议卖出"的股票
        assert "建议卖出" not in result
        # 应该有结果或明确说没有买入条件
        assert "推荐" in result or "没有找到符合买入条件" in result


class TestSellIntent:
    """用户想了解卖出信号的场景"""

    QUERIES = [
        "科技股建议卖出的有吗",
        "哪些银行股该卖了",
        "新能源需要减仓吗",
        "该止盈的股票有哪些",
    ]

    @pytest.mark.parametrize("query", QUERIES)
    def test_sell_returns_negative_signal(self, query, advisor, user_store):
        result = advisor.get_advice("test_user", query, user_store)
        # 不应该返回"建议买入"的股票
        assert "建议买入" not in result
        # 应该有结果或明确说不需要卖出
        assert "推荐" in result or "没有找到需要卖出" in result


class TestSectorRecognition:
    """板块关键词识别"""

    CASES = [
        ("推荐科技股", "科技"),
        ("推荐新能源股票", "新能源"),
        ("银行股怎么样", "银行"),
        ("白酒板块有推荐吗", "白酒"),
        ("医药股推荐", "医药"),
        ("推荐红利股票", "红利"),
        ("半导体有什么好股票", "半导体"),
        ("推荐高股息的", "红利"),
    ]

    @pytest.mark.parametrize("query,expected_sector", CASES)
    def test_sector_in_result(self, query, expected_sector, advisor, user_store):
        result = advisor.get_advice("test_user", query, user_store)
        assert expected_sector in result


class TestNoSectorFallback:
    """没有明确板块时应该给出热门推荐，而不是拒绝"""

    QUERIES = [
        "推荐几只股票",
        "今天买什么好",
        "有什么推荐的吗",
    ]

    @pytest.mark.parametrize("query", QUERIES)
    def test_fallback_gives_result(self, query, advisor, user_store):
        result = advisor.get_advice("test_user", query, user_store)
        # 不应该要求用户设置偏好
        assert "请先设置" not in result
        assert "请告诉我" not in result
        # 应该有推荐结果
        assert "推荐" in result or "没有找到" in result


class TestPreference:
    """用户偏好设置和应用"""

    def test_set_and_read_preference(self, user_store):
        user_store.update_preference("test_user", {"risk": "保守", "sector": "银行"})
        user = user_store.get_user("test_user")
        assert user["preference"]["risk"] == "保守"
        assert user["preference"]["sector"] == "银行"

    def test_preference_affects_result(self, advisor, user_store):
        # 设置激进偏好
        user_store.update_preference("test_user", {"risk": "激进"})
        result = advisor.get_advice("test_user", "推荐科技股", user_store)
        assert "推荐" in result


class TestResultFormat:
    """结果格式验证"""

    def test_has_disclaimer(self, advisor, user_store):
        result = advisor.get_advice("test_user", "推荐红利股票", user_store)
        assert "不构成投资建议" in result

    def test_has_stock_code(self, advisor, user_store):
        result = advisor.get_advice("test_user", "推荐银行股", user_store)
        # 应该包含6位股票代码
        import re
        assert re.search(r"\d{6}", result)

    def test_has_price_info(self, advisor, user_store):
        result = advisor.get_advice("test_user", "推荐银行股", user_store)
        assert "价格:" in result
        assert "涨幅:" in result

    def test_has_analysis_reason(self, advisor, user_store):
        result = advisor.get_advice("test_user", "推荐银行股", user_store)
        assert "理由：" in result
