from linkkit import linkkit
import sys
import configparser , time , logging , os , sys , random, json
from threading import Thread
from time import sleep

logging.getLogger().setLevel(logging.INFO)
# 一机一密 认证连接
lk = linkkit.LinkKit(
    host_name="cn-shanghai",
    product_key="k18q7K1SDVA",
    device_name="jetson_nano",
    device_secret="3b307a5a29ff64af416f6f2b67318026")

lk.config_mqtt(endpoint="iot-06z00g76h74xs5m.mqtt.iothub.aliyuncs.com",
               port=1883, protocol="MQTTv311", transport="TCP",
               secure="TLS", keep_alive=60, clean_session=True,
               max_inflight_message=20, max_queued_message=0,
               auto_reconnect_min_sec=1,
               auto_reconnect_max_sec=60,
               cadata=None)


# 回调函数
# 设备与物联网平台建立连接后，您可通过on_connect回调函数了解连接结果。连接成功后，如果连接断开，您#可通过on_disconnect回调函数，编写业务场景所需的处理逻辑。
def on_connect(session_flag, rc, userdata):
    print("on_connect:%d,rc:%d,userdata:" % (session_flag, rc))
    pass

# 断开连接
def on_disconnect(rc , userdata) :
    logging.info("on_disconnect:rc:%d,userdata:" % rc)

# Topic消息
def on_topic_message(topic , payload , qos , userdata) :
    logging.info("on_topic_message:" + topic + " payload:" + str(payload) + " qos:" + str(qos))
    # print("on_topic_message:" + topic + " payload:" + str(payload) + " qos:" + str(qos))

# 订阅
def on_subscribe_topic(mid , granted_qos , userdata) :
    logging.info("on_subscribe_topic mid:%d, granted_qos:%s" %
                 (mid , str(','.join('%s' % it for it in granted_qos))))

# 取消订阅
def on_unsubscribe_topic(mid , userdata) :
    logging.info("on_unsubscribe_topic mid:%d" % mid)

# 发布消息
def on_publish_topic(mid , userdata) :
    logging.info("on_publish_topic mid:%d" % mid)

# 上报属性
def on_thing_prop_post(request_id , code , data , message , userdata) :
    logging.info("on_thing_prop_post request id:%s, code:%d message:%s, data:%s,userdata:%s" %
                 (request_id , code , message , data , userdata))

# 用户可以进行属性上报，事件上报，服务响应，此调用需要在连接前
def on_thing_enable(userdata) :
    logging.info("on_thing_enable")


# def on_subscribe_topic(mid, granted_qos, userdata):  # 订阅topic
#     print("on_subscribe_topic mid:%d, granted_qos:%s" %
#           (mid, str(','.join('%s' % it for it in granted_qos))))
#     pass

# 接收云端的数据

# def on_topic_message(topic, payload, qos, userdata):
#     # 设备端的接收到的数据却是b:"123"用了一个切片去处理数据
#     print("阿里云上传回的数值是:", str(payload))
#     # 拿到接收来的数据
#     data = str(payload)[2:-1]
#     print("阿里云上传回的数值是:", data)
#     dataDict = json.loads(data)
#     print("阿里云上传回的数值是:", type(dataDict))  # 切片左闭右开 取头不取尾
#     # # print(dataDict["jiang"])
#     # # 多层解析
#     # # {"temp":{"value":62}}
#     # print(dataDict["temp"]["value"])  # 解析多层数据
#     pass


# 发布消息的结果，判断是否成功调用发布函数


lk.on_connect = on_connect
print(lk.on_connect)
lk.on_disconnect = on_disconnect
lk.on_thing_enable = on_thing_enable
lk.on_subscribe_topic = on_subscribe_topic
lk.on_unsubscribe_topic = on_unsubscribe_topic
lk.on_topic_message = on_topic_message
lk.on_publish_topic = on_publish_topic
lk.on_thing_prop_post = on_thing_prop_post
# 注册云端订阅的方法
lk.on_subscribe_topic = on_subscribe_topic
# 注册当接受到云端发送的数据的时候的方法
lk.on_topic_message = on_topic_message
# 注册向云端发布数据的时候顺便所调用的方法
lk.on_publish_topic = on_publish_topic
# 注册取消云端订阅的方法
lk.on_unsubscribe_topic = on_unsubscribe_topic

# 启动连接
lk.connect_async()
lk.start_worker_loop()
time.sleep(2)  # 延时



#订阅主题
rc, mid = lk.subscribe_topic(lk.to_full_topic("user/update")) # 订阅接收微信小程序控制信息的主题
#发布主题
# rc, mid = lk.subscribe_topic(lk.to_full_topic("user/get"))
# rc, mid = lk.unsubscribe_topic("/k18q7K1SDVA/jetson_nano/user/get")


# def usart1_irq_handler():
#     global RxState, pRxPacket, Serial_RxPacket, Serial_RxFlag
#
#     # 模拟USART接收数据可用的状态检查
#     if is_rx_data_available(USART1):  # 这里需要一个模拟的函数来表示数据可读
#         RxData = read_usart_data(USART1)  # 同样，这里需要一个函数来模拟读取USART数据
#
#         if RxState == 0:
#             if RxData == '!' and Serial_RxFlag == 0:
#                 RxState = 1
#                 pRxPacket = 0
#         elif RxState == 1:
#             if RxData == '\r':
#                 RxState = 2
#             else:
#                 Serial_RxPacket[pRxPacket] = RxData
#                 pRxPacket += 1
#         elif RxState == 2:
#             if RxData == '\n':
#                 RxState = 0
#                 Serial_RxPacket[pRxPacket] = '\0'
#                 Serial_RxFlag = 1

# 只读属性数据
pdata = {
    "flow_msg": random.randint(8000, 12000),
    "pump1_switch": 1,
    "pump2_switch": 0
}


while True:
    pump_data = {
        "flow_msg": {
            "pump1": random.randint(8000, 12000),
            "pump2": random.randint(8000, 12000),
            "pump3": random.randint(8000, 12000),
            "pump4": random.randint(8000, 12000),
            "pump5": random.randint(8000, 12000),
            "pump6": random.randint(8000, 12000),
            "pump7": random.randint(8000, 12000),
            "pump8": random.randint(8000, 12000)
        },
        "switch": {
            "pump1": 0,
            "pump2": 1,
            "pump3": 0,
            "pump4": 1,
            "pump5": 0,
            "pump6": 1,
            "pump7": 0,
            "pump8": 1
        }
    }
    # 终止订阅主题

    rc, mid = lk.publish_topic(lk.to_full_topic("user/get"), str(pump_data))
    time.sleep(2)
    pass


# while True:
#     try:
#         rc , request_id = lk.thing_post_property({**pdata})
#         if rc == 0:
#             logging.info("thing_post_property success:%r,mid:%r,\npost_data:%s" % (rc, request_id, pdata))
#         else:
#             logging.warning("thing_post_property failed:%d" % rc)
#             time.sleep(10)
#     except KeyboardInterrupt:
#         sys.exit()
