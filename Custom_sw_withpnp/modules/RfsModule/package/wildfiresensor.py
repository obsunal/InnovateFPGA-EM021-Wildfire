from package.sensor import *
from package.sensorcontroller import *

class WildfireSensor(SensorController):
    def __init__(self,name='wildfire', real=False, offset=0x0) -> None:
        self.name = name
        self.modules = {
            'flame_sensor'  : Sensor('flame_sensor',            'int',real,       offset),
            'adc1'          : Sensor('adc1',            'int',real,       offset+0x4),
            'adc2'          : Sensor('adc1',            'int',real,       offset+0x8),
            'CO_AX'          : Sensor('CO_AX',            'float',real,       offset+0xC)
        }
        
        if(real) :
            logger.debug(" \n\
                    WildFireSensors: \n\
                        Flame Sensor Value: \n\
                            Data : {} \n\
                        ADC1: \n\
                            Data : {} \n\
                        ADC2: \n\
                            Data : {} \n\
                        CO_AX: \n\
                            Data : {} \n"
                .format(
                    hex(self.modules.get('flame_sensor').offset),
                    hex(self.modules.get('adc1').offset),
                    hex(self.modules.get('adc2').offset),
                    hex(self.modules.get('CO_AX').offset)
                )
            )