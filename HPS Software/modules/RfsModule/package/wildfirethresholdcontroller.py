# Copyright (C) 2021 Intel Corporation 
# Licensed under the MIT license. See LICENSE file in the project root for
# full license information.

from package.threshold import *

class WildfireThresholdController:

    def __init__(self,real=False, bridge=None,offset=0x0) -> None:
        self.modules = {
            'flame_sensor'  : Threshold('flame_sensor', 'float', real,        bridge, offset,         offset+0x4,             0,      200),
            'adc1'          : Threshold('adc1',         'float', real,        bridge, offset+0x8*1,   offset+0x4+0x8*1,       0,      4096), 
            'adc2'          : Threshold('adc2',         'float', real,        bridge, offset+0x8*2,   offset+0x4+0x8*2,       0,      4096),
            'CO_AX'         : Threshold('CO_AX',        'float', real,        bridge, offset+0x8*3,   offset+0x4+0x8*3,       0,      100), 
        }
        if(real) :
            logger.debug(" \n\
                    WidfireThresholdController: \n\
                        flame_sensor : \n\
                            Min  : {} Max  : {} \n\
                        adc1 : \n\
                            Min  : {} Max  : {} \n\
                        adc2 : \n\
                            Min  : {} Max  : {} \n\
                        CO_AX : \n\
                            Min  : {} Max  : {} \n"
                .format(
                    hex(self.modules.get('flame_sensor').offset.get('min')),    hex(self.modules.get('flame_sensor').offset.get('max')),
                    hex(self.modules.get('adc1').offset.get('min')),            hex(self.modules.get('adc1').offset.get('max')),
                    hex(self.modules.get('adc2').offset.get('min')),            hex(self.modules.get('adc2').offset.get('max')),
                    hex(self.modules.get('CO_AX').offset.get('min')),           hex(self.modules.get('CO_AX').offset.get('max')),
                )
            )

    '''
        Method get_module_value(self, bridge, module_key, min_max_key=None)
        Arguments:
            bridge              : data bridge can be accessed
            module_key          : module name keyword to search in modules
            min_max_key         : min/max keyword to search in a module
        Return Value:
            False : Fails due to some reasons
            or 
            <Value>(float/int): Read Value(real is True) or Set Value(real is False) in the specific module
    '''
    def get_module_value(self, bridge, module_key, min_max_key):
        module = self.modules.get(module_key)
        if (module is not None) :
            return module.get_value(bridge, min_max_key)
        else : 
            return False

    '''
        Method generate_module_dummy_value(self, bridge, module_key)
        Arguments:
            module_key          : module name keyword to search in modules
        Return Value:
            False : Fails due to some reasons
            or 
            <Value>(float/int): Random value between minimum value and maximum
    '''
    def generate_module_dummy_value(self, module_key):
        module = self.modules.get(module_key)
        if (module is not None) :
            return module.generate_dummy_value()
        return False

    '''
        Method update_component_property(self, bridge, patch)
        Arguments:
            bridge              : data bridge can be accessed
            patch : 
                Component Type  : { 'thresholdProperty': {'__t': 'c', 'lux': {'Min': 10, 'Max': 11}}, '$version': 4}
        Return Value:
            dict : reported patch's contents
    '''
    def update_component_property(self, bridge, patch:dict) -> dict:
        ignore_keys = ["__t", "$version"]
        component_prefix = list(patch.keys())[0] # Expected Value is component name(Default:thresholdProperty)
        values = patch[component_prefix]
        print("Values received are :-")
        print(values)

        version = patch["$version"]

        res_dict = {}
        for prop_name, prop_value in values.items():
            if prop_name in ignore_keys:
                continue
            else:
                module =  self.modules.get(prop_name)
                if not (module is None) :
                    temp_dict = module.update_threshold_from_azure(bridge, version, prop_value)
                    res_dict.update(temp_dict)

        properties_dict = dict()
        if component_prefix:
            properties_dict[component_prefix] = res_dict
            properties_dict[component_prefix]['__t'] = 'c'
        else:
            properties_dict = res_dict
        return properties_dict
