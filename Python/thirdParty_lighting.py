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

class RTTrPL():
	def __init__(self, header):
		self.rttrp_head = header
		self.data = header.data

class Packet():
	
	# Generic packet information, each module contains this data, which
	# is used to help determine how to proceed with data extraction
	def __init__(self, data, intSig, fltSig):
		self.pkType = data[0]
		self.intSig = intSig
		self.fltSig = fltSig
		self.data = data[1:]

class LightingOutputModule(Packet):
	def __init__(self, data, intSig, fltSig):
		super().__init__(data, intSig, fltSig)
		
		if (hex(self.intSig) == "0x4154"):
			(self.size, self.sequenceNumber, self.actionType, self.holdTime, self.numUniverses) = \
					struct.unpack("!HfBfH", self.data[0:13])
		elif (hex(slef.intSig) == "0x5441"):
			(self.size, self.sequenceNumber, self.actionType, self.holdTime, self.numUniverses) = \
					struct.unpack("HfBfH", self.data[0:13])
		
		self.data = self.data[13:]	
		
	def printModule(self):
		print("=============Lighting Output Module==============")
		print("Packet Type		:	", hex(self.pkType))
		print("Packet Size		:	", self.size)
		print("Sequence Number	:	", self.sequenceNumber)
		if (hex(self.actionType) == "0x01"):
			print("Action			:	Snapshot")
		elif (hex(self.actionType) == "0x00"):
			print("Action			:	Update")
		else:
			print "Action 			:	Unknown")
		print("Hold Time		:	", self.holdTime)
		print("Num of Universes	:	", self.numUniverses)	
	
	
#Snapshots contain universe(s)
class UniverseModule(Packet):
	def __init__(self, data, intSig, fltSig):
		super().__init__(data, intSig, fltSig)
	
		if (hex(self.intSig) == "0x4154"):
			(self.size, self.universeID, self.numSpots) = struct.unpack("!HHH", self.data[0:6])
		elif (hex(slef.intSig) == "0x5441"):
			(self.size, self.universeID, self.numSpots) = struct.unpack("HHH", self.data[0:6])
		
		self.data = self.data[6:]
		
	def printModule(self):
		print("=============Universe Module==============")
		print("Packet Type		:	", hex(self.pkType))
		print("Packet Size		:	", self.size)
		print("Universe ID		:	", self.universeID)
		print("Num of Spots		:	", self.numSpots)
		
		
#Universes contain spot(s)	
class SpotModule(Packet):
	def __init__(self, data, intSig, fltSig):
		super().__init__(data, intSig, fltSig)
	
		if (hex(self.intSig) == "0x4154"):
			(self.size, self.spotID, self.spotOffset, self.numChannels) = struct.unpack("!HHHH", self.data[0:8])
		elif (hex(slef.intSig) == "0x5441"):
			(self.size, self.spotID, self.spotOffset, self.numChannels) = struct.unpack("HHHH", self.data[0:8])
		
		self.data = self.data[8:]
		
	def printModule(self):
		print("=============Spot Module==============")
		print("Packet Type		:	", hex(self.pkType))
		print("Packet Size		:	", self.size)
		print("Spot ID			:	", self.spotID)
		print("Num of Channels	:	", self.numChannels)	
		
#Spots contain channel(s)		
class ChannelModule(Packet):
	def __init__(self, data, intSig, fltSig):
		super().__init__(data, intSig, fltSig)
	
		if (hex(self.intSig) == "0x4154"):
			(self.channelOffset, self.xFade, self.channelValue) = struct.unpack("!HHB", self.data[0:6])
		elif (hex(slef.intSig) == "0x5441"):
			(self.channelOffset, self.xFade, self.channelValue) = struct.unpack("HHB", self.data[0:6])
		
		self.data = self.data[6:]
		
	def printModule(self):
		print("=============Channel Module==============")
		print("Packet Type		:	", hex(self.pkType))
		print("CHannel			:	", self.channelOffset)
		print("Value			:	", self.channelValue)
