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

# Main class for the packet header, to keep things simply we just leave this as a generic class
# we won't derive sub-classes from here.
class RTTrP:

	def __init__(self, data):
			# Unpack the signature fields from the packet, followed by the packet version, these are ALWAYS
			# in network (Big Endian) order.
			(self.intHeader, self.fltHeader) = struct.unpack("!H H", data[0:4])
			(self.version) = struct.unpack("!H", data[4:6])[0] 
			
			# Determine Endianness. In general all x86-64 machines are Little Endian, this gets confusing because
			# Tracking Adapter can send in Big/Little Endian format. This means if values are sent as Big Endian from
			# Tracking Adapter, they are still represented in Little Endian by the OS, so we need to instruct
			# "unpack" to treat the values as Big Endian. For Little Endian, we just read values straight from the
			# packet without worrying about byte order.
			if (hex(self.intHeader) == "0x4154"):
				(self.pID, self.pForm, self.pktSize, self.context, self.numMods) = struct.unpack("!IBHIB", data[6:18])
			elif (hex(self.intHeader) == "0x5441"):
				(self.pID, self.pForm) = struct.unpack("IB", data[6:11])
				(self.pktSize) = struct.unpack("H", data[11:13])[0]
				(self.context, self.numMods) = struct.unpack("IB", data[13:18])

			self.data = data[18:]

	def printHeader(self):
		print("===============RTTrP Header================")
		print("Integer Signature	:	", hex(self.intHeader))
		print("Float Signature		:	", hex(self.fltHeader))
		print("Version			:	", self.version)
		print("Packet ID		:	", self.pID)
		print("Packet Format		:	", self.pForm)
		print("Packet Size		:	", self.pktSize)
		print("Context			:	", hex(self.context))
		print("Number of Modules	:	", self.numMods)

