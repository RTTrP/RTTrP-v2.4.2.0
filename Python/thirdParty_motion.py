# MIT License

#Copyright(c) 2019 CAST Group of Companies Inc

#Permission is hereby granted, free of charge, to any person obtaining a copy
#of this software and associated documentation files(the "Software"), to deal
#in the Software without restriction, including without limitation the rights
#to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
#copies of the Software, and to permit persons to whom the Software is
#furnished to do so, subject to the following conditions :

#The above copyright notice and this permission notice shall be included in all
#copies or substantial portions of the Software.

#THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
#AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
#OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
#SOFTWARE. 

import struct
import RTTrP

class RTTrPM():

	def __init__(self, header):
		""" 	Header:	RTTrP header, with data attached 
				
				Description:

				This function initializes the RTTrPM packet, with the header values from
				the RTTrP module passed in. All other values are extracted byte by byte.
		"""
				
		self.rttrp_head = header
		self.data = header.data
		self.trackable = None #0x01
		self.centroidMod = None #0x02
		self.quatMod = None #0x03
		self.eulerMod = None #0x04
		self.ledMod = [] #0x06
		self.centroidAccVelMod = None #0x20
		self.LEDAccVelMod = [] #0x21

	def printPacket(self):
		self.rttrp_head.printHeader()
		self.trackable.printModule()

		if self.centroidMod:
			self.centroidMod.printModule()

		if self.ledMod:
			for LED in self.ledMod:
				LED.printModule()

		if self.quatMod:
			self.quatMod.printModule()

		if self.eulerMod:
			self.eulerMod.printModule()

		if self.centroidAccVelMod:
			self.centroidAccVelMod.printModule()

		if self.LEDAccVelMod:
			for LED in self.LEDAccVelMod:
				LED.printModule()

class Packet():
	
	# Generic packet information, each module contains this data, which
	# is used to help determine how to proceed with data extraction
	def __init__(self, data, intSig, fltSig):
		self.pkType = data[0]
		self.intSig = intSig
		self.fltSig = fltSig
		self.data = data[1:]

class Trackable(Packet):
	
	def __init__(self, data, intSig, fltSig):

		super().__init__(data, intSig, fltSig)

		# Big Endian
		if (hex(self.intSig) == "0x4154"):
			(self.size, self.nameLen) = struct.unpack("!HB", self.data[0:3])

			numChars = "!"

			for i in range(self.nameLen):
				numChars = numChars + "c"

			(temp) = struct.unpack(numChars, self.data[3:self.nameLen+3])

			self.name = ""

			for i in range(self.nameLen):
				self.name = self.name + (temp[i].decode("utf-8"))

			# The rest of this module depends on the size of the name, so we need to take this
			# into consideration when we extract data from the rest of the packet
			if (self.pkType == 1):
				(self.numMods) = struct.unpack("!B", self.data[self.nameLen+3:self.nameLen+4])[0]
				self.data = self.data[self.nameLen+4:]
			elif (self.pkType == 81):
				(self.timeStamp) = struct.unpack("!I", self.data[self.nameLen+3:self.nameLen+7])[0]
				(self.numMods) = struct.unpack("!B", self.data[self.nameLen+7:self.nameLen+8])[0]
				self.data = self.data[self.nameLen+8:]
		# Little Endian
		elif (hex(self.intSig) == "0x5441"):
			(self.size, self.nameLen) = struct.unpack("HB", self.data[0:3])

			numChars = ""

			for i in range(self.nameLen):
				numChars = numChars + "c"

			(temp) = struct.unpack(numChars, self.data[3:self.nameLen+3])

			self.name = ""

			for i in range(self.nameLen):
				self.name = self.name + (temp[i].decode("utf-8"))

			# The rest of this module depends on the size of the name, so we need to take this
			# into consideration when we extract data from the rest of the packet
			if (self.pkType == 1):
				(self.numMods) = struct.unpack("B", self.data[self.nameLen+3:self.nameLen+4])[0]
				self.data = self.data[self.nameLen+4:]
			elif (self.pkType == 81):
				(self.timeStamp) = struct.unpack("I", self.data[self.nameLen+3:self.nameLen+7])[0]
				(self.numMods) = struct.unpack("B", self.data[self.nameLen+7:self.nameLen+8])[0]
				self.data = self.data[self.nameLen+8:]

	def printModule(self):
		print("=============Trackable Module==============")
		print("Packet Type		:	", hex(self.pkType))
		print("Packet Size		:	", self.size)
		print("Name Length		:	", self.nameLen)
		print("Name			:	", self.name)

		if (hex(self.pkType) == "0x51"):
			print("Timestamp		:	", self.timeStamp)

		print("Num of Mods		:	", self.numMods)

class CentroidMod(Packet):

	def __init__(self, data, intSig, fltSig):
		super().__init__(data, intSig, fltSig)

		if (hex(self.intSig) == "0x4154"):
			(self.size, self.latency) = struct.unpack("!HH", self.data[0:4])
		elif (hex(self.intSig) == "0x5441"):
			(self.size, self.latency) = struct.unpack("HH", self.data[0:4])

		if (hex(self.fltSig) == "0x4334"):
			(self.x, self.y, self.z) = struct.unpack("!ddd", self.data[4:28])
		elif (hex(self.fltSig) == "0x3443"):
			(self.x, self.y, self.z) = struct.unpack("ddd", self.data[4:28])

		self.data = self.data[28:]

	def printModule(self):
		print("==============Centroid Module==============")
		print("Type			:	", self.pkType)
		print("Module Size		:	", self.size)
		print("Latency			:	", self.latency)
		print("X			:	", self.x)
		print("Y			:	", self.y)
		print("Z			:	", self.z)

class LEDModule(Packet):
	
	def __init__(self, data, intSig, fltSig):
		super().__init__(data, intSig, fltSig)

		if (hex(self.intSig) == "0x4154"):
			(self.size, self.latency) = struct.unpack("!HH", self.data[0:4])
		elif (hex(slef.intSig) == "0x5441"):
			(self.size, self.latency) = struct.unpack("HH", self.data[0:4])

		if (hex(self.fltSig) == "0x4334"):
			(self.x, self.y, self.z) = struct.unpack("!ddd", self.data[4:28])
		elif (hex(self.fltSig) == "0x3443"):
			(self.x, self.y, self.z)  = struct.unpack("ddd", self.data[4:28])

		(self.index) = struct.unpack("B", self.data[28:29])[0]
		
		self.data = self.data[29:]

	def printModule(self):
		print("=================LED Module================")
		print("Type			:	", self.pkType)
		print("Module Size		:	", self.size)
		print("Latency			:	", self.latency)
		print("X			:	", self.x)
		print("Y			:	", self.y)
		print("Z			:	", self.z)
		print("Index			:	", self.index)

class QuatModule(Packet):

	def __init__(self, data, intSig, fltSig):
		super().__init__(data, intSig, fltSig)

		if (hex(self.intSig) == "0x4154"):
			(self.size, self.latency) = struct.unpack("!HH", self.data[0:4])
		elif (hex(self.intSig) == "0x5441"):
			(self.size, self.latency) = struct.unpack("HH", self.data[0:4])

		if (hex(self.fltSig) == "0x4334"):
			(self.qx, self.qy, self.qz, self.qw) = struct.unpack("!dddd", self.data[4:36])
		elif (hex(self.fltSig) == "0x3443"):
			(self.qx, self.qy, self.qz, self.qw) = struct.unpack("dddd", self.data[4:36])

		self.data = self.data[36:]
	
	def printModule(self):
		print("=============Quaternion Module=============")
		print("Type			:	", self.pkType)
		print("Module Size		:	", self.size)
		print("Latency			:	", self.latency)
		print("Qx			:	", self.qx)
		print("Qy			:	", self.qy)
		print("Qz			:	", self.qz)
		print("Qw			:	", self.qw)

class EulerModule(Packet):

	def __init__(self, data, intSig, fltSig):
		super().__init__(data, intSig, fltSig)

		if (hex(self.intSig) == "0x4154"):
			(self.size, self.latency, self.order) = struct.unpack("!HHH", self.data[0:6])
		elif (hex(self.intSig) == "0x5441"):
			(self.size, self.latency, self.order) = struct.unpack("HHH", self.data[0:6])

		if (hex(self.fltSig) == "0x4334"):
			(self.R1, self.R2, self.R3) = struct.unpack("!ddd", self.data[6:30])
		elif (hex(self.fltSig) == "0x3443"):
			(self.R1, self.R2, self.R3) = struct.unpack("ddd", self.data[6:30])

		self.data = self.data[30:]

	def printModule(self):
		print("===============Euler Module================")
		print("Type			:	", self.pkType)
		print("Module Size		:	", self.size)
		print("Latency			:	", self.latency)
		print("Order			:	", self.order)
		print("R1			:	", self.R1*57.2957795)
		print("R2			:	", self.R2*57.2957795)
		print("R3			:	", self.R3*57.2957795)

class CentroidAccVelMod(Packet):
	
	def __init__(self, data, intSig, fltSig):
		super().__init__(data, intSig, fltSig)

		if (hex(self.intSig) == "0x4154"):
			(self.size) = struct.unpack("!H", self.data[0:2])[0]
		elif (hex(self.intSig) == "0x5441"):
			(self.size) = struct.unpack("H", self.data[0:2])[0]

		if (hex(self.fltSig) == "0x4334"):
			(self.x, self.y, self.z) = struct.unpack("!ddd", self.data[2:26])
			(self.ax, self.ay, self.az) = struct.unpack("!fff", self.data[26:38])
			(self.vx, self.vy, self.vz) = struct.unpack("!fff", self.data[38:50])
		elif (hex(self.fltSig) == "0x3443"):
			(self.x, self.y, self.z) = struct.unpack("ddd", self.data[2:26])
			(self.ax, self.ay, self.az) = struct.unpack("fff", self.data[26:38])
			(self.vx, self.vy, self.vz) = struct.unpack("fff", self.data[38:50])

		self.data = self.data[50:]

	def printModule(self):
		print("=============Centroid Acc/Vel==============")
		print("Type			:	", self.pkType)
		print("Module Size		:	", self.size)
		print("X			:	", self.x)
		print("Y			:	", self.y)
		print("Z			:	", self.z)
		print("Accel X			:	", self.ax)
		print("Accel Y			:	", self.ay)
		print("Accel Z			:	", self.az)
		print("Vel X			:	", self.vx)
		print("Vel Y			:	", self.vy)
		print("Vel Z			:	", self.vz)

class LEDAccVelMod(Packet):
	
	def __init__(self, data, intSig, fltSig):
		super().__init__(data, intSig, fltSig)

		if (hex(self.intSig) == "0x4154"):
			(self.size) = struct.unpack("!H", self.data[0:2])[0]
		elif (hex(self.intSig) == "0x5441"):
			(self.size) = struct.unpack("H", self.data[0:2])[0]

		if (hex(self.fltSig) == "0x4334"):
			(self.x, self.y, self.z) = struct.unpack("!ddd", self.data[2:26])
			(self.ax, self.ay, self.az) = struct.unpack("!fff", self.data[26:38])
			(self.vx, self.vy, self.vz) = struct.unpack("!fff", self.data[38:50])
		elif (hex(self.fltSig) == "0x3443"):
			(self.x, self.y, self.z) = struct.unpack("ddd", self.data[2:26])
			(self.ax, self.ay, self.az) = struct.unpack("fff", self.data[26:38])
			(self.vx, self.vy, self.vz) = struct.unpack("fff", self.data[38:50])

		(self.index) = struct.unpack("B", self.data[50:51])[0]
		self.data = self.data[51:]

	def printModule(self):
		print("================LED Acc/Vel================")
		print("Type			:	", self.pkType)
		print("Module Size		:	", self.size)
		print("X			:	", self.x)
		print("Y			:	", self.y)
		print("Z			:	", self.z)
		print("Accel X			:	", self.ax)
		print("Accel Y			:	", self.ay)
		print("Accel Z			:	", self.az)
		print("Vel x			:	", self.vx)
		print("Vel y			:	", self.vy)
		print("Vel z			:	", self.vz)
		print("Index			:	", self.index)
