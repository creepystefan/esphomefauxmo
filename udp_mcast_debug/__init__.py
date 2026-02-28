from esphome import automation
from esphome.automation import Trigger
import esphome.codegen as cg

import esphome.config_validation as cv
from esphome.const import CONF_DATA, CONF_ID, CONF_PORT
from esphome.core import ID
from esphome.cpp_generator import MockObj

CODEOWNERS = ["@creepystefan"]
DEPENDENCIES = ["network"]
AUTO_LOAD = ["socket"]

udp_mcast_debug_ns = cg.esphome_ns.namespace("udp_mcast_debug")
udp_mcast_debugComponent = udp_mcast_debug_ns.class_("udp_mcast_debugComponent", cg.Component)

CONF_UDP_MCAST_DEBUG_ID = "udp_mcast_debug_id"

CONFIG_SCHEMA = cv.COMPONENT_SCHEMA.extend(
    
    {
        cv.GenerateID(): cv.declare_id(udp_mcast_debugComponent),
    }
).extend(cv.COMPONENT_SCHEMA)


async def register_udp_client(var, config):
    udp_var = await cg.get_variable(config[CONF_UDP_MCAST_DEBUG_ID])
    cg.add(var.set_parent(udp_mcast_debug_var))
    return udp_var

async def to_code(config):
    #cg.add_define("USE_UDP")
    #cg.add_global(udp_ns.using)
    var = cg.new_Pvariable(config[CONF_ID])
    var = await cg.register_component(var, config)
  