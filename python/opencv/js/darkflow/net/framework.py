from . import yolo
from os import sep



class framework(object):
    # constructor = vanilla.constructor
    # loss = vanilla.train.loss
    
    def __init__(self, meta, FLAGS):
        model = meta['model'].split(sep)[-1]
        model = '.'.join(model.split('.')[:-1])
        meta['name'] = model
        
        self.constructor(meta, FLAGS)

    def is_inp(self, file_name):
        return True


class YOLO(framework):
	constructor = yolo.constructor
	loss = yolo.train.loss
	resize_input = yolo.test.resize_input
	


"""
framework factory
"""

types = {
    '[detection]': YOLO,
    # '[region]': YOLOv2
}

def create_framework(meta, FLAGS):
    net_type = meta['type']
    this = types.get(net_type, framework)
    return this(meta, FLAGS)