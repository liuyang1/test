import pygame
import random

# 参数设置
SCREEN_WIDTH = 1600
SCREEN_HEIGHT = 800
LANE_WIDTH = 80  # 每车道宽度
LANE_COUNT = 4   # 车道数量
FPS = 60         # 帧率

# 颜色定义
WHITE = (255, 255, 255)
BLACK = (0, 0, 0)
RED = (255, 0, 0)
GREEN = (0, 255, 0)
BLUE = (0, 0, 255)
YELLOW = (255, 255, 0)  # 用于表示失控车辆
GRAY = (128, 128, 128)  # 变道限制标记颜色

# 出入口位置与变道限制
EXIT_POSITIONS = [
    {"start": 350, "end": 450, "lane": 3},  # 出口在最右侧车道
]

LANE_CHANGE_RESTRICTIONS = [
    {
        "start": 200,
        "end": 300,
        "left_lane": 3,  # 右侧车道变道限制
        "right_lane": 2,
        "type": "no_lane_change",  # 完全禁止变道
    },
    {
        "start": 800,
        "end": 900,
        "left_lane": 1,
        "right_lane": 0,
        "type": "one_way",  # 只可从 left_lane 到 right_lane
    },
]

# 车辆类
class Vehicle:
    vehicle_id = 0  # 车辆编号

    def __init__(self, pos, lane, speed, params, color=RED):
        self.id = Vehicle.vehicle_id  # 车辆编号
        Vehicle.vehicle_id += 1
        self.pos = pos            # 位置（以道路起始点为原点）
        self.lane = lane          # 车道
        self.speed = speed        # 当前速度
        self.max_speed = params['max_speed']  # 最大速度
        self.acceleration = params['acceleration']  # 加速度
        self.braking_distance = params['braking_distance']  # 刹车距离
        self.destination = params['destination']  # 目的地
        self.color = color        # 车辆颜色
        self.crashed = False      # 是否发生碰撞

        # 车辆状态
        self.is_accident = False  # 是否处于事故状态
        self.accident_timer = 0   # 事故持续时间
        self.stopped_time = 0     # 停止时间
        self.moving = True        # 是否移动

    def check_lane_change_restriction(self, target_lane):
        # 检查当前位置是否处于变道限制区间
        for restriction in LANE_CHANGE_RESTRICTIONS:
            if self.pos >= restriction["start"] and self.pos <= restriction["end"]:
                # 检查是否为目标车道对
                if (self.lane == restriction["left_lane"] and target_lane == restriction["right_lane"]) or \
                   (self.lane == restriction["right_lane"] and target_lane == restriction["left_lane"]):
                    if restriction["type"] == "no_lane_change":
                        return False  # 完全禁止
                    elif restriction["type"] == "one_way":
                        if self.lane == restriction["left_lane"] and target_lane == restriction["right_lane"]:
                            return True  # 只允许单侧
                        else:
                            return False
        return True  # 无限制或允许

    def update(self, dt, nearby_vehicles):
        # 检测前方车辆
        front_vehicle = next(
            (v for v in nearby_vehicles if v.lane == self.lane and v.pos > self.pos),
            None
        )
        distance = front_vehicle.pos - self.pos if front_vehicle else float('inf')

        # 处理事故状态
        if self.is_accident:
            self.speed = 0
            self.crashed = True
            self.color = YELLOW
            self.moving = False
            self.stopped_time += dt
            if self.stopped_time >= 5:  # 模拟事故处理时间
                self.is_accident = False
                self.stopped_time = 0
                self.color = RED
                self.moving = True
        else:
            if not self.moving:
                # 从事故状态恢复后加速
                self.speed = min(self.speed + self.acceleration * dt * 2, self.max_speed)
                self.moving = True
            else:
                # 计算期望速度
                desired_speed = self.max_speed
                if front_vehicle:
                    # 根据前车距离调整期望速度
                    desired_speed = min(self.max_speed, max(0, self.speed - (self.braking_distance - distance) * 0.1))
                else:
                    # 路上没有前车，全力加速
                    desired_speed = min(self.max_speed, self.speed + self.acceleration * dt)
                
                # 调整速度
                if self.speed > desired_speed:
                    # 刹车
                    self.speed = max(0, self.speed - self.acceleration * dt)
                else:
                    # 加速
                    self.speed = min(self.speed + self.acceleration * dt, desired_speed)

                # 变道逻辑
                if random.random() < 0.02:  # 较低的变道概率
                    target_lane = self.lane  # 默认不变道
                    # 检查相邻车道是否有更快的车流
                    left_lane = self.lane - 1
                    right_lane = self.lane + 1
                    
                    # 获取左侧车道车辆
                    left_vehicles = [v for v in nearby_vehicles if v.lane == left_lane]
                    if left_lane >= 0 and front_vehicle and left_vehicles:
                        # 如果左侧车道前方车辆速度更快，尝试变道
                        if left_vehicles[-1].speed > front_vehicle.speed:
                            target_lane = left_lane
                    # 获取右侧车道车辆
                    right_vehicles = [v for v in nearby_vehicles if v.lane == right_lane]
                    if right_lane < LANE_COUNT and front_vehicle and right_vehicles:
                        if right_vehicles[-1].speed > front_vehicle.speed:
                            target_lane = right_lane
                    
                    # 检查变道限制
                    if self.check_lane_change_restriction(target_lane):
                        # 检测目标车道是否有足够的空间
                        target_vehicles = [v for v in nearby_vehicles if v.lane == target_lane]
                        if not target_vehicles or (self.pos - target_vehicles[-1].pos > 50):
                            self.lane = target_lane

        # 计算移动距离
        self.pos += self.speed * dt

        # 判断是否到达出口
        for exit in EXIT_POSITIONS:
            if self.destination == exit['lane'] and self.pos > exit['start'] and self.pos < exit['end']:
                return None  # 车辆驶出

        # 检测碰撞
        for v in nearby_vehicles:
            if v != self and abs(v.pos - self.pos) < 20 and v.lane == self.lane:
                self.is_accident = True

        return self

    def draw(self, screen, lane_offset):
        x = self.pos
        y = SCREEN_HEIGHT - (self.lane * LANE_WIDTH + LANE_WIDTH / 2)
        pygame.draw.rect(screen, self.color, (x, y - lane_offset, 20, 10))  # 车辆绘制
        # 绘制车辆编号
        font = pygame.font.SysFont(None, 18)
        text = font.render(str(self.id), True, BLACK)
        screen.blit(text, (x + 30, y - lane_offset - 5))

# 道路类
class Road:
    def __init__(self):
        self.vehicles = []
        self.lanes = LANE_COUNT
        self.speed_limit = 30  # 道路限速

    def add_vehicle(self, vehicle):
        self.vehicles.append(vehicle)

    def remove_vehicle(self, vehicle):
        if vehicle in self.vehicles:
            self.vehicles.remove(vehicle)

    def step(self, dt):
        new_vehicles = []
        for vehicle in self.vehicles:
            nearby = self.get_nearby_vehicles(vehicle, 200)
            updated_vehicle = vehicle.update(dt, nearby)
            if updated_vehicle:
                new_vehicles.append(updated_vehicle)
            else:
                self.remove_vehicle(vehicle)
        self.vehicles = new_vehicles

        # 增加新车生成的概率
        if random.random() < 0.1:  # 调整为10%的概率生成新车
            lane = random.choice(range(LANE_COUNT))
            speed = random.uniform(10, self.speed_limit)
            params = {
                'max_speed': self.speed_limit,
                'acceleration': 1.0,
                'braking_distance': 30,
                'destination': random.choice([e['lane'] for e in EXIT_POSITIONS])
            }
            self.add_vehicle(Vehicle(-100, lane, speed, params))  # 车辆从屏幕外进入

    def get_nearby_vehicles(self, vehicle, distance):
        return [v for v in self.vehicles if abs(v.pos - vehicle.pos) <= distance and v.lane == vehicle.lane]

    def draw(self, screen):
        # 绘制车道和出入口、变道限制标记
        for i in range(self.lanes):
            pygame.draw.rect(screen, BLACK, (0, SCREEN_HEIGHT - (i+1)*LANE_WIDTH, SCREEN_WIDTH, LANE_WIDTH), 2)
        
        # 绘制变道限制标记
        for restriction in LANE_CHANGE_RESTRICTIONS:
            x_start = restriction["start"]
            x_end = restriction["end"]
            y_left = SCREEN_HEIGHT - (restriction["left_lane"] + 1) * LANE_WIDTH
            y_right = SCREEN_HEIGHT - (restriction["right_lane"] + 1) * LANE_WIDTH
            pygame.draw.line(screen, GRAY, (x_start, y_left), (x_end, y_right), 2)  # 连接线

        # 绘制出入口
        for exit in EXIT_POSITIONS:
            x_start = exit['start']
            x_end = exit['end']
            y = SCREEN_HEIGHT - (exit['lane']+1)*LANE_WIDTH
            pygame.draw.rect(screen, GREEN, (x_start, y, x_end - x_start, LANE_WIDTH // 2))

# 主程序
def main():
    pygame.init()
    screen = pygame.display.set_mode((SCREEN_WIDTH, SCREEN_HEIGHT))
    clock = pygame.time.Clock()

    road = Road()

    # 强制添加一些初始车辆（用于调试）
    for i in range(3):
        params = {
            'max_speed': 30,
            'acceleration': 1.0,
            'braking_distance': 30,
            'destination': 3  # 目的地设为最右侧车道
        }
        road.add_vehicle(Vehicle(-100, i, random.uniform(10, 30), params))

    running = True
    while running:
        dt = clock.tick(FPS) / 1000.0

        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False

        road.step(dt)

        screen.fill(WHITE)
        road.draw(screen)
        for vehicle in road.vehicles:
            vehicle.draw(screen, LANE_WIDTH // 2)

        pygame.display.flip()

    pygame.quit()

if __name__ == "__main__":
    main()
