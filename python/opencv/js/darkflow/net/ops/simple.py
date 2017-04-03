from .baseop import BaseOp
import tensorflow as tf

class identity(BaseOp):
	def __init__(self, inp):
		self.inp = None
		self.out = inp