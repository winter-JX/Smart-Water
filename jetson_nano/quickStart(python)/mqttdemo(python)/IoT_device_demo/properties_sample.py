# -*- encoding: utf-8 -*-
'''
平台设置属性，查询属性，设备上报属性demo
'''
import time
import logging
import json

from IoT_device.client.IoT_client_config import IoTClientConfig
from IoT_device.client.IoT_client import IotClient
from IoT_device.request.services_properties import ServicesProperties

# 日志设置
logging.basicConfig(level=logging.INFO)
logger = logging.getLogger(__name__)

def run():
    # 客户端配置
    client_cfg = IoTClientConfig(server_ip='3b4898dd3e.st1.iotda-device.cn-north-4.myhuaweicloud.com',
                                 device_id='667e64916bc31504f070fa26_jetson_nano',
                                 secret='bc8164cb3331a3e8dd8ce332d35b263f', is_ssl=False)

    # 创建设备
    iot_client = IotClient(client_cfg)
    iot_client.connect() # 建立连接

    # 自定义callback
    def property_set_callback(request_id, payload):
        # 遍历services
        for service in json.loads(payload)['services']:
            logger.info(('property set service id = ', service['service_id']))
            for property in service['properties']:
                logger.info(('property name = ', property))
                logger.info(('property value = ', service['properties'][property]))

        # 上行响应
        iot_client.respond_property_set(request_id, result_code='success')
        # 上报属性
        service_property = ServicesProperties()
        service_property.add_service_property(service_id="Battery", property='batteryLevel', value=1)
        iot_client.report_properties(service_property.service_property, qos=1)
        print('------------------this is myself callback')

    # 自定义callback
    def property_get_callback(request_id, payload):
        logger.info(('property get service id:', json.loads(payload)['service_id']))
        service_property = ServicesProperties()
        service_property.add_service_property(service_id="Battery", property='batteryLevel', value=2)
        service_property.add_service_property(service_id="analog", property='PHV-phsA', value=1)
        iot_client.respond_property_get(request_id, service_property.service_property)
        print('------------------this is myself callback')

    # 设置平台设置设备属性的回调
    iot_client.set_property_set_callback(property_set_callback)
    # 设置平台查询设备属性的回调
    iot_client.set_property_get_callback(property_get_callback)
    iot_client.start()  # 线程启动

    pump_switch_data = {
        "pump1": "1",
        "pump2": "1",
        "pump3": "1",
        "pump4": "1",
        "pump5": "1",
        "pump6": "1",
        "pump7": "1",
        "pump1": "1"
    }
    pump_switch_data = json.dumps(pump_switch_data)

    flow_msg_data = "12.34"
    # 定时上报属性
    while True:
        # 按照产品模型设置属性
        service_property = ServicesProperties()
        service_property.add_service_property(service_id="jetson_nano", property='flow_msg', value=flow_msg_data)
        service_property.add_service_property(service_id="jetson_nano", property='pump_switch', value=pump_switch_data)
        iot_client.report_properties(service_properties=service_property.service_property, qos=1)
        time.sleep(10)

if __name__ == '__main__':
    run()
