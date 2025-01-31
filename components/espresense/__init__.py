import esphome.config_validation as cv
import esphome.codegen as cg
from esphome.const import CONF_ID
from esphome.components import esp32_ble_tracker


esphomepresense_ns = cg.esphome_ns.namespace("espresense")
ESPHomePresenseComponent = esphomepresense_ns.class_("ESPHomePresense", esp32_ble_tracker.ESPBTDeviceListener, cg.Component)
NODE_NAME = "nodeName"

DEPENDENCIES = ['esp32_ble_tracker','mqtt']
CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(ESPHomePresenseComponent),
        #ad the optional value for the node name that is a string with
        cv.Optional(NODE_NAME, default=""): cv.string,
    }
).extend(esp32_ble_tracker.ESP_BLE_DEVICE_SCHEMA).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await esp32_ble_tracker.register_ble_device(var, config)
    cg.add(var.set_node_name(config[NODE_NAME]))