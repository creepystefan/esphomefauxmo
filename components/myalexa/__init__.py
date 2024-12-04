import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import CONF_ID

cg.add_library(
    name="ESPFauxmo",
    repository="https://github.com/creepystefan/esphomefauxmo",
    version=None,
)

myalexa_ns = cg.esphome_ns.namespace("myalexa")
MYALEXA = myalexa_ns.class_("MYALEXA", cg.Component)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(MYALEXA),
                       
    }
).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    
    


    